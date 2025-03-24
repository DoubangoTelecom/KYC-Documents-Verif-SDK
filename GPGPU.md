- [GPGPU acceleration](#gpu-acceleration)
  - [OpenVINO](#gpu-acceleration-openvino)
  - [Tensorflow libraries](#gpu-acceleration-tensorflow)
    - [Windows](#gpu-acceleration-tensorflow-windows)
    - [Linux](#gpu-acceleration-tensorflow-linux)
- [Migration to Tensorflow 2.x and CUDA 11.x and 12.x](#migration-tf2)
  - Tensorflow 2.6 and CUDA 11.2 
  - Latest Tensorflow and CUDA versions   

<a name="gpu-acceleration"></a>
# GPGPU acceleration #

We use [Tensorflow](https://www.tensorflow.org/) and [OpenVINO](https://docs.openvinotoolkit.org/) as deep learning frameworks. The current repository contains [Tensorflow](https://www.tensorflow.org/) libraries built without GPU functions to reduce the size. Also, few developers need GPGPU accelerated libraries. The GPU libraries will work on your device even if you don't have NVIDIA GPU.

<a name="gpu-acceleration-openvino"></a>
## OpenVINO ##
By default we use the "CPU" device when OpenVINO is enabled. If you have an Intel GPU and want to use it, then change the device type to "GPU" (`--openvino_device="GNA"|"HETERO"|"CPU"|"MULTI"|"GPU"|"MYRIAD"|"HDDL"|"FPGA"` command). More information at https://www.doubango.org/SDKs/kyc-documents-verif/docs/Configuration_options.html#openvino-device.

<a name="gpu-acceleration-tensorflow"></a>
## Tensorflow libraries ##
The Tensorflow libraries are hosted at:
 - [1] Windows_x86_64_CPU+GPU: https://doubango.org/deep_learning/libtensorflow_r1.15_cpu+gpu_windows_x86-64.zip
 - [2] Windows_x86_64_CPU: https://doubango.org/deep_learning/libtensorflow_r1.15_cpu_windows_x86-64.zip
 - [3] Linux_x86_64_CPU+GPU: https://doubango.org/deep_learning/libtensorflow_r1.14_cpu+gpu_linux_x86-64.tar.gz
 - [4] Linux_x86_64_CPU: https://doubango.org/deep_learning/libtensorflow_r1.14_cpu_linux_x86-64.tar.gz
 - [5] Jetson_aarch64_GPU: https://doubango.org/deep_learning/libtensorflow-1.15.4-jetson-gpu.tar.xz

<a name="gpu-acceleration-tensorflow-window"></a>
### Windows ###
To use the Tensorflow version with GPU funtions you'll need to download [[1]](https://doubango.org/deep_learning/libtensorflow_r1.15_cpu+gpu_windows_x86-64.zip), extract **tensorflow.dll** and override CPU-only [tensorflow.dll](../../binaries/windows/x86_64/tensorflow.dll) in [binaries/windows/x86_64](../../binaries/windows/x86_64)

<a name="gpu-acceleration-tensorflow-linux"></a>
### Linux ###
On Linux x86_64, [libtensorflow.so](../../binaries/linux/x86_64/libtensorflow.so) is missing in the [binaries folder](../../binaries/linux/x86_64). You'll need to download your preferred Tensorflow version ([[3]](https://doubango.org/deep_learning/libtensorflow_r1.14_cpu+gpu_linux_x86-64.tar.gz) or [[4]](https://doubango.org/deep_learning/libtensorflow_r1.14_cpu_linux_x86-64.tar.gz)) and copy the content to [binaries/linux/x86_64](../../binaries/linux/x86_64).

<a name="migration-tf2"></a>
# Migration to Tensorflow 2.x and CUDA 11.x and 12.x#

## Tensorflow 2.6 and CUDA 11.2
Our SDK is built and shipped with Tensorflow 1.x to make it work on oldest NVIDIA GPUs. If you want to use newest NVIDIA GPUs (e.g. RTX3060) which requires CUDA 11.x, then you'll need to upgrade the Tensorflow version. Check https://www.tensorflow.org/install/source#gpu to know which CUDA version is required for your Tensorflow version.

***This section is about Tensorflow 2.6, Ubuntu 20.04.2 LTS, NVIDIA RTX3060 GPU and cuda_11.1.TC455_06.29190527_0***. Tensorflow 2.6 is the latest (**11/29/2021**) public version published at [https://www.tensorflow.org/install/lang_c](https://www.tensorflow.org/install/lang_c?hl=en). Please note that we use CUDA 11.1 instead of 11.2 as suggested at https://www.tensorflow.org/install/source#gpu but both will work.

- Links:
  - Linux CPU only:	https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-linux-x86_64-2.6.0.tar.gz
  - Linux GPU support:	https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-linux-x86_64-2.6.0.tar.gz
  - Windows CPU only:	https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-windows-x86_64-2.6.0.zip
  - Windows GPU support:	https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-windows-x86_64-2.6.0.zip

- Download and uzip Tensorflow 2.6 inside the binaries folder
```
cd  KYC-Documents-Verif-SDK/binaries/linux/x86_64
wget https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-linux-x86_64-2.6.0.tar.gz
tar -xf libtensorflow-gpu-linux-x86_64-2.6.0.tar.gz
cp lib/* .
```
make sure you don't have older Tensorflow binaries in that directory.
`ls` from the current folder will give you `libtensorflow_framework.so  libtensorflow_framework.so.2  libtensorflow_framework.so.2.6.0  libtensorflow.so  libtensorflow.so.2  libtensorflow.so.2.6.0`

- Checking dependencies and workaround

Now when you run `ldd libKYCDocumentsVerifSDK.so` you'll see `libtensorflow.so.1 => not found`. That's normal because the SDK is built for Tensorflow 1.x. Do not worry, we use the [C-API](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/c/c_api.h) which is the same for all Tensorflow versions.

The litte trick is to duplicate and rename the symbolic link: `cp libtensorflow.so.2 libtensorflow.so.1`

## Latest Tensorflow and CUDA versions   
The above guide could be used for latest Tensorflow and CUDA versions if CUDA 11.2 is too old for your GPU:
  - [TF 2.18.0 (CUDA 12.5)](https://storage.googleapis.com/tensorflow/versions/2.18.0/libtensorflow-gpu-linux-x86_64.tar.gz)
  - [TF 2.16.1 (CUDA 12.3)](https://storage.googleapis.com/tensorflow/versions/2.16.1/libtensorflow-gpu-linux-x86_64.tar.gz)
  - [TF 2.14.0 (CUDA 11.8)](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-linux-x86_64-2.14.0.tar.gz)
