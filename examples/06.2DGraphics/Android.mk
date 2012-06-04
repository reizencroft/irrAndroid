JNI_FOLDER := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := twodgraphics

### Add all source file names to be included in lib separated by a whitespace
LOCAL_SRC_FILES :=  ../../examples/06.2DGraphics/main.cpp

LOCAL_C_INCLUDES :=	\
					../../include

LOCAL_CFLAGS := -DBUILD_OGLES2 -g -D_DEBUG

ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS += -fno-stack-protector 
endif

LOCAL_LDLIBS :=  \
				-llog \
				-landroid \
				-lEGL \
				-lGLESv2 \
				-lz

LOCAL_STATIC_LIBRARIES := \
				          android_native_app_glue \
						  IrrAndroid

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)