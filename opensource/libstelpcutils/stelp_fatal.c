/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */
#if defined(ANDROID)
#define LOG_TAG "STE_SYSLOG_FATAL"
#include <linux_utils.h>
#include <unwind.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int idx=0; // stack depth index, protected by above mutex
static void __do_stelp_print_callstack_line(unsigned int addr)
{
    FILE* f=fopen("/proc/self/maps", "rb");
    if (f) {
        unsigned int start, end;
        char line[120];
        while (fgets(line, 120, f)) {
            sscanf(line, "%X-%X", &start, &end);
            if (addr >= start && addr <= end) {
                if (*line) {
                    line[strlen(line)-1] = 0; // chop \n
                    char* libname = line + strlen(line);
                    while (libname > line && *libname != ' ') libname--;
                    // Almost same format as debuggerd to re-use scripts..
                    LOGE("   (debug callstack) #%d  pc %x   %s\n", idx++,addr - start, libname);
                    fclose(f);
                    return;
                }
            }
        }
        fclose(f);
    }

    LOGE("(debug callstack) unresolved %X\n", addr);
}

static _Unwind_Reason_Code trace_function(_Unwind_Context *context, void *arg)
{
   _Unwind_Word ip = _Unwind_GetIP(context);
   if (ip) {
            __do_stelp_print_callstack_line((unsigned int) ip);
        }
   return _URC_OK;
}

void __do_stelp_print_backtrace()
{
    LOGE("Start of callstack dump for debug\n");
	pthread_mutex_lock(&mutex);
    idx=0;
    _Unwind_Backtrace(trace_function, 0);
	pthread_mutex_unlock(&mutex);

    LOGE("End of callstack dump for debug\n");
}
#endif


