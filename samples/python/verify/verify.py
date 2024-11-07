'''
    * Copyright (C) 2011-2024 Doubango Telecom <https://www.doubango.org>
    * File author: Mamadou DIOP (Doubango Telecom, France).
    * License: For non commercial use only.
    * Source code: https://github.com/DoubangoTelecom/KYC-Documents-Verif-SDK
    * WebSite: https://www.doubango.org/webapps/kyc-documents-verif


    https://github.com/DoubangoTelecom/KYC-Documents-Verif-SDK/blob/master/samples/python/verify/README.md
	Usage: 
		verify.py \
			--image <path-to-image-to-process> \
			--assets <path-to-assets-folder>\
            [--tokenfile <path-to-license-token-file>] \
			[--tokendata <base64-license-token-data>]
	Example:
		python ../../../samples/python/verify/verify.py --image "../../../assets/images/United States - California Driving License (2017).jpg" --assets ../../../assets
'''

import KYCDocumentsVerifSDK as KycVerifSdk
import os, argparse, json

# Defines the default JSON configuration. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html
JSON_CONFIG = {
    "debug_level": "info",
    "debug_write_input_image_enabled": False,
    "debug_internal_data_path": ".",
    
    "gpu_ctrl_memory_enabled": False,
    "num_threads": -1,
    "max_latency": -1,
    "max_batchsize": -1,
    "asm_enabled": True,
    "intrin_enabled": True,
    
    "openvino_activation": "auto",
    "openvino_device": "CPU",
    
    "graph_type": "dense",
    "graph_2passes_enabled": True,
    "graph_2ndpass_ocr_threshold": 0.9,
    "graph_2ndpass_umeyama_enabled": True,
    
    "ocr_2passes": True,
    "ocr_2ndpass_threshold": 0.9,
    "ocr_patch_antialias": True,
    "ocr_tunning_apply_patterns": True,
    "ocr_tunning_apply_whitelist": True,
    "ocr_tunning_apply_blacklist": True,
    
    "magsac_sigma": 2.0,
    "magsac_max_iters": 5000,
    "magsac_resampler": "prosac",
    
    "tps_enabled": True,
    "tps_speed": 1,
    "tps_cost": "l2",
    
    "stn_enabled": True,
    
    "detect_threshold": 0.3,
    
    "text_segmentation_type": "watershed"
}

TAG = "[KycVerifySdk] "

# Check result
def checkResult(operation, result):
    if not result.isOK():
        print(TAG + operation + ": failed -> " + result.phrase())
        assert False
    else:
        print(TAG + operation + ": OK -> " + result.json())

# Entry point
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="""
    This is the recognizer sample using python language
    """)

    parser.add_argument("--image", required=True, help="Path to the image to process")
    parser.add_argument("--assets", required=False, default="../../../assets", help="Path to the assets folder")
    parser.add_argument("--tokenfile", required=False, default="", help="Path to license token file")
    parser.add_argument("--tokendata", required=False, default="", help="Base64 license token data")

    args = parser.parse_args()

    # Check if image exist
    if not os.path.isfile(args.image):
        print(TAG + "File doesn't exist: %s" % args.image)
        assert False

    # Update JSON options using values from the command args
    JSON_CONFIG["assets_folder"] = args.assets
    JSON_CONFIG["license_token_file"] = args.tokenfile
    JSON_CONFIG["license_token_data"] = args.tokendata

    # Initialize the engine
    checkResult("Init", 
                KycVerifSdk.KycVerifSdkEngine_init(json.dumps(JSON_CONFIG))
               )

    # Process
    # Please note that the first time you call this function all deep learning models will be loaded 
    # and initialized which means it will be slow. In your application you've to initialize the engine
    # once and do all the processing you need, then deinitialize it.
    with open(args.image, 'rb') as file:
        checkResult("Process",
                    KycVerifSdk.KycVerifSdkEngine_process(
                        file.read(),
                        os.fstat(file.fileno()).st_size
                        )
            )

    # Press any key to exit
    input("\nPress Enter to exit...\n") 

    # DeInit the engine
    checkResult("DeInit", 
                KycVerifSdk.KycVerifSdkEngine_deInit()
               )
    
    