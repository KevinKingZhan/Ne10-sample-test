#include "fft_tester.h"

#define LOG_TAG "Ne10_FFTTester"

#define SAMPLES 16

FFTTester::FFTTester() {
}

FFTTester::~FFTTester() {
}

void FFTTester::test() {
	ne10_fft_cpx_float32_t src[SAMPLES]; // A source array of input data
	ne10_fft_cpx_float32_t dst[SAMPLES]; // A destination array for the transformed data
	ne10_fft_cfg_float32_t cfg;          // An FFT "configuration structure"

	ne10_fft_r2c_cfg_float32_t r2cCfg;
	ne10_result_t stat;
	// Initialise Ne10, using hardware auto-detection to set library function pointers.
	if ((stat = ne10_init()) != NE10_OK) {
		LOGE("Failed to initialise Ne10.\n");
	}
	stat = ne10_init_math (stat);
	if(stat != NE10_OK) {
		LOGE("Failed to initialise Ne10 Math Module.\n");
	}
	stat = ne10_init_dsp (stat);
	if(stat != NE10_OK) {
		LOGE("Failed to initialise Ne10 DSP Module.\n");
	}
	// Prepare the complex-to-complex single precision floating point FFT configuration
	// structure for inputs of length `SAMPLES`. (You need only generate this once for a
	// particular input size.)
//	cfg = ne10_fft_alloc_c2c_float32(SAMPLES);
	cfg = ne10_fft_alloc_c2c_float32_neon(SAMPLES);
	r2cCfg = ne10_fft_alloc_r2c_float32(SAMPLES);

	ne10_float32_t rSrc[SAMPLES];

	// Generate test input values (with both real and imaginary components)
	for (int i = 0; i < SAMPLES; i++) {
		src[i].r = (ne10_float32_t) rand() / RAND_MAX * 50.0f;
		src[i].i = 0.0f;//(ne10_float32_t) rand() / RAND_MAX * 50.0f;
		rSrc[i] = src[i].r;
	}

	// Perform the FFT (for an IFFT, the last parameter should be `1`)
//	ne10_fft_c2c_1d_float32(dst, src, cfg, 0);
	ne10_fft_c2c_1d_float32_neon(dst, src, cfg, 0);
//	ne10_fft_r2c_1d_float32(dst, rSrc, r2cCfg);
//	ne10_fft_r2c_1d_float32_neon(dst, rSrc, r2cCfg);

	// Display the results
	LOGI("Display the results...");
	for (int i = 0; i < SAMPLES; i++) {
		LOGI("IN[%2d]: %10.4f + %10.4fi\t", i, src[i].r, src[i].i);
		LOGI("OUT[%2d]: %10.4f + %10.4fi\n", i, dst[i].r, dst[i].i);
	}

	// Free the allocated configuration structure
	ne10_fft_destroy_c2c_float32(cfg);
}
