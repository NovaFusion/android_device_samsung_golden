/*
 * Copyright (C) ST-Ericsson AB 2010
 *
 * \file    hwmem_gralloc.c
 * \brief   Gralloc implementation that uses hwmem
 * \author  Johan Mossberg <johan.xx.mossberg@stericsson.com> and
 * Fredrik Allansson <fredrik.allansson@stericsson.com> for ST-Ericsson
 *
 */


/*
 * IMPLEMENTATION SHORTCOMINGS
 * - The implementation assumes ints are 32 bit.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <errno.h>
#include <limits.h>
#include <linux/hwmem.h>
#include <string.h>
#include <stdarg.h>

#include <cutils/native_handle.h>

#include <cutils/atomic.h>
#include <cutils/log.h>
#include <cutils/threads.h>
#include <cutils/hashmap.h>
#include <hardware/hardware.h>
#include <hardware/gralloc.h>
#include <system/graphics.h>
#include "gralloc_stericsson_ext.h"

#include "hwmem_gralloc.h"
#include "hwmem_gralloc_pmem.h"
#include "hwmem_gralloc_framebuffer.h"


#if defined(INT_MAX) && INT_MAX != 2147483647
    #error "Hwmem gralloc only supports 32 bit ints"
#endif


/* Use these macro when it's probably a user error. */
#define LOG_USER_ERROR_ALWAYS(...) ALOGE("[User error] " __VA_ARGS__);

#ifndef LOG_USER_ERROR
#define LOG_USER_ERROR(...) ALOGE("[User error] " __VA_ARGS__);
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(...) ALOGE(__VA_ARGS__);
#endif

#define LOG_WARNING_ALWAYS(...) ALOGW(__VA_ARGS__);

#ifndef LOG_API_CALL
/* #define LOG_API_CALL(...) ALOGI(__VA_ARGS__); */
#define LOG_API_CALL(...)
#endif


enum buf_counters
{
    REGISTER_COUNTER,
    LOCK_COUNTER
};

struct buf_tg_info
{
    void *addr;
    int mmap_prot;
    int reg_cnt;
    int lock_cnt;
};


const int num_fds_in_hwmem_gralloc_buf_handle = 1;
const int num_ints_in_hwmem_gralloc_buf_handle = 14;
const int hwmem_gralloc_buf_handle_type_identifier = 0xA63B8FF0;


static const char hwmem_files_full_name[] = "/dev/" HWMEM_DEFAULT_DEVICE_NAME;

/* To ensure we don't get overflows in the width + height + format -> size calculations. Should
be isqrt(INT_MAX / max_bpp) but isqrt does not seem to exist. The value below is the result of
the previous calculation when INT_MAX = 2147483647 and max_bpp = 32 which is currently correct. */
static const int max_width_height = 8191;

static mutex_t buf_tg_infos_mutex = MUTEX_INITIALIZER;
static Hashmap *buf_handl_2_tg_info_map = NULL;


/* Gralloc module API functions */
static int gralloc_open_device(const struct hw_module_t* module, const char* name,
    struct hw_device_t** device);

static int gralloc_register_buffer(struct gralloc_module_t const* module, buffer_handle_t handle);
static int gralloc_unregister_buffer(struct gralloc_module_t const* module, buffer_handle_t handle);

static int gralloc_lock(struct gralloc_module_t const* module, buffer_handle_t handle, int usage,
    int left, int top, int width, int height, void** addr);
static int gralloc_unlock(struct gralloc_module_t const* module, buffer_handle_t handle);

static int gralloc_perform(struct gralloc_module_t const* module, int operation, ...);

static int gralloc_create_handle_from_buffer(int fd, size_t size, size_t offset, void* base,
    native_handle_t** handle);

static int gralloc_get_buf_allocator_handle(struct gralloc_module_t const* module,
    buffer_handle_t handle);

static int gralloc_pin_buf(struct gralloc_module_t const *module,
    buffer_handle_t handle);

static int gralloc_unpin_buf(struct gralloc_module_t const *module,
    buffer_handle_t handle);

static int gralloc_get_buf_offset(struct gralloc_module_t const* module,
    buffer_handle_t handle);

static int gralloc_get_buf_type(struct gralloc_module_t const* module,
    buffer_handle_t handle);

static int gralloc_get_buf_size(struct gralloc_module_t const* module,
    buffer_handle_t handle);

static int gralloc_get_buf_width(struct gralloc_module_t const* module,
    buffer_handle_t handle);

static int gralloc_get_buf_height(struct gralloc_module_t const* module,
    buffer_handle_t handle);

static int gralloc_get_buf_format(struct gralloc_module_t const* module,
    buffer_handle_t handle);

static int gralloc_get_buf_usage(struct gralloc_module_t const* module,
    buffer_handle_t handle);


static struct hw_module_methods_t hwmem_gralloc_module_methods = {
        open: gralloc_open_device
};

struct hwmem_gralloc_module_t HAL_MODULE_INFO_SYM =
{
    base:
    {
        common:
        {
            tag: HARDWARE_MODULE_TAG,
            version_major: 1,
            version_minor: 0,
            id: GRALLOC_HARDWARE_MODULE_ID,
            name: "Graphics Memory Allocator Module",
            author: "ST-Ericsson",
            methods: &hwmem_gralloc_module_methods
        },
        registerBuffer: gralloc_register_buffer,
        unregisterBuffer: gralloc_unregister_buffer,
        lock: gralloc_lock,
        unlock: gralloc_unlock,
        perform: gralloc_perform
    },
    type_identifier: (int)HWMEM_GRALLOC_MODULE_TYPE_IDENTIFIER,
    framebuffer: 0,
    flags: 0,
    numBuffers: 0,
    bufferMask: 0,
    lock: PTHREAD_MUTEX_INITIALIZER,
    currentBuffer: 0
};


/* Gralloc module alloc device API functions */
static int gralloc_alloc(struct alloc_device_t* device, int width, int height, int format,
    int usage, buffer_handle_t* handle, int* stride);
static int gralloc_free(struct alloc_device_t* device, buffer_handle_t handle);

static int open_alloc_device(const struct hwmem_gralloc_module_t* module,
    struct alloc_device_t** alloc_device);
static int gralloc_close_alloc_device(struct hw_device_t* device);

/* Hwmem API wrappers */
static int set_buffer_domain(struct hwmem_gralloc_buf_handle_t* buf, int usage, int left,
    int top, int width, int height);

/* Mmap */
static int mmap_buf_if_necessary(struct hwmem_gralloc_buf_handle_t *buf, void **addr,
    int *mmap_prot);
/* buf_tg_infos_mutex must be held when calling this function */
static void munmap_buf_if_necessary(struct hwmem_gralloc_buf_handle_t* buf);

/* Helpers */
static enum hwmem_access usage_2_hwmem_access(int usage);

static int does_lock_usage_match_alloc_usage(int lock_usage, int alloc_usage);
static int does_usage_match_mmap_prot(int usage, int mmap_prot);

static int get_format_bpp(int format, int* bpp);
static int is_non_planar_and_independent_pixel_format(int format);
static int get_format_width_and_height_alignment(int format, int usage,
    int* width_alignment, int* height_alignment);

static int set_up_hwmem_region(int left, int top, int width, int height, int buf_width,
    int buf_height, int buf_format, struct hwmem_region_us* hwmem_region);

static int align_up(int value, int alignment, int* result);

static int calc_buf_size(int width, int height, int format, int usage,
    int *actual_width, int *actual_height);

/*
 * Quicker method that just calculates the size as
 * "width * height * get_format_bpp(format) / 8" without any checks.
 */
static int get_buf_size(int width, int height, int format);

static int get_hwmem_file_info(dev_t* device, ino_t* serial_number);
static int is_hwmem_fd(int fd, int* is_hwmem_fd_var);

static enum hwmem_alloc_flags usage_2_hwmem_alloc_flags(int usage);
static enum hwmem_mem_type usage_2_hwmem_mem_type(int usage);

static int hwmem_access_2_mmap_prot(enum hwmem_access hwmem_access);
static int limit_mmap_prot_to_usage(int mmap_prot, int usage);

static int inc_buf_cnt(struct hwmem_gralloc_buf_handle_t* buf,
    enum buf_counters buf_counter);
static void dec_buf_cnt(struct hwmem_gralloc_buf_handle_t* buf,
    enum buf_counters buf_counter, int lock);
static int dec_buf_reg_cnt_if_last_usr(struct hwmem_gralloc_buf_handle_t* buf);

/* buf_tg_infos_mutex muste be held when calling these functions */
static int get_create_buf_tg_info(struct hwmem_gralloc_buf_handle_t* buf,
    struct buf_tg_info **buf_tg_info);
static struct buf_tg_info *get_buf_tg_info(struct hwmem_gralloc_buf_handle_t* buf);
static void remove_buf_tg_info(struct hwmem_gralloc_buf_handle_t* buf);
static int ensure_buf_handl_2_tg_info_map_exists(void);

static int ptr_2_int(void *ptr);
static bool are_ptrs_equal(void *ptr1, void *ptr2);


/* Gralloc module API functions */

static int gralloc_open_device(const struct hw_module_t* module, const char* name,
    hw_device_t** device)
{
    const struct hwmem_gralloc_module_t* gralloc;

    LOG_API_CALL("%s(%p, %s, %p)", __func__, module, name, device);

    if (NULL == name || NULL == device)
    {
        LOG_USER_ERROR("%s: NULL == name || NULL == device", __func__);
        return -EINVAL;
    }

    /* module_2_hwmem_gralloc_module does not write to module so the const to non
    const cast is ok */
    if (!module_2_hwmem_gralloc_module((struct gralloc_module_t const*)module,
        (struct hwmem_gralloc_module_t**)&gralloc))
        return -errno;

    if (0 == strcmp(name, GRALLOC_HARDWARE_GPU0))
    {
        if (!open_alloc_device(gralloc, (struct alloc_device_t**)device))
            return -errno;
    }
    else if (0 == strcmp(name, GRALLOC_HARDWARE_FB0))
        return fb_device_open(module, name, device);
    else
    {
        LOG_USER_ERROR("%s: Unknown device, %s", __func__, name);
        return -EINVAL;
    }

    return 0;
}

static int gralloc_register_buffer(gralloc_module_t const* module, buffer_handle_t handle)
{
    struct hwmem_gralloc_module_t* gralloc;
    struct hwmem_gralloc_buf_handle_t* buf;
    struct buf_tg_info *buf_tg_info;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
        !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_register_buffer_pmem(module, buf);

    if (!inc_buf_cnt(buf, REGISTER_COUNTER))
        return -errno;

    return 0;
}

static int gralloc_unregister_buffer(gralloc_module_t const* module, buffer_handle_t handle)
{
    struct hwmem_gralloc_module_t* gralloc;
    struct hwmem_gralloc_buf_handle_t* buf;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
        !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -EINVAL;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_unregister_buffer_pmem(module, buf);

    dec_buf_cnt(buf, REGISTER_COUNTER, 1);

    return 0;
}

static int gralloc_lock(gralloc_module_t const* module, buffer_handle_t handle, int usage,
    int left, int top, int width, int height, void** addr)
{
    struct hwmem_gralloc_module_t* gralloc;
    struct hwmem_gralloc_buf_handle_t* buf;

    int ret = 0;

    LOG_API_CALL("%s(%p, %p, %#x, %i, %i, %i, %i, %p)", __func__,
        module, (void *)handle, (unsigned int)usage, left, top, width, height,
        addr);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
        !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_lock_pmem(module, buf, usage, left, top, width, height, addr);

    /* TODO: Uncomment when applications learn to allocate buffers with correct usage */
    /*if (!does_lock_usage_match_alloc_usage(usage, buf->usage))
    {
        LOG_USER_ERROR("!does_lock_usage_match_alloc_usage");
        return -EINVAL;
    }*/

    if (usage & (GRALLOC_USAGE_SW_READ_MASK | GRALLOC_USAGE_SW_WRITE_MASK))
    {
        int mmap_prot = 0;

        if (NULL == addr)
        {
            LOG_USER_ERROR("%s: Software usage specified but addr == NULL", __func__);
            return -EINVAL;
        }

        if (!mmap_buf_if_necessary(buf, addr, &mmap_prot))
            return -errno;

        if (!does_usage_match_mmap_prot(usage, mmap_prot))
        {
            LOG_USER_ERROR("%s: !does_usage_match_mmap_prot(usage, mmap_prot)", __func__);
            return -EACCES;
        }
    }

    if (!set_buffer_domain(buf, usage, left, top, width, height))
        return -errno;

    if (!inc_buf_cnt(buf, LOCK_COUNTER))
        return -errno;

    return 0;
}

static int gralloc_unlock(gralloc_module_t const* module, buffer_handle_t handle)
{
    struct hwmem_gralloc_module_t* gralloc;
    struct hwmem_gralloc_buf_handle_t* buf;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
        !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_unlock_pmem(module, buf);

    dec_buf_cnt(buf, LOCK_COUNTER, 1);

    return 0;
}

static int gralloc_perform(struct gralloc_module_t const* module, int operation, ...)
{
    va_list args;
    va_start(args, operation);

    LOG_API_CALL("%s(%p, %i, ...)", __func__, module, operation);

    switch (operation)
    {
        case GRALLOC_MODULE_PERFORM_GET_BUF_ALLOCATOR_HANDLE:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_allocator_handle(module, handle);
        }

        case GRALLOC_MODULE_PERFORM_PIN_BUF:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_pin_buf(module, handle);
        }

        case GRALLOC_MODULE_PERFORM_UNPIN_BUF:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_unpin_buf(module, handle);
        }

        case GRALLOC_MODULE_PERFORM_GET_BUF_OFFSET:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_offset(module, handle);
        }

        case GRALLOC_MODULE_PERFORM_GET_BUF_TYPE:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_type(module, handle);
        }

        case GRALLOC_MODULE_PERFORM_GET_BUF_SIZE:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_size(module, handle);
        }

        case GRALLOC_MODULE_PERFORM_GET_BUF_WIDTH:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_width(module, handle);
        }
        case GRALLOC_MODULE_PERFORM_GET_BUF_HEIGHT:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_height(module, handle);
        }
        case GRALLOC_MODULE_PERFORM_GET_BUF_FORMAT:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_format(module, handle);
        }
        case GRALLOC_MODULE_PERFORM_GET_BUF_USAGE:
        {
            buffer_handle_t handle = va_arg(args, buffer_handle_t);
            va_end(args);

            return gralloc_get_buf_usage(module, handle);
        }
        case GRALLOC_MODULE_PERFORM_COMPOSITION_COMPLETE:
        {
            return fb_compositionComplete();
        }


        default:
            va_end(args);

            LOG_USER_ERROR("%s: Unknown operation, %i", __func__, operation);
            return -EINVAL;
    }
}

static int gralloc_create_handle_from_buffer(int fd, size_t size, size_t offset, void* base,
    native_handle_t** handle)
{
    int is_hwmem_fd_var;
    struct hwmem_get_info_request hwmem_get_info_request;
    struct hwmem_gralloc_buf_handle_t* hwmem_gralloc_buf_handle;

    LOG_API_CALL("%s(%i, %u, %u, %p, %p)", __func__, fd, size, ofset, base, handle);

    if (offset > (unsigned int)INT_MAX || size > (unsigned int)INT_MAX || size == 0 || NULL == handle)
    {
        LOG_USER_ERROR("%s: offset > (unsigned int)INT_MAX || size > (unsigned int)INT_MAX || size == 0 || NULL == handle", __func__);
        return -EINVAL;
    }

    if (!is_hwmem_fd(fd, &is_hwmem_fd_var))
        return -errno;
    if (!is_hwmem_fd_var)
    {
        LOG_USER_ERROR("%s: !is_hwmem_fd_var", __func__);
        return -EINVAL;
    }

    hwmem_get_info_request.id = 0;
    if (ioctl(fd, HWMEM_GET_INFO_IOC, &hwmem_get_info_request) < 0)
    {
        LOG_USER_ERROR("%s: HWMEM_GET_INFO_IOC failed, %s", __func__, strerror(errno));
        return -errno;
    }

    if (offset + size > hwmem_get_info_request.size)
    {
        LOG_USER_ERROR("%s: offset + buf_size > hwmem_get_info_request.size", __func__);
        return -EINVAL;
    }

    hwmem_gralloc_buf_handle =
        (struct hwmem_gralloc_buf_handle_t*)native_handle_create(
        num_fds_in_hwmem_gralloc_buf_handle, num_ints_in_hwmem_gralloc_buf_handle);
    if (NULL == hwmem_gralloc_buf_handle)
    {
        LOG_ERROR("%s: Out of memory!", __func__);
        return -ENOMEM;
    }

    hwmem_gralloc_buf_handle->fd = fd;
    hwmem_gralloc_buf_handle->type_identifier = hwmem_gralloc_buf_handle_type_identifier;
    hwmem_gralloc_buf_handle->width = -1;
    hwmem_gralloc_buf_handle->height = -1;
    hwmem_gralloc_buf_handle->format = -1;
    hwmem_gralloc_buf_handle->usage = GRALLOC_USAGE_SW_READ_MASK |
        GRALLOC_USAGE_SW_WRITE_MASK | GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_RENDER |
        GRALLOC_USAGE_HW_2D;
    hwmem_gralloc_buf_handle->addr = (int)((char *)base + offset);
    hwmem_gralloc_buf_handle->offset = (int)offset;
    hwmem_gralloc_buf_handle->size = (int)size;
    hwmem_gralloc_buf_handle->type = GRALLOC_BUF_TYPE_HWMEM_CONTIGUOUS;

    *handle = (native_handle_t*)hwmem_gralloc_buf_handle;

    return 0;
}

static int gralloc_get_buf_allocator_handle(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_module_t* gralloc;
    struct hwmem_gralloc_buf_handle_t* buf;

    int buf_hwmem_name;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
        !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_get_buf_allocator_handle_pmem(module, buf);

    buf_hwmem_name = ioctl(buf->fd, HWMEM_EXPORT_IOC, 0);
    if (buf_hwmem_name < 0)
    {
        LOG_ERROR("%s: HWMEM_EXPORT_IOC failed, %s", __func__, strerror(errno));
    }

    return buf_hwmem_name;
}

static int gralloc_pin_buf(struct gralloc_module_t const *module,
        buffer_handle_t handle)
{
    struct hwmem_gralloc_module_t *gralloc;
    struct hwmem_gralloc_buf_handle_t *buf;

    struct hwmem_pin_request pin;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
            !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_pin_buf_pmem(module, buf);

    pin.id = 0;
    if (ioctl(buf->fd, HWMEM_PIN_IOC, &pin) < 0) {
        ALOGE("%s: Could not pin hwmem buffer!", __func__);
        return -errno;
    }

    return (int)pin.phys_addr + buf->offset;
}

static int gralloc_unpin_buf(struct gralloc_module_t const* module,
        buffer_handle_t handle)
{
    struct hwmem_gralloc_module_t *gralloc;
    struct hwmem_gralloc_buf_handle_t *buf;

    struct hwmem_pin_request pin;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
            !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_unpin_buf_pmem(module, buf);

    if (ioctl(buf->fd, HWMEM_UNPIN_IOC, 0) < 0) {
        ALOGE("%s: Could not unpin hwmem buffer!", __func__);
        return -errno;
    }

    return 0;
}

static int gralloc_get_buf_offset(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_module_t* gralloc;
    struct hwmem_gralloc_buf_handle_t* buf;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!module_2_hwmem_gralloc_module(module, &gralloc) ||
        !handle_2_hwmem_gralloc_handle(handle, &buf))
        return -EINVAL;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_get_buf_offset_pmem(module, buf);

    return buf->offset;
}

static int gralloc_get_buf_type(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_buf_handle_t* buf;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    return buf->type;
}

static int gralloc_get_buf_size(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_buf_handle_t* buf;
    int buf_size;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    /* If this is a wrapped handle, just return the stored size */
    if (buf->size != -1)
        return buf->size;

    buf_size = get_buf_size(buf->width, buf->height, buf->format);
    if (buf_size < 0)
        return -errno;

    return buf_size;
}

static int gralloc_get_buf_width(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_buf_handle_t* buf;
    int buf_size;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    return buf->width;
}

static int gralloc_get_buf_height(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_buf_handle_t* buf;
    int buf_size;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    return buf->height;
}

static int gralloc_get_buf_format(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_buf_handle_t* buf;
    int buf_size;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    return buf->format;
}

static int gralloc_get_buf_usage(struct gralloc_module_t const* module,
    buffer_handle_t handle)
{
    struct hwmem_gralloc_buf_handle_t* buf;
    int buf_size;

    LOG_API_CALL("%s(%p, %p)", __func__, module, (void *)handle);

    if (!handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    return buf->usage;
}


/* Gralloc module alloc device API functions */

static int gralloc_alloc(struct alloc_device_t* device, int width, int height, int format,
    int usage, buffer_handle_t* handle, int* stride)
{
    int ret = 0;
    int last_result;
    int hwmem_fd = -1;
    int actual_width;
    int actual_height;
    int buf_size;
    struct hwmem_alloc_request hwmem_alloc_request;
    struct hwmem_gralloc_buf_handle_t* buf_handle = NULL;

    LOG_API_CALL("%s(%p, %i, %i, %#x, %#x, %p, %p)", __func__, device,
        width, height, (unsigned int)format, (unsigned int)usage,
        handle, stride);

    if (NULL == device || NULL == handle)
    {
        LOG_USER_ERROR("%s: NULL == device || NULL == handle", __func__);
        return -EINVAL;
    }

    if (!(usage & GRALLOC_USAGE_HW_MASK))
    {
        LOG_WARNING_ALWAYS("%s: Gralloc is used to alloc software only buffers!", __func__);
    }

    buf_size = calc_buf_size(width, height, format, usage, &actual_width, &actual_height);
    if (buf_size < 0)
        return -errno;

    if (usage & GRALLOC_USAGE_HW_FB)
    {
        int handled;

        last_result = gralloc_alloc_framebuffer(device, buf_size, usage, handle, stride, &handled);
        if (last_result < 0)
        {
            LOG_ERROR("%s: gralloc_alloc_framebuffer (google) failed, %s", __func__, strerror(-last_result));
            return last_result;
        }

        if (handled)
            return 0;
        else
            usage &= ~GRALLOC_USAGE_HW_FB;
    }

    if (!is_non_planar_and_independent_pixel_format(format))
        usage |= GRALLOC_USAGE_HW_2D;

    hwmem_fd = open(hwmem_files_full_name, O_RDWR);
    if (hwmem_fd < 0)
    {
        LOG_ERROR("%s: open failed, %s", __func__, strerror(errno));
        return -errno;
    }

    hwmem_alloc_request.size = (unsigned int)buf_size;
    hwmem_alloc_request.flags = usage_2_hwmem_alloc_flags(usage);
    hwmem_alloc_request.default_access =
        /* TODO: Uncomment when applications learn to allocate buffers with correct usage */
        /* usage_2_hwmem_access(usage); */
        HWMEM_ACCESS_READ | HWMEM_ACCESS_WRITE;
    hwmem_alloc_request.mem_type = usage_2_hwmem_mem_type(usage);
    if (ioctl(hwmem_fd, HWMEM_ALLOC_FD_IOC, &hwmem_alloc_request) < 0)
    {
        LOG_ERROR("%s: HWMEM_ALLOC_IOC failed, %s", __func__, strerror(errno));
        ret = -errno;
        goto alloc_failed;
    }

    buf_handle =
        (struct hwmem_gralloc_buf_handle_t*)native_handle_create(
        num_fds_in_hwmem_gralloc_buf_handle, num_ints_in_hwmem_gralloc_buf_handle);
    if (NULL == buf_handle)
    {
        LOG_ERROR("%s: Out of memory!", __func__);
        ret = -ENOMEM;
        goto native_handle_create_failed;
    }

    buf_handle->fd = hwmem_fd;
    buf_handle->type_identifier = hwmem_gralloc_buf_handle_type_identifier;
    buf_handle->width = actual_width;
    buf_handle->height = actual_height;
    buf_handle->format = format;
    buf_handle->usage = usage;
    buf_handle->offset = 0;
    buf_handle->size = -1;
    if (hwmem_alloc_request.mem_type == HWMEM_MEM_SCATTERED_SYS)
    {
        buf_handle->type = GRALLOC_BUF_TYPE_HWMEM_SCATTERED;
    }
    else
    {
        buf_handle->type = GRALLOC_BUF_TYPE_HWMEM_CONTIGUOUS;
    }

    if (!inc_buf_cnt(buf_handle, REGISTER_COUNTER))
    {
        ret = -errno;
        goto inc_buf_cnt_failed;
    }

    *handle = (buffer_handle_t)buf_handle;

    if (stride != NULL)
    {
        *stride = actual_width;
    }

    goto out;

inc_buf_cnt_failed:
    last_result = native_handle_delete((native_handle_t *)buf_handle);
    if (last_result < 0)
    {
        LOG_ERROR("%s: native_handle_delete, %s. Resource leak!", __func__, strerror(-last_result));
    }
native_handle_create_failed:
alloc_failed:
    if (close(hwmem_fd) < 0)
    {
        LOG_ERROR("%s: close failed, %s. Resource leak!", __func__, strerror(errno));
    }

out:
    return ret;
}

static int gralloc_free(struct alloc_device_t* device, buffer_handle_t handle)
{
    int last_ret;
    struct hwmem_gralloc_buf_handle_t* buf;
    struct buf_tg_info *buf_tg_info;

    LOG_API_CALL("%s(%p, %p)", __func__, device, (void *)handle);

    if (!handle_2_hwmem_gralloc_handle(handle, &buf))
        return -errno;

    if ((buf->type == GRALLOC_BUF_TYPE_PMEM) || (buf->type == GRALLOC_BUF_TYPE_FB))
        return gralloc_free_pmem(device, buf);

    if (!dec_buf_reg_cnt_if_last_usr(buf))
        return -errno;

    last_ret = native_handle_close((const native_handle_t*)buf);
    if (last_ret < 0)
    {
        LOG_ERROR("%s: native_handle_close failed, %s. Resource leak!", __func__, strerror(-last_ret));
    }

    last_ret = native_handle_delete((native_handle_t*)buf);
    if (last_ret < 0)
    {
        LOG_ERROR("%s: native_handle_delete failed, %s. Resource leak!", __func__, strerror(-last_ret));
    }

    return 0;
}

static int open_alloc_device(const struct hwmem_gralloc_module_t* module,
    struct alloc_device_t** alloc_device)
{
    const struct hwmem_gralloc_module_t* gralloc;

    LOG_API_CALL("%s(%p, %p)", __func__, module, alloc_device);

    /* module_2_hwmem_gralloc_module does not write to module so the const to non
    const cast is ok */
    if (!module_2_hwmem_gralloc_module((struct gralloc_module_t const*)module,
        (struct hwmem_gralloc_module_t**)&gralloc))
        return 0;

    if (NULL == alloc_device)
    {
        LOG_USER_ERROR("%s: NULL == alloc_device", __func__);
        errno = EINVAL;
        return 0;
    }

    *alloc_device = (alloc_device_t*)malloc(sizeof(alloc_device_t));
    if (NULL == *alloc_device)
    {
        LOG_ERROR("%s: Out of memory!", __func__);
        errno = ENOMEM;
        return 0;
    }

    memset(*alloc_device, 0, sizeof(**alloc_device));

    (*alloc_device)->common.tag = HARDWARE_DEVICE_TAG;
    (*alloc_device)->common.version = 1;
    /* Dangerous cast but it can't be avoided given the current gralloc API. */
    (*alloc_device)->common.module = (hw_module_t*)gralloc;
    (*alloc_device)->common.close = gralloc_close_alloc_device;

    (*alloc_device)->alloc = gralloc_alloc;
    (*alloc_device)->free = gralloc_free;

    return 1;
}

static int gralloc_close_alloc_device(struct hw_device_t* device)
{
    LOG_API_CALL("%s(%p)", __func__, device);

    if (NULL == device)
    {
        LOG_USER_ERROR("%s: NULL == device", __func__);
        return -EINVAL;
    }

    /* It would be nice to free all the device's buffers here in case the application
    forgot. The problem with that approach is the native_handle_close/delete functions.
    It's not unlikely that an application erronously closes a handle with
    native_handle_close/delete instead of free and in that case the cleanup code here
    will try to free an invalid handle which will probably crash the application. Not
    cleaning up here on the other hand might lead to temporary resource leaks (till the
    application dies) so the choice is between possibly crashing a faulty application or
    letting it temporarily leak some resources, I choose the second alternative. If
    problems arise we'll have to re-evaluate this choice. */

    free(device);

    return 0;
}


/* Hwmem API wrappers */

static int set_buffer_domain(struct hwmem_gralloc_buf_handle_t* buf,
    int usage, int left, int top, int width, int height)
{
    struct hwmem_set_domain_request request;

    request.id = 0;
    request.access = usage_2_hwmem_access(usage);

    if (buf->size != -1)
    {
        /* We don't have enough information for a partial domain switch in the wrapping
        handle case so we switch domain for the entire buffer. */

        request.region.offset = buf->offset;
        request.region.count = 1;
        request.region.start = 0;
        request.region.end = buf->size;
        request.region.size = buf->size;
    }
    else
    {
        if (!set_up_hwmem_region(left, top, width, height, buf->width, buf->height,
            buf->format, &request.region))
            return 0;
    }

    if (usage & (GRALLOC_USAGE_SW_READ_MASK | GRALLOC_USAGE_SW_WRITE_MASK)) {
        if (ioctl(buf->fd, HWMEM_SET_CPU_DOMAIN_IOC, &request) < 0)
        {
            LOG_ERROR("%s: HWMEM_SET_DOMAIN_IOC failed, %s", __func__, strerror(errno));
            return 0;
        }
    } else {
        if (ioctl(buf->fd, HWMEM_SET_SYNC_DOMAIN_IOC, &request) < 0)
        {
            LOG_ERROR("%s: HWMEM_SET_DOMAIN_IOC failed, %s", __func__, strerror(errno));
            return 0;
        }
    }

    return 1;
}


/* Mmap */

static int mmap_buf_if_necessary(struct hwmem_gralloc_buf_handle_t *buf, void **addr,
    int *mmap_prot)
{
    int ret = 1;
    struct buf_tg_info *buf_tg_info;
    struct hwmem_get_info_request buf_info;

    /* Wrapper handles are not memory mapped by gralloc */
    if (buf->size != -1)
    {
        if ((int)NULL == buf->addr)
        {
            LOG_USER_ERROR("%s, Wrapper handle is not memory mapped", __func__);
            errno = -ENOMSG;
            return 0;
        }

        *addr = (void *)buf->addr;
        *mmap_prot = PROT_READ | PROT_WRITE;

        return 1;
    }

    mutex_lock(&buf_tg_infos_mutex);

    buf_tg_info = get_buf_tg_info(buf);
    if (NULL == buf_tg_info)
    {
        LOG_USER_ERROR("%s: Memory mapping unregistered buffer", __func__);
        errno = ENOMSG;
        goto no_buf_tg_info;
    }

    /* Check if we are already mapped */
    if (buf_tg_info->addr != NULL)
    {
        *addr = buf_tg_info->addr;
        *mmap_prot = buf_tg_info->mmap_prot;

        goto out;
    }

    buf_info.id = 0;
    if (ioctl(buf->fd, HWMEM_GET_INFO_IOC, &buf_info) < 0)
    {
        LOG_ERROR("%s: HWMEM_GET_INFO_IOC failed, %s", __func__, strerror(errno));
        goto hwmem_get_info_failed;
    }

    buf_tg_info->mmap_prot = hwmem_access_2_mmap_prot(buf_info.access);
    /* TODO: Uncomment when applications learns to allocate buffers with correct usage */
    /*buf_tg_info->mmap_prot = limit_mmap_prot_to_usage(buf_tg_info->mmap_prot,
        buf->usage);*/

    buf_tg_info->addr = mmap(NULL, buf_info.size, buf_tg_info->mmap_prot, MAP_SHARED,
        buf->fd, 0);
    if (MAP_FAILED == buf_tg_info->addr)
    {
        LOG_ERROR("%s: mmap failed, %s", __func__, strerror(errno));
        goto mmap_failed;
    }

    *addr = buf_tg_info->addr;
    *mmap_prot = buf_tg_info->mmap_prot;

    goto out;

mmap_failed:
    buf_tg_info->addr = NULL;
hwmem_get_info_failed:
no_buf_tg_info:
    ret = 0;

out:
    mutex_unlock(&buf_tg_infos_mutex);

    return ret;
}

/* buf_tg_infos_mutex must be held when calling this function */
static void munmap_buf_if_necessary(struct hwmem_gralloc_buf_handle_t* buf)
{
    struct buf_tg_info *buf_tg_info;
    struct hwmem_get_info_request buf_info;

    buf_tg_info = get_buf_tg_info(buf);
    if (NULL == buf_tg_info)
    {
        LOG_USER_ERROR("%s: Unmapping unregistered buffer", __func__);
        return;
    }

    if (NULL == buf_tg_info->addr)
        return;

    buf_info.id = 0;
    if (ioctl(buf->fd, HWMEM_GET_INFO_IOC, &buf_info) < 0)
    {
        LOG_ERROR("%s: HWMEM_GET_INFO_IOC failed, %s. Resource leak!", __func__, strerror(errno));
        return;
    }

    if (munmap(buf_tg_info->addr, buf_info.size) < 0)
    {
        LOG_ERROR("%s: munmap failed, %s. Resource leak!", __func__, strerror(errno));
        return;
    }

    buf_tg_info->addr = NULL;
}


/* Helpers */

int module_2_hwmem_gralloc_module(struct gralloc_module_t const* module,
    struct hwmem_gralloc_module_t** gralloc_out)
{
    struct hwmem_gralloc_module_t* gralloc =
            (struct hwmem_gralloc_module_t *)module;

    if (NULL == gralloc || gralloc->base.common.tag != HARDWARE_MODULE_TAG ||
        gralloc->type_identifier != (int)HWMEM_GRALLOC_MODULE_TYPE_IDENTIFIER)
    {
        LOG_USER_ERROR("%s, Invalid module", __func__);
        errno = EINVAL;
        return 0;
    }

    *gralloc_out = gralloc;

    return 1;

}

int handle_2_hwmem_gralloc_handle(buffer_handle_t handle,
        struct hwmem_gralloc_buf_handle_t **buf_out)
{
    struct hwmem_gralloc_buf_handle_t *buf =
            (struct hwmem_gralloc_buf_handle_t*)handle;

    if (NULL == buf || buf->base.version != sizeof(native_handle_t) ||
        buf->base.numFds != num_fds_in_hwmem_gralloc_buf_handle ||
        buf->base.numInts != num_ints_in_hwmem_gralloc_buf_handle ||
        buf->type_identifier != hwmem_gralloc_buf_handle_type_identifier)
    {
        LOG_USER_ERROR("%s: Invalid handle", __func__);
        errno = EINVAL;
        return 0;
    }

    *buf_out = buf;

    return 1;
}

static enum hwmem_access usage_2_hwmem_access(int usage)
{
    enum hwmem_access hwmem_access = (enum hwmem_access)0;

    if (usage & GRALLOC_USAGE_SW_READ_MASK)
        hwmem_access |= HWMEM_ACCESS_READ;
    if (usage & GRALLOC_USAGE_SW_WRITE_MASK || usage & GRALLOC_USAGE_HW_RENDER)
        hwmem_access |= HWMEM_ACCESS_WRITE;
    if (usage & (GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_2D | GRALLOC_USAGE_HW_FB))
        hwmem_access |= HWMEM_ACCESS_READ | HWMEM_ACCESS_WRITE;

    return hwmem_access;
}

static int does_lock_usage_match_alloc_usage(int lock_usage, int alloc_usage)
{
    int lock_sw_read = lock_usage & GRALLOC_USAGE_SW_READ_MASK;
    int lock_sw_write = lock_usage & GRALLOC_USAGE_SW_WRITE_MASK;
    int alloc_sw_read = alloc_usage & GRALLOC_USAGE_SW_READ_MASK;
    int alloc_sw_write = alloc_usage & GRALLOC_USAGE_SW_WRITE_MASK;

    if ((lock_sw_read && !alloc_sw_read) ||
        (lock_sw_write && !alloc_sw_write) ||
        ((lock_usage & GRALLOC_USAGE_HW_MASK) & (alloc_usage & GRALLOC_USAGE_HW_MASK)) != (lock_usage & GRALLOC_USAGE_HW_MASK))
        return 0;
    else
        return 1;
}

static int does_usage_match_mmap_prot(int usage, int mmap_prot)
{
    int usage_sw_read = usage & GRALLOC_USAGE_SW_READ_MASK;
    int usage_sw_write = usage & GRALLOC_USAGE_SW_WRITE_MASK;
    int mmap_prot_read = mmap_prot & PROT_READ;
    int mmap_prot_write = mmap_prot & PROT_WRITE;

    if ((usage_sw_read && !mmap_prot_read) ||
        (usage_sw_write && !mmap_prot_write))
        return 0;
    else
        return 1;
}

static int get_format_bpp(int format, int* bpp)
{
    switch (format)
    {
        case HAL_PIXEL_FORMAT_RGBA_8888:
        case HAL_PIXEL_FORMAT_RGBX_8888:
        case HAL_PIXEL_FORMAT_BGRA_8888:
            *bpp = 32;
            break;

        case HAL_PIXEL_FORMAT_RGB_888:
            *bpp = 24;
            break;

        case HAL_PIXEL_FORMAT_RGB_565:
        case HAL_PIXEL_FORMAT_RGBA_5551:
        case HAL_PIXEL_FORMAT_RGBA_4444:
        case HAL_PIXEL_FORMAT_YCbCr_422_SP:
        case HAL_PIXEL_FORMAT_YCbCr_422_P:
        case HAL_PIXEL_FORMAT_YCbCr_422_I:
        case HAL_PIXEL_FORMAT_CbYCrY_422_I:
        case HAL_PIXEL_FORMAT_YCrCb_422_SP:
        case HAL_PIXEL_FORMAT_YCrCb_422_P:
            *bpp = 16;
            break;

        case HAL_PIXEL_FORMAT_YCBCR42XMBN: /* Interpreted as HAL_PIXEL_FORMAT_YCBCR420MBN */
        case HAL_PIXEL_FORMAT_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_YCbCr_420_P:
        case HAL_PIXEL_FORMAT_YCbCr_420_I:
        case HAL_PIXEL_FORMAT_CbYCrY_420_I:
        case HAL_PIXEL_FORMAT_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_YCrCb_420_P:
        case HAL_PIXEL_FORMAT_YV12:
            *bpp = 12;
            break;

        default:
            LOG_USER_ERROR("%s: Unknown format, %i", __func__, format);
            errno = EINVAL;
            return 0;
    }

    return 1;
}

static int is_non_planar_and_independent_pixel_format(int format)
{
    switch (format)
    {
        case HAL_PIXEL_FORMAT_RGBA_8888:
        case HAL_PIXEL_FORMAT_RGBX_8888:
        case HAL_PIXEL_FORMAT_BGRA_8888:
        case HAL_PIXEL_FORMAT_RGB_888:
        case HAL_PIXEL_FORMAT_RGB_565:
        case HAL_PIXEL_FORMAT_RGBA_5551:
        case HAL_PIXEL_FORMAT_RGBA_4444:
            return 1;
    }

    return 0;
}

static int get_format_width_and_height_alignment(int format, int usage,
    int* width_alignment, int* height_alignment)
{
    switch (format) {
        case HAL_PIXEL_FORMAT_RGBX_8888:
	    if (usage & (GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_COMPOSER | GRALLOC_USAGE_HW_RENDER))
	        *width_alignment = 2;
	    else
	        *width_alignment = 1;
	    *height_alignment = 1;
            break;

        case HAL_PIXEL_FORMAT_RGBA_8888:
        case HAL_PIXEL_FORMAT_BGRA_8888:
	    if (usage & (GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_COMPOSER | GRALLOC_USAGE_HW_RENDER))
                *width_alignment = 2;
            else
                *width_alignment = 1;
            *height_alignment = 1;
            break;

        case HAL_PIXEL_FORMAT_YCbCr_422_SP:
        case HAL_PIXEL_FORMAT_YCbCr_422_P:
        case HAL_PIXEL_FORMAT_YCbCr_422_I:
        case HAL_PIXEL_FORMAT_CbYCrY_422_I:
        case HAL_PIXEL_FORMAT_YCrCb_422_SP:
        case HAL_PIXEL_FORMAT_YCrCb_422_P:
            *width_alignment = 2;
            *height_alignment = 1;
            break;

        case HAL_PIXEL_FORMAT_YCbCr_420_SP_TILED:
        case HAL_PIXEL_FORMAT_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_YCbCr_420_P:
        case HAL_PIXEL_FORMAT_YCbCr_420_I:
        case HAL_PIXEL_FORMAT_YCrCb_420_SP_TILED:
        case HAL_PIXEL_FORMAT_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_YCrCb_420_P:
        case HAL_PIXEL_FORMAT_CbYCrY_420_I:
            *width_alignment = 2;
            *height_alignment = 2;
            break;

        case HAL_PIXEL_FORMAT_RGB_565:
        case HAL_PIXEL_FORMAT_RGBA_5551:
        case HAL_PIXEL_FORMAT_RGBA_4444:
            if (usage & (GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_RENDER))
                *width_alignment = 4;
            else
                *width_alignment = 1;
            *height_alignment = 1;
            break;

        case HAL_PIXEL_FORMAT_RGB_888:
            if (usage & (GRALLOC_USAGE_HW_TEXTURE | GRALLOC_USAGE_HW_COMPOSER | GRALLOC_USAGE_HW_RENDER))
                *width_alignment = 8;
            else if (usage & GRALLOC_USAGE_HW_2D)
                *width_alignment = 4;
            else
                *width_alignment = 1;
            *height_alignment = 1;
            break;

        case HAL_PIXEL_FORMAT_YCBCR42XMBN:
            *width_alignment = 16;
            *height_alignment = 16;
            break;

        case HAL_PIXEL_FORMAT_YV12:
            *width_alignment = 16;
            *height_alignment = 2;
            break;

        default:
            LOG_USER_ERROR("%s: Unknown format, %i", __func__, format);
            errno = EINVAL;
            return 0;
    }

    return 1;
}

static int set_up_hwmem_region(int left, int top, int width, int height, int buf_width,
    int buf_height, int buf_format, struct hwmem_region_us* hwmem_region)
{
    int buf_format_bpp;
    int actual_left, actual_top, actual_width, actual_height;
    int pitch;

    if (left < 0 || top < 0 || width < 0 || height < 0 || left > INT_MAX - width ||
        left + width > buf_width || top > INT_MAX - height ||
        top + height > buf_height)
    {
        LOG_USER_ERROR("%s: left < 0 || top < 0 || width < 0 || height < 0 || left > INT_MAX - width || left + width > buf_width || top > INT_MAX - height || top + height > buf_height", __func__);
        errno = EINVAL;
        return 0;
    }

    if (!get_format_bpp(buf_format, &buf_format_bpp))
        return 0;

    if (is_non_planar_and_independent_pixel_format(buf_format))
    {
        actual_left = left;
        actual_top = top;
        actual_width = width;
        actual_height = height;
    }
    else
    {
        /* TODO: Locking entire buffer as a quick safe solution. In the future
        we should lock less to avoid unecessary cache syncing. Pixel interleaved
        YCbCr formats should be quite easy, just align start and stop points on 2. */
        actual_left = 0;
        actual_top = 0;
        actual_width = buf_width;
        actual_height = buf_height;
    }

    pitch = (buf_width * buf_format_bpp) / 8;

    hwmem_region->offset = (unsigned int)(actual_top * pitch);
    hwmem_region->count = (unsigned int)actual_height;
    hwmem_region->start = (unsigned int)((actual_left * buf_format_bpp) / 8);
    hwmem_region->end = (unsigned int)
        (((actual_left + actual_width) * buf_format_bpp) / 8);
    hwmem_region->size = (unsigned int)pitch;

    return 1;
}

static int align_up(int value, int alignment, int* result)
{
    int remainder = value % alignment;
    int value_to_add;

    /* We never align negative values so there is no point dealing with that */
    if (value < 0 || alignment < 0)
    {
        LOG_ERROR("%s: value < 0 || alignment < 0", __func__);
        errno = EINVAL;
        return 0;
    }

    if (remainder > 0)
        value_to_add = alignment - remainder;
    else
        value_to_add = 0;

    if (value > INT_MAX - value_to_add)
    {
        /* Will result in overflow */
        LOG_ERROR("%s: value > INT_MAX - value_to_add", __func__);
        errno = ERANGE;
        return 0;
    }

    *result = value + value_to_add;

    return 1;
}

static int calc_buf_size(int width, int height, int format, int usage,
    int* actual_width, int* actual_height)
{
    int bpp;
    int format_width_alignment;
    int format_height_alignment;

    if (width < 0 || height < 0)
    {
        LOG_USER_ERROR("%s: width < 0 || height < 0", __func__);
        errno = EINVAL;
        return -1;
    }

    if (!get_format_bpp(format, &bpp))
        return -1;

    if (!get_format_width_and_height_alignment(format, usage,
        &format_width_alignment, &format_height_alignment))
        return -1;

    if (!align_up(width, format_width_alignment, actual_width) ||
        !align_up(height, format_height_alignment, actual_height))
        return -1;

    if (*actual_width > max_width_height || *actual_height > max_width_height)
    {
        LOG_USER_ERROR("%s: actual_width > max_width_height || actual_height > max_width_height", __func__);
        errno = EINVAL;
        return -1;
    }

    if (format == HAL_PIXEL_FORMAT_YV12) {
        int uv_stride;
        int mean_stride;
        /*
         * Y stride needs to be aligned to 16 pixels, and
         * the U/V stride needs to be half of Y stride
         * aligned to 16. In other words:
         *   y_size = stride * height
         *   c_size = ALIGN(stride/2, 16) * height/2
         *   size = y_size + c_size * 2
         * However, b2r2 does not recognize the YV12 format
         * being any different than the YVU420P which requires
         * the U/V stride to be half of the Y stride, and
         * therefore we align the pitch to 32.
         */
        align_up(*actual_width >> 1, 16, &uv_stride);
        mean_stride = uv_stride + (*actual_width >> 1);
        return (mean_stride * (*actual_height) * bpp) >> 3;
    } else {
        return ((*actual_width) * (*actual_height) * bpp) >> 3;
    }
}

static int get_buf_size(int width, int height, int format)
{
    int bpp;

    if (!get_format_bpp(format, &bpp))
        return -1;

    return (width * height * bpp) / 8;
}

static int get_hwmem_file_info(dev_t* device, ino_t* serial_number)
{
    static int write_values_initiated = 0;
    static int error_occured_when_retreiving_hwmem_file_info = 0;
    static int error_code_produced_when_retreiving_hwmem_file_info;
    static int cached_values_present = 0;
    static dev_t hwmem_files_device;
    static ino_t hwmem_files_serial_number;

    struct stat hwmem_file_info;

    if (error_occured_when_retreiving_hwmem_file_info)
    {
        errno = error_code_produced_when_retreiving_hwmem_file_info;

        return 0;
    }
    else if (cached_values_present)
    {
        *device = hwmem_files_device;
        *serial_number = hwmem_files_serial_number;

        return 1;
    }

    if (stat(hwmem_files_full_name, &hwmem_file_info) < 0)
    {
        if (0 == android_atomic_cmpxchg(0, 1, &write_values_initiated))
        {
            error_code_produced_when_retreiving_hwmem_file_info = errno;
            android_atomic_write(1, &error_occured_when_retreiving_hwmem_file_info);
        }

        LOG_ERROR("%s: stat failed, %s", __func__, strerror(errno));
        return 0;
    }

    if (0 == android_atomic_cmpxchg(0, 1, &write_values_initiated))
    {
        hwmem_files_device = hwmem_file_info.st_dev;
        hwmem_files_serial_number = hwmem_file_info.st_ino;
        android_atomic_write(1, &cached_values_present);
    }

    *device = hwmem_file_info.st_dev;
    *serial_number = hwmem_file_info.st_ino;

    return 1;
}

static int is_hwmem_fd(int fd, int* is_hwmem_fd_var)
{
    dev_t hwmem_file_device;
    ino_t hwmem_file_serial_number;

    struct stat file_info;

    if (!get_hwmem_file_info(&hwmem_file_device, &hwmem_file_serial_number))
        return 0;

    if (fstat(fd, &file_info) < 0)
    {
        LOG_USER_ERROR("%s: stat failed, %s", __func__, strerror(errno));
        return 0;
    }

    if (file_info.st_dev == hwmem_file_device &&
        file_info.st_ino == hwmem_file_serial_number)
        *is_hwmem_fd_var = 1;
    else
        *is_hwmem_fd_var = 0;

    return 1;
}

static enum hwmem_alloc_flags usage_2_hwmem_alloc_flags(int usage)
{
    int sw_usage = usage & (GRALLOC_USAGE_SW_READ_MASK | GRALLOC_USAGE_SW_WRITE_MASK);
    int hw_usage = usage & GRALLOC_USAGE_HW_MASK;

    if (sw_usage == 0)
        return HWMEM_ALLOC_HINT_UNCACHED;
    else if (sw_usage & GRALLOC_USAGE_SW_READ_OFTEN ||
        (sw_usage & GRALLOC_USAGE_SW_READ_RARELY && !(hw_usage &
        (GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_HW_2D))))
    {
        /* Read often, or rarely with no chance of HW write -> SW read (invalidate
        needed) */
        if (sw_usage & GRALLOC_USAGE_SW_WRITE_OFTEN)
            return HWMEM_ALLOC_HINT_WRITE_COMBINE | HWMEM_ALLOC_HINT_CACHED |
                HWMEM_ALLOC_HINT_CACHE_WB;
        else
            return HWMEM_ALLOC_HINT_WRITE_COMBINE | HWMEM_ALLOC_HINT_CACHED |
                HWMEM_ALLOC_HINT_CACHE_WT;
    }
    else
        /* Write often/rarely, read rarely with risk of HW write -> SW read or a
        combination of the two */
        return HWMEM_ALLOC_HINT_UNCACHED | HWMEM_ALLOC_HINT_WRITE_COMBINE;
}

static enum hwmem_mem_type usage_2_hwmem_mem_type(int usage)
{
    if (usage & GRALLOC_USAGE_PROTECTED) {
        return HWMEM_MEM_PROTECTED_SYS;
    }
    else if (usage & GRALLOC_USAGE_HW_2D) {
        return HWMEM_MEM_CONTIGUOUS_SYS;
    }

    return HWMEM_MEM_SCATTERED_SYS;
}

static int hwmem_access_2_mmap_prot(enum hwmem_access hwmem_access)
{
    int mmap_prot = 0;

    if (hwmem_access & HWMEM_ACCESS_READ)
        mmap_prot |= PROT_READ;
    if (hwmem_access & HWMEM_ACCESS_WRITE)
        mmap_prot |= PROT_WRITE;

    return mmap_prot;
}

static int limit_mmap_prot_to_usage(int mmap_prot, int usage)
{
    int new_mmap_prot = mmap_prot & ~PROT_EXEC;

    if (!(usage & GRALLOC_USAGE_SW_READ_MASK))
        new_mmap_prot &= ~PROT_READ;
    if (!(usage & GRALLOC_USAGE_SW_WRITE_MASK))
        new_mmap_prot &= ~PROT_WRITE;

    return new_mmap_prot;
}

static int inc_buf_cnt(struct hwmem_gralloc_buf_handle_t* buf,
    enum buf_counters buf_counter)
{
    int ret = 1;
    struct buf_tg_info *buf_tg_info;
    int *cnt;

    /* Wrapper handles don't have thread group info */
    if (buf->size != -1)
        return 1;

    mutex_lock(&buf_tg_infos_mutex);

    if (!get_create_buf_tg_info(buf, &buf_tg_info))
        goto get_create_buf_tg_info_failed;

    switch (buf_counter)
    {
        case REGISTER_COUNTER:
            cnt = &buf_tg_info->reg_cnt;
            break;

        case LOCK_COUNTER:
            cnt = &buf_tg_info->lock_cnt;
            break;

        default:
            LOG_ERROR("%s: Unknown buffer counter, %i", __func__, buf_counter);
            goto out;
    }

    if (*cnt == INT_MAX)
    {
        LOG_ERROR("%s: *cnt == INT_MAX", __func__);
        goto overflow;
    }

    (*cnt)++;

    goto out;

overflow:
get_create_buf_tg_info_failed:
    ret = 0;

out:
    mutex_unlock(&buf_tg_infos_mutex);

    return ret;
}

static void dec_buf_cnt(struct hwmem_gralloc_buf_handle_t* buf,
    enum buf_counters buf_counter, int lock)
{
    struct buf_tg_info *buf_tg_info;
    int *cnt;

    /* Wrapper handles don't have thread group info */
    if (buf->size != -1)
        return;

    if (lock)
        mutex_lock(&buf_tg_infos_mutex);

    buf_tg_info = get_buf_tg_info(buf);
    if (NULL == buf_tg_info)
    {
        LOG_USER_ERROR("%s: Usage count mismatch", __func__);
        goto out;
    }

    switch (buf_counter)
    {
        case REGISTER_COUNTER:
            cnt = &buf_tg_info->reg_cnt;
            break;

        case LOCK_COUNTER:
            cnt = &buf_tg_info->lock_cnt;
            break;

        default:
            LOG_ERROR("%s: Unknown buffer counter, %i", __func__, buf_counter);
            goto out;
    }

    (*cnt)--;
    if (*cnt < 0)
    {
        LOG_USER_ERROR("%s: Usage count mismatch", __func__);
        *cnt = 0;
    }

    if (0 == buf_tg_info->reg_cnt && 0 == buf_tg_info->lock_cnt)
    {
        remove_buf_tg_info(buf);
    }

    goto out;

out:
    if (lock)
        mutex_unlock(&buf_tg_infos_mutex);
}

static int dec_buf_reg_cnt_if_last_usr(struct hwmem_gralloc_buf_handle_t* buf)
{
    int ret = 1;
    struct buf_tg_info *buf_tg_info;

    /* Wrapper handles don't have thread group info */
    if (buf->size != -1)
        return 1;

    mutex_lock(&buf_tg_infos_mutex);

    buf_tg_info = get_buf_tg_info(buf);
    if (NULL == buf_tg_info)
    {
        LOG_USER_ERROR("%s: Usage count mismatch", __func__);
        goto out;
    }

    if (buf_tg_info->reg_cnt > 1 || buf_tg_info->lock_cnt != 0)
    {
        LOG_USER_ERROR("%s: Buffer is in use", __func__);
        errno = ENOMSG;
        goto buf_in_use;
    }

    dec_buf_cnt(buf, REGISTER_COUNTER, 0);

    goto out;

buf_in_use:
    ret = 0;

out:
    mutex_unlock(&buf_tg_infos_mutex);

    return ret;
}

/* buf_tg_infos_mutex must be held when calling this function */
static int get_create_buf_tg_info(struct hwmem_gralloc_buf_handle_t* buf,
    struct buf_tg_info **buf_tg_info)
{
    if (buf->size != -1)
    {
        LOG_ERROR("%s: Creating thread group info for wrapper handle", __func__);
        return 0;
    }

    if (!ensure_buf_handl_2_tg_info_map_exists())
        return 0;

    *buf_tg_info = (struct buf_tg_info *)hashmapGet(
        buf_handl_2_tg_info_map, buf);
    if (*buf_tg_info != NULL)
        return 1;

    *buf_tg_info = (struct buf_tg_info *)malloc(sizeof(struct buf_tg_info));
    memset(*buf_tg_info, 0, sizeof(**buf_tg_info));
    (*buf_tg_info)->addr = NULL;

    errno = 0;
    if (NULL == hashmapPut(buf_handl_2_tg_info_map, buf, *buf_tg_info) && ENOMEM == errno)
    {
        LOG_ERROR("%s: NULL == hashmapPut(buf_handl_2_tg_info_map, buf, *buf_tg_info) && ENOMEM == errno", __func__);
        goto hashmap_put_failed;
    }

    return 1;

hashmap_put_failed:
    free(*buf_tg_info);

    return 0;
}

/* buf_tg_infos_mutex must be held when calling this function */
static struct buf_tg_info *get_buf_tg_info(struct hwmem_gralloc_buf_handle_t* buf)
{
    if (!ensure_buf_handl_2_tg_info_map_exists())
        return NULL;

    return (struct buf_tg_info *)hashmapGet(buf_handl_2_tg_info_map, buf);
}

/* buf_tg_infos_mutex must be held when calling this function */
static void remove_buf_tg_info(struct hwmem_gralloc_buf_handle_t* buf)
{
    struct buf_tg_info *buf_tg_info;

    if (!ensure_buf_handl_2_tg_info_map_exists())
        return;

    munmap_buf_if_necessary(buf);
    buf_tg_info = (struct buf_tg_info *)hashmapRemove(buf_handl_2_tg_info_map,
        buf);
    if (NULL == buf_tg_info)
    {
        LOG_ERROR("%s: Removing non existing thread group info", __func__);
    }
    free(buf_tg_info);
}

/* buf_tg_infos_mutex must be held when calling this function */
static int ensure_buf_handl_2_tg_info_map_exists(void)
{
    if (buf_handl_2_tg_info_map != NULL)
        return 1;

    buf_handl_2_tg_info_map = hashmapCreate(10, ptr_2_int, are_ptrs_equal);
    if (NULL == buf_handl_2_tg_info_map)
    {
        LOG_ERROR("%s: Failed to create hashmap, %s", __func__, strerror(errno));
        return 0;
    }

    return 1;
}

static int ptr_2_int(void *ptr)
{
    return (int)ptr;
}

static bool are_ptrs_equal(void *ptr1, void *ptr2)
{
    return ptr1 == ptr2;
}
