/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _OMX_3A_CAMERAEXT_H_
#define _OMX_3A_CAMERAEXT_H_

#include "OMX_3A_OtherExt.h"

/**
 *Focus control
 */
// The value of the lens special command.(in Type SET)
#define OMX_3A_CONFIG_LENS_SET_MECHANICAL_INF 0
#define OMX_3A_CONFIG_LENS_SET_INF            3000
#define OMX_3A_CONFIG_LENS_SET_1M             4000
#define OMX_3A_CONFIG_LENS_SET_MACRO          5000
#define OMX_3A_CONFIG_LENS_WIDTH              64
#define OMX_3A_CONFIG_LENS_HEIGHT             50
#define OMX_3A_CONFIG_GAMMA_SIZE              129
#define OMX_3A_CONFIG_SDL_SIZE                257
#define OMX_3A_CONFIG_MAX_TABLE_SIZE          3200		//OMX_3A_CONFIG_LENS_WIDTH*OMX_3A_CONFIG_LENS_HEIGHT


// sensor name
typedef enum
{
  OMX_3A_SENSOR_NAME_IMX072,
  OMX_3A_SENSOR_NAME_IMX111,
  OMX_3A_SENSOR_NAME_MAX,
  OMX_3A_SENSOR_NAME_UNKNOWN = 255
} OMX_3A_SENSOR_NAME_TYPE;

typedef enum
{
  OMX_3A_SENSORMODE_USAGE_VF = 0,
  OMX_3A_SENSORMODE_USAGE_AF = 1,
  OMX_3A_SENSORMODE_USAGE_STILL_CAPTURE = 2,
  OMX_3A_SENSORMODE_USAGE_NIGHT_STILL_CAPTURE = 3,
  OMX_3A_SENSORMODE_USAGE_STILL_SEQ_CAPTURE = 4,
  OMX_3A_SENSORMODE_USAGE_VIDEO_CAPTURE = 5,
  OMX_3A_SENSORMODE_USAGE_NIGHT_VIDEO_CAPTURE = 6,
  OMX_3A_SENSORMODE_USAGE_HQ_VIDEO_CAPTURE = 7,
  OMX_3A_SENSORMODE_USAGE_HS_VIDEO_CAPTURE = 8,
  OMX_3A_SENSORMODE_USAGE_MAX,
  OMX_3A_SENSORMODE_USAGE_UNDEFINED = 255,
} OMX_3A_SENSORMODE_USAGE_TYPE;

// Lens setting type.
typedef enum
{
  OMX_3A_CONFIG_LENS_SET,
  OMX_3A_CONFIG_LENS_MOVE_INF,
  OMX_3A_CONFIG_LENS_MOVE_MACRO,
} OMX_3A_CONFIG_LENS_SETTING_TYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_LENSSHADINGCORRECTIONTYPE
{
  OMX_U8  nEnable;
  OMX_U16 nGridWidth;
  OMX_U16 nGridHeight;
  OMX_U8  nPixelOrder;
  OMX_U8  nActiveReferenceCastCount;
  OMX_S32 nCastPosition0; // Fixed point coding S12.20 (for precision priority)
  OMX_S32 nCastPosition1;
  OMX_S32 nCastPosition2;
  OMX_S32 nCastPosition3;
} OMX_3A_ISPPARAM_STATIC_LENSSHADINGCORRECTIONTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_LENSSHADINGCORRECTIONTYPE
{
  OMX_S32 nLiveCast;  // Fixed point coding S12.20 (for precision priority)
} OMX_3A_ISPPARAM_DYNAMIC_LENSSHADINGCORRECTIONTYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_NOISEREDUCTIONTYPE
{
  OMX_U8 nEnable;
} OMX_3A_ISPPARAM_STATIC_NOISEREDUCTIONTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_NOISEREDUCTIONTYPE
{
  OMX_U16 nFrameSigma;
  OMX_U8 nScytheControl_hi;
  OMX_U8 nScytheControl_lo;
  OMX_U8 nCenterCorrectionSigmaFactor;
  OMX_U8 nRingCorrectionNormThr;
} OMX_3A_ISPPARAM_DYNAMIC_NOISEREDUCTIONTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_COLORCORRECTIONTYPE
{
  OMX_S32 nRedInRed;      // Fixed point coding S8.24 (for precision priority)
  OMX_S32 nGreenInRed;
  OMX_S32 nBlueInRed;
  OMX_S32 nRedInGreen;
  OMX_S32 nGreenInGreen;
  OMX_S32 nBlueInGreen;
  OMX_S32 nRedInBlue;
  OMX_S32 nGreenInBlue;
  OMX_S32 nBlueInBlue;
  OMX_S16 nOffset_R;
  OMX_S16 nOffset_G;
  OMX_S16 nOffset_B;
} OMX_3A_ISPPARAM_DYNAMIC_COLORCORRECTIONTYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_COLORCONVERSIONTYPE
{
  OMX_U8 nDisable;
} OMX_3A_ISPPARAM_STATIC_COLORCONVERSIONTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_COLORCONVERSIONTYPE
{
  OMX_U32 nTransform_Type;
  OMX_U32 nLumaExcursion;
  OMX_U32 nLumaMidpointTimes2;
  OMX_U32 nChromaCbExcursion;
  OMX_U32 nChromaCbMidpointTimes2;
  OMX_U32 nChromaCrExcursion;
  OMX_U32 nChromaCrMidpointTimes2;
  OMX_U8  nContrast;
  OMX_U8  nMaxContrast;
  OMX_U8  nColourSaturation;
  OMX_U8  nMaxColourSaturation;
  OMX_U32 nW0_0;
  OMX_U32 nW0_1;
  OMX_U32 nW0_2;
  OMX_U32 nW1_0;
  OMX_U32 nW1_1;
  OMX_U32 nW1_2;
  OMX_U32 nW2_0;
  OMX_U32 nW2_1;
  OMX_U32 nW2_2;
} OMX_3A_ISPPARAM_DYNAMIC_COLORCONVERSIONTYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_EDGEENHANCEMENTTYPE
{
  OMX_U8 nEnable;
  OMX_U8 nAdaptiveSharpeningEnable;
  OMX_U8 nRadialPeakingEanble;
} OMX_3A_ISPPARAM_STATIC_EDGEENHANCEMENTTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_EDGEENHANCEMENTTYPE
{
  OMX_U8 nGain;
  OMX_U8 nCoringLevel;
  OMX_U8 nOverShootGainBright;
  OMX_U8 nOverShootGainDark;
  OMX_S16 nLens_Centre_HOffset;
  OMX_S16 nLens_Centre_VOffset;
  OMX_U8 nRadial_Adsoc_RP_Polycoef0;
  OMX_U8 nRadial_Adsoc_RP_Polycoef1;
  OMX_U8 nRadial_Adsoc_RP_COF_Shift;
  OMX_U8 nRadial_Adsoc_RP_Unity;
} OMX_3A_ISPPARAM_DYNAMIC_EDGEENHANCEMENTTYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_BLACKLEVELCORRECTIONTYPE
{
  OMX_U8 nEnable;
  OMX_U32 nXCoefGr;
  OMX_U32 nYCoefGr;
  OMX_U32 nXCoefR;
  OMX_U32 nYCoefR;
  OMX_U32 nXCoefB;
  OMX_U32 nYCoefB;
  OMX_U32 nXCoefGb;
  OMX_U32 nYCoefGb;
  OMX_U16 nDcTermGr;
  OMX_U16 nDcTermR;
  OMX_U16 nDcTermB;
  OMX_U16 nDcTermGb;
  OMX_U16 nXSlantOrigin;
  OMX_U16 nYSlantOrigin;
} OMX_3A_ISPPARAM_STATIC_BLACKLEVELCORRECTIONTYPE;

// ISP Specific 1
typedef struct OMX_3A_ISPPARAM_STATIC_BABYLONTYPE
{
  OMX_U8 nEnable;
} OMX_3A_ISPPARAM_STATIC_BABYLONTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_BABYLONTYPE
{
  OMX_U8 nZipperKill;
  OMX_U8 nFlat_Threshold;
} OMX_3A_ISPPARAM_DYNAMIC_BABYLONTYPE;

// ISP Specific 2
typedef struct OMX_3A_ISPPARAM_STATIC_BAYERREPAIRTYPE
{
  OMX_U8 nEnable;
} OMX_3A_ISPPARAM_STATIC_BAYERREPAIRTYPE;

// ISP Specific 3
typedef struct OMX_3A_ISPPARAM_STATIC_CHANNELGAINTYPE
{
  OMX_U32 nRedGain;
  OMX_U32 nGreenInRedGain;
  OMX_U32 nGreenInBlueGain;
  OMX_U32 nBlueGain;
} OMX_3A_ISPPARAM_STATIC_CHANNELGAINTYPE;

// ISP Specific 4
typedef struct OMX_3A_ISPPARAM_STATIC_SCORPIOTYPE
{
  OMX_U8 nEnable;
  OMX_U8 nCoringLevel_Ctrl;
} OMX_3A_ISPPARAM_STATIC_SCORPIOTYPE;

// ISP Specific 5
typedef struct OMX_3A_ISPPARAM_STATIC_SDLTYPE
{
  OMX_U32 nSDLMode_Control;
} OMX_3A_ISPPARAM_STATIC_SDLTYPE;

// ISP Specific
typedef struct OMX_3A_ISPPARAM_ISPSPECIFICTYPE
{
  //BABYLONTYPE
  OMX_3A_ISPPARAM_STATIC_BABYLONTYPE babylon_static;
  OMX_3A_ISPPARAM_DYNAMIC_BABYLONTYPE babylon_dynamic;
  //BAYERREPAIRTYPE
  OMX_3A_ISPPARAM_STATIC_BAYERREPAIRTYPE bayerRepair_static;
  //CHANNELGAINTYPE
  OMX_3A_ISPPARAM_STATIC_CHANNELGAINTYPE channelGain_static;
  //SCORPIOTYPE
  OMX_3A_ISPPARAM_STATIC_SCORPIOTYPE scorpio_static;
  //SDLTYPE
  OMX_3A_ISPPARAM_STATIC_SDLTYPE sdl_static;
} OMX_3A_ISPPARAM_ISPSPECIFICTYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_AEWBTYPE
{
  OMX_U8 nHGridSize;
  OMX_U8 nVGridSize;
  OMX_U32 nHBlockSizeFraction;
  OMX_U32 nVBlockSizeFraction;
  OMX_U32 nHROIStartFraction;
  OMX_U32 nVROIStartFraction;
  OMX_U8 nRedSaturationLevel;
  OMX_U8 nGreenSaturationLevel;
  OMX_U8 nBlueSaturationLevel;
  OMX_U32 nGlaceDataSource;
} OMX_3A_ISPPARAM_STATIC_AEWBTYPE;

typedef struct OMX_3A_ISPPARAM_AFWINDOWTYPE
{
  OMX_U16 nHostAFZoneStartX;
  OMX_U16 nHostAFZoneStartY;
  OMX_U16 nHostAFZoneEndX;
  OMX_U16 nHostAFZoneEndY;
  OMX_U8  nEnable;
} OMX_3A_ISPPARAM_AFWINDOWTYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_AFTYPE
{
  OMX_U8 nCoringValue;
  OMX_U8 nHRatioNum;
  OMX_U8 nHRatioDen;
  OMX_U8 nVRatioNum;
  OMX_U8 nVRatioDen;
  OMX_U8 nHostActiveZonesCount;
  OMX_U8 nFlag_AbsSquareEnabled;
  OMX_U32 nFlag_ReducedZoneSetup;
} OMX_3A_ISPPARAM_STATIC_AFTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_AFTYPE
{
  OMX_3A_ISPPARAM_AFWINDOWTYPE nHostAFZone[OMX_3A_OTHER_STATSAF_WINDOW_NUMBER_MAX];
} OMX_3A_ISPPARAM_DYNAMIC_AFTYPE;

typedef struct OMX_3A_ISPPARAM_STATIC_HISTSTATSTYPE
{
  OMX_U32 nHistSizeRelativeToFOV_X;
  OMX_U32 nHistSizeRelativeToFOV_Y;
  OMX_U32 nHistOffsetRelativeToFOV_X;
  OMX_U32 nHistOffsetRelativeToFOV_Y;
  OMX_U32 nHistInputSrc;
} OMX_3A_ISPPARAM_STATIC_HISTSTATSTYPE;

typedef struct OMX_3A_ISPPARAM_DYNAMIC_WHITEBALANCETYPE
{
  OMX_U32 nRedManualgain;
  OMX_U32 nGreenManualgain;
  OMX_U32 nBlueManualgain;
} OMX_3A_ISPPARAM_DYNAMIC_WHITEBALANCETYPE;

typedef struct OMX_3A_CONFIG_ISPPARAMETERSTYPE
{
  OMX_U32 nSize;                    /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;         /**< OMX specification version information */
  OMX_U32 nPortIndex;               /**< Port that this structure applies to */
  //LENSSHADINGCORRECTIONTYPE
  OMX_3A_ISPPARAM_STATIC_LENSSHADINGCORRECTIONTYPE  nispparam_lensshad_static;
  OMX_3A_ISPPARAM_DYNAMIC_LENSSHADINGCORRECTIONTYPE nispparam_lensshad_dynamic;
  //NOISEREDUCTIONTYPE
  OMX_3A_ISPPARAM_STATIC_NOISEREDUCTIONTYPE         nispparam_noiser_static;
  OMX_3A_ISPPARAM_DYNAMIC_NOISEREDUCTIONTYPE        nispparam_noiser_dynamic;
  //COLORCORRECTIONTYPE
  OMX_3A_ISPPARAM_DYNAMIC_COLORCORRECTIONTYPE       nispparam_colormatrix_dynamic;
  //COLORCONVERSIONTYPE
  OMX_3A_ISPPARAM_STATIC_COLORCONVERSIONTYPE        nispparam_colorconversion_static;
  OMX_3A_ISPPARAM_DYNAMIC_COLORCONVERSIONTYPE       nispparam_colorconversion_dynamic;
  //EDGEENHANCEMENTTYPE
  OMX_3A_ISPPARAM_STATIC_EDGEENHANCEMENTTYPE        nispparam_edgeEnhancement_static;
  OMX_3A_ISPPARAM_DYNAMIC_EDGEENHANCEMENTTYPE       nispparam_edgeEnhancement_dynamic;
  //BLACKLEVELCORRECTIONTYPE
  OMX_3A_ISPPARAM_STATIC_BLACKLEVELCORRECTIONTYPE   nispparam_black_static;
  //ISPSPECIFICTYPE
  OMX_3A_ISPPARAM_ISPSPECIFICTYPE                   nispparam_ispSpecific;
  //AEWBTYPE
  OMX_3A_ISPPARAM_STATIC_AEWBTYPE                   nispparam_aewb_static;
  //AFTYPE
  OMX_3A_ISPPARAM_STATIC_AFTYPE                     nispparam_af_static;
  OMX_3A_ISPPARAM_DYNAMIC_AFTYPE                    nispparam_af_dynamic;
  //HISTSTATSTYPE
  OMX_3A_ISPPARAM_STATIC_HISTSTATSTYPE              nispparam_histstats_static;
  //WHITEBALANCETYPE
  OMX_3A_ISPPARAM_DYNAMIC_WHITEBALANCETYPE          nispparam_wb_dynamic;
} OMX_3A_CONFIG_ISPPARAMETERSTYPE;

typedef struct OMX_3A_CONFIG_GAMMATABLE_TYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_U32 nsizeOfTable;       /**< MaxTableSize */
  OMX_U32 nGammaR[OMX_3A_CONFIG_GAMMA_SIZE];
  OMX_U32 nGammaG[OMX_3A_CONFIG_GAMMA_SIZE];
  OMX_U32 nGammaB[OMX_3A_CONFIG_GAMMA_SIZE];
} OMX_3A_CONFIG_GAMMATABLE_TYPE;

typedef struct OMX_3A_CONFIG_LSCTABLE_TYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_U32 nsizeOfTable;       /**< MaxTableSize */
  OMX_U32 nCastPos0GridsData[OMX_3A_CONFIG_MAX_TABLE_SIZE];
  OMX_U32 nCastPos1GridsData[OMX_3A_CONFIG_MAX_TABLE_SIZE];
  OMX_U32 nCastPos2GridsData[OMX_3A_CONFIG_MAX_TABLE_SIZE];
  OMX_U32 nCastPos3GridsData[OMX_3A_CONFIG_MAX_TABLE_SIZE];
} OMX_3A_CONFIG_LSCTABLE_TYPE;

typedef struct OMX_3A_CONFIG_LINEARIZERTABLE_TYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_U32 nsizeOfTable;       /**< MaxTableSize */
  OMX_U32 nSdlR[OMX_3A_CONFIG_SDL_SIZE];
  OMX_U32 nSdlGr[OMX_3A_CONFIG_SDL_SIZE];
  OMX_U32 nSdlGb[OMX_3A_CONFIG_SDL_SIZE];
  OMX_U32 nSdlB[OMX_3A_CONFIG_SDL_SIZE];
} OMX_3A_CONFIG_LINEARIZERTABLE_TYPE;

typedef struct OMX_3A_CONFIG_EXPOSURETYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_U16 gain;               /**< gain value */
  OMX_U32 nTargetexposuretime;/**< nTarget Exposure time (sec)*/
} OMX_3A_CONFIG_EXPOSURETYPE;

typedef struct OMX_3A_CONFIG_FOCUSTYPE
{
  OMX_U32 nSize;            /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
  OMX_U32 nPortIndex;       /**< Port that this structure applies to */
  OMX_U32 lensSettingType;  /**< Les setting type (Set, Move to Inf, Move to Macro)> */
  OMX_U16 lensPos;          /**< Lens position */
  OMX_U32 silentVal;        /**< Lens Silent control parameter */
  OMX_U8 lensIsReady;       /**< Lens ready flag(1:lens is ready 0:lens not ready)*/
} OMX_3A_CONFIG_FOCUSTYPE;

typedef struct OMX_3A_CONFIG_MAXFRAMERATETYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_U32 framerate;          /**< framerate U 16.16 format*/
} OMX_3A_CONFIG_MAXFRAMERATETYPE;

typedef struct OMX_3A_CONFIG_FLASHSYNCHROTYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_BOOL bSync;             /**< Flash sync (OMX_TRUE:sync flash, OMX_FALSE:disable sync) */
} OMX_3A_CONFIG_FLASHSYNCHROTYPE;

typedef struct OMX_3A_PARAM_SENSORMODETYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_3A_SENSORMODE_USAGE_TYPE sensorMode;          /**< sensor restriction mode */
} OMX_3A_CONFIG_SENSORMODETYPE;

typedef struct OMX_3A_CONFIG_EEPROMDATATYPE
{
  OMX_U32 nSize;              /**< Size of the structure in bytes */
  OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
  OMX_U32 nPortIndex;         /**< Port that this structure applies to */
  OMX_U32 nSizeOfData;        /**< The size of unparsed NVM data */
  OMX_U8  eepromData[2048];   /**< actual data */
} OMX_3A_CONFIG_EEPROMDATATYPE;


#endif
