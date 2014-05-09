/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 *
 * 
 *
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   audio_av_sync_chipset_api.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef AUDIO_AV_SYNC_CHIPSET_API_H
#define AUDIO_AV_SYNC_CHIPSET_API_H

/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully 
 */

#include <OMX_Audio.h>

/* AV sync related configuration structure */
typedef struct OMX_AUDIO_CONFIG_PROCESSEDDATAAMOUNT
{
	OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_TICKS nProcessedDataAmount;
} OMX_AUDIO_CONFIG_PROCESSEDDATAAMOUNT;

#endif
