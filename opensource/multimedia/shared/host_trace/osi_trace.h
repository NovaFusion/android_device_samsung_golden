/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

#ifndef __OSI_TRACE_H
#define __OSI_TRACE_H

#ifdef _MSC_VER
  #define snprintf _snprintf
  #define __func__ __FUNCTION__
  #define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifdef __SYMBIAN32__

  #define ASYNC_IN0  IN0
  #define ASYNC_OUT0  OUT0
  #include <nomadik_trace.h>
  #define DUMP_METHODS 1 // Show trace
  #define DUMP_THREAD_ID // Show thread number

  //New OSI trace API - stub for now
  #define DBGT_IN(...)
  #define DBGT_OUT(...)
  #define DBGT_ASYNC_IN(...)
  #define DBGT_ASYNC_OUT(...)
  #define DBGT_MSG(...)

  #define DBGT_PTRACE(...)
  #define DBGT_PDEBUG(...)
  #define DBGT_PROLOG(...)
  #define DBGT_EPILOG(...)

  #define DBGT_CRITICAL(...)
  #define DBGT_WARNING(...)

  #define GET_AND_SET_TRACE_LEVEL(name)

  #ifdef OMXCOMPONENT
    #define _CNAME_ OMXCOMPONENT
  #endif

  #ifndef USOURCE
    #define USOURCE KNMFENS // Category of trace
  #endif
  #include <stutrace.h>

#else  /* #ifdef __SYMBIAN32__ */

  #define LOG_TAG "OSI-Trace"

  #ifndef DBGT_PREFIX
    #ifdef OMXCOMPONENT
      #define DBGT_PREFIX OMXCOMPONENT
    #else
      #define DBGT_PREFIX ""
    #endif
  #endif

  /* Below system includes are for backward compatibility reasons:
     some modules using this header file does not include those system
     libraries but use them */
  #include <string.h>
  #include <stdlib.h>

  #include "debug_trace.h"

  //Compatibility
  #define ASYNC_IN0  DBGT_PROLOG
  #define ASYNC_OUT0 DBGT_EPILOG
  #define IN0  DBGT_PROLOG
  #define OUT0 DBGT_EPILOG
  #define MSG  DBGT_PTRACE
  #define MSG0 DBGT_PTRACE
  #define MSG1 DBGT_PTRACE
  #define MSG2 DBGT_PTRACE
  #define MSG3 DBGT_PTRACE
  #define MSG4 DBGT_PTRACE
  #define MSG9 DBGT_PTRACE
  #define OUTR(msg, ret) DBGT_EPILOG(" %d\n", ret)

  //New OSI trace API (powered with android debug support)

  //No restrictions on number of parameters
  #define DBGT_IN  DBGT_PROLOG
  #define DBGT_OUT DBGT_EPILOG
  #define DBGT_ASYNC_IN  DBGT_PROLOG
  #define DBGT_ASYNC_OUT SYNC_OUT0
  #define DBGT_MSG DBGT_PTRACE

#endif // __SYMBIAN32__

#endif  // __OSI_TRACE_H

