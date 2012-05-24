LOCAL_PATH := $(realpath $(call my-dir)/../../../../../..)
PVRSDKDIR := $(LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := OGLES2HelloTriangle

### Add all source file names to be included in lib separated by a whitespace
LOCAL_SRC_FILES := \
				   TrainingCourse/HelloTriangle/OGLES2/OGLES2HelloTriangle_Android.cpp

LOCAL_C_INCLUDES :=	\
				    $(PVRSDKDIR)/Builds/OGLES2/Include

LOCAL_CFLAGS := -DBUILD_OGLES2

ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS += -fno-stack-protector 
endif

LOCAL_LDLIBS :=  \
				-llog \
				-landroid \
				-lEGL \
				-lGLESv2

LOCAL_STATIC_LIBRARIES := \
				          android_native_app_glue 

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)



