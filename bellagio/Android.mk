LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    src/common.c \
    src/content_pipe_file.c \
    src/content_pipe_inet.c \
    src/getline.c \
    src/omx_create_loaders_linux.c \
    src/omx_reference_resource_manager.c \
    src/omxcore.c \
    src/queue.c \
    src/st_static_component_loader.c \
    src/tsemaphore.c \
    src/utils.c

LOCAL_SRC_FILES += \
    src/base/omx_base_audio_port.c \
    src/base/omx_base_clock_port.c \
    src/base/omx_base_component.c \
    src/base/omx_base_filter.c \
    src/base/omx_base_image_port.c \
    src/base/omx_base_port.c \
    src/base/omx_base_sink.c \
    src/base/omx_base_source.c \
    src/base/omx_base_video_port.c \
    src/base/OMXComponentRMExt.c

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include

LOCAL_CFLAGS += -DRELEASE -D__RELEASE

LOCAL_MODULE := libomxil-bellagio
LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES := \
   libdl \
   liblog

include $(BUILD_SHARED_LIBRARY)