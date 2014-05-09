/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __TRACE_H__
#define __TRACE_H__

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Open the trace driver
 * Return 0 if OK, an error code otherwise.
 */
int stm_trace_open(void);

/* Close the trace driver */
void stm_trace_close(void);

/* Provides stm_trace_XX() and stm_tracet_XX() trace API
 * - These functions must be called after a successful call to stm_trace_open()
 *   For efficiency reasons, there is no check in these functions
 *   => these functions must NOT be called if stm_trace_open() failed or if
 *      stm_trace_close() has been called.
 * - These functions are lock-less
 * - The main purpose of these functions is to satisfy NMF needs
 *   i.e. direct output some traces on a given channel
 *   The use of these API in combination with stm_trace_buffer()
 *   outside the scope of NMF may lead to a mix of traces in some channels !
 *   Otherwise, the driver must be updated to reserve some channels to not use
 *   them through the stm_trace_buffer() API
 * - The channel number must be between 0 and 255
 */
#define DECLLLTFUN(type, size) \
	void stm_trace_##size(unsigned char channel, unsigned type data); \
	void stm_tracet_##size(unsigned char channel, unsigned type data)
DECLLLTFUN(char, 8);
DECLLLTFUN(short, 16);
DECLLLTFUN(int, 32);
DECLLLTFUN(long long, 64);

/* Retrieve the channel ID if already registered
   if not already registered then register this thread. */
int stm_trace_get_thread_channel(int *channel);

/* Unregister the channel used by caller thread */
void stm_trace_free_thread_channel(void);

/* Output the buffer into a per-thread channel dynamically allocated:
 * - The first time a thread call it, a channel is requested and reserved
 *   for the calling thread, and the buffer is output in the allocated
 *   channel using the above API.
 *   The buffer is split into words of 8/4/2/1 byte(s) accroding to the size;
 *   the last output is timestamped
 * - The following calls performed by the same thread leads to some output
 *   on the same allocated channel
 * - Return 0 if ok, -1 if no channel is available.
 */
int stm_trace_buffer(size_t size, const char *buffer);

#ifdef __cplusplus
}
#endif

#endif /* __TRACE_H__ */
