/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _IFM_TYPES_H_
#define _IFM_TYPES_H_

#include "OMX_Types.h"
#include "OMX_IVCommon.h"
#include "OMX_Image.h"
#include "ENS_Index.h"

#include "omxilosalservices.h"

#ifdef __SYMBIAN32__
#  include <openmax/il/shai/OMX_Symbian_CameraIndexExt.h>
#  include <openmax/il/shai/OMX_Symbian_ExtensionNames.h>
#  include <openmax/il/shai/OMX_Symbian_CameraExt.h>
#  include <openmax/il/shai/OMX_Symbian_IVCommonExt.h>
#else /* __SYMBIAN32__ */
#  include <OMX_Symbian_CameraIndexExt_Ste.h>
#  include <OMX_Symbian_ExtensionNames_Ste.h>
#  include <OMX_Symbian_CameraExt_Ste.h>
#  include <OMX_Symbian_IVCommonExt_Ste.h>
#endif /* __SYMBIAN32__ */

#define IFM_SIZE_LIST_PE_READ 32
#define IFM_SIZE_LIST_PE_WRITE 64
#define IFM_MAX_SIZE_SERIALSHOOTING 10

// Maximum number of Focus Status ROIs
#define IFM_MAX_NUMBER_OF_FOCUS_STATUS_ROIS OMX_SYMBIAN_MAX_NUMBER_OF_ROIS

// Size of Focus Status config
#define IFM_CONFIG_EXTFOCUSSTATUS_SIZE(n) (sizeof(OMX_SYMBIAN_CONFIG_EXTFOCUSSTATUSTYPE)- sizeof(OMX_SYMBIAN_AFROITYPE) + (n)*sizeof(OMX_SYMBIAN_AFROITYPE))

// Maximum number of Bracketing values
#define IFM_MAX_NUMBER_OF_BRACKETING_VALUES 4
#define ZSLHDR_MAX_FRAMELIMIT 3

// Size of Bracketing config
#define IFM_CONFIG_BRACKETING_SIZE(n) (sizeof(OMX_SYMBIAN_CONFIG_BRACKETINGTYPE) - sizeof(OMX_U32)+ (n)*sizeof(OMX_U32))

// Size of RGB Histogram bins
#define HISTO_BIN_NB  256
// For Camera
typedef struct IFM_CONFIG_READPETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U8 nPEToRead;
    OMX_U16 nPEList_addr[IFM_SIZE_LIST_PE_READ];  // PE list to read
    OMX_U32 nPEList_data[IFM_SIZE_LIST_PE_READ];  // PE values
} IFM_CONFIG_READPETYPE;

typedef struct IFM_CONFIG_WRITEPETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U8 nPEToWrite;
    OMX_U16 nPEList_addr[IFM_SIZE_LIST_PE_WRITE];  // PE address list to write
    OMX_U32 nPEList_data[IFM_SIZE_LIST_PE_WRITE];  // PE data list to write
} IFM_CONFIG_WRITEPETYPE;

typedef enum t_operating_mode_camera{
	OpMode_Cam_Unknown=0,
	OpMode_Cam_VideoPreview,
	OpMode_Cam_VideoRecord,
	OpMode_Cam_StillPreview,
	OpMode_Cam_StillCaptureSingle,
	OpMode_Cam_StillCaptureBurstLimited,
	OpMode_Cam_StillCaptureBurstInfinite,
	OpMode_Cam_StillFaceTracking,
	OpMode_Cam_StillCaptureSingleInVideo,
	OpMode_Cam_StillCaptureBurstLimitedInVideo,
	OpMode_Cam_StillCaptureBurstInfiniteInVideo,
	OpMode_Cam_MAX = 0x7FFFFFFF
}t_operating_mode_camera;


typedef struct IFM_CONFIG_OPMODE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    t_operating_mode_camera eOpMode;
} IFM_CONFIG_OPMODE_CHECK;

/* Config for configuring if camera does alive */
/* checks and whether it will print traces. */
typedef struct IFM_CONFIG_CHECK_ALIVE {
  OMX_U32 nSize;
  OMX_VERSIONTYPE nVersion;
  OMX_U32 nPortIndex;
  OMX_U32 nCheckIntervalMsec; // how often check is performed, 0 means disabled
  OMX_BOOL bPrintStatus; // whether status will be printed to trace
} IFM_CONFIG_CHECK_ALIVE;

/* our own extradata types on top of OMX defaults */
/* Caution ; this enum is defined again in stab; please update it when updating this one */
typedef enum {
	IFM_BMS_CAPTURE_CONTEXT_TYPE = OMX_ExtraDataVendorStartUnused,
	IFM_DAMPERS_DATA_TYPE,
	OMX_ExtraDataCameraProductionFlashTest,
	OMX_STE_ExtraData_CaptureParameters,	//Added for extraData
	IFM_EXTRADATA_MAX = 0x7FFFFFFF
} IFM_EXTRADATATYPE;


typedef struct IFM_CONFIG_ISPSHARED {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnabled;
} IFM_CONFIG_ISPSHARED;


typedef struct IFM_CONFIG_3A_TRACEDUMPCONTROL {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnabled;  /* If enabled, the 3A library execution and makernote trace files are produced. */
} IFM_CONFIG_3A_TRACEDUMPCONTROL;


typedef enum t_test_mode_camera{
	TestMode_Cam_None=0,
	TestMode_Cam_NormalMode,
	TestMode_Cam_ColorBar,
	TestMode_Cam_GreyScale,
	TestMode_Cam_SolidBar_Black,
	TestMode_Cam_SolidBar_Blue,
	TestMode_Cam_SolidBar_Cyan,
	TestMode_Cam_SolidBar_Green,
	TestMode_Cam_SolidBar_Magenta,
	TestMode_Cam_SolidBar_Red,
	TestMode_Cam_SolidBar_White,
	TestMode_Cam_SolidBar_Yellow,
	TestMode_Cam_SolidBar_Custom_RGB,
	TestMode_Cam_ColorMatrix_0_Unitary,
	TestMode_Cam_ColorMatrix_1_Unitary,
	TestMode_Cam_AntiFlicker_On,
	TestMode_Cam_AntiFlicker_Off,
	TestMode_Cam_MAX = 0x7FFFFFFF
}t_test_mode_camera;

typedef struct IFM_CONFIG_TESTMODE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    t_test_mode_camera eTestMode;
    OMX_U8 aSolidBar_R; //only used by TestMode_Cam_SolidBar_custom_RGB test mode to define suitable color for solidbar
    OMX_U8 aSolidBar_G; //only used by TestMode_Cam_SolidBar_custom_RGB test mode to define suitable color for solidbar
    OMX_U8 aSolidBar_B; //only used by TestMode_Cam_SolidBar_custom_RGB test mode to define suitable color for solidbar
} IFM_CONFIG_TESTMODE;

// previous 2 structures will have to be removed as they are replaced by the 2 following one

typedef enum OMX_STE_TESTMODETYPE {
	OMX_STE_TestModeNone=0,
	OMX_STE_TestModeNormalMode,
	OMX_STE_TestModeColorBar,
	OMX_STE_TestModeGreyScale,
	OMX_STE_TestModeSolidBar_Black,
	OMX_STE_TestModeSolidBar_Blue,
	OMX_STE_TestModeSolidBar_Cyan,
	OMX_STE_TestModeSolidBar_Green,
	OMX_STE_TestModeSolidBar_Magenta,
	OMX_STE_TestModeSolidBar_Red,
	OMX_STE_TestModeSolidBar_White,
	OMX_STE_TestModeSolidBar_Yellow,
    OMX_STE_TestModeSolidBar_Custom_RGB,
	OMX_STE_TestModePN9,
    OMX_STE_TestModeMAX = 0x7FFFFFFF
} OMX_STE_TESTMODETYPE;

typedef struct OMX_STE_CONFIG_SENSORTESTTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_STE_TESTMODETYPE eTestMode;
    OMX_U8 nSolidBar_R; //only used by TestMode_Sensor_SolidBar_Custom_RGB test mode to define suitable color for solidbar
    OMX_U8 nSolidBar_G; //only used by TestMode_Sensor_SolidBar_Custom_RGB test mode to define suitable color for solidbar
    OMX_U8 nSolidBar_B; //only used by TestMode_Sensor_SolidBar_Custom_RGB test mode to define suitable color for solidbar
} OMX_STE_CONFIG_SENSORTESTTYPE;

typedef enum t_sensor_alignment{
	SensorAlign_None=0,
	SensorAlign_raw_r_g,
	SensorAlign_raw_b_g,
	SensorAlign_raw_g_r,
	SensorAlign_raw_g_b,
	SensorAlign_MAX = 0x7FFFFFFF
}t_sensor_alignment;

typedef struct IFM_CONFIG_SENSOR_ALIGNMENT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    t_sensor_alignment eSensorAlign;
} IFM_CONFIG_SENSOR_ALIGNMENT;



// For DisplaySink

typedef struct IFM_CONFIG_DISPLAYSINK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nTarg_width;
    OMX_U32 nTarg_height;
} IFM_CONFIG_DISPLAYSINK;


typedef enum t_stab{
	stab_disable=0,
	stab_equal,
	stab_not_equal
}t_stab;

// For ImgFilter

typedef enum t_env_test_info{
	env_test_Unknown=0,
	env_test_Standalone,
	env_test_Darkbox,
	env_test_MAX = 0x7FFFFFFF
}t_env_test_info;

typedef struct IFM_CONFIG_IMAGEFILTERTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_IMAGEFILTERTYPE eImageFilter;
} IFM_CONFIG_IMAGEFILTERTYPE_CHECK;

typedef struct IFM_CONFIG_IMAGEFILTERTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_IMAGEFILTERTYPE eImageFilter;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_IMAGEFILTERTYPE_RESULT;

typedef enum t_color_enhancement_kind{
	color_enhancement_None=0,
	color_enhancement_sepia,
	color_enhancement_black_and_white,
	color_enhancement_MAX = 0x7FFFFFFF
}t_color_enhancement_kind;

typedef struct IFM_CONFIG_COLORENHANCEMENTTYPE_CHECK {
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_BOOL bColorEnhancement;
	OMX_U8 nCustomizedU;
	OMX_U8 nCustomizedV;
	t_color_enhancement_kind eEffect;
} IFM_CONFIG_COLORENHANCEMENTTYPE_CHECK;

typedef struct IFM_CONFIG_COLORENHANCEMENTTYPE_RESULT {
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_BOOL bColorEnhancement;
	OMX_U8 nCustomizedU;
	OMX_U8 nCustomizedV;
	t_color_enhancement_kind eEffet;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_COLORENHANCEMENTTYPE_RESULT;

typedef struct IFM_CONFIG_WHITEBALANCETYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_WHITEBALCONTROLTYPE eWhiteBalControl;
} IFM_CONFIG_WHITEBALANCETYPE_CHECK;

typedef struct IFM_CONFIG_WHITEBALANCETYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_WHITEBALCONTROLTYPE eWhiteBalControl;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_WHITEBALANCETYPE_RESULT;

typedef struct IFM_CONFIG_BRIGHTNESSTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_U32 nBrightness;
} IFM_CONFIG_BRIGHTNESSTYPE_CHECK;

typedef struct IFM_CONFIG_BRIGHTNESSTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_U32 nBrightness;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_BRIGHTNESSTYPE_RESULT;

typedef struct IFM_CONFIG_CONTRASTTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_S32 nContrast;
} IFM_CONFIG_CONTRASTTYPE_CHECK;

typedef struct IFM_CONFIG_CONTRASTTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_S32 nContrast;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_CONTRASTTYPE_RESULT;

typedef struct IFM_CONFIG_SATURATIONTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_S32 nSaturation;
} IFM_CONFIG_SATURATIONTYPE_CHECK;

typedef struct IFM_CONFIG_SATURATIONTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_S32 nSaturation;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_SATURATIONTYPE_RESULT;

typedef struct IFM_CONFIG_EXPOSURECONTROLTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_EXPOSURECONTROLTYPE eExposureControl;
} IFM_CONFIG_EXPOSURECONTROLTYPE_CHECK;

typedef struct IFM_CONFIG_EXPOSURECONTROLTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_EXPOSURECONTROLTYPE eExposureControl;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_EXPOSURECONTROLTYPE_RESULT;

typedef enum t_expo_val_type {
	expo_val_Unknown=0,
	expo_val_xEVCompensation	=0x00000001,
	expo_val_nApertureFNumber	=0x00000010,
	expo_val_bAutoAperture		=0x00000100,
	expo_val_nShutterSpeedMsec	=0x00001000,
	expo_val_bAutoShutterSpeed	=0x00010000,
	expo_val_nSensitivity		=0x00100000,
	expo_val_bAutoSensitivity	=0x01000000,
	expo_val_MAX = 0x7FFFFFFF
}t_expo_val_type;

typedef struct IFM_CONFIG_SOFTWAREVERSION{
    OMX_U32 nSize;            /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    OMX_U32 nPortIndex;       /**< Port that this structure applies to */
    OMX_U8 cSoftware[128];        /**< String containing version of the software
                                   (client allocates buffer for 128 characters including NULL termination) */
} IFM_CONFIG_SOFTWAREVERSION;

typedef struct IFM_CONFIG_EXPOSUREVALUETYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_METERINGTYPE eMetering;
    OMX_S32 xEVCompensation;      /**< Fixed point value stored as Q16 */
    OMX_U32 nApertureFNumber;     /**< e.g. nApertureFNumber = 2 implies "f/2" - Q16 format */
    OMX_BOOL bAutoAperture;		  /**< Whether aperture number is defined automatically */
    OMX_U32 nShutterSpeedMsec;    /**< Shutterspeed in milliseconds */
    OMX_BOOL bAutoShutterSpeed;	  /**< Whether shutter speed is defined automatically */
    OMX_U32 nSensitivity;         /**< e.g. nSensitivity = 100 implies "ISO 100" */
    OMX_BOOL bAutoSensitivity;	  /**< Whether sensitivity is defined automatically */
	t_expo_val_type nExpoValType; /**< which parameter is going to be checked*/
} IFM_CONFIG_EXPOSUREVALUETYPE_CHECK;

typedef struct IFM_CONFIG_EXPOSUREVALUETYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_METERINGTYPE eMetering;
    OMX_S32 xEVCompensation;      /**< Fixed point value stored as Q16 */
    OMX_U32 nApertureFNumber;     /**< e.g. nApertureFNumber = 2 implies "f/2" - Q16 format */
    OMX_BOOL bAutoAperture;		  /**< Whether aperture number is defined automatically */
    OMX_U32 nShutterSpeedMsec;    /**< Shutterspeed in milliseconds */
    OMX_BOOL bAutoShutterSpeed;	  /**< Whether shutter speed is defined automatically */
    OMX_U32 nSensitivity;         /**< e.g. nSensitivity = 100 implies "ISO 100" */
    OMX_BOOL bAutoSensitivity;	  /**< Whether sensitivity is defined automatically */
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
	t_expo_val_type nExpoValType; /**< which parameter is going to be checked*/
} IFM_CONFIG_EXPOSUREVALUETYPE_RESULT;

typedef struct IFM_CONFIG_SCALEFACTORTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_S32 xWidth;
    OMX_S32 xHeight;
} IFM_CONFIG_SCALEFACTORTYPE_CHECK;

typedef struct IFM_CONFIG_SCALEFACTORTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_S32 xWidth;
    OMX_S32 xHeight;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_SCALEFACTORTYPE_RESULT;

typedef struct IFM_IMAGE_CONFIG_FOCUSCONTROLTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_IMAGE_FOCUSCONTROLTYPE eFocusControl;
} IFM_IMAGE_CONFIG_FOCUSCONTROLTYPE_CHECK;

typedef struct IFM_IMAGE_CONFIG_FOCUSCONTROLTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_IMAGE_FOCUSCONTROLTYPE eFocusControl;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_IMAGE_CONFIG_FOCUSCONTROLTYPE_RESULT;

typedef struct IFM_PARAM_PORTDEFINITION_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 xFrameRate;
} IFM_PARAM_PORTDEFINITION_CHECK;

typedef struct IFM_PARAM_PORTDEFINITION_RESULT{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 xFrameRate;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_PARAM_PORTDEFINITION_RESULT;

typedef enum IFM_FIRMWARETRACE_MECHANISMTYPE {
   IFM_FirmwareTraceMechanism_None,
   IFM_FirmwareTraceMechanism_XTI,
   IFM_FirmwareTraceMechanism_SharedMemory
} IFM_FIRMWARETRACE_MECHANISMTYPE;

typedef struct IFM_PARAM_FIRMWARETRACETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bEnabled;
    IFM_FIRMWARETRACE_MECHANISMTYPE eTraceMechanism;
    OMX_U32 nLogLevel;
} IFM_PARAM_FIRMWARETRACETYPE;

typedef struct IFM_CONFIG_FORMATTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_COLOR_FORMATTYPE eFormatControl;
} IFM_CONFIG_FORMATTYPE_CHECK;

typedef struct IFM_CONFIG_FORMATTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_COLOR_FORMATTYPE eFormatControl;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_FORMATTYPE_RESULT;

typedef struct IFM_IMAGE_CONFIG_FOCUSREGION_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_BOOL bCenter;
	OMX_BOOL bLeft;
	OMX_BOOL bRight;
	OMX_BOOL bTop;
	OMX_BOOL bBottom;
	OMX_BOOL bTopLeft;
	OMX_BOOL bTopRight;
	OMX_BOOL bBottomLeft;
	OMX_BOOL bBottomRight;
} IFM_IMAGE_CONFIG_FOCUSREGION_CHECK;

typedef struct IFM_IMAGE_CONFIG_FOCUSREGION_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
	OMX_BOOL bCenter;
	OMX_BOOL bLeft;
	OMX_BOOL bRight;
	OMX_BOOL bTop;
	OMX_BOOL bBottom;
	OMX_BOOL bTopLeft;
	OMX_BOOL bTopRight;
	OMX_BOOL bBottomLeft;
	OMX_BOOL bBottomRight;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_IMAGE_CONFIG_FOCUSREGION_RESULT;

typedef struct IFM_CONFIG_CAPTUREMODETYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_BOOL bContinuous;
    OMX_BOOL bFrameLimited;
    OMX_U32 nFrameLimit;
} IFM_CONFIG_CAPTUREMODETYPE_CHECK;

typedef struct IFM_CONFIG_CAPTUREMODETYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_BOOL bContinuous;
    OMX_BOOL bFrameLimited;
    OMX_U32 nFrameLimit;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_CAPTUREMODETYPE_RESULT;

typedef struct IFM_CONFIG_RESOLUTIONTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 nWidth;
    OMX_U32 nHeight;
} IFM_CONFIG_RESOLUTIONTYPE_CHECK;

typedef struct IFM_CONFIG_RESOLUTIONTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 nWidth;
    OMX_U32 nHeight;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_RESOLUTIONTYPE_RESULT;

typedef struct IFM_CONFIG_COLORTONETYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U16 eColorToneControl;
} IFM_CONFIG_COLORTONETYPE_CHECK;

typedef struct IFM_CONFIG_COLORTONETYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U16 eColorToneControl;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_COLORTONETYPE_RESULT;

typedef struct IFM_CONFIG_ZOOMFACTORTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 xZoomFactor;
} IFM_CONFIG_ZOOMFACTORTYPE_CHECK;

typedef struct IFM_CONFIG_ZOOMFACTORTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 xZoomFactor;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_ZOOMFACTORTYPE_RESULT;

typedef struct IFM_CONFIG_CLOCK_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 xFrameRate;
	OMX_U32 nToleranceDynamic;
} IFM_CONFIG_CLOCK_CHECK;

typedef struct IFM_CONFIG_CLOCK_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 xFrameRate;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_CLOCK_RESULT;

typedef struct IFM_CONFIG_GAMMATYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_S32 nGamma;
} IFM_CONFIG_GAMMATYPE_CHECK;

typedef struct IFM_CONFIG_GAMMATYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_S32 nGamma;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_GAMMATYPE_RESULT;

typedef struct IFM_CONFIG_EXIF_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
} IFM_CONFIG_EXIF_CHECK;

typedef struct IFM_CONFIG_EXIF_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 nTolerance;
    t_env_test_info eTestInfo;
} IFM_CONFIG_EXIF_RESULT;

typedef struct IFM_CONFIG_STAB_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_S16 nCrop_x;
    OMX_S16 nCrop_y;
    t_stab state;
    OMX_U32 nTolerance;
} IFM_CONFIG_STAB_CHECK;

typedef struct IFM_CONFIG_STAB_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_U32 nTolerance;
    t_env_test_info eTestInfo;
} IFM_CONFIG_STAB_RESULT;

typedef struct IFM_CONFIG_READPETYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_BOOL ePipeEnable;
    OMX_U16 nPEList[IFM_SIZE_LIST_PE_READ];  // PE list to log : 0 field value means list end
    OMX_U32 nPEList_data[IFM_SIZE_LIST_PE_READ];  // PE data list of expected values
} IFM_CONFIG_READPETYPE_CHECK;

typedef struct IFM_CONFIG_READPETYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_BOOL ePipeEnable;
    OMX_U16 nPEList[IFM_SIZE_LIST_PE_READ];  // PE list to log : 0 field value means list end
    OMX_U32 nPEList_data[IFM_SIZE_LIST_PE_READ];  // PE data list of expected values
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_READPETYPE_RESULT;

typedef enum t_latency_estimation{
	Latency_Unknown=0,
	Latency_CameraOff_VfOn,
	Latency_ShotToSnapShot,
	Latency_ShotToShot,
	Latency_MAX = 0x7FFFFFFF
}t_latency_estimation;

typedef struct IFM_CONFIG_LATENCY_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    t_latency_estimation eLatency;
    OMX_U32 bValue;
} IFM_CONFIG_LATENCY_CHECK;

typedef struct IFM_CONFIG_LATENCY_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    t_latency_estimation eLatency;
    OMX_U32 bValue;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_LATENCY_RESULT;

typedef struct IFM_CONFIG_SAVEIMAGE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_S32 Index;
} IFM_CONFIG_SAVEIMAGE;

typedef struct IFM_CONFIG_STOPMEMCOPY {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bStop;
} IFM_CONFIG_STOPMEMCOPY;

typedef enum t_path_type{
	path_Unknown=0,
	path_BackupImage,
	path_ConfigFileMetric,
	path_SensorName,
	path_NbMax,
	path_MAX = 0x7FFFFFFF
}t_path_type;

typedef struct IFM_CONFIG_SETPATH {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    t_path_type ePathType;
    OMX_U8 contentURI[1];
} IFM_CONFIG_SETPATH;


typedef struct IFM_CONFIG_ROTATIONTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_S32 nRotation;
} IFM_CONFIG_ROTATIONTYPE_CHECK;

typedef struct IFM_CONFIG_ROTATIONTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_S32 nRotation;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_ROTATIONTYPE_RESULT;

typedef struct IFM_CONFIG_MIRRORTYPE_CHECK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_MIRRORTYPE  eMirror;
} IFM_CONFIG_MIRRORTYPE_CHECK;

typedef struct IFM_CONFIG_MIRRORTYPE_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    OMX_MIRRORTYPE  eMirror;
	OMX_U32 nTolerance;
	t_env_test_info eTestInfo;
} IFM_CONFIG_MIRRORTYPE_RESULT;

typedef enum t_param_validation{
	Param_Unknown=0,
	Param_nBufferCountActual,
	Param_width,
	Param_height,
	Param_stride,
	Param_colorformat,
	Param_frameCounter,
	Param_MAX = 0x7FFFFFFF
}t_param_validation;

typedef struct IFM_CONFIG_PARAMETER_VALIDATION_RESULT {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bResult;
    t_param_validation  eParamValid;
	OMX_BOOL bTestOnExpectedValue;
	OMX_S32 nExpectedValue;
	OMX_U32 nTolerance;
	OMX_S32 nNotExpectedValue;
} IFM_CONFIG_PARAMETER_VALIDATION_RESULT;

typedef enum t_set_setting {
	Set_Unknown=0,
	Set_frameCounter,
	Set_MAX = 0x7FFFFFFF
}t_set_setting;

typedef struct IFM_CONFIG_SETTING {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    t_set_setting  eSetting;
	OMX_S32 nNewValue;
} IFM_CONFIG_SETTING;


// For ImageProcessor

typedef enum t_param_mirror{
	param_mirror_none= -2,
	param_mirror_x   =  0,
	param_mirror_y   =  1,
	param_mirror_xy  =  -1,
	param_mirror_MAX = 0x7FFFFFFF
}t_param_mirror;

typedef struct IFM_CONFIG_FILTER {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
	OMX_U32 nAlgoType;
	OMX_U32 nAlgoKeyCheck;
	OMX_U32 nParamU32_1;
	OMX_U32 nParamU32_2;
	OMX_U32 nParamU32_3;
	OMX_U32 nParamU32_4;
	OMX_S32 nParamU32_5;
	OMX_S32 nParamU32_6;
	OMX_S32 nParamU32_7;
	OMX_S32 nParamU32_8;
	OMX_U32 nBitMaskParam; // Mask for each param
} IFM_CONFIG_FILTER;

typedef struct IFM_CONFIG_ERROR {
	OMX_U32 nSize;
	OMX_VERSIONTYPE nVersion;
	OMX_U32    nPortIndex;
	OMX_U32    nPortError;
	OMX_BOOL   bClearPortError;
	OMX_STRING cPortErrorString;

	OMX_U32    nComponentError;
	OMX_BOOL   bComponentError;
	OMX_STRING cComponentErrorString;
} IFM_CONFIG_ERROR;

typedef struct IFM_BMS_CAPTURE_CONTEXT {
	OMX_U32 Zoom_Control_s16_CenterOffsetX;
	OMX_U32 Zoom_Control_s16_CenterOffsetY;
	OMX_U32 Zoom_Control_f_SetFOVX;
	OMX_U32 FrameDimensionStatus_u16_MaximumUsableSensorFOVX;
	OMX_U32 FrameDimensionStatus_u16_MaximumUsableSensorFOVY;
	OMX_U32 SensorFrameConstraints_u16_MaxOPXOutputSize;
	OMX_U32 SensorFrameConstraints_u16_MaxOPYOutputSize;
	OMX_U32 SensorFrameConstraints_u16_VTXAddrMin;
	OMX_U32 SensorFrameConstraints_u16_VTYAddrMin;
	OMX_U32 SensorFrameConstraints_u16_VTXAddrMax;
	OMX_U32 SensorFrameConstraints_u16_VTYAddrMax;
	OMX_U32 CurrentFrameDimension_f_PreScaleFactor;
	OMX_U32 CurrentFrameDimension_u16_VTXAddrStart;
	OMX_U32 CurrentFrameDimension_u16_VTYAddrStart;
	OMX_U32 CurrentFrameDimension_u16_VTXAddrEnd;
	OMX_U32 CurrentFrameDimension_u16_VTYAddrEnd;
	OMX_U32 PipeStatus_0_u8_FramesStreamedOutOfPipeLastRun;
	OMX_U32 PipeStatus_1_u8_FramesStreamedOutOfPipeLastRun;
	OMX_U32 CurrentFrameDimension_u8_NumberOfStatusLines;
	OMX_U32 SystemConfig_Status_e_PixelOrder;
	OMX_BOOL TimeNudgeEnabled;
} IFM_BMS_CAPTURE_CONTEXT;

typedef struct IFM_DAMPERS_DATA {
   struct {
       float IntegrationTime;
       float AnalogGain;
       float PostDgMinimumGain;
       float PostDgAverageGain;
       float PostErMinimumGain;
       float PostErAverageGain;
       float PostCsMinimumGain;
       float PostCsAverageGain;
       float NomalizedPixelLevel;
       float NomalizedPixelLevelRed;
       float NomalizedPixelLevelGreen;
       float NomalizedPixelLevelBlue;
       float NomalizedSnr;
       float NomalizedSnrRed;
       float NomalizedSnrGreen;
       float NomalizedSnrBlue;
   } BasesValues;
   struct {
       float AverageDigitalGain;
   } Other;
} IFM_DAMPERS_DATA;

/* Structures for passing data to EXIF builder */
typedef enum {
   IFM_ResolutionUnit_NoUnit,
   IFM_ResolutionUnit_Inch,
   IFM_ResolutionUnit_Centimeter
} IFM_RESOLUTION_UNIT;

typedef enum {
   IFM_YCbCrPositioning_Unknown,
   IFM_YCbCrPositioning_Centered,
   IFM_YCbCrPositioning_CoSited
} IFM_YCbCr_POSITIONING;

typedef enum {
   IFM_ComponentsConfiguration_RGB,
   IFM_ComponentsConfiguration_YCbCr
} IFM_COMPONENTS_CONFIGURATION;

typedef enum {
   IFM_ColorSpace_Uncalibrated,
   IFM_ColorSpace_sRGB
} IFM_COLOR_SPACE;

typedef struct {
	OMX_U16 year;
	OMX_U8  month;
	OMX_U8  day;
	OMX_U8  hour;
	OMX_U8  minutes;
	OMX_U8  seconds;
} IFM_DATE_TIME;

typedef struct IFM_EXIF_SETTINGS {
	const char*                       pManufacturer;
	const char*                       pModel;
	const char*			              pSoftware;
	IFM_DATE_TIME                     sDateTime;
	OMX_SYMBIAN_ORIENTATIONORIGINTYPE eOrientation;
	OMX_U32                           nXResolution;
	OMX_U32                           nYResolution;
	IFM_RESOLUTION_UNIT               eResolutionUnit;
	IFM_YCbCr_POSITIONING             eYCbCrPositioning;
	OMX_U32                           nSensitivity;
	IFM_COMPONENTS_CONFIGURATION      eComponentsConfig;
	OMX_U32                           nPictureNumPixelsX;
	OMX_U32                           nPictureNumPixelsY;
	OMX_WHITEBALCONTROLTYPE           eWhiteBalanceControl;
	float                             fFocalLength;
	OMX_BOOL                          bPictureEnhanced;
	OMX_SYMBIAN_SCENEMODETYPE         eSceneType;
	OMX_METERINGTYPE                  eMeteringMode;
	OMX_BOOL                          bFlashFired;
	OMX_IMAGE_FLASHCONTROLTYPE        eFlashControl;
	OMX_BOOL                          bFlashPresent;
	OMX_U16                           nMaxNumPixelsX;
	OMX_U32                           nExposureTimeUsec;
	OMX_U32                           xFNumber;
	OMX_U32                           nBrightness;
	OMX_U32                           nSubjectDistance;
	OMX_S32                           nEvCompensation;
	OMX_EXPOSURECONTROLTYPE           eExposureControl;
	OMX_BOOL                          bBracketingEnabled;
	OMX_S32                           xAnalogGain;
	OMX_S32                           xDigitalGain;
	OMX_S32                           nContrast;
	OMX_SYMBIAN_RECTTYPE              sAutoFocusROI;
	const void*                       pMakerNote;
	OMX_U32                           nMakerNoteSize;
	IFM_COLOR_SPACE                   eColorSpace;
	OMX_U32			nZoomFactor;
} IFM_EXIF_SETTINGS;
/*
 * enums for Android product
 */
#define OMX_STE_CAMERA_EXTENSIONS_START_OFFSET 0x00010000

typedef enum OMX_STE_IMAGEFILTERTYPE_EXT{ // Extending OMX_IMAGEFILTERTYPE
    // Khronos extension 1.1.2, §3.3.5 page 11
    OMX_STE_ImageFilterWatercolor = OMX_ImageFilterVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_STE_ImageFilterPastel,
    OMX_STE_ImageFilterFilm,
    OMX_STE_ImageFilterGrayscaleNegative,
    OMX_STE_ImageFilterPosterize,
    OMX_ImageFilterBeach,
    OMX_ImageFilterSnow,
    OMX_STE_ImageFilterMax = 0x7FFFFFFF
}OMX_STE_IMAGEFILTERTYPE_EXT;
typedef enum OMX_STE_WHITEBALCONTROLTYPE_EXT{ // Extending OMX_WHITEBALCONTROLTYPE
    // Khronos extension 1.1.2, §3.5.5 page 25
    OMX_STE_WhiteBalControlFacePriorityMode = OMX_WhiteBalControlVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_STE_WhiteBalControlMax = 0x7FFFFFFF
}OMX_STE_WHITEBALCONTROLTYPE_EXT;
typedef enum OMX_STE_EXPOSURECONTROLTYPE_EXT{ // Extending OMX_EXPOSURECONTROLTYPE
    // Khronos extension 1.1.2, §3.5.6 page 26
    OMX_STE_ExposureControlFacePriorityMode = OMX_ExposureControlVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_STE_ExposureControlMax = 0x7FFFFFFF
}OMX_STE_EXPOSURECONTROLTYPE_EXT;
typedef enum OMX_STE_SCENEMODETYPE_EXT{ // Extending OMX_SYMBIAN_SCENEMODETYPE (defined in the SHAI)
    OMX_STE_Scene_Document = OMX_SYMBIAN_SceneVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_STE_Scene_Beach,
    OMX_STE_Scene_Snow,
    OMX_STE_Scene_Party,
    OMX_STE_Scene_Aqua,
    OMX_STE_Scene_Candlelight,
    OMX_STE_Scene_Backlight,
    OMX_STE_Scene_Sunset,
    OMX_STE_Scene_HDR,
    OMX_STE_Scene_Max = 0x7FFFFFFF
}OMX_STE_SCENEMODETYPE_EXT;
typedef enum OMX_STE_METERINGTYPE_EXT{ // Extending OMX_METERINGTYPE
    OMX_STE_MeteringModeCenterWeighted = OMX_MeteringVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_STE_MeteringModeBeachAndSnow,
    OMX_STE_MeteringModeScenery,
    OMX_STE_MeteringModePortrait,
    OMX_STE_MeteringModeMax = 0x7FFFFFFF
}OMX_STE_METERINGTYPE_EXT;
typedef enum OMX_STE_FOCUSREGIONCONTROL_EXT{ // Extending OMX_SYMBIAN_FOCUSREGIONCONTROL (defined in the SHAI)
    OMX_STE_FocusRegionCenter = OMX_SYMBIAN_FocusRegionVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
	OMX_STE_FocusRegionTouchPriority,
	OMX_STE_FocusRegion_Max = 0x7FFFFFFF
}OMX_STE_FOCUSREGIONCONTROL_EXT;
typedef enum OMX_STE_ROIOBJECTTYPE_EXT{ // Extending OMX_SYMBIAN_ROIOBJECTTYPE  (defined in the SHAI)
    OMX_STE_RoiObjectTypeTouch  = OMX_SYMBIAN_RoiObjectVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_STE_RoiObjectMax = 0x7FFFFFFF
}OMX_STE_ROIOBJECTTYPE_EXT;
typedef enum OMX_STE_LOCKTYPE_EXT{ // Extending OMX_SYMBIAN_LOCKTYPE  (defined in the SHAI)
    OMX_STE_LockLensBestPosition = OMX_SYMBIAN_LockVendorStartUnused + OMX_STE_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_STE_Lock_Max = 0x7FFFFFFF
}OMX_STE_LOCKTYPE_EXT;

//Added for flash production test
typedef struct OMX_STE_PRODUCTIONTESTTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex; //Not needed
    OMX_U32 nIlluminanceValue;
    OMX_U32 nFlashChromaPointX;  //1st co-ordinate
    OMX_U32 nFlashChromaPointY;  //2nd co-ordinate
} OMX_STE_PRODUCTIONTESTTYPE;

// IQT
typedef struct IFM_CONFIG_IQTCAPTUREMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nSaveMode;	/**< First Frames , FIFO, Normal*/
    OMX_U32 bSaveFrameCount;	/**< Number of frames to save */
    OMX_U32 nSaveFirstFrame;	/**< First Frame to save */
    OMX_BOOL bSaveFrameByFrame; /**< Saving Frame by Frame */
    OMX_U32 nFrameSkippingNumber; /**< Frame Skipping Number */
    OMX_U32 nInstanceNumber;	/**< Instantiation management */
    OMX_U32 nAcknowledge;	/**< Acknowledge management */
} IFM_CONFIG_IQTCAPTUREMODETYPE;

typedef struct OMX_CONFIG_EXPOSURE_ZONE_TYPE {
    float           left;             //              starting x position
    float           top;              //              starting y position
    float           width;            //              x size
    float           height;           //              y size
    float           weight;
}OMX_CONFIG_EXPOSURE_ZONE_TYPE;

//Added for multiple region support for exposure
typedef struct IFM_CONFIG_EXPOSURE_REGION {
    OMX_U32 nSize;
   OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nZoneCount;
    OMX_CONFIG_EXPOSURE_ZONE_TYPE sExposureZones[OMX_SYMBIAN_MAX_NUMBER_OF_ROIS];
}IFM_CONFIG_EXPOSURE_REGION;

//Added for measuring latencies
typedef struct IFM_TIME
{
	OMX_U64 days;
	OMX_U64 h;
	OMX_U64 min;
	OMX_U64 s;
	OMX_U64 us;
}IFM_TIME;

//Added for measuring latencies
typedef struct IFM_TIMESTAMP
{
	IFM_TIME t0;
	IFM_TIME t1;
}IFM_TIMESTAMP;

//Added for measuring latencies in hsmcamera
typedef struct IFM_LATENCY_HSMCAMERA {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	IFM_TIMESTAMP e_OMXCAM_ShotToShot;
	IFM_TIMESTAMP e_OMXCAM_ShotToSnapshotStart;
	IFM_TIMESTAMP e_OMXCAM_VFOffToOn;
	IFM_TIMESTAMP e_OMXCAM_ShotToSave_Start;
	IFM_TIMESTAMP e_OMXCAM_TotalShutterLag;
	IFM_TIMESTAMP e_OMXCAM_AF;
	IFM_TIMESTAMP e_OMXCAM_SerialShooting[IFM_MAX_SIZE_SERIALSHOOTING];
	IFM_TIMESTAMP e_OMXCAM_ShotToVF;
	IFM_TIMESTAMP e_OMXCAM_VPB0FrameRate;
	IFM_TIMESTAMP e_OMXCAM_VPB1FrameRate;
	IFM_TIMESTAMP e_OMXCAM_VPB2FrameRate;
	IFM_TIMESTAMP e_OMXCAM_CapturingBitVPB1;
	IFM_TIMESTAMP e_OMXCAM_StartStreaming;
	IFM_TIMESTAMP e_OMXCAM_StopISP;
	IFM_TIMESTAMP e_OMXCAM_ShotToCapture;
	IFM_TIMESTAMP e_OMXCAM_ShutterReleaseLag;
	OMX_U32 count_SerialShooting ;
}IFM_LATENCY_HSMCAMERA;

//Added for measuring latencies in ispproc
typedef struct IFM_LATENCY_ISPPROC
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	IFM_TIMESTAMP e_OMXISPSNAP_Process;
	IFM_TIMESTAMP e_OMXISPSTILL_Process;
}IFM_LATENCY_ISPPROC;

/* for OMX_STE_IndexConfigFrameSelectRatio */
/* available only in variable framerate on VPB1 */
/* nSelectRatio = 0 : forbidden value */
/* nSelectRatio = 1 : all frames are kept (default value) */
/* nSelectRatio = n : only 1/n frames are kept */
typedef struct OMX_STE_CONFIG_FRAMESELECTRATIOTYPE{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nSelectRatio;
}OMX_STE_CONFIG_FRAMESELECTRATIOTYPE;


typedef struct OMX_STE_PARAM_FRAMERATERANGE{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;	
    OMX_U32 xFrameRateMin;
    OMX_U32 xFrameRateMax;
}OMX_STE_PARAM_FRAMERATERANGE;



#define OMX_STE_GAMMA_LUT_SAMPLE_COUNT 129 
/* Structure added to store 3 channel gamma information */
typedef struct OMX_STE_CAPTUREPARAMETERSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex; //Not needed
    OMX_BOOL bValidGamma;
    OMX_BOOL bValidSFX;
    OMX_BOOL bValidEffect;
    OMX_U32 nRedGammaLUT[OMX_STE_GAMMA_LUT_SAMPLE_COUNT];
    OMX_U32 nGreenGammaLUT[OMX_STE_GAMMA_LUT_SAMPLE_COUNT];
    OMX_U32 nBlueGammaLUT[OMX_STE_GAMMA_LUT_SAMPLE_COUNT];
    OMX_S32 nColorMatrix[9];
    OMX_S32 nOffsetR;
    OMX_S32 nOffsetG;
    OMX_S32 nOffsetB;
    OMX_S32 nSFXSolarisControl;         // NB: Negative logic; same semantics as the xp70 FW PE
    OMX_S32 nSFXNegativeControl;        // NB: Negative logic; same semantics as the xp70 FW PE
    OMX_U8 uAdsocPKEmbossEffectCtrl;
    OMX_U8 uAdsocPKFlipperEffectCtrl;
    OMX_U8 uAdsocPKGrayBackEffectCtrl;
    OMX_U32 nFocalLength;
} OMX_STE_CAPTUREPARAMETERSTYPE;

// Analyzers Feedbacks config related defines
#define IFM_CONFIG_FEEDBACK_SIZE(s) (sizeof(OMX_SYMBIAN_CONFIG_FEEDBACKTYPE) - sizeof(OMX_U8) + (s))
#define IFM_CONFIG_OPAQUE_FEEDBACK_SIZE 1024
#define IFM_MAX_NUMBER_OF_OPAQUE_FEEDBACKS 10

// Face Tracker feedback config container
typedef union IFM_CONFIG_FACETRACKER_FEEDBACKTYPE {
    OMX_SYMBIAN_CONFIG_FEEDBACKTYPE sFeedback;
    OMX_U8 cContainer[IFM_CONFIG_FEEDBACK_SIZE(sizeof(OMX_SYMBIAN_ROITYPE))];
} IFM_CONFIG_FACETRACKER_FEEDBACKTYPE;

// Object Tracker feedback config container
typedef union IFM_CONFIG_OBJECTTRACKER_FEEDBACKTYPE {
    OMX_SYMBIAN_CONFIG_FEEDBACKTYPE sFeedback;
    OMX_U8 cContainer[IFM_CONFIG_FEEDBACK_SIZE(sizeof(OMX_SYMBIAN_ROITYPE))];
} IFM_CONFIG_OBJECTTRACKER_FEEDBACKTYPE;

// Motion Level feedback config container
typedef union IFM_CONFIG_MOTIONLEVEL_FEEDBACKTYPE {
    OMX_SYMBIAN_CONFIG_FEEDBACKTYPE sFeedback;
    OMX_U8 cContainer[IFM_CONFIG_FEEDBACK_SIZE(sizeof(OMX_U32))];
} IFM_CONFIG_MOTIONLEVEL_FEEDBACKTYPE;

// Scene Detection feedback config container
typedef union IFM_CONFIG_SCENEDETECTION_FEEDBACKTYPE {
    OMX_SYMBIAN_CONFIG_FEEDBACKTYPE sFeedback;
    OMX_U8 cContainer[IFM_CONFIG_FEEDBACK_SIZE(sizeof(OMX_SYMBIAN_SCENEMODETYPE))];
} IFM_CONFIG_SCENEDETECTION_FEEDBACKTYPE;

// Opaque feedback config container
typedef union IFM_CONFIG_OPAQUE_FEEDBACKTYPE {
    OMX_SYMBIAN_CONFIG_FEEDBACKTYPE sFeedback;
    OMX_U8 cContainer[IFM_CONFIG_FEEDBACK_SIZE(IFM_CONFIG_OPAQUE_FEEDBACK_SIZE)];
} IFM_CONFIG_OPAQUE_FEEDBACKTYPE;

//typedef for smooth zoom callback pEventData structure
typedef struct OMX_STE_SMOOTH_ZOOM_RETURNTYPE {
	OMX_U32		cCurrentZoomFactor;
	OMX_ERRORTYPE	smoothZoomError;
}OMX_STE_SMOOTH_ZOOM_RETURNTYPE;

// typedef for focal length
typedef struct OMX_STE_CONFIG_LENSPARAMETERTYPE {
    OMX_U32 nSize;                     /*Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;          /* OMX specification version information */
    OMX_U32 nFocalLength;              /* Focal length in Q16 format */
    OMX_U32 nHorizontolViewAngle;      /* HorizontolViewAngle */
    OMX_U32 nVerticalViewAngle;        /* VerticalViewAngle */
}OMX_STE_CONFIG_LENSPARAMETERTYPE ;

typedef struct OMX_STE_CONFIG_PICTURESIZETYPE {
    OMX_U32 nSize;                     /*Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;          /* OMX specification version information */
    OMX_U32 modenumber;              /* modenumber */
    OMX_U32 nOutputResWidth;
    OMX_U32 nOutputResHeight;    
}OMX_STE_CONFIG_PICTURESIZETYPE ;
#ifdef FREQ_SCALING_ALLOWED

#define TRESOURCEID_MAX 3 //based on enum TResourseId in file omxilosalservices.h
typedef struct IFM_CONFIG_SCALING_MODETYPE {
    OMX_U32 nSize;                         /*Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;              /* OMX specification version information */
    OMXIL_OSAL_RESOUCE_ID sResource;       /* Resource ID defined in omxilosalservices.h*/
    OMX_U32 scalingFactor[TRESOURCEID_MAX];  /* Frequency scaling factor */

} IFM_CONFIG_SCALING_MODETYPE ;
#endif //FREQ_SCALING_ALLOWED

//ZSLHDR related state enum
    typedef enum eZSLHDR_State {
        ZSLHDRState_Invalid = -1,
        ZSLHDRState_StartHDR,
        ZSLHDRState_StopHiddenCapture,
        ZSLHDRState_HiddenCaptureStopped,
        ZSLHDRState_ReadyForCapture,
        ZSLHDRState_StopPreview,
        ZSLHDRState_PreviewStopped,
        ZSLHDRState_TakeCapture,
        ZSLHDRState_CaptureFinished,
        ZSLHDRState_ResumePreview,
        ZSLHDRState_StopHDR
    }eZSLHDR_State;

#endif // _IFM_TYPES_H_
