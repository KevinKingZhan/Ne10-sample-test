LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS := -pie -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=hard -mfpu=vfp3 -Wl,--no-warn-mismatch -std=gnu99 -fPIC

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../common \
    $(LOCAL_PATH)/../inc  
       
LOCAL_SRC_FILES = \
./slide_window/slide_window.cpp \
./mayer_fft/mayer_fft.cpp \
./fft_performance_tester.cpp \
./discrete_point_fft.cpp \
./fft_compare_util.cpp 

LOCAL_MODULE := libfft_comparator
include $(BUILD_STATIC_LIBRARY)