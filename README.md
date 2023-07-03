Documents recognition and verification is the core feature for all [KYC (Know Your Customer)](https://en.wikipedia.org/wiki/Know_your_customer) solutions.

With support for more than [140+ languages](Languages.pdf), [250+ countries/territories](Graphs.pdf), [5,000+ (and counting) document formats](Graphs.pdf) (Passport, Driver license, ID card, Resident card, Visa...)... we built a solution that can boost your KYC platform and put your company ahead of the competition.

We can automatically determine the issuing country and extract every field from the document (Name, Date of Birth, Date of Expiry, Address, Portrait, Signature...) with zero configuration. We also return the exact location (bounding boxes) of each field for visual inspection and verification.

We use state-of-the-art Deep Learning methods developed using Keras with Tensorflow benkend. Our framework is GPGPU accelerated using CUDA and optimized for CPUs (SIMD) using Intel OpenVINO. On both CPUs and GPUs the result is returned in **few milliseconds**.

|   |  | |
| ------------- | ------------- |-- |
| ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-1.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-2.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-3.jpg) |
| ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-4.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-5.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-6.jpg) |
| ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-7.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-8.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-9.jpg) |

We're planing to add support for **document forgery detection** and **document liveness** check in the next major release. Stay tuned or send us a mail if you want to be a beta-tester for these features.
The initial version(v0.1) supports both **Linux and Windows (x86-64)** and works on-premise (your own servers) with no connection to any external server. Future versions will support **iOS, Android, NVIDIA Jetson and RPI**.

The code released on Github (https://github.com/DoubangoTelecom/IdentityOCR_SDK) comes with many ready-to-use samples to help you get started easily. You can also check our online cloud-based implementation (no registration required) at https://www.doubango.org/webapps/kyc-documents-verif to check out the accuracy and precision before starting to play with the SDK.
