#include "com_example_ne10_sample_FFTComparator.h"
#include "fft_compare_util.h"
#include "fft_performance_tester.h"
#include "discrete_point_fft.h"

#define LOG_TAG "Ne10_JNI_Layer"

JNIEXPORT void JNICALL Java_com_example_ne10_1sample_FFTComparator_doFFT
  (JNIEnv * env, jclass clazz, jstring audioFilePathParam,
		  jstring mayerFFTResultPathParam, jstring dspFFTResultPathParam) {
	const char* audioFilePath = env->GetStringUTFChars(audioFilePathParam, NULL);
	const char* mayerFFTResultPath = env->GetStringUTFChars(mayerFFTResultPathParam, NULL);
	const char* dspFFTResultPath = env->GetStringUTFChars(dspFFTResultPathParam, NULL);
	LOGI("/** Start fft 测试结果对比 **/");
//	FFTCompareUtil* fftCompareUtil = new FFTCompareUtil();
//	fftCompareUtil->test(audioFilePath, mayerFFTResultPath, dspFFTResultPath);
//	delete fftCompareUtil;
	DisCretePointFFT* disCretePointFFT = new DisCretePointFFT();
	disCretePointFFT->test();
	delete disCretePointFFT;
//	FFTPerformanceTester* tester = new FFTPerformanceTester();
//	tester->test(audioFilePath, mayerFFTResultPath, dspFFTResultPath);
//	delete tester;
	LOGI("/** Ending fft 测试结果对比 **/");
	env->ReleaseStringUTFChars(audioFilePathParam, audioFilePath);
	env->ReleaseStringUTFChars(mayerFFTResultPathParam, mayerFFTResultPath);
	env->ReleaseStringUTFChars(dspFFTResultPathParam, dspFFTResultPath);
}
