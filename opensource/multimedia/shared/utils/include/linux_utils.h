/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _LINUX_UTILS_H_
#define _LINUX_UTILS_H_

/** Abstract the API between Android and Linux environment for various usefull services */

/********************************************************************************
 * LOG services using Android API and providing an implementation for Linux:
 * LOG_TAG CPP macro must define the String to use
 */

#ifndef LOG_TAG
#define LOG_TAG "?"
#endif

#if defined(ENABLE_FEATURE_BUILD_HATS)

#include <stdio.h>
#include <stdlib.h>
#define LOGE(...) printf("E/" LOG_TAG ": " __VA_ARGS__)
#define LOGI(...) printf("I/" LOG_TAG ": " __VA_ARGS__)
#define LOGD(...) printf("D/" LOG_TAG ": " __VA_ARGS__)
#define LOGV(...) printf("V/" LOG_TAG ": " __VA_ARGS__)
#define LOGW(...) printf("W/" LOG_TAG ": " __VA_ARGS__)
#define ALOGE(...) printf("E/" LOG_TAG ": " __VA_ARGS__)
#define ALOGI(...) printf("I/" LOG_TAG ": " __VA_ARGS__)
#define ALOGD(...) printf("D/" LOG_TAG ": " __VA_ARGS__)
#define ALOGV(...) printf("V/" LOG_TAG ": " __VA_ARGS__)
#define ALOGW(...) printf("W/" LOG_TAG ": " __VA_ARGS__)

#elif defined(ANDROID) && !defined(ENABLE_FEATURE_BUILD_HATS)

#include <cutils/log.h>
#undef LOGE
#undef LOGI
#undef LOGD
#undef LOGW
#define SYSLOG_FILTERED 1
#define SYSLOG_UNFILTERED 0
#define LOGE(...) __DO_LOG(ANDROID_LOG_ERROR,   LOG_TAG,SYSLOG_UNFILTERED, __VA_ARGS__)
#define LOGI(...) __DO_LOG(ANDROID_LOG_INFO,    LOG_TAG,SYSLOG_UNFILTERED, __VA_ARGS__)
#define LOGD(...) __DO_LOG(ANDROID_LOG_DEBUG,   LOG_TAG,SYSLOG_UNFILTERED, __VA_ARGS__)
#define LOGW(...) __DO_LOG(ANDROID_LOG_WARN, LOG_TAG,SYSLOG_UNFILTERED, __VA_ARGS__)

#if LOG_NDEBUG
#define LOGV(...) ((void)0)
#else
#define LOGV(...) __DO_LOG(ANDROID_LOG_VERBOSE, LOG_TAG,SYSLOG_UNFILTERED, __VA_ARGS__)
#endif

#define __DO_LOG(prio,tag,filtered,fmt, args ...)  do { __do_stelp_syslog(prio,tag,filtered,fmt, ## args); } while (0)

#define STELP_FATAL(...) {\
    extern char* __progname;                                 \
    FILE* f=fopen("/data/ste-debug/stedump", "w");        \
    fprintf(f,"%s",__progname);                            \
    fclose(f);                                             \
    __DO_LOG(ANDROID_LOG_FATAL, LOG_TAG,SYSLOG_UNFILTERED, __VA_ARGS__);     \
    __do_stelp_print_backtrace();                          \
    __DO_LOG(ANDROID_LOG_FATAL, LOG_TAG,SYSLOG_UNFILTERED, "ENTERING FOREVER LOOP");     \
    while(1) {sleep(1);}                                    \
}

__BEGIN_DECLS
extern void __do_stelp_syslog(int priority, const char * tag,int is_filtered,const char *fmt, ...);
extern int stelp_syslog_dump_circularbuffer(void);
extern void __do_stelp_print_backtrace();
extern int stelp_get_timeofday(char *buffer, size_t size);
extern unsigned long long stelp_get_timestamp(void); // returns uptime in us on linux 
__END_DECLS

#else  /* !defined(ANDROID) && !defined(ENABLE_FEATURE_BUILD_HATS) */

#include <stdlib.h>
#ifndef LOG_NDEBUG
#define LOG_NDEBUG 1
#endif

#ifndef ANDROID_LOG_DEBUG
#define ANDROID_LOG_DEBUG 0
#endif

#define LOGE(...) __DO_LOG("E " LOG_TAG ": " __VA_ARGS__)
#define LOGI(...) __DO_LOG("I " LOG_TAG ": " __VA_ARGS__)
#define LOGD(...) __DO_LOG("D " LOG_TAG ": " __VA_ARGS__)
#define LOGW(...) __DO_LOG("W " LOG_TAG ": " __VA_ARGS__)

#define ALOGE(...) __DO_LOG("E" LOG_TAG ": " __VA_ARGS__)
#define ALOGI(...) __DO_LOG("I" LOG_TAG ": " __VA_ARGS__)
#define ALOGD(...) __DO_LOG("D" LOG_TAG ": " __VA_ARGS__)
#define ALOGV(...) __DO_LOG("V" LOG_TAG ": " __VA_ARGS__)
#define ALOGW(...) __DO_LOG("W" LOG_TAG ": " __VA_ARGS__)

#if LOG_NDEBUG
#define LOGV(...) ((void)0)
#else
#define LOGV(...) __DO_LOG("V " LOG_TAG ": " __VA_ARGS__)
#endif

#define __DO_LOG(fmt, args ...)  do { __do_stelp_log(fmt, ## args); } while (0)

#include <sys/cdefs.h>

__BEGIN_DECLS

extern void __do_stelp_log(const char *fmt, ...);
extern int stelp_get_timeofday(char *buffer, size_t size);
extern unsigned long long stelp_get_timestamp(void); // returns uptime in us on linux 
extern int stelp_get_timeofday(char *buffer, size_t size);

__END_DECLS

/* FIXME: kept for legacy reasons */
#include <string.h>

#endif

#ifndef USEC_PER_SEC
#define USEC_PER_SEC 1000000L
#endif
#ifndef NSECS_PER_USEC
#define NSECS_PER_USEC 1000ULL
#endif

#ifndef USEC_PER_MEC
#define USEC_PER_MEC 1000L
#endif



/********************************************************************************
 * property services using Android API and providing an implementation for Linux:
 * using getenv
 */

#ifdef ANDROID
#include <cutils/properties.h>
#else
#include <stdlib.h>
#endif

#ifndef PROPERTY_KEY_MAX
#define PROPERTY_KEY_MAX   32
#endif

#ifndef PROPERTY_VALUE_MAX
#define PROPERTY_VALUE_MAX  92
#endif

#ifdef ANDROID
#define GET_PROPERTY(key, value, default_value) \
    char value[PROPERTY_VALUE_MAX];		\
    property_get(key, value, default_value);
#else
#define GET_PROPERTY(key, value, default_value)			\
    char * value = getenv(key);					\
    if (value == NULL) {					\
	value = (char *) default_value;				\
    }
#endif

#ifdef ANDROID
#define SET_PROPERTY(key, value)				\
    property_set((const char *) key, (const char *) value);
#else
#define SET_PROPERTY(key, value)				\
    setenv((const char *) key, (const char *) value, 1);
#endif


#endif /* _LINUX_UTILS_H_ */


