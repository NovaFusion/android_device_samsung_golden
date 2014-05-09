/*
 * Copyright (C) ST-Ericsson SA 2011
 *
 * ST-Ericsson Display overlay compositer device driver
 *
 * Author: Anders Bauer <anders.bauer@stericsson.com>
 * for ST-Ericsson.
 *
 * Modified: Per-Daniel Olsson <per-daniel.olsson@stericsson.com>
 * for ST-Ericsson.
 *
 * License terms: GNU General Public License (GPL), version 2.
 */

#ifndef _COMPDEV_H_
#define _COMPDEV_H_

#if !defined(__KERNEL__) && !defined(_KERNEL)
#include <stdint.h>
#else
#include <linux/types.h>
#include <video/mcde.h>
#endif

#if defined(__KERNEL__) || defined(_KERNEL)
#include <linux/mm_types.h>
#include <linux/bitops.h>
#else
#define BIT(nr)			(1UL << (nr))
#endif

#define COMPDEV_DEFAULT_DEVICE_PREFIX "comp"


enum compdev_fmt {
	COMPDEV_FMT_RGB565,
	COMPDEV_FMT_RGB888,
	COMPDEV_FMT_RGBX8888,
	COMPDEV_FMT_RGBA8888,
	COMPDEV_FMT_YUV422,
	COMPDEV_FMT_YCBCR42XMBN,
	COMPDEV_FMT_YUV420_SP,
	COMPDEV_FMT_YVU420_SP,
	COMPDEV_FMT_YUV420_P,
	COMPDEV_FMT_YVU420_P,
	COMPDEV_FMT_YV12,
};

struct compdev_size {
	__u16 width;
	__u16 height;
};

/* Display rotation */
enum compdev_transform {
	COMPDEV_TRANSFORM_ROT_0       = 0x00,
	/* flip source image horizontally (around the vertical axis) */
	COMPDEV_TRANSFORM_FLIP_H      = 0x01,
	/* flip source image vertically (around the horizontal axis)*/
	COMPDEV_TRANSFORM_FLIP_V      = 0x02,
	/* rotate source image 180 degrees */
	COMPDEV_TRANSFORM_ROT_180     = 0x03,
	/* rotate source image 90 degrees clockwise */
	COMPDEV_TRANSFORM_ROT_90_CW   = 0x04,
	    /* rotate source image 270 degrees clockwise */
	COMPDEV_TRANSFORM_ROT_270_CW  = 0x07,
	/* rotate source image 90 degrees counterclockwise */
	COMPDEV_TRANSFORM_ROT_90_CCW  = COMPDEV_TRANSFORM_ROT_270_CW,
	/* rotate source image 270 degrees counterclockwise */
	COMPDEV_TRANSFORM_ROT_270_CCW = COMPDEV_TRANSFORM_ROT_90_CW,
	 /* Rotate source image 90 degrees clockwise With Flip H*/
	COMPDEV_TRANSFORM_ROT_90_CW_FLIP_H   = 0x05,
	 /* Rotate source image 90 degrees clockwise with Flip V */
	COMPDEV_TRANSFORM_ROT_90_CW_FLIP_V   = 0x06,
};

enum compdev_flag {
	COMPDEV_NO_FLAG            = 0x00000000,
	COMPDEV_OVERLAY_FLAG       = 0x00000001,
	COMPDEV_FRAMEBUFFER_FLAG   = 0x00000002,
	COMPDEV_BYPASS_FLAG        = 0x00000004,
	COMPDEV_EXTERNAL_DISP_FLAG = 0x00000008,
	COMPDEV_PROTECTED_FLAG     = 0x00000010,
	COMPDEV_INTERNAL_TEMP_FLAG = 0x01000000,
};

enum compdev_ptr_type {
	COMPDEV_PTR_PHYSICAL,
	COMPDEV_PTR_HWMEM_BUF_NAME_OFFSET,
};

enum compdev_listener_state {
	COMPDEV_LISTENER_OFF,
	COMPDEV_LISTENER_ON,
};


struct compdev_rect {
	__s16  x;
	__s16  y;
	__u16  width;
	__u16  height;
};

struct compdev_buf {
	enum compdev_ptr_type  type;
	__s32                  hwmem_buf_name;
	__s32                  fd;
	__u32                  offset;
	__u32                  len;
};

struct compdev_img {
	enum compdev_fmt     fmt;
	struct compdev_buf   buf;
	__u16                width;
	__u16                height;
	__u16                pitch;
	__u8                 z_position;
	struct compdev_rect  dst_rect;
	struct compdev_rect  src_rect;
	enum   compdev_transform  transform;
	__u32                flags;
};

struct compdev_scene_info {
	enum   compdev_transform  app_transform;
	enum   compdev_transform  fb_transform;
	__u8                      img_count;
	__u8                      reuse_fb_img;
	enum   compdev_transform  hw_transform;
};

struct compdev_video_mode {
	__u32 xres;
	__u32 yres;
	__u32 pixclock; /* pixel clock in ps (pico seconds) */
	__u32 hbp;      /* horizontal back porch: left margin (excl. hsync) */
	__u32 hfp;      /* horizontal front porch: right margin (excl. hsync) */
	__u32 hsw;      /* horizontal sync width */
	__u32 vbp;      /* vertical back porch: upper margin (excl. vsync) */
	__u32 vfp;      /* vertical front porch: lower margin (excl. vsync) */
	__u32 vsw;      /* vertical sync width*/
	__u32 interlaced;
	__u32 force_update; /* when switching between hdmi and sdtv */
};

#define COMPDEV_GET_SIZE_IOC           _IOR('D', 1, struct compdev_size)
#define COMPDEV_POST_BUFFER_IOC        _IOW('D', 2, struct compdev_img)
#define COMPDEV_POST_SCENE_INFO_IOC    _IOW('D', 3, struct compdev_scene_info)
#define COMPDEV_GET_LISTENER_STATE_IOC _IOR('D', 4, enum compdev_listener_state)
#define COMPDEV_SET_VIDEO_MODE_IOC     _IOW('D', 5, struct compdev_video_mode)
#define COMPDEV_WAIT_FOR_VSYNC_IOC     _IOR('D', 6, __s64)


#if defined(__KERNEL__) || defined(_KERNEL)

#define MAX_NBR_OF_COMPDEVS 2
#define CONFIG_COMPDEV_JANITOR

struct compdev;
typedef void (*post_buffer_callback)(void *data, struct compdev_img *img);
typedef void (*post_scene_info_callback)(void *data,
		struct compdev_scene_info *s_info);
typedef void (*size_changed_callback)(void *data, struct compdev_size *size);


int compdev_create(struct mcde_display_device *ddev,
		struct mcde_overlay *parent_ovly,
		bool mcde_rotation,
		struct compdev **dev);
void compdev_destroy(struct mcde_display_device *ddev);

int compdev_get(int dev_idx, struct compdev **dev);
int compdev_put(struct compdev *dev);
int compdev_get_size(struct compdev *dev, struct compdev_size *size);
int compdev_post_buffer(struct compdev *dev, struct compdev_img *img);
int compdev_post_single_buffer_asynch(struct compdev *dev,
		struct compdev_img *img, int b2r2_handle,
		int b2r2_req_id);

int compdev_post_scene_info(struct compdev *dev,
		struct compdev_scene_info *s_info);
int compdev_set_video_mode(struct compdev *dev,
		struct compdev_video_mode *video_mode);
int compdev_get_listener_state(struct compdev *dev,
		enum compdev_listener_state *listener_state);
int compdev_wait_for_vsync(struct compdev *cd, s64 *timestamp);
const char *compdev_get_device_name(struct compdev *dev);
int compdev_clear_screen(struct compdev *dev);

int compdev_register_listener_callbacks(struct compdev *dev, void *data,
		post_buffer_callback pb_cb, post_scene_info_callback si_cb,
		size_changed_callback sc_cb);
int compdev_deregister_callbacks(struct compdev *dev);

#endif /* __KERNEL__ */

#endif /* _COMPDEV_H_ */

