/* ---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*
 * MODULE NAME                                                                *  
 *----------------------------------------------------------------------------*
 * Audio Render Processing Chipset API                                        *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * DETAILED DESCRIPTION                                                       * 
 * ---------------------------------------------------------------------------*
 * Audio render processing chipset API header file. This file in addition 		*
 * with standard OpenMAX IL 1.1.1 header files constitute complete header  		*
 * files for Nokia Audio render processing Chipset API.												*
 *                                                                            *
 * Copyright (c) 2007 Nokia                                                   *
 *                                                                            *
 *                                                                            *
 * Version History:                                                           *
 * ---------------------------------------------------------------------------*
 *   Date                Author                Description                    *
 *   ----                ------                -----------                    *
 * 17-NOV-2008        Ashish Malot    Created the initial version.						*
 * 24-MAR-2009				Ashish Malot		Updates according to 0.3 version of doc *
 * 30-MAR-2009				Ashish Malot		Updates according to 1.0 version of doc *
 * 20-APR-2009        Vaibhav Goyal   Minor changes to compile the file       *
 * 18-JUN-2008				Ashish Malot		Corresponding to 1.2 version						*
 *----------------------------------------------------------------------------*/

#ifndef AUDIO_RENDER_CHIPSET_API_H
#define AUDIO_RENDER_CHIPSET_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully 
 */

#include <OMX_Audio.h>

/* Volume ramp duration parameters */
typedef struct OMX_AUDIO_CONFIG_VOLUMERAMPTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;            
     OMX_U32 nChannel;
     OMX_BOOL bLinear;
     OMX_BS32 sStartVolume;
     OMX_BS32 sEndVolume;
     OMX_BU32 nRampDuration;
     OMX_BOOL bRampTerminate;
     OMX_BS32 sCurrentVolume;
     OMX_TICKS nRampCurrentTime;
 } OMX_AUDIO_CONFIG_VOLUMERAMPTYPE;
 
typedef struct OMX_AUDIO_CONFIG_PORTPAUSETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bIsPaused;
} OMX_AUDIO_CONFIG_PORTPAUSETYPE;

typedef struct OMX_AUDIO_PARAM_SYNCHRONIZEDTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bIsSynchronized;
} OMX_AUDIO_PARAM_SYNCHRONIZEDTYPE;


/* Positional 3D parameters */

typedef enum OMX_AUDIO_3DOUTPUTTYPE {
OMX_AUDIO_3DOutputHeadphones,	/*Positional 3D audio for headphones */
OMX_AUDIO_3DOutputLoudspeakers,	/*Positional 3D audio for two closely spaced loudspeakers */
OMX_AUDIO_3DOutputMax	/*Allowance for expansion in the number of positional 3D audio types */
} OMX_AUDIO_3DOUTPUTTYPE;

typedef enum OMX_AUDIO_ROLLOFFMODEL {
OMX_AUDIO_RollOffExponential,	/*Positional 3D audio for headphones */
OMX_AUDIO_RollOffLinear	/*Positional 3D audio for two closely spaced loudspeakers */
} OMX_AUDIO_ROLLOFFMODEL;

typedef struct OMX_AUDIO_CONFIG_3DOUTPUTTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_AUDIO_3DOUTPUTTYPE e3DOutputType;
} OMX_AUDIO_CONFIG_3DOUTPUTTYPE;

typedef struct OMX_AUDIO_CONFIG_3DLOCATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nX;
    OMX_S32 nY;
    OMX_S32 nZ;
} OMX_AUDIO_CONFIG_3DLOCATIONTYPE;

typedef struct OMX_AUDIO_PARAM_3DDOPPLERMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnabled;
} OMX_AUDIO_PARAM_3DDOPPLERMODETYPE;

typedef struct OMX_AUDIO_CONFIG_3DDOPPLERSETTINGSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nSoundSpeed;
    OMX_S32 nSourceVelocity;
    OMX_S32 nListenerVelocity;
 } OMX_AUDIO_CONFIG_3DDOPPLERSETTINGSTYPE;

typedef struct OMX_AUDIO_CONFIG_3DLEVELSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BS32 sDirectLevel;
    OMX_BS32 sRoomLevel;
} OMX_AUDIO_CONFIG_3DLEVELSTYPE;

typedef struct OMX_AUDIO_CONFIG_3DDISTANCEATTENUATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BS32 sMinDistance;
    OMX_BS32 sMaxDistance;
    OMX_BS32 sRollOffFactor;
    OMX_BS32 sRoomRollOffFactor;
    OMX_AUDIO_ROLLOFFMODEL eRollOffModel;
    OMX_BOOL bMuteAfterMax;
} OMX_AUDIO_CONFIG_3DDISTANCEATTENUATIONTYPE;

typedef struct OMX_AUDIO_CONFIG_3DDIRECTIVITYSETTINGSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BS32 sInnerAngle;
    OMX_BS32 sOuterAngle;
    OMX_BS32 sOuterLevel;
} OMX_AUDIO_CONFIG_3DDIRECTIVITYSETTINGSTYPE;

typedef struct OMX_AUDIO_CONFIG_3DDIRECTIVITYORIENTATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nXFront;
    OMX_S32 nYFront;
    OMX_S32 nZFront;
} OMX_AUDIO_CONFIG_3DDIRECTIVITYORIENTATIONTYPE;

typedef struct OMX_AUDIO_CONFIG_3DMACROSCOPICORIENTATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nXFront;
    OMX_S32 nYFront;
    OMX_S32 nZFront;
    OMX_S32 nXAbove;
    OMX_S32 nYAbove;
    OMX_S32 nZAbove;

} OMX_AUDIO_CONFIG_3DMACROSCOPICORIENTATIONTYPE;

typedef struct OMX_AUDIO_CONFIG_3DMACROSCOPICSIZETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nWidth;
    OMX_S32 nHeight;
    OMX_S32 nDepth;
} OMX_AUDIO_CONFIG_3DMACROSCOPICSIZETYPE;

typedef struct OMX_AUDIO_CONFIG_3DCOMMITMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bDeferred;
} OMX_AUDIO_CONFIG_3DCOMMITMODETYPE;

typedef struct OMX_AUDIO_CONFIG_3DCOMMITTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
} OMX_AUDIO_CONFIG_3DCOMMITTYPE;

/** End of positional 3D related **/


/* Audio virtualizer related parameters */
typedef enum OMX_AUDIO_VIRTUALIZERTYPE {
  OMX_AUDIO_VirtualizerLoudspeakers, /* Loudspeakers */
  OMX_AUDIO_VirtualizerHeadphones      /* Headphones */
} OMX_AUDIO_VIRTUALIZERTYPE;

typedef struct OMX_AUDIO_CONFIG_VIRTUALIZERTYPE {
OMX_U32 nSize;
OMX_VERSIONTYPE nVersion;
OMX_U32 nPortIndex;
OMX_BOOL bEnable;
OMX_AUDIO_VIRTUALIZERTYPE eVirtualizationType;
OMX_U32 nVirtualizationStrength;
 } OMX_AUDIO_CONFIG_VIRTUALIZERTYPE;

typedef struct OMX_AUDIO_CONFIG_VIRTUALIZERLOUDSPEAKERTYPE {
OMX_U32 nSize;
OMX_VERSIONTYPE nVersion;
OMX_U32 nPortIndex;
OMX_U32 nListeningAngle;
 } OMX_AUDIO_CONFIG_VIRTUALIZERLOUDSPEAKERTYPE;


/* End of audio virtualizer related params */

/* Audio visualizer related parameters */
typedef enum OMX_AUDIO_VISUALIZATIONTYPE {
  OMX_AUDIO_VisualizationPresetNone, 
  OMX_AUDIO_VisualizationPresetWaveform,
  OMX_AUDIO_VisualizationPresetSpectrum,
  OMX_AUDIO_VisualizationPresetWaveformAndSpectrum      
} OMX_AUDIO_VISUALIZATIONTYPE;

typedef struct OMX_AUDIO_CONFIG_VISUALIZATIONTYPE {
OMX_U32 nSize;
OMX_VERSIONTYPE nVersion;
OMX_U32 nPortIndex;
OMX_BOOL bEnable;
OMX_AUDIO_VISUALIZATIONTYPE ePreset;
OMX_BU32 sRate;
} OMX_AUDIO_CONFIG_VISUALIZATIONTYPE;

typedef struct OMX_AUDIO_CONFIG_VISUALIZATIONSPECTRALTYPE {
OMX_U32 nSize;
OMX_VERSIONTYPE nVersion;
OMX_U32 nPortIndex;
OMX_BU32 sBandIndex;
OMX_BU32 sCenterFreq;
OMX_U32 nType;
} OMX_AUDIO_CONFIG_VISUALIZATIONSPECTRALTYPE;

typedef struct OMX_AUDIO_CONFIG_VISUALIZATIONWAVEFORMTYPE {
OMX_U32 nSize;
OMX_VERSIONTYPE nVersion;
OMX_U32 nPortIndex;
OMX_BU32 sWaveformLength;
} OMX_AUDIO_CONFIG_VISUALIZATIONWAVEFORMTYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

