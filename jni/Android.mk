LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -D__STDC_CONSTANT_MACROS
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/common \
	$(LOCAL_PATH)/audio_fft_test \
	$(LOCAL_PATH)/test \
    $(LOCAL_PATH)/inc 
    
LOCAL_SRC_FILES := \
FFTComparator.cpp \
SongStudio.cpp 

LOCAL_STATIC_LIBRARIES := libne10_test libfft_comparator

LOCAL_LDFLAGS += $(LOCAL_PATH)/prebuilt/libNE10.a
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog

#LOCAL_LDLIBS += -L$(LOCAL_PATH)/prebuilt -lNE10

LOCAL_MODULE := libsongstudio
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))