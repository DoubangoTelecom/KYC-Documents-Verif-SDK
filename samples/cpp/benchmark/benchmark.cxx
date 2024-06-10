/* Copyright (C) 2011-2024 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: For non commercial use only.
* Source code: https://github.com/DoubangoTelecom/KycVerifDetection-SDK
* WebSite: https://www.doubango.org/webapps/kyc-documents-verif/
*/
#include <KYC_VERIF-SDK-API-PUBLIC.h>

#include "kyc_image_utils.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <mutex>
#include <condition_variable>
#if defined(_WIN32)
#include <algorithm> // std::replace
#endif

using namespace KycVerif;

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
"\"magsac_sigma\": 1.5,"
"\"magsac_max_iters\": 5000,"
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

int main(int argc, char *argv[])
{
	// local variables
	KycVerifSdkResult result;
	MyKycVerifSdkParallelDeliveryCallback parallelDeliveryCallbackCallback(nullptr);
	std::string assetsFolder, licenseTokenData, licenseTokenFile;
	std::string vinoActivation = "auto";
	bool isParallelDeliveryEnabled = true;
	bool isGpuCtrlMemoryEnabled = true;
	size_t loopCount = 20;
	std::string imagePath;

	// Parsing args
	std::map<std::string, std::string > args;
	if (!kycParseArgs(argc, argv, args)) {
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
	
	if (args.find("--loops") != args.end()) {
		const int loops = std::atoi(args["--loops"].c_str());
		if (loops < 1) {
			printUsage("--loops must be within [1, inf]");
			return -1;
		}
		loopCount = static_cast<size_t>(loops);
	}
	if (args.find("--parallel") != args.end()) {
		isParallelDeliveryEnabled = (args["--parallel"].compare("true") == 0);
	}
	if (args.find("--gpu_ctrl_mem") != args.end()) {
		isGpuCtrlMemoryEnabled = (args["--gpu_ctrl_mem"].compare("true") == 0);
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
	jsonConfig += std::string(",\"gpu_ctrl_memory_enabled\": ") + (isGpuCtrlMemoryEnabled ? "true" : "false") + std::string("");

	jsonConfig += "}"; // end-of-config

	// Decode image
	FldFile kycFile;
	if (!kycDecodeFile(imagePath, kycFile)) {
		KYC_VERIF_SDK_PRINT_INFO("Failed to read image file: %s", imagePath.c_str());
		return -1;
	}

	// Function to wait until parallel callback is called
	auto funcWaitPtr = [&](const size_t& count) -> void {
		if (isParallelDeliveryEnabled) {
			static std::mutex parallelNotifMutex;
			std::unique_lock<std::mutex > lk(parallelNotifMutex);
			parallelNotifCondVar.wait_for(lk,
				std::chrono::milliseconds(15 * 1000), // maximum number of millis to wait for before giving up, must never wait this long
				[&count] { return (parallelNotifCount == count); }
			);
			parallelNotifCount = 0;
		}
	};

	// Init
	KYC_VERIF_SDK_PRINT_INFO("Starting benchmark...");
	KYC_VERIF_SDK_ASSERT((result = KycVerifSdkEngine::init(
		ASSET_MGR_PARAM()
		jsonConfig.c_str(),
		isParallelDeliveryEnabled ? &parallelDeliveryCallbackCallback : nullptr
	)).isOK());

	// WarmUp
	// We load the models into the memory the first time the inference is called which
	// means it'll be very slow.
	KYC_VERIF_SDK_PRINT_INFO("Starting warmup...");
	KYC_VERIF_SDK_ASSERT((result = KycVerifSdkEngine::process(
		kycFile.type,
		kycFile.uncompressedData,
		kycFile.width,
		kycFile.height
	)).isOK());
	funcWaitPtr(1);
	KYC_VERIF_SDK_PRINT_INFO("Warmup done.");

	// Processing
	KYC_VERIF_SDK_PRINT_INFO("Starting processing...");
	const std::chrono::high_resolution_clock::time_point timeStart = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < loopCount; ++i) {
		KYC_VERIF_SDK_ASSERT((result = KycVerifSdkEngine::process(
			kycFile.type,
			kycFile.uncompressedData,
			kycFile.width,
			kycFile.height
		)).isOK());
	}
	funcWaitPtr(loopCount);

	// Compute the estimated frame rate.
	// At this step all frames are already processed but the result could be still on the delivery
	// queue due to the console display latency. You can move here the code used to wait until all
	// messages are displayed to include the delivery latency.
	const std::chrono::high_resolution_clock::time_point timeEnd = std::chrono::high_resolution_clock::now();
	const double elapsedTimeInMillis = std::chrono::duration_cast<std::chrono::duration<double >>(timeEnd - timeStart).count() * 1000.0;
	KYC_VERIF_SDK_PRINT_INFO("Elapsed time (KYC) = [[[ %lf millis ]]]", elapsedTimeInMillis);

	// Print latest result
	const std::string& json_ = result.json();
	if (!json_.empty()) {
		KYC_VERIF_SDK_PRINT_INFO("result: %s", json_.c_str());
	}

	// Print estimated frame rate
	const double estimatedFps = 1000.f / (elapsedTimeInMillis / (double)loopCount);
	KYC_VERIF_SDK_PRINT_INFO("*** elapsedTimeInMillis: %lf, estimatedFps: %lf ***", elapsedTimeInMillis, estimatedFps);

	KYC_VERIF_SDK_PRINT_INFO("Press any key to terminate !!");
	getchar();

	// DeInit
	KYC_VERIF_SDK_PRINT_INFO("Ending benchmark...");
	KYC_VERIF_SDK_ASSERT((result = KycVerifSdkEngine::deInit()).isOK());

	return 0;
}

static void printUsage(const std::string& message /*= ""*/)
{
	if (!message.empty()) {
		KYC_VERIF_SDK_PRINT_ERROR("%s", message.c_str());
	}

	KYC_VERIF_SDK_PRINT_INFO(
		"\n********************************************************************************\n"
		"benchmark\n"
		"\t--image <path-to-image-with-a-face-to-analyse> \n"
		"\t--assets <path-to-assets-folder> \n"
		"\t[--loops <number-of-loops>] \n"
		"\t[--vino_activation <openvino-activation-mode>] \n"
		"\t[--parallel <whether-to-enable-inter-parallel-mode:true/false>] \n"
		"\t[--gpu_ctrl_mem <whether-to-enable-gpu-memory-ctrl:true/false>] \n"
		"\t[--tokenfile <path-to-license-token-file>] \n"
		"\t[--tokendata <base64-license-token-data>] \n"
		"\n"
		"Options surrounded with [] are optional.\n"
		"\n"
		"--image: Path to an image(JPEG/PNG/BMP) with a license face. This image will be used to evaluate the KYC verification processor. \n\n"
		"--assets: Path to the assets folder containing the configuration files and models.\n\n"
		"--loops: Number of times to run the processing function. Default: 20. \n\n"
		"--vino_activation: OpenVINO activation type. Default: \"auto\". \n\n"
		"--parallel: Whether to enabled the parallel mode. Default: true.\n\n"
		"--gpu_ctrl_mem: Whether to enabled the GPU memory control. Default: true.\n\n"
		"--tokenfile: Path to the file containing the base64 license token if you have one. If not provided then, the application will act like a trial version. Default: null.\n\n"
		"--tokendata: Base64 license token if you have one. If not provided then, the application will act like a trial version. Default: null.\n\n"
		"********************************************************************************\n"
	);
}