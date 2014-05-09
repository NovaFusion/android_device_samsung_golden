/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _IFM_TRACE_H_
#define _IFM_TRACE_H_

//#define IMG_TIME_LOG_ENABLE
//#define IMG_TIME_LOG_USE_BTRACE

#ifdef IMG_TIME_LOG_ENABLE
#   ifdef IMG_TIME_LOG_USE_BTRACE
#       define IMG_TIME_VPB0                    1
#       define IMG_TIME_3A_DONE                 2
#       define IMG_TIME_3A_STATS_AF_REQUESTED   3
#       define IMG_TIME_3A_STATS_AEW_REQUESTED  4
#       define IMG_TIME_3A_STATS_AF_READY       5
#       define IMG_TIME_3A_STATS_AEW_READY      6
#       define IMG_TIME_3A_PROCESS_BEGIN        7
#       define IMG_TIME_3A_PROCESS_END          8
#       include <e32std.h>
#       include <e32btrace.h>
#       define IMG_TIME_LOG(_a_) BTraceContext4(BTrace::EPlatformSpecificFirst, 0, (_a_)) ;
#   else /* IMG_TIME_LOG_USE_BTRACE */
#       define IMG_TIME_VPB0                    "VPB0"
#       define IMG_TIME_3A_DONE                 "done"
#       define IMG_TIME_3A_STATS_AF_REQUESTED   "AF (?) stats requested"
#       define IMG_TIME_3A_STATS_AEW_REQUESTED  "AEW stats requested"
#       define IMG_TIME_3A_STATS_AF_READY       "AF stats ready"
#       define IMG_TIME_3A_STATS_AEW_READY      "AEW stats ready"
#       define IMG_TIME_3A_PROCESS_BEGIN        "process begin"
#       define IMG_TIME_3A_PROCESS_END          "process end"
#       include "omxilosalservices.h"
#       ifdef __SYMBIAN32__
#           define IMG_TIME_LOG(_a_) {  \
                OMX_U64 u64_time = omxilosalservices::OmxILOsalTimer::GetSystemTime(); \
                RDebug::Printf(" 3Atime %u %s", (unsigned long)(u64_time & 0xFFFFFFFF), (_a_)); \
            }
#       else /* __SYMBIAN32__ */
#           define IMG_TIME_LOG(_a_) {  \
                OMX_U64 u64_time = omxilosalservices::OmxILOsalTimer::GetSystemTime(); \
                printf(" 3Atime %u %s\n", (unsigned long)(u64_time & 0xFFFFFFFF), (_a_)); \
            }
#       endif /* __SYMBIAN32__ */
#   endif /* IMG_TIME_LOG_USE_BTRACE */
#else /* IMG_TIME_LOG_ENABLE */
#   define IMG_TIME_LOG(_a_)
#endif /* IMG_TIME_LOG_ENABLE */


#ifdef LATENCY_LOG_ENABLE
#   include "omxilosalservices.h"
#   ifdef __SYMBIAN32__
#      define LATENCY_TIME_LOG(_a_,_b_) RDebug::Printf("e_%s %d", (_a_), (_b_))
#   else /* __SYMBIAN32__ */
#      define LATENCY_TIME_LOG(_a_,_b_) {\
          OMX_U64 u64_time =  omxilosalservices::OmxILOsalTimer::GetSystemTime(); \
          OMX_U64 days = u64_time/24/60/60/1000000; \
          OMX_U64 h = (u64_time - days*24*60*60*1000000)/60/60/1000000; \
          OMX_U64 min = (u64_time - (days*24+h)*60*60*1000000)/60/1000000; \
          OMX_U64 s = (u64_time - ((days*24+h)*60+min)*60*1000000)/1000000; \
          OMX_U64 us = u64_time - (((days*24+h)*60+min)*60+s)*1000000; \
          printf("%02llu:%02llu:%02llu.%06llu e_%s %d\n", (h), (min), (s), (us), (_a_), (_b_)); \
          }
#   endif /* __SYMBIAN32__ */
#else /* LATENCY_LOG_ENABLE */
#   define LATENCY_TIME_LOG(_a_,_b_)
#endif /* LATENCY_LOG_ENABLE */

#endif /* _IFM_TRACE_H_ */
