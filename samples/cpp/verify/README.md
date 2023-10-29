- [Dependencies](#dependencies)
  - [Debugging missing dependencies](#dependencies-debugging)
- [GPGPU acceleration](#gpu-acceleration)
- [Pre-built binaries](#prebuilt)
- [Building](#building)
- [Testing](#testing)
  - [Usage](#testing-usage)
  - [Examples](#testing-examples)


This application is used as reference code for developers to show how to use the [C++ API](https://www.doubango.org/SDKs/kyc-documents-verif/docs/cpp-api.html) and could
be used to easily to check the accuracy. The application accepts path to a JPEG/PNG/BMP/... file as input.

If you don't want to build this sample and is looking for a quick way to check the accuracy then try the online web demo hosted at [https://www.doubango.org/webapps/kyc-documents-verif](https://www.doubango.org/webapps/kyc-documents-verif).

This sample is open source and doesn't require registration or license key.

<a name="dependencies"></a>
# Dependencies #
**The SDK is developed in C++11** and you'll need **glibc 2.27+** on *Linux* and **[Microsoft Visual C++ 2015 Redistributable(x64) - 14.0.24123](https://www.microsoft.com/en-us/download/details.aspx?id=52685)** (any later version is ok) on *Windows*.  **You most likely already have these dependencies on you machine** as almost every program require it.

If you're planning to use [OpenVINO](https://docs.openvinotoolkit.org/), then you'll need [Intel C++ Compiler Redistributable](https://software.intel.com/en-us/articles/intel-compilers-redistributable-libraries-by-version) (choose newest). Please note that OpenVINO is packaged in the SDK as plugin and loaded (`dlopen`) at runtime. The engine will fail to load the plugin if [Intel C++ Compiler Redistributable](https://software.intel.com/en-us/articles/intel-compilers-redistributable-libraries-by-version) is missing on your machine **but the program will work as expected** with Tensorflow as fallback. We highly recommend using [OpenVINO](https://docs.openvinotoolkit.org/) to speedup the inference time and reduce memory usage.

<a name="dependencies-debugging"></a>
## Debugging missing dependencies ##
To check if all dependencies are present:
- **Windows x86_64:** Use [Dependency Walker](https://www.dependencywalker.com/) on [binaries/windows/x86_64/KYCDocumentsVerifSDK.dll](../../../binaries/windows/x86_64/KYCDocumentsVerifSDK.dll) and [binaries/windows/x86_64/plugin_vino.dll](../../../binaries/windows/x86_64/plugin_vino.dll) if you're planning to use [OpenVINO](https://docs.openvinotoolkit.org/).
- **Linux x86_64:** Use `ldd <your-shared-lib>` on [binaries/linux/x86_64/libKYCDocumentsVerifSDK.so](../../../binaries/linux/x86_64/libKYCDocumentsVerifSDK.so) and [binaries/linux/x86_64/libplugin_vino.so](../../../binaries/linux/x86_64/libplugin_vino.so) if you're planning to use [OpenVINO](https://docs.openvinotoolkit.org/).


<a name="gpu-acceleration"></a>
# GPGPU acceleration #
- On x86-64, GPGPU acceleration is disabled by default. Check [here](../../../GPGPU.md#gpu-acceleration) for more information on how to enable it.

<a name="prebuilt"></a>
# Pre-built binaries #

If you don't want to build this sample by yourself, then use the pre-built C++ versions:
 - Windows x86_64: [verify.exe](../../../binaries/windows/x86_64/verify.exe) under [binaries/windows/x86_64](../../../binaries/windows/x86_64)
 - Linux x86_64: [verify](../../../binaries/linux/x86_64/verify) under [binaries/linux/x86_64](../../../binaries/linux/x86_64). Built on Ubuntu 18. **You'll need to download libtensorflow.so as explained [here](../../../GPGPU.md#gpu-acceleration-tensorflow-linux)**.

<a name="building"></a>
# Building #

You'll need [CMake](https://cmake.org/) to build this sample.

- Create build folder and move into it: `mkdir build && cd build`

To generate the build files:
- Windows (Visual Studio files): `cmake .. -DCMAKE_BUILD_TYPE=Release`
- Linux (Makefile): `cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release`

To build the project:
- Windows: Open the VS solution and build the projet
- Linux: Run `make` to build the project 

<a name="testing"></a>
# Testing #
After [building](#building) the application you can test it on your local machine.

The test image looks like this:

![Test image](../../../assets/images/United%20States%20-%20California%20Driving%20License%20%282017%29.jpg)

<a name="testing-usage"></a>
## Usage ##

verify is a command line application with the following usage:
```
verify \
      --image <path-to-image-to-process> \
      --assets <path-to-assets-folder> \
      [--vino_activation <openvino-activation-mode:auto/on/off>] \
      [--parallel <whether-to-enable-inter-parallel-mode:true/false>] \
      [--tokenfile <path-to-license-token-file>] \
      [--tokendata <base64-license-token-data>]
```
Options surrounded with **[]** are optional.
- `--image` Path to the image (JPEG/PNG/BMP/...) to process. 
- `--assets` Path to the [assets](../../../assets) folder containing the configuration files and models. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#assets-folder.
- `--vino_activation` OpenVINO activation type. Default: *"auto"*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#openvino-activation.
- `--parallel` Whether to enabled the inter parallel processing mode. Default: *false*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Parallel_processing.html#inter-processing.
- `--tokenfile` Path to the file containing the base64 license token if you have one. If not provided, then the application will act like a trial version. Default: *null*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-file.
- `--tokendata` Base64 license token if you have one. If not provided, then the application will act like a trial version. Default: *null*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-data.

<a name="testing-examples"></a>
## Examples ##

- On **Linux x86_64**, you may use the next command:
```
LD_LIBRARY_PATH=../../../binaries/linux/x86_64:$LD_LIBRARY_PATH ./verify \
    --image "../../../assets/images/United States - California Driving License (2017).jpg" \
    --assets ../../../assets \
    --vino_activation "auto" \
    --parallel false
```
Very important: you'll need to download Tensorflow libraries as explained [here](../../../GPGPU.md#gpu-acceleration-tensorflow-linux).

you can also use [binaries/linux/x86_64/verify.sh](../../../binaries/linux/x86_64/verify.sh) to make your life easier.

- On **Windows x86_64**, you may use the next command:
```
verify.exe ^
    --image "../../../assets/images/United States - California Driving License (2017).jpg" ^
    --assets ../../../assets ^
    --vino_activation "auto" ^
    --parallel false
```
you can also use [binaries/windows/x86_64/verify.bat](../../../binaries/windows/x86_64/verify.bat) to make your life easier.

