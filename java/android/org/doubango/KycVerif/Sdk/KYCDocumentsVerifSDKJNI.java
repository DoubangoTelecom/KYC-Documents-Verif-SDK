/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.9
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.doubango.KycVerif.Sdk;

public class KYCDocumentsVerifSDKJNI {

  static {
    System.loadLibrary("FaceLivenessDetectionSDK");
  }

  public final static native long new_KycVerifSdkResult__SWIG_0();
  public final static native long new_KycVerifSdkResult__SWIG_1(int jarg1, String jarg2, String jarg3, long jarg4);
  public final static native long new_KycVerifSdkResult__SWIG_2(int jarg1, String jarg2, String jarg3);
  public final static native long new_KycVerifSdkResult__SWIG_3(long jarg1, KycVerifSdkResult jarg1_);
  public final static native void delete_KycVerifSdkResult(long jarg1);
  public final static native int KycVerifSdkResult_code(long jarg1, KycVerifSdkResult jarg1_);
  public final static native String KycVerifSdkResult_phrase(long jarg1, KycVerifSdkResult jarg1_);
  public final static native String KycVerifSdkResult_json(long jarg1, KycVerifSdkResult jarg1_);
  public final static native long KycVerifSdkResult_numVIZs(long jarg1, KycVerifSdkResult jarg1_);
  public final static native boolean KycVerifSdkResult_isOK(long jarg1, KycVerifSdkResult jarg1_);
  public final static native void delete_KycVerifSdkParallelDeliveryCallback(long jarg1);
  public final static native void KycVerifSdkParallelDeliveryCallback_onNewResult(long jarg1, KycVerifSdkParallelDeliveryCallback jarg1_, long jarg2, KycVerifSdkResult jarg2_);
  public final static native long KycVerifSdkEngine_init__SWIG_0(String jarg1, long jarg2, KycVerifSdkParallelDeliveryCallback jarg2_);
  public final static native long KycVerifSdkEngine_init__SWIG_1(String jarg1);
  public final static native long KycVerifSdkEngine_init__SWIG_2();
  public final static native long KycVerifSdkEngine_deInit();
  public final static native long KycVerifSdkEngine_process__SWIG_0(int jarg1, java.nio.ByteBuffer jarg2, long jarg3, long jarg4, long jarg5, int jarg6);
  public final static native long KycVerifSdkEngine_process__SWIG_1(int jarg1, java.nio.ByteBuffer jarg2, long jarg3, long jarg4, long jarg5);
  public final static native long KycVerifSdkEngine_process__SWIG_2(int jarg1, java.nio.ByteBuffer jarg2, long jarg3, long jarg4);
  public final static native long KycVerifSdkEngine_process__SWIG_3(int jarg1, java.nio.ByteBuffer jarg2, java.nio.ByteBuffer jarg3, java.nio.ByteBuffer jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9, long jarg10, int jarg11);
  public final static native long KycVerifSdkEngine_process__SWIG_4(int jarg1, java.nio.ByteBuffer jarg2, java.nio.ByteBuffer jarg3, java.nio.ByteBuffer jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9, long jarg10);
  public final static native long KycVerifSdkEngine_process__SWIG_5(int jarg1, java.nio.ByteBuffer jarg2, java.nio.ByteBuffer jarg3, java.nio.ByteBuffer jarg4, long jarg5, long jarg6, long jarg7, long jarg8, long jarg9);
  public final static native long KycVerifSdkEngine_process__SWIG_6(java.nio.ByteBuffer jarg1, long jarg2);
  public final static native int KycVerifSdkEngine_exifOrientation(java.nio.ByteBuffer jarg1, long jarg2);
  public final static native long KycVerifSdkEngine_requestRuntimeLicenseKey__SWIG_0(boolean jarg1);
  public final static native long KycVerifSdkEngine_requestRuntimeLicenseKey__SWIG_1();
  public final static native void delete_KycVerifSdkEngine(long jarg1);
}
