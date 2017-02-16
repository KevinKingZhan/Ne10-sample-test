LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../common \
    $(LOCAL_PATH)/../inc  
       
LOCAL_SRC_FILES = \
./matrix_multiply_tester.cpp \
./intro_tester.cpp \
./fir_tester.cpp \
./fft_tester.cpp 

LOCAL_MODULE := libne10_test
include $(BUILD_STATIC_LIBRARY)