#ifndef SONG_STUDIO_COMMON
#define SONG_STUDIO_COMMON
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "NE10.h"

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static inline long long currentTimeUs(){
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (long long)tv.tv_sec * 1000 + tv.tv_usec;
}
static inline long long currentTimeMills(){
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static inline float mix(const float in1, const float in2) {
	float mix = 0;
	float product = 0;
	if ((in1 > 0) && (in2 > 0)) {
		product = in1 * in2;
		mix = in1 + in2 - product;
	} else if ((in1 < 0) && (in2 < 0)) {
		product = in1 * in2;
		mix = in1 + in2 + product;
	} else {
		mix = in1 + in2;
	}
	return mix;
}
#endif //SONG_STUDIO_COMMON
