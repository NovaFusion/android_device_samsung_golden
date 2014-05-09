/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

/*
 * DeBuG Trace facilities under Android/Linux
 *
 * Author: <hugues.fruchet@stericsson.com>
 *
 *   **************
 *   ** features **
 *   **************
 *- several trace levels:
 *  - 4 bits are reserved for level selection (1 bit for each level)
 *  - unconditional : DBGT_ASSERT/DBGT_CRITICAL/DBGT_ERROR/DBGT_WARNING
 *    (automatic expansion of faulty function)
 *  - unconditional light informative trace : DBGT_PINFO
 *  - level 1: light traces (DBGT_PTRACE)
 *  - level 2: prolog/epilog of functions (DBGT_PROLOG/DBGT_EPILOG)
 *    (automatic expansion of short function name only, not prototype-)
 *  - level 3: verbose mode (DBGT_PDEBUG)
 *  - level 4: for future use
 *  - Examples (warning, must be given in hexa):
 *    - 0x1: light traces
 *    - 0x2: prolog/epilog
 *    - 0x4: verbose debug
 *    - 0x3: light trace+prolog/epilog
 *    - 0xF: all
 *
 *- several layers can be traced:
 *  - up to 8 layers
 *  - DBGT_LAYER to select layer to trace, typically 1 layer by sub-function
 *  - prolog/epilog will then be indented regading layer depth
 *  - Examples:
 *    - 0xF   will enable all traces in top layer
 *    - 0xF0  will enable all traces in top-1 layer
 *    - 0xF00 will enable all traces in top-2 layer
 *
 *- dynamic selection of item to trace & level:
 *  - on Android environment:
 *    - setprop mechanism; setprop <property> <level>
 *    - traces will be persistent through reboot if /data/local.prop file
 *      contain <property>=<level>
 *  - on LBP environment: export mechanism: export <property>=<level>
 *  - automatic expansion of prop trace name giving basic name
 *  - Android example:
 *    - giving name "camera" through DBGT_TRACE_INIT() macro
 *      will create trace name "debug.camera.trace" used through:
 *      setprop debug.camera.trace 0x600 (warning must be hexa)
 *      => this will set prolog/epilog+verbose traces in layer top - 2
 *    - adb shell "echo "debug.camera.trace=0x600" >> /data/local.prop"
 *      will enable this trace automatically after reboot
 *  - LBP example:
 *    - giving name "camera" through DBGT_TRACE_INIT() macro
 *      will create trace name "debug_camera_trace" used through:
 *      export debug_camera_trace=0x600
 *      => this will set prolog/epilog+verbose traces in layer top - 2
 *
 *- all traces could be discarded at compile time:
 *  => disable switch DBGT_CONFIG_DEBUG
 *
 *   ****************************
 *   ** How to instrument code **
 *   ****************************
 *  - makefile or top .h file:
 *    - define DBGT_CONFIG_DEBUG
 *    - define DBGT_CONFIG_AUTOVAR
 *    - define DBGT_TAG, the trace tag  (will be used as LOG_TAG in android)
 *    - define DBGT_PREFIX, a prefix which will be appended at begining of
 *      trace to identify a sub-layer.
 *  - in the module to trace .cpp entry file (typically where contructor is)
 *    - define DBGT_DECLARE_AUTOVAR, to declare the variable used to filter
 *    - in conctructor, call DBGT_TRACE_INIT(<module name>), examples:
 *      DBGT_TRACE_INIT(camera);     //expands debug.camera.trace
 *      DBGT_TRACE_INIT(postfilter); //expands debug.postfilter.trace
 *
 *  - in .cpp file to instrument with traces:
 *  int function MyFunc(int a, int b, u8* p)
 *  {
 *    DBGT_PROLOG("%d, %d", a, b);//expands "> MyFunc 3, 2" if level=0x2
 *
 *    //expands "!!! assertion ([p != NULL]) failed at function
 *    //MyFunc(int a, int b, u8* p), myfunc.cpp:45"
 *    //and will force exit of calling thread (with stackframe on Android)
 *    DBGT_ASSERT(p != NULL);
 *
 *    //expands "!!! assertion ([a > b] a=45, b=22) failed at function
 *    //MyFunc(int a, int b, u8* p), myfunc.cpp:50"
 *    //and will force exit of calling thread (with stackframe on Android)
 *    DBGT_ASSERT(a > b, "a=%d, b=%d", a, b);
 *
 *
 *
 *    DBGT_PTRACE("a+b=%d", a+b);//expands "a+b=5" if level =0x1
 *    DBGT_PDEBUG("a+b=%d", a+b);//expands "a+b=5" if level =0x4
 *
 *    if (a==3) {
 *      //expands"? function MyFunc(int a, int b, u8* p) a may be faulty (a=3)"
 *      //whatever level is
 *      DBGT_WARNING("a may be faulty (a=%d)", a);
 *    }
 *    if (b==2) {
 *      //expands "! function MyFunc(int a, int b, u8* p)
 *      //faulty value for b (b=2)" whatever level is
 *      DBGT_ERROR("faulty value for b (b=%d)", b);
 *      err=BAD_CONDITION;
 *      goto error;
 *    }
 *
 *  switch (c) {
 *     case 0:
 *       break;
 *     default:
 *       //expands "!! function MyFunc(int a, int b, u8* p) should
 *       //not be here myfunc.cpp:60"
 *       DBGT_CRITICAL("should not be here");
 *       err=INTERNAL_DBGT_ERROR;
 *       goto error;
 *  }
 *
 *
 *  DBGT_PINFO("[context]a=%d, b=%d c=%d, p=%p",
 *        a, b, c, p);//expands unconditionally (!STRONGLY RESTRICT USAGE!)
 *
 *  //expands "< MyFunc" if level=0x2 (don't print return value if no error)
 *  DBGT_EPILOG();
 *  return (0);
 *
 *  error:
 *    //expands "< MyFunc err=255(0xF)" if level=0x2
 *    DBGT_EPILOG("err=%d(%x)", err);
 *    return(err);
 *  }
 *
 *   ****************************
 *   ** How to verify **
 *   ****************************
 *  - setprop debug.camera.trace 0xFF
 *  - start component and check in logcat
 *  - adb logcat | grep "DBGT"
 *    [DBGT][debug.camera.trace] enabled with level 0xFF
 *  - logcat *:s <DBGT_TAG>:* => traces must be there
 *  - in case of problem track [DBGT] entries in logcat
 *
 */

#ifndef  DBGT_H
#define  DBGT_H


#include <stdlib.h> /*strtoul*/


/* +Keep compatibility with older version */
#ifdef CONFIG_DEBUG
  #define DBGT_CONFIG_DEBUG
#endif

#ifdef CONFIG_DEBUG
  #ifdef ANDROID
    #define DBGT_TRACE_NAME(a) "persist.debug."a".level"
  #else
    /* '.' are not valid in bash variables */
    #define DBGT_TRACE_NAME(a) "dbgt_"a"_level"
  #endif
#endif

#ifdef CONFIG_DEBUG_AUTOVAR
  #define DBGT_CONFIG_AUTOVAR
#endif

#ifdef DECLARE_AUTOVAR
  #define DBGT_DECLARE_AUTOVAR
#endif

#define GET_AND_SET_TRACE_LEVEL DBGT_TRACE_INIT
/* -Keep compatibility with older version */


#ifndef DBGT_TAG
  #define DBGT_TAG "DBGT"
#endif

#ifndef DBGT_PREFIX
  #define DBGT_PREFIX ""
#endif

#ifndef LOG_TAG
  #define LOG_TAG DBGT_TAG
#endif

#ifndef DBGT_LAYER
  #define DBGT_LAYER 0
#endif


/* LBP compatibility */
#include "linux_utils.h"

/**
 * DBGT core
 */

/* LOG_ALWAYS_FATAL_IF not in linux_utils */
#ifndef ANDROID
  #include <assert.h>
  #ifndef LOG_ALWAYS_FATAL_IF
  #define LOG_ALWAYS_FATAL_IF(condition, ...)\
    do {                                     \
      if (condition) {                       \
        ALOGE(__VA_ARGS__);                   \
        assert(!condition);                  \
      }                                      \
    } while (0)
  #endif
#endif /* ANDROID */


#define DBGT_INDENT_0 " "
#define DBGT_INDENT_1 "  "
#define DBGT_INDENT_2 "   "
#define DBGT_INDENT_3 "    "
#define DBGT_INDENT_4 "     "
#define DBGT_INDENT_5 "      "
#define DBGT_INDENT_6 "       "
#define DBGT_INDENT_(i) DBGT_INDENT_##i
#define DBGT_INDENT(i)  DBGT_INDENT_(i)

/* Waiting to find macro which expands class/prototype  */
#ifndef __PRETTY_FUNCTION__
  #define __PRETTY_FUNCTION__ __FUNCTION__
#endif

/* Unconditional, logged as assert, current process killed */
#define DBGT_ASSERT(condition, args...)                           \
  do {                                                            \
    if (!(condition)) {                                           \
      ALOGE("%s" DBGT_INDENT(DBGT_LAYER)"! "                       \
           "assertion !(" #condition ") ""failed at %s, %s:%d",   \
           DBGT_PREFIX, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
      LOG_ALWAYS_FATAL_IF(!(condition), ## args);                 \
    }                                                             \
  } while (0)


/* Unconditional, logged as error, function with signature, file, line */
#define DBGT_CRITICAL(fmt, args...)                             \
    ALOGE( "%s" DBGT_INDENT(DBGT_LAYER)"! %s "                   \
          fmt " %s:%d", DBGT_PREFIX, __PRETTY_FUNCTION__,       \
          ## args, __FILE__, __LINE__)

/* Unconditional, logged as error, function with signature only */
#define DBGT_ERROR(fmt, args...)                                \
    ALOGE( "%s" DBGT_INDENT(DBGT_LAYER)"! %s "                   \
         fmt, DBGT_PREFIX, __PRETTY_FUNCTION__, ## args)

/* Unconditional, logged as warning, function with signature only */
#define DBGT_WARNING(fmt, args...)                              \
    ALOGW( "%s" DBGT_INDENT(DBGT_LAYER)"? %s "                   \
         fmt, DBGT_PREFIX, __PRETTY_FUNCTION__, ## args)

#ifndef DBGT_CONFIG_DEBUG

#define DBGT_PINFO(...)
#define DBGT_PTRACE(...)
#define DBGT_PDEBUG(...)
#define DBGT_PROLOG(...)
#define DBGT_EPILOG(...)

#define DBGT_TRACE_INIT(name)                                \
    ALOGW("[DBGT] %s compiled without trace support"          \
         " (DBGT_CONFIG_DEBUG switch not enabled)", #name);

#else  /* #ifndef DBGT_CONFIG_DEBUG */
#ifdef DBGT_CONFIG_AUTOVAR
  #ifndef DBGT_VAR
    #define DBGT_VAR mDBGTvar
  #endif

  #ifndef DBGT_VAR_INIT
    #define DBGT_VAR_INIT 0
  #endif

  #ifdef DBGT_DECLARE_AUTOVAR
    int DBGT_VAR = DBGT_VAR_INIT;
  #else
    extern int DBGT_VAR;
  #endif
#else  /* #ifdef DBGT_CONFIG_AUTOVAR */
  #ifndef DBGT_VAR
    #error "[DBGT] DBGT_VAR needs to be defined when DBGT_CONFIG_AUTOVAR is not"
  #endif
#endif  /* #ifdef DBGT_CONFIG_AUTOVAR */

/* Unconditional, logged as info, short function only */
/* STRONGLY LIMIT OCCURENCE TO NOT INTRODUCE TRACE POLLUTION */
#define DBGT_PINFO(fmt, args...)                    \
    ALOGI( "%s" DBGT_INDENT(DBGT_LAYER) ". %s() "    \
      fmt, DBGT_PREFIX, __FUNCTION__, ## args)

/* All trace points below are conditional to trace level */
#define DBGT_PTRACE(fmt, args...)                       \
    do { if (DBGT_VAR & (0x1<<(DBGT_LAYER*4))) {        \
            ALOGI( "%s" DBGT_INDENT(DBGT_LAYER)          \
                  fmt, DBGT_PREFIX, ## args);           \
        } } while (0)

#define DBGT_PDEBUG(fmt, args...)                        \
    do { if (DBGT_VAR & (0x4<<(DBGT_LAYER*4))) {         \
            ALOGD( "%s" DBGT_INDENT(DBGT_LAYER)". "       \
                  fmt, DBGT_PREFIX, ## args);            \
        } } while (0)

#define DBGT_PROLOG(fmt, args...)                                       \
    do { if (DBGT_VAR & (0x2<<(DBGT_LAYER*4))) {                        \
            ALOGD( "%s" DBGT_INDENT(DBGT_LAYER)"> %s() "                 \
                  fmt, DBGT_PREFIX, __FUNCTION__, ## args);             \
        } } while (0)

#define DBGT_EPILOG(fmt, args...)                                       \
    do { if (DBGT_VAR & (0x2<<(DBGT_LAYER*4))) {                        \
            ALOGD( "%s" DBGT_INDENT(DBGT_LAYER)"< %s() "                 \
                  fmt, DBGT_PREFIX, __FUNCTION__, ## args);             \
        } } while (0)

#define STR(x) x

#ifndef DBGT_TRACE_NAME
#ifdef ANDROID
  #define DBGT_TRACE_NAME(a) "debug."a".trace"
#else
  /* '.' are not valid in bash variables */
  #define DBGT_TRACE_NAME(a) "debug_"a"_trace"
#endif
#endif

/* Warning, value to give to setprop is in hexadecimal! */
#define DBGT_TRACE_INIT(name)                                             \
    do {                                                                  \
        if (strlen(STR(DBGT_TRACE_NAME(#name))) > PROPERTY_KEY_MAX) {     \
            ALOGE("[DBGT]! Property key name [" STR(DBGT_TRACE_NAME(#name))\
                 "] exceed %d char length, will be cut to %d...",         \
                 PROPERTY_KEY_MAX, PROPERTY_KEY_MAX);                     \
        } else {                                                          \
            GET_PROPERTY(STR(DBGT_TRACE_NAME(#name)), value, "0");        \
            DBGT_VAR = strtoul(value, NULL, 16);                          \
            if (DBGT_VAR > 0) {                                           \
                ALOGI("[DBGT]["STR(DBGT_TRACE_NAME(#name))                 \
                     "] enabled with level 0x%x", DBGT_VAR);              \
            } else {                                                      \
                ALOGI("[DBGT]["STR(DBGT_TRACE_NAME(#name))                 \
                     "] disabled");                                       \
            }                                                             \
        }                                                                 \
    } while (0)

/* To allow update of trace level at run-time */
#define DBGT_SET_TRACE_LEVEL(level)                               \
    do {                                                          \
            DBGT_VAR = level;                                     \
            if (DBGT_VAR > 0) {                                   \
                ALOGI("[DBGT] trace set to level 0x%x", DBGT_VAR); \
            } else {                                              \
                ALOGI("[DBGT] trace set to 0, trace is disabled"); \
            }                                                     \
        }                                                         \
    } while (0)


#endif  /* #ifndef DBGT_CONFIG_DEBUG */


#endif  /* DBGT_H */
