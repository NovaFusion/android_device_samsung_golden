/*
  OMX_Symbian_AudioExt.h
  
  Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
  All rights reserved.

  This program and the accompanying materials are made available 
  under the terms of the Eclipse Public License v1.0 which accompanies 
  this distribution, and is available at 
  http://www.eclipse.org/legal/epl-v10.html

  Initial Contributors:
  Nokia Corporation - initial contribution.
*/

/** @file
@brief Symbian OpenMAX IL Extension Data Structures in the Audio Domain.

This file contains the extension structures for the Symbian IL extensions that
pertain to audio components.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_AudioExt_h
#define OMX_Symbian_AudioExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_Audio.h>
#include <OMX_Symbian_OtherExt_Ste.h>
#else
#include <OMX_Types.h>
#include <OMX_Audio.h>
#include <OMX_Symbian_OtherExt_Ste.h>
#endif

/** @name Generic Audio Control Definitions. */

/*@{*/

/**
 * Offset to Symbian extensions
 */
#define OMX_SYMBIAN_AUDIO_EXTENSIONS_START_OFFSET 0x00D00000

/**  Simple effect enable/disable switch control.

This struct can be used to control all effects that only require an on/off
type of control.

This data structure is currently used for:
- Earpiece Active Noise Control (EANC) settings with extension string 
  "OMX.Symbian.Index.Config.Audio.EANC"
- Algorithm for Preventing System Shutdown by Audio System (APSSAS) params
  with extension string "OMX.Symbian.Index.Param.Audio.APSSAS".
- Uplink algorithms configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.UplinkAlgorithms".
- Downlink algorithms configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.DownlinkAlgorithms".
- Automatic volume control configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.AutomaticVolumeControl".
- Comfort noise generation configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.ComfortNoiseGeneration".
- Multimicrophone noise control configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.MultiMicrophoneNoiseControl".
- Transducer dynamic range control (DRC) configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.TransducerDRC".
- Uplink level normalization configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.UplinkLevelNormalization".
- Voice clarity configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.VoiceClarity".
- Sidetone howling control configuration parameters with extension string
  "OMX.Symbian.Index.Config.Audio.SidetoneHowlingControl"
- Automatic gain control configuration parameters with extension string 
  "OMX.Symbian.Index.Config.Audio.AutomaticGainControl".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE { 
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< port that this structure applies to */
    OMX_BOOL bEnable;           /**< enable (OMX_TRUE) or disable (OMX_FALSE) the effect */
} OMX_SYMBIAN_AUDIO_CONFIG_EFFECTENABLETYPE;

/*@}*/

/** @name Audio Codecs Extensions */

/*@{*/

/** WMA params.

The OMX_AUDIO_PARAM_WMAAVGBYTESTYPE structure is used to set or query
the WMA codec related parameters. The setting can be changed using
the OMX_SetParam() function, and the current state can be queried using 
the OMX_GetParam() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Param.Audio.WMAAvgBytes".
*/
typedef struct OMX_SYMBIAN_AUDIO_PARAM_WMAAVGBYTESTYPE { 
    OMX_U32 nSize; 
    OMX_VERSIONTYPE nVersion; 
    OMX_U32 nPortIndex;  
    OMX_U32 nAvgBytesPerSec;
} OMX_SYMBIAN_AUDIO_PARAM_WMAAVGBYTESTYPE;

/** The version of the Windows Media Audio Pro codec */
typedef enum OMX_SYMBIAN_AUDIO_WMAPROFORMATTYPE {
    OMX_SYMBIAN_AUDIO_WMAPROFormatUnused,   /**< The version of the Windows Media audio codec is
                                               either not applicable or is unknown. */
    OMX_SYMBIAN_AUDIO_WMAPROFormat9,        /**< Windows Media Audio Pro version 9 */
    OMX_SYMBIAN_AUDIO_WMAPROFormat10,       /**< Windows Media Audio Pro version 10 */
    OMX_SYMBIAN_AUDIO_WMAPROFormatMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_WMAPROFORMATTYPE;

/** The profile of the Windows Media Audio Pro codec */
typedef enum OMX_SYMBIAN_AUDIO_WMAPROPROFILETYPE {
    OMX_SYMBIAN_AUDIO_WMAPROProfileUnused,  /**< The profile of the Windows Media Audio Pro codec is
                                               either not applicable or is unknown. */
    OMX_SYMBIAN_AUDIO_WMAPROProfileM0,      /**< Windows Media Audio Pro profile M0. */
    OMX_SYMBIAN_AUDIO_WMAPROProfileM1,      /**< Windows Media Audio Pro profile M1. */
    OMX_SYMBIAN_AUDIO_WMAPROProfileM2,      /**< Windows Media Audio Pro profile M2. */
    OMX_SYMBIAN_AUDIO_WMAPROProfileM3,      /**< Windows Media Audio Pro profile M3. */
    OMX_SYMBIAN_AUDIO_WMAPROProfileMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_WMAPROPROFILETYPE;

/** WMA Pro params.

The OMX_SYMBIAN_AUDIO_PARAM_WMAPROTYPE structure is used to set or query
the WMA pro codec related parameters. The setting can be changed using
the OMX_SetParam() function, and the current state can be queried using 
the OMX_GetParam() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Param.Audio.WMAPro".
*/
typedef struct OMX_SYMBIAN_AUDIO_PARAM_WMAPROTYPE {
    OMX_U32 nSize;                    /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;         /**< OMX specification version information */
    OMX_U32 nPortIndex;               /**< port that this structure applies to */
    OMX_U16 nChannels;                /**< Number of channels */
    OMX_U32 nBitRate;                 /**< Bit rate of the input data.  Use 0 for variable
                                           rate or unknown bit rates */
    OMX_SYMBIAN_AUDIO_WMAPROFORMATTYPE eFormat;     /**< The version of the Windows Media Audio Pro codec */
    OMX_SYMBIAN_AUDIO_WMAPROPROFILETYPE eProfile;   /**< The profile of the Windows Media Audio Pro codec */
    OMX_U32 nSamplingRate;            /**< Sampling rate of the source data. */
    OMX_U32 nBlockAlign;              /**< The block alignment, or block size (in bytes)
                                           of the audio codec */
    OMX_U32 nSuperBlockAlign;         /**< WMA Type specific data. It is used only by encoder 
                                               and is don’t care for decoder. */
    OMX_U32 nEncodeOptions;           /**< WMA Type specific opaque data */
    OMX_U32 nAdvancedEncodeOptions;   /**< WMA pro specific opaque data (don’t care for others) */
    OMX_U32 nAdvancedEncodeOptions2;  /**< WMA pro LBR specific opaque data (don’t care for others) */
    OMX_U32 nChannelMask;             /**< Output channel configuration; used only when down mixing
                                           is enabled. Channel mask for stereo down mix is 0x03. */
} OMX_SYMBIAN_AUDIO_PARAM_WMAPROTYPE;

/** AMR configs.

The OMX_SYMBIAN_AUDIO_CONFIG_AMRMODETYPE structure is used to set or query
the AMR codec related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.AMRMode".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_AMRMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_AUDIO_AMRBANDMODETYPE eAMRBandMode;
} OMX_SYMBIAN_AUDIO_CONFIG_AMRMODETYPE;

/** SBC configs.

The OMX_SYMBIAN_AUDIO_CONFIG_SBCBITPOOLTYPE structure is used to set or query
the SBC codec related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.SBCBitpool".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_SBCBITPOOLTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nNewBitPool; 
} OMX_SYMBIAN_AUDIO_CONFIG_SBCBITPOOLTYPE;

/** iLBC params.

The OMX_SYMBIAN_AUDIO_PARAM_ILBCTYPE structure is used to set or query
the iLBC codec related parameters. The setting can be changed using
the OMX_SetParam() function, and the current state can be queried using 
the OMX_GetParam() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Param.Audio.ILBC".
*/
typedef struct OMX_SYMBIAN_AUDIO_PARAM_ILBCTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;            
    OMX_U32 nFrameLength;
    OMX_BOOL bIsDTXCNGSupported;
    OMX_BOOL bDTXCNGSelected;                       
    OMX_BOOL bPLCSelected;                                                                       
} OMX_SYMBIAN_AUDIO_PARAM_ILBCTYPE;

/** iLBC configs.

The OMX_SYMBIAN_AUDIO_CONFIG_ILBCFRAMETYPE structure is used to set or query
the iLBC codec related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.ILBCFrame".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_ILBCFRAMETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nFrameLength; 
} OMX_SYMBIAN_AUDIO_CONFIG_ILBCFRAMETYPE;

/** G.711 params.

The OMX_SYMBIAN_AUDIO_PARAM_G711TYPE structure is used to set or query
the G.711 codec related parameters. The setting can be changed using
the OMX_SetParam() function, and the current state can be queried using 
the OMX_GetParam() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Param.Audio.G711".
*/
typedef struct OMX_SYMBIAN_AUDIO_PARAM_G711TYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex; 
    OMX_BOOL bIsDTXCNGSupported;
    OMX_BOOL bIsPLCSupported;       
    OMX_BOOL bDTXCNGEnabled;           
    OMX_BOOL bPLCEnabled;
    OMX_U32  nFrameLength;                                                                     
} OMX_SYMBIAN_AUDIO_PARAM_G711TYPE;

/*@}*/

/** @name Audio Capture Extensions */

/*@{*/

/*@}*/

/** @name Audio Rendering Extensions */

/*@{*/

/** Mixer port pause config.

The OMX_SYMBIAN_AUDIO_CONFIG_PORTPAUSETYPE structure is used to set or query
the mixer/splitter related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.PortPause".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_PORTPAUSETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bIsPaused;
} OMX_SYMBIAN_AUDIO_CONFIG_PORTPAUSETYPE;

/** Mixer port synchronized param.

The OMX_SYMBIAN_AUDIO_PARAM_SYNCHRONIZEDTYPE structure is used to set or query
the mixer/splitter related parameters. The setting can be changed using
the OMX_SetParam() function, and the current state can be queried using 
the OMX_GetParam() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Param.Audio.Synchronized".
*/
typedef struct OMX_SYMBIAN_AUDIO_PARAM_SYNCHRONIZEDTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bIsSynchronized;
} OMX_SYMBIAN_AUDIO_PARAM_SYNCHRONIZEDTYPE;

/** Output transducer setting for 3D. */
typedef enum OMX_SYMBIAN_AUDIO_3DOUTPUTTYPE {
    OMX_SYMBIAN_AUDIO_3DOutputHeadphones,	    /**< Positional 3D audio for headphones */
    OMX_SYMBIAN_AUDIO_3DOutputLoudspeakers,   /**< Positional 3D audio for two closely spaced loudspeakers */
    OMX_SYMBIAN_AUDIO_3DOutputMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_3DOUTPUTTYPE;

/** Output transducer setting for 3D.

The OMX_SYMBIAN_AUDIO_CONFIG_3DOUTPUTTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DOutput".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DOUTPUTTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_SYMBIAN_AUDIO_3DOUTPUTTYPE e3DOutputType;
} OMX_SYMBIAN_AUDIO_CONFIG_3DOUTPUTTYPE;

/** 3D location setting.

The OMX_SYMBIAN_AUDIO_CONFIG_3DLOCATIONTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DLocation".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DLOCATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nX;
    OMX_S32 nY;
    OMX_S32 nZ;
} OMX_SYMBIAN_AUDIO_CONFIG_3DLOCATIONTYPE;

/** Doppler effect mode for 3D.

The OMX_SYMBIAN_AUDIO_PARAM_3DDOPPLERMODETYPE structure is used to set or query
the 3D mixer related parameters. The setting can be changed using
the OMX_SetParam() function, and the current state can be queried using 
the OMX_GetParam() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Param.Audio.3DDopplerMode".
*/
typedef struct OMX_SYMBIAN_AUDIO_PARAM_3DDOPPLERMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnabled;
} OMX_SYMBIAN_AUDIO_PARAM_3DDOPPLERMODETYPE;

/** Doppler effect settings.

The OMX_SYMBIAN_AUDIO_CONFIG_3DDOPPLERSETTINGSTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DDopplerSettings".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DDOPPLERSETTINGSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nSoundSpeed;
    OMX_S32 nSourceVelocity;
    OMX_S32 nListenerVelocity;
} OMX_SYMBIAN_AUDIO_CONFIG_3DDOPPLERSETTINGSTYPE;

/** 3D mixer level settings.

The OMX_SYMBIAN_AUDIO_CONFIG_3DLEVELSTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DLevels".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DLEVELSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BS32 sDirectLevel;
    OMX_BS32 sRoomLevel;
} OMX_SYMBIAN_AUDIO_CONFIG_3DLEVELSTYPE;

/** Distance attenuation roll-off model setting for 3D. */
typedef enum OMX_SYMBIAN_AUDIO_ROLLOFFMODELTYPE {
    OMX_SYMBIAN_AUDIO_RollOffExponential,
    OMX_SYMBIAN_AUDIO_RollOffLinear,
    OMX_SYMBIAN_AUDIO_RollOffMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_ROLLOFFMODELTYPE;

/** 3D mixer distance attenuation.

The OMX_SYMBIAN_AUDIO_CONFIG_3DDISTANCEATTENUATIONTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DDistanceAttenuation".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DDISTANCEATTENUATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BS32 sMinDistance;
    OMX_BS32 sMaxDistance;
    OMX_BS32 sRollOffFactor;
    OMX_BS32 sRoomRollOffFactor;
    OMX_SYMBIAN_AUDIO_ROLLOFFMODELTYPE eRollOffModel;
    OMX_BOOL bMuteAfterMax;
} OMX_SYMBIAN_AUDIO_CONFIG_3DDISTANCEATTENUATIONTYPE;

/** 3D mixer directivity settings.

The OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYSETTINGSTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DDirectivitySettings".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYSETTINGSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BS32 sInnerAngle;
    OMX_BS32 sOuterAngle;
    OMX_BS32 sOuterLevel;
} OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYSETTINGSTYPE;

/** 3D mixer orientation setting for the directivity effect.

The OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYORIENTATIONTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DDirectivityOrientation".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYORIENTATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nXFront;
    OMX_S32 nYFront;
    OMX_S32 nZFront;
} OMX_SYMBIAN_AUDIO_CONFIG_3DDIRECTIVITYORIENTATIONTYPE;

/** 3D mixer orientation setting for the macroscopic effect.

The OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICORIENTATIONTYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DMacroscopicOrientation".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICORIENTATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nXFront;
    OMX_S32 nYFront;
    OMX_S32 nZFront;
    OMX_S32 nXAbove;
    OMX_S32 nYAbove;
    OMX_S32 nZAbove;
} OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICORIENTATIONTYPE;

/** 3D mixer macroscopic effect size setting.

The OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICSIZETYPE structure is used to set or query
the 3D mixer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.3DMacroscopicSize".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICSIZETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 nWidth;
    OMX_S32 nHeight;
    OMX_S32 nDepth;
} OMX_SYMBIAN_AUDIO_CONFIG_3DMACROSCOPICSIZETYPE;



/** Audio virtualizer transducer type */
typedef enum OMX_SYMBIAN_AUDIO_VIRTUALIZERTYPE {
    OMX_SYMBIAN_AUDIO_VirtualizerLoudspeakers,    /**< Loudspeakers */
    OMX_SYMBIAN_AUDIO_VirtualizerHeadphones,      /**< Headphones */
    OMX_SYMBIAN_AUDIO_VirtualizerMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_VIRTUALIZERTYPE;

/** Virtualizer mode setting.

The OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERTYPE structure is used to set or query
the Virtualizer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.Virtualizer".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_SYMBIAN_AUDIO_VIRTUALIZERTYPE eVirtualizationType;
    OMX_U32 nVirtualizationStrength;
} OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERTYPE;

/** Virtualizer loudspeaker angle setting.

The OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERLOUDSPEAKERTYPE structure is used to set or query
the Virtualizer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.VirtualizerLoudspeaker".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERLOUDSPEAKERTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nListeningAngle;
} OMX_SYMBIAN_AUDIO_CONFIG_VIRTUALIZERLOUDSPEAKERTYPE;

/** Audio visualizer mode type */
typedef enum OMX_SYMBIAN_AUDIO_VISUALIZATIONTYPE {
    OMX_SYMBIAN_AUDIO_VisualizationPresetNone,
    OMX_SYMBIAN_AUDIO_VisualizationPresetWaveform,
    OMX_SYMBIAN_AUDIO_VisualizationPresetSpectrum,
    OMX_SYMBIAN_AUDIO_VisualizationPresetWaveformAndSpectrum,
    OMX_SYMBIAN_AUDIO_VisualizationPresetMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_VISUALIZATIONTYPE;

/** Audio visualizer mode setting.

The OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONTYPE structure is used to set or query
the Audio visualizer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.Visualization".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_SYMBIAN_AUDIO_VISUALIZATIONTYPE ePreset;
    OMX_BU32 sRate;
} OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONTYPE;

/** Audio visualizer settings for the spectral mode.

The OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONSPECTRALTYPE structure is used to set or query
the Audio visualizer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.VisualizationSpectral".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONSPECTRALTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BU32 sBandIndex;
    OMX_BU32 sCenterFreq;
    OMX_U32 nType;
} OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONSPECTRALTYPE;

/** Audio visualizer settings for the waveform mode.

The OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONWAVEFORMTYPE structure is used to set or query
the Audio visualizer related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.VisualizationWaveform".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONWAVEFORMTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BU32 sWaveformLength;
} OMX_SYMBIAN_AUDIO_CONFIG_VISUALIZATIONWAVEFORMTYPE;

/*@}*/

/** @name Audio Transducer Processing Extensions */

/*@{*/

/** Equalizer filter type */
typedef enum OMX_SYMBIAN_AUDIO_FILTERTYPE {
    OMX_SYMBIAN_AUDIO_FilterNotDefined,   
    OMX_SYMBIAN_AUDIO_FilterPeaking,	
    OMX_SYMBIAN_AUDIO_FilterLowpass,	
    OMX_SYMBIAN_AUDIO_FilterHighpass,
    OMX_SYMBIAN_AUDIO_FilterLowshelf,	
    OMX_SYMBIAN_AUDIO_FilterHighshelf,
    OMX_SYMBIAN_AUDIO_FilterMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_FILTERTYPE;

/** IIR Transducer EQ settings.

The OMX_SYMBIAN_AUDIO_CONFIG_IIREQUALIZERTYPE structure is used to set or query
the Transducer EQ related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.IIREqualizer".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_IIREQTYPE  {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;  
    OMX_U32 nPortIndex;
    OMX_U32 nChannel;
    OMX_BU32 sBandIndex; 
    OMX_SYMBIAN_AUDIO_FILTERTYPE eFilterType;
    OMX_U32 nFilterCenterCutoffFrequency;
    OMX_S32 nFilterGain;
    OMX_U32 nFilterBandwidth; 
} OMX_SYMBIAN_AUDIO_CONFIG_IIREQTYPE;


/** The maximum number of frequency response points for the FIR EQ */
#define OMX_SYMBIAN_AUDIO_MAX_FIR_RESPONSE_POINTS 100

/** FIR Transducer EQ configs.

The OMX_SYMBIAN_AUDIO_CONFIG_FIRRESPONSETYPE structure is used to set or query
the Transducer EQ related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.FIRResponse".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_FIRRESPONSETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;  
    OMX_U32 nPortIndex;
    OMX_U32 nChannel;
    OMX_U32 nFrequencyPoints; 
    OMX_S32 nFrequencies[OMX_SYMBIAN_AUDIO_MAX_FIR_RESPONSE_POINTS];
    OMX_S32 nGains[OMX_SYMBIAN_AUDIO_MAX_FIR_RESPONSE_POINTS];
    OMX_U32 nFirLength;
} OMX_SYMBIAN_AUDIO_CONFIG_FIRRESPONSETYPE;

/** Phase response enum */
typedef enum OMX_SYMBIAN_AUDIO_PHASERESPONSETYPE {
    OMX_SYMBIAN_AUDIO_PhaseNotDefined,   
    OMX_SYMBIAN_AUDIO_LinearPhase,	
    OMX_SYMBIAN_AUDIO_MinimumPhase,
} OMX_SYMBIAN_AUDIO_PHASERESPONSETYPE;

/** Transducer EQ main settings.

The OMX_SYMBIAN_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE structure is used to set or query
the Transducer EQ related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.TransducerEqualizer".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;            
    OMX_BOOL bEnableFir;
    OMX_BOOL bEnableIir;
    OMX_U32 nChannel;
    OMX_SYMBIAN_AUDIO_PHASERESPONSETYPE ePhaseResponseTargetFir;
    OMX_SYMBIAN_AUDIO_PHASERESPONSETYPE ePhaseResponseTargetIir;
    OMX_U32 nIIREqualizerBlocks;
    OMX_U32 nIIREqualizerGain;
} OMX_SYMBIAN_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE;

/** Multi-band DRC settings.

The OMX_SYMBIAN_AUDIO_CONFIG_MDRCTYPE structure is used to set or query
the MDRC related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.MDRC".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_MDRCTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nChannel;	
    OMX_BOOL bEnable;
    OMX_BU32 sBandIndex;
    OMX_BU32 sBandRange;
    OMX_BS32 sBandLevel;
    OMX_BU32 sKneePoints;	
    OMX_S32 nDynamicResponse[10][2];
    OMX_BU32 sAttackTime; 
    OMX_BU32 sReleaseTime;	    
} OMX_SYMBIAN_AUDIO_CONFIG_MDRCTYPE;

/** Transducer protection settings.

The OMX_SYMBIAN_AUDIO_CONFIG_LSPROTECTIONTYPE structure is used to set or query
the Transducer protection related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.LSProtection".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_LSPROTECTIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_AUDIO_CHANNELTYPE eChannel;
    OMX_BOOL bEnable;
} OMX_SYMBIAN_AUDIO_CONFIG_LSPROTECTIONTYPE;

/** Transducer SPL limiting mode*/
typedef enum OMX_SYMBIAN_AUDIO_SPLLIMITTYPE {
    OMX_SYMBIAN_AUDIO_SPLLimitation,	
    OMX_SYMBIAN_AUDIO_SPLNormalization,	
    OMX_SYMBIAN_AUDIO_SPLMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_SPLLIMITTYPE;

/** SPL Limiter configs.

The OMX_SYMBIAN_AUDIO_CONFIG_SPLLIMITTYPE structure is used to set or query
the SPL Limiter related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.SPLLimit".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_LIMITTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_S32 nSplLimit;
    OMX_S32 nGain;	    
} OMX_SYMBIAN_AUDIO_CONFIG_LIMITTYPE;

/** Click Removal settings.

The OMX_SYMBIAN_AUDIO_CONFIG_CLICKREMOVALTYPE structure is used to set or query
the Click Removal related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.ClickRemoval".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_CLICKREMOVALTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_BU32 sClickControl;
} OMX_SYMBIAN_AUDIO_CONFIG_CLICKREMOVALTYPE;

/** APSSAS configs.

The OMX_SYMBIAN_AUDIO_CONFIG_BATTERYVOLTAGETYPE structure is used to set or query
the APSSAS related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.BatteryVoltage".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_BATTERYVOLTAGETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nBattVol;
} OMX_SYMBIAN_AUDIO_CONFIG_BATTERYVOLTAGETYPE;

/*@}*/

/** HDMI related configuration structure. */
typedef struct OMX_SYMBIAN_AUDIO_PARAM_HDMITYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nSubFrameSize;
    OMX_U32 nVaildBitsPerSample;
    OMX_U32 nQFormat;
} OMX_SYMBIAN_AUDIO_PARAM_HDMITYPE;

/** Audio Physical Transport Type

Sinks and sources that utilize a certain digital transport layer such as USB, Bluetooth or HDMI
require that the format of the audio data to be transfered over the digital bus is also configured.

The index specified for this structure is retrieved using
OMX_GetExtensionIndex() with the extension string
"OMX.Symbian.Index.Config.Audio.PhysicalTransportType"
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_PHYSICALTRANSPORTTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U16 nBitsPerSample;
} OMX_SYMBIAN_AUDIO_CONFIG_PHYSICALTRANSPORTTYPE;

/** Audio tracing data type.

Audio tracing data type is used for reading algorithm variables via the OpenMAX IL 
component API. The audio variables form an opaque data block, which only the receiver
and the algorithm understand.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.TracingData".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_TRACINGDATATYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nChannel;
    OMX_U8 nData[1];
} OMX_SYMBIAN_AUDIO_CONFIG_TRACINGDATATYPE;

/** Tone generation mode enum. */
typedef enum OMX_SYMBIAN_AUDIO_TONEGENTONETYPE { 
    OMX_SYMBIAN_AUDIO_TonegenNormal,  /**< Normal Tone */ 
    OMX_SYMBIAN_AUDIO_TonegenDtmf,    /**< DTMF Tone */ 
    OMX_SYMBIAN_AUDIO_TonegenMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_TONEGENTONETYPE; 

/** Tone generator configs.

The OMX_SYMBIAN_AUDIO_CONFIG_TONEGENERATORTYPE structure is used to set or query
the Tone generator related configurations. The setting can be changed using
the OMX_SetConfig() function, and the current state can be queried using 
the OMX_GetConfig() function. When calling either function, the index 
specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.ToneGenerator".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_TONEGENERATORTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_SYMBIAN_AUDIO_TONEGENTONETYPE eTonetype;
    OMX_U32 nFrequency[2];
    OMX_U32 nDigit;
} OMX_SYMBIAN_AUDIO_CONFIG_TONEGENERATORTYPE;

/*@}*/

/** @name Audio Telephony Extensions */

/*@{*/

/** Speech decoder status information.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.DecoderStatus".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_DECODERSTATUSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion ;
    OMX_AUDIO_CODINGTYPE nCoding;
    OMX_U32 nBitrate;
} OMX_SYMBIAN_AUDIO_CONFIG_DECODERSTATUSTYPE;

/** Speech encoder status information.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.EncoderStatus".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_ENCODERSTATUSTYPE { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion ;
    OMX_AUDIO_CODINGTYPE nCoding;
    OMX_U32 nBitrate;
    OMX_BOOL bDtx;
    OMX_BOOL bAudioActivityControl;
    OMX_BOOL bNsync;
} OMX_SYMBIAN_AUDIO_CONFIG_ENCODERSTATUSTYPE;

/** Uplink timing control.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.UplinkTiming".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_UPLINKTIMINGTYPE { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nDeliveryTime;
    OMX_U32 nModemProcessingTime;
} OMX_SYMBIAN_AUDIO_CONFIG_UPLINKTIMINGTYPE;

/** RF-TX Power information.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.RFTxPower".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_RFTXPOWERTYPE { 
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPower;
    OMX_U32 nTime;
} OMX_SYMBIAN_AUDIO_CONFIG_RFTXPOWERTYPE;

/** Uplink timing information.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.UplinkTimingInfo".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_UPLINKTIMINGINFOTYPE { 
     OMX_U32 nSize;
     OMX_VERSIONTYPE nVersion;
     OMX_S32 nMaxTime;
     OMX_S32 nAverageTime;
     OMX_S32 nMinTime;
} OMX_SYMBIAN_AUDIO_CONFIG_UPLINKTIMINGINFOTYPE;

/** Uplink delay information.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.UplinkDelayInfo".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_UPLINKDELAYINFOTYPE { 
     OMX_U32 nSize;
     OMX_VERSIONTYPE nVersion;
     OMX_S32 nMaxDelay;
     OMX_S32 nAverageDelay;
     OMX_S32 nMinDelay;
} OMX_SYMBIAN_AUDIO_CONFIG_UPLINKDELAYINFOTYPE;

/** Downlink frame information.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.DownlinkFrameInfo".
*/
 typedef struct OMX_SYMBIAN_AUDIO_CONFIG_DOWNLINKFRAMEINFOTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion ;
     OMX_U32 nBadFrameCounter;
     OMX_U32 nSilenceFrameCounter;
} OMX_SYMBIAN_AUDIO_CONFIG_DOWNLINKFRAMEINFOTYPE;

/** Acoustic echo control configuration parameters.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.AcousticEchoControl".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_ACOUSTICECHOCONTROLTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_S32 sEchoGain;
    OMX_U32 nEchoMinDelay;
    OMX_U32 nEchoMaxDelay;
} OMX_SYMBIAN_AUDIO_CONFIG_ACOUSTICECHOCONTROLTYPE;

/** Artificial bandwidth expansion configuration parameters.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.BandwidthExpansion".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_BANDWIDTHEXPANSIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_BU32 nStrength;
} OMX_SYMBIAN_AUDIO_CONFIG_BANDWIDTHEXPANSIONTYPE;

/**  Background noise control configuration parameters.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.BackgroundNoiseControl".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_BACKGROUNDNOISECONTROLTYPE { 
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;            
    OMX_BOOL bEnable;
    OMX_BU32 nAttenuation;
} OMX_SYMBIAN_AUDIO_CONFIG_BACKGROUNDNOISECONTROLTYPE;

/** Downlink noise control configuration parameters.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.DownLinkNoiseControl".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_DOWNLINKNOISECONTROLTYPE { 
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;            
    OMX_BOOL bEnable;
    OMX_BU32 nStrength;
} OMX_SYMBIAN_AUDIO_CONFIG_DOWNLINKNOISECONTROLTYPE;

/** Wind noise control configuration parameters.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.WindNoiseControl".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_WINDNOISECONTROLTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnable;
    OMX_BU32 nSensitivity;
} OMX_SYMBIAN_AUDIO_CONFIG_WINDNOISECONTROLTYPE;

/** 
 * Extradata extensions
 */
typedef enum OMX_SYMBIAN_AUDIO_EXTRADATATYPE {
    OMX_SYMBIAN_AudioExtraDataDlSpeechInfo = OMX_ExtraDataVendorStartUnused + OMX_SYMBIAN_AUDIO_EXTENSIONS_START_OFFSET,
    OMX_SYMBIAN_AudioExtraDataTimingDiff,
    OMX_SYMBIAN_AudioExtraDataVadInfo,
    OMX_SYMBIAN_AudioExtraDataMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_EXTRADATATYPE;

/** Downlink speech information extra data type. */
typedef struct OMX_SYMBIAN_AUDIO_EXTRADATA_DLSPEECHINFOTYPE {
    OMX_SYMBIAN_OTHER_EXTRADATABASETYPE base;
    OMX_BOOL bComfortNoiseFrame;
    OMX_BOOL bCorruptedFrame;
} OMX_SYMBIAN_AUDIO_EXTRADATA_DLSPEECHINFOTYPE;

/** DA-AD timing difference information extra data type. */
typedef struct OMX_SYMBIAN_AUDIO_EXTRADATA_DAADTIMINGDIFFTYPE {
    OMX_SYMBIAN_OTHER_EXTRADATABASETYPE base;
    OMX_TICKS nDaAdTimingDifference;
} OMX_SYMBIAN_AUDIO_EXTRADATA_DAADTIMINGDIFFTYPE;

/** Voice Activity Detection information extra data type. */
typedef struct OMX_SYMBIAN_AUDIO_EXTRADATA_VADINFOTYPE {
    OMX_SYMBIAN_OTHER_EXTRADATABASETYPE base;
    OMX_BOOL bUplinkVad;
    OMX_BOOL bDownlinkVad;
} OMX_SYMBIAN_AUDIO_EXTRADATA_VADINFOTYPE;

/** Audio tuning data type.

Audio tuning data type is used for passing tuning parameters via the OpenMAX IL 
component API to audio algorithms. The audio tuning parameters form an opaque 
data block, which only the sender and the algorithm understand.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.TuningData".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_TUNINGDATATYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nChannel;
    OMX_U8 nData[1];
} OMX_SYMBIAN_AUDIO_CONFIG_TUNINGDATATYPE;

/** Speech path loops control type.

Loop control for speech signal path.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.SpeechLoops".
*/

typedef struct OMX_SYMBIAN_AUDIO_CONFIG_SPEECHLOOPSTYPE { 
     OMX_U32 nSize;
     OMX_VERSIONTYPE nVersion;
     OMX_BOOL bLoopUplinkDownlink;
     OMX_BOOL bLoopUplinkDownlinkAlg;
     OMX_BOOL bLoopDownlinkUplink;
     OMX_BOOL bLoopDownlinkUplinkAlg;
} OMX_SYMBIAN_AUDIO_CONFIG_SPEECHLOOPSTYPE;

/**  Sidetone attenuation info type.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string "OMX.Symbian.Index.Config.Audio.SidetoneAttenuationInfo"
*/
typedef OMX_AUDIO_CONFIG_VOLUMETYPE OMX_AUDIO_CONFIG_SIDETONEATTENUATIONINFOTYPE;

/**  Volume info type.

This struct can be used to control sidetone attenuation.

This data structure is currently used for reading volume info with extension string 
  "OMX.Symbian.Index.Config.Audio.VolumeInfo"
*/
typedef OMX_AUDIO_CONFIG_VOLUMETYPE OMX_AUDIO_CONFIG_VOLUMEINFOTYPE;


/*@}*/

/** @name Audio HW Control Extensions */

/*@{*/

/** Audio power control.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.Power".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_POWERTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;            
    OMX_U32 nChannel;
    OMX_BOOL bPower;
} OMX_SYMBIAN_AUDIO_CONFIG_POWERTYPE;

/** Audio volume ramp control.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.VolumeRamp".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_VOLUMERAMPTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;            
    OMX_U32 nChannel;
    OMX_BOOL bLinear;
    OMX_BS32 sStartVolume;
    OMX_BS32 sEndVolume;
    OMX_TICKS nRampDuration;
    OMX_BOOL bRampTerminate;
    OMX_BS32 sCurrentVolume;
    OMX_TICKS nRampCurrentTime;
    OMX_TICKS nRampMinDuration;
    OMX_TICKS nRampMaxDuration;
    OMX_U32 nVolumeStep;
} OMX_SYMBIAN_AUDIO_CONFIG_VOLUMERAMPTYPE;

/** Audio register bank query.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.RegisterBankQuery".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_REGISTERBANKQUERYTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;   
    OMX_U16 nNumRegisterBanks;           
} OMX_SYMBIAN_AUDIO_CONFIG_REGISTERBANKQUERYTYPE;


typedef enum OMX_SYMBIAN_AUDIO_HWREGISTERTYPE { 
    OMX_SYMBIAN_AUDIO_RegisterIO = 0,
    OMX_SYMBIAN_AUDIO_RegisterI2C,
    OMX_SYMBIAN_AUDIO_RegisterSlimbus,
    OMX_SYMBIAN_AUDIO_RegisterCBUS = 0xFFFF,
    OMX_SYMBIAN_AUDIO_RegisterMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_HWREGISTERTYPE;
 
typedef struct OMX_SYMBIAN_AUDIO_REGBANKDESCRIPTORTYPE { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nRegBankIndex;   
    OMX_U32 nLogicalStartAddr;
    OMX_U32 nLogicalEndAddr;
    OMX_SYMBIAN_AUDIO_HWREGISTERTYPE eRegBankType;
} OMX_SYMBIAN_AUDIO_REGBANKDESCRIPTORTYPE;

/** Audio register bank table control.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.RegisterBankTable".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_REGISTERBANKTABLETYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;   
    OMX_U16 nNumRegisterBanks; 
    OMX_SYMBIAN_AUDIO_REGBANKDESCRIPTORTYPE RegBankDescriptorTable;     
} OMX_SYMBIAN_AUDIO_CONFIG_REGISTERBANKTABLETYPE;


typedef enum OMX_SYMBIAN_AUDIO_HWCTRLSTATUSTYPE { 
    OMX_SYMBIAN_AUDIO_HWCtrlStatusOk = 0,
    OMX_SYMBIAN_AUDIO_HWCtrlStatusFail,
    /* if callback facility is somehow solved in OpenMAX IL for getconfig/setconfig parameter calls, it is expected */
    /* to have here extra status information that can be returned for e.g. multi-read/write commands */
    OMX_SYMBIAN_AUDIO_HWCtrlStatusNotSupported = 0xFFFF,
    OMX_SYMBIAN_AUDIO_HWCtrlStatusMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_HWCTRLSTATUSTYPE;

typedef enum OMX_SYMBIAN_AUDIO_HWOPERATIONTYPE { 
    OMX_SYMBIAN_AUDIO_HWOperationNoOp = 0,
    OMX_SYMBIAN_AUDIO_HWOperation8BitSingleRead,
    OMX_SYMBIAN_AUDIO_HWOperation8BitSingleWrite,
    OMX_SYMBIAN_AUDIO_HWOperation16BitSingleRead,
    OMX_SYMBIAN_AUDIO_HWOperation16BitSingleWrite,
    OMX_SYMBIAN_AUDIO_HWOperation32BitSingleRead,
    OMX_SYMBIAN_AUDIO_HWOperation32BitSingleWrite,
    OMX_SYMBIAN_AUDIO_HWOperation8BitMultiRead,
    OMX_SYMBIAN_AUDIO_HWOperation8BitMultiWrite,
    OMX_SYMBIAN_AUDIO_HWOperation16BitMultiRead,
    OMX_SYMBIAN_AUDIO_HWOperation16BitMultiWrite,
    OMX_SYMBIAN_AUDIO_HWOperation32BitMultiRead,
    OMX_SYMBIAN_AUDIO_HWOperation32BitMultiWrite,
    OMX_SYMBIAN_AUDIO_HWOperation8BitBurstMultiRead,
    OMX_SYMBIAN_AUDIO_HWOperation8BitBurstMultiWrite,
    OMX_SYMBIAN_AUDIO_HWOperation16BitBurstMultiRead,
    OMX_SYMBIAN_AUDIO_HWOperation16BitBurstMultiWrite,
    OMX_SYMBIAN_AUDIO_HWOperation32BitBurstMultiRead,
    OMX_SYMBIAN_AUDIO_HWOperation32BitBurstMultiWrite = 0xFFFF,
    OMX_SYMBIAN_AUDIO_HWOperationMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_HWOPERATIONTYPE;

/** Custom audio HW control.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.CustomHwControl".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_CUSTOMHWCONTROLTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;   
    OMX_SYMBIAN_AUDIO_HWCTRLSTATUSTYPE eStatus;
    OMX_SYMBIAN_AUDIO_HWOPERATIONTYPE eHwOperation;
    OMX_SYMBIAN_AUDIO_HWREGISTERTYPE eRegisterType;
    OMX_U16 nDataCount;
    OMX_U8 nAddressDataSequence[1];   
} OMX_SYMBIAN_AUDIO_CONFIG_CUSTOMHWCONTROLTYPE;


typedef enum OMX_SYMBIAN_AUDIO_HWLOOPTYPE { 
    OMX_SYMBIAN_AUDIO_HWLoopNone = 0,
    OMX_SYMBIAN_AUDIO_HWLoopSidetone = 0xFFFF,
    /* rest of HW loops are to be defined on a vendor specific basis */
    OMX_SYMBIAN_AUDIO_HWLoopMax = 0x7FFFFFFF
} OMX_SYMBIAN_AUDIO_HWLOOPTYPE;

/** Audio HW loop support query.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.HwLoopSupport".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_HWLOOPSUPPORTTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;
    OMX_U32 nSupportedLoops;    
    OMX_SYMBIAN_AUDIO_HWLOOPTYPE eLoopIndex;
} OMX_SYMBIAN_AUDIO_CONFIG_HWLOOPSUPPORTTYPE;

/** Audio HW loop control.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.HwLoopControl".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_HWLOOPCONTROLTYPE  { 
    OMX_U32 nSize;                                    
    OMX_VERSIONTYPE nVersion;                   
    OMX_U32 nPortIndex;   
    OMX_SYMBIAN_AUDIO_HWLOOPTYPE eLoopIndex;
    OMX_U32 nChannel;
    OMX_BOOL bControlSwitch;
    OMX_BOOL bLinear;
    OMX_BS32 sLoopVolume;
} OMX_SYMBIAN_AUDIO_CONFIG_HWLOOPCONTROLTYPE;

/** Audio Processed Data Amount.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Audio.DataAmount".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_PROCESSEDDATAAMOUNTTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_TICKS nProcessedDataAmount;
} OMX_SYMBIAN_AUDIO_CONFIG_PROCESSEDDATAAMOUNTTYPE; 

/**
The OMX_SYMBIAN_AUDIO_CONFIG_POWERSAVETYPE structure is used to set or query
the port to power save mode.
In powersave mode audio signal playback or recording is done so that power consumption is minimized.
Typically this is achieved by processing and transferring bigger amount of data at a time so that HW sleep periods are maximized.
This is used also specifically for Audio Low Power mode for AVsink;
The index specified for this structure is retrieved using OMX_GetExtensionIndex()
with the extension string "OMX.Symbian.Index.Config.Audio.PowerSave".
*/
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_POWERSAVETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bPowerSave;
} OMX_SYMBIAN_AUDIO_CONFIG_POWERSAVETYPE;

/*@}*/

/** @name Haptics Extensions */

/*@{*/

/** Haptics driver properties query.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Param.Audio.HapticsDriverProperties".
*/
typedef struct OMX_SYMBIAN_AUDIO_PARAM_HAPTICSDRIVERPROPERTIESTYPE 
{
    OMX_U32 nSize;	
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nNativeSamplingRate;
    OMX_U32 nHapticsDriverNominalVoltage;    
    OMX_U32 nHapticsDriverImpedance;		 
} OMX_SYMBIAN_AUDIO_PARAM_HAPTICSDRIVERPROPERTIESTYPE;

/*@}*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_AudioExt_h */
