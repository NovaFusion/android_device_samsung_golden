/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */
/**
 * \file   AFM_Core.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_CORE_H_
#define _AFM_CORE_H_

#include "OMX_Core.h"

/** Extensions to the standard IL errors. */
typedef enum AFM_ERRORTYPE
{
    // Sent when the DSP panics (crash detected by CM).
    // pEventData is of type t_nmf_service_data*
    AFM_ErrorDspPanic = (OMX_S32) (OMX_ErrorVendorStartUnused + 0x00B00001),
} AFM_ERRORTYPE;

#endif
