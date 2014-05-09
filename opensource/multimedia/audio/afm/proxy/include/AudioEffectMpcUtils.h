/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AudioEffectMpcUtils.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIOEFFECT_MPC_UTILS_H_
#define _AUDIOEFFECT_MPC_UTILS_H_

#include "ENS_Nmf.h"

namespace AudioEffectMpcUtils {
    t_cm_memory_handle allocDSPMemoryForSetConfig(OMX_U32 domainId, OMX_U32 size, OMX_U32 *configStructArmAddress, OMX_U32 *configStructDspAddress);
}

#endif //_AUDIOEFFECT_MPC_UTILS_H_
