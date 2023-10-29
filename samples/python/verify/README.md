- [Dependencies](#dependencies)
  - [Debugging missing dependencies](#dependencies-debugging)
- [GPGPU acceleration](#gpu-acceleration)
- [Prerequisite](#prerequisite)
- [Usage](#testing-usage)
- [Examples](#testing-examples)
- [Know issues](#testing-know-issues)

This application is a reference implementation for developers to show how to use the Python API and could
be used to easily check the accuracy. The Python API is a wrapper around the C++ API defined at [https://www.doubango.org/SDKs/kyc-documents-verif/docs/cpp-api.html](https://www.doubango.org/SDKs/kyc-documents-verif/docs/cpp-api.html). A C++ twin sample application is at [cpp/verify](../../cpp/verify).

The application accepts path to a JPEG/PNG/BMP/... file as input.

If you don't want to build this sample and is looking for a quick way to check the accuracy then try
our online webapp demo at [https://www.doubango.org/webapps/kyc-documents-verif](https://www.doubango.org/webapps/kyc-documents-verif).

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

<a name="prerequisite"></a>
# Prerequisite #

[**You must build the Python extension**](../../../python/README.md) before trying to run this sample. More information on how to build the extension could be found [here](../../../python/README.md)

<a name="testing-usage"></a>
# Usage #

`verify.py` is a Python command line application with the following usage:
```
verify.py \
      --image <path-to-image-to-process> \
      --assets <path-to-assets-folder> \
      [--tokenfile <path-to-license-token-file>] \
      [--tokendata <base64-license-token-data>]
```
Options surrounded with **[]** are optional.
- `--image` Path to the image(JPEG/PNG/BMP) to process. You can use California Driving License stored [here](../../../assets/images/United%20States%20-%20California%20Driving%20License%20%282017%29.jpg)
- `--assets` Path to the [assets](../../../assets) folder containing the configuration files and models. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#assets-folder.
- `--tokenfile` Path to the file containing the base64 license token if you have one. If not provided, then the application will act like a trial version. Default: *null*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-file.
- `--tokendata` Base64 license token if you have one. If not provided, then the application will act like a trial version. Default: *null*. More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#license-token-data.

<a name="testing-examples"></a>
# Examples #

## Move to the binaries folder ##
Before trying the next examples you have to navigate to the folder containing the [binaries](../binaries):
```
cd KYC-Documents-Verif-SDK/binaries/<<os>>/<<arch>>
```
For example:
 * On Windows x86_64: [KYC-Documents-Verif-SDK/binaries/windows/x86_64](../../../binaries/windows/x86_64)
 * On Linux x86_64: [KYC-Documents-Verif-SDK/binaries/linux/x86_64](../../../binaries/linux/x86_64)
 * ... you got the idea

## Try ##

- On **Linux x86_64**, you may use the next command:
```
PYTHONPATH=$PYTHONPATH:.:../../../python \
LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH \
python ../../../samples/python/verify/verify.py --image "../../../assets/images/United States - California Driving License (2017).jpg" --assets ../../../assets
```
Before trying to run the program on **Linux x86_64 you'll need to download libtensorflow.so as explained [here](../../../GPGPU.md#gpu-acceleration-tensorflow-linux)**.

- On **Windows x86_64**, you may use the next command:
```
setlocal
set PYTHONPATH=%PYTHONPATH%;.;../../../python
set PATH=%PATH%;%~dp0
python ../../../samples/python/liveness/verify.py --image "../../../assets/images/United States - California Driving License (2017).jpg" --assets ../../../assets
endlocal
```
If you want to make your life easier run [python_verify.bat](../../../binaries/windows/x86_64/python_verify.bat) to test on Windows. You can edit the file using Notepad to change the parameters.

The test image looks like this:

![Test image](../../../assets/images/United%20States%20-%20California%20Driving%20License%20%282017%29.jpg)


<a name="testing-know-issues"></a>
# Know issues #
If you get `undefined symbol: PyUnicode_FromFormat` error message, then make sure you're using Python 3 and same version as the one used to buid the extension. We tested the code on version **3.6.9** (Windows 8), **3.6.8** (Ubuntu 18) and **3.7.3** (Raspbian Buster). Run `python --version` to print your Python version. You may use `python3` instead of `python` to make sure you're using version 3.


