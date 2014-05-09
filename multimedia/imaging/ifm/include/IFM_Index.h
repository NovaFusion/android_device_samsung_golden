/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _IFM_INDEX_H_
#define _IFM_INDEX_H_

#include "ENS_Index.h"

#define OMX_STE_INDEX_CONFIG_FRAMESELECTRATIO_NAME       "OMX.STE.Index.Config.FrameSelectRatio"
#define OMX_STE_INDEX_PARAM_FRAMERATERANGE_NAME       "OMX.STE.Index.Param.FrameRateRange"
#define OMX_STE_INDEX_CONFIG_COMMON_ZOOMMODE_NAME	"OMX.STE.Index.Config.Common.ZoomMode"
#define OMX_STE_INDEX_CONFIG_COMMON_LENSPARAMETER   "OMX.STE.Index.Config.Common.LensParameters"
#ifdef FREQ_SCALING_ALLOWED
#define OMX_STE_INDEX_CONFIG_COMMON_SCALING         "OMX.STE.Index.Config.Common.Scaling"
#endif //FREQ_SCALING_ALLOWED
#define OMX_STE_INDEX_CONFIG_METERINGON_NAME   "OMX.STE.Index.Config.MeteringOn"
#define OMX_STE_INDEX_CONFIG_EXIF_TIMESTAMP_NAME "OMX.STE.Index.Config.Exif.Timestamp"
#define OMX_STE_INDEX_CONFIG_PICTURE_SIZE   "OMX.STE.Index.Config.Picture.Size"


/** The IFM_INDEXTYPE enumeration is used to select a structure when either
 *  getting or setting parameters and/or configuration data.  Each entry in
 *  this enumeration maps to an IFM specified structure.
 */
typedef enum IFM_INDEXTYPE {

    IFM_IndexStartExtensions = ENS_IndexIFMStartUnused,
    IFM_IndexConfigCommonReadPe,                /**< reference: IFM_CONFIG_READPETYPE */
    IFM_IndexConfigCommonWritePe,               /**< reference: IFM_CONFIG_WRITEPETYPE */
    IFM_IndexConfigCommonOpMode_Check,          /**< reference: IFM_CONFIG_OPMODE_CHECK */
    IFM_IndexConfigCheckAlive,                  /**< reference: IFM_CONFIG_CHECK_ALIVE */

    /* --pure Camera ones*/
    IFM_IndexIsISPShared,                       /**< reference: IFM_CONFIG_ISPSHARED */
    IFM_IndexConfigTestMode,                    /**< reference: OMX_STE_CONFIG_SENSORTESTTYPE */
    IFM_IndexConfigSensorAlignment,             /**< reference: IFM_CONFIG_SENSOR_ALIGNMENT */


    IFM_IndexStartDisplaySink = (IFM_IndexStartExtensions+100),
    IFM_IndexConfigDisplaysink,                 /**< reference: IFM_CONFIG_DISPLAYSINK */

    IFM_IndexStartImgFilter = (IFM_IndexStartExtensions+200),
    IFM_IndexConfigCommonImageFilter_Check,     /**< reference: IFM_CONFIG_IMAGEFILTERTYPE_CHECK */
    IFM_IndexConfigCommonImageFilter_Result,    /**< reference: IFM_CONFIG_IMAGEFILTERTYPE_RESULT */
    IFM_IndexConfigCommonWhiteBalance_Check,    /**< reference: IFM_CONFIG_WHITEBALANCETYPE_CHECK */
    IFM_IndexConfigCommonWhiteBalance_Result,   /**< reference: IFM_CONFIG_WHITEBALANCETYPE_RESULT */
    IFM_IndexConfigCommonGamma_Check,           /**< reference: IFM_CONFIG_GAMMATYPE_CHECK */
    IFM_IndexConfigCommonGamma_Result,          /**< reference: IFM_CONFIG_GAMMATYPE_RESULT */
    IFM_IndexConfigCommonBrightness_Check,      /**< reference: IFM_CONFIG_BRIGHTNESSTYPE_CHECK */
    IFM_IndexConfigCommonBrightness_Result,     /**< reference: IFM_CONFIG_BRIGHTNESSTYPE_RESULT */
    IFM_IndexConfigCommonContrast_Check,        /**< reference: IFM_CONFIG_CONTRASTTYPE_CHECK */
    IFM_IndexConfigCommonContrast_Result,       /**< reference: IFM_CONFIG_CONTRASTTYPE_RESULT */
    IFM_IndexConfigCommonSaturation_Check,      /**< reference: IFM_CONFIG_SATURATIONTYPE_CHECK */
    IFM_IndexConfigCommonSaturation_Result,     /**< reference: IFM_CONFIG_SATURATIONTYPE_RESULT*/
    IFM_IndexConfigCommonExposure_Check,        /**< reference: IFM_CONFIG_EXPOSURECONTROLTYPE_CHECK */
    IFM_IndexConfigCommonExposure_Result,       /**< reference: IFM_CONFIG_EXPOSURECONTROLTYPE_RESULT */
    IFM_IndexConfigCommonExposureValue_Check,   /**< reference: IFM_CONFIG_EXPOSUREVALUETYPE_CHECK */
    IFM_IndexConfigCommonExposureValue_Result,  /**< reference: IFM_CONFIG_EXPOSUREVALUETYPE_RESULT */
    IFM_IndexConfigCommonScale_Check,           /**< reference: IFM_CONFIG_SCALEFACTORTYPE_CHECK */
    IFM_IndexConfigCommonScale_Result,          /**< reference: IFM_CONFIG_SCALEFACTORTYPE_RESULT */
    IFM_IndexConfigExtDigitalZoom_Check,        /**< reference: IFM_CONFIG_ZOOMFACTORTYPE_CHECK */
    IFM_IndexConfigExtDigitalZoom_Result,       /**< reference: IFM_CONFIG_ZOOMFACTORTYPE_RESULT */
    IFM_IndexConfigFocusControl_Check,          /**< reference: IFM_IMAGE_CONFIG_FOCUSCONTROLTYPE_CHECK */
    IFM_IndexConfigFocusControl_Result,         /**< reference: IFM_IMAGE_CONFIG_FOCUSCONTROLTYPE_RESULT */
    IFM_IndexConfigCommonFocusRegion_Check,     /**< reference: IFM_IMAGE_CONFIG_FOCUSREGION_CHECK */
    IFM_IndexConfigCommonFocusRegion_Result,    /**< reference: IFM_IMAGE_CONFIG_FOCUSREGION_RESULT */
    IFM_IndexParamPortDefinition_Check,         /**< reference: IFM_PARAM_PORTDEFINITION_CHECK */
    IFM_IndexParamPortDefinition_Result,        /**< reference: IFM_PARAM_PORTDEFINITION_RESULT */
    IFM_IndexConfigCommonFormat_Check,          /**< reference: IFM_CONFIG_FORMATTYPE_CHECK */
    IFM_IndexConfigCommonFormat_Result,         /**< reference: IFM_CONFIG_FORMATTYPE_RESULT */
    IFM_IndexConfigCaptureMode_Check,           /**< reference: IFM_CONFIG_CAPTUREMODETYPE_CHECK */
    IFM_IndexConfigCaptureMode_Result,          /**< reference: IFM_CONFIG_CAPTUREMODETYPE_RESULT */
    IFM_IndexConfigCommonResolution_Check,      /**< reference: IFM_CONFIG_RESOLUTIONTYPE_CHECK */
    IFM_IndexConfigCommonResolution_Result,     /**< reference: IFM_CONFIG_RESOLUTIONTYPE_RESULT */
    IFM_IndexConfigCommonColorTone_Check,       /**< reference: IFM_CONFIG_COLORTONETYPE_CHECK */
    IFM_IndexConfigCommonColorTone_Result,      /**< reference: IFM_CONFIG_COLORTONETYPE_RESULT */
    IFM_IndexConfigClock_Check,                 /**< reference: IFM_CONFIG_CLOCK_CHECK */
    IFM_IndexConfigClock_Result,                /**< reference: IFM_CONFIG_CLOCK_RESULT */
    IFM_IndexExif_Check,                        /**< reference: IFM_CONFIG_EXIF_CHECK */
    IFM_IndexExif_Result,                       /**< reference: IFM_CONFIG_EXIF_RESULT */
    IFM_IndexConfigCommonReadPe_Check,          /**< reference: IFM_CONFIG_READPETYPE_CHECK */
    IFM_IndexConfigCommonReadPe_Result,         /**< reference: IFM_CONFIG_READPETYPE_RESULT */
    IFM_IndexConfigLatency_Check,               /**< reference: IFM_CONFIG_LATENCY_CHECK */
    IFM_IndexConfigLatency_Result,              /**< reference: IFM_CONFIG_LATENCY_RESULT */
    IFM_IndexConfigCommonSaveImage,             /**< reference: IFM_CONFIG_SAVEIMAGE */
    IFM_IndexConfigStopMemCopy,                 /**< reference: IFM_CONFIG_STOPMEMCOPY */
    IFM_IndexConfigSetPath,                     /**< reference: IFM_CONFIG_SETPATH */
    IFM_IndexConfigCommonRotate_Check,          /**< reference: IFM_CONFIG_ROTATIONTYPE_CHECK */
    IFM_IndexConfigCommonRotate_Result,         /**< reference: IFM_CONFIG_ROTATIONTYPE_RESULT */
    IFM_IndexConfigCommonMirror_Check,          /**< reference: IFM_CONFIG_MIRRORTYPE_CHECK */
    IFM_IndexConfigCommonMirror_Result,         /**< reference: IFM_CONFIG_MIRRORTYPE_RESULT */
    IFM_IndexConfigCommonColorEnhancement_Check,/**< reference: IFM_CONFIG_COLORENHANCEMENTTYPE_CHECK */
    IFM_IndexConfigCommonColorEnhancement_Result, /**< reference: IFM_CONFIG_COLORENHANCEMENTTYPE_RESULT */
    IFM_IndexConfigParameterValidation_Result,  /**< reference: IFM_CONFIG_PARAMETER_VALIDATION_RESULT */
    IFM_IndexConfigSetting,                     /**< reference: IFM_CONFIG_SETTING */
    IFM_IndexStab_Check,                        /**< reference: IFM_CONFIG_STAB_CHECK */
    IFM_IndexStab_Result,                       /**< reference: IFM_CONFIG_STAB_RESULT */
    IFM_IndexParamFirmwareTrace,                /**< reference: IFM_PARAM_FIRMWARETRACETYPE */
    IFM_IndexConfigExposureRegion,              /**< reference: IFM_CONFIG_EXPOSURE_REGION */

    IFM_IndexStartImageProcessor = (IFM_IndexStartExtensions+300),
    IFM_IndexConfigCommonFilter,                /**< reference: IFM_CONFIG_FILTER */
    IFM_IndexConfigError,                       /**< reference: IFM_CONFIG_ERROR */
    IFM_IndexConfigNorcosEnabling,              /**< reference: IFM_CONFIG_NORCOS_ENABLING */
    IFM_IndexConfigNorcosTuning,                /**< reference: IFM_CONFIG_NORCOS_TUNNING */
    IFM_IndexConfigNorcosControlMode,           /**< reference: IFM_CONFIG_NORCOS_CONTROLMODE */
    IFM_IndexConfigREDThreshold,                /**< reference: IFM_CONFIG_RED_THRESHOLD */
    IFM_IndexConfigASDThreshold,                /**< reference: IFM_CONFIG_ASD_THRESHOLDTYPE */

    IFM_IndexStart3A = (IFM_IndexStartExtensions+400),
    IFM_IndexConfig3ATraceDumpControl,          /**< reference: IFM_CONFIG_3A_TRACEDUMPCONTROL */

	IFM_IndexStartIQT = (IFM_IndexStartExtensions+500),
	IFM_IndexParamFakesinkPathname,             /**< reference: OMX_PARAM_CONTENTURITYPE */
	IFM_IndexConfigIqtCaptureMode,              /**< reference: IFM_CONFIG_IQTCAPTUREMODETYPE */
	IFM_IndexLatency_Hsmcamera,                 /**< reference: IFM_LATENCY_HSMCAMERA */
	IFM_IndexLatency_Ispproc,                   /**< reference: IFM_LATENCY_ISPPROC */

    IFM_IndexStartOmxExtensions = (IFM_IndexStartExtensions+600),
    OMX_Symbian_IndexConfigSupportedResolutions,        /**< reference: OMX_SYMBIAN_CONFIG_SUPPORTEDRESOLUTIONS */
    OMX_Symbian_IndexParamDeviceMakeModel,              /**< reference: OMX_SYMBIAN_PARAM_DEVICEMAKEMODEL */
    OMX_Symbian_IndexConfig_AutoRotationControl,        /**< reference: OMX_SYMBIAN_CONFIG_BOOLEANTYPE */
    OMX_Symbian_IndexConfigSensorNvmData,               /**< reference: OMX_SYMBIAN_CONFIG_SENSORNVMDATATYPE */
    OMX_Symbian_IndexParamColorPrimary,                 /**< reference: OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE */
    OMX_STE_IndexConfigFrameSelectRatio,                /**< reference: OMX_STE_CONFIG_FRAMESELECTRATIOTYPE */
    OMX_STE_IndexParamFrameRateRange,                /**< reference: OMX_STE_PARAM_FRAMERATERANGE */
    OMX_Symbian_IndexConfigAnalyzerFeedback,            /**< reference: OMX_SYMBIAN_CONFIG_FEEDBACKTYPE */
    OMX_STE_IndexConfigCommonZoomMode,                  /**< reference: OMX_CONFIG_BOOLEANTYPE*/
    IFM_IndexConfigSoftware,                            /**< reference: IFM_CONFIG_SOFTWAREVERSION*/
    OMX_STE_IndexConfigCommonLensParameters,            /**< reference: OMX_STE_CONFIG_LENSPARAMETERTYPE*/
    OMX_STE_IndexConfigMeteringOn,                       /**< reference: OMX_CONFIG_BOOLEANTYPE*/
    OMX_STE_IndexConfigPictureSize,				        /**< reference: OMX_STE_CONFIG_PICTURESIZETYPE*/    
#ifdef FREQ_SCALING_ALLOWED
    OMX_STE_IndexConfigCommonScaling,                   /**< reference: IFM_CONFIG_SCALING_MODETYPE*/
#endif //FREQ_SCALING_ALLOWED
    OMX_STE_IndexConfigExtradata,                       /**< reference: OMX_SYMBIAN_CONFIG_EXIFSETTINGSTYPE*/
    OMX_STE_IndexConfigExifTimestamp, /**< reference: OMX_TIME_CONFIG_TIMESTAMPTYPE */
    IFM_IndexStartImgfilter = (IFM_IndexStartExtensions+700)

} IFM_INDEXTYPE;

#endif // _IFM_INDEX_H_

