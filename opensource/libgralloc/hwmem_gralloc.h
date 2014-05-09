/*
 * Copyright (C) ST-Ericsson AB 2010
 *
 * \file    hwmem_gralloc.h
 * \brief   Gralloc implementation that uses hwmem
 * \author  Johan Mossberg <johan.xx.mossberg@stericsson.com> and
 * Fredrik Allansson <fredrik.allansson@stericsson.com> for ST-Ericsson
 *
 */


#ifndef HWMEM_GRALLOC_H_
#define HWMEM_GRALLOC_H_


#include <pthread.h>
#include <linux/fb.h>
#include <cutils/native_handle.h>


/* Pmem stuff copied from the old gralloc implementation. */
enum {
    PRIV_FLAGS_FRAMEBUFFER = 0x00000001,
    PRIV_FLAGS_USES_PMEM   = 0x00000002,
};

struct hwmem_gralloc_buf_handle_t
{
    native_handle_t base;

    /* File descriptors */
    int fd;

    /* Ints */
    int type_identifier;

    /* Actual width and height of the buffer, not the values supplied to alloc. */
    int width;
    int height;
    int format;

    int usage;

    /* TODO: Remove when "create handle from buffer" is removed */
    int offset;
    /* If not -1 then the this handle wrapps a buffer not created by gralloc */
    int size;
    int addr;

    /* TODO: Remove when pmem and old framebuffer device implementation is gone! */
    int type;

    /* Pmem stuff copied from the old gralloc implementation. */
    int     flags;
    int     base_addr;
    int     lockState;
    int     writeOwner;
    int     pid;
};

extern const int num_fds_in_hwmem_gralloc_buf_handle;
extern const int num_ints_in_hwmem_gralloc_buf_handle;
extern const int hwmem_gralloc_buf_handle_type_identifier;


struct hwmem_gralloc_module_t {
    struct gralloc_module_t base;

    int type_identifier;

    /* For the old framebuffer implementation, remove when the new framebuffer implementation is
    in place */
    pthread_mutex_t lock;
    struct hwmem_gralloc_buf_handle_t* framebuffer;
    uint32_t flags;
    uint32_t numBuffers;
    uint32_t bufferMask;
    int currentBufferIndex;
    buffer_handle_t currentBuffer;

    struct fb_var_screeninfo info;
    struct fb_fix_screeninfo finfo;
    float xdpi;
    float ydpi;
    float fps;
};

#define HWMEM_GRALLOC_MODULE_TYPE_IDENTIFIER 0xCC20FA38

int module_2_hwmem_gralloc_module(struct gralloc_module_t const* module,
    struct hwmem_gralloc_module_t** gralloc_out);
int handle_2_hwmem_gralloc_handle(buffer_handle_t handle,
        struct hwmem_gralloc_buf_handle_t** buf_out);


#endif /* HWMEM_GRALLOC_H_ */
