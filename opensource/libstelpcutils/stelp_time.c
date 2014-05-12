/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */
#include <linux_utils.h>
#include <time.h>
#if defined(ANDROID)
int stelp_get_timeofday(char *buffer, size_t size)
{
    struct timeval tv;
    struct tm tm;
    size_t pos;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);

    pos = strftime(buffer, size, "%m-%d %H:%M:%S", &tm);
    return pos+snprintf(buffer + pos, size - pos > 0 ? size - pos : 0,
             ".%06d", (int) tv.tv_usec);
}

unsigned long long stelp_get_timestamp(void) {
	struct timespec ts;
	unsigned long long result;

	if(clock_gettime(CLOCK_MONOTONIC, &ts))
	{
	  return 0;
	}
	else
	{
	  result = (unsigned long long)ts.tv_sec * USEC_PER_SEC + (unsigned long long)(ts.tv_nsec/NSECS_PER_USEC);
	  return result;
	}
}

#else
int stelp_get_timeofday(char *buffer, size_t size)
{
    struct timeval tv;
    struct tm tm;
    size_t pos;

    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);

    pos = strftime(buffer, size, "%m-%d %H:%M:%S", &tm);
    return pos+snprintf(buffer + pos, size - pos > 0 ? size - pos : 0,
             ".%06d", (int) tv.tv_usec);
}
#endif
