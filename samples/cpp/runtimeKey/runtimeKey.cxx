/* Copyright (C) 2011-2024 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: For non commercial use only.
* Source code: https://github.com/DoubangoTelecom/KycVerifDetection-SDK
* WebSite: https://www.doubango.org/webapps/kyc-documents-verif/
*/
#include <KYC_VERIF-SDK-API-PUBLIC.h>

#include <map>
#if defined(_WIN32)
#include <algorithm> // std::replace
#endif

using namespace KycVerif;

static void printUsage(const std::string& message = "");
static bool parseArgs(int argc, char *argv[], std::map<std::string, std::string >& values);

/*
* Entry point
*/
int main(int argc, char *argv[])
{
	// Parsing args
	std::map<std::string, std::string > args;
	if (!parseArgs(argc, argv, args)) {
		printUsage();
		return -1;
	}
	if (args.find("--assets") == args.end()) {
		printUsage("--assets required");
		return -1;
	}
	bool rawInsteadOfJSON = false;
	if (args.find("--json") != args.end()) {
		rawInsteadOfJSON = (args["--json"].compare("true") != 0);
	}

	// Build JSON string
	std::string jsonConfig;
	if (args.find("--assets") != args.end()) {
		std::string assetsFolder = args["--assets"];
#if defined(_WIN32)
		std::replace(assetsFolder.begin(), assetsFolder.end(), '\\', '/');
#endif
		jsonConfig += std::string("\"assets_folder\": \"") + assetsFolder + std::string("\"");
	}
	if (args.find("--type") != args.end()) {
		jsonConfig += (jsonConfig.empty() ? "" : ",")
			+ std::string("\"license_host_type\": \"") + args["--type"] + std::string("\"");
	}
	if (args.find("--usbserial") != args.end()) {
		jsonConfig += (jsonConfig.empty() ? "" : ",")
			+ std::string("\"license_usbserial\": \"") + args["--usbserial"] + std::string("\"");
	}
	jsonConfig = "{" + jsonConfig + "}";

	// Initialize the engine
	KYC_VERIF_SDK_ASSERT(KycVerifSdkEngine::init(jsonConfig.c_str()).isOK());

	// Request runtime license key
	const KycVerifSdkResult result = KycVerifSdkEngine::requestRuntimeLicenseKey(rawInsteadOfJSON);
	if (result.isOK()) {
		KYC_VERIF_SDK_PRINT_INFO("\n\n%s\n\n",
			result.json()
		);
	}
	else {
		KYC_VERIF_SDK_PRINT_ERROR("\n\n*** Failed: code -> %d, phrase -> %s ***\n\n",
			result.code(),
			result.phrase()
		);
	}
	
	KYC_VERIF_SDK_PRINT_INFO("Press any key to terminate !!");
	getchar();

	// DeInitialize the engine
	KYC_VERIF_SDK_ASSERT(KycVerifSdkEngine::deInit().isOK());

	return 0;
}

/*
* Print usage
*/
static void printUsage(const std::string& message /*= ""*/)
{
	if (!message.empty()) {
		KYC_VERIF_SDK_PRINT_ERROR("%s", message.c_str());
	}

	KYC_VERIF_SDK_PRINT_INFO(
		"\n********************************************************************************\n"
		"runtimeKey\n"
		"\t[--json <json-output:bool>] \n"
		"\t--assets <path-to-assets-folder> \n"
		"\t--type <license-type> \n"
		"\n"
		"Options surrounded with [] are optional.\n"
		"\n"
		"--json: Whether to output the runtime license key as JSON string instead of raw string. Default: true.\n"
		"--assets: Path to the assets folder containing the configuration files and models.\n"
		"--type: Defines how the license is attached to the machine/host. Possible values are 'aws-instance', 'aws-byol', 'azure-instance' or 'azure-byol'. Default: null.\n"
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
