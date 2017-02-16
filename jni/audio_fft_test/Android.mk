LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -DHAVE_CONFIG_H -DFPM_ARM -ffast-math -O3
LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../common \
    $(LOCAL_PATH)/../inc  
       
LOCAL_SRC_FILES = \
./mayer_fft/mayer_fft.cpp \
./discrete_point_fft.cpp \
./fft_compare_util.cpp 

LOCAL_MODULE := libfft_comparator
include $(BUILD_STATIC_LIBRARY)