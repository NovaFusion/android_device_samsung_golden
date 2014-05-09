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


#ifndef HWMEM_GRALLOC_PMEM_H_
#define HWMEM_GRALLOC_PMEM_H_


#include <hardware/hardware.h>
#include <hardware/gralloc.h>

#include "hwmem_gralloc.h"


int gralloc_create_handle_from_buffer_pmem(int fd, size_t size, size_t offset, void* base,
    native_handle_t** handle);

int gralloc_get_buf_allocator_handle_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t* hnd);

int gralloc_pin_buf_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t *hnd);

int gralloc_unpin_buf_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t *hnd);

int gralloc_get_buf_offset_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t* hnd);

int gralloc_register_buffer_pmem(gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t* hnd);
int gralloc_unregister_buffer_pmem(gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t* hnd);

int gralloc_lock_pmem(gralloc_module_t const* module, struct hwmem_gralloc_buf_handle_t* hnd,
    int usage, int l, int t, int w, int h, void** vaddr);
int gralloc_unlock_pmem(gralloc_module_t const* module, struct hwmem_gralloc_buf_handle_t* hnd);

int gralloc_free_pmem(alloc_device_t* dev, struct hwmem_gralloc_buf_handle_t* hnd);


#endif
