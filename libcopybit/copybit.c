/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2014 NovaFusion http://novafusion.pl
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
/*
 * Copyright (C) ST-Ericsson AB 2009
 *
 * \file    copybit.c
 * \brief   HW acceleration of Copybit HAL using the BLT API and B2R2
 * \author  Robert Lind <robert.lind@stericsson.com> for ST-Ericsson
 *
 */


// for access to PMEM driver ioctls and structures
#include <linux/android_pmem.h>
#include <linux/fb.h>

#include <cutils/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <hardware/copybit.h>
#include "blt_api.h"

#include <hardware/hardware.h>
#include <hardware/gralloc.h>
#include <gralloc_stericsson_ext.h>


/* Values returned for copybit_get */
#define MINIFICATION_LIMIT  8
#define MAGNIFICATION_LIMIT 8
#define SCALING_FRAC_BITS   32
#define ROTATION_STEP_DEG   90

/* Default priority for B2R2 requests */
#define BLT_DEFAULT_PRIO 4

/* FB finfo.id identifying the ST frame buffer device */
#define FINFO_ID "ST MCDE"

#define DEBUG_LOG_COPYBIT 0


/******************************************************************************/

/** State information for each device instance */
struct copybit_context_t {
    struct copybit_device_t device;
    int    mFD;
    int    blt_handle;
    struct blt_req blt_req;
    const struct gralloc_module_t *gralloc;
};


/** Common hardware methods */
static int open_copybit(const struct hw_module_t *module,
                        const char *name,
                        struct hw_device_t **device);

static struct hw_module_methods_t copybit_module_methods = {
    .open =  open_copybit
};

/** The COPYBIT Module */
struct copybit_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .version_major = 1,
        .version_minor = 0,
        .id = COPYBIT_HARDWARE_MODULE_ID,
        .name = "STE BLT_API COPYBIT Module",
        .author = "ST-Ericsson",
        .methods = &copybit_module_methods,
    }
};


/*****************************************************************************/

/** min of int a, b */
static inline int min(int a, int b) {
     return (a<b) ? a : b;
}

/** max of int a, b */
static inline int max(int a, int b) {
     return (a>b) ? a : b;
}

/** Determine the intersection of lhs & rhs store in out */
static void intersect(struct copybit_rect_t *out,
                      const struct copybit_rect_t *lhs,
                      const struct copybit_rect_t *rhs) {
     out->l = max(lhs->l, rhs->l);
     out->t = max(lhs->t, rhs->t);
     out->r = min(lhs->r, rhs->r);
     out->b = min(lhs->b, rhs->b);
}

/** Convert a copybit format to a blt format */
static enum blt_fmt to_blt_fmt(int copybit_format)
{
    switch (copybit_format) {
    case COPYBIT_FORMAT_RGBA_8888:
    case COPYBIT_FORMAT_RGBX_8888:
        return BLT_FMT_32_BIT_ABGR8888;
    case COPYBIT_FORMAT_BGRA_8888:
        return BLT_FMT_32_BIT_ARGB8888;
    case COPYBIT_FORMAT_RGB_565:
        return BLT_FMT_16_BIT_RGB565;
    case COPYBIT_FORMAT_YCbCr_422_SP:
        return BLT_FMT_YUV422_PACKED_SEMI_PLANAR;
    case COPYBIT_FORMAT_YCbCr_420_SP:
        return BLT_FMT_YUV420_PACKED_SEMI_PLANAR;
    case COPYBIT_FORMAT_YCBCR42XMBN:
        return BLT_FMT_YUV420_PACKED_SEMIPLANAR_MB_STE;
    case COPYBIT_FORMAT_YCbCr_422_P:
        return BLT_FMT_YUV422_PACKED_PLANAR;
    case COPYBIT_FORMAT_YCbCr_420_P:
        return BLT_FMT_YUV420_PACKED_PLANAR;
    case COPYBIT_FORMAT_YCbCr_422_I:
	return BLT_FMT_CB_Y_CR_Y;
    case COPYBIT_FORMAT_YCrCb_422_SP:
        return BLT_FMT_YVU422_PACKED_SEMI_PLANAR;
    case COPYBIT_FORMAT_YCrCb_420_SP:
        return BLT_FMT_YVU420_PACKED_SEMI_PLANAR;
    case COPYBIT_FORMAT_YCrCb_422_P:
        return BLT_FMT_YVU422_PACKED_PLANAR;
    case COPYBIT_FORMAT_YCrCb_420_P:
        return BLT_FMT_YVU420_PACKED_PLANAR;
    default:
        return BLT_FMT_UNUSED;
    }
}

static int convert_image(const struct gralloc_module_t *gralloc,
        struct copybit_image_t const *cp_img,
        struct blt_img *blt_img)
{
    buffer_handle_t buf_hnd = cp_img->handle;

    int alloc_hnd;
    int offset;
    int type;
    int size;

    ALOGD_IF(DEBUG_LOG_COPYBIT,"-----%s: %d------\n", __FUNCTION__, __LINE__);

    if(buf_hnd){
      //check if handle is ok
    } else {
        ALOGD("copybit.c: cp_img->handle is NULL...not good\n"
            "cp_img->w=%d\n"
            "cp_img->h=%d\n"
            "cp_img->format=%d\n"
            "cp_img->base=0x%p\n"
            "cp_img->handle=0x%p",
            cp_img->w, cp_img->h, cp_img->format, cp_img->base, cp_img->handle);
      return -EINVAL;
    }

    memset(blt_img, 0, sizeof(*blt_img));

    blt_img->fmt = to_blt_fmt(cp_img->format);
    if (blt_img->fmt == BLT_FMT_UNUSED) {
        ALOGE("Unsupported format %d!", cp_img->format);
        return -EINVAL;
    }

    alloc_hnd = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_ALLOCATOR_HANDLE, buf_hnd);
    if (alloc_hnd < 0) {
        ALOGE("%s: Could not get allocator handle from gralloc!", __func__);
        return -EINVAL;
    }

    size = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_SIZE, buf_hnd);
    if (size < 0) {
        ALOGE("%s: Could not get buf size from gralloc!", __func__);
        return -EINVAL;
    }

    offset = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_OFFSET, buf_hnd);
    if (offset < 0) {
        ALOGE("%s: Could not get buf offset from gralloc!", __func__);
        return -EINVAL;
    }

    type = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_TYPE, buf_hnd);
    if (type < 0) {
        ALOGE("%s: Could not get buf type from gralloc!", __func__);
        return -EINVAL;
    }

    if (type == GRALLOC_BUF_TYPE_HWMEM) {
        blt_img->buf.type = BLT_PTR_HWMEM_BUF_NAME_OFFSET;
        blt_img->buf.hwmem_buf_name = alloc_hnd;
    } else {
        blt_img->buf.type = BLT_PTR_FD_OFFSET;
        blt_img->buf.fd = alloc_hnd;
    }

    blt_img->buf.offset = offset;
    blt_img->buf.bits = NULL;
    blt_img->buf.len = size;

    blt_img->width = cp_img->w;
    blt_img->height = cp_img->h;

    return 0;
}

static int convert_color(struct copybit_color_t *copybit_color, int copybit_fmt, uint32_t *color)
{
    switch (copybit_fmt) {
        case COPYBIT_FORMAT_RGBA_8888:
            *color = (copybit_color->a << 24) |
                     (copybit_color->b << 16) |
                     (copybit_color->g << 8) |
                     copybit_color->r;
            break;
        case COPYBIT_FORMAT_RGBX_8888:
            *color = (0xff << 24) |
                     (copybit_color->b << 16) |
                     (copybit_color->g << 8) |
                     copybit_color->r;
            break;
        case COPYBIT_FORMAT_BGRA_8888:
            *color = (copybit_color->a << 24) |
                     (copybit_color->r << 16) |
                     (copybit_color->g << 8) |
                     copybit_color->b;
            break;
        case COPYBIT_FORMAT_RGB_565:
            *color = (((copybit_color->r * 31) / 255) << 11) |
                     (((copybit_color->g * 63) / 255) << 5) |
                     ((copybit_color->b * 31) / 255);
            break;
        default:
            return -EINVAL;
    }

    return 0;
}

/** setup rectangles */
static void set_rects(
        const struct copybit_rect_t *src_rect,
        const struct copybit_rect_t *dst_rect,
        const struct copybit_rect_t *clip_rect,
        uint32_t transforms,
        struct blt_rect *src_rect_out,
        struct blt_rect *dst_rect_out)
{

    struct blt_rect src_norm;
    struct blt_rect dst_norm;
    struct blt_rect clip_norm;

    struct blt_rect src_clipped;
    struct blt_rect dst_clipped;

    int32_t l, r, t, b;

    /* Rescale factors (in 16.16 fixed point format) */
    uint32_t h_rsf;
    uint32_t v_rsf;

    /* Normalize the rectangles */
    src_norm.x = 0;
    src_norm.y = 0;
    src_norm.width = src_rect->r - src_rect->l;
    src_norm.height = src_rect->b - src_rect->t;

    dst_norm.x = 0;
    dst_norm.y = 0;
    dst_norm.width = dst_rect->r - dst_rect->l;
    dst_norm.height = dst_rect->b - dst_rect->t;

    /*
     * The clip rectangle will be normalized to the destination rectangle
     * coordinate space.
     */
    clip_norm.x = clip_rect->l - dst_rect->l;
    clip_norm.y = clip_rect->t - dst_rect->t;
    clip_norm.width = clip_rect->r - clip_rect->l;
    clip_norm.height = clip_rect->b - clip_rect->t;

    /* Clip the destination */
    l = max(dst_norm.x, clip_norm.x);
    t = max(dst_norm.y, clip_norm.y);
    r = min(dst_norm.x + dst_norm.width, clip_norm.x + clip_norm.width);
    b = min(dst_norm.y + dst_norm.height, clip_norm.y + clip_norm.height);

    dst_clipped.x = l;
    dst_clipped.y = t;
    dst_clipped.width = r - l;
    dst_clipped.height = b - t;

    src_clipped = dst_clipped;

    /* Transform the source rectangle */
    if (transforms & BLT_TRANSFORM_FLIP_V) {
        src_clipped.y = dst_norm.height - (dst_clipped.height + dst_clipped.y);
    }
    if (transforms & BLT_TRANSFORM_FLIP_H) {
        src_clipped.x = dst_norm.width - (dst_clipped.width + dst_clipped.x);
    }
    if (transforms & BLT_TRANSFORM_CCW_ROT_90) {
        int32_t tmp_x = src_clipped.x;

        src_clipped.x = dst_norm.height - (src_clipped.y + src_clipped.height);
        src_clipped.y = tmp_x;

        src_clipped.width = dst_clipped.height;
        src_clipped.height = dst_clipped.width;
    }

    /* Rescale the source rectangle */
    if (transforms & BLT_TRANSFORM_CCW_ROT_90) {
        h_rsf = (src_norm.width << 16) / dst_norm.height;
        v_rsf = (src_norm.height << 16) / dst_norm.width;
    } else {
        h_rsf = (src_norm.width << 16) / dst_norm.width;
        v_rsf = (src_norm.height << 16) / dst_norm.height;
    }

    if (h_rsf != (1 << 16)) {
        src_clipped.x *= h_rsf;
        src_clipped.width *= h_rsf;

        src_clipped.x = src_clipped.x >> 16;
        src_clipped.width = (src_clipped.width + (1 << 15)) >> 16;
    }
    if (v_rsf != (1 << 16)) {
        src_clipped.y *= v_rsf;
        src_clipped.height *= v_rsf;

        src_clipped.y = src_clipped.y >> 16;
        src_clipped.height = (src_clipped.height + (1 << 15)) >> 16;
    }

    /* De-normalize the clipped rectangles */
    src_clipped.x += src_rect->l;
    src_clipped.y += src_rect->t;

    dst_clipped.x += dst_rect->l;
    dst_clipped.y += dst_rect->t;

    /* And finally return the results */
    *src_rect_out = src_clipped;
    *dst_rect_out = dst_clipped;
}

/*****************************************************************************/

/** Set a copybit parameter */
static int copybit_set_parameter(struct copybit_device_t *dev,
                                 int name,
                                 int value)
{
    struct copybit_context_t *context = (struct copybit_context_t *) dev;
    int status = 0;

    ALOGD_IF(DEBUG_LOG_COPYBIT,"-----%s: %d------\n", __FUNCTION__, __LINE__);

    if (context) {
        switch (name) {

        case COPYBIT_ROTATION_DEG:
            context->blt_req.transform &= ~(BLT_TRANSFORM_CCW_ROT_90|
                                            BLT_TRANSFORM_CCW_ROT_180|
                                            BLT_TRANSFORM_CCW_ROT_270);
            switch (value) {
            case 0:
                break;
            case 90:
                context->blt_req.transform |= BLT_TRANSFORM_CCW_ROT_270;
                break;
            case 180:
                context->blt_req.transform |= BLT_TRANSFORM_CCW_ROT_180;
                break;
            case 270:
                context->blt_req.transform |= BLT_TRANSFORM_CCW_ROT_90;
                break;
            default:
                ALOGE("Invalid value for COPYBIT_ROTATION_DEG");
                status = -EINVAL;
                break;
            }
            break;

        case COPYBIT_PLANE_ALPHA:
            if (value >= 0 && value < 256) {
                context->blt_req.global_alpha = value;

                /* Disable global alpha if opaque */
                if (context->blt_req.global_alpha == 255) {
                    context->blt_req.flags &= ~BLT_FLAG_GLOBAL_ALPHA_BLEND;
                } else {
                    context->blt_req.flags |= BLT_FLAG_GLOBAL_ALPHA_BLEND;
                }
            } else {
                ALOGE("Invalid value for COPYBIT_PLANE_ALPHA");
                status = -EINVAL;
            }
            break;

        case COPYBIT_DITHER:
            switch (value) {
            case COPYBIT_ENABLE:
                context->blt_req.flags |= BLT_FLAG_DITHER;
                break;
            case COPYBIT_DISABLE:
                context->blt_req.flags &= ~BLT_FLAG_DITHER;
                break;
            default:
                ALOGE("Invalid value for COPYBIT_DITHER");
                status = -EINVAL;
                break;
            }
            break;

        case COPYBIT_TRANSFORM:

            context->blt_req.transform = 0;

            if (value & COPYBIT_TRANSFORM_FLIP_H) {
                context->blt_req.transform |= BLT_TRANSFORM_FLIP_H;
            }
            if (value & COPYBIT_TRANSFORM_FLIP_V) {
                context->blt_req.transform |= BLT_TRANSFORM_FLIP_V;
            }
            if (value & COPYBIT_TRANSFORM_ROT_90) {
                context->blt_req.transform ^= BLT_TRANSFORM_CCW_ROT_270;
            }

            break;

        case COPYBIT_BLUR:
            switch (value) {
            case COPYBIT_ENABLE:
                context->blt_req.flags |= BLT_FLAG_BLUR;
                break;
            case COPYBIT_DISABLE:
                context->blt_req.flags &= ~BLT_FLAG_BLUR;
                break;
            default:
                ALOGE("Invalid value for COPYBIT_BLUR");
                status = -EINVAL;
                break;
            }
            break;

        default:
            ALOGE("Invalid name for copybit_set_parameter");
            status = -EINVAL;
            break;

        }
    } else {
        status = -EINVAL;
    }

    return status;
}

/** Get a static copybit information */
static int copybit_get(struct copybit_device_t *dev, int name)
{
    struct copybit_context_t *context = (struct copybit_context_t *) dev;
    int value = 0;

    //ALOGI("%s: %d", __FUNCTION__, __LINE__);

    if (context) {
        switch(name) {
        case COPYBIT_MINIFICATION_LIMIT:
            value = MINIFICATION_LIMIT;
            break;
        case COPYBIT_MAGNIFICATION_LIMIT:
            value = MAGNIFICATION_LIMIT;
            break;
        case COPYBIT_SCALING_FRAC_BITS:
            value = SCALING_FRAC_BITS;
            break;
        case COPYBIT_ROTATION_STEP_DEG:
            value = ROTATION_STEP_DEG;
            break;
        default:
            value = -EINVAL;
            break;
        }
    } else {
        value = -EINVAL;
    }

    return value;
}


static int check_stretch_limits(const struct blt_rect *src_rect,
        const struct blt_rect *dst_rect, int rotation)
{
    int status = 0;

    struct blt_rect to;

    /* Determine dimensions to compare with */
    if (rotation) {
        to.width = dst_rect->height;
        to.height = dst_rect->width;
    } else {
        to.width = dst_rect->width;
        to.height = dst_rect->height;
    }

    /* Check horizontal stretch */
    if (src_rect->width > to.width) {
        if ((src_rect->width / to.width) > MINIFICATION_LIMIT) {
            ALOGE("Stretch beyond minification limit!");
            status = -EINVAL;
            goto error;
        }
    } else if (src_rect->width < to.width) {
        if ((to.width / src_rect->width) > MAGNIFICATION_LIMIT) {
            ALOGE("Stretch beyond magnification limit!");
            status = -EINVAL;
            goto error;
        }
    }

    /* Check vertical stretch */
    if (src_rect->height > to.height) {
        if ((src_rect->height / to.height) > MINIFICATION_LIMIT) {
            ALOGE("Stretch beyond minification limit!");
            status = -EINVAL;
            goto error;
        }
    } else if (src_rect->height < to.height) {
        if ((to.height / src_rect->height) > MAGNIFICATION_LIMIT) {
            ALOGE("Stretch beyond magnification limit!");
            status = -EINVAL;
            goto error;
        }
    }

error:
    return status;
}

/** Execute the stretch bit blit copy operation */
static int copybit_stretch(struct copybit_device_t *dev,
                           struct copybit_image_t const *dst,
                           struct copybit_image_t const *src,
                           struct copybit_rect_t const *dst_rect,
                           struct copybit_rect_t const *src_rect,
                           struct copybit_region_t const *region)
{
    struct copybit_context_t *context = (struct copybit_context_t *) dev;
    int status = 0;

    struct copybit_rect_t bounds = {0, 0, 0, 0};
    struct copybit_rect_t clip;

    ALOGD_IF(DEBUG_LOG_COPYBIT,"--%s: %d--pid=%d-context=%p---\n", __FUNCTION__,
                    __LINE__, getpid(), context);

    if (!context || !dst || !src || !dst_rect || !src_rect || !region) {
        ALOGE("%s: Inparam is NULL (context=%p, dst=%p, src=%p, "
            "dst_rect=%p, src_rect=%p, region=%p)!", __func__, context, dst,
            src, dst_rect, src_rect, region);
        status = -EINVAL;
        goto error;
    }

    bounds.r = dst->w;
    bounds.b = dst->h;

    /* Set up source image */
    if (convert_image(context->gralloc, src, &(context->blt_req.src_img)) < 0) {
        ALOGE("%s: Could not convert source image", __func__);
        status = -EINVAL;
        goto error;
    }

    /* Set up destination image */
    if (convert_image(context->gralloc, dst, &(context->blt_req.dst_img)) < 0) {
        ALOGE("%s: Could not convert destination image", __func__);
        status = -EINVAL;
        goto error;
    }

    /* If the source format is RGBX or destination format is YUV we shouldn't do any blending */
    if (src->format == COPYBIT_FORMAT_RGBX_8888 ||
        dst->format == COPYBIT_FORMAT_YCbCr_422_SP ||
        dst->format == COPYBIT_FORMAT_YCbCr_420_SP ||
        dst->format == COPYBIT_FORMAT_YCbCr_422_P  ||
        dst->format == COPYBIT_FORMAT_YCbCr_420_P  ||
        dst->format == COPYBIT_FORMAT_YCrCb_422_SP ||
        dst->format == COPYBIT_FORMAT_YCrCb_420_SP ||
        dst->format == COPYBIT_FORMAT_YCrCb_422_P  ||
        dst->format == COPYBIT_FORMAT_YCrCb_420_P  ||
        dst->format == COPYBIT_FORMAT_YCBCR42XMBN) {
        context->blt_req.flags &= ~(BLT_FLAG_PER_PIXEL_ALPHA_BLEND |
                                    BLT_FLAG_SRC_IS_NOT_PREMULT);
    } else {
        context->blt_req.flags |= (BLT_FLAG_PER_PIXEL_ALPHA_BLEND |
                                    BLT_FLAG_SRC_IS_NOT_PREMULT);
    }

    while (region->next(region, &clip)) {
        struct blt_req *req = &context->blt_req;

        /* Clamp the clip rectangle to the destination buffer bounds */
        intersect(&clip, &bounds, &clip);

        set_rects(src_rect, dst_rect, &clip, req->transform,
                &req->src_rect, &req->dst_rect);

        /* Check magnification/minification limits */
        status = check_stretch_limits(&context->blt_req.src_rect,
                &context->blt_req.dst_rect,
                (context->blt_req.transform & BLT_TRANSFORM_CCW_ROT_90));
        if (status < 0)
            goto error;

        /* If the resulting rectangle is empty, ignore it */
        if ((context->blt_req.dst_rect.width == 0) ||
                (context->blt_req.dst_rect.height == 0) ||
                (context->blt_req.src_rect.width == 0) ||
                (context->blt_req.src_rect.height == 0)) {
            ALOGD("Resulting rectangle is empty. Ignoring...");
            continue;
        }

        context->blt_req.dst_clip_rect.x = clip.l;
        context->blt_req.dst_clip_rect.y = clip.t;
        context->blt_req.dst_clip_rect.width = clip.r - clip.l;
        context->blt_req.dst_clip_rect.height = clip.b - clip.t;

        status = blt_request(context->blt_handle, &(context->blt_req));
        if (status < 0) {
            ALOGE("%s: blt request failed (%s)!", __func__, strerror(errno));
            goto error;
        }
    }

    status = blt_synch(context->blt_handle, 0);
    if (status < 0) {
        ALOGE("%s: blt synch failed (%s)!", __func__, strerror(errno));
        goto error;
    }

    ALOGD_IF(DEBUG_LOG_COPYBIT,"copybit_stretch src=(%d, %d, %d, %d) dst=(%d, %d, %d, %d)!",
            context->blt_req.src_rect.x, context->blt_req.src_rect.y,
            context->blt_req.src_rect.width, context->blt_req.src_rect.height,
            context->blt_req.dst_rect.x, context->blt_req.dst_rect.y,
            context->blt_req.dst_rect.width, context->blt_req.dst_rect.height);

    return 0;

error:
    return status;
}

/** Execute the bit blit copy operation */
static int copybit_blit(struct copybit_device_t *dev,
                        struct copybit_image_t const *dst,
                        struct copybit_image_t const *src,
                        struct copybit_region_t const *region)
{
    struct copybit_rect_t dr = { 0, 0, dst->w, dst->h };
    struct copybit_rect_t sr = { 0, 0, src->w, src->h };

    //ALOGI("%s: %d", __FUNCTION__, __LINE__);

    return copybit_stretch(dev, dst, src, &dr, &sr, region);
}

static int copybit_fill(struct copybit_device_t *dev,
                        struct copybit_image_t *dst,
                        struct copybit_rect_t *rect,
                        struct copybit_color_t *color,
                        struct copybit_region_t *region)
{
    int ret;
    struct blt_req req;
    struct copybit_context_t *context = (struct copybit_context_t *) dev;
    struct copybit_rect_t clip;

    if (!context || !dst || !rect || !color) {
        ALOGE("%s: Inparam NULL! context=%p, dst=%p, rect=%p, color=%p",
                __func__, context, dst, rect, color);
        return -EINVAL;
    }

    memset(&req, 0, sizeof(req));
    req.size = sizeof(req);

    if (convert_image(context->gralloc, dst, &req.dst_img) < 0) {
        ALOGE("%s: Could not convert source image!", __func__);
        return -EINVAL;
    }

    if (convert_color(color, dst->format, &req.src_color) < 0) {
        ALOGE("%s: Could not convert fill color!", __func__);
        return -EINVAL;
    }

    req.flags |= BLT_FLAG_SOURCE_FILL_RAW;
    req.src_img.fmt = req.dst_img.fmt;

    if (color->a < 255)
        req.flags |= BLT_FLAG_PER_PIXEL_ALPHA_BLEND;

    while (region->next(region, &clip)) {
        struct copybit_rect_t tmp;

        intersect(&tmp, &clip, rect);

        req.dst_rect.x = tmp.l;
        req.dst_rect.y = tmp.t;
        req.dst_rect.width = tmp.r - tmp.l;
        req.dst_rect.height = tmp.b - tmp.t;

        ret = blt_request(context->blt_handle, &req);
        if (ret < 0) {
            ALOGE("%s: blt_request failed (%s)", __func__, strerror(errno));
            return ret;
        }
    }

    blt_synch(context->blt_handle, 0);

    return 0;

}

/*****************************************************************************/

/** Close the copybit device */
static int close_copybit(struct hw_device_t *dev)
{
    struct copybit_context_t *context = (struct copybit_context_t *) dev;

    ALOGD_IF(DEBUG_LOG_COPYBIT,"-----%s: %d--pid=%d----\n", __FUNCTION__, __LINE__, getpid());

    if (context) {
        if (context->blt_handle >= 0)
            blt_close(context->blt_handle);

        free(context);
    }

    return 0;
}

/** Open a new instance of a copybit device using name */
static int open_copybit(const struct hw_module_t *module,
                        const char *name,
                        struct hw_device_t **device)
{
    int status;
    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo fvinfo;
    struct copybit_context_t *context = malloc(sizeof(struct copybit_context_t));

    ALOGD_IF(DEBUG_LOG_COPYBIT,"-----%s: %d---pid=%d---\n", __FUNCTION__, __LINE__, getpid());

    if (!context) {
        status = -ENOMEM;
        goto error_alloc_context;
    }
    memset(context, 0, sizeof(*context));

    context->device.common.tag = HARDWARE_DEVICE_TAG;
    context->device.common.version = 0;
    context->device.common.module = (struct hw_module_t *)module;
    context->device.common.close = close_copybit;
    context->device.set_parameter = copybit_set_parameter;
    context->device.get = copybit_get;
    context->device.blit = copybit_blit;
    context->device.stretch = copybit_stretch;
    context->device.fill = copybit_fill;

    context->blt_req.size = sizeof(struct blt_req);
    context->blt_req.prio = BLT_DEFAULT_PRIO;

    /* Always set these flags since they will have no effect for a
       source without alpha */
    context->blt_req.flags = BLT_FLAG_PER_PIXEL_ALPHA_BLEND |
                                            BLT_FLAG_SRC_IS_NOT_PREMULT;

    context->blt_handle = blt_open();
    if (context->blt_handle < 0) {
        ALOGE("Error opening BLT library %d", context->blt_handle);
        status = context->blt_handle;
        goto error_open_blt;
    }

    status = hw_get_module(GRALLOC_HARDWARE_MODULE_ID,
            (const struct hw_module_t **)&context->gralloc);
    if (status) {
        ALOGE("Error opening gralloc module");
        goto error_open_gralloc;
    }

    *device = &(context->device.common);

    return 0;

error_open_gralloc:
    blt_close(context->blt_handle);
error_open_blt:
    free(context);
error_alloc_context:

    return status;
}
