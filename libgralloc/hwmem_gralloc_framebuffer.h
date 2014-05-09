/*
 * Copyright (C) 2008 The Android Open Source Project
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


#ifndef HWMEM_GRALLOC_FRAMEBUFFER_H_
#define HWMEM_GRALLOC_FRAMEBUFFER_H_


#include <hardware/hardware.h>
#include <hardware/gralloc.h>
#include <hardware/fb.h>
#include <system/graphics.h>
#include <system/window.h>

int gralloc_alloc_framebuffer(alloc_device_t* dev,
    size_t size, int usage, buffer_handle_t* pHandle, int* stride,
    int* handled);

int fb_device_open(hw_module_t const* module, const char* name,
    hw_device_t** device);

int fb_compositionComplete();

#endif
