/* Copyright (C) 2011-2024 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: For non commercial use only.
* Source code: https://github.com/DoubangoTelecom/KYC-Documents-Verif-SDK
* WebSite: https://www.doubango.org/webapps/kyc-documents-verif/
*/
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Web.Script.Serialization;
using System.Runtime.InteropServices;
using org.doubango.KycVerif.Sdk;

namespace liveness
{
    class Program
    {
        static void Main(String[] args)
        {
            // Parse arguments
            IDictionary<String, String> parameters = ParseArgs(args);

            // Make sur the image is provided using args
            if (!parameters.ContainsKey("--image"))
            {
                Console.Error.WriteLine("--image required");
                throw new Exception("--image required");
            }
            // Make sure the image exists
            String file = parameters["--image"];
            if (!System.IO.File.Exists(file))
            {
                throw new System.IO.FileNotFoundException("File not found:" + file);
            }

            // Extract assets folder
            // https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#assets-folder
            String assetsFolder = parameters.ContainsKey("--assets")
                ? parameters["--assets"] : String.Empty;

            // License data - Optional
            // https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-data
            String tokenDataBase64 = parameters.ContainsKey("--tokendata")
                ? parameters["--tokendata"] : String.Empty;

            // Initialize the engine: Load deep learning models and init GPU shaders
            // Make sure de disable VS hosting process to see logs from native code: https://social.msdn.microsoft.com/Forums/en-US/5da6cdb2-bc2b-4fff-8adf-752b32143dae/printf-from-dll-in-console-app-in-visual-studio-c-2010-express-does-not-output-to-console-window?forum=Vsexpressvcs
            // This function should be called once.
            KycVerifSdkResult result = CheckResult("Init", KycVerifSdkEngine.init(BuildJSON(assetsFolder, tokenDataBase64)));

            // Read file bytes (encoded JPEG or PNG)
            byte[] bytes = System.IO.File.ReadAllBytes(file);

            // Processing
            // Please note that the first inference is very slow because we'll load the models and
            // perform some initialization.
            GCHandle pinnedBuffer = GCHandle.Alloc(bytes, GCHandleType.Pinned);
            result = CheckResult("Process", KycVerifSdkEngine.process(
                        pinnedBuffer.AddrOfPinnedObject(),
                        (uint)bytes.Length
                ));
            pinnedBuffer.Free();
            // Print result to console
            Console.WriteLine("Result: {0}", result.json());

            // Wait until user press a key
            Console.WriteLine("Press any key to terminate !!");
            Console.Read();

            // Now that you're done, deInit the engine before exiting
            CheckResult("DeInit", KycVerifSdkEngine.deInit());
        }

        static IDictionary<String, String> ParseArgs(String[] args)
        {
            Console.WriteLine("Args: {0}", string.Join(" ", args));

            if ((args.Length & 1) != 0)
            {
                String errMessage = String.Format("Number of args must be even: {0}", args.Length);
                Console.Error.WriteLine(errMessage);
                throw new Exception(errMessage);
            }

            // Parsing
            Dictionary<String, String> values = new Dictionary<String, String>();
            for (int index = 0; index < args.Length; index += 2)
            {
                String key = args[index];
                if (key.Length < 2 || key[0] != '-' || key[1] != '-')
                {
                    String errMessage = String.Format("Invalid key: {0}", key);
                    Console.Error.WriteLine(errMessage);
                    throw new Exception(errMessage);
                }
                values[key] = args[index + 1];
            }
            return values;
        }

        static KycVerifSdkResult CheckResult(String functionName, KycVerifSdkResult result)
        {
            if (!result.isOK())
            {
                String errMessage = String.Format("{0}: Execution failed: {1}", new String[] { functionName, result.json() });
                Console.Error.WriteLine(errMessage);
                throw new Exception(errMessage);
            }
            return result;
        }

        // https://www.doubango.org/SDKs/mrz/docs/Configuration_options.html
        static String BuildJSON(String assetsFolder = "", String tokenDataBase64 = "")
        {
            return new JavaScriptSerializer().Serialize(new
            {
                debug_level = "info",
                debug_write_input_image_enabled = false,
                debug_internal_data_path = ".",

                gpu_ctrl_memory_enabled = false,
                num_threads = -1,
                max_latency = -1,
                max_batchsize = -1,
                asm_enabled = true,
                intrin_enabled = true,

                openvino_activation = "auto",
                openvino_device = "CPU",

                graph_type = "dense",
                graph_2passes_enabled = true,
                graph_2ndpass_ocr_threshold = 0.9,
                graph_2ndpass_umeyama_enabled = true,

                ocr_2passes = true,
                ocr_2ndpass_threshold = 0.9,
                ocr_patch_antialias = true,
                ocr_tunning_apply_patterns = true,
                ocr_tunning_apply_whitelist = true,
                ocr_tunning_apply_blacklist = true,

                magsac_sigma = 4.0,
                magsac_max_iters = 2000,
                magsac_resampler = "uniform",

                tps_enabled = true,
                tps_speed = 1,
                tps_cost = "l2",

                stn_enabled = true,

                detect_threshold = 0.3,

                text_segmentation_type = "watershed",

                // Value added using command line args
                assets_folder = assetsFolder,
                license_token_data = tokenDataBase64,
            });
        }
    }
}
