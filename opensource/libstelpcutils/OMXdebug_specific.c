/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "OMX_debug.h"

#ifdef ANDROID
int OMX_debug_android_log_print
(int prio, const char *tag, char *buffer, size_t size, const char *fmt, ...)
{
    va_list ap;
    (void) buffer;
    (void) size;
    va_start(ap, fmt);
    int ret = __android_log_vprint(prio, tag, fmt, ap);
    va_end(ap);
    return ret;
}
#endif

int OMX_debug_sprintf
(int prio, const char *tag, char *buffer, size_t size, const char *fmt, ...)
{
    va_list ap;
    (void) prio;
    (void) tag;
    va_start(ap, fmt);
    int ret = vsnprintf(buffer, size, fmt, ap);
    va_end(ap);
    return ret;
}

char * strOMX_VERSIONTYPE(OMX_VERSIONTYPE *version, char *buffer, size_t size)
{
    if (version) {
        snprintf(buffer, size,"\n"
                 "              + nVersion     =0x%08x\n"
                 "              + nVersionMajor=%d\n"
                 "              + nVersionMinor=%d\n"
                 "              + nRevision    =%d\n"
                 "              + nStep        =%d",
                 (unsigned int) version->nVersion,
                 (int) version->s.nVersionMajor,
                 (int) version->s.nVersionMinor,
                 (int) version->s.nRevision,
                 (int) version->s.nStep);
    } else {
        snprintf(buffer, size, "\n"
                 "              + nVersion = NULL");
    }
    return buffer;
}

char * strOMX_UUIDTYPE(OMX_UUIDTYPE *pComponentUUID, char *buffer, size_t size)
{
    char bufferCUUID[128*2+5];
    int nb_char = 0;
    if(pComponentUUID) {
        bufferCUUID[0] = '\0';
        char * tmpi = bufferCUUID;
        int index = 0;
        while(index < 128) {
            if((index != 0) && (index%32 == 0))
                *(tmpi++) = '\0';
            sprintf(tmpi, "%02x", (unsigned int) pComponentUUID[index++]);
            tmpi += 2;
        }
        *(tmpi++) = '\0';

#define BUFFER buffer + nb_char, ( size - nb_char > 0 ? size - nb_char : 0)

        nb_char += snprintf(BUFFER, "\n");
        nb_char += snprintf(BUFFER, "              + ComponentUUID[ 0...31]=%s\n", bufferCUUID);
        nb_char += snprintf(BUFFER, "              + ComponentUUID[32...63]=%s\n", bufferCUUID+64+1);
        nb_char += snprintf(BUFFER, "              + ComponentUUID[64...95]=%s\n", bufferCUUID+128+2);
        nb_char += snprintf(BUFFER, "              + ComponentUUID[96..127]=%s\n", bufferCUUID+192+3);
    } else {
        snprintf(buffer, size, "\n              + ComponentUUID = NULL");
    }
    return buffer;
}

char * strOMX_EVENTHANDLER(OMX_EVENTTYPE eEvent,
                           OMX_U32 nData1,
                           OMX_U32 nData2,
                           OMX_PTR pEventData,
                           char *buffer, size_t size)
{
#define SEPARATOR_STR " : "
#define DESCR_BUFFER_SIZE 1024

    char event_str[128];
    char extra_str[128];
    char descr_str[DESCR_BUFFER_SIZE];

    switch(eEvent) {
    case OMX_EventCmdComplete:
        strOMX_COMMAND((OMX_COMMANDTYPE) nData1, nData2, descr_str, DESCR_BUFFER_SIZE);
    case OMX_EventError:
        extra_str[0] = ' ';
        extra_str[1] = '\0';
        break;
    case OMX_EventPortSettingsChanged:
    case OMX_EventBufferFlag:
        sprintf(extra_str, " - port %d", (int) nData1);
        break;
    default:
        extra_str[0] = '\0';
    }

    snprintf(buffer, size, "%s%s%s", strOMX_EVENTTYPE(eEvent, event_str, 128), extra_str,
             (eEvent == OMX_EventCmdComplete ? descr_str :
              (eEvent == OMX_EventError ?
               strOMX_ERRORTYPE((OMX_ERRORTYPE) nData1, descr_str, DESCR_BUFFER_SIZE) : "")));
    return buffer;
}

char * strOMX_TUNNELSETUPTYPE(OMX_TUNNELSETUPTYPE* pTunnelSetup, char *buffer, size_t size)
{
    char supplier_str[128];
    if(pTunnelSetup) {
        snprintf(buffer, size,
                "TunnelSetup.nTunnelFlags=0x%08x TunnelSetup.eSupplier=%s",
                (unsigned int) pTunnelSetup->nTunnelFlags,
                strOMX_BUFFERSUPPLIERTYPE(pTunnelSetup->eSupplier, supplier_str, 128));
    } else {
        snprintf(buffer, size, "TunnelSetup = NULL");
    }
    return buffer;
}

char * strOMX_BUFFERHEADERTYPE(OMX_BUFFERHEADERTYPE* pBuffer, char *buffer, size_t size)
{
    buffer[0] = '\0';
    if (pBuffer) {
        snprintf(buffer, size,
                 "     + pBuffer              : 0x%08x\n"
                 "     + nAllocLen            : 0x%08x\n"
                 "     + nFilledLen           : 0x%08x\n"
                 "     + nOffset              : 0x%08x\n"
                 "     + pAppPrivate          : 0x%08x\n"
                 "     + pPlatformPrivate     : 0x%08x\n"
                 "     + pInputPortPrivate    : 0x%08x\n"
                 "     + pOutputPortPrivate   : 0x%08x\n"
                 "     + hMarkTargetComponent : 0x%08x\n"
                 "     + pMarkData            : 0x%08x\n"
                 "     + nTickCount           : 0x%08x\n"
                 "     + nTimeStamp           : 0x%016llx\n"
                 "     + nFlags               : 0x%08x\n"
                 "     + nOutputPortIndex     : %d\n"
                 "     + nInputPortIndex      : %d\n",
                 (unsigned int) pBuffer->pBuffer,
                 (unsigned int) pBuffer->nAllocLen,
                 (unsigned int) pBuffer->nFilledLen,
                 (unsigned int) pBuffer->nOffset,
                 (unsigned int) pBuffer->pAppPrivate,
                 (unsigned int) pBuffer->pPlatformPrivate,
                 (unsigned int) pBuffer->pInputPortPrivate,
                 (unsigned int) pBuffer->pOutputPortPrivate,
                 (unsigned int) pBuffer->hMarkTargetComponent,
                 (unsigned int) pBuffer->pMarkData,
                 (unsigned int) pBuffer->nTickCount,
                 pBuffer->nTimeStamp,
                 (unsigned int) pBuffer->nFlags,
                 (int) pBuffer->nOutputPortIndex,
                 (int) pBuffer->nInputPortIndex);
    } else {
        snprintf(buffer, size, "     + NULL");
    }
    return buffer;
}

#define OMX_BUFFERDATA_STR_BYTE_FORMAT "%02x "
#define OMX_BUFFERDATA_STR_BYTE_LENGTH 3
#define OMX_BUFFERDATA_STR_SEPARATOR_FORMAT "\n [...]\n"
#define OMX_BUFFERDATA_STR_SEPARATOR_LENGTH 8

char * strOMX_BUFFERDATA(OMX_BUFFERHEADERTYPE* pBuffer,
                         char **buffer, size_t *size,
                         unsigned int nb_bytes_to_dump,
                         unsigned int nb_end_bytes,
                         unsigned int nb_bytes_per_lines)
{
    unsigned int i = 0;
    unsigned int n;
    OMX_U8* current = NULL;
    char* str = NULL;
    int dump_end = 1;
    unsigned int needed_buffer_size;

    if (nb_bytes_to_dump < 2*nb_end_bytes) {
        nb_bytes_to_dump = 2*nb_end_bytes;
    }

    needed_buffer_size = (nb_bytes_to_dump * OMX_BUFFERDATA_STR_BYTE_LENGTH) +
        OMX_BUFFERDATA_STR_SEPARATOR_LENGTH;

    if(*size < needed_buffer_size) {
        *size = needed_buffer_size;
        if(*buffer) {
            free(*buffer);
        }
        *buffer = (char *) malloc(needed_buffer_size);
    }

    if (pBuffer->nFilledLen == 0) {
        *buffer[0] = '\0';
        return *buffer;
    }

    n = nb_bytes_to_dump;

    if (pBuffer->nFilledLen <= n) {
        n = pBuffer->nFilledLen - 1;
        dump_end = 0;
    } else if (pBuffer->nFilledLen <= (n + nb_end_bytes)) {
        n = pBuffer->nFilledLen;
        dump_end = 0;
    } else {
        n -= nb_end_bytes;
        dump_end = 1;
    }

    current = (OMX_U8*)&(pBuffer->pBuffer[pBuffer->nOffset]);
    str = *buffer;

    unsigned int nb_bytes_for_line = 0;
    for (i = 0 ; i < n ; i++) {
        sprintf(str, OMX_BUFFERDATA_STR_BYTE_FORMAT, (*current));
        current++;
        str += OMX_BUFFERDATA_STR_BYTE_LENGTH;
        nb_bytes_for_line++;
        if (nb_bytes_for_line == nb_bytes_per_lines) {
            *(str-1) = '\n';
            nb_bytes_for_line = 0;
        }
    }

    if(dump_end) {
        *(str-1) = ' '; // Since BUFFERDATA_STR_SEPARATOR start with '\n' force space before
        sprintf(str, OMX_BUFFERDATA_STR_SEPARATOR_FORMAT);
        str += OMX_BUFFERDATA_STR_SEPARATOR_LENGTH;

        current = (OMX_U8*)&(pBuffer->pBuffer[pBuffer->nFilledLen]) - nb_end_bytes;
        for(i = 0 ; i < nb_end_bytes; i++) {
            sprintf(str, OMX_BUFFERDATA_STR_BYTE_FORMAT, (*current));
            current++;
            str += OMX_BUFFERDATA_STR_BYTE_LENGTH;
        }
    }
    return *buffer;
}

char * strOMX_COMMAND(OMX_COMMANDTYPE cmd, OMX_U32 param, char *buffer, size_t size)
{
    char arg_str[128];
    char command_str[128];

    arg_str[0] = '\0';

    switch(cmd) {
    case OMX_CommandFlush:
    case OMX_CommandPortDisable:
    case OMX_CommandPortEnable:
    case OMX_CommandMarkBuffer:
        sprintf(arg_str, "port %d", (int) param);
        break;
    default:
        // Nothing specific
        break;
    }
    snprintf(buffer, size, "%s - %s",
             strOMX_COMMANDTYPE(cmd, command_str, 128),
             (cmd == OMX_CommandStateSet ?
              strOMX_STATETYPE((OMX_STATETYPE) param, arg_str, 128) :
              arg_str));
    return buffer;
}
