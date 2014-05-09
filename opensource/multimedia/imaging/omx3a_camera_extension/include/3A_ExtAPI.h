/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _3A_EXTAPI_H_
#define _3A_EXTAPI_H_
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
  typedef OMX_ERRORTYPE (*StatsCallbackFunc_t)(void* userData,OMX_IN OMX_BUFFERHEADERTYPE* pBuffer); // seems fillBufferDone
  typedef void (*SetCallbackFunc_t)(void* handler, void* ud, StatsCallbackFunc_t func);
  typedef OMX_ERRORTYPE (*FillThisBufferFunc_t)(void* handler, OMX_BUFFERHEADERTYPE* pBuffer);
  typedef void (*FlushPortFunc_t)(void* handler, int nPortIndex);
typedef struct
{
  void * handler;
  SetCallbackFunc_t SetCallback;
  FillThisBufferFunc_t FillThisBuffer;
  FlushPortFunc_t FlushPort;
} OMX3A_ExtInterface_t;

OMX3A_ExtInterface_t* Get3AExtInterface();
#ifdef __cplusplus
}
#endif // __cplusplus
#endif //_3a_EXTAPI_H_
