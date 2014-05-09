/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   ENS_Client.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _ENS_CLIENT_H_
#define _ENS_CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "OMX_Core.h"
#include "ENS_macros.h"

/// Trace levels
#define TRACE_LEVEL_NONE 0
#define TRACE_LEVEL_CLIENT 1
#define TRACE_LEVEL_COMPLETE 2

/** @defgroup ens_client_api ENS OSI Client APIs
 * This module presents the Client APIs of the ENS-OSI
 * */

/** The ENS_SetTraceCallback method is used to register a user callback
 *  function which will be used to output traces into a log file at
 *  execution time.
 *
 *  @param [in] log
 *      Function that writes a message, using a printf-like formatted
 *      string (only one parameter is supported per string)
 *  @return OMX_ERRORTYPE
 *      If the command successfully executes, the return code will be
 *      OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
ENS_API_IMPORT OMX_ERRORTYPE ENS_SetTraceCallback(void (*log)(const char *, OMX_U32));

/** The ENS_SetTraceLevel method is used to change the amount of trace output.
 *
 *  @param [in] nTraceLevel
 *      0: none          Trace output is disabled
 *      1: client part   Minimal trace necessary to know the network contents
 *      2: complete      Whole trace with internal calls for debugging purpose
 *  @return OMX_ERRORTYPE
 *      If the command successfully executes, the return code will be
 *      OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
ENS_API_IMPORT OMX_ERRORTYPE ENS_SetTraceLevel(OMX_U32 nTraceLevel);

/** The ENS_GetTraceLevel method is used to retrieve the current trace level.
 *
 *  @param [out] pTraceLevel
 *      0: none          Trace output is disabled
 *      1: client part   Minimal trace necessary to know the network contents
 *      2: complete      Whole trace with internal calls for debugging purpose
 *  @return OMX_ERRORTYPE
 *      If the command successfully executes, the return code will be
 *      OMX_NoError.  Otherwise the appropriate OMX error will be returned.
 */
ENS_API_IMPORT OMX_ERRORTYPE ENS_GetTraceLevel(OMX_U32 *pTraceLevel);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // _ENS_CLIENT_H_
