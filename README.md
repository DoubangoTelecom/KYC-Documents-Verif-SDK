- [Getting started](#getting-started)
  - [Checking out the source code](#checkout-source)
  - [Roadmap](#roadmap)
  - [Getting help](#technical-questions)


- Online web demo at https://www.doubango.org/webapps/face-liveness/
- Full documentation for the SDK at https://www.doubango.org/SDKs/face-liveness/docs/
- Supported languages (API): **C++**, **C#**, **Java** and **Python**
- Open source Computer Vision Library: https://github.com/DoubangoTelecom/compv
<hr />

Documents recognition and verification is the core feature for all [KYC (Know Your Customer)](https://en.wikipedia.org/wiki/Know_your_customer) solutions.

With support for more than [140+ languages](Languages.pdf), [250+ countries/territories](Graphs.pdf), [5,000+ (and counting) document formats](Graphs.pdf) (Passport, Driver license, ID card, Resident card, Visa...)... we built a solution that can boost your KYC platform and put your company ahead of the competition.

We can automatically determine the issuing country and extract every field from the document (Name, Date of Birth, Date of Expiry, Address, Portrait, Signature...) with zero configuration. We also return the exact location (bounding boxes) of each field for visual inspection and verification.

We use state-of-the-art Deep Learning methods developed using Keras with Tensorflow benkend. Our framework is GPGPU accelerated using CUDA and optimized for CPUs (SIMD) using Intel OpenVINO. On both CPUs and GPUs the result is returned in **few milliseconds**.

|   |  | |
| ------------- | ------------- |-- |
| ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-1.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-2.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-3.jpg) |
| ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-4.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-5.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-6.jpg) |
| ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-7.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-8.jpg)  | ![plot](https://www.doubango.org/webapps/kyc-documents-verif/img/low-9.jpg) |

<a name="getting-started"></a>
# Getting started #
This version supports both Windows and Linux x86_64.

<a name="checkout-source"></a>
## Checking out the source code ##
The deep learning models, the binaries and the datasets are on a private reporsitory for obvious reasons. To get access to the private repository you'll need to:
  - 1/ Opt-out from all Doubango's private repositories. You cannot be beta tester on more than 1 repo at the same time.
  - 2/ [Send us a mail](https://www.doubango.org/#contact) with your company name and Github user name (to be added to the private repo). The mail must come from @YourCompanyName, mails from other domains (e.g. @Gmail) will be ignored. **The terms of use do not allow you to decompile or reverse engineer the models.**

To checkout the source code: `git clone --recurse-submodules -j8 https://github.com/DoubangoTelecom/KYC-Documents-Verif-SDK`
If you already have the code and want to update to the latest version: `git pull --recurse-submodules`

<a name="roadmap"></a>
# Roadmap #
This is not a wish list, we already have all listed features already working on demos.
 - Image forgery detection
 - Barecode reader
 - Document liveness detection (will be based on https://github.com/DoubangoTelecom/FaceLivenessDetection-SDK)
 - Credit card OCR (improved version of https://github.com/DoubangoTelecom/ultimateCreditCard-SDK)
 - Bank check information extraction/OCR from Magnetic Ink Character Recognition [MICR] (E-13B & CMC-7) (improved version of https://github.com/DoubangoTelecom/ultimateMICR-SDK)
 - Age estimation

<a name="technical-questions"></a>
# Technical questions #
Please check our [discussion group](https://groups.google.com/forum/#!forum/doubango-ai) or [twitter account](https://twitter.com/doubangotelecom?lang=en)
