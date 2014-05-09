/*
 * Copyright (C) 2014 Android Open Source Project
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

#ifndef SYSTEM_CORE_INCLUDE_ANDROID_GRAPHICS_H
#define SYSTEM_CORE_INCLUDE_ANDROID_GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>

/*
 * If the HAL needs to create service threads to handle graphics related
 * tasks, these threads need to run at HAL_PRIORITY_URGENT_DISPLAY priority
 * if they can block the main rendering thread in any way.
 *
 * the priority of the current thread can be set with:
 *
 *      #include <sys/resource.h>
 *      setpriority(PRIO_PROCESS, 0, HAL_PRIORITY_URGENT_DISPLAY);
 *
 */

#define HAL_PRIORITY_URGENT_DISPLAY     (-8)

/**
 * pixel format definitions
 */

enum {
    HAL_PIXEL_FORMAT_RGBA_8888          = 1,
    HAL_PIXEL_FORMAT_RGBX_8888          = 2,
    HAL_PIXEL_FORMAT_RGB_888            = 3,
    HAL_PIXEL_FORMAT_RGB_565            = 4,
    HAL_PIXEL_FORMAT_BGRA_8888          = 5,
    HAL_PIXEL_FORMAT_RGBA_5551          = 6,
    HAL_PIXEL_FORMAT_RGBA_4444          = 7,

/*
     * sRGB color pixel formats:
     *
     * The red, green and blue components are stored in sRGB space, and converted
     * to linear space when read, using the standard sRGB to linear equation:
     *
     * Clinear = Csrgb / 12.92                  for Csrgb <= 0.04045
     *         = (Csrgb + 0.055 / 1.055)^2.4    for Csrgb >  0.04045
     *
     * When written the inverse transformation is performed:
     *
     * Csrgb = 12.92 * Clinear                  for Clinear <= 0.0031308
     *       = 1.055 * Clinear^(1/2.4) - 0.055  for Clinear >  0.0031308
     *
     *
     *  The alpha component, if present, is always stored in linear space and
     *  is left unmodified when read or written.
     *
     */
    HAL_PIXEL_FORMAT_sRGB_A_8888        = 0xC,
    HAL_PIXEL_FORMAT_sRGB_X_8888        = 0xD,

 /*
     * Android Y8 format:
     *
     * This format is exposed outside of the HAL to the framework.
     * The expected gralloc usage flags are SW_* and HW_CAMERA_*,
     * and no other HW_ flags will be used.
     *
     * Y8 is a YUV planar format comprised of a WxH Y plane,
     * with each pixel being represented by 8 bits.
     *
     * It is equivalent to just the Y plane from YV12.
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels
     * - a vertical stride equal to the height
     *
     *   size = stride * height
     *
     */
    HAL_PIXEL_FORMAT_Y8     = 0x20203859,
    
    /*
     * Android Y16 format:
     *
     * This format is exposed outside of the HAL to the framework.
     * The expected gralloc usage flags are SW_* and HW_CAMERA_*,
     * and no other HW_ flags will be used.
     *
     * Y16 is a YUV planar format comprised of a WxH Y plane,
     * with each pixel being represented by 16 bits.
     *
     * It is just like Y8, but has double the bits per pixel (little endian).
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels
     * - a vertical stride equal to the height
     * - strides are specified in pixels, not in bytes
     *
     *   size = stride * height * 2
     *
     */
    HAL_PIXEL_FORMAT_Y16    = 0x20363159,
    /* 0x8 - 0xFF range unavailable */

    /*
     * 0x100 - 0x1FF
     *
     * This range is reserved for pixel formats that are specific to the HAL
     * implementation.  Implementations can use any value in this range to
     * communicate video pixel formats between their HAL modules.  These formats
     * must not have an alpha channel.  Additionally, an EGLimage created from a
     * gralloc buffer of one of these formats must be supported for use with the
     * GL_OES_EGL_image_external OpenGL ES extension.
     */

    /*
     * Android YUV format:
     *
     * This format is exposed outside of the HAL to software decoders and
     * applications.  EGLImageKHR must support it in conjunction with the
     * OES_EGL_image_external extension.
     *
     * YV12 is a 4:2:0 YCrCb planar format comprised of a WxH Y plane followed
     * by (W/2) x (H/2) Cr and Cb planes.
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels
     * - a vertical stride equal to the height
     *
     *   y_size = stride * height
     *   c_stride = ALIGN(stride/2, 16)
     *   c_size = c_stride * height/2
     *   size = y_size + c_size * 2
     *   cr_offset = y_size
     *   cb_offset = y_size + c_size
     *
     */
    HAL_PIXEL_FORMAT_YV12   = 0x32315659, // YCrCb 4:2:0 Planar

    /*
     * Android RAW sensor format:
     *
     * This format is exposed outside of the HAL to applications.
     *
     * RAW_SENSOR is a single-channel 16-bit format, typically representing raw
     * Bayer-pattern images from an image sensor, with minimal processing.
     *
     * The exact pixel layout of the data in the buffer is sensor-dependent, and
     * needs to be queried from the camera device.
     *
     * Generally, not all 16 bits are used; more common values are 10 or 12
     * bits. All parameters to interpret the raw data (black and white points,
     * color space, etc) must be queried from the camera device.
     *
     * This format assumes
     * - an even width
     * - an even height
     * - a horizontal stride multiple of 16 pixels (32 bytes).
     */

    /* STE: Added Support of YUV42XMBN, required for Copybit CC acceleration */
    HAL_PIXEL_FORMAT_YCBCR42XMBN = 0xE,
    /* Legacy formats (deprecated), used by ImageFormat.java */
    HAL_PIXEL_FORMAT_YCbCr_422_SP       = 0x10, // NV16
    HAL_PIXEL_FORMAT_YCrCb_420_SP       = 0x11, // NV21
    HAL_PIXEL_FORMAT_YCbCr_422_P        = 0x12,
    HAL_PIXEL_FORMAT_YCbCr_420_P        = 0x13,
    HAL_PIXEL_FORMAT_YCbCr_422_I        = 0x14, // YUY2
    HAL_PIXEL_FORMAT_YCbCr_420_I        = 0x15,
    HAL_PIXEL_FORMAT_CbYCrY_422_I       = 0x16,
    HAL_PIXEL_FORMAT_CbYCrY_420_I       = 0x17,
    HAL_PIXEL_FORMAT_YCbCr_420_SP_TILED = 0x20,
    HAL_PIXEL_FORMAT_YCbCr_420_SP       = 0x21,
    HAL_PIXEL_FORMAT_YCrCb_420_SP_TILED = 0x22,
    HAL_PIXEL_FORMAT_YCrCb_422_SP       = 0x23,
    HAL_PIXEL_FORMAT_YCrCb_422_P        = 0x24,
    HAL_PIXEL_FORMAT_YCrCb_420_P        = 0x25,
};


/**
 * Transformation definitions
 *
 * IMPORTANT NOTE:
 * HAL_TRANSFORM_ROT_90 is applied CLOCKWISE and AFTER HAL_TRANSFORM_FLIP_{H|V}.
 *
 */

enum {
    /* flip source image horizontally (around the vertical axis) */
    HAL_TRANSFORM_FLIP_H    = 0x01,
    /* flip source image vertically (around the horizontal axis)*/
    HAL_TRANSFORM_FLIP_V    = 0x02,
    /* rotate source image 90 degrees clockwise */
    HAL_TRANSFORM_ROT_90    = 0x04,
    /* rotate source image 180 degrees */
    HAL_TRANSFORM_ROT_180   = 0x03,
    /* rotate source image 270 degrees clockwise */
    HAL_TRANSFORM_ROT_270   = 0x07,
};

enum {
    HAL_PIXEL_FORMAT_RAW_SENSOR = 0x20,
    HAL_PIXEL_FORMAT_BLOB = 0x21,
    HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED = 0x22,
    HAL_PIXEL_FORMAT_YCbCr_420_888 = 0x23,
};

struct android_ycbcr {
    void *y;
    void *cb;
    void *cr;
    size_t ystride;
    size_t cstride;
    size_t chroma_step;

    /** reserved for future use, set to 0 by gralloc's (*lock_ycbcr)() */
    uint32_t reserved[8];
};

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_CORE_INCLUDE_ANDROID_GRAPHICS_H */
