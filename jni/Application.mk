APP_ABI := armeabi-v7a
APP_CPPFLAGS := -pie -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=softfp -mfpu=vfp3 -Wl,--no-warn-mismatch -std=gnu99 -fPIC
APP_CFLAGS := -pie -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=softfp -mfpu=vfp3 -Wl,--no-warn-mismatch -std=gnu99 -fPIC
APP_LDFLAGS := -mthumb-interwork -mthumb -march=armv7-a -mfloat-abi=softfp -mfpu=vfp3 -Wl,--no-warn-mismatch -std=gnu99 -fPIC
APP_PLATFORM := android-15
NDK_TOOLCHAIN_VERSION = 4.9
ANDROID=1
DEBUG=1