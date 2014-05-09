/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 * Author: Julius Gustavson <julius.gustavsson@stericsson.com> for ST-Ericsson.
 */

#define LOG_TAG "STE-HWComposer-VSyncMonitor"
#define DEBUG_STE_HWCOMPOSER 0
/* Uncomment to enable verbose prints */
// #define LOG_NDEBUG 0


#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <linux/compdev.h>
#include <cutils/log.h>

#include "vsync_monitor.h"

/* Set this flag until the video mode support is integrated */
#define FAKE_VSYNC 1

// Thread instance data
struct vsync_monitor_context {
    pthread_t               thread;
    pthread_mutex_t         mutex;
    pthread_cond_t          cond_var;
    bool                    stop;
    bool                    enable;
    struct hwc_procs*       procs;
    int                     compdev_fd;
#ifdef FAKE_VSYNC
    struct timespec         next_vsync;
#endif
};


static struct vsync_monitor_context s_ctx = {
    .thread     = 0,
    .mutex      = PTHREAD_MUTEX_INITIALIZER,
    .cond_var   = PTHREAD_COND_INITIALIZER,
    .stop       = true,
    .enable     = false,
    .procs      = NULL,
    .compdev_fd = -1,
};

#define NSEC_PER_SEC    1000000000L
#define NSEC_PER_MSEC   1000000L

#ifdef FAKE_VSYNC
#ifndef REFRESH_RATE
#define REFRESH_RATE (60.0)
#endif // REFRESH_RATE
#define REFRESH_PERIOD ((int64_t)(NSEC_PER_SEC / REFRESH_RATE))

#endif // FAKE_VSYNC

/* ta < tb  --> -1
 * ta == tb --> 0
 * ta > tb  --> 1
 */
static inline int timespec_compare(const struct timespec *ta,
                                   const struct timespec *tb)
{
    if (ta->tv_sec < tb->tv_sec)
        return -1;
    if (ta->tv_sec > tb->tv_sec)
        return 1;
    return ta->tv_nsec - tb->tv_nsec;
}

static inline void timespec_add_ns(struct timespec* ts, int64_t nsec)
{
    int64_t tmp_nsec = ts->tv_nsec + nsec;
    while (tmp_nsec >= NSEC_PER_SEC) {
        ts->tv_sec += 1;
        tmp_nsec -= NSEC_PER_SEC;
    }
    ts->tv_nsec = tmp_nsec;
}

static inline int64_t timespec_to_ns(struct timespec* ts)
{
    int64_t tmp_nsec = (int64_t)ts->tv_sec * NSEC_PER_SEC;
    tmp_nsec += ts->tv_nsec;
    ALOGD_IF(DEBUG_STE_HWCOMPOSER && tmp_nsec < 0, "%s: overflow!  sec: %ld nsec: %ld",
                                                        __func__, ts->tv_sec, ts->tv_nsec);
    return tmp_nsec;
}

static int sleep_ns_locked(struct vsync_monitor_context* ctx, int64_t nsec)
{
    struct timespec timeout;
    int ret = 0;
    clock_gettime(CLOCK_MONOTONIC, &timeout);
    timespec_add_ns(&timeout, nsec);
    ALOGV("Sleeping %dms ..", (int)(nsec / NSEC_PER_MSEC));

    while(!ctx->stop && ctx->enable && ret == 0) {
        ret = pthread_cond_timedwait_monotonic_np(&ctx->cond_var, &ctx->mutex, &timeout);
    }

    if (ret && ret != ETIMEDOUT) {
            ALOGW("%s: pthread_cond_timedwait failed: error: %d (%s)", __func__,
                                                                ret, strerror(errno));
            return ret;
    }
    ALOGV("Woke up ..");
    return 0;
}

static inline int64_t wait_for_next_vsync_locked(struct vsync_monitor_context* ctx)
{
    int64_t timestamp;
#ifdef FAKE_VSYNC
    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);
    if (timespec_compare(&now, &ctx->next_vsync) < 0) {
        // we have time to spare. sleep until next vsync
        int ret = 0;
        int64_t sleep = timespec_to_ns(&ctx->next_vsync) - timespec_to_ns(&now);
        if (sleep_ns_locked(ctx, sleep)) {
            return -1;
        }
    }
    timestamp = timespec_to_ns(&ctx->next_vsync);
    timespec_add_ns(&ctx->next_vsync, REFRESH_PERIOD);
#else
    pthread_mutex_unlock(&ctx->mutex);

    if(ioctl(ctx->compdev_fd, COMPDEV_WAIT_FOR_VSYNC_IOC, &timestamp)) {
        timestamp = -1;
    }

    pthread_mutex_lock(&ctx->mutex);
#endif // FAKE_VSYNC

    return timestamp;
}

static void vsync_monitor_cleanup_locked()
{
    s_ctx.thread = 0;
    s_ctx.stop = true;
    s_ctx.enable = false;
    s_ctx.procs = NULL;
    s_ctx.compdev_fd = -1;
}

static void* vsync_monitor_loop(void *data)
{
    struct vsync_monitor_context* ctx = (struct vsync_monitor_context*) data;
#ifdef FAKE_VSYNC
    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "Generating VSYNC with rate: %.2f Hz", REFRESH_RATE);
    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "VSYNC period: %lld ns", REFRESH_PERIOD);
#endif

    setpriority(PRIO_PROCESS, 0, HAL_PRIORITY_URGENT_DISPLAY);

    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "VSync monitor thread running");

    pthread_mutex_lock(&ctx->mutex);

    for (;;) {
        int64_t timestamp;

        if (ctx->stop) {
            ALOGD_IF(DEBUG_STE_HWCOMPOSER, "VSync monitor stopping.");
            break;
        }

        if (!ctx->enable) {
            ALOGD_IF(DEBUG_STE_HWCOMPOSER, "VSync monitor disabled, sleeping...");

            while(!ctx->stop && !ctx->enable)
                    pthread_cond_wait(&ctx->cond_var, &ctx->mutex);

            ALOGD_IF(DEBUG_STE_HWCOMPOSER, "VSync monitor woke up %s...",
                                            (ctx->enable) ? "enabled" : "disabled");
#ifdef FAKE_VSYNC
            clock_gettime(CLOCK_MONOTONIC, &ctx->next_vsync);
            timespec_add_ns(&ctx->next_vsync, REFRESH_PERIOD);
#endif
            continue;
        }

        timestamp = wait_for_next_vsync_locked(ctx);

        if (timestamp <= 0) {
            ALOGW("Failed to wait for VSYNC. Re-trying...");
        } else if (!ctx->enable) {
            ALOGD_IF(DEBUG_STE_HWCOMPOSER, "Ignored VSYNC t=%lld", timestamp);
        } else if (ctx->procs && ctx->procs->vsync){
            int ret = 0;
            struct hwc_procs* procs = ctx->procs;
            ALOGV("Got vsync t=%lld", timestamp);
            /* Call the vsync callback while unlocked otherwise there is risk for deadlock
             * in case surfaceflinger tries to disable us at the same time as we trigger
             * the callback */
            pthread_mutex_unlock(&ctx->mutex);
            procs->vsync(procs, 0, timestamp);
            pthread_mutex_lock(&ctx->mutex);
        } else {
            ALOGW("Unable to deliver VSYNC. Calback was NULL!");
        }
    }

    vsync_monitor_cleanup_locked();
    pthread_mutex_unlock(&ctx->mutex);
    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "VSync monitor stopped.");
    return NULL;
}


int vsync_monitor_init(int compdev_fd)
{
    if (s_ctx.thread)
        return 0;  // Already started

    s_ctx.stop = false;
    s_ctx.compdev_fd = compdev_fd;

    if (pthread_create(&(s_ctx.thread), NULL,
                        vsync_monitor_loop, (void *)&s_ctx) < 0) {
        ALOGW("%s: Failed create vsync monitor thread!", __func__);
        return -errno;
    }

    return 0;

}

int vsync_monitor_enable(struct hwc_procs* procs)
{
    int ret = 0;
    pthread_mutex_lock(&s_ctx.mutex);

    if (s_ctx.thread == 0) {
        ALOGE("%s: Monitor thread is not running!", __func__);
        ret = -1;
        goto unlock;
    }

    if (s_ctx.enable)
        goto unlock;

    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);
    s_ctx.procs = procs;
    s_ctx.enable = true;

    pthread_cond_signal(&s_ctx.cond_var);

unlock:
    pthread_mutex_unlock(&s_ctx.mutex);
    return ret;
}

int vsync_monitor_disable()
{
    int ret = 0;
    pthread_mutex_lock(&s_ctx.mutex);

    if (s_ctx.thread == 0) {
        ALOGW("%s: Monitor thread is not running!", __func__);
        goto unlock;
    }

    if (!s_ctx.enable)
        goto unlock; // aldready disabled

    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "%s", __func__);

    s_ctx.procs = NULL;
    s_ctx.enable = false;

    pthread_cond_signal(&s_ctx.cond_var);

unlock:
    pthread_mutex_unlock(&s_ctx.mutex);
    return ret;
}

void vsync_monitor_destroy()
{
    if (s_ctx.thread == 0)
        return;  // Already stopped

    pthread_mutex_lock(&s_ctx.mutex);

    s_ctx.stop = true;
    s_ctx.enable = false;

    pthread_cond_signal(&s_ctx.cond_var);
    pthread_mutex_unlock(&s_ctx.mutex);
    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "Joining with vsync monitor thread!");
    pthread_join(s_ctx.thread, NULL);
    ALOGD_IF(DEBUG_STE_HWCOMPOSER, "vsync monitor thread stopped!");
}

