/* Copyright (C) 2011-2024 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: For non commercial use only.
* Source code: https://github.com/DoubangoTelecom/KycVerifDetection-SDK
* WebSite: https://www.doubango.org/webapps/kyc-documents-verif/
*/
#include <KYC_VERIF-SDK-API-PUBLIC.h>

#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <mutex>
#include <map>
#include <sys/stat.h>
#include <condition_variable>
#if defined(_WIN32)
#include <algorithm> // std::replace
#endif

using namespace KycVerif;

struct FldFile {
	void* compressedDataPtr = nullptr;
	size_t compressedDataSize = 0;
	FILE* filePtr = nullptr;
	virtual ~FldFile() {
		if (compressedDataPtr) free(compressedDataPtr), compressedDataPtr = nullptr;
		if (filePtr) fclose(filePtr), filePtr = nullptr;
	}
	bool isValid() const {
		return compressedDataPtr && compressedDataSize > 0;
	}
};

/*
* Parallel callback function used for notification. Not mandatory.
* More info about parallel delivery: https://www.doubango.org/SDKs/kyc-documents-verif/docs/Parallel_versus_sequential_processing.html
*/
static size_t parallelNotifCount = 0;
static std::condition_variable parallelNotifCondVar;
class MyKycVerifSdkParallelDeliveryCallback : public KycVerifSdkParallelDeliveryCallback {
public:
	MyKycVerifSdkParallelDeliveryCallback(const void* userData) : m_pMyDummyData(userData) {}
	virtual void onNewResult(const KycVerifSdkResult* result) const override {
		// Use m_pMyDummyData here if you want
		KYC_VERIF_SDK_ASSERT(result != nullptr);
		const std::string& json = result->json();
		// Printing to the console could be very slow and delayed -> stop displaying the result as soon as all faces are processed
		KYC_VERIF_SDK_PRINT_INFO("MyKycVerifSdkParallelDeliveryCallback::onNewResult(%d, %s, %zu): %s",
			result->code(),
			result->phrase(),
			++parallelNotifCount,
			!json.empty() ? json.c_str() : "{}"
		);
		parallelNotifCondVar.notify_one();
	}
private:
	const void* m_pMyDummyData;
};

static void printUsage(const std::string& message = "");
static bool parseArgs(int argc, char *argv[], std::map<std::string, std::string >& values);
static bool readFile(const std::string& path, FldFile& file);

// Asset manager used on Android to files in "assets" folder
#if KYC_VERIF_SDK_OS_ANDROID 
#	define ASSET_MGR_PARAM() __sdk_android_assetmgr, 
#else
#	define ASSET_MGR_PARAM() 
#endif /* KYC_VERIF_SDK_OS_ANDROID */

// Configuration for the deep learning engine
static const char* __jsonConfig =
"{"
"\"debug_level\": \"info\","
"\"debug_write_input_image_enabled\": false,"
"\"debug_internal_data_path\": \".\","
""
"\"gpu_ctrl_memory_enabled\": false,"
"\"num_threads\": -1,"
"\"max_latency\": -1,"
"\"max_batchsize\": -1,"
"\"asm_enabled\": true,"
"\"intrin_enabled\": true,"
""
"\"openvino_device\": \"CPU\","
""
"\"graph_type\": \"dense\","
"\"graph_2passes_enabled\": true,"
"\"graph_2ndpass_ocr_threshold\": 0.9,"
"\"graph_2ndpass_umeyama_enabled\": true,"
""
"\"ocr_2passes\": true,"
"\"ocr_2ndpass_threshold\": 0.9,"
"\"ocr_patch_antialias\": true,"
"\"ocr_tunning_apply_patterns\": true,"
"\"ocr_tunning_apply_whitelist\": true,"
"\"ocr_tunning_apply_blacklist\": true,"
""
"\"magsac_sigma\": 4.0,"
"\"magsac_max_iters\": 2000,"
"\"magsac_resampler\": \"uniform\","
""
"\"tps_enabled\": true,"
"\"tps_speed\": 1,"
"\"tps_cost\": \"l2\","
""
"\"stn_enabled\": true,"
""
"\"detect_threshold\": 0.3,"
""
"\"text_segmentation_type\": \"watershed\""
;

int main(int argc, char *argv[])
{
	// local variables
	KycVerifSdkResult result;
	MyKycVerifSdkParallelDeliveryCallback parallelDeliveryCallbackCallback(nullptr);
	std::string assetsFolder, licenseTokenData, licenseTokenFile;
	std::string vinoActivation = "auto";
	bool isParallelDeliveryEnabled = true;
	std::string imagePath;

	// Parsing args
	std::map<std::string, std::string > args;
	if (!parseArgs(argc, argv, args)) {
		printUsage();
		return -1;
	}
	if (args.find("--image") == args.end()) {
		printUsage("--image required");
		return -1;
	}
	if (args.find("--assets") == args.end()) {
		printUsage("--assets required");
		return -1;
	}
	imagePath = args["--image"];

	if (args.find("--parallel") != args.end()) {
		isParallelDeliveryEnabled = (args["--parallel"].compare("true") == 0);
	}
	if (args.find("--assets") != args.end()) {
		assetsFolder = args["--assets"];
#if defined(_WIN32)
		std::replace(assetsFolder.begin(), assetsFolder.end(), '\\', '/');
#endif
	}
	if (args.find("--vino_activation") != args.end()) {
		vinoActivation = args["--vino_activation"];
	}

	if (args.find("--tokenfile") != args.end()) {
		licenseTokenFile = args["--tokenfile"];
#if defined(_WIN32)
		std::replace(licenseTokenFile.begin(), licenseTokenFile.end(), '\\', '/');
#endif
	}
	if (args.find("--tokendata") != args.end()) {
		licenseTokenData = args["--tokendata"];
	}


	// Update JSON config
	std::string jsonConfig = __jsonConfig;
	if (!assetsFolder.empty()) {
		jsonConfig += std::string(",\"assets_folder\": \"") + assetsFolder + std::string("\"");
	}
	if (!vinoActivation.empty()) {
		jsonConfig += std::string(",\"openvino_activation\": \"") + vinoActivation + std::string("\"");
	}
	if (!licenseTokenFile.empty()) {
		jsonConfig += std::string(",\"license_token_file\": \"") + licenseTokenFile + std::string("\"");
	}
	if (!licenseTokenData.empty()) {
		jsonConfig += std::string(",\"license_token_data\": \"") + licenseTokenData + std::string("\"");
	}

	jsonConfig += "}"; // end-of-config

	// Read the file (encoded)
	FldFile file;
	if (!readFile(imagePath, file)) {
		KYC_VERIF_SDK_PRINT_INFO("Can't process %s", imagePath.c_str());
		return -1;
	}
	KYC_VERIF_SDK_ASSERT(file.isValid());

	// Init
	KYC_VERIF_SDK_PRINT_INFO("Starting liveness sample...");
	KYC_VERIF_SDK_ASSERT((result = KycVerifSdkEngine::init(
		ASSET_MGR_PARAM()
		jsonConfig.c_str(),
		isParallelDeliveryEnabled ? &parallelDeliveryCallbackCallback : nullptr
	)).isOK());

	// Processing
	// Please note that the first inference is very slow because we'll load the models and
	// perform some initialization.
	KYC_VERIF_SDK_ASSERT((result = KycVerifSdkEngine::process(
		file.compressedDataPtr,
		file.compressedDataSize
	)).isOK());

	// Printing to the console is very slow and use a low priority thread.
	// Wait until all results are displayed.
	if (isParallelDeliveryEnabled) {
		static std::mutex parallelNotifMutex;
		std::unique_lock<std::mutex > lk(parallelNotifMutex);
		parallelNotifCondVar.wait_for(lk,
			std::chrono::milliseconds(1500), // maximum number of millis to wait for before giving up, must never wait this long
			[] { return (parallelNotifCount == 1); }
		);
	}

	// Print latest result
	const std::string& json_ = result.json();
	if (!json_.empty()) {
		KYC_VERIF_SDK_PRINT_INFO("result: %s", json_.c_str());
	}

	KYC_VERIF_SDK_PRINT_INFO("Press any key to terminate !!");
	getchar();

	// DeInit
	KYC_VERIF_SDK_PRINT_INFO("Ending liveness...");
	KYC_VERIF_SDK_ASSERT((result = KycVerifSdkEngine::deInit()).isOK());

	return 0;
}

static void printUsage(const std::string& message /*= ""*/)
{
	if (!message.empty()) {
		KYC_VERIF_SDK_PRINT_INFO("%s", message.c_str());
	}

	KYC_VERIF_SDK_PRINT_INFO(
		"\n********************************************************************************\n"
		"benchmark\n"
		"\t--image <path-to-image-with-a-face-to-analyse> \n"
		"\t--assets <path-to-assets-folder> \n"
		"\t[--parallel <whether-to-enable-parallel-mode:true / false>] \n"
		"\t[--vino_activation <openvino-activation-mode>] \n"
		"\t[--tokenfile <path-to-license-token-file>] \n"
		"\t[--tokendata <base64-license-token-data>] \n"
		"\n"
		"Options surrounded with [] are optional.\n"
		"\n"
		"--image: Path to an image(JPEG/PNG/BMP) to evaluate. \n\n"
		"--assets: Path to the assets folder containing the configuration files and models.\n\n"
		"--parallel: Whether to enabled the parallel mode. More info about the parallel mode at https ://www.doubango.org/SDKs/kyc-documents-verif/docs/Parallel_versus_sequential_processing.html. Default: true.\n\n"
		"--vino_activation: OpenVINO activation type. Default: \"auto\". \n\n"
		"--tokenfile: Path to the file containing the base64 license token if you have one. If not provided then, the application will act like a trial version. Default: null.\n\n"
		"--tokendata: Base64 license token if you have one. If not provided then, the application will act like a trial version. Default: null.\n\n"
		"********************************************************************************\n"
	);
}

static bool parseArgs(int argc, char *argv[], std::map<std::string, std::string >& values)
{
	KYC_VERIF_SDK_ASSERT(argc > 0 && argv != nullptr);

	values.clear();

	// Make sure the number of arguments is even
	if ((argc - 1) & 1) {
		KYC_VERIF_SDK_PRINT_INFO("Number of args must be even");
		return false;
	}

	// Parsing
	for (int index = 1; index < argc; index += 2) {
		std::string key = argv[index];
		if (key.size() < 2 || key[0] != '-' || key[1] != '-') {
			KYC_VERIF_SDK_PRINT_INFO("Invalid key: %s", key.c_str());
			return false;
		}
		values[key] = argv[index + 1];
	}

	return true;
}

static bool readFile(const std::string& path, FldFile& file)
{
	// Open the file
	if ((file.filePtr = fopen(path.c_str(), "rb")) == nullptr) {
		KYC_VERIF_SDK_PRINT_INFO("Can't open %s", path.c_str());
		return false;
	}

	// Retrieve file size
	struct stat st_;
	if (stat(path.c_str(), &st_) != 0) {
		KYC_VERIF_SDK_PRINT_INFO("File is empty %s", path.c_str());
	}
	file.compressedDataSize = static_cast<size_t>(st_.st_size);

	// Alloc memory and read data
	file.compressedDataPtr = ::malloc(file.compressedDataSize);
	if (!file.compressedDataPtr) {
		KYC_VERIF_SDK_PRINT_INFO("Failed to alloc mem with size = %zu", file.compressedDataSize);
		return false;
	}
	size_t read_;
	if (file.compressedDataSize != (read_ = fread(file.compressedDataPtr, 1, file.compressedDataSize, file.filePtr))) {
		KYC_VERIF_SDK_PRINT_INFO("fread(%s) returned %zu instead of %zu", path.c_str(), read_, file.compressedDataSize);
		return false;
	}

	return file.isValid();
}