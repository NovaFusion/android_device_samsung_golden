/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_PcmProcessingInputPort.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_PCMPROCESSINGINPUTPORT_H_
#define _AFM_PCMPROCESSINGINPUTPORT_H_

#include "AFM_PcmPort.h"

class AFM_PcmProcessingInputPort: public AFM_PcmPort {
    public:

        AFM_PcmProcessingInputPort(
                OMX_U32 nIndex,
                OMX_U32 nBufferSizeMin,
                const OMX_AUDIO_PARAM_PCMMODETYPE &defaultPcmSettings,
                ENS_Component &enscomp);

        virtual OMX_ERRORTYPE setParameter(
                OMX_INDEXTYPE nParamIndex,
                OMX_PTR pComponentParameterStructure);
};

typedef AFM_PcmProcessingInputPort * AFM_PcmProcessingInputPort_p;

#endif // _AFM_PCMPROCESSINGINPUTPORT_H_
