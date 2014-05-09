/*
  OMX_Symbian_IndexExt.h

  Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
  All rights reserved.

  This program and the accompanying materials are made available
  under the terms of the Eclipse Public License v1.0 which accompanies
  this distribution, and is available at
  http://www.eclipse.org/legal/epl-v10.html

  Initial Contributors:
  Nokia Corporation - initial contribution.
*/

/**
 *  Version 1
 *  First version with version numbering
 */
#define OMX_SYMBIAN_CAMERAINDEXEXT_VERSION 1

/** @file
@brief Draft Symbian OpenMAX IL Extension Indices.

This header file lists draft Symbian extensions to OpenMAX IL.  The indices here
should not be used by IL client software.  IL clients should always query
the extension index by calling OMX_GetExtensionIndex() with the appropriate
string.  These indices should always be used by components implementing draft Symbian
specific extensions.  Binary compatibility between different versions of this
file is not guaranteed.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_CameraIndexExt_h
#define OMX_Symbian_CameraIndexExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully
 */
#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Index.h>
#include <OMX_Symbian_IVCommonExt_Ste.h>
#else
#include <OMX_Index.h>
#include <OMX_Symbian_IVCommonExt_Ste.h>
#endif



/** Symbian camera extension indices.

This enum lists the current Symbian camera extension indices to OpenMAX IL. This list
should only be used by component implementations written to comply to the
Symbian Camera Chipset API. IL Clients should always get the index directly from the
component using OMX_GetExtensionIndex() with the strings defined in the API
documentation.
*/
typedef enum OMX_SYMBIAN_INDEXTYPE {

    /* Image & Video common configurations */
    OMX_Symbian_IndexCommonStartUnused = OMX_IndexVendorStartUnused + OMX_SYMBIAN_IV_COMMON_EXTENSIONS_START_OFFSET,
    OMX_Symbian_IndexConfigCommonExtCapturing,   /**< reference: OMX_SYMBIAN_CONFIG_BOOLEANTYPE */
    OMX_Symbian_IndexConfigCommonExtraVideoStabilizationData,

    /* Camera configurations */
    OMX_Symbian_IndexCameraStartUnused = OMX_IndexVendorStartUnused + OMX_SYMBIAN_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_Symbian_IndexConfigCameraFuncTestSelect,
    OMX_Symbian_IndexConfigCameraSelfTestSelect, /**< reference: OMX_SYMBIAN_CONFIG_CAMERASELFTESTSELECTTYPE */
    OMX_Symbian_IndexConfigCameraTesting,        /**< reference: OMX_CONFIG_BOOLEANTYPE */
    OMX_Symbian_IndexConfigImageMaxJpegSize,
    OMX_Symbian_IndexConfigSceneMode,            /**< reference: OMX_SYMBIAN_CONFIG_SCENEMODETYPE*/
    OMX_Symbian_IndexConfigRGBHistogram,         /**< reference: OMX_SYMBIAN_CONFIG_RGBHISTOGRAM */
    OMX_Symbian_IndexConfigHistogramControl,     /**< reference: OMX_SYMBIAN_CONFIG_HISTOGRAMCONTROLTYPE */
    OMX_Symbian_IndexConfigXenonLifeCounter,     /**< reference: OMX_SYMBIAN_CONFIG_S32TYPE */
    OMX_Symbian_IndexConfigROISelection,         /**< reference: OMX_SYMBIAN_CONFIG_ROISELECTIONTYPE */
    OMX_Symbian_IndexConfigREDComplexity,
    OMX_Symbian_IndexConfigRedEyeRemoval,        /**< reference: OMX_CONFIG_BOOLEANTYPE */
    OMX_Symbian_IndexConfigOrientationScene,     /**< reference: OMX_SYMBIAN_CONFIG_ORIENTATIONTYPE */
    OMX_Symbian_IndexParamRevertAspectRatio,     /**< reference: OMX_CONFIG_BOOLEANTYPE */
    OMX_Symbian_IndexConfigNDFilterControl,      /**< reference: OMX_SYMBIAN_CONFIG_HIGHLEVELCONTROLTYPE */
    OMX_Symbian_IndexConfigExposureLock,         /**< reference: OMX_SYMBIAN_CONFIG_LOCKTYPE */
    OMX_Symbian_IndexConfigWhiteBalanceLock,     /**< reference: OMX_SYMBIAN_CONFIG_LOCKTYPE */
    OMX_Symbian_IndexConfigFocusLock,            /**< reference: OMX_SYMBIAN_CONFIG_LOCKTYPE */
    OMX_Symbian_IndexConfigAllLock,              /**< reference: OMX_SYMBIAN_CONFIG_LOCKTYPE */
    OMX_Symbian_IndexConfigCenterFieldOfView,    /**< reference: OMX_SYMBIAN_CONFIG_POINTTYPE */
    OMX_Symbian_IndexConfigExtDigitalZoom,       /**< reference: OMX_SYMBIAN_CONFIG_ZOOMFACTORTYPE */
    OMX_Symbian_IndexConfigExtOpticalZoom,       /**< reference: OMX_SYMBIAN_CONFIG_ZOOMFACTORTYPE */
    OMX_Symbian_IndexConfigFocusRange,           /**< reference: OMX_SYMBIAN_CONFIG_FOCUSRANGETYPE */
    OMX_Symbian_IndexConfigExtFocusStatus,       /**< reference: OMX_SYMBIAN_CONFIG_EXTFOCUSSTATUSTYPE */
    OMX_Symbian_IndexConfigFlickerRemoval,       /**< reference: OMX_SYMBIAN_CONFIG_FLICKERREMOVALTYPE */
    OMX_Symbian_IndexConfigFlashControl,         /**< reference: OMX_SYMBIAN_CONFIG_FLASHCONTROLTYPE */
    OMX_Symbian_IndexConfigAFAssistantLight,     /**< reference: OMX_SYMBIAN_CONFIG_HIGHLEVELCONTROLTYPE */
    OMX_Symbian_IndexConfigHintPowerVsQuality,
    OMX_Symbian_IndexConfigHintDepthOfField,
    OMX_Symbian_IndexConfigFocusRegion,          /**< reference: OMX_SYMBIAN_CONFIG_FOCUSREGIONTYPE */
    OMX_Symbian_IndexConfigROI,                  /**< reference: OMX_SYMBIAN_CONFIG_ROITYPE */
    OMX_Symbian_IndexConfigCameraSensorInfo,     /**< reference: OMX_SYMBIAN_CONFIG_HWINFOTYPE */
    OMX_Symbian_IndexConfigFlashGunInfo,         /**< reference: OMX_SYMBIAN_CONFIG_HWINFOTYPE */
    OMX_Symbian_IndexConfigMotionLevel,          /**< reference: OMX_SYMBIAN_CONFIG_U32TYPE */
    OMX_Symbian_IndexConfigSharpness,            /**< reference: OMX_SYMBIAN_CONFIG_S32TYPE */
    OMX_Symbian_IndexConfigExtCaptureMode,       /**< reference: OMX_SYMBIAN_CONFIG_EXTCAPTUREMODETYPE */
    OMX_Symbian_IndexConfigXenonFlashStatus,
    OMX_Symbian_IndexConfigPreCaptureExposureTime, /**< reference: OMX_SYMBIAN_CONFIG_PRECAPTUREEXPOSURETIMETYPE */
    OMX_Symbian_IndexConfigExposureInitiated,    /**< reference: OMX_CONFIG_CALLBACKREQUESTTYPE */
    OMX_Symbian_IndexConfigBracketing,           /**< reference: OMX_SYMBIAN_CONFIG_BRACKETINGTYPE */
    OMX_Symbian_IndexConfigRAWPreset,            /**< reference: OMX_SYMBIAN_CONFIG_RAWPRESETTYPE */
    OMX_Symbian_IndexConfigApplyUserSettings,
    OMX_Symbian_IndexConfigVideoLight,
    OMX_Symbian_IndexConfigExtraCameraUserSettingsData,
    OMX_Symbian_IndexConfigExtraEXIFAppMarkerData,
    OMX_Symbian_IndexConfigExtraFrameDescriptionData,
    OMX_Symbian_IndexConfigExtraCaptureParametersData,
    OMX_Symbian_IndexConfigExtraROIData,
    OMX_Symbian_IndexConfigExtraHistogramData,
    OMX_Symbian_IndexConfigSupportedZoomResolutions,

    OMX_Symbian_IndexMax = 0x7FFFFFFF
} OMX_SYMBIAN_INDEXTYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_IndexExt_h */
