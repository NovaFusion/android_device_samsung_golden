/* ---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*
 * MODULE NAME                                                                *  
 *----------------------------------------------------------------------------*
 * Audio Codecs Chipset API                                                   *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * DETAILED DESCRIPTION                                                       * 
 * ---------------------------------------------------------------------------*
 * Audio codecs chipset API header file. This file in addition with standard  *
 * OpenMAX IL 1.1.1 header files constitute complete header files for Nokia   *
 * Audio Codecs Chipset API.												  *
 *                                                                            *
 * Copyright (c) 2007 Nokia                                                   *
 *                                                                            *
 *                                                                            *
 * Version History:                                                           *
 * ---------------------------------------------------------------------------*
 *   Date                Author                Description                    *
 *   ----                ------                -----------                    *
 * 17-NOV-2008        Ashish Malot    			Created the initial version.			*
 * 30-MAR-2009        Ashish Malot    			Updates according to 1.0 version  *
 * 20-APR-2009        Vaibhav Goyal         Minor changes done to compile it	*
 * 18-June-2009				Ashish Malot					Corresponding to 1.2 version			*
 *----------------------------------------------------------------------------*/

#ifndef AUDIO_CODECS_CHIPSET_API_H
#define AUDIO_CODECS_CHIPSET_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully 
 */

#include <OMX_Audio.h>


/** WMA Pro params **/

typedef enum OMX_AUDIO_WMAPROPROFILETYPE {
    OMX_AUDIO_WMAPROProfileUnused = 0,          /**< AMRNB Mode unused / unknown */
    OMX_AUDIO_WMAPROProfileM0,
    OMX_AUDIO_WMAPROProfileM1,
    OMX_AUDIO_WMAPROProfileM2,
    OMX_AUDIO_WMAPROProfileM3,               
    OMX_AUDIO_WMAPROProfileMax = 0x7FFFFFFF
} OMX_AUDIO_WMAPROPROFILETYPE;
    
typedef struct OMX_AUDIO_PARAM_WMAPROTYPE {
		OMX_U32 nSize;
		OMX_VERSIONTYPE nVersion;
		OMX_U32 nPortIndex;
		OMX_U16 nChannels;
		OMX_U32 nBitRate;
		OMX_AUDIO_WMAPROPROFILETYPE eProfile;
		OMX_U32 nSamplingRate;
		OMX_U32 nBlockAlign;
		OMX_U32 nSuperBlockAlign;
		OMX_U32 nEncodeOptions;
		OMX_U32 nAdvancedEncodeOptions;
		OMX_U32 nAdvancedEncodeOptions2;
		OMX_U32 nChannelMask;
		OMX_U32 nSourceBitsPerSample;
} OMX_AUDIO_PARAM_WMAPROTYPE;

/** End of WMA Pro related **/

typedef struct OMX_AUDIO_CONFIG_AMRMODETYPE {
		OMX_U32 nSize;
		OMX_VERSIONTYPE nVersion;
		OMX_U32 nPortIndex;
		OMX_AUDIO_AMRBANDMODETYPE eAMRBandMode;
} OMX_AUDIO_CONFIG_AMRMODETYPE;

typedef struct OMX_AUDIO_CONFIG_ILBCFRAMETYPE {
		OMX_U32 nSize;
		OMX_VERSIONTYPE nVersion;
		OMX_U32 nPortIndex;
		OMX_U32 nFrameLength; 
} OMX_AUDIO_CONFIG_ILBCFRAMETYPE;

/** SBC codec related **/
typedef struct OMX_AUDIO_CONFIG_SBCBITPOOLTYPE {
	  OMX_U32 nSize;
	  OMX_VERSIONTYPE nVersion;
	  OMX_U32 nPortIndex;
	  OMX_U32 nNewBitPool; 
} OMX_AUDIO_CONFIG_SBCBITPOOLTYPE;

/** End of SBC codec related **/

/** iLBC related **/
typedef struct OMX_AUDIO_PARAM_ILBCTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;            
     OMX_U32 nFrameLength;
     OMX_BOOL bIsDTXCNGSupported;
     OMX_BOOL bDTXCNGSelected;                       
     OMX_BOOL bPLCSelected;                                                                       
 } OMX_AUDIO_PARAM_ILBCTYPE;

/** End of iLBC related **/

/** G711 related **/
typedef struct OMX_AUDIO_PARAM_G711TYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex; 
     OMX_BOOL bIsDTXCNGSupported;
     OMX_BOOL bIsPLCSupported;       
     OMX_BOOL bDTXCNGEnabled;           
     OMX_BOOL bPLCEnabled;
     OMX_U32  nFrameLength;                                                                     
 } OMX_AUDIO_PARAM_G711TYPE;

/** End of G711 related **/

/** FLAC related **/
typedef struct OMX_AUDIO_PARAM_FLACTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex; 
     OMX_U32 nFrameLength;          /**<Number of samples per frame  */                     
     OMX_U32 nChannels;             /**< Number of channels */
     OMX_U32 nSampleRate;           
     OMX_U32 nBitsPerSample;        /**< Number of bits in each sample */ 
 } OMX_AUDIO_PARAM_FLACTYPE;

/** End of FLAC related **/

/** DDPLUS related **/
typedef struct OMX_AUDIO_PARAM_DDPLUSTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;
     OMX_U16 nOutnChans;              /**< Number of channels */ 
     OMX_U16 nOutLfe;                 /**< lfe present or not */
     OMX_U32 nSampleRate;             
     OMX_U16 nOutChanconfig;          /**< output channel configuration */
     OMX_U16 nStereomode;             /**< stereo output mode */
     OMX_U32 nCompMode;               /**< dynamic range compression mode */
     OMX_U32 nDynscaleLow;            /**< dynamic range compression boost scale factor */
     OMX_U32 nDynscaleHigh;           /**< dynamic range compression cut scale factor */

} OMX_AUDIO_PARAM_DDPLUSTYPE;
 
/** End of DDPLUS related **/ 

/** DTS related **/
typedef struct OMX_AUDIO_PARAM_DTSTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;                                                                     
 } OMX_AUDIO_PARAM_DTSTYPE;
 
/** End of DTS related **/

/** AC3 related **/
typedef struct OMX_AUDIO_PARAM_AC3TYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;                                                                     
 } OMX_AUDIO_PARAM_AC3TYPE;
 
/** End of AC3 related **/
 
typedef struct OMX_SYMBIAN_AUDIO_CONFIG_CHANNELSWITCHBOARDTYPE {
OMX_U32 nSize;
OMX_VERSIONTYPE nVersion;
OMX_U32 nPortIndex;
OMX_U32 nOutputChannel[OMX_AUDIO_MAXCHANNELS];
} OMX_SYMBIAN_AUDIO_CONFIG_CHANNELSWITCHBOARDTYPE;


/* A2DP related configuration structure */
typedef struct OMX_AUDIO_PARAM_A2DPTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
} OMX_AUDIO_PARAM_A2DPTYPE;
/** End of Dummy component **/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

