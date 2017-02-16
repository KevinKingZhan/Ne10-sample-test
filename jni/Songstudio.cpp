#include "com_example_ne10_sample_Ne10Tester.h"
#include "matrix_multiply_tester.h"
#include "intro_tester.h"
#include "fir_tester.h"
#include "fft_tester.h"

#define LOG_TAG "Ne10_JNI_Layer"

JNIEXPORT void JNICALL Java_com_example_ne10_1sample_Ne10Tester_test(JNIEnv * env, jclass clazz) {
	LOGI("/** 矩阵相乘的测试 **/");
	MatrixMultiplyTester* matrixMultiplyTester = new MatrixMultiplyTester();
	matrixMultiplyTester->test();
	delete matrixMultiplyTester;

	LOGI("/** FFT的测试 **/");
	FFTTester* fftTester = new FFTTester();
	fftTester->test();
	delete fftTester;

	LOGI("/** FIR的测试 **/");
	FIRTester* firTester = new FIRTester();
	firTester->test();
	delete firTester;

	LOGI("/** 浮点数运算的测试 **/");
	IntroTester* introTester = new IntroTester();
	introTester->test();
	delete introTester;
}
