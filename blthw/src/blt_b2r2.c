/*
 * Copyright (C) ST-Ericsson AB 2009 - All rights reserved
 * Copyright (C) 2014 NovaFusion http://novafusion.pl
 * Reproduction and Communication of this document is strictly prohibited
 * unless specifically authorized in writing by ST-Ericsson
 *
 * \file    blt_b2r2.c
 * \brief   Android driver for B2R2 hardware
 * \author  ST-Ericsson
 *
 */

#define LOG_TAG "libblt_hw"

#include <cutils/log.h>

#include <blt_api.h>
#include <video/b2r2_blt.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define B2R2_BLT_DEV "/dev/b2r2_blt"

enum thread_event {
    EVENT_NONE = 0,
    EVENT_HANDLE_TERMINATED = 1,
    EVENT_REQUEST_CALLBACK = 2
};

struct blt_b2r2_data {
    int fd;
    pthread_t callback_thread;
    pthread_cond_t event_cond;
    pthread_mutex_t event_mutex;
    volatile enum thread_event event;
    volatile int number_reports;
};

#define DATAS_START_SIZE 10
#define DATAS_GROW_SIZE 5
static struct blt_b2r2_data **datas = NULL;
static int data_count = 0;

static int grow_datas(void)
{
    struct blt_b2r2_data **new_datas = NULL;
    int new_data_count = data_count + DATAS_GROW_SIZE;

    new_datas = malloc(new_data_count*sizeof(*new_datas));
    if (new_datas == NULL) {
        ALOGE("Out of memory!");
        errno = -ENOMEM;
        goto error;
    }

    memset(new_datas, 0, new_data_count*sizeof(*new_datas));
    memcpy(new_datas, datas, data_count*sizeof(*datas));

    free(datas);

    data_count = new_data_count;
    datas = new_datas;

    return 0;

error:
    return -1;
}

static int get_handle(struct blt_b2r2_data *data) {
    int handle;

    if (datas == NULL) {

        datas = malloc(DATAS_START_SIZE*sizeof(*datas));
        if (datas == NULL) {
            ALOGE("Out of memory!\n");
            errno = -ENOMEM;
            goto error;
        }

        data_count = DATAS_START_SIZE;
        memset(datas, 0, data_count*sizeof(*datas));
    }

    for (handle = 0; handle < data_count; handle++) {
        if (datas[handle] == NULL) {
            datas[handle] = data;
            break;
        }

        if (handle == data_count - 1) {
            if (grow_datas() < 0)
                goto error;
        }
    }

    return handle;

error:
    return -1;
}

static struct blt_b2r2_data *get_data(int handle) {
    if (handle >= data_count || handle < 0)
        return NULL;
    else
        return datas[handle];
}

static void free_handle(int handle) {
    if (handle < data_count && handle >= 0) {
        datas[handle] = NULL;
    }
}

static void *callback_thread_run(void *arg)
{
    struct blt_b2r2_data *data = (struct blt_b2r2_data *) arg;
    int number_reports = 0;

    while (1) {
        enum thread_event event;
        struct b2r2_blt_report report;

        /* Read parent command */
        pthread_mutex_lock(&data->event_mutex);
        if (data->event == EVENT_NONE)
            pthread_cond_wait(&data->event_cond, &data->event_mutex);
        event = data->event;
        number_reports += data->number_reports;
        data->event = EVENT_NONE;
        data->number_reports = 0;
        pthread_mutex_unlock(&data->event_mutex);

        while (number_reports > 0) {
            ssize_t count;
            memset(&report, 0, sizeof(report));
            count = read(data->fd, &report, sizeof(report));
            if (count < 0) {
                ALOGE("Pt%d: Could not read report from b2r2 (%s)",
                    data->fd, strerror(errno));
                goto thread_exit;
            } else if (report.report1 != 0) {
                void (*callback)(int, uint32_t) = (void*)report.report1;
                callback(report.request_id, (uint32_t)report.report2);
                number_reports--;
            }
        }

        if (event == EVENT_HANDLE_TERMINATED) {
            goto thread_exit;
        }
    }

thread_exit:
    if (number_reports) {
        ALOGE("Pt%d: Exit with outstanding reports: %d",
            data->fd, number_reports);
    }

    return NULL;
}

int blt_open(void)
{
    struct blt_b2r2_data *data = NULL;
    int fd;
    int handle;

    fd = open(B2R2_BLT_DEV, O_RDWR);
    if (fd < 0) {
        ALOGE("Could not open device %s", B2R2_BLT_DEV);
        goto error;
    }

    data = malloc(sizeof(struct blt_b2r2_data));
    if (data == NULL) {
        ALOGE("Out of memory");
        goto error;
    }

    memset(data, 0, sizeof(*data));

    data->fd = fd;
    data->callback_thread = -1;
    data->event = EVENT_NONE;
    data->number_reports = 0;
    pthread_cond_init(&data->event_cond, NULL);
    pthread_mutex_init(&data->event_mutex, NULL);

    handle = get_handle(data);
    if (handle < 0)
        goto error_free;

    pthread_create(&data->callback_thread, NULL, callback_thread_run,
            (void *)data);

    ALOGI("Library opened (handle = %d, fd = %d)", handle, fd);

    return handle;

error_free:
    free(data);
error:
    return -1;
}

void blt_close(int blt_handle)
{
    struct blt_b2r2_data *data = get_data(blt_handle);

    if (data == NULL)
        goto out;

    if (data->callback_thread > 0) {
        pthread_mutex_lock(&data->event_mutex);
        data->event = EVENT_HANDLE_TERMINATED;
        pthread_cond_signal(&data->event_cond);
        pthread_mutex_unlock(&data->event_mutex);
        pthread_join(data->callback_thread, NULL);
    }

    pthread_mutex_destroy(&data->event_mutex);
    pthread_cond_destroy(&data->event_cond);
    close(data->fd);

    ALOGI("Library closed (handle = %d, fd = %d)",
            blt_handle, data->fd);
    free_handle(blt_handle);
    free(data);
out:
    return;
}

int blt_request(int blt_handle, struct blt_req *req)
{
    struct blt_b2r2_data *data = get_data(blt_handle);
    int ret;

    if (data == NULL) {
        errno = EINVAL;
        ret = -1;
        goto out;
    }

    if (req->callback != NULL) {
        req->flags |= B2R2_BLT_FLAG_REPORT_WHEN_DONE;
#ifdef BLT_B2R2_DEBUG_PERFORMANCE
        req->flags |= B2R2_BLT_FLAG_REPORT_PERFORMANCE;
#endif
    }

    ret = ioctl(data->fd, B2R2_BLT_IOC, (struct b2r2_blt_req *) req);
    if (ret < 0)
        goto out;

    if (req->callback != NULL) {
        /* Tell callback worker that there will be a report
         * to read */
        pthread_mutex_lock(&data->event_mutex);
        data->event = EVENT_REQUEST_CALLBACK;
        data->number_reports++;
        pthread_cond_signal(&data->event_cond);
        pthread_mutex_unlock(&data->event_mutex);
    }

out:
    return ret;
}

int blt_synch(int blt_handle, int request_id)
{
    struct blt_b2r2_data *data = get_data(blt_handle);

    if (data == NULL) {
        errno = EINVAL;
        return -1;
    }

    return ioctl(data->fd, B2R2_BLT_SYNCH_IOC, request_id);
}

int blt_query_cap(int blt_handle,
		enum blt_fmt fmt,
		enum blt_cap capability,
		uint32_t *cap)
{
    ALOGE("blt_query_cap not implemented yet");
    errno = ENOSYS;
    return -1;
}
