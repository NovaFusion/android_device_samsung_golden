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


#include <limits.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <cutils/log.h>
#include <cutils/atomic.h>

#include <hardware/hardware.h>
#include <hardware/gralloc.h>
#include "gralloc_stericsson_ext.h"

#include "hwmem_gralloc.h"

#if HAVE_ANDROID_OS
#include <linux/android_pmem.h>
#endif


// we need this for now because pmem cannot mmap at an offset
#define PMEM_HACK   1

/* desktop Linux needs a little help with gettid() */
#if defined(ARCH_X86) && !defined(HAVE_ANDROID_OS)
#define __KERNEL__
# include <linux/unistd.h>
pid_t gettid() { return syscall(__NR_gettid);}
#undef __KERNEL__
#endif

/*****************************************************************************/

enum {
    LOCK_STATE_WRITE     =   1<<31,
    LOCK_STATE_MAPPED    =   1<<30,
    LOCK_STATE_READ_MASK =   0x3FFFFFFF
};

static int gralloc_map(gralloc_module_t const* module,
        struct hwmem_gralloc_buf_handle_t* hnd,
        void** vaddr)
{
    if (!(hnd->flags & PRIV_FLAGS_FRAMEBUFFER)) {
        size_t size = hnd->size;
#if PMEM_HACK
        size += hnd->offset;
#endif
        void* mappedAddress = mmap(0, size,
                PROT_READ|PROT_WRITE, MAP_SHARED, hnd->fd, 0);
        if (mappedAddress == MAP_FAILED) {
            ALOGE("Could not mmap %s", strerror(errno));
            return -errno;
        }
        hnd->base_addr = (int)mappedAddress + hnd->offset;
        //ALOGD("gralloc_map() succeeded fd=%d, off=%d, size=%d, vaddr=%p",
        //        hnd->fd, hnd->offset, hnd->size, mappedAddress);
    }
    *vaddr = (void*)hnd->base_addr;
    return 0;
}

static int gralloc_unmap(gralloc_module_t const* module,
        struct hwmem_gralloc_buf_handle_t* hnd)
{
    if (!(hnd->flags & PRIV_FLAGS_FRAMEBUFFER)) {
        void* base = (void*)hnd->base_addr;
        size_t size = hnd->size;
#if PMEM_HACK
        base = (void*)((int)base - hnd->offset);
        size += hnd->offset;
#endif
        //ALOGD("unmapping from %p, size=%d", base, size);
        if (munmap(base, size) < 0) {
            ALOGE("Could not unmap %s", strerror(errno));
        }
    }
    hnd->base_addr = 0;
    return 0;
}

static int get_pmem_file_info(dev_t* device, ino_t* serial_number)
{
    static int write_values_initiated = 0;
    static int error_occured_when_retreiving_pmem_file_info = 0;
    static int error_code_produced_when_retreiving_pmem_file_info;
    static int cached_values_present = 0;
    static dev_t pmem_files_device;
    static ino_t pmem_files_serial_number;

    struct stat pmem_file_info;

    if (error_occured_when_retreiving_pmem_file_info)
    {
        errno = error_code_produced_when_retreiving_pmem_file_info;

        return 0;
    }
    else if (cached_values_present)
    {
        *device = pmem_files_device;
        *serial_number = pmem_files_serial_number;

        return 1;
    }

    if (stat("/dev/pmem_hwb", &pmem_file_info) < 0)
    {
        if (0 == android_atomic_cmpxchg(0, 1, &write_values_initiated))
        {
            error_code_produced_when_retreiving_pmem_file_info = errno;
            android_atomic_write(1, &error_occured_when_retreiving_pmem_file_info);
        }

        return 0;
    }

    if (0 == android_atomic_cmpxchg(0, 1, &write_values_initiated))
    {
        pmem_files_device = pmem_file_info.st_dev;
        pmem_files_serial_number = pmem_file_info.st_ino;
        android_atomic_write(1, &cached_values_present);
    }

    *device = pmem_file_info.st_dev;
    *serial_number = pmem_file_info.st_ino;

    return 1;
}

static int is_pmem_fd(int fd, int* is_pmem_fd_var)
{
    dev_t pmem_file_device;
    ino_t pmem_file_serial_number;

    struct stat file_info;

    if (!get_pmem_file_info(&pmem_file_device, &pmem_file_serial_number))
        return 0;

    if (fstat(fd, &file_info) < 0)
        return 0;

    if (file_info.st_dev == pmem_file_device &&
        file_info.st_ino == pmem_file_serial_number)
        *is_pmem_fd_var = 1;
    else
        *is_pmem_fd_var = 0;

    return 1;
}

/*****************************************************************************/

static pthread_mutex_t sMapLock = PTHREAD_MUTEX_INITIALIZER;

/*****************************************************************************/

int gralloc_create_handle_from_buffer_pmem(int fd, size_t size, size_t offset, void* base,
    native_handle_t** handle)
{
    struct hwmem_gralloc_buf_handle_t* hwmem_gralloc_buf_handle;
    int is_pmem_fd_var;

    if (!is_pmem_fd(fd, &is_pmem_fd_var))
        return -errno;
    if (!is_pmem_fd_var)
        return -EINVAL;

    hwmem_gralloc_buf_handle =
        (struct hwmem_gralloc_buf_handle_t*)native_handle_create(
        num_fds_in_hwmem_gralloc_buf_handle,
        num_ints_in_hwmem_gralloc_buf_handle);
    if (NULL == hwmem_gralloc_buf_handle)
    {
        return -ENOMEM;
    }

    hwmem_gralloc_buf_handle->fd = fd;
    hwmem_gralloc_buf_handle->flags = PRIV_FLAGS_USES_PMEM;
    hwmem_gralloc_buf_handle->type_identifier = hwmem_gralloc_buf_handle_type_identifier;
    hwmem_gralloc_buf_handle->type = GRALLOC_BUF_TYPE_PMEM;
    hwmem_gralloc_buf_handle->size = size;
    hwmem_gralloc_buf_handle->offset = offset;
    hwmem_gralloc_buf_handle->base_addr = (int)base + offset;
    hwmem_gralloc_buf_handle->lockState = LOCK_STATE_MAPPED;

    *handle = (native_handle_t*)hwmem_gralloc_buf_handle;

    return 0;
}

int gralloc_get_buf_allocator_handle_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t* hnd)
{
    return hnd->fd;
}

int gralloc_pin_buf_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t *hnd)
{
    if (hnd->type == GRALLOC_BUF_TYPE_PMEM) {
        struct pmem_region region;
        if (ioctl(hnd->fd, PMEM_GET_PHYS, &region) < 0)
            return -errno;
        return region.offset + hnd->offset;
    } else if (hnd->type == GRALLOC_BUF_TYPE_FB) {
        struct fb_fix_screeninfo fix_info;

        if (ioctl(hnd->fd, FBIOGET_FSCREENINFO, &fix_info) < 0)
            return -errno;
        return (int)fix_info.smem_start + hnd->offset;
    }

    return -EINVAL;
}

int gralloc_unpin_buf_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t *hnd)
{
    /* PMEM and FD does not have support for pinning in the first place... */
    return 0;
}

int gralloc_get_buf_offset_pmem(struct gralloc_module_t const* module,
    struct hwmem_gralloc_buf_handle_t* hnd)
{
    return hnd->offset;
}

int gralloc_register_buffer_pmem(gralloc_module_t const* module,
        struct hwmem_gralloc_buf_handle_t* hnd)
{
    // In this implementation, we don't need to do anything here

    /* NOTE: we need to initialize the buffer as not mapped/not locked
     * because it shouldn't when this function is called the first time
     * in a new process. Ideally these flags shouldn't be part of the
     * handle, but instead maintained in the kernel or at least
     * out-of-line
     */

    // if this handle was created in this process, then we keep it as is.
    if (hnd->pid != getpid()) {
        hnd->base_addr = 0;
        hnd->lockState  = 0;
        hnd->writeOwner = 0;
    }
    return 0;
}

int gralloc_unregister_buffer_pmem(gralloc_module_t const* module,
        struct hwmem_gralloc_buf_handle_t* hnd)
{
    /*
     * If the buffer has been mapped during a lock operation, it's time
     * to un-map it. It's an error to be here with a locked buffer.
     * NOTE: the framebuffer is handled differently and is never unmapped.
     */

    ALOGE_IF(hnd->lockState & LOCK_STATE_READ_MASK,
            "[unregister] handle %p still locked (state=%08x)",
            hnd, hnd->lockState);

    // never unmap buffers that were created in this process
    if (hnd->pid != getpid()) {
        if (hnd->lockState & LOCK_STATE_MAPPED) {
            gralloc_unmap(module, hnd);
        }
        hnd->base_addr = 0;
        hnd->lockState  = 0;
        hnd->writeOwner = 0;
    }
    return 0;
}

static int terminateBuffer(gralloc_module_t const* module,
        struct hwmem_gralloc_buf_handle_t* hnd)
{
    /*
     * If the buffer has been mapped during a lock operation, it's time
     * to un-map it. It's an error to be here with a locked buffer.
     */

    ALOGE_IF(hnd->lockState & LOCK_STATE_READ_MASK,
            "[terminate] handle %p still locked (state=%08x)",
            hnd, hnd->lockState);

    if (hnd->lockState & LOCK_STATE_MAPPED) {
        // this buffer was mapped, unmap it now
        if ((hnd->flags & PRIV_FLAGS_USES_PMEM) &&
            (hnd->pid == getpid())) {
            // ... unless it's a "master" pmem buffer, that is a buffer
            // mapped in the process it's been allocated.
            // (see gralloc_alloc_buffer())
        } else {
            gralloc_unmap(module, hnd);
        }
    }

    return 0;
}

int gralloc_lock_pmem(gralloc_module_t const* module,
        struct hwmem_gralloc_buf_handle_t* hnd, int usage,
        int l, int t, int w, int h,
        void** vaddr)
{
    int err = 0;
    int32_t current_value, new_value;
    int retry;

    do {
        current_value = hnd->lockState;
        new_value = current_value;

        if (current_value & LOCK_STATE_WRITE) {
            // already locked for write
            ALOGE("handle %p already locked for write", hnd);
            return -EBUSY;
        } else if (current_value & LOCK_STATE_READ_MASK) {
            // already locked for read
            if (usage & (GRALLOC_USAGE_SW_WRITE_MASK | GRALLOC_USAGE_HW_RENDER)) {
                ALOGE("handle %p already locked for read", hnd);
                return -EBUSY;
            } else {
                // this is not an error
                //ALOGD("%p already locked for read... count = %d",
                //        hnd, (current_value & ~(1<<31)));
            }
        }

        // not currently locked
        if (usage & (GRALLOC_USAGE_SW_WRITE_MASK | GRALLOC_USAGE_HW_RENDER)) {
            // locking for write
            new_value |= LOCK_STATE_WRITE;
        }
        new_value++;

        retry = android_atomic_cmpxchg(current_value, new_value,
                (volatile int32_t*)&hnd->lockState);
    } while (retry);

    if (new_value & LOCK_STATE_WRITE) {
        // locking for write, store the tid
        hnd->writeOwner = gettid();
    }

    if (usage & (GRALLOC_USAGE_SW_READ_MASK | GRALLOC_USAGE_SW_WRITE_MASK)) {
        if (!(current_value & LOCK_STATE_MAPPED)) {
            // we need to map for real
            pthread_mutex_t* const lock = &sMapLock;
            pthread_mutex_lock(lock);
            if (!(hnd->lockState & LOCK_STATE_MAPPED)) {
                err = gralloc_map(module, hnd, vaddr);
                if (err == 0) {
                    android_atomic_or(LOCK_STATE_MAPPED,
                            (volatile int32_t*)&(hnd->lockState));
                }
            }
            pthread_mutex_unlock(lock);
        }
        *vaddr = (void*)hnd->base_addr;
    }

    return err;
}

int gralloc_unlock_pmem(gralloc_module_t const* module,
        struct hwmem_gralloc_buf_handle_t* hnd)
{
    int32_t current_value, new_value;

    do {
        current_value = hnd->lockState;
        new_value = current_value;

        if (current_value & LOCK_STATE_WRITE) {
            // locked for write
            if (hnd->writeOwner == gettid()) {
                hnd->writeOwner = 0;
                new_value &= ~LOCK_STATE_WRITE;
            }
        }

        if ((new_value & LOCK_STATE_READ_MASK) == 0) {
            ALOGE("handle %p not locked", hnd);
            return -EINVAL;
        }

        new_value--;

    } while (android_atomic_cmpxchg(current_value, new_value,
            (volatile int32_t*)&hnd->lockState));

    return 0;
}

int gralloc_free_pmem(alloc_device_t* dev,
        struct hwmem_gralloc_buf_handle_t* hnd)
{
    if (hnd->flags & PRIV_FLAGS_FRAMEBUFFER) {
        // free this buffer
        struct hwmem_gralloc_module_t* m = (struct hwmem_gralloc_module_t*)
                dev->common.module;
        const size_t bufferSize = m->finfo.line_length * m->info.yres;
        int index = (hnd->base_addr - m->framebuffer->base_addr) / bufferSize;
        m->bufferMask &= ~(1<<index);
    } else {
#if HAVE_ANDROID_OS
        if (hnd->flags & PRIV_FLAGS_USES_PMEM) {
            if (hnd->fd >= 0) {
                struct pmem_region sub = { hnd->offset, hnd->size };
                int err = ioctl(hnd->fd, PMEM_UNMAP, &sub);
                ALOGE_IF(err<0, "PMEM_UNMAP failed (%s), "
                        "fd=%d, sub.offset=%i, sub.size=%i",
                        strerror(errno), hnd->fd, hnd->offset, hnd->size);
            }
        }
#endif // HAVE_ANDROID_OS
        terminateBuffer(NULL, hnd); /* NULL is ok here as the parameter is not used by terminateBuffer */
    }

    close(hnd->fd);
    free(hnd);
    return 0;
}
