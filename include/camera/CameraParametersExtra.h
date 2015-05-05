/*
 * Copyright (C) 2014 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "camera/CameraParametersExtraDurationTimer.h"

#define CAMERA_PARAMETERS_EXTRA_C \
const char CameraParameters::SCENE_MODE_AQUA[] = "aqua"; \
const char CameraParameters::SCENE_MODE_BACKLIGHT[] = "backlight"; \
const char CameraParameters::SCENE_MODE_DUSKDAWN[] = "duskdawn"; \
const char CameraParameters::SCENE_MODE_FALLCOLOR[] = "fallcolor"; \
const char CameraParameters::SCENE_MODE_TEXT[] = "text"; \
const char CameraParameters::PIXEL_FORMAT_YUV420SPNV12[] = "yuv420spnv12"; \
const char CameraParameters::PIXEL_FORMAT_YUV420MB[] = "yuv420mb"; \
const char CameraParameters::PIXEL_FORMAT_YVU422SP[] = "yvu422sp"; \
const char CameraParameters::PIXEL_FORMAT_YVU422P[] = "yvu422p"; \
const char CameraParameters::PIXEL_FORMAT_YVU420SP[] = "yvu420sp"; \
const char CameraParameters::PIXEL_FORMAT_YVU420P[]  = "yvu420p"; \
const char CameraParameters::KEY_RECORD_STRIDE[] = "record-stride"; \
const char CameraParameters::KEY_RECORD_SLICE_HEIGHT[] = "record-slice-height"; \
CAMERA_PARAMETERS_EXTRA_C_DURATION_TIMER \

#define CAMERA_PARAMETERS_EXTRA_H \
    static const char SCENE_MODE_AQUA[]; \
    static const char SCENE_MODE_BACKLIGHT[]; \
    static const char SCENE_MODE_DUSKDAWN[]; \
    static const char SCENE_MODE_FALLCOLOR[]; \
    static const char SCENE_MODE_TEXT[]; \
    static const char PIXEL_FORMAT_YUV420SPNV12[]; \
    static const char PIXEL_FORMAT_YVU422SP[]; \
    static const char PIXEL_FORMAT_YVU422P[];  \
    static const char PIXEL_FORMAT_YVU420SP[]; \
    static const char PIXEL_FORMAT_YVU420P[]; \
    static const char PIXEL_FORMAT_YUV420MB[]; \
    static const char KEY_RECORD_STRIDE[]; \
    static const char KEY_RECORD_SLICE_HEIGHT[];
