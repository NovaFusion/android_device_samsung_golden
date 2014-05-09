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

#include <sys/mman.h>

#include <dlfcn.h>

#include <cutils/ashmem.h>
#include <cutils/log.h>

#include <hardware/hardware.h>
#include <hardware/gralloc.h>
#include <hardware/fb.h>
#include "gralloc_stericsson_ext.h"

#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>

#include <cutils/log.h>
#include <cutils/atomic.h>

#include <system/graphics.h>
#include <system/window.h>

#if HAVE_ANDROID_OS
#include <linux/fb.h>
#endif

#include <GLES/gl.h>

#include "hwmem_gralloc.h"
#include <linux/compdev.h>
#define COMPDEV_PATH "/dev/comp0"

enum {
    PAGE_FLIP = 0x00000001,
    LOCKED = 0x00000002
};

enum {
    // flag to indicate we'll post this buffer
    PRIV_USAGE_LOCKED_FOR_POST = 0x80000000
};

struct fb_context_t {
    framebuffer_device_t  device;
    int compdev;
};

/*****************************************************************************/

static inline size_t roundUpToPageSize(size_t x) {
    return (x + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1);
}

static void init_hwmem_gralloc_buf_handle(struct hwmem_gralloc_buf_handle_t* handle, int fd, int size, int flags)
{
    memset(handle, 0, sizeof(struct hwmem_gralloc_buf_handle_t));
    handle->fd = fd;
    handle->size = size;
    handle->flags = flags;
    handle->pid = getpid();
    handle->type_identifier = hwmem_gralloc_buf_handle_type_identifier;
    handle->type = GRALLOC_BUF_TYPE_FB;
    handle->base.version = sizeof(native_handle_t);
    handle->base.numInts = num_ints_in_hwmem_gralloc_buf_handle;
    handle->base.numFds = num_fds_in_hwmem_gralloc_buf_handle;
}

static int fb_setSwapInterval(struct framebuffer_device_t* dev,
            int interval)
{
    struct fb_context_t* ctx = (struct fb_context_t*)dev;
    if (interval < dev->minSwapInterval || interval > dev->maxSwapInterval)
        return -EINVAL;
    // FIXME: implement fb_setSwapInterval
    return 0;
}

static int fb_setUpdateRect(struct framebuffer_device_t* dev,
        int l, int t, int w, int h)
{
    struct fb_context_t* ctx = (struct fb_context_t*)dev;
    struct hwmem_gralloc_module_t* m = (struct hwmem_gralloc_module_t*)
            dev->common.module;

    if (((w|h) <= 0) || ((l|t)<0))
        return -EINVAL;

    m->info.reserved[0] = 0x54445055; // "UPDT";
    m->info.reserved[1] = (uint16_t)l | ((uint32_t)t << 16);
    m->info.reserved[2] = (uint16_t)(l+w) | ((uint32_t)(t+h) << 16);
    return 0;
}

static unsigned int fb_rotate_to_compdev(__u32 fb_rotate)
{
    switch (fb_rotate) {
    case FB_ROTATE_UR:
        return COMPDEV_TRANSFORM_ROT_0;
    case FB_ROTATE_CW:
        return COMPDEV_TRANSFORM_ROT_90_CW;
    case FB_ROTATE_UD:
        return COMPDEV_TRANSFORM_ROT_180;
    case FB_ROTATE_CCW:
        return COMPDEV_TRANSFORM_ROT_90_CCW;
    default:
        ALOGE("%s: Illegal fb rotation supplied", __func__);
        return 0;
    }
}

static void send_to_compdev(struct fb_context_t* ctx, struct hwmem_gralloc_module_t* m)
{
    struct fb_fix_screeninfo *lcd_finfo = &m->finfo;
    struct fb_var_screeninfo *lcd_vinfo = &m->info;
    int i;
    int ret;
    struct compdev_img img;

    memset(&img, 0, sizeof(img));

    /* lcd_vinfo->yoffset contains the LCD actual visible buffer */
    img.buf.offset = lcd_finfo->smem_start +
            lcd_finfo->line_length * lcd_vinfo->yoffset;

    /* Input */
    switch (lcd_vinfo->bits_per_pixel) {
    case 16:
        img.fmt = COMPDEV_FMT_RGB565;
        break;
    case 24:
        img.fmt = COMPDEV_FMT_RGB888;
        break;
    case 32:
    default:
        img.fmt = COMPDEV_FMT_RGBA8888;
        break;
    }
    img.width = lcd_vinfo->xres;
    img.height = lcd_vinfo->yres;
    img.pitch = lcd_finfo->line_length;
    img.buf.type = COMPDEV_PTR_PHYSICAL;
    img.buf.len = lcd_finfo->line_length * lcd_vinfo->yres;
    img.src_rect.x = 0;
    img.src_rect.y = 0;
    img.src_rect.width = lcd_vinfo->xres;
    img.src_rect.height = lcd_vinfo->yres;

    img.dst_rect.x = 0;
    img.dst_rect.y = 0;
    if (m->info.rotate == FB_ROTATE_CW ||
        m->info.rotate == FB_ROTATE_CCW) {
        img.dst_rect.width = lcd_vinfo->yres;
        img.dst_rect.height = lcd_vinfo->xres;
    } else {
        img.dst_rect.width = lcd_vinfo->xres;
        img.dst_rect.height = lcd_vinfo->yres;
    }

    img.z_position = 1; // HWC can decide to put a buffer either on top(0) or below(2).

    img.flags = (uint32_t)COMPDEV_FRAMEBUFFER_FLAG;

    img.transform = fb_rotate_to_compdev(m->info.rotate);

    ret = ioctl(ctx->compdev, COMPDEV_POST_BUFFER_IOC,
            (struct compdev_img*)&img);
    if (ret < 0)
        ALOGE("%s: Failed to post buffers to compdev, %s", __func__,
                strerror(errno));

}

static int fb_post(struct framebuffer_device_t* dev, buffer_handle_t buffer)
{
    struct fb_context_t* ctx = (struct fb_context_t*)dev;

    struct hwmem_gralloc_module_t* m;
    struct hwmem_gralloc_buf_handle_t * hnd;

    if (!module_2_hwmem_gralloc_module((struct gralloc_module_t const*)dev->common.module, &m) ||
        !handle_2_hwmem_gralloc_handle(buffer, &hnd))
        return -errno;

    if (m->currentBuffer) {
        m->base.unlock(&m->base, m->currentBuffer);
        m->currentBuffer = 0;
    }

    if (hnd->flags & PRIV_FLAGS_FRAMEBUFFER) {
        size_t offset;

        m->base.lock(&m->base, buffer,
                PRIV_USAGE_LOCKED_FOR_POST,
                0, 0, m->info.xres, m->info.yres, NULL);

        offset = hnd->base_addr - m->framebuffer->base_addr;
        m->info.activate = FB_ACTIVATE_VBL;
        m->info.yoffset = offset / m->finfo.line_length;

        // Send the frame to compdev driver
        send_to_compdev(ctx, m);
        m->currentBuffer = buffer;
    } else {
        // If we can't do the page_flip, just copy the buffer to the front
        // FIXME: use copybit HAL instead of memcpy

        void* fb_vaddr;
        void* buffer_vaddr;

        m->base.lock(&m->base, (buffer_handle_t)m->framebuffer,
                GRALLOC_USAGE_SW_WRITE_RARELY,
                0, 0, m->info.xres, m->info.yres,
                &fb_vaddr);

        m->base.lock(&m->base, buffer,
                GRALLOC_USAGE_SW_READ_RARELY,
                0, 0, m->info.xres, m->info.yres,
                &buffer_vaddr);

        memcpy(fb_vaddr, buffer_vaddr, m->finfo.line_length * m->info.yres);

        m->base.unlock(&m->base, buffer);
        m->base.unlock(&m->base, (buffer_handle_t)m->framebuffer);
    }

    return 0;
}

int fb_compositionComplete()
{
    //Before we start updating applications buffers again i.e. setting up the next frame we need to
    //make sure all buffers have been used by the compositor in *this* frame. This is done by
    //flushing all outstanding draw calls. A flush will allways be done if a call to glReadPixels()
    //is made whereas a call to glFinish() might be "optimized" out by the driver.
    //
    //This forced synchonous behaviour in the compositor should not affect applications ability to
    //do asynchonous rendering.
    unsigned char dummy_pixel[4];
    glReadPixels(0,0,1,1, GL_RGBA, GL_UNSIGNED_BYTE, dummy_pixel);
    return 0;
}

static int is_pitch_mcde_aligned(__u32 pitch)
{
    /* TODO: We should get the alignment from the driver instead of hard coding it. */
    static const unsigned int mcde_pitch_alignment = 8;

    return (pitch & (mcde_pitch_alignment - 1)) == 0;
}

static unsigned int fb_rotate_to_degree(__u32 fb_rotate)
{
    switch (fb_rotate) {
    case FB_ROTATE_UR:
        return 0;
    case FB_ROTATE_CW:
        return 90;
    case FB_ROTATE_UD:
        return 180;
    case FB_ROTATE_CCW:
        return 270;
    default:
        ALOGE("%s: Illegal fb rotation supplied", __func__);
        return 0;
    }
}

static __u32 degree_to_fb_rotate(unsigned int degree)
{
    degree = degree % 360;

    switch (degree) {
    case 0:
        return FB_ROTATE_UR;
    case 270:
        return FB_ROTATE_CCW;
    case 180:
        return FB_ROTATE_UD;
    case 90:
        return FB_ROTATE_CW;
    default:
        ALOGE("%s: Illegal degree supplied", __func__);
        return FB_ROTATE_UR;
    }
}

static void switch_uint(unsigned int* uint1, unsigned int* uint2)
{
    unsigned int tmp = *uint1;
    *uint1 = *uint2;
    *uint2 = tmp;
}

static void switch_float(float* float1, float* float2)
{
    float tmp = *float1;
    *float1 = *float2;
    *float2 = tmp;
}

static int fb_rotate(struct framebuffer_device_t* dev, unsigned int absolute_degree)
{
    struct hwmem_gralloc_module_t* module;
    __u32 old_rotation_degree;
    int switch_width_height;

    if (dev == NULL || absolute_degree % 90 != 0)
        return -EINVAL;

    if (!module_2_hwmem_gralloc_module((struct gralloc_module_t const*)dev->common.module, &module))
        return -errno;

    if (!is_pitch_mcde_aligned((module->info.xres * module->info.bits_per_pixel) / 8) ||
        !is_pitch_mcde_aligned((module->info.yres * module->info.bits_per_pixel) / 8))
    {
        /* Rotation might cause fb re-alloc which we can't handle. */
        ALOGW("Unable to rotate in hardware due to unaligned framebuffer width and/or height");
        return -ENOMSG;
    }

    old_rotation_degree = fb_rotate_to_degree(module->info.rotate);
    module->info.rotate = degree_to_fb_rotate(absolute_degree);

    switch_width_height = old_rotation_degree % 180 != absolute_degree % 180;
    if (switch_width_height) {
        int num_bufs = module->info.yres_virtual / module->info.yres;
        switch_uint(&module->info.xres, &module->info.yres);
        module->info.xres_virtual = module->info.xres;
        module->info.yres_virtual = module->info.yres * num_bufs;
        module->finfo.line_length = (module->info.xres * module->info.bits_per_pixel) / 8;

        /*
         * This isn't thread safe and can't be made thread safe without changing
         * the API. As far as I can see they are currently only used by one thread
         * at a time so not a problem right now.
         */
        /*
         * TODO: Change api and make thread safe so that we don't run into nasty
         * problems in the future.
         */
        switch_uint(&dev->width, &dev->height);
        switch_float(&dev->xdpi, &dev->ydpi);
        switch_float(&module->xdpi, &module->ydpi);
        dev->stride = module->info.xres;
    }

    return 0;
}

/*****************************************************************************/

static int mapFrameBufferLocked(struct hwmem_gralloc_module_t* module)
{
    char const * const device_template[] = {
            "/dev/graphics/fb%u",
            "/dev/fb%u",
            0 };

    int fd = -1;
    int i=0;
    char name[64];
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo info;
    uint32_t flags;
    float xdpi;
    float ydpi;
    float fps;

    // already initialized...
    if (module->framebuffer) {
        return 0;
    }

    while ((fd==-1) && device_template[i]) {
        snprintf(name, 64, device_template[i], 0);
        fd = open(name, O_RDWR, 0);
        i++;
    }
    if (fd < 0)
        return -errno;

    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1)
        return -errno;

    if (ioctl(fd, FBIOGET_VSCREENINFO, &info) == -1)
        return -errno;

    info.reserved[0] = 0;
    info.reserved[1] = 0;
    info.reserved[2] = 0;
    info.xoffset = 0;
    info.yoffset = 0;
    info.activate = FB_ACTIVATE_NOW;

    flags = PAGE_FLIP;

    if (info.yres_virtual < info.yres * 2) {
        // we need at least 2 for page-flipping
        info.yres_virtual = info.yres;
        flags &= ~PAGE_FLIP;
        ALOGW("page flipping not supported (yres_virtual=%d, requested=%d)",
                info.yres_virtual, info.yres*2);
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &info) == -1)
        return -errno;

    uint64_t divisor = (info.upper_margin + info.lower_margin + info.yres)
            * (info.left_margin  + info.right_margin + info.xres)
            * info.pixclock;

    int refreshRate = 0;
    if (divisor)
        refreshRate = (int)(1000000000000000LLU / divisor);

    if (refreshRate == 0) {
        // bleagh, bad info from the driver
        refreshRate = 60*1000;  // 60 Hz
    }

    if ((int)info.width <= 0 || (int)info.height <= 0) {
        // the driver doesn't return that information
        // default to 160 dpi
        info.width  = ((info.xres * 25.4f)/160.0f + 0.5f);
        info.height = ((info.yres * 25.4f)/160.0f + 0.5f);
    }

    xdpi = (info.xres * 25.4f) / info.width;
    ydpi = (info.yres * 25.4f) / info.height;
    fps  = refreshRate / 1000.0f;

    ALOGI(   "using (fd=%d)\n"
            "id           = %s\n"
            "xres         = %d px\n"
            "yres         = %d px\n"
            "xres_virtual = %d px\n"
            "yres_virtual = %d px\n"
            "bpp          = %d\n"
            "r            = %2u:%u\n"
            "g            = %2u:%u\n"
            "b            = %2u:%u\n",
            fd,
            finfo.id,
            info.xres,
            info.yres,
            info.xres_virtual,
            info.yres_virtual,
            info.bits_per_pixel,
            info.red.offset, info.red.length,
            info.green.offset, info.green.length,
            info.blue.offset, info.blue.length
    );

    ALOGI(   "width        = %d mm (%f dpi)\n"
            "height       = %d mm (%f dpi)\n"
            "refresh rate = %.2f Hz\n",
            info.width,  xdpi,
            info.height, ydpi,
            fps
    );


    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1)
        return -errno;

    if (finfo.smem_len <= 0)
        return -errno;

    module->flags = flags;
    module->info = info;
    module->finfo = finfo;
    module->xdpi = xdpi;
    module->ydpi = ydpi;
    module->fps = fps;

    /*
     * map the framebuffer
     */
    {
        int err;
        void* vaddr;
        size_t fbSize = roundUpToPageSize(finfo.line_length * info.yres_virtual);
        module->framebuffer = malloc(sizeof(struct hwmem_gralloc_buf_handle_t));
        init_hwmem_gralloc_buf_handle(module->framebuffer, dup(fd), fbSize, PRIV_FLAGS_USES_PMEM);

        module->numBuffers = info.yres_virtual / info.yres;
        module->bufferMask = 0;
        module->currentBufferIndex = -1;

        vaddr = mmap(0, fbSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        if (vaddr == MAP_FAILED) {
            ALOGE("Error mapping the framebuffer (%s)", strerror(errno));
            return -errno;
        }
        module->framebuffer->base_addr = (int)vaddr;
        memset(vaddr, 0, fbSize);
    }
    return 0;
}

static int mapFrameBuffer(struct hwmem_gralloc_module_t* module)
{
    int err;
    pthread_mutex_lock(&module->lock);
    err = mapFrameBufferLocked(module);
    pthread_mutex_unlock(&module->lock);
    return err;
}

/*****************************************************************************/

static int fb_close(struct hw_device_t *dev)
{
    struct fb_context_t* ctx = (struct fb_context_t*)dev;
    if (ctx) {
        close(ctx->compdev);
        free(ctx);
    }
    return 0;
}

int fb_device_open(hw_module_t const* module, const char* name,
        hw_device_t** device)
{
    int status = -EINVAL;
    if (!strcmp(name, GRALLOC_HARDWARE_FB0)) {
        alloc_device_t* gralloc_device;
        struct fb_context_t *dev;
        struct hwmem_gralloc_module_t* m;

        status = gralloc_open(module, &gralloc_device);
        if (status < 0)
            return status;

        /* initialize our state here */
        dev = (struct fb_context_t*)malloc(sizeof(*dev));
        memset(dev, 0, sizeof(*dev));

        /* initialize the procs */
        dev->device.common.tag = HARDWARE_DEVICE_TAG;
        dev->device.common.version = 0;
        dev->device.common.module = (hw_module_t*)module;
        dev->device.common.close = fb_close;
        dev->device.setSwapInterval = fb_setSwapInterval;
        dev->device.post            = fb_post;
        dev->device.setUpdateRect = 0;
        dev->device.compositionComplete = fb_compositionComplete;
        dev->device.rotate          = fb_rotate;

        m = (struct hwmem_gralloc_module_t*)module;
        status = mapFrameBuffer(m);
        if (status >= 0) {
            int stride = m->finfo.line_length / (m->info.bits_per_pixel >> 3);
            *((uint32_t*)&dev->device.flags) = 0;
            if (m->info.bits_per_pixel == 16) {
                *((int*)&dev->device.format) = HAL_PIXEL_FORMAT_RGB_565;
            } else if (m->info.bits_per_pixel == 32) {
                *((int*)&dev->device.format) = HAL_PIXEL_FORMAT_BGRA_8888;
            } else {
                ALOGE("fb_device_open: Unsupported format");
                *((int*)&dev->device.format) = HAL_PIXEL_FORMAT_RGB_565;
            }
            dev->device.width = m->info.xres;
            dev->device.height = m->info.yres;
            dev->device.stride = stride;
            dev->device.xdpi = m->xdpi;
            dev->device.ydpi = m->ydpi;
            *((float*)&dev->device.fps) = m->fps;
            *((int*)&dev->device.minSwapInterval) = 1;
            *((int*)&dev->device.maxSwapInterval) = 1;
            *device = &dev->device.common;

            struct fb_context_t* ctx = (struct fb_context_t*)dev;
            /* Open the compdev */
            ctx->compdev = open(COMPDEV_PATH, O_RDWR, 0);
            if (ctx->compdev < 0) {
                ALOGE("%s: Error Opening "COMPDEV_PATH": %s\n",__func__,
                        strerror(errno));
            }

        } else {
            free(dev);
        }
    }

    return status;
}

static int gralloc_alloc_framebuffer_locked(alloc_device_t* dev,
        size_t size, int usage, buffer_handle_t* pHandle, int *stride,
        int* handled)
{
    struct hwmem_gralloc_module_t* m = (struct hwmem_gralloc_module_t*)
            dev->common.module;
    uint32_t bufferMask;
    uint32_t numBuffers;
    size_t bufferSize;
    int vaddr;
    uint32_t i;
    struct hwmem_gralloc_buf_handle_t* hnd;

    *handled = 1;

    // allocate the framebuffer
    if (m->framebuffer == NULL) {
        // initialize the framebuffer, the framebuffer is mapped once
        // and forever.
        int err = mapFrameBufferLocked(m);
        if (err < 0)
            return err;
    }

    bufferMask = m->bufferMask;
    numBuffers = m->numBuffers;
    bufferSize = m->finfo.line_length * m->info.yres;
    if (numBuffers == 1) {
        // If we have only one buffer, we never use page-flipping. Instead,
        // we return a regular buffer which will be memcpy'ed to the main
        // screen when post is called.
        *handled = 0;
        return 0;
    }

    if (bufferMask >= ((1LU<<numBuffers)-1)) {
        // We ran out of buffers.
        return -ENOMEM;
    }

    // create a "fake" handles for it
    vaddr = m->framebuffer->base_addr;
    hnd = malloc(sizeof(struct hwmem_gralloc_buf_handle_t));
    init_hwmem_gralloc_buf_handle(hnd, dup(m->framebuffer->fd), size, PRIV_FLAGS_USES_PMEM | PRIV_FLAGS_FRAMEBUFFER);

    // find a free slot
    for (i = 0 ; i < numBuffers ; i++) {
        m->currentBufferIndex = (m->currentBufferIndex + 1) % numBuffers;
        if ((bufferMask & (1LU<<m->currentBufferIndex)) == 0) {
            m->bufferMask |= (1LU<<m->currentBufferIndex);
            break;
        }
    }
    vaddr += bufferSize * m->currentBufferIndex;

    hnd->base_addr = vaddr;
    hnd->offset = vaddr - m->framebuffer->base_addr;
    *pHandle = (buffer_handle_t)hnd;
    *stride = (m->finfo.line_length * 8) / m->info.bits_per_pixel;

    return 0;
}

int gralloc_alloc_framebuffer(alloc_device_t* dev,
        size_t size, int usage, buffer_handle_t* pHandle, int *stride,
        int* handled)
{
    struct hwmem_gralloc_module_t* m = (struct hwmem_gralloc_module_t*)
            dev->common.module;
    int err;
    *handled = 1;
    pthread_mutex_lock(&m->lock);
    err = gralloc_alloc_framebuffer_locked(dev, size, usage, pHandle, stride,
                handled);
    pthread_mutex_unlock(&m->lock);
    return err;
}
