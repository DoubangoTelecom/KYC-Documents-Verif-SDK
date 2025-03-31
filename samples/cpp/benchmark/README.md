- [Dependencies](#dependencies)
  - [Debugging missing dependencies](#dependencies-debugging)
- [GPGPU acceleration](#gpu-acceleration)
- [Peformance numbers](#peformance-numbers)
- [Pre-built binaries](#prebuilt)
- [Building](#building)
- [Testing](#testing)
  - [Usage](#testing-usage)
  - [Examples](#testing-examples)


This application is used to check everything is ok and running as fast as expected, check the [C++ API](https://www.doubango.org/SDKs/kyc-documents-verif/docs/cpp-api.html) for more information.
It's open source and doesn't require registration or license key.

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


<a name="peformance-numbers"></a>
# Peformance numbers #

These performance numbers are obtained using **version 0.0.10**. You can use any later version.


We ran the benchmark application for **#20 times loops** [United States - California Driving License (2017).jpg](../../../assets/images/United%20States%20-%20California%20Driving%20License%20%282017%29.jpg) file.

The first number **(1476 millis, 13.54fps)** means:
  - it takes 1476 milliseconds to process #20 Californian driver licenses
  - it takes 73.8 milliseconds to process #1 Californian driver license (73.8 = 1476/20)
  - we can process 13.54 Californian driver licenses per second

| | [Inter Parallel processing enabled](https://www.doubango.org/SDKs/kyc-documents-verif/docs/Parallel_processing.html#inter-processing) | [Inter Parallel processing disabled](https://www.doubango.org/SDKs/kyc-documents-verif/docs/Parallel_processing.html#inter-processing) |
| --- | --- | --- |
| AMD Ryzen 9 7950X<br/>RTX 4060<br/>Ubuntu 24 | 1476 millis<br/>13.54 fps | 2793 millis<br/>7.15 fps |
| AMD Ryzen 7 3700X 8<br/>RTX 3060<br/>Ubuntu 20 | 2669 millis<br/>7.49 fps | 5097 millis<br/>3.92 fps |
| Intel(R) Xeon(R) E3-1230 v6 @ 3.50GHz<br/>GTX 1070<br/>Ubuntu 18 | 4073 millis<br/>4.91 fps | 6779 millis<br/>2.95 fps |
| AMD Ryzen 9 7950X<br/>No GPU<br/>Ubuntu  24| 3600 millis<br/>5.55 fps | 5358 millis<br/>3.73 fps |
| Intel(R) i7-4790Kz<br/>No GPU<br/>Windows 8 Pro | 24587 millis<br/>0.81 fps | 24776 millis<br/>0.80 fps |

Some important notes:
 - All tests are done with OpenVINO activation mode set to "auto". You'll have very poor performance numbers if you disable OpenVINO without having a GPU. Another reason th use OpenVINO instead of Tensorflow is that the former consumes far less memory.
 - Set OpenVINO activation mode to "on" instead of "auto" if you have a GPU but don't want to use it. "on" will force all inference to be done on OpenVINO device (default = "CPU").
 - Support for CUDA is checked at runtime, check the logs to make sure evrything is ok.
 - Inter parallel processing mode is faster than sequential mode only when you have a GPU or NPU. More at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Parallel_processing.html#inter-processing

<a name="prebuilt"></a>
# Pre-built binaries #

If you don't want to build this sample by yourself, then use the pre-built versions:
 - Windows x86_64: [benchmark.exe](../../../binaries/windows/x86_64/benchmark.exe) under [binaries/windows/x86_64](../../../binaries/windows/x86_64)
 - Linux x86_64: [benchmark](../../../binaries/linux/x86_64/benchmark) under [binaries/linux/x86_64](../../../binaries/linux/x86_64). Built on Ubuntu 18. **You'll need to download libtensorflow.so as explained [here](../../../GPGPU.md#gpu-acceleration-tensorflow-linux)**.

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

<a name="testing-usage"></a>

The test image looks like this:

![Test image](../../../assets/images/United%20States%20-%20California%20Driving%20License%20%282017%29.jpg)

## Usage ##

Benchmark is a command line application with the following usage:
```
benchmark \
      --image <path-to-image-to-process> \
      --assets <path-to-assets-folder> \
      [--loops <number-of-loops>] \
      [--vino_activation <openvino-activation-mode:auto/on/off>] \
      [--parallel <whether-to-enable-inter-parallel-mode:true/false>] \
      [--gpu_ctrl_mem <whether-to-enable-gpu-memory-ctrl:true/false>] \
      [--tokenfile <path-to-license-token-file>] \
      [--tokendata <base64-license-token-data>]
```
Options surrounded with **[]** are optional.
- `--image` Path to the image (JPEG/PNG/BMP/...) to process. 
- `--assets` Path to the [assets](../../../assets) folder containing the configuration files and models. More at information https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#assets-folder.
- `--loops` Number of times to run the processing function. Default: *20*.
- `--vino_activation` OpenVINO activation type. Default: *"auto"*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#openvino-activation.
- `--parallel` Whether to enabled the inter parallel processing mode. Default: *true*. More info at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Parallel_processing.html#inter-processing.
- `--gpu_ctrl_mem` Whether to enabled the GPU memory controller. Default: *false*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#gpu-ctrl-memory-enabled
- `--tokenfile` Path to the file containing the base64 license token if you have one. If not provided, then the application will act like a trial version. Default: *null*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-file.
- `--tokendata` Base64 license token if you have one. If not provided, then the application will act like a trial version. Default: *null*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-data.

<a name="testing-examples"></a>
## Examples ##


- On **Linux x86_64**, you may use the next command:
```
LD_LIBRARY_PATH=../../../binaries/linux/x86_64:$LD_LIBRARY_PATH ./benchmark \
    --image "../../../assets/images/United States - California Driving License (2017).jpg" \
    --assets ../../../assets \
    --loops 20 \
    --vino_activation "auto" \
    --gpu_ctrl_mem false \
    --parallel true
```
Very important: you'll need to download Tensorflow libraries as explained [here](../../../GPGPU.md#gpu-acceleration-tensorflow-linux).

you can also use [binaries/linux/x86_64/benchmark.sh](../../../binaries/linux/x86_64/benchmark.sh) to make your life easier.

- On **Windows x86_64**, you may use the next command:
```
benchmark.exe ^
    --image "../../../assets/images/United States - California Driving License (2017).jpg" ^
    --assets ../../../assets ^
    --loops 20 ^
    --vino_activation "auto" ^
    --gpu_ctrl_mem false ^
    --parallel true
```
you can also use [binaries/windows/x86_64/benchmark.bat](../../../binaries/windows/x86_64/benchmark.bat) to make your life easier.

