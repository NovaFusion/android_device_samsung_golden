/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_Utils.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_UTILS_H_
#define _AFM_UTILS_H_

#include "host/samplefreq.idt.h"
#include "OMX_Types.h"
#include "AFM_macros.h"

namespace AFM {
    AFM_API_IMPORT OMX_U32         OMX_SamplingRate(t_sample_freq eFreq);
    AFM_API_IMPORT t_sample_freq   sampleFreq(OMX_U32 nSamplingRate);
}

#endif //_AFM_UTILS_H_
