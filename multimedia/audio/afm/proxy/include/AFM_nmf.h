/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_nmf.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_NMF_H_
#define _AFM_NMF_H_

#include <cm/inc/cm.hpp>

#include "host/memorybank.idt.h"
#include "AFM_macros.h"


namespace AFM {
 
    AFM_API_IMPORT OMX_ERRORTYPE connectPorts(
                t_cm_instance_handle hNmfOut, 
                const char * outSuffix,
                t_cm_instance_handle hNmfIn,
                const char * inSuffix,
                bool pcm);

    AFM_API_IMPORT OMX_ERRORTYPE disconnectPorts(
                t_cm_instance_handle hNmfOut, 
                const char * outSuffix,
                t_cm_instance_handle hNmfIn,
                const char * inSuffix,
                bool pcm);

    AFM_API_IMPORT t_cm_mpc_memory_type memoryBank(t_memory_bank bank);
}

#endif // _AFM_NMF_H_
