LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/common \
	$(LOCAL_PATH)/audio_fft_test \
	$(LOCAL_PATH)/test \
    $(LOCAL_PATH)/inc 
    
LOCAL_SRC_FILES := \
FFTComparator.cpp \
SongStudio.cpp 

LOCAL_STATIC_LIBRARIES := libne10_test libfft_comparator

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_LDFLAGS += $(LOCAL_PATH)/prebuilt/libNE10.a
#LOCAL_CFLAGS := -pie -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=hard -mfpu=vfp3 -Wl,--no-warn-mismatch -std=gnu99 -fPIC
#LOCAL_LDFLAGS += -pie -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=hard -mfpu=vfp3 -Wl,--no-warn-mismatch -std=gnu99 -fPIC

#LOCAL_LDLIBS += -L$(LOCAL_PATH)/prebuilt -lNE10

LOCAL_MODULE := libsongstudio
include $(BUILD_SHARED_LIBRARY)
include $(call all-makefiles-under,$(LOCAL_PATH))