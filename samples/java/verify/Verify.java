/* Copyright (C) 2011-2024 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: For non commercial use only.
* Source code: https://github.com/DoubangoTelecom/KYC-Documents-Verif-SDK
* WebSite: https://www.doubango.org/webapps/kyc-documents-verif/
*/

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.Hashtable;
import java.util.IllegalFormatException;
import java.util.List;
import java.util.Arrays;
import java.util.stream.Collectors;
import java.lang.IllegalArgumentException;

import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

import org.doubango.KycVerif.Sdk.KYC_VERIF_SDK_IMAGE_TYPE;
import org.doubango.KycVerif.Sdk.KycVerifSdkEngine;
import org.doubango.KycVerif.Sdk.KycVerifSdkResult;

public class Verify {

   public static void main(String[] args) throws IllegalArgumentException, FileNotFoundException, IOException {
      // Parse arguments
      final Hashtable<String, String> parameters = ParseArgs(args);

      // Make sur the image is provided using args
      if (!parameters.containsKey("--image"))
      {
         System.err.println("--image required");
         throw new IllegalArgumentException("--image required");
      }
      // Extract assets folder
      // https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#assets-folder
      String assetsFolder = parameters.containsKey("--assets")
          ? parameters.get("--assets") : "";

      // License data - Optional
      // https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-data
      String tokenDataBase64 = parameters.containsKey("--tokendata")
          ? parameters.get("--tokendata") : "";

      // Load the native library
      System.loadLibrary("KYCDocumentsVerifSDK");

      // Initialize the engine
      KycVerifSdkResult result = CheckResult("Init", KycVerifSdkEngine.init(BuildJSON(assetsFolder, tokenDataBase64)));

      // Decode the JPEG/PNG/BMP file
      final File file = new File(parameters.get("--image"));
      if (!file.exists())
      {
          throw new FileNotFoundException("File not found: " + file.getAbsolutePath());
      }

      // Reading The data
      ByteBuffer nativeBuffer = null;
      try(RandomAccessFile inFile = new RandomAccessFile(file, "r")) {
         final FileChannel inChannel = inFile.getChannel();
         nativeBuffer = ByteBuffer.allocateDirect((int)inChannel.size());
         inChannel.read(nativeBuffer);
         nativeBuffer.flip();         
      } catch (IOException e) {
         e.printStackTrace();
         throw e;
      }
      
      // Processing
      // First inference is expected to be slow because models will be loaded and initialized.
      // Please do not report about inference being slow.
      result = CheckResult("Process", KycVerifSdkEngine.process(
            nativeBuffer,
            nativeBuffer.remaining()
         ));
      // Print result to console
      System.out.println("Result: " + result.json() + System.lineSeparator());

      // Wait until user press a key
      System.out.println("Press any key to terminate !!" + System.lineSeparator());
      final java.util.Scanner scanner = new java.util.Scanner(System.in);
      if (scanner != null) {
         scanner.nextLine();
         scanner.close();
      }

      // Now that you're done, deInit the engine before exiting
      CheckResult("DeInit", KycVerifSdkEngine.deInit());
   }

   static Hashtable<String, String> ParseArgs(String[] args) throws IllegalArgumentException
   {
      System.out.println("Args: " + String.join(" ", args) + System.lineSeparator());

      if ((args.length & 1) != 0)
      {
            String errMessage = String.format("Number of args must be even: %d", args.length);
            System.err.println(errMessage);
            throw new IllegalArgumentException(errMessage);
      }

      // Parsing
      Hashtable<String, String> values = new Hashtable<String, String>();
      for (int index = 0; index < args.length; index += 2)
      {
            String key = args[index];
            if (!key.startsWith("--"))
            {
               String errMessage = String.format("Invalid key: %s", key);
               System.err.println(errMessage);
               throw new IllegalArgumentException(errMessage);
            }
            values.put(key, args[index + 1].replace("$(ProjectDir)", System.getProperty("user.dir").trim()));
      }
      return values;
   }

   static KycVerifSdkResult CheckResult(String functionName, KycVerifSdkResult result) throws IOException
   {
      if (!result.isOK())
      {
            String errMessage = String.format("%s: Execution failed: %s", functionName, result.json());
            System.err.println(errMessage);
            throw new IOException(errMessage);
      }
      return result;
   }

   // https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html
   static String BuildJSON(String assetsFolder, String tokenDataBase64)
   {
      return String.format(
         "{" +
         "\"debug_level\": \"info\"," +
         "\"debug_write_input_image_enabled\": false," +
         "\"debug_internal_data_path\": \".\"," +
         "" +
         "\"gpu_ctrl_memory_enabled\": true," +
         "\"num_threads\": -1," +
         "\"max_latency\": -1," +
         "\"max_batchsize\": -1," +
         "\"asm_enabled\": true," +
         "\"intrin_enabled\": true," +
         "" +
         "\"openvino_activation\": \"auto\"," +
         "\"openvino_device\": \"CPU\"," +
         "" +
         "\"graph_type\": \"dense\"," +
         "\"graph_2passes_enabled\": true," +
         "\"graph_2ndpass_ocr_threshold\": 0.9," +
         "\"graph_2ndpass_umeyama_enabled\": true," +
         "" +
         "\"ocr_2passes\": true," +
         "\"ocr_2ndpass_threshold\": 0.9," +
         "\"ocr_patch_antialias\": true," +
         "\"ocr_tunning_apply_patterns\": true," +
         "\"ocr_tunning_apply_whitelist\": true," +
         "\"ocr_tunning_apply_blacklist\": true," +
         "" +
         "\"magsac_sigma\": 1.0," +
         "\"magsac_max_iters\": 5000," +
         "\"magsac_resampler\": \"uniform\"," +
         "" +
         "\"tps_enabled\": true," +
         "\"tps_speed\": 1," +
         "\"tps_cost\": \"l2\"," +
         "" +
         "\"stn_enabled\": true," +
         "" +
         "\"detect_threshold\": 0.3," +
         "" +
         "\"text_segmentation_type\": \"watershed\"," +
         "" +
         "\"assets_folder\": \"%s\"," +
         "\"license_token_data\": \"%s\"" +
         "}"
         , 
         // Value added using command line args
         assetsFolder,
         tokenDataBase64
      );
   }
}