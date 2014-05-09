/*
 * Copyright (C) ST-Ericsson AB 2010
 *
 * \file    gralloc_stericsson_ext.h
 * \brief   Gralloc implementation that uses hwmem
 * \author  Johan Mossberg <johan.xx.mossberg@stericsson.com> and
 * Fredrik Allansson <fredrik.allansson@stericsson.com> for ST-Ericsson
 *
 */


#ifndef GRALLOC_STERICSSON_EXT_H
#define GRALLOC_STERICSSON_EXT_H


enum {
    GRALLOC_BUF_TYPE_HWMEM,
    GRALLOC_BUF_TYPE_HWMEM_CONTIGUOUS = GRALLOC_BUF_TYPE_HWMEM,
    GRALLOC_BUF_TYPE_HWMEM_SCATTERED,
    GRALLOC_BUF_TYPE_PMEM,
    GRALLOC_BUF_TYPE_FB,
    GRALLOC_BUF_TYPE_GPU,
};

enum {
    /*
     * params: buffer_handle_t handle
     *
     * Returns a handle to the buffer that works with the underlying allocator or
     * a negative error code on failure. The returned handle is owned by gralloc
     * and shall therefore not be closed.
     */
    GRALLOC_MODULE_PERFORM_GET_BUF_ALLOCATOR_HANDLE,

    /*
     * DEPRECATED: Pin from driver instead!
     *
     * params buffer_handle_t handle
     *
     * Pins the buffer in memory and returns the physical address of the
     * buffer.
     *
     * On failure a negative error code is returned.
     */
    GRALLOC_MODULE_PERFORM_PIN_BUF,

    /*
     * DEPRECATED: Unpin from driver instead!
     *
     * params buffer_handle_t handle
     *
     * Unpins the buffer and makes it possible for the underlying allocator to
     * perform maintainance tasks such as defragmentation and swapping.
     *
     * After calling unpin, the physical address returned from pin is no
     * longer valid.
     *
     * Returns zero on success or a negative error code on failure.
     */
    GRALLOC_MODULE_PERFORM_UNPIN_BUF,

    /*
     * TODO: Remove when "create handle from buffer" is removed
     *
     * params: buffer_handle_t handle
     *
     * Returns the buffer's offset or a negative error code on failure.
     */
    GRALLOC_MODULE_PERFORM_GET_BUF_OFFSET,

    /*
     * TODO: Remove when pmem and the old framebuffer device implementation is gone.
     *
     * params: buffer_handle_t handle
     *
     * Returns the buffer type or a negative error code on failure.
     */
    GRALLOC_MODULE_PERFORM_GET_BUF_TYPE,

    /*
     * TODO: Remove when "create handle from buffer is removed" is removed
     *       (This is only really needed for pmem buffers)
     *
     * params: buffer_handle_t handle
     *
     * Returns the buffer's size or a negative error code on failure.
     */
    GRALLOC_MODULE_PERFORM_GET_BUF_SIZE,

    GRALLOC_MODULE_PERFORM_GET_BUF_WIDTH,
    GRALLOC_MODULE_PERFORM_GET_BUF_HEIGHT,
    GRALLOC_MODULE_PERFORM_GET_BUF_FORMAT,
    GRALLOC_MODULE_PERFORM_GET_BUF_USAGE,
    GRALLOC_MODULE_PERFORM_COMPOSITION_COMPLETE,

};


#endif
