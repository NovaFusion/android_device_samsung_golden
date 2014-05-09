/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_PcmProcessing.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_PCMPROCESSING_H_
#define _AFM_PCMPROCESSING_H_

#include "AFM_Component.h"

class AFM_PcmProcessing: public AFM_Component {
public:

    AFM_API_IMPORT virtual ~AFM_PcmProcessing() = 0;

    AFM_API_IMPORT virtual OMX_ERRORTYPE getParameter(
            OMX_INDEXTYPE nParamIndex,  
            OMX_PTR pComponentParameterStructure) const;

    virtual OMX_U32 getBlockSize() const { return DEFAULT_PCM_BLOCKSIZE;};
    virtual OMX_U32 getBlockDurationInMs() const { return DEFAULT_PCM_BLOCKDURATION;};
    virtual OMX_U32 getSampleBitSize() const { return 24;};

protected:
#ifndef BC_BREAK
	  AFM_API_IMPORT OMX_ERRORTYPE construct(
              const OMX_AUDIO_PARAM_PCMMODETYPE & defaultPcmSettingsIn,
              const OMX_AUDIO_PARAM_PCMMODETYPE & defaultPcmSettingsOut,
              OMX_U32 nRoleCount=1, bool ArmOnly=false);
#endif
	  AFM_API_IMPORT OMX_ERRORTYPE construct(OMX_U32 nRoleCount=1, bool ArmOnly=false);

};

#endif // _AFM_PCMPROCESSING_H_
