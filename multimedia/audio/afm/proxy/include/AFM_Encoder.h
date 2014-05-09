/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_Encoder.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIO_ENCODER_H_
#define _AUDIO_ENCODER_H_

#include "AFM_Component.h"

/// @defgroup afmenc AFM Encoder Components
/// @ingroup afmcomp
/// @{

/// Abstract base class for AFM encoder components
/// AFM encoder components have one input port with index 0
/// which accepts audio in PCM format , and one output port with index 1
/// which emits encoded audio.
class AFM_Encoder: public AFM_Component {
public:


	AFM_API_IMPORT virtual ~AFM_Encoder() = 0;

	AFM_API_IMPORT virtual OMX_ERRORTYPE getParameter(
		OMX_INDEXTYPE nParamIndex,
		OMX_PTR pComponentParameterStructure) const;

	AFM_API_IMPORT virtual int getSampleFrameSize() const = 0;
	AFM_API_IMPORT virtual int getMaxChannels() const = 0;
	AFM_API_IMPORT virtual int getChannels() const = 0;
	// Added to be able to send correct buffer size to wrapper
    AFM_API_IMPORT virtual int getMaxFrameSize() const = 0;
	virtual int getSampleBitSize() const {
		return 24;
	};


protected:
#ifndef BC_BREAK
	AFM_API_IMPORT OMX_ERRORTYPE  construct(
		OMX_AUDIO_CODINGTYPE eEncoding,
		OMX_U32 nFrameSize,
		const OMX_AUDIO_PARAM_PCMMODETYPE & defaultPcmSettings,
        OMX_U32 nRoleCount=1);
#endif

	AFM_API_IMPORT OMX_ERRORTYPE  construct(OMX_U32 nRoleCount=1);
};

/// @}

#endif // _AUDIO_ENCODER_H_
