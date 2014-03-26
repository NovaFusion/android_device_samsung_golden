/*
 * Copyright (C) ST-Ericsson AB 2009 - All rights reserved
 * Reproduction and Communication of this document is strictly prohibited
 * unless specifically authorized in writing by ST-Ericsson
 *
 * \file    blt_api.h
 * \brief   Blitting library userspace API
 * \author  ST-Ericsson
 *
 */


#ifdef __cplusplus
extern "C" {
#endif

#ifndef _BLT_API_H
#define _BLT_API_H

#include <stdint.h>

/**
 * @brief Specifies a blit rectangle
 *
 * This type specifies a blit rectangle.
 */
struct blt_rect {
    /**
     * @brief X-coordinate of top left corner
     */
    int32_t  x;
    /**
     * @brief Y-coordinate of top left corner
     */
    int32_t  y;
    /**
     * @brief Rectangle width. Must be >= 0.
     */
    int32_t  width;
    /**
     * @brief Rectangle height. Must be >= 0.
     */
    int32_t  height;
};

/**
 * @brief Defines the available buffer formats.
 *        Inspired by Khronos OpenMAX, please see
 *        OpenMAX IL specification for detailed descriptions of the formats.
 *        All OpenMAX formats are enumerated for possible future use but only
 *        a subset is supported by the actual hardware.
 */
enum blt_fmt {
    /**
     * @brief Placeholder value when format is unknown,
     *        or specified using vendor specific means.
     */
    BLT_FMT_UNUSED = 0,
    /*BLT_FMT_MONOCHROME = 1,*/
    /*BLT_FMT_8_BIT_RGB332 = 2,*/
    /*BLT_FMT_12_BIT_RGB444 = 3,*/
    /**
     * @brief 16 bits per pixel ARGB format with colors stored as Alpha 15:12,
     *        Red 11:8, Green 7:4, and Blue 3:0.
     */
    BLT_FMT_16_BIT_ARGB4444 = 4,
    /**
     * @brief 16 bits per pixel ARGB format with colors stored as Alpha 15,
     *        Red 14:10, Green 9:5, and Blue 4:0.
     */
    BLT_FMT_16_BIT_ARGB1555 = 5,
    /**
     * @brief 16 bits per pixel RGB format with colors stored as Red 15:11,
     *        Green 10:5, and Blue 4:0.
     */
    BLT_FMT_16_BIT_RGB565 = 6,
    /*BLT_FMT_16_BIT_BGR565 = 7,*/
    /*BLT_FMT_18_BIT_RGB666 = 8,*/
    /*BLT_FMT_18_BIT_ARGB1665 = 9,*/
    /*BLT_FMT_19_BIT_ARGB1666 = 10, */
    /**
     * @brief 24 bits per pixel RGB format with colors stored as Red 23:16,
     *        Green 15:8, and Blue 7:0.
     */
    BLT_FMT_24_BIT_RGB888 = 11,
    /*BLT_FMT_24_BIT_BGR888 = 12,*/
    /*BLT_FMT_24_BIT_ARGB1887 = 13,*/
    /*BLT_FMT_25_BIT_ARGB1888 = 14,*/
    /*BLT_FMT_32_BIT_BGRA8888 = 15,*/
    /**
     * @brief 32 bits per pixel ARGB format with colors stored as Alpha 31:24,
     *        Red 23:16, Green 15:8, and Blue 7:0.
     */
    BLT_FMT_32_BIT_ARGB8888 = 16,
    /*BLT_FMT_YUV_411_PLANAR = 17,*/
    /*BLT_FMT_YUV_411_PACKED_PLANAR = 18,*/
    /*BLT_FMT_YUV_420_PLANAR = 19,*/
    /**
     * @brief YUV planar format, organized with three separate planes,
     *        one for each color component, namely Y, U, and V.
     *        U and V pixels are subsampled by a factor of two
     *        both horizontally and vertically.
     *        The buffer shall contain a plane of Y, U, and V data in this
     *        order.
     */
    BLT_FMT_YUV420_PACKED_PLANAR = 20,
    /*BLT_FMT_YUV420_SEMI_PLANAR = 21,*/
    /*BLT_FMT_YUV422_PLANAR = 22,*/
    /**
     * @brief YUV planar format, organized with three separate planes,
     *        one for each color component, namely Y, U, and V.
     *        U and V pixels are subsampled by a factor of two horizontally.
     *        The buffer shall contain a plane of Y, U, and V data in this
     *        order.
     */
    BLT_FMT_YUV422_PACKED_PLANAR = 23,
    /*BLT_FMT_YUV422_SEMI_PLANAR = 24,*/
    /**
     * @brief 16 bits per pixel YUV interleaved format organized as YUYV
     *        (i.e., YCbYCr).
     *        (Corresponds to YUV422 interleaved)
     */
    BLT_FMT_Y_CB_Y_CR = 25,
    /*BLT_FMT_Y_CR_Y_CB = 26,*/
    /**
     * @brief 16 bits per pixel YUV interleaved format organized as UYVY
     *        (i.e., CbYCrY).
     *        (Corresponds to YUV422R)
     */
    BLT_FMT_CB_Y_CR_Y = 27,
    /*BLT_FMT_CR_Y_CB_Y=28,*/
    /*BLT_FMT_YUV_444_INTERLEAVED=29,*/
    /*BLT_FMT_RAW_BAYER_8_BIT=30,*/
    /*BLT_FMT_RAW_BAYER_10_BIT=31,*/
    /*BLT_FMT_RAW_BAYER_8_BIT_COMPRESSED=32,*/
    /*BLT_FMT_L2=33, */
    /*BLT_FMT_L4=34, */
    /*BLT_FMT_L8=35, */
    /*BLT_FMT_L16=36, */
    /*BLT_FMT_L24=37, */
    /*BLT_FMT_L32=38,*/
    /**
     * @brief YUV planar format, organized with a first plane containing Y
     *        pixels, and a second plane containing U and V pixels interleaved
     *        with the first U value first. U and V pixels are subsampled by
     *        a factor of two both horizontally and vertically. The buffer
     *        shall contain a plane of Y, U and V data.
     *        (Same as B2R2 420 Raster 2 buffer - 420 R2B)
     */
    BLT_FMT_YUV420_PACKED_SEMI_PLANAR=39,
    /**
     * @brief YUV planar format, organized with a first plane containing Y
     *        pixels, and a second plane containing U and V pixels interleaved
     *        with the first U value first. U and V pixels are subsampled by
     *        a factor of two horizontally. The buffer shall contain a plane
     *        of Y, U and V data.
     *        (Same as B2R2 422 Raster 2 buffer - 422 R2B)
     */
    BLT_FMT_YUV422_PACKED_SEMI_PLANAR=40,
    /*BLT_FMT_18_BIT_BGR_666=41,*/
    /*BLT_FMT_24_BIT_ARGB_6666=42,*/
    /*BLT_FMT_24_BIT_ABGR_6666=43,*/

    /* Extensions, non OpenMAX formats */

    /**
     * @brief 32 bits per pixel ABGR format with colors stored as Alpha 31:24,
     *        Blue 23:16, Green 15:8, and Red 7:0.
     */
    BLT_FMT_32_BIT_ABGR8888 = 0x7F000000, /* OpenMax vendor start */
    /**
     * @brief 24 bits per pixel ARGB format with colors stored as Alpha 23:16,
     *        Red 15:11, Green 10:5, and Blue 4:0.
     */
    BLT_FMT_24_BIT_ARGB8565 = 0x7F000001,
    /**
     * @brief 24 bits per pixel YUV format with colors stored as
     *        Y 23:16, U 15:8, and V 7:0.
     */
    BLT_FMT_24_BIT_YUV888 = 0x7F000002,
    /**
     * @brief 32 bits per pixel AYUV format with colors stored as
     *        Alpha 31:24, Y 23:16, U 15:8, and V 7:0.
     */
    BLT_FMT_32_BIT_AYUV8888 = 0x7F000003,
    /**
     * @brief Nomadik YUV 420 macro block format, see B2R2 specification for details
     */
    BLT_FMT_YUV420_PACKED_SEMIPLANAR_MB_STE = 0x7F000004,
    /**
     * @brief Nomadik YUV 422 macro block format, see B2R2 specification for details
     */
    BLT_FMT_YUV422_PACKED_SEMIPLANAR_MB_STE = 0x7F000005,
    /**
     * @brief 1 bit per pixel alpha format
     */
    BLT_FMT_1_BIT_A1 = 0x7F000006,
    /**
     * @brief 8 bit per pixel alpha format
     */
    BLT_FMT_8_BIT_A8 = 0x7F000007,
    /**
     * @brief YUV planar format, organized with
     * three separate planes, one for each color component, namely Y, U, and V.
     * All planes use full resolution, there is no subsampling.
     * The buffer shall contain a plane of Y, U, and V data in this order.
     */
    BLT_FMT_YUV444_PACKED_PLANAR = 0x7F000008,
    /**
     * @brief YVU planar format, organized with a first plane containing Y
     *        pixels, and a second plane containing V and U pixels interleaved
     *        with the first V value first. V and U pixels are subsampled by
     *        a factor of two both horizontally and vertically. The buffer
     *        shall contain two planes, one with Y, and one with V and U data.
     *        (Same as B2R2 420 Raster 2 buffer - 420 R2B except that chroma
     *        order is swapped.)
     */
    BLT_FMT_YVU420_PACKED_SEMI_PLANAR = 0x7F000009,
    /**
     * @brief YVU planar format, organized with a first plane containing Y
     *        pixels, and a second plane containing V and U pixels interleaved
     *        with the first V value first. V and U pixels are subsampled by
     *        a factor of two both horizontally. The buffer shall contain
     *        two planes, one with Y, and one with V and U data.
     *        (Same as B2R2 422 Raster 2 buffer - 422 R2B except that chroma
     *        order is swapped.)
     */
    BLT_FMT_YVU422_PACKED_SEMI_PLANAR = 0x7F00000A,
    /**
     * @brief YVU planar format, organized with three separate planes,
     *        one for each color component, namely Y, V, and U.
     *        V and U pixels are subsampled by a factor of two
     *        both horizontally and vertically.
     *        The buffer shall contain a plane of Y, V, and U data in this
     *        order. (Same as BLT_FMT_YUV420_PACKED_PLANAR, except that chroma
     *        order is swapped.)
     */
    BLT_FMT_YVU420_PACKED_PLANAR = 0x7F00000B,
    /**
     * @brief YVU planar format, organized with three separate planes,
     *        one for each color component, namely Y, V, and U.
     *        V and U pixels are subsampled by a factor of two horizontally.
     *        The buffer shall contain a plane of Y, V, and U data in this
     *        order. (Same as BLT_FMT_YUV422_PACKED_PLANAR except that chroma
     *        order is swapped.)
     */
    BLT_FMT_YVU422_PACKED_PLANAR = 0x7F00000C,
    /**
     * @brief 24 bits per pixel VUY format with colors stored as
     *        V 23:16, U 15:8, and Y 7:0.
     */
    BLT_FMT_24_BIT_VUY888 = 0x7F00000D,
    /**
     * @brief 32 bits per pixel VUYA format with colors stored as
     *        V 31:24, U 23:16, Y 15:8, and Alpha 7:0.
     */
    BLT_FMT_32_BIT_VUYA8888 = 0x7F00000E,
};


/**
 * @brief Specifies a buffer pointer
 */
enum blt_ptr_type {
    /**
     * @brief No pointer (NULL). E.g. src fill.
     */
    BLT_PTR_NONE,
    /**
     * @brief Use offset as a userspace virtual address
     */
    BLT_PTR_VIRTUAL,
    /**
     * @brief Use offset as a physical address
     */
    BLT_PTR_PHYSICAL,
    /**
     * @brief Use fd and offset to determine buffer location.
     */
    BLT_PTR_FD_OFFSET,
    /**
     * @brief Use hwmem_buf_name and offset to determine buffer location.
     */
    BLT_PTR_HWMEM_BUF_NAME_OFFSET,
};

/**
 * @brief Specifies a buffer
 */
struct blt_buf {
    /**
     * @brief Pointer type
     */
    enum blt_ptr_type  type;
    /**
     * @brief Hwmem buffer name.
     */
    int32_t            hwmem_buf_name;
    /**
     * @brief File descriptor (e.g. file handle to a pmem device)
     */
    int32_t            fd;
    /**
     * @brief Offset where buffer can be found. When used in 
     * conjunction with a hwmem buffer the offset must be a multiple 
     * of the image size. If 'type' is set to BLT_PTR_VIRTUAL or 
     * BLT_PTR_PHYSICAL, then the offset is interpreted as an address. 
     */
    uint32_t           offset;
    /**
     * @brief Size of buffer in bytes
     */
    uint32_t           len;
	/**
	 * @brief Pointer to the bitmap data. This field can be used to specify
	 * an alternative way to access the buffer. Whenever the 'bits' pointer
	 * is set to non-NULL, the underlying implementation is free to decide 
     * whether or not to use it in favor of other ways to locate the buffer.
	 */
	void *bits;
};


/**
 * @brief Specifies a image
 */
struct blt_img {
    /**
     * @brief Pixel format of image
     */
    enum blt_fmt       fmt;
    /**
     * @brief Buffer pointer
     */
    struct blt_buf     buf;
    /**
     * @brief Width in pixels
     */
    int32_t            width;
    /**
     * @brief Height in pixels
     */
    int32_t            height;
    /**
     * @brief Pitch in bytes (set to zero for default based on width)
     */
    uint32_t           pitch;
};


/**
 * @brief Specifies rotation and flipping, mutually exclusive
 */
enum blt_transform {
    /**
     * @brief No rotation or flip
     */
    BLT_TRANSFORM_NONE             = 0,
    /**
     * @brief Flip horizontally
     */
    BLT_TRANSFORM_FLIP_H           = 1,
    /**
     * @brief Flip vertically
     */
    BLT_TRANSFORM_FLIP_V           = 2,
    /**
     * @brief Rotate 90 degrees counter clockwise
     */
    BLT_TRANSFORM_CCW_ROT_90        = 4,
    /**
     * @brief Rotate 180 degrees (same as flip horizontally together with flip
     * vertically)
     */
    BLT_TRANSFORM_CCW_ROT_180       = 3,
    /**
     * @brief Rotate 270 degrees counter clockwise
     */
    BLT_TRANSFORM_CCW_ROT_270       = 7,
    /**
     * @brief Flip horizontally and then rotate 90 degrees counter clockwise
     */
    BLT_TRANSFORM_FLIP_H_CCW_ROT_90 = 5,
    /**
     * @brief Flip vertically and then rotate 90 degrees counter clockwise
     */
    BLT_TRANSFORM_FLIP_V_CCW_ROT_90 = 6,
};


/**
 * @brief Flags that specifies the request to the blt_api
 *
 * Can be combined
 */
enum blt_flag {
    /**
     * @brief Asynchronous request. blt_request will return when the request
     *        has been queued.
     */
    BLT_FLAG_ASYNCH                = 0x1,
    /**
     * @brief Dry run, just to check if request can be performed.
     */
    BLT_FLAG_DRY_RUN               = 0x2,
    /**
     * @brief Enable per pixel alpha blend
     *        (assumes alpha is embedded in source pixel format).
     */
    BLT_FLAG_PER_PIXEL_ALPHA_BLEND = 0x4,
    /**
     * @brief Enable global alpha blend (alpha value in global_alpha).
     */
    BLT_FLAG_GLOBAL_ALPHA_BLEND    = 0x8,
    /**
     * @brief Enable source color key (color in src_color). Color should
     * be in raw format.
     *
     * BLT_FLAG_SOURCE_COLOR_KEY, BLT_FLAG_SOURCE_FILL and BLT_FLAG_SOURCE_FILL_RAW
     * cannot be specified at the same time. BLT_FLAG_SOURCE_COLOR_KEY and
     * BLT_FLAG_DEST_COLOR_KEY cannot be specified at the same time.
     */
    BLT_FLAG_SOURCE_COLOR_KEY      = 0x10,
    /**
     * @brief Enable ARGB/AYUV source fill (color in src_color). Which of
     *        ARGB and AYUV is determined by destination format.
     *
     * BLT_FLAG_SOURCE_COLOR_KEY, BLT_FLAG_SOURCE_FILL and BLT_FLAG_SOURCE_FILL_RAW
     * cannot be specified at the same time.
     */
    BLT_FLAG_SOURCE_FILL           = 0x20,
    /**
     * @brief Enable raw source fill (color in src_color)
     *
     * BLT_FLAG_SOURCE_COLOR_KEY, BLT_FLAG_SOURCE_FILL and BLT_FLAG_SOURCE_FILL_RAW
     * cannot be specified at the same time.
     */
    BLT_FLAG_SOURCE_FILL_RAW       = 0x40,
    /**
     * @brief Enable dest color key (color in dst_color). Color should be in raw format.
     * BLT_FLAG_SOURCE_COLOR_KEY and BLT_FLAG_DEST_COLOR_KEY cannot be specified at the same time.
     */
    BLT_FLAG_DEST_COLOR_KEY        = 0x80,
    /**
     * @brief Source color not premultiplied (Valid only for formats with embedded alpha).
     */
    BLT_FLAG_SRC_IS_NOT_PREMULT    = 0x100,
    /**
     * @brief Enable dithering.
     */
    BLT_FLAG_DITHER                = 0x200,
    /**
     * @brief Enable blur.
     */
    BLT_FLAG_BLUR                  = 0x400,
    /**
     * @brief Enable source mask.
     */
    BLT_FLAG_SOURCE_MASK           = 0x800,
    /**
     * @brief Enable destination clip rectangle.
     */
    BLT_FLAG_DESTINATION_CLIP      = 0x1000,
    /**
     * @brief Inherit process priority.
     */
    BLT_FLAG_INHERIT_PRIO          = 0x2000,
    /**
     * @brief Indicate that a background buffer for blend operation is supplied.
     *
     * B2R2_BLT_FLAG_PER_PIXEL_ALPHA_BLEND, B2R2_BLT_FLAG_SRC_IS_NOT_PREMULT, and
     * B2R2_BLT_FLAG_GLOBAL_ALPHA_BLEND will control the blend operation.
     */
    BLT_FLAG_BG_BLEND              = 0x20000,
    /**
     * @brief Use color look-up table for color correction.
     * Pointer to the table must be specified in *clut field of
     * the b2r2_blt_req structure.
     * The table must map all input color values
     * for each channel to the desired output values.
     * It is an array with the following format:
     * R0 G0 B0 A0 R1 G1 B1 A1...R255 G255 B255 A255
     * where R0 is the 8 bit output value for red channel whenever its input
     * equals 0.
     * Similarly, R1 through R255 are the red channel outputs whenever
     * the channel's inputs equal 1 through 255 respectively.
     * Gn, Bn, An denote green, blue and alpha channel.
     * Whenever the input bitmap format lacks the alpha channel,
     * all alpha values in the color correction table should be set to 255.
     * Size of the array that specifies the color correction table
     * must be 1024 bytes.
     * A table that does not change anything has the form:
     * 0 0 0 0 1 1 1 1 2 2 2 2 ... 254 254 254 254 255 255 255 255.
     * CLUT color correction can be applied to YUV raster buffers as well,
     * in which case the RGB color channels are mapped onto YUV-space
     * as follows:
     * R = red chrominance
     * G = luminance
     * B = blue chrominance
     * A = alpha
     * If any of the planar or semi-planar formats is used, luminance cannot
     * be changed by the color correction table.
     */
    BLT_FLAG_CLUT_COLOR_CORRECTION = 0x80000000
};

/**
 * @brief Specifies a request to blt_api.
 */
struct blt_req {
    /**
     * @brief Size of this structure (used for versioning). Must be set to the size of this structure.
     */
    uint32_t              size;
    /**
     * @brief Flags that control the BLT request.
     */
    enum    blt_flag      flags;
    /**
     * @brief Flip & rotate.
     */
    enum    blt_transform transform;
    /**
     * @brief Priority (-20 to 19). Inherits process prio
     * if BLT_FLAG_INHERIT_PRIO is set. Given priority is mapped onto BLT.
     * TBD: How?
     */
    int32_t               prio;
    /**
     * @brief Pointer to the look-up table for color correction.
     */
	void                  *clut;
    /**
     * @brief Source image. Ignored when source fill is specified.
     */
    struct blt_img        src_img;
    /**
     * @brief Source mask image. Ignored when source fill is specified.
     */
    struct blt_img        src_mask;
    /**
     * @brief Source area to be blitted.
     */
    struct blt_rect       src_rect;
    /**
     * @brief Source fill color or color key.
     */
    uint32_t              src_color;
    /**
     * @brief Background image.
     */
    struct blt_img        bg_img;
    /**
     * @brief Background area to be blitted.
     */
    struct blt_rect       bg_rect;
    /**
     * @brief Destination image.
     */
    struct blt_img        dst_img;
    /**
     * @brief Destination area for the blit.
     */
    struct blt_rect       dst_rect;
    /**
     * @brief Destination clip rectangle.
     */
    struct blt_rect       dst_clip_rect;
    /**
     * @brief Destination color key.
     */
    uint32_t              dst_color;
    /**
     * @brief Global alpha value (0 - 255).
     */
    uint8_t               global_alpha;
    /**
     * @brief Callback function, will be called when request is finished.
     * It will be called from another thread. May be NULL.
     */
    void (*callback)(int request_id, uint32_t callback_data);
    /**
     * @brief Data to callback function.
     */
    uint32_t               callback_data;
};

/**
 * @brief Capabilities that can be queried for. Capabilities can be queried
 *        for a specific format or for formats in general. To query for
 *        capabilities in general, specify BLT_FMT_UNUSED as format.
 */
enum blt_cap {
    /**
     * @brief Unused/unspecified capability.
     */
    BLT_CAP_UNUSED = 0,
    /**
     * @brief Is format supported as source?
     */
    BLT_CAP_FMT_SOURCE,
    /**
     * @brief Is format supported as source mask?
     */
    BLT_CAP_FMT_SOURCE_MASK,
    /**
     * @brief Is format supported as destination?
     */
    BLT_CAP_FMT_DEST,
    /**
     * @brief Is per pixel alpha blending supported with format as source?
     */
    BLT_CAP_PER_PIXEL_ALPHA_BLEND,
    /**
     * @brief Is global alpha blending supported with format as source?
     */
    BLT_CAP_GLOBAL_ALPHA_BLEND,
    /**
     * @brief Is source color key supported with format as source?
     */
    BLT_CAP_SOURCE_COLOR_KEY,
    /**
     * @brief Is source fill supported with format as source?
     */
    BLT_CAP_SOURCE_FILL,
    /**
     * @brief Is source fill raw supported with format as dest?
     */
    BLT_CAP_SOURCE_FILL_RAW,
    /**
     * @brief Is dest color key supported with format as dest?
     */
    BLT_CAP_DEST_COLOR_KEY,
    /**
     * @brief Is dithering supported with format as source?
     */
    BLT_CAP_DITHER,
    /**
     * @brief Is blur supported with format as source?
     */
    BLT_CAP_BLUR,
    /**
     * @brief Minification limit (copybit support).
     */
    BLT_CAP_MINIFICATION_LIMIT,
    /**
     * @brief Magnification limit (copybit support).
     */
    BLT_CAP_MAGNIFICATION_LIMIT,
    /**
     * @brief Number of scaling fractional bits (copybit support).
     */
    BLT_CAP_SCALING_FRAC_BITS,
    /**
     * @brief Supported rotation step in degrees (copybit support).
     */
    BLT_CAP_ROTATION_STEP_DEG,
};

/**
 * The blt_open() function opens the blit library and returns
 * a handle to it.
 *
 * @return Returns a handle to the blit library if >= 0,
 *         else a negative error code
 * @retval -ESOMERROR Description of an error
 *
 * @see blt_close()
 */
int blt_open(void);

/**
 * The blt_close() function closes the blit library.
 *
 * @param [in] blt_handle Handle to the blit library.
 *
 * @return Returns nothing
 *
 * @see blt_open()
 */
void blt_close(int blt_handle);

/**
 * The blt_request() adds a blit request.
 *
 * blt_request() returns when the blit has been performed unless
 *            asynchronous execution has been specified. In that case,
 *            control is returned as soon as the request has been queued.
 *
 * @param [in] blt_handle Handle to the blit library.
 * @param [in] req Blit request structure
 *
 * @return Returns a unique request id if >= 0, else a negative error code.
 *         This request id can be waited for using blt_synch().
 * @retval -ESOMERROR Description of an error
 *
 * @see blt_synch()
 */
int blt_request(int blt_handle, struct blt_req *req);

/**
 * blt_synch() waits for all or for a specific request to be finished.
 *
 * @param [in] blt_handle Handle to the blit library.
 * @param [in] request_id Request id returned by blt_request or 0 to specify all
 * requests.
 *
 * @return Returns 0 upon success, a negative error code otherwise.
 * @retval -ESOMERROR Description of an error
 *
 * @see blt_request()
 */
int blt_synch(int blt_handle, int request_id);

/**
 * blt_query_cap() returns capability information for all or
 *               for a certain format
 *
 * @param [in] blt_handle Handle to the blit library.
 * @param [in] fmt Format to query capabilities for or BLT_FMT_UNUSED to consider
 * all formats.
 * @param [in] capability Capability to query for.
 * @param [out] cap Returned capability. Interpretaion of this variable varies
 *             with the capability queried.
 *
 * @return Returns 0 upon success, a negative error code otherwise.
 * @retval -ESOMERROR Description of an error
 */
int blt_query_cap(int blt_handle,
		enum blt_fmt fmt,
		enum blt_cap capability,
		uint32_t *cap);

#endif /* #ifdef _BLT_API_H */

#ifdef __cplusplus
}
#endif
