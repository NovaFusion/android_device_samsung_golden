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
 * \file   audio_sourcesink_chipset_api.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
/* ---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*
 * MODULE NAME                                                                *  
 *----------------------------------------------------------------------------*
 * Audio Source Sink Chipset API                                               *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * DETAILED DESCRIPTION                                                       * 
 * ---------------------------------------------------------------------------*
 * Audio Sources and Sinks chipset API header file. This file in addition 		*
 * with standard OpenMAX IL 1.1.1 header files constitute complete header 		*
 * files for Nokia Audio Source Sink Chipset API.												      *
 *                                                                            *
 * Copyright (c) 2008 Nokia                                                   *
 *                                                                            *
 *                                                                            *
 * Version History:                                                           *
 * ---------------------------------------------------------------------------*
 *   Date                Author                Description                    *
 *   ----                ------                -----------                    *
 * 17-NOV-2008        Vaibhav Goyal          Created the initial version.     *
 * 24-MAR-2009				Ashish Malot		Updates according to 0.3 version of doc *
 * 30-MAR-2009				Ashish Malot		Updates according to 1.0 version of doc *
 * 20-APR-2009        Vaibhav Goyal   Minor changes to compile the file       *
 * 18-JUN-2009				Ashish Malot		Corresponding to 1.2 version						*
 * 06-AUG-2009				Ashish Malot		Corresponding to 1.3 version						*
 *----------------------------------------------------------------------------*/

#ifndef AUDIO_SOURCESINK_CHIPSET_API_H
#define AUDIO_SOURCESINK_CHIPSET_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully 
 */

#include <OMX_Audio.h>

/* Including audio_hw_control_chipset_api_v0_1.h */
/*#include "audio_hw_control_chipset_api_v0_1.h"*/

typedef OMX_AUDIO_CONFIG_VOLUMETYPE OMX_AUDIO_CONFIG_SIDETONEATTENUATIONTYPE;
typedef OMX_AUDIO_CONFIG_VOLUMETYPE OMX_AUDIO_CONFIG_SIDETONEGAINTYPE;

/* HDMI related configuration structure */
typedef struct OMX_AUDIO_PARAM_HDMITYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nSubFrameSize;
    OMX_U32 nValidBitsPerSample;
    OMX_U32 nQFormat;
} OMX_AUDIO_PARAM_HDMITYPE;

/* Enumerations */
typedef enum OMX_TONEGENTONETYPE { 
    OMX_TONEGEN_NormalTone,  /**Normal Tone */ 
    OMX_TONEGEN_DtmfTone,    /**DTMF Tone */ 
  } OMX_TONEGENTONETYPE; 

    
typedef struct OMX_AUDIO_CONFIG_TONEGENERATORTYPE {
		OMX_U32 nSize;
		OMX_VERSIONTYPE nVersion;
		OMX_U32 nPortIndex;
		OMX_BOOL bEnable;
		OMX_TONEGENTONETYPE eTonetype;
		OMX_U32 nFrequency[2];
		OMX_U32 nDigit;
} OMX_AUDIO_CONFIG_TONEGENERATORTYPE;

typedef struct OMX_AUDIO_CONFIG_ALSADEVICETYPE {
		OMX_U32 nSize;
		OMX_VERSIONTYPE nVersion;
		OMX_U32 nPortIndex;
		OMX_BOOL bEnable;
		OMX_U8 cDeviceName[OMX_MAX_STRINGNAME_SIZE];
} OMX_AUDIO_CONFIG_ALSADEVICETYPE;

typedef struct OMX_AUDIO_CONFIG_LATENCYTYPE {
		OMX_U32 nSize;
		OMX_VERSIONTYPE nVersion;
		OMX_U32 nLatency;  /**Latency in ms */
} OMX_AUDIO_CONFIG_LATENCYTYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

