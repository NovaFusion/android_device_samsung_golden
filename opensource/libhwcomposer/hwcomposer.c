/*
 * Copyright (C) ST-Ericsson SA 2011
 * Author: Per-Daniel Olsson <per-daniel.olsson@stericsson.com> for ST-Ericsson.
 */

#define LOG_TAG "STE-HWComposer"

#include <hardware/hardware.h>
#include "hwcomposer.h"
#include <cutils/log.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <hardware/gralloc.h>
#include <gralloc_stericsson_ext.h>
#include <linux/hwmem.h>
#include <linux/compdev.h>
#include <limits.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "vsync_monitor.h"

/* Use the STE version if available, otherwise fall back to
 * the corresponding vanilla version.
 * Note: the STE version is normally only unavailable
 * during the bring-up period of an Android upgrade.
 */
#ifdef HWC_DEVICE_API_VERSION_0_3_STE
#define STE_HWC_DEVICE_API_CURRENT HWC_DEVICE_API_VERSION_0_3_STE
#else
#define STE_HWC_DEVICE_API_CURRENT HWC_DEVICE_API_VERSION_0_3
#endif

#define DEBUG_STE_HWCOMPOSER 0
#define DEBUG_STE_HWCOMPOSER_LAYER_DUMP 0
#define DEBUG_STE_HWCOMPOSER_ALLOC 0

#define HWMEM_PATH ("/dev/" HWMEM_DEFAULT_DEVICE_NAME)
#define COMPDEV_PATH "/dev/comp0"

#ifdef ENABLE_HDMI
#define HDMID_SOCKET_LISTEN_PATH "/dev/socket/hdmid"
#endif

/*
 * Only 2 buffers are needed, one for blitting and one for display HW.
 * If the display posting is asynchronous, 3 buffers are needed.
 */
#define OUT_IMG_COUNT 2

/* Max number of layers that can be cached. */
#define CACHED_LAYERS_SIZE 16


#ifdef ENABLE_HDMI
typedef struct hwc_hdmi_setting {
    bool                 hdmi_plugged;
    bool                 resolutionchanged;
    int                  hdmid_sockfd;
    struct compdev_rect  res;
} hwc_hdmi_settings_t;
#endif

static int hwcomposer_device_open(const struct hw_module_t *module,
        const char *name, struct hw_device_t **device);

static int hwcomposer_setparameter(struct hwc_composer_device *dev,
                int param, int value);

static int hwcomposer_eventControl(struct hwc_composer_device* dev,
                int event, int enabled);

static struct hw_module_methods_t hwcomposer_module_methods = {
    .open = hwcomposer_device_open,
};

static struct hwc_methods hwcomposer_methods = {
    .eventControl = hwcomposer_eventControl,
#ifdef HWC_DEVICE_API_VERSION_0_3_STE
    .setParameter = hwcomposer_setparameter,
#endif /* HWC_DEVICE_API_VERSION_0_3_STE */
};

struct hwc_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = HWC_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = HWC_HARDWARE_MODULE_ID,
        .name = "ST-Ericsson HWComposer module",
        .author = "ST-Ericsson SA",
        .methods = &hwcomposer_module_methods,
   },
};


struct worker_context {
    pthread_mutex_t mutex;
    pthread_t worker_thread;
    pthread_cond_t signal_update;
    pthread_cond_t signal_done;
    pthread_attr_t thread_attr;
    bool alive;
    bool working;
    int hwmem;
    int compdev;
    const struct gralloc_module_t *gralloc;
    hwc_layer_list_t* work_list;
    struct hwc_rect* frame_rect;
    uint32_t compdev_layer_count;
    uint32_t compdev_bypass_count;
    int disp_width;
    int disp_height;
    uint32_t ui_orientation;
    uint32_t hardware_rotation;
};

struct hwcomposer_context {
    struct hwc_composer_device dev;
    pthread_mutex_t hwc_mutex;
    int hwmem;
    int compdev;
    const struct gralloc_module_t *gralloc;
    struct worker_context worker_ctx;
    int disp_width;
    int disp_height;
    bool bypass_ovly;
    uint32_t ui_orientation;
    uint32_t hardware_rotation;
    bool videoplayback;
#if DEBUG_STE_HWCOMPOSER
    int frames;
    int overlay_layers;
    int framebuffer_layers;
#endif
#ifdef ENABLE_HDMI
    hwc_hdmi_settings_t hdmi_settings;
#endif
    bool pending_rotation;
    buffer_handle_t *cached_layers;
    size_t cached_layers_count;
    bool grabbed_all_layers;
    int32_t *actual_composition_types;
    uint32_t actual_composition_types_size;
    struct hwc_procs *procs;
};

static enum compdev_fmt to_compdev_format(int hal_format);
static int to_comp_transform(int hal_transform);

static int worker_init(struct hwcomposer_context *ctx,
                        int hwmem, int compdev,
                        const struct gralloc_module_t *gralloc);
static int worker_destroy(struct hwcomposer_context *ctx);
static int worker_signal_update(struct hwcomposer_context *ctx,
                                hwc_layer_list_t* work_list,
                                uint32_t compdev_layer_count,
                                uint32_t compdev_bypass_count,
                                struct hwc_rect* frame_rect);
static int worker_wait_for_done(struct hwcomposer_context *ctx);

static inline int min(int a, int b) {
    return (a < b) ? a : b;
}

static inline int max(int a, int b) {
    return (a > b) ? a : b;
}

static int get_stride(uint32_t width, int hal_format)
{
    int stride=0;
    switch (hal_format) {
        case HAL_PIXEL_FORMAT_RGB_565:
            stride = width * 2;
            break;
        case HAL_PIXEL_FORMAT_RGB_888:
            stride = width * 3;
            break;
        case HAL_PIXEL_FORMAT_RGBA_8888:
        case HAL_PIXEL_FORMAT_RGBX_8888:
        case HAL_PIXEL_FORMAT_BGRA_8888:
            stride = width * 4;
            break;
        case HAL_PIXEL_FORMAT_YV12:
        case HAL_PIXEL_FORMAT_YCbCr_422_SP:
        case HAL_PIXEL_FORMAT_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_YCbCr_422_P:
        case HAL_PIXEL_FORMAT_YCbCr_420_P:
        case HAL_PIXEL_FORMAT_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_YCrCb_422_SP:
        case HAL_PIXEL_FORMAT_YCrCb_422_P:
        case HAL_PIXEL_FORMAT_YCrCb_420_P:
            stride = width;
            break;
        case HAL_PIXEL_FORMAT_YCBCR42XMBN:
            stride = width;
            if (width % 16)
                stride += 16 - (width % 16);
            break;
        case HAL_PIXEL_FORMAT_YCbCr_422_I:
        case HAL_PIXEL_FORMAT_CbYCrY_422_I:
            stride = width * 2;
            break;
        default:
            ALOGE("%s: Stride calculated using incorrect format!", __func__);
            stride = width;
            break;
    }

    /* The display controller requires 8 byte aligned strides */
    if (stride % 8)
        stride += 8 - (stride % 8);

    return stride;
}

static enum compdev_fmt to_compdev_format(int hal_format)
{
    switch (hal_format) {
        case HAL_PIXEL_FORMAT_RGBA_8888:
            return COMPDEV_FMT_RGBA8888;
        case HAL_PIXEL_FORMAT_RGBX_8888:
            return COMPDEV_FMT_RGBX8888;
        case HAL_PIXEL_FORMAT_RGB_888:
            return COMPDEV_FMT_RGB888;
        case HAL_PIXEL_FORMAT_RGB_565:
            return COMPDEV_FMT_RGB565;
        case HAL_PIXEL_FORMAT_YCbCr_422_I:
            return COMPDEV_FMT_YUV422;
        case HAL_PIXEL_FORMAT_YCBCR42XMBN:
            return COMPDEV_FMT_YCBCR42XMBN;
        case HAL_PIXEL_FORMAT_YCrCb_420_SP:
            return COMPDEV_FMT_YVU420_SP;
        case HAL_PIXEL_FORMAT_YCbCr_420_SP:
            return COMPDEV_FMT_YUV420_SP;
        case HAL_PIXEL_FORMAT_YV12:
            return COMPDEV_FMT_YV12;
        case HAL_PIXEL_FORMAT_YCrCb_420_P:
            return COMPDEV_FMT_YVU420_P;
        case HAL_PIXEL_FORMAT_YCbCr_420_P:
            return COMPDEV_FMT_YUV420_P;
        default:
            ALOGE("%s: Compdev format not found %d", __func__, hal_format);
            return (enum compdev_fmt)0;
    }
}

static enum compdev_transform to_compdev_transform(int hal_transform)
{
    switch(hal_transform) {
    case HAL_TRANSFORM_ROT_90:
        return COMPDEV_TRANSFORM_ROT_90_CW;
    case HAL_TRANSFORM_ROT_180:
        return COMPDEV_TRANSFORM_ROT_180;
    case HAL_TRANSFORM_ROT_270:
        return COMPDEV_TRANSFORM_ROT_270_CW;
    case HAL_TRANSFORM_FLIP_H:
        return COMPDEV_TRANSFORM_FLIP_H;
    case HAL_TRANSFORM_FLIP_V:
        return COMPDEV_TRANSFORM_FLIP_V;
    case (HAL_TRANSFORM_ROT_90 + HAL_TRANSFORM_FLIP_H):
        return COMPDEV_TRANSFORM_ROT_90_CW_FLIP_H;
    case (HAL_TRANSFORM_ROT_90 + HAL_TRANSFORM_FLIP_V):
        return COMPDEV_TRANSFORM_ROT_90_CW_FLIP_V;
    default:
        return COMPDEV_TRANSFORM_ROT_0;
    }
}

static uint32_t to_compdev_rotation(uint32_t rotation)
{
    switch(rotation) {
    case 90:
        return COMPDEV_TRANSFORM_ROT_90_CW;
    case 180:
        return COMPDEV_TRANSFORM_ROT_180;
    case 270:
        return COMPDEV_TRANSFORM_ROT_270_CW;
    default:
        return COMPDEV_TRANSFORM_ROT_0;
    }
}

static uint32_t to_degrees(enum compdev_transform transform)
{
    switch(transform) {
    case COMPDEV_TRANSFORM_ROT_90_CW:
    case COMPDEV_TRANSFORM_ROT_90_CW_FLIP_H:
    case COMPDEV_TRANSFORM_ROT_90_CW_FLIP_V:
        return 90;
    case COMPDEV_TRANSFORM_ROT_180:
        return 180;
    case COMPDEV_TRANSFORM_ROT_270_CW:
        return 270;
    default:
        return 0;
    }
}

static enum compdev_transform remove_rot(enum compdev_transform transform)
{
    switch(transform) {
    case COMPDEV_TRANSFORM_FLIP_H:
    case COMPDEV_TRANSFORM_ROT_90_CW_FLIP_H:
        return COMPDEV_TRANSFORM_FLIP_H;
    case COMPDEV_TRANSFORM_FLIP_V:
    case COMPDEV_TRANSFORM_ROT_90_CW_FLIP_V:
        return COMPDEV_TRANSFORM_FLIP_V;
    default:
        return COMPDEV_TRANSFORM_ROT_0;
    }
}


static int convert_image(const struct gralloc_module_t *gralloc,
        buffer_handle_t buf_handle,
        struct compdev_img *img)
{
    int alloc_hnd;
    int offset;
    int type;
    int size;
    int width;
    int height;
    int format;

    if (!buf_handle) {
        ALOGE("%s: Buffer handle is NULL!", __func__);
        return -EINVAL;
    }

    type = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_TYPE, buf_handle);
    if (type < 0) {
        ALOGE("%s: Could not get buf type from gralloc!", __func__);
        return -EINVAL;
    }

    if (type != GRALLOC_BUF_TYPE_HWMEM) {
        ALOGE("%s: Buffer is not HWMEM!", __func__);
        return -EINVAL;
    }

    memset(img, 0, sizeof(*img));

    width = gralloc->perform(gralloc,
            GRALLOC_MODULE_PERFORM_GET_BUF_WIDTH, buf_handle);

    height = gralloc->perform(gralloc,
            GRALLOC_MODULE_PERFORM_GET_BUF_HEIGHT, buf_handle);

    format = gralloc->perform(gralloc,
            GRALLOC_MODULE_PERFORM_GET_BUF_FORMAT, buf_handle);


    img->fmt = to_compdev_format(format);
    img->pitch = get_stride(width, format);

    /* TODO: Fix gralloc so that we can get buffer info for both 8500 and 9540 */

    alloc_hnd = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_ALLOCATOR_HANDLE, buf_handle);
    if (alloc_hnd < 0) {
        ALOGE("%s: Could not get allocator handle from gralloc!", __func__);
        return -EINVAL;
    }

    size = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_SIZE, buf_handle);
    if (size < 0) {
        ALOGE("%s: Could not get buf size from gralloc!", __func__);
        return -EINVAL;
    }

    offset = gralloc->perform(gralloc,
                    GRALLOC_MODULE_PERFORM_GET_BUF_OFFSET, buf_handle);
    if (offset < 0) {
        ALOGE("%s: Could not get buf offset from gralloc!", __func__);
        return -EINVAL;
    }

    if (type == GRALLOC_BUF_TYPE_HWMEM) {
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: GRALLOC_BUF_TYPE_HWMEM", __func__);
        img->buf.type = COMPDEV_PTR_HWMEM_BUF_NAME_OFFSET;
        img->buf.hwmem_buf_name = alloc_hnd;
        img->buf.offset = offset;
    }

    img->buf.len = size;
    img->width = width;
    img->height = height;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    return 0;
}

static bool rectIsEqual(struct hwc_rect* firstRect, struct hwc_rect* secondRect)
{
    if (firstRect->left == secondRect->left &&
        firstRect->right == secondRect->right &&
        firstRect->top == secondRect->top &&
        firstRect->bottom == secondRect->bottom)
        return true;
    else
        return false;
}


static void hwc_post_compdev_scene(int compdev,
        enum compdev_transform fb_transform,
        enum compdev_transform app_transform, uint8_t layer_count,
        uint8_t reuse_fb, uint32_t hw_transform)
{
    int ret;
    struct compdev_scene_info s_info;
    memset(&s_info, 0, sizeof(s_info));
    s_info.img_count = layer_count;
    s_info.fb_transform = fb_transform;
    s_info.app_transform = app_transform;
    s_info.reuse_fb_img = reuse_fb;
    s_info.hw_transform = hw_transform;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s layercount is: %d, reuse_fb is: %d,"
            " hw_trans = 0x%02X, fb_trans = 0x%02X, app_trans = 0x%02X",
            __func__, layer_count, reuse_fb, hw_transform,
            fb_transform, app_transform);

    ret = ioctl(compdev, COMPDEV_POST_SCENE_INFO_IOC, &s_info);
    if (ret < 0) {
        ALOGE("%s: Failed to start sync (%d) compdev, %s",
                __func__, layer_count, strerror(errno));
    }
}

static bool compdevHasListener(int compdev)
{
    enum compdev_listener_state state;
    int ret;
    ret = ioctl(compdev, COMPDEV_GET_LISTENER_STATE_IOC, &state);
    if (ret < 0) {
        ALOGE("%s: Failed to get listener state for compdev, %s",
                __func__, strerror(errno));
    }
    if (state == COMPDEV_LISTENER_ON) {
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "COMPDEV_LISTENER_ON");
        return true;
    } else {
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "COMPDEV_LISTENER_OFF");
        return false;
    }
}

static void hwc_rotate(const hwc_rect_t* rect, int hal_rot, int fb_rot, int disp_rot,
            int disp_width, int disp_height, hwc_rect_t* rOut)
{
    int frame_width;
    int frame_height;

    if (disp_rot % 180) {
        frame_width = disp_height;
        frame_height = disp_width;
    } else {
        frame_width = disp_width;
        frame_height = disp_height;
    }

    if (hal_rot & HAL_TRANSFORM_ROT_90) {
        rOut->top = rect->left;
        rOut->left = rect->top;
        rOut->right = rect->bottom;
        rOut->bottom = rect->right;
    } else {
        rOut->top = rect->top;
        rOut->left = rect->left;
        rOut->right = rect->right;
        rOut->bottom = rect->bottom;
    }
}

static void hwc_get_dst_rect(struct hwc_rect *frame_rect,
        enum compdev_transform transform,
        int disp_width, int disp_height,
        struct compdev_rect *dst_rect)
{
    /*
     * TODO: Confusing when comparing with hwc_rotate.
     *       Make sure we can handle all layers.
     */
    if (transform == COMPDEV_TRANSFORM_ROT_0 ||
            transform == COMPDEV_TRANSFORM_ROT_180) {
        dst_rect->x = frame_rect->left;
        dst_rect->y = frame_rect->top;
        dst_rect->width = frame_rect->right - frame_rect->left;
        dst_rect->height = frame_rect->bottom - frame_rect->top;
    } else {
        dst_rect->x = frame_rect->top;
        dst_rect->y = frame_rect->left;
        dst_rect->width = frame_rect->bottom - frame_rect->top;
        dst_rect->height = frame_rect->right - frame_rect->left;
    }
}

static bool bufferIsYUV(const struct gralloc_module_t *gralloc,
        buffer_handle_t buf_handle)
{
    bool ret = false;
    int format;

    /* TODO: Use Gralloc perform instead */

    if (!buf_handle) {
        ALOGE("%s: Buffer handle is NULL!", __func__);
    }

    format = gralloc->perform(gralloc,
            GRALLOC_MODULE_PERFORM_GET_BUF_FORMAT, buf_handle);

    switch (format) {
    case HAL_PIXEL_FORMAT_YV12:
    case HAL_PIXEL_FORMAT_YCbCr_422_SP:
    case HAL_PIXEL_FORMAT_YCbCr_420_SP:
    case HAL_PIXEL_FORMAT_YCbCr_422_P:
    case HAL_PIXEL_FORMAT_YCbCr_420_P:
    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
    case HAL_PIXEL_FORMAT_YCrCb_422_SP:
    case HAL_PIXEL_FORMAT_YCrCb_422_P:
    case HAL_PIXEL_FORMAT_YCrCb_420_P:
    case HAL_PIXEL_FORMAT_YCBCR42XMBN:
    case HAL_PIXEL_FORMAT_YCbCr_422_I:
    case HAL_PIXEL_FORMAT_CbYCrY_422_I:
        ret = true;
        break;
    default:
        ret = false;
    }
    return ret;
}

static bool bufferIsHWMEM(const struct gralloc_module_t *gralloc,
        buffer_handle_t buf_handle)
{
    bool ret = false;
    int type;

    /* TODO: Use Gralloc perform instead */

    if (!buf_handle) {
        ALOGE("%s: Buffer handle is NULL!", __func__);
    }

    type = gralloc->perform(gralloc,
            GRALLOC_MODULE_PERFORM_GET_BUF_TYPE, buf_handle);

    if (type == GRALLOC_BUF_TYPE_HWMEM)
        ret = true;

    return ret;
}

#ifdef ENABLE_HDMI
static void get_hdmi_resolution(__u16 width, __u16 height,
        struct compdev_rect *hdmi_res)
{
    struct compdev_rect disp_rect;

    memset(hdmi_res, 0, sizeof (*hdmi_res));

    /* TODO: take into account the framebuffer rotation
     *
     * if (fb_rot % 180) {
     *     disp_rect.width = height;
     *     disp_rect.height = width;
     * } else {
     *    disp_rect.width = width;
     *    disp_rect.height = height;
     * }
     */
    disp_rect.width = width;
    disp_rect.height = height;

    /*
     * Based on the AV8100 supported resolutions
     */
    if (disp_rect.width > disp_rect.height) {
        if (disp_rect.width <= 1280) {
            hdmi_res->width = 1280;
            if (disp_rect.height <= 720)
                hdmi_res->height = 720;
            else if (disp_rect.height <= 768)
                hdmi_res->height = 768;
            else
                hdmi_res->height = 800;
        } else if (disp_rect.width <= 1360) {
            hdmi_res->width = 1360;
            hdmi_res->height = 768;
        } else if (disp_rect.width <= 1366) {
            hdmi_res->width = 1366;
            hdmi_res->height = 768;
        } else if (disp_rect.width <= 1440) {
            hdmi_res->width = 1440;
            if (disp_rect.height <= 480)
                hdmi_res->height = 480;
            else
                hdmi_res->height = 576;
        } else if (disp_rect.width <= 1920) {
            hdmi_res->width = 1920;
            hdmi_res->height = 1080;
        } else {
            /* For now stick to this as the maximum resolution */
            hdmi_res->width = 1920;
            hdmi_res->height = 1080;
        }
    } else {
        if (disp_rect.height <= 600) {
            hdmi_res->width = 800;
            hdmi_res->height = 600;
        } else if (disp_rect.height <= 720) {
            hdmi_res->height = 720;
            hdmi_res->width = 1280;
        } else if (disp_rect.height <= 768) {
            hdmi_res->height = 768;
            if (disp_rect.width <= 1024)
                hdmi_res->width = 1024;
            else if (disp_rect.width <= 1280)
                hdmi_res->width = 1280;
            else if (disp_rect.width <= 1360)
                hdmi_res->width = 1360;
            else
                hdmi_res->width = 1366;
        } else if (disp_rect.height <= 800) {
            hdmi_res->height = 800;
            hdmi_res->width = 1280;
        } else {
            hdmi_res->height = 1080;
            hdmi_res->width = 1920;
        }
    }

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "HDMI resolution detected: %d x %d",
            hdmi_res->width, hdmi_res->height);
}

int send_hdmi_command(int sockfd,
        const char * const command,
        const char * const data,
        const size_t datalength)
{
    char buf[128];
    int pos = 0;
    size_t totlen = strlen(command) + 1;
    ssize_t written;

    if (sockfd < 0) {
        ALOGE("Invalid socket");
        return -1;
    }

    if (data != NULL)
        totlen += datalength + 1;

    if (totlen > sizeof(buf)) {
        ALOGE("Requested HDMI command length too big");
        return -1;
    }

    pos = snprintf(buf, sizeof(buf), "%s", command);
    if (data != NULL) {
        buf[pos++] = ' ';
        memcpy(&buf[pos], data, datalength);
        buf[pos + datalength] = '\0';
    }

    written = write(sockfd, buf, totlen);
    if (written < 0) {
        ALOGE("Failed to send command");
        return -1;
    }

    if ((size_t)written < totlen) {
        ALOGE("Failed to send complete command");
        return -1;
    }

    return 0;
}

int update_hdmi_prefered_resolution(struct hwcomposer_context *ctx,
        struct compdev_rect *hdmi_res)
{
    char buf[sizeof(__u16) * 2];

    buf[0] = hdmi_res->width & 0xFF;
    buf[1] = (hdmi_res->width >> 8) & 0xFF;
    buf[2] = hdmi_res->height & 0xFF;
    buf[3] = (hdmi_res->height >> 8) & 0xFF;

    return send_hdmi_command(ctx->hdmi_settings.hdmid_sockfd,
            "hdmi_preferred_resolution", buf, sizeof(buf));
}

int set_hdmi_default_resolution(struct hwcomposer_context *ctx)
{
    return send_hdmi_command(ctx->hdmi_settings.hdmid_sockfd,
            "hdmi_default_resolution", NULL, 0);
}

int open_hdmid_socket(void)
{
    int ret = -1;
    struct sockaddr_un addr;
    int sockfd;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "Opening socket to hdmid");

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        ALOGE("Failed to create socket");
        goto exit;
    }

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, HDMID_SOCKET_LISTEN_PATH);
    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
        ALOGE("%s/%s failed to connect %s, %s",
                getpwuid(geteuid())->pw_name,
                getgrgid(getegid())->gr_name,
                HDMID_SOCKET_LISTEN_PATH,
                strerror(errno));
        goto exit;
    }

    ret = sockfd;

exit:
    return ret;
}
#endif


/*initializes and clears the layers_cache, also used for clearing the cache */
int init_layer_cache_locked(struct hwcomposer_context *ctx)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);

    if (ctx->cached_layers == NULL) {
        ctx->cached_layers = malloc(CACHED_LAYERS_SIZE*sizeof(buffer_handle_t));
        if (ctx->cached_layers == NULL) {
           ALOGE("Out of memory!\n");
           errno = -ENOMEM;
         return -1;
        }
    }

    ctx->cached_layers_count = 0;
    ctx->grabbed_all_layers = false;
    memset((buffer_handle_t*) ctx->cached_layers, 0, CACHED_LAYERS_SIZE*sizeof(buffer_handle_t));

    return 0;
}

void populate_cached_layer_locked(struct hwcomposer_context *ctx, hwc_layer_list_t* list)
{
    size_t i;
    int32_t compositionType = HWC_FRAMEBUFFER;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);

    /*reset count*/
    init_layer_cache_locked(ctx);

    /* lets poplulate the list and and return false since we need a new reference composition done */
    for (i = 0; i < list->numHwLayers; i++) {
        struct hwc_layer* layer = &list->hwLayers[i];

        if(layer->compositionType == HWC_OVERLAY)
            compositionType = HWC_OVERLAY;

        ctx->cached_layers[i] = layer->handle;
        ctx->cached_layers_count++;;
    }

    if(compositionType != HWC_OVERLAY)
    {
        /* We must find atleast one layer with HWC_OVERLAY otherwise the layer_cache is invalid */
        /* Let us clear the layer_cache instead*/
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "There were no HWC_OVERLAY layers, resetting cached_layers");
        ctx->cached_layers_count = 0;
    }
}

bool check_cached_layers_locked(struct hwcomposer_context *ctx, hwc_layer_list_t* list)
{
    size_t i;
    int32_t compositionType = HWC_FRAMEBUFFER;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);

    if (ctx->cached_layers == NULL) {
        init_layer_cache_locked(ctx);
    }

    /* Check if there are more layers than the cache can manage */
    if (list->numHwLayers > 16) {
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "list->numHwLayers > 16, list->numHwLayers = %d",
                list->numHwLayers);
        return false;
    }

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "cached_layers_count = %d, numHwLayers = %d",
            ctx->cached_layers_count, list->numHwLayers);

    if (ctx->cached_layers_count == 0 ||
        ctx->cached_layers_count != list->numHwLayers) {
        populate_cached_layer_locked(ctx, list);
        return false;
    } else {
        /* List already populated, lets see if layers match, video or camera buffers may
         * change, but order must be preserved */
        bool is_ok = true;
        for (i = 0; i < list->numHwLayers; i++) {
            struct hwc_layer* layer = &list->hwLayers[i];
            if (layer->handle != NULL && !(layer->flags & HWC_SKIP_LAYER)) {
                ALOGI_IF(DEBUG_STE_HWCOMPOSER, "iteration %d", i);

                if(layer->compositionType == HWC_OVERLAY){
                    compositionType = HWC_OVERLAY;
                    /*Do not care about the handle, most likely it changes*/
                    if (layer->handle == ctx->cached_layers[i]) {
                        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s Cached handle same as previous frame, %d, "
                        "%d Media is sending same buffers", __func__,
                        (int)layer->handle, (int)ctx->cached_layers[i]);
                    }
                    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "layer %d == %d cached, HWC_OVERLAY ok to mismatch",
                        (int)layer->handle, (int)ctx->cached_layers[i]);
                    /* check next layer */
                    continue;
                }

                if (layer->handle == ctx->cached_layers[i]) {
                    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "layer %d == %d cached",
                            (int)layer->handle, (int)ctx->cached_layers[i]);
                    /*great contuinue*/
                    continue;
                } else {
                    is_ok = false;
                    /* List has changed. No caching this time, lets populate list */
                    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "mismatch in list, layer %d == %d cache, repopulate",
                            (int)layer->handle, (int)ctx->cached_layers[i]);
                    populate_cached_layer_locked(ctx, list);
                    break;
                }
            }else {
                is_ok = false;
                ALOGI_IF(DEBUG_STE_HWCOMPOSER, "Some Layer is Null, layer %d == %d cache, reset counter",
                    (int)layer->handle, (int)ctx->cached_layers[i]);
                /* Do not populate since we do not want to store NULL in our cached_layers,
                 * repopulate next time */
                ctx->cached_layers_count = 0;
                break;
            }

        }
        return is_ok;
    }

    return false;
}


int grab_all_layers_locked(struct hwcomposer_context *ctx, hwc_layer_list_t* list)
{
    size_t i;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);

    if ((ctx->actual_composition_types_size != list->numHwLayers)
                                        && (ctx->actual_composition_types != NULL)) {
        free(ctx->actual_composition_types);
        ctx->actual_composition_types = NULL;
        ctx->actual_composition_types_size = 0;
    }

    if (ctx->actual_composition_types == NULL) {
        ctx->actual_composition_types = malloc(list->numHwLayers*sizeof(int32_t));
        ctx->actual_composition_types_size = list->numHwLayers;
        if (ctx->actual_composition_types == NULL) {
            ALOGE("Out of memory!\n");
            errno = -ENOMEM;
            return -1;
        }
    }

    for (i = 0; i < list->numHwLayers; i++) {
        struct hwc_layer* layer = &list->hwLayers[i];

        ctx->actual_composition_types[i] = layer->compositionType;
        layer->compositionType = HWC_OVERLAY;
    }
    /* info needed for making correct job in set()*/
    ctx->grabbed_all_layers = true;
    return 0;
}

void ungrab_fb_layers_locked(struct hwcomposer_context *ctx, hwc_layer_list_t* list)
{
    size_t i;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);

    for (i = 0; i < list->numHwLayers; i++) {
        struct hwc_layer* layer = &list->hwLayers[i];
            ALOGI_IF(DEBUG_STE_HWCOMPOSER,
            "HWC composition layer restored to composition type %d for handle %d",
            ctx->actual_composition_types[i], (int)layer->handle);
        layer->compositionType = ctx->actual_composition_types[i];
    }
}

static int hwcomposer_prepare(struct hwc_composer_device *dev, hwc_layer_list_t* list)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;
    size_t i;
    int ret = 0;

    pthread_mutex_lock(&ctx->hwc_mutex);
    ctx->bypass_ovly = false;

    if (NULL != list) {
        bool skip_layer_scene = false;
        /* Check for skip layer in the bottom layer */
        for (i = 0; i < list->numHwLayers; i++) {
            struct hwc_layer* layer = &list->hwLayers[i];
            if (layer->flags & HWC_SKIP_LAYER) {
                skip_layer_scene = true;
                break;
            }
        }
        if (skip_layer_scene || list->flags & HWC_GEOMETRY_CHANGED) {

            /* Find out if there is a video layer */
            ctx->videoplayback = false;
            for (i = 0; i < list->numHwLayers; i++) {
                struct hwc_layer* layer = &list->hwLayers[i];
                if (layer->handle != NULL &&
                        bufferIsHWMEM(ctx->gralloc, layer->handle) &&
                        bufferIsYUV(ctx->gralloc, layer->handle)) {
                    __u16 width;
                    __u16 height;
                    struct compdev_rect hdmi_res;

#ifdef ENABLE_HDMI
                    /* Video layer found */
                    ctx->videoplayback = true;
                    if (ctx->hdmi_settings.resolutionchanged ||
                            !ctx->hdmi_settings.hdmi_plugged)
                        break;
#endif

                    /* Convert to known resolution */
                    width = ctx->gralloc->perform(ctx->gralloc,
                            GRALLOC_MODULE_PERFORM_GET_BUF_WIDTH, layer->handle);
                    height = ctx->gralloc->perform(ctx->gralloc,
                            GRALLOC_MODULE_PERFORM_GET_BUF_HEIGHT, layer->handle);
#ifdef ENABLE_HDMI
                    get_hdmi_resolution(width, height, &hdmi_res);
#endif

#ifdef ENABLE_HDMI
                    /* Update hdmid with information on preferred resolution */
                    if (ctx->hdmi_settings.res.width != hdmi_res.width ||
                            ctx->hdmi_settings.res.height != hdmi_res.height) {
                        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "New HDMI resolution detected: "
                                "%d x %d", hdmi_res.width, hdmi_res.height);

                        /* Send a request to HDMIDaemon to update the resolution */
                        if (update_hdmi_prefered_resolution(ctx, &hdmi_res) < 0)
                            ALOGE("Failed to update HDMI preferred resolution");

                        ctx->hdmi_settings.res.width = hdmi_res.width;
                        ctx->hdmi_settings.res.height = hdmi_res.height;
                        ctx->hdmi_settings.resolutionchanged = true;
                    }
#endif
                }
            }

            /*
             * Strategy:
             * 1. Try to find a bottom layer that is not marked as HWC_SKIP_LAYER.
             *    Check rotation to eliminate B2R2 rotation, if it doesn't work,
             *    send it to the framebuffer and GL.
             *    Handle only YUV layers initially.
             * 2. TODO: Handle two layers in Compdev.
             *
             * General: All B2R2 work will be done inside the kernel in compdev.
             */
            for (i = 0; i < list->numHwLayers; i++) {
                struct hwc_layer* layer = &list->hwLayers[i];

                /* The initial state should always be HWC_FRAMEBUFFER */
                layer->compositionType = HWC_FRAMEBUFFER;

                /*
                 * We're currently only interested in handling the bottom layer.
                 * TODO: Remove when we implement support for two HWC layers.
                 */
                if (i > 0)
                    break;

                /* This bug has been seen on HC, probably solved. */
                if (layer->visibleRegionScreen.numRects <= 0) {
                    ALOGE("%s: RegionScreen.numRects=%d", __func__,
                            layer->visibleRegionScreen.numRects);
                    break;
                }

                /*
                 * After one skip layer has been found,
                 * all following layers must be handled by GPU.
                 * A layer with a NULL handle is equivalent with SKIP.
                 */
                if ((layer->flags & HWC_SKIP_LAYER) || layer->handle == NULL) {
                    /* A skipped layer might be needed by listeners for compdev */
                    if (layer->handle != NULL && bufferIsHWMEM(ctx->gralloc, layer->handle)) {
                        if (bufferIsYUV(ctx->gralloc, layer->handle)) {
                            ctx->bypass_ovly = true;
                            ALOGI_IF(DEBUG_STE_HWCOMPOSER,
                                    "Buffer for potential compdev "
                                    "listener bypass found.");
                        }
                    }
                    break;
                }

                if (bufferIsHWMEM(ctx->gralloc, layer->handle)) {
                    if (bufferIsYUV(ctx->gralloc, layer->handle)) {
                        ALOGI_IF(DEBUG_STE_HWCOMPOSER,
                            "HWC composition layer detected, layer->transform = %d",
                            layer->transform);

                        layer->compositionType = HWC_OVERLAY;
                        layer->hints = HWC_HINT_TRIPLE_BUFFER | HWC_HINT_CLEAR_FB;
                    }
                }
            }
        } else {
            ALOGI_IF(DEBUG_STE_HWCOMPOSER, "No changed geometry");
        }

        /*check if we can power optimize overlays*/
        if (list->numHwLayers != 1 && !ctx->pending_rotation) {
            if (check_cached_layers_locked(ctx, list))
            {
                ALOGI_IF(DEBUG_STE_HWCOMPOSER, "Full layer_cache hit, grab all layers, optimize");
                ret = grab_all_layers_locked(ctx, list);
                if(ret < 0) {
                    ALOGE("%s: Failed to grab all layers, %s", __func__,
                            strerror(errno));
                    goto exit;
                }
            }
        } else {
            /* clear old cache info */
            init_layer_cache_locked(ctx);
        }
        ctx->pending_rotation = false;
    }
    else {
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "list is NULL");
    }

#if DEBUG_STE_HWCOMPOSER_LAYER_DUMP
    if (NULL != list) {
        /*if (list->flags & HWC_GEOMETRY_CHANGED)*/ {
            ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "********************************************");
            ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP,  "Number of layers: %d\n", list->numHwLayers);
            for (i = 0; i < list->numHwLayers; i++) {
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "---------------------------------------------\n");
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Transform: %d", list->hwLayers[i].transform);
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Blending: %d", list->hwLayers[i].blending);
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Flags: %d", list->hwLayers[i].flags);
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "compositionType: %d", list->hwLayers[i].compositionType);
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Handle: %x", list->hwLayers[i].handle);
                if(ctx->cached_layers)
                    ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "cached Handle: %x", ctx->cached_layers[i]);
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "sourceCrop left %d, top %d, right %d, bottom %d",
                    list->hwLayers[i].sourceCrop.left,
                    list->hwLayers[i].sourceCrop.top,
                    list->hwLayers[i].sourceCrop.right,
                    list->hwLayers[i].sourceCrop.bottom);
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "displayFrame left %d, top %d, right %d, bottom %d",
                    list->hwLayers[i].displayFrame.left,
                    list->hwLayers[i].displayFrame.top,
                    list->hwLayers[i].displayFrame.right,
                    list->hwLayers[i].displayFrame.bottom);
                uint j;
                if (list->hwLayers[i].visibleRegionScreen.rects != NULL) {
                    for (j = 0; j < list->hwLayers[i].visibleRegionScreen.numRects; j++) {
                        ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "visibleRegionScreen%d left %d, top %d, right %d, bottom %d", j,
                        list->hwLayers[i].visibleRegionScreen.rects[j].left,
                        list->hwLayers[i].visibleRegionScreen.rects[j].top,
                        list->hwLayers[i].visibleRegionScreen.rects[j].right,
                        list->hwLayers[i].visibleRegionScreen.rects[j].bottom);
                    }
                }
                if (list->hwLayers[i].handle != NULL) {
                    int type;
                    int width;
                    int height;
                    int format;
                    int usage;

                    type = ctx->gralloc->perform(ctx->gralloc,
                            GRALLOC_MODULE_PERFORM_GET_BUF_TYPE, list->hwLayers[i].handle);

                    width = ctx->gralloc->perform(ctx->gralloc,
                            GRALLOC_MODULE_PERFORM_GET_BUF_WIDTH, list->hwLayers[i].handle);

                    height = ctx->gralloc->perform(ctx->gralloc,
                            GRALLOC_MODULE_PERFORM_GET_BUF_HEIGHT, list->hwLayers[i].handle);

                    format = ctx->gralloc->perform(ctx->gralloc,
                            GRALLOC_MODULE_PERFORM_GET_BUF_FORMAT, list->hwLayers[i].handle);

                    usage = ctx->gralloc->perform(ctx->gralloc,
                            GRALLOC_MODULE_PERFORM_GET_BUF_USAGE, list->hwLayers[i].handle);

                    if (type == GRALLOC_BUF_TYPE_HWMEM)
                        ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Buffer type GRALLOC_BUF_TYPE_HWMEM");
                    else if (type == GRALLOC_BUF_TYPE_GPU)
                        ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Buffer type GRALLOC_BUF_TYPE_GPU");
                    else
                        ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Buffer type UNKNOWN");

                    ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "Buffer width %d, height %d, format 0x%X, usage 0x%X",
                        width, height, format, usage);

                }
            }
                ALOGI_IF(DEBUG_STE_HWCOMPOSER_LAYER_DUMP, "***********************END************************\n");
        }
    }
#endif

#ifdef ENABLE_HDMI
    if ((!ctx->videoplayback || !ctx->hdmi_settings.hdmi_plugged) &&
            ctx->hdmi_settings.resolutionchanged) {
        /* Send a request to HDMIDaemon to update the resolution */
        if (ctx->hdmi_settings.hdmi_plugged) {
            ALOGI_IF(DEBUG_STE_HWCOMPOSER, "Setting hdmi default resolution");
            if (set_hdmi_default_resolution(ctx) < 0)
                ALOGE("Failed to set hdmi default resolution");
        }
        ctx->hdmi_settings.resolutionchanged = false;
        ctx->hdmi_settings.res.width = 0;
        ctx->hdmi_settings.res.height = 0;
    }
#endif

exit:
    pthread_mutex_unlock(&ctx->hwc_mutex);
    return ret;
}

static int hwcomposer_set(struct hwc_composer_device *dev,
                hwc_display_t dpy,
                hwc_surface_t sur,
                hwc_layer_list_t* list)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;
    int ret;
    uint32_t i;
    struct hwc_rect frame_rect;
    int compdevLayerCount = 0;
    int compdevBypassCount = 0;
    int layerCount = 0;
    enum compdev_transform app_transform = COMPDEV_TRANSFORM_ROT_0;
    enum compdev_transform fb_transform;
    enum compdev_transform hw_transform;

    /* Clear framerect for calculating visible area */
    memset(&frame_rect, 0, sizeof(frame_rect));
    frame_rect.top = frame_rect.left = INT_MAX;

    if (NULL != list) {
        pthread_mutex_lock(&ctx->hwc_mutex);

    /* Since prepare screwed up the compositiontype it is time for restoring it as well for
     * the wellbeeing of the rest. TODO Fix a better solution */
    if (ctx->grabbed_all_layers)
        ungrab_fb_layers_locked(ctx, list);

#if DEBUG_STE_HWCOMPOSER
        ctx->frames++;
        for (i = 0; i < list->numHwLayers; i++) {
            if (list->hwLayers[i].compositionType == HWC_OVERLAY)
                ctx->overlay_layers++;
            else
                ctx->framebuffer_layers++;
        }
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "Frame count=%d, HWC layers=%d, GL layers=%d, numHwLayers = %d",
                ctx->frames, ctx->overlay_layers, ctx->framebuffer_layers, list->numHwLayers);
#endif

        fb_transform = to_compdev_rotation(ctx->ui_orientation);
        hw_transform = to_compdev_rotation(ctx->hardware_rotation);

        /* 1. Figure out how many layers to send to compdev and the combined size */
        for (i = 0; i < list->numHwLayers; i++) {
            struct hwc_layer* layer = &list->hwLayers[i];

            if (layer->compositionType == HWC_FRAMEBUFFER) {
                break;
            }

            if (layer->visibleRegionScreen.numRects > 0 &&
                    layer->visibleRegionScreen.rects != NULL) {
                uint32_t rect_index;
                compdevLayerCount++;
                for (rect_index = 0; rect_index<layer->visibleRegionScreen.numRects; rect_index++) {
                    hwc_rect_t rectRot;
                    hwc_rect_t* rect = (hwc_rect_t*)&layer->visibleRegionScreen.rects[rect_index];
                    hwc_rotate(rect, layer->transform, to_degrees(fb_transform),
                            to_degrees(hw_transform), ctx->disp_width, ctx->disp_height, &rectRot);
                    frame_rect.top = min(rectRot.top, frame_rect.top);
                    frame_rect.left = min(rectRot.left, frame_rect.left);
                    frame_rect.right = max(rectRot.right, frame_rect.right);
                    frame_rect.bottom = max(rectRot.bottom, frame_rect.bottom);
                }
            }
        }
        layerCount = list->numHwLayers;

        /* 2. Post the scene to compdev */
        if (compdevLayerCount > 0 && layerCount != compdevLayerCount) {
            if (ctx->grabbed_all_layers) {
                ALOGI_IF(DEBUG_STE_HWCOMPOSER, "Forced1layer: fb_transform: %d, app_transform: %d, hw_transform %d",
                        fb_transform, app_transform, hw_transform);
                hwc_post_compdev_scene(ctx->compdev, fb_transform, app_transform, 1, 1, hw_transform);
            }
            else {
                ALOGI_IF(DEBUG_STE_HWCOMPOSER, "2layer: fb_transform: %d, app_transform: %d, hw_transform %d",
                        fb_transform, app_transform, hw_transform);
                hwc_post_compdev_scene(ctx->compdev, fb_transform, app_transform, 2, 0, hw_transform);
            }
        } else {
            /*
             * For normal use cases, show portrait application
             * standing up on the clone
             */
            if (((to_degrees(fb_transform) + to_degrees(hw_transform)) % 360) == 0)
                app_transform = COMPDEV_TRANSFORM_ROT_90_CW;

            ALOGI_IF(DEBUG_STE_HWCOMPOSER, "1layer: fb_transform: %d, app_transform: %d, hw_transform %d",
                    fb_transform, app_transform, hw_transform);
            hwc_post_compdev_scene(ctx->compdev, fb_transform, app_transform, 1, 0, hw_transform);
        }
    }

    /* Handle potential overlay bypass */
    if (ctx->bypass_ovly) {
        /* Check if compdev has any listeners. */
        if (compdevHasListener(ctx->compdev))
            compdevBypassCount++;

    }

    /* 3. Send layer/layers to compdev in a separate thread */
    if (compdevLayerCount > 0 || compdevBypassCount > 0) {
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Frame.left=%d, Frame.top=%d, Frame.right=%d, "
                "Frame.bottom=%d", __func__, frame_rect.left, frame_rect.top,
                frame_rect.right, frame_rect.bottom);
        if (worker_signal_update(ctx, list, compdevLayerCount,
                compdevBypassCount, &frame_rect))
            ALOGE("%s: Error signaling update to the worker", __func__);
    }

    /* If all layers are grabbed....do not call for a GPU job, buffer already composited */
    if (!ctx->grabbed_all_layers) {
        /* 4. Start the GPU */
        if (layerCount != compdevLayerCount || NULL == list) {
            /* Composition complete */
            ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Calling compositionComplete", __func__);
            ctx->gralloc->perform(ctx->gralloc, GRALLOC_MODULE_PERFORM_COMPOSITION_COMPLETE, NULL);

            /* Swap buffers */
            ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Calling eglSwapBuffers", __func__);
            if (dpy && sur)
                eglSwapBuffers(dpy, sur);
        }
    } else {
        ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: All layers are grabbed, skipping GPU job for optimal power performance", __func__);
    }

    /* 5. Wait for the the Blit jobs to finish */
    worker_wait_for_done(ctx);

    /* 6. This step is handled by libCompose */

    pthread_mutex_unlock(&ctx->hwc_mutex);
    return 0;
error:
    pthread_mutex_unlock(&ctx->hwc_mutex);
    return ret;
}

void hwcomposer_dump(struct hwc_composer_device* dev, char *buff, int buff_len)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;
    pthread_mutex_lock(&ctx->hwc_mutex);
    /* Nothing to do yet */
    pthread_mutex_unlock(&ctx->hwc_mutex);
}

static int hwcomposer_setparameter(struct hwc_composer_device *dev,
                int param, int value)
{
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;
    int ret = 0;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Setting parameter %d to %d",
            __func__, param, value);

    pthread_mutex_lock(&ctx->hwc_mutex);
    switch (param) {
        case HWC_UI_ORIENTATION:
            ctx->ui_orientation = value;
            ctx->pending_rotation = true;
            break;
        case HWC_HARDWARE_ROTATION:
            ctx->hardware_rotation = value;
            ctx->pending_rotation = true;
            break;
#ifdef ENABLE_HDMI
        case HWC_HDMI_PLUGGED:
            ctx->hdmi_settings.hdmi_plugged = value > 0 ? true : false;
            break;
#endif
        default:
            ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: No such parameter: %d",
                    __func__, param);
            ret = -1;
            break;
    }
    pthread_mutex_unlock(&ctx->hwc_mutex);

    return ret;
}

static void hwcomposer_register_procs(struct hwc_composer_device* dev, hwc_procs_t const* procs)
{
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;
    ctx->procs = (struct hwc_procs *)procs;
    ALOGI_IF(DEBUG_STE_HWCOMPOSER && procs, "%s: New set of procs registered.", __func__);
    ALOGI_IF(DEBUG_STE_HWCOMPOSER && !procs, "%s: procs deregistered", __func__);
}

static int hwcomposer_query(struct hwc_composer_device* dev, int what, int* value)
{
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: what=%d", __func__, what);
    switch (what) {
    case HWC_BACKGROUND_LAYER_SUPPORTED:
    {
        /* we don't support the background layer yet since this is
         * currently not used in JB.
         */
        value[0] = 0;
    }
    break;
    case HWC_VSYNC_PERIOD:
    {
        /* FIXME: implement. Does not seem to be used in JB though. */
        ALOGW("query for VSYNC period called but not implemented!");
        value[0] = 0;
    }
    break;
    default:
        // unsupported query
        return -EINVAL;
    }
    return 0;
}

static int hwcomposer_eventControl(struct hwc_composer_device* dev, int event, int enabled)
{
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;
    int ret = -EINVAL;

    if (enabled != 0 && enabled != 1)
        return -EINVAL;

    switch (event) {
    case HWC_EVENT_VSYNC:
    {
        if (enabled) {
            if (ctx->procs && ctx->procs->vsync) {
                ret = vsync_monitor_enable(ctx->procs);
            } else {
                ALOGW("%s: Enable VSYNC called without a registered callback", __func__);
            }
        } else {
            ret = vsync_monitor_disable();
        }
    }
    break;
    default:
        ALOGW("%s: got unknown event id: %d", __func__, event);
    }

    return ret;
}

static enum compdev_transform get_dst_transform(uint32_t hw_rot, enum compdev_transform img_transform)
{
    uint32_t img_rot = to_degrees(img_transform);
    enum compdev_transform transform = remove_rot(img_transform);

    return to_compdev_rotation((360 + img_rot - hw_rot) % 360) | transform;
}

static void worker_send_compdev_layer(struct worker_context* wctx)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    struct comp_img* dest_img = NULL;
    int ret = 0;
    uint32_t i = 0;
    struct compdev_img img;
    uint32_t usage = 0;
    enum compdev_transform rect_transform;
    double x_scale;
    double y_scale;
    hwc_rect_t localSourceCrop;

    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Frame.left=%d, Frame.top=%d, Frame.right=%d, "
            "Frame.bottom=%d", __func__, wctx->frame_rect->left,
            wctx->frame_rect->top, wctx->frame_rect->right,
            wctx->frame_rect->bottom);

    /* Direct composition */
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Direct composition", __func__);

    struct hwc_layer* layer = &wctx->work_list->hwLayers[0];

    if (convert_image(wctx->gralloc, layer->handle, &img))
        ALOGE("%s: Convert image failed", __func__);

    switch (layer->transform) {
        case HAL_TRANSFORM_ROT_90:
        case HAL_TRANSFORM_ROT_270:
        {
            x_scale = (double)(layer->sourceCrop.right - layer->sourceCrop.left)
                              / (double)(layer->displayFrame.bottom -
                                         layer->displayFrame.top);
            y_scale = (double)(layer->sourceCrop.bottom - layer->sourceCrop.top)
                              / (double)(layer->displayFrame.right -
                                         layer->displayFrame.left);
        }
        break;
        default:
        {
            x_scale = (double)(layer->sourceCrop.right - layer->sourceCrop.left)
                              / (double)(layer->displayFrame.right -
                                         layer->displayFrame.left);
            y_scale = (double)(layer->sourceCrop.bottom - layer->sourceCrop.top)
                              / (double)(layer->displayFrame.bottom -
                                         layer->displayFrame.top);
        }
    }

    localSourceCrop = layer->sourceCrop;
    if (layer->displayFrame.left < layer->visibleRegionScreen.rects[0].left)
        localSourceCrop.left = layer->sourceCrop.left +
            ((layer->visibleRegionScreen.rects[0].left -
              layer->displayFrame.left) * x_scale + 0.5);
    if (layer->displayFrame.top < layer->visibleRegionScreen.rects[0].top)
        localSourceCrop.top = layer->sourceCrop.top +
            ((layer->visibleRegionScreen.rects[0].top -
              layer->displayFrame.top) * y_scale + 0.5);
    if (layer->displayFrame.right > layer->visibleRegionScreen.rects[0].right)
        localSourceCrop.right = layer->sourceCrop.right -
            ((layer->displayFrame.right -
              layer->visibleRegionScreen.rects[0].right) * x_scale + 0.5);
    if (layer->displayFrame.bottom > layer->visibleRegionScreen.rects[0].bottom)
        localSourceCrop.bottom = layer->sourceCrop.bottom -
            ((layer->displayFrame.bottom -
              layer->visibleRegionScreen.rects[0].bottom) * y_scale + 0.5);

    img.src_rect.x = localSourceCrop.left;
    img.src_rect.y = localSourceCrop.top;
    img.src_rect.width = localSourceCrop.right - localSourceCrop.left;
    img.src_rect.height = localSourceCrop.bottom - localSourceCrop.top;

    img.transform = to_compdev_transform(layer->transform);

    rect_transform = get_dst_transform(wctx->hardware_rotation, img.transform);
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: rect_transform %d", __func__, rect_transform);

    hwc_get_dst_rect(wctx->frame_rect, rect_transform,
            wctx->disp_width, wctx->disp_height, &img.dst_rect);

    /* This layer should always be below the framebuffer */
    img.z_position = 2;
    img.flags = (uint32_t)COMPDEV_OVERLAY_FLAG;

    /* Set correct flags */
    if (wctx->compdev_bypass_count > 0)
        img.flags |= COMPDEV_BYPASS_FLAG;

    usage = wctx->gralloc->perform(wctx->gralloc,
            GRALLOC_MODULE_PERFORM_GET_BUF_USAGE, layer->handle);

    if (usage & GRALLOC_USAGE_EXTERNAL_DISP)
        img.flags |= COMPDEV_EXTERNAL_DISP_FLAG;

    if (usage & GRALLOC_USAGE_PROTECTED)
        img.flags |= COMPDEV_PROTECTED_FLAG;

    ret = ioctl(wctx->compdev, COMPDEV_POST_BUFFER_IOC,
            (struct compdev_img*)&img);
    if (ret < 0)
        ALOGE("%s: Failed to post buffers to compdev, %s", __func__,
                strerror(errno));

}


/* The worker thread function */
static void* worker(void* context)
{
    struct worker_context* wctx = (struct worker_context*)context;
    pthread_mutex_lock(&wctx->mutex);
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Worker started", __func__);
    while (wctx->alive) {
       pthread_cond_wait(&wctx->signal_update, &wctx->mutex);
       if (wctx->alive) {
           ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s: Worker processing layers", __func__);
           worker_send_compdev_layer(wctx);
           wctx->working = false;
           pthread_cond_signal(&wctx->signal_done);
       }
    }
    pthread_mutex_unlock(&wctx->mutex);
    pthread_exit(NULL);
    return 0;
}

static int worker_init(struct hwcomposer_context *ctx,
                        int hwmem, int compdev,
                        const struct gralloc_module_t *gralloc)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    int ret = 0;
    pthread_mutex_init(&ctx->worker_ctx.mutex, NULL);
    pthread_mutex_lock(&ctx->worker_ctx.mutex);

    pthread_attr_init(&ctx->worker_ctx.thread_attr);
    pthread_attr_setdetachstate(&ctx->worker_ctx.thread_attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&ctx->worker_ctx.worker_thread, &ctx->worker_ctx.thread_attr, worker, (void*)&ctx->worker_ctx);
    pthread_cond_init(&ctx->worker_ctx.signal_update, NULL);
    pthread_cond_init(&ctx->worker_ctx.signal_done, NULL);
    ctx->worker_ctx.alive = true;
    ctx->worker_ctx.hwmem = hwmem;
    ctx->worker_ctx.compdev = compdev;
    ctx->worker_ctx.gralloc = gralloc;
    ctx->worker_ctx.disp_width = ctx->disp_width;
    ctx->worker_ctx.disp_height = ctx->disp_height;
    pthread_mutex_unlock(&ctx->worker_ctx.mutex);
    return ret;
}

static int worker_destroy(struct hwcomposer_context *ctx)
{
    int i;
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    pthread_mutex_lock(&ctx->worker_ctx.mutex);
    /* Kill the thread */
    ctx->worker_ctx.alive = false;
    ctx->worker_ctx.working = false;
    pthread_cond_signal(&ctx->worker_ctx.signal_update);
    pthread_mutex_unlock(&ctx->worker_ctx.mutex);
    pthread_join(ctx->worker_ctx.worker_thread, NULL);
    pthread_mutex_lock(&ctx->worker_ctx.mutex);
    pthread_cond_destroy(&ctx->worker_ctx.signal_update);
    pthread_cond_destroy(&ctx->worker_ctx.signal_done);
    pthread_attr_destroy(&ctx->worker_ctx.thread_attr);
    pthread_mutex_unlock(&ctx->worker_ctx.mutex);
    return 0;
}

static int worker_signal_update(struct hwcomposer_context *ctx,
                                hwc_layer_list_t* work_list,
                                uint32_t compdev_layer_count,
                                uint32_t compdev_bypass_count,
                                struct hwc_rect* frame_rect)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    pthread_mutex_lock(&ctx->worker_ctx.mutex);
    ctx->worker_ctx.work_list = work_list;
    ctx->worker_ctx.compdev_layer_count = compdev_layer_count;
    ctx->worker_ctx.compdev_bypass_count = compdev_bypass_count;
    ctx->worker_ctx.frame_rect = frame_rect;
    ctx->worker_ctx.working = true;
    ctx->worker_ctx.ui_orientation = ctx->ui_orientation;
    ctx->worker_ctx.hardware_rotation = ctx->hardware_rotation;
    pthread_cond_signal(&ctx->worker_ctx.signal_update);
    pthread_mutex_unlock(&ctx->worker_ctx.mutex);
    return 0;
}

static int worker_wait_for_done(struct hwcomposer_context *ctx)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    pthread_mutex_lock(&ctx->worker_ctx.mutex);
    /* NOTE: This check is a slight overkill since the mutex is proof enough */
    if (ctx->worker_ctx.working)
        pthread_cond_wait(&ctx->worker_ctx.signal_done, &ctx->worker_ctx.mutex);
    pthread_mutex_unlock(&ctx->worker_ctx.mutex);
    return 0;
}

static int hwcomposer_close(struct hw_device_t *dev)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    struct hwcomposer_context *ctx = (struct hwcomposer_context *)dev;

    if (ctx) {
        pthread_mutex_lock(&ctx->hwc_mutex);
        if (worker_destroy(ctx))
            ALOGE("Error destroying egl worker");

        vsync_monitor_destroy();

        close(ctx->compdev);
        close(ctx->hwmem);
#ifdef ENABLE_HDMI
        close(ctx->hdmi_settings.hdmid_sockfd);
#endif

        if (ctx->cached_layers != NULL)
            free(ctx->cached_layers);

        if (ctx->actual_composition_types != NULL)
            free(ctx->actual_composition_types);
        /*
         * TODO: Use reference counting and cleanup the ctx only
         * when there are no more clients
         */
        pthread_mutex_destroy(&ctx->hwc_mutex);
        free(ctx);
    }

    return 0;
}

static int hwcomposer_device_open(const struct hw_module_t *module,
        const char *name, struct hw_device_t **device)
{
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    ALOGI_IF(DEBUG_STE_HWCOMPOSER, "HWComposer device opened by pid=%d", getpid());
    struct hwcomposer_context *ctx;
    if (!strcmp(name, HWC_HARDWARE_COMPOSER)) {
        ctx = malloc(sizeof(*ctx));
        if (!ctx)
            return -ENOMEM;

        memset(ctx, 0, sizeof(*ctx));

        pthread_mutex_init(&ctx->hwc_mutex, NULL);
        pthread_mutex_lock(&ctx->hwc_mutex);

        ctx->dev.common.tag = HARDWARE_DEVICE_TAG;
        ctx->dev.common.version = STE_HWC_DEVICE_API_CURRENT;
        ctx->dev.common.module = (struct hw_module_t *)module;
        ctx->dev.common.close = hwcomposer_close;

        ctx->dev.prepare       = hwcomposer_prepare;
        ctx->dev.set           = hwcomposer_set;
        ctx->dev.dump          = hwcomposer_dump;
        ctx->dev.registerProcs = hwcomposer_register_procs;
        ctx->dev.query         = hwcomposer_query;
        ctx->dev.methods       = &hwcomposer_methods;

        ctx->hwmem = open(HWMEM_PATH, O_RDWR);
        if (ctx->hwmem < 0) {
            ALOGE("%s: could not open %s (%s)", __func__, HWMEM_PATH, strerror(errno));
            goto error;
        }

        ctx->compdev = open(COMPDEV_PATH, O_RDWR, 0);
        if (ctx->compdev < 0) {
            ALOGE("%s: Error Opening "COMPDEV_PATH": %s\n",__func__,
                    strerror(errno));
            goto compdev_error;

        }

        struct compdev_size display_size;
        if (ioctl(ctx->compdev, COMPDEV_GET_SIZE_IOC, (struct compdev_size*)&display_size)) {
            ALOGE("%s: Failed to get size from compdev, %s", __func__, strerror(errno));
            goto error;
        }

        ctx->disp_width = display_size.width;
        ctx->disp_height = display_size.height;

        if (hw_get_module(GRALLOC_HARDWARE_MODULE_ID,
            (const struct hw_module_t **)&ctx->gralloc)) {
            ALOGE("Error opening gralloc module");
            goto gralloc_error;
        }

        if (worker_init(ctx, ctx->hwmem, ctx->compdev, ctx->gralloc)) {
            ALOGE("Error initializing egl worker");
            goto worker_error;
        }

        if (vsync_monitor_init(ctx->compdev)) {
            ALOGE("Error initializing VSync monitor");
            goto worker_error;
        }

#ifdef ENABLE_HDMI
        ctx->hdmi_settings.hdmid_sockfd = open_hdmid_socket();
        if (ctx->hdmi_settings.hdmid_sockfd < 0)
            ALOGE("Failed to open communication channel to hdmid");
#endif

        *device = &ctx->dev.common;
        pthread_mutex_unlock(&ctx->hwc_mutex);
    } else
        return -EINVAL;

    return 0;
worker_error:
gralloc_error:
    close(ctx->compdev);
compdev_error:
    close(ctx->hwmem);
error:
    if (ctx)
        free(ctx);
    pthread_mutex_unlock(&ctx->hwc_mutex);
    return -1;
}

