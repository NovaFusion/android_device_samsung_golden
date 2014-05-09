# Copyright (C) 2008 The Android Open Source Project
# Copyright (C) 2014 NovaFusion http://novafusion.pl
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)
MULTIMEDIA_PATH := $(LOCAL_PATH)/../multimedia

include $(CLEAR_VARS)
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_SHARED_LIBRARIES := liblog libcutils libGLESv1_CM

LOCAL_SRC_FILES := 	\
	hwmem_gralloc.c hwmem_gralloc_pmem.c hwmem_gralloc_framebuffer.c

LOCAL_C_INCLUDES += $(MULTIMEDIA_PATH)/linux/b2r2lib/include $(MULTIMEDIA_PATH)
LOCAL_MODULE := gralloc.montblanc
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS:= -DLOG_TAG=\"gralloc\"
include $(BUILD_SHARED_LIBRARY)
