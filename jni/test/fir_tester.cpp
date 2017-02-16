#include "fir_tester.h"

#define LOG_TAG "Ne10_FIRTester"

#define NUMTAPS   4
#define NUMBLOCKS 2
#define BLOCKSIZE 8
#define BUFFSIZE (NUMBLOCKS * BLOCKSIZE)

FIRTester::FIRTester() {
}

FIRTester::~FIRTester() {
}

void FIRTester::test() {
	ne10_float32_t src[BUFFSIZE]; // A source array of input data
	ne10_float32_t dst[BUFFSIZE]; // A destination array for the transformed data
	ne10_float32_t st[NUMTAPS + BLOCKSIZE - 1]; // A "state" buffer for use within the FIR
	ne10_float32_t coeffs[NUMTAPS]; // An array of FIR coefficients (in reverse order)
	ne10_fir_instance_f32_t cfg;    // An FIR "instance structure"

	// Initialise Ne10, using hardware auto-detection to set library function pointers.
	if (ne10_init() != NE10_OK) {
		LOGE("Failed to initialise Ne10.\n");
	}

	// Prepare the FIR instance structure, storing `NUMTAPS`, `coeffs`, and `st` within
	// it, and clearing the state buffer. (For constant parameters, this process can
	// instead be performed manually.)
	if (ne10_fir_init_float(&cfg, NUMTAPS, coeffs, st, BLOCKSIZE) != NE10_OK) {
		fprintf(stderr, "Failed to initialise FIR instance structure.\n");
	}
	// Generate test coefficient values
	for (int i = 0; i < NUMTAPS; i++) {
		coeffs[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
	}

	// Generate test input values
	for (int i = 0; i < BUFFSIZE; i++) {
		src[i] = (ne10_float32_t) rand() / RAND_MAX * 20.0f;
	}

	// Perform the FIR filtering of the input buffer in `NUMBLOCKS` blocks of `BLOCKSIZE`
	// elements using the parameters set up in the FIR instance structure `cfg`.
	for (int b = 0; b < NUMBLOCKS; b++) {
		ne10_fir_float(&cfg, src + (b * BLOCKSIZE), dst + (b * BLOCKSIZE), BLOCKSIZE);
	}
	// Display the results (dst[i] = b[0] * src[i] + b[1] * src[i - 1] + b[2] * src[i - 2]
	//                               + ... + b[NUMTAPS - 1] * src[i - (NUMTAPS - 1)])
	LOGI("Coefficients:\n");
	for (int i = NUMTAPS - 1; i >= 0; i--) {
		LOGI("\tb[%d] = %5.4f\n", NUMTAPS - (i + 1), coeffs[i]);
	}
	for (int i = 0; i < BUFFSIZE; i++) {
		LOGI("IN[%2d]: %9.4f\t", i, src[i]);
		LOGI("OUT[%2d]: %9.4f\n", i, dst[i]);
	}
}
