LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
   stelp_log.c \
   stelp_fatal.c \
   stelp_time.c \
   OMXdebug.cpp \
   OMXdebug_specific.c

LOCAL_LDLIBS := -llog -lpthread -lrt 
LOCAL_CFLAGS += -DFREQ_SCALING_ALLOWED -DANDROID

LOCAL_MODULE:= libstelpcutils
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false

LOCAL_SHARED_LIBRARIES:= \
	libutils \
	libcutils \
	liblog

MULTIMEDIA_PATH := $(LOCAL_PATH)/../multimedia

LOCAL_C_INCLUDES += \
	$(TOP)/hardware/libhardware/include  \
	$(TOP)/frameworks/native/include/media/hardware \
	$(TOP)/frameworks/native/include \
	$(MULTIMEDIA_PATH)/shared/utils/include \
	$(MULTIMEDIA_PATH)/shared/omxilosalservices_api \
	$(MULTIMEDIA_PATH)/shared/ens_interface/nmf/common \
	$(MULTIMEDIA_PATH)/shared/ens_interface/include \
	$(MULTIMEDIA_PATH)/shared/ens_interface/omxil \
	$(MULTIMEDIA_PATH)/shared/omxil \
	$(MULTIMEDIA_PATH)/linux/trace/api \
	$(MULTIMEDIA_PATH)/audio/drc/proxy \
	$(MULTIMEDIA_PATH)/audio/afm/proxy/include \
	$(MULTIMEDIA_PATH)/audio/audio_chipset_apis \
	$(MULTIMEDIA_PATH)/audio/speech_proc_vcs/wrapper/inc \
	$(MULTIMEDIA_PATH)/video/video_chipset_apis \
	$(MULTIMEDIA_PATH)/video/vfm/include \
	$(MULTIMEDIA_PATH)/audio/comfortnoise/proxy \
	$(MULTIMEDIA_PATH)/shared/ste_shai/include \
	$(LOCAL_PATH)/../

include $(BUILD_SHARED_LIBRARY)
