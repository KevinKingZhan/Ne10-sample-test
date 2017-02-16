#include "intro_tester.h"

#define LOG_TAG "Ne10_IntroTester"

#define ARR_LEN 5

IntroTester::IntroTester() {
}

IntroTester::~IntroTester() {
}

void IntroTester::test() {
	// Initialise Ne10, using hardware auto-detection to set function pointers such as
	// `ne10_addc_float` to point to the Ne10 code best optimised for this machine. (There
	// is no need to do this if the C or NEON versions of all Ne10 functions are called
	// directly rather than through function pointers.)
	if (ne10_init() != NE10_OK) {
		LOGE("Failed to initialise Ne10.\n");
	}

	test_add_dynamic();
	test_add_static();
}

/**
 * @brief A simple example of calling the C and NEON specific versions of Ne10 functions
 *        directly -- in this case, `ne10_addc_float_c` and `ne10_addc_float_neon`.
 */
void IntroTester::test_add_static() {
	ne10_float32_t src[ARR_LEN];
	ne10_float32_t cst;
	ne10_float32_t dst_c[ARR_LEN];
	ne10_float32_t dst_neon[ARR_LEN];

	for (int i = 0; i < ARR_LEN; i++) {
		src[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
	}
	cst = (ne10_float32_t) rand() / RAND_MAX * 5.0f;

	ne10_addc_float_c(dst_c, src, cst, ARR_LEN);
	ne10_addc_float_neon(dst_neon, src, cst, ARR_LEN);

	LOGI("test_intro[test_add_static]:\n");
	for (int i = 0; i < ARR_LEN; i++) {
		LOGI("\tne10_addc_float_c:\t%f + %f = %f\n", src[i], cst, dst_c[i]);
		LOGI("\tne10_addc_float_neon:\t%f + %f = %f\n", src[i], cst, dst_neon[i]);
	}
}

/**
 * @brief A simple example of using `ne10_addc_float`, an Ne10 function pointer that
 *        gets dynamically initialised to the most appropriate function for the hardware.
 */
void IntroTester::test_add_dynamic() {
	ne10_float32_t src[ARR_LEN]; // A source array of scalar floats
	ne10_float32_t cst;          // A constant scalar to add to the elements in `src`
	ne10_float32_t dst[ARR_LEN]; // A destination array for the results of the addition

	// Generate test input values for `src` and `cst` using `rand()`
	for (int i = 0; i < ARR_LEN; i++) {
		src[i] = (ne10_float32_t) rand() / RAND_MAX * 5.0f;
	}
	cst = (ne10_float32_t) rand() / RAND_MAX * 5.0f;

	// Perform the operation! This will use the NEON-optimised version of the function
	// if NEON hardware has been detected, or will otherwise fall back to the C version.
	ne10_addc_float(dst, src, cst, ARR_LEN);

	// Display the results
	LOGI("test_intro[test_add_dynamic]:\n");
	for (int i = 0; i < ARR_LEN; i++) {
		LOGI("\tne10_addc_float:\t%f + %f = %f\n", src[i], cst, dst[i]);
	}
}
