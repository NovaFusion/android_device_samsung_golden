/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * ST-Ericsson B2R2 user interface
 *
 * Author: Robert Fekete <robert.fekete@stericsson.com>
 * Author: Paul Wannback
 * for ST-Ericsson.
 *
 * License terms: GNU General Public License (GPL), version 2.
 */


#ifndef _LINUX_VIDEO_B2R2_BLT_H
#define _LINUX_VIDEO_B2R2_BLT_H

#include <linux/types.h>

#if defined(__KERNEL__)
#include <linux/mm_types.h>
#include <linux/bitops.h>
#else
#define BIT(nr)			(1UL << (nr))
#endif

/**
 * struct b2r2_blt_rect - Specifies a B2R2 rectangle
 *
 * @left: X-coordinate of top left corner
 * @top: Y-coordinate of top left corner
 * @width: Rectangle width. Must be >= 0.
 * @height: Rectangle height. Must be >= 0.
 */
struct b2r2_blt_rect {
	__s32  x;
	__s32  y;
	__s32  width;
	__s32  height;
};

/**
 * enum b2r2_blt_fmt - Defines the available B2R2 buffer formats
 *
 *        Inspired by Khronos OpenMAX, please see
 *        OpenMAX IL specification for detailed descriptions of the formats
 *
 * @B2R2_BLT_FMT_UNUSED: Placeholder value when format is unknown,
 * or specified using a vendor-specific means.
 * @B2R2_BLT_FMT_16_BIT_ARGB4444: 16 bits per pixel ARGB format with colors
 * stored as Alpha 15:12, Red 11:8, Green 7:4, and Blue 3:0.
 * @B2R2_BLT_FMT_16_BIT_ARGB1555: 16 bits per pixel ARGB format with colors
 * stored as Alpha 15, Red 14:10, Green 9:5, and Blue 4:0.
 * @B2R2_BLT_FMT_16_BIT_RGB565: 16 bits per pixel RGB format with colors
 * stored as Red 15:11, Green 10:5, and Blue 4:0.
 * @B2R2_BLT_FMT_24_BIT_RGB888: 24 bits per pixel RGB format with colors
 * stored as Red 23:16, Green 15:8, and Blue 7:0.
 * @B2R2_BLT_FMT_32_BIT_ARGB8888: 32 bits per pixel ARGB format with colors
 * stored as Alpha 31:24, Red 23:16, Green 15:8, and Blue 7:0.
 * @B2R2_BLT_FMT_YUV420_PACKED_PLANAR: YUV planar format, organized with
 * three separate planes for each color component, namely Y, U, and V.
 * U and V pixels are sub-sampled by a factor of two both horizontally and
 * vertically. The buffer shall contain a plane of Y, U, and V data in this
 * order
 * @B2R2_BLT_FMT_YUV422_PACKED_PLANAR: YUV planar format, organized with
 * three separate planes for each color component, namely Y, U, and V.
 * U and V pixels are subsampled by a factor of two horizontally.
 * The buffer shall contain a plane of Y, U, and V data in this order.
 * @B2R2_BLT_FMT_CB_Y_CR_Y: 16 bits per pixel YUV interleaved format organized
 * as UYVY (i.e., CbYCrY).
 * (Corresponds to YUV422R)
 * @B2R2_BLT_FMT_YUV420_PACKED_SEMI_PLANAR: YUV planar format, organized with
 * a first plane containing Y pixels, and a second plane containing U and V
 * pixels interleaved with the first U value first. U and V pixels are
 * sub-sampled by a factor of two both horizontally and vertically. The buffer
 * shall contain a plane of Y, U and V data.
 * (Same as B2R2 420 Raster 2 buffer - 420 R2B)
 * @B2R2_BLT_FMT_YUV422_PACKED_SEMI_PLANAR: YUV planar format, organized with
 * a first plane containing Y pixels, and a second plane containing U and V
 * pixels interleaved with the first U value first. U and V pixels are
 * sub-sampled by a factor of two horizontally. The buffer shall contain a
 * plane of Y, U and V data.
 * (Same as B2R2 422 Raster 2 buffer - 422 R2B)
 * @B2R2_BLT_FMT_32_BIT_ABGR8888: 32 bits per pixel ABGR format with colors
 * stored as Alpha 31:24,Blue 23:16, Green 15:8, and Red 7:0.
 * @B2R2_BLT_FMT_24_BIT_ARGB8565: 24 bits per pixel ARGB format with colors
 * stored as Alpha 23:16, Red 15:11, Green 10:5, and Blue 4:0.
 * @B2R2_BLT_FMT_24_BIT_YUV888: 24 bits per pixel YUV format with colors
 * stored as Y 23:16, U 15:8, and V 7:0.
 * @B2R2_BLT_FMT_32_BIT_AYUV8888: 32 bits per pixel AYUV format with colors
 * stored as Alpha 31:24, Y 23:16, U 15:8, and V 7:0.
 * @B2R2_BLT_FMT_YUV420_PACKED_SEMIPLANAR_MB_STE: Nomadik YUV 420 macro block
 * format, see B2R2 spec for details
 * @B2R2_BLT_FMT_YUV422_PACKED_SEMIPLANAR_MB_STE: Nomadik YUV 422 macro block
 * format, see B2R2 spec for details
 * @B2R2_BLT_FMT_1_BIT_A1: 1 bit per pixel A format, 1 bit alpha
 * @B2R2_BLT_FMT_8_BIT_A8: 8 bit per pixel A format, 8 bit alpha
 * @B2R2_BLT_FMT_YUV444_PACKED_PLANAR: YUV planar format, organized with
 * three separate planes, one for each color component, namely Y, U, and V.
 * All planes use full resolution, there is no subsampling.
 * The buffer shall contain a plane of Y, U, and V data in this order.
 * @B2R2_BLT_FMT_YVU420_PACKED_SEMI_PLANAR: YVU planar format, organized with
 * a first plane containing Y pixels, and a second plane containing V and U
 * pixels interleaved with the first V value first. V and U pixels are
 * sub-sampled by a factor of two both horizontally and vertically. The buffer
 * shall contain two planes, one plane with Y, and one with V and U data.
 * (Same as B2R2 420 Raster 2 buffer - 420 R2B except that chroma order is
 * swapped.)
 * @B2R2_BLT_FMT_YVU422_PACKED_SEMI_PLANAR: YVU planar format, organized with
 * a first plane containing Y pixels, and a second plane containing V and U
 * pixels interleaved with the first V value first. V and U pixels are
 * sub-sampled by a factor of two horizontally. The buffer shall contain a
 * two planes, one with Y, and one with V and U data.
 * (Same as B2R2 422 Raster 2 buffer - 422 R2B except that chroma order is
 * swapped.)
 * @B2R2_BLT_FMT_YVU420_PACKED_PLANAR: YVU planar format, organized with
 * three separate planes for each color component, namely Y, V, and U.
 * V and U pixels are sub-sampled by a factor of two both horizontally and
 * vertically. The buffer shall contain a plane of Y, V, and U data in this
 * order. (Same as B2R2_BLT_FMT_YUV420_PACKED_PLANAR except that chroma
 * order is swapped.)
 * @B2R2_BLT_FMT_YVU422_PACKED_PLANAR: YVU planar format, organized with
 * three separate planes for each color component, namely Y, V, and U.
 * V and U pixels are subsampled by a factor of two horizontally.
 * The buffer shall contain a plane of Y, V, and U data in this order.
 * (Same as B2R2_BLT_FMT_YUV422_PACKED_PLANAR except that chroma
 * order is swapped.)
 * @B2R2_BLT_FMT_24_BIT_VUY888: 24 bits per pixel VUY format with colors
 * stored as V 23:16, U 15:8, and Y 7:0.
 * @B2R2_BLT_FMT_32_BIT_VUYA8888: 32 bits per pixel VUYA format with colors
 * stored as V 31:24, U 23:16, Y 15:8, and Alpha 7:0.
 * @B2R2_BLT_FMT_16_BIT_ABGR4444: 16 bits per pixel ABGR format with colors
 * stored as Alpha 15:12, Blue 11:8, Green 7:4, and Red 3:0.
 */
enum b2r2_blt_fmt {
	B2R2_BLT_FMT_UNUSED = 0,
	B2R2_BLT_FMT_16_BIT_ARGB4444 = 4,
	B2R2_BLT_FMT_16_BIT_ARGB1555 = 5,
	B2R2_BLT_FMT_16_BIT_RGB565 = 6,
	B2R2_BLT_FMT_24_BIT_RGB888 = 11,
	B2R2_BLT_FMT_32_BIT_ARGB8888 = 16,
	B2R2_BLT_FMT_YUV420_PACKED_PLANAR = 20,
	B2R2_BLT_FMT_YUV422_PACKED_PLANAR = 23,
	B2R2_BLT_FMT_Y_CB_Y_CR = 25, /* Unsupported format */
	B2R2_BLT_FMT_CB_Y_CR_Y = 27,
	B2R2_BLT_FMT_YUV420_PACKED_SEMI_PLANAR = 39,
	B2R2_BLT_FMT_YUV422_PACKED_SEMI_PLANAR = 40,
	/* Extensions, non OpenMAX formats */
	B2R2_BLT_FMT_32_BIT_ABGR8888 = 0x7F000000, /* OpenMax vendor start */
	B2R2_BLT_FMT_24_BIT_ARGB8565 = 0x7F000001,
	B2R2_BLT_FMT_24_BIT_YUV888 = 0x7F000002,
	B2R2_BLT_FMT_32_BIT_AYUV8888 = 0x7F000003,
	B2R2_BLT_FMT_YUV420_PACKED_SEMIPLANAR_MB_STE = 0x7F000004,
	B2R2_BLT_FMT_YUV422_PACKED_SEMIPLANAR_MB_STE = 0x7F000005,
	B2R2_BLT_FMT_1_BIT_A1 = 0x7F000006,
	B2R2_BLT_FMT_8_BIT_A8 = 0x7F000007,
	B2R2_BLT_FMT_YUV444_PACKED_PLANAR = 0x7F000008,
	B2R2_BLT_FMT_YVU420_PACKED_SEMI_PLANAR = 0x7F000009,
	B2R2_BLT_FMT_YVU422_PACKED_SEMI_PLANAR = 0x7F00000A,
	B2R2_BLT_FMT_YVU420_PACKED_PLANAR = 0x7F00000B,
	B2R2_BLT_FMT_YVU422_PACKED_PLANAR = 0x7F00000C,
	B2R2_BLT_FMT_24_BIT_VUY888 = 0x7F00000D,
	B2R2_BLT_FMT_32_BIT_VUYA8888 = 0x7F00000E,
	B2R2_BLT_FMT_16_BIT_ABGR4444 = 0x7F00000F,
	/*
	 * YV12 is a YVU420P format with alignment requirements
	 * on luma and chroma stride
	 */
	B2R2_BLT_FMT_YV12 = 0x7F000010,
};

/**
 * enum b2r2_blt_ptr_type - Specifies a B2R2 buffer pointer type
 *
 * @B2R2_BLT_PTR_NONE:
 *    No pointer (NULL). E.g. src fill.
 * @B2R2_BLT_PTR_VIRTUAL:
 *    Use offset as a userspace virtual address
 * @B2R2_BLT_PTR_PHYSICAL:
 *    Use offset as a physical address
 * @B2R2_BLT_PTR_FD_OFFSET:
 *    Use fd + offset to determine buffer location.
 * @B2R2_BLT_PTR_HWMEM_BUF_NAME:
 *    Use hwmem_buf_name and offset to determine buffer location.
 */
enum b2r2_blt_ptr_type {
	B2R2_BLT_PTR_NONE,
	B2R2_BLT_PTR_VIRTUAL,
	B2R2_BLT_PTR_PHYSICAL,
	B2R2_BLT_PTR_FD_OFFSET,
	B2R2_BLT_PTR_HWMEM_BUF_NAME_OFFSET,
};

/**
 * struct b2r2_blt_buf - Specifies a B2R2 buffer pointer
 *
 * @type: Buffer pointer type
 * @hwmem_global_buf_id: Hwmem buffer name
 * @fd: File descriptor (e.g. file handle to pmem or fb device)
 * @offset: Offset where buffer can be found or address.
 * @len: Size of buffer in bytes
 * @bits: Pointer to the bitmap data. This field can be used to specify
 * an alternative way to access the buffer. Whenever the 'bits' pointer
 * is set to non-NULL, the underlying implementation is free to decide
 * whether or not to use it in favor of other ways to locate the buffer.
 */
struct b2r2_blt_buf {
	enum b2r2_blt_ptr_type type;
	__s32                  hwmem_buf_name;
	__s32                  fd;
	__u32                  offset;
	__u32                  len;
	void                   *bits;
};


/**
 * struct b2r2_blt_img - Specifies a B2R2 image
 *
 * @fmt: Pixel format of image
 * @buf: Pixel buffer
 * @width: Width in pixels
 * @height: Height in pixels
 * @pitch: Pitch in bytes (from start of one line to start of next)
 */
struct b2r2_blt_img {
	enum b2r2_blt_fmt   fmt;
	struct b2r2_blt_buf buf;
	__s32               width;
	__s32               height;
	__u32               pitch;
};


/**
 * enum b2r2_blt_transform- Specifies rotation and flipping, mutually exclusive
 * @B2R2_BLT_TRANSFORM_NONE:
 *    No rotation or flip
 * @B2R2_BLT_TRANSFORM_FLIP_H
 *    Flip horizontally
 * @B2R2_BLT_TRANSFORM_FLIP_V
 *    Flip vertically
 * @B2R2_BLT_TRANSFORM_CCW_ROT_90
 *    Rotate 90 degrees counter clockwise
 * @B2R2_BLT_TRANSFORM_CCW_ROT_180
 *    Rotate 180 degrees (same as flip horizontally together with
 *    flip vertically)
 * @B2R2_BLT_TRANSFORM_CCW_ROT_270
 *    Rotate 270 degrees counter clockwise
 * @B2R2_BLT_TRANSFORM_FLIP_H_CCW_ROT_90
 *    Flip horizontally and then rotate 90 degrees counter clockwise
 * @B2R2_BLT_TRANSFORM_FLIP_V_CCW_ROT_90
 *    Flip vertically and then rotate 90 degrees counter clockwise
 */
enum b2r2_blt_transform {
	B2R2_BLT_TRANSFORM_NONE              = 0,
	B2R2_BLT_TRANSFORM_FLIP_H            = 1,
	B2R2_BLT_TRANSFORM_FLIP_V            = 2,
	B2R2_BLT_TRANSFORM_CCW_ROT_90        = 4,
	B2R2_BLT_TRANSFORM_CCW_ROT_180       = 3,
	B2R2_BLT_TRANSFORM_CCW_ROT_270       = 7,
	B2R2_BLT_TRANSFORM_FLIP_H_CCW_ROT_90 = 5,
	B2R2_BLT_TRANSFORM_FLIP_V_CCW_ROT_90 = 6,
};


/**
 * enum b2r2_blt_flag - Flags that controls the B2R2 request
 *
 * Can be combined.
 *
 * @B2R2_BLT_FLAG_ASYNCH:
 *    Asynchronous request. b2r2_blt will returns when the request
 *    has been queued.
 * @B2R2_BLT_FLAG_DRY_RUN:
 *    Dry run, just to check if request can be performed.
 * @B2R2_BLT_FLAG_PER_PIXEL_ALPHA_BLEND:
 *    Enable per pixel alpha blend
 * @B2R2_BLT_FLAG_GLOBAL_ALPHA_BLEND:
 *    Enable global alpha blend (alpha value in global_alpha)
 * @B2R2_BLT_FLAG_SOURCE_COLOR_KEY:
 *    Enable source color key (color in src_color). Color should be in raw
 *    format.
 *    B2R2_BLT_FLAG_SOURCE_COLOR_KEY, B2R2_BLT_FLAG_SOURCE_FILL and
 *    B2R2_BLT_FLAG_SOURCE_FILL_RAW cannot be specified at the same time.
 *    B2R2_BLT_FLAG_SOURCE_COLOR_KEY and B2R2_BLT_FLAG_DEST_COLOR_KEY cannot be
 *    specified at the same time.
 * @B2R2_BLT_FLAG_SOURCE_FILL:
 *    Enable ARGB/AYUV source fill (color in src_color). Which of ARGB and AYUV
 *    is determined by the destination format.
 *    B2R2_BLT_FLAG_SOURCE_COLOR_KEY, B2R2_BLT_FLAG_SOURCE_FILL and
 *    B2R2_BLT_FLAG_SOURCE_FILL_RAW cannot be specified at the same time
 * @B2R2_BLT_FLAG_SOURCE_FILL_RAW:
 *    Enable raw color source fill (color in src_color)
 *    B2R2_BLT_FLAG_SOURCE_COLOR_KEY, B2R2_BLT_FLAG_SOURCE_FILL and
 *    B2R2_BLT_FLAG_SOURCE_FILL_RAW cannot be specified at the same time
 * @B2R2_BLT_FLAG_DEST_COLOR_KEY:
 *    Enable dest color key (color in dst_color). Color in raw format.
 * @B2R2_BLT_FLAG_SRC_IS_NOT_PREMULT:
 *    Source color not premultiplied (Valid for alpha formats only).
 * @B2R2_BLT_FLAG_DITHER:
 *    Enable dithering
 * @B2R2_BLT_FLAG_BLUR:
 *    Enable blur
 * @B2R2_BLT_FLAG_SOURCE_MASK:
 *    Enable source mask
 * @B2R2_BLT_FLAG_DESTINATION_CLIP:
 *    Enable destination clip rectangle
 * @B2R2_BLT_FLAG_INHERIT_PRIO
 *    Inherit process priority
 * @B2R2_BLT_FLAG_SRC_NO_CACHE_FLUSH
 *    Skip cache flush of source image buffer
 * @B2R2_BLT_FLAG_SRC_MASK_NO_CACHE_FLUSH
 *    Skip cache flush of source mask buffer
 * @B2R2_BLT_FLAG_DST_NO_CACHE_FLUSH
 *    Skip cache flush of destination image buffer
 * @B2R2_BLT_FLAG_BG_BLEND
 *    Indicate that a background buffer is supplied
 *    to the blit operation. B2R2_BLT_FLAG_PER_PIXEL_ALPHA_BLEND,
 *    B2R2_BLT_FLAG_SRC_IS_NOT_PREMULT, and
 *    B2R2_BLT_FLAG_GLOBAL_ALPHA_BLEND will control the blend operation.
 *    The destination blending is in this case disabled and the destination
 *    buffer will be overwritten with the source and background blend result.
 * @B2R2_BLT_FLAG_BG_NO_CACHE_FLUSH
 *    Skip cache flush of background image buffer
 * @B2R2_BLT_FLAG_REPORT_WHEN_DONE
 *    Report through b2r2_blt file when done. A b2r2_blt_report structure is
 *    read. Use poll() or select() if anything to read. (i.e. to help user space
 *    to implement callback functionality)
 * @B2R2_BLT_FLAG_REPORT_PERFORMANCE
 *    Include performance data in the report structure
 * @B2R2_BLT_FLAG_CLUT_COLOR_CORRECTION
 *    Use color look-up table for color correction.
 *    Pointer to the table must be specified in *clut field of
 *    the b2r2_blt_req structure.
 *    The table must map all input color values
 *    for each channel to the desired output values.
 *    It is an array with the following format:
 *    R0 G0 B0 A0 R1 G1 B1 A1...R255 G255 B255 A255
 *    where R0 is the 8 bit output value for red channel whenever its input
 *    equals 0.
 *    Similarly, R1 through R255 are the red channel outputs whenever
 *    the channel's inputs equal 1 through 255 respectively.
 *    Gn, Bn, An denote green, blue and alpha channel.
 *    Whenever the input bitmap format lacks the alpha channel,
 *    all alpha values in the color correction table should be set to 255.
 *    Size of the array that specifies the color correction table
 *    must be 1024 bytes.
 *    A table that does not change anything has the form:
 *    0 0 0 0 1 1 1 1 2 2 2 2 ... 254 254 254 254 255 255 255 255.
 *    CLUT color correction can be applied to YUV raster buffers as well,
 *    in which case the RGB color channels are mapped onto YUV-space
 *    as follows:
 *    R = red chrominance
 *    G = luminance
 *    B = blue chrominance
 *    A = alpha
 *    If any of the planar or semi-planar formats is used, luminance cannot
 *    be changed by the color correction table.
 */
enum b2r2_blt_flag {
	B2R2_BLT_FLAG_ASYNCH                = BIT(0),/*0x1*/
	B2R2_BLT_FLAG_DRY_RUN               = BIT(1),/*0x2*/
	B2R2_BLT_FLAG_PER_PIXEL_ALPHA_BLEND = BIT(2),/*0x4*/
	B2R2_BLT_FLAG_GLOBAL_ALPHA_BLEND    = BIT(3),/*0x8*/
	B2R2_BLT_FLAG_SOURCE_COLOR_KEY      = BIT(4),/*0x10*/
	B2R2_BLT_FLAG_SOURCE_FILL           = BIT(5),/*0x20*/
	B2R2_BLT_FLAG_SOURCE_FILL_RAW       = BIT(6),/*0x40*/
	B2R2_BLT_FLAG_DEST_COLOR_KEY        = BIT(7),/*0x80*/
	B2R2_BLT_FLAG_SRC_IS_NOT_PREMULT    = BIT(8),/*0x100*/
	B2R2_BLT_FLAG_DITHER                = BIT(9),/*0x200*/
	B2R2_BLT_FLAG_BLUR                  = BIT(10),/*0x400*/
	B2R2_BLT_FLAG_SOURCE_MASK           = BIT(11),/*0x800*/
	B2R2_BLT_FLAG_DESTINATION_CLIP      = BIT(12),/*0x1000*/
	B2R2_BLT_FLAG_INHERIT_PRIO          = BIT(13),/*0x2000*/
	B2R2_BLT_FLAG_SRC_NO_CACHE_FLUSH    = BIT(14),/*0x4000*/
	B2R2_BLT_FLAG_SRC_MASK_NO_CACHE_FLUSH = BIT(15),/*0x8000*/
	B2R2_BLT_FLAG_DST_NO_CACHE_FLUSH    = BIT(16),/*0x10000*/
	B2R2_BLT_FLAG_BG_BLEND              = BIT(17),/*0x20000*/
	B2R2_BLT_FLAG_BG_NO_CACHE_FLUSH     = BIT(18),/*0x40000*/
	B2R2_BLT_FLAG_FULL_RANGE_YUV        = BIT(19),/*0x20000*/
	B2R2_BLT_FLAG_REPORT_WHEN_DONE      = BIT(29),/*0x20000000*/
	B2R2_BLT_FLAG_REPORT_PERFORMANCE    = BIT(30),/*0x40000000*/
	B2R2_BLT_FLAG_CLUT_COLOR_CORRECTION = BIT(31),/*0x80000000*/
};


/**
 * struct b2r2_blt_req - Specifies a request to B2R2
 *
 * @size: Size of this structure. Used for versioning. MUST be specified.
 * @flags: Flags that control the B2R2 request ORed together
 * @tfm: How source should be flipped and rotated when blitting
 * @prio: Priority (-20 to 19). Inherits process prio
 *        if B2R2_BLT_FLAG_INHERIT_PRIO. Given priority is mapped onto B2R2.
 *        TBD: How?
 * @clut: Pointer to the look-up table for color correction.
 * @src_img: Source image. Not used if source fill.
 * @src_mask: Source mask. Not used if source fill.
 * @src_rect: Source area to be blitted.
 * @src_color: Source fill color or color key
 * @bg_img: Background image.
 * @bg_rect: Background area to blend with.
 * @dst_img: Destination image.
 * @dst_rect: Destination area to be blitted to.
 * @dst_color: Destination color key
 * @dst_clip_rect: Destination clip rectangle.
 * @global_alpha: Global alpha value (0 - 255)
 * @report1: Data 1 to report back when request is done.
 *           See struct b2r2_blt_report.
 * @report2: Data 2 to report back when request is done.
 *           See struct b2r2_blt_report.
 *
 */
struct b2r2_blt_req {
	__u32                     size;
	enum   b2r2_blt_flag      flags;
	enum   b2r2_blt_transform transform;
	__s32                     prio;
	void                      *clut;
	struct b2r2_blt_img       src_img;
	struct b2r2_blt_img       src_mask;
	struct b2r2_blt_rect      src_rect;
	__u32                     src_color;
	struct b2r2_blt_img       bg_img;
	struct b2r2_blt_rect      bg_rect;
	struct b2r2_blt_img       dst_img;
	struct b2r2_blt_rect      dst_rect;
	struct b2r2_blt_rect      dst_clip_rect;
	__u32                     dst_color;
	__u8                      global_alpha;
	__u32                     report1;
	__u32                     report2;
};

/**
 * enum b2r2_blt_cap -  Capabilities that can be queried for.
 *
 * Capabilities can be queried for a specific format or for formats in
 * general. To query for capabilities in general, specify BLT_FMT_UNUSED
 * as format.
 *
 * B2R2_BLT_CAP_UNUSED: Unused/unspecified capability
 * B2R2_BLT_CAP_FMT_SOURCE: Is format supported as source?
 * B2R2_BLT_CAP_FMT_SOURCE_MASK: Is format supported as source mask?
 * B2R2_BLT_CAP_FMT_DEST: Is format supported as dest?
 * B2R2_BLT_CAP_PER_PIXEL_ALPHA_BLEND: Is per pixel alpha blending supported
 * with format as source
 * B2R2_BLT_CAP_GLOBAL_ALPHA_BLEND: Is per global alpha blending supported
 * with format as source
 * B2R2_BLT_CAP_SOURCE_COLOR_KEY: Is source color key supported with format as
 * source
 * B2R2_BLT_CAP_SOURCE_FILL: Is source fill supported with format as source
 * B2R2_BLT_CAP_SOURCE_FILL_RAW: Is source fill raw supported with format as
 * dest
 * B2R2_BLT_CAP_DEST_COLOR_KEY: Is dest color key supported with format as dest
 * B2R2_BLT_CAP_DITHER: Is dithering supported with format as source
 * B2R2_BLT_CAP_BLUR: Is blur supported with format as source
 * B2R2_BLT_CAP_MINIFICATION_LIMIT: Minification limit (copybit support)
 * B2R2_BLT_CAP_MAGNIFICATION_LIMIT: Magnification limit (copybit support)
 * B2R2_BLT_CAP_SCALING_FRAC_BITS: Number of scaling fractional bits (copybit
 * support)
 * B2R2_BLT_CAP_ROTATION_STEP_DEG: Supported rotation step in degrees (copybit
 * support)
 */

enum b2r2_blt_cap {
    B2R2_BLT_CAP_UNUSED = 0,
    /**
     * @brief Is format supported as source.
     */
    B2R2_BLT_CAP_FMT_SOURCE,
    /**
     * @brief Is format supported as source mask
     */
    B2R2_BLT_CAP_FMT_SOURCE_MASK,
    /**
     * @brief Is format supported as destination
     */
    B2R2_BLT_CAP_FMT_DEST,
    /**
     * @brief Is per pixel alpha blending supported with format as source
     */
    B2R2_BLT_CAP_PER_PIXEL_ALPHA_BLEND,
    /**
     * @brief Is global alpha blending supported with format as source
     */
    B2R2_BLT_CAP_GLOBAL_ALPHA_BLEND,
    /**
     * @brief Is source color key supported with format as source
     */
    B2R2_BLT_CAP_SOURCE_COLOR_KEY,
    /**
     * @brief Is source fill supported with format as source
     */
    B2R2_BLT_CAP_SOURCE_FILL,
    /**
     * @brief Is source fill raw supported with format as dest
     */
    B2R2_BLT_CAP_SOURCE_FILL_RAW,
    /**
     * @brief Is dest color key supported with format as dest
     */
    B2R2_BLT_CAP_DEST_COLOR_KEY,
    /**
     * @brief Is dithering supported with format as source
     */
    B2R2_BLT_CAP_DITHER,
    /**
     * @brief Is blur supported with format as source
     */
    B2R2_BLT_CAP_BLUR,
    /**
     * @brief Minification limit (copybit support)
     */
    B2R2_BLT_CAP_MINIFICATION_LIMIT,
    /**
     * @brief Magnification limit (copybit support)
     */
    B2R2_BLT_CAP_MAGNIFICATION_LIMIT,
    /**
     * @brief Number of scaling fractional bits (copybit support)
     */
    B2R2_BLT_CAP_SCALING_FRAC_BITS,
    /**
     * @brief Supported rotation step in degrees (copybit support)
     */
    B2R2_BLT_CAP_ROTATION_STEP_DEG,
};

/**
 * struct b2r2_blt_query_cap - Query B2R2 capabilities
 *
 * fmt: Format to query capabilities for or BLT_FMT_UNUSED for all
 * cap: Capability to query for
 * result: Returned capability. Interpretaion of this variable varies
 *         with the capability queried
 */
struct b2r2_blt_query_cap {
	enum b2r2_blt_fmt fmt;
	enum b2r2_blt_cap cap;
	__u32             result;
};

/**
 * struct b2r2_blt_report - Report from B2R2 driver back to user space
 *
 * This structure can be read from B2R2 driver if B2R2_BLT_FLAG_REPORT_WHEN_DONE
 * flag was specified when the request was issued.
 *
 * @request_id: The id for the request, same as reported from blt_request
 * @report1: Client data specified in struct blt_request
 * @report2: Client data specified in struct blt_request
 * @usec_elapsed: Number of microseconds needed to perform this blit
 *                if B2R2_BLT_FLAG_REPORT_PERFORMANCE was specified when the
 *                request was issued.
 *
 */
struct b2r2_blt_report {
	__u32 request_id;
	__u32 report1;
	__u32 report2;
	__u32 usec_elapsed;
};

/**
 * B2R2 BLT driver is used in the following way:
 *
 * Obtain a file descriptor to the driver:
 *        fd = open("/dev/b2r2_blt", O_RDWR);
 *
 * Issue requests:
 *        struct b2r2_blt_request blt_request;
 *        blt_request.size = sizeof(blt_request);
 *        ... Fill request with data...
 *
 *        request_id = ioctl(fd, B2R2_BLT_IOC, (__u32) &blt_request);
 *
 * Wait for a request to finish
 *        ret = ioctl(fd, B2R2_BLT_SYNCH_IOC, (__u32) request_id);
 *
 * Wait for all requests from this context to finish
 *        ret = ioctl(fd, B2R2_BLT_SYNCH_IOC, (__u32) 0);
 *
 * Wait indefinitely for report data from driver:
 *        pollfd.fd = fd
 *        pollfd.events = 0xFFFFFFFF;
 *        pollfd.revents = 0;
 *        ret = poll(&pollfd, 1, -1);
 *
 * Read report data from driver
 *        struct b2r2_blt_report blt_report;
 *
 *        nread = read(fd, &blt_report, sizeof(blt_report));
 *
 * Close the driver
 *        close(fd);
 */

/* B2R2 BLT IOCTLS */

/**
 * B2R2_BLT_IOC_MAGIC is ioctl type group for B2R2 driver
 */
#define B2R2_BLT_IOC_MAGIC 0xb2

/**
 * The B2R2_BLT_IOC ioctl adds a blit request to B2R2.
 *
 * The ioctl returns when the blit has been performed if not
 * asynchronous execution has been specified. If asynchronous,
 * control is returned as soon as the request has been queued.
 *
 * Supplied parameter shall be a pointer to a struct b2r2_blt_req.
 *
 * Returns an unique request id if >= 0, else a negative error code.
 * This request id can be waited for using B2R2_BLT_SYNC_IOC.
 * Return values: -ESOMERROR Description of an error
 */
#define B2R2_BLT_IOC        _IOW(B2R2_BLT_IOC_MAGIC, 1, struct b2r2_blt_req)

/**
 * The B2R2_BLT_SYNC_IOC waits for all or a specified request to be finished.
 *
 * Supplied parameter shall be a request id previously returned by
 * B2R2_BLT_IOC or 0 for all requests.
 *
 * Returns 0 if OK, else a negative error code
 * Return value: -ESOMERROR Description of an error
 */
#define B2R2_BLT_SYNCH_IOC  _IOW(B2R2_BLT_IOC_MAGIC, 2, int)

/**
 * The BLT_QUERY_CAP_IOC returns capability information for all or
 *                       for a certain format
 *
 * Supplied parameter shall be a pointer to a struct b2r2_blt_query_cap.
 *
 * @return Returns 0 if OK, else a negative error code
 * @retval -ESOMERROR Description of an error
 */
#define B2R2_BLT_QUERY_CAP_IOC  _IOWR(B2R2_BLT_IOC_MAGIC, 3, \
				  struct b2r2_blt_query_cap)

/**
 * struct b2r2_platform_data - The b2r2 core hardware configuration
 *
 * @regulator_id: The name of the b2r2 power source
 * @clock_id: The name of the b2r2 clock
 *
 */
struct b2r2_platform_data {
	const char *regulator_id;
	const char *clock_id;
};

/**
 * b2r2_blt_open - Opening a handle for use with the blitter interface
 *
 * Returns a handle (0 or greater) to a B2R2 blitter intance on success
 */
int b2r2_blt_open(void);

/**
 * b2r2_blt_close - Free the blitter instance and its resources
 *
 * @handle: The B2R2 BLT instance handle
 *
 * All active jobs are finished or cancelled and allocated data
 * is released.
 *
 * Returns 0 on success
 */
int b2r2_blt_close(int handle);

/**
 * b2r2_blt_request - Request a blit operation
 *
 * @handle: The B2R2 BLT instance handle
 *
 * Returns 0 on success
 */
int b2r2_blt_request(int handle, struct b2r2_blt_req *user_req);

/**
 * b2r2_blt_synch - Wait for all or a specified job
 *
 * @handle: The B2R2 BLT instance handle
 * @request_id: If 0, wait for all requests on this instance to finish.
 *              Else wait for the request with the given request id to finish.
 *
 * Returns 0 on success
 */
int b2r2_blt_synch(int handle, int request_id);

#endif /* #ifdef _LINUX_VIDEO_B2R2_BLT_H */
