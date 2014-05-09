/*
  OMX_Symbian_CameraExt.h
  
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
@brief Symbian OpenMAX IL Extension Data Structures for Camera.

This file contains the extension structures for the Symbian IL extensions that
are needed for camera functionality.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_CameraExt_h
#define OMX_Symbian_CameraExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_IVCommon.h>
#include <openmax/il/khronos/v1_x/OMX_Image.h>
#include <OMX_Symbian_IVCommonExt_Ste.h>
#else
#include <OMX_Types.h>
#include <OMX_IVCommon.h>
#include <OMX_Image.h>
#include <OMX_Symbian_IVCommonExt_Ste.h>
#endif


/* Extensions to standard enums */
/* ---------------------------- */


/** 
 * Exposure Control extensions
 */
typedef enum OMX_SYMBIAN_EXPOSURECONTROLTYPE {
    OMX_SYMBIAN_ExposureControlCenter = OMX_ExposureControlVendorStartUnused + OMX_SYMBIAN_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_SYMBIAN_ExposureControlVeryLong,
    OMX_SYMBIAN_ExposureControlHwFunctionalTesting,
    OMX_SYMBIAN_ExposureControlMax = 0x7FFFFFFF
} OMX_SYMBIAN_EXPOSURECONTROLTYPE;

/** 
 * Image Filter extensions
 */
typedef enum OMX_SYMBIAN_IMAGEFILTERTYPE {
    OMX_SYMBIAN_ImageFilterSepia = OMX_ImageFilterVendorStartUnused + OMX_SYMBIAN_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_SYMBIAN_ImageFilterGrayScale,
    OMX_SYMBIAN_ImageFilterNatural,
    OMX_SYMBIAN_ImageFilterVivid,
    OMX_SYMBIAN_ImageFilterMax = 0x7FFFFFFF
} OMX_SYMBIAN_IMAGEFILTERTYPE;

/** 
 * Flash Control extensions
 */
typedef enum OMX_SYMBIAN_FLASHCONTROLTYPE {
    OMX_SYMBIAN_IMAGE_FlashControlSlowFrontSync = OMX_IMAGE_FlashControlVendorStartUnused + OMX_SYMBIAN_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_SYMBIAN_IMAGE_FlashControlSlowRearSync,
    OMX_SYMBIAN_IMAGE_FlashControlOnTest,               /**< Allows production testing of the flash with a higher flash intensity
                                                             than the torch (may be that of a normal flash) and test (some) HW that 
                                                             it turns OFF flash when automatic fail-safe happens (or capacitor
                                                             discharge in case of Xenon */
    OMX_SYMBIAN_IMAGE_FlashControlTorchOnTest,          /**< Turns on flash in torch mode */
    OMX_SYMBIAN_IMAGE_FlashControlIndicatorLightOnTest, /**< Turns on indicator light */
    OMX_SYMBIAN_IMAGE_FlashControlMax = 0x7FFFFFFF
} OMX_SYMBIAN_FLASHCONTROLTYPE;


/* Extended params and configs */
/* --------------------------- */


/** 
 * Zoom Factor config struct.
 * 
 * Note: Zoom factor is not the same as scale factor.
 * The zoom factor actually tells how much field of view
 * is decreased i.e. when zoom factor is 2x the 
 * FoV is decreased to half (1/2) of the original.
 * The scaling factor caused by decreasing the FoV is 
 * dependent on the output resolution. This config does not
 * affect frame size. Component indicates bounds for zoom
 * value, client can not change the bounds
*/
typedef struct OMX_SYMBIAN_CONFIG_ZOOMFACTORTYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_BU32 xZoomFactor;       /**< nValue: actual zoom factor in unsigned Q16 format,
                                     nMin: Q16 format, always 1.0
                                     nMax: Q16 format, maximum achievable zoom factor taking into account current params*/
}OMX_SYMBIAN_CONFIG_ZOOMFACTORTYPE;

/** 
 * Enumeration of possible focus ranges 
 */
typedef enum OMX_SYMBIAN_FOCUSRANGETYPE {
    OMX_SYMBIAN_FocusRangeAuto = 0,   /**< Focus range automatically decided by implementation */
    OMX_SYMBIAN_FocusRangeHyperfocal, /**< Focus range hyperfocal */
    OMX_SYMBIAN_FocusRangeNormal,     /**< Focus range normal */
    OMX_SYMBIAN_FocusRangeSuperMacro, /**< Focus range supermacro */
    OMX_SYMBIAN_FocusRangeMacro,      /**< Focus range macro */
    OMX_SYMBIAN_FocusRangeInfinity,   /**< Focus range infinity */
    OMX_SYMBIAN_FocusRangeFull,       /**< Focus range full */
    OMX_SYMBIAN_FocusRangeVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_FocusRangeMax = 0x7FFFFFFF
} OMX_SYMBIAN_FOCUSRANGETYPE;

/** 
 * Focus Range config struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_FOCUSRANGETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_FOCUSRANGETYPE eFocusRange; /**< Focus range */
} OMX_SYMBIAN_CONFIG_FOCUSRANGETYPE;

/** 
 * Autofocus Regions of Interest generic structure.
*/
typedef struct OMX_SYMBIAN_AFROITYPE {
    OMX_SYMBIAN_SIZETYPE nReferenceWindow;  /**< Size of the reference window */
    OMX_SYMBIAN_RECTTYPE nRect;             /**< Coordinates of ROI */
    OMX_S32 xFocusDistance;                 /**< Focusing distance in meters in Q16 format for this region */
    OMX_FOCUSSTATUSTYPE eFocusStatus;       /**< Focus status for this region */
} OMX_SYMBIAN_AFROITYPE;

/** 
 * Focus Status.
 * Read-only config for reporting focus status
*/
typedef struct OMX_SYMBIAN_CONFIG_EXTFOCUSSTATUSTYPE { 
    OMX_U32 nSize;                    /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;         /**< OMX specification version information */
    OMX_BOOL bFocused;                /**< Boolean indicating whether any of the focus 
                                           regions is currently succesfully focused */
    OMX_U32 nMaxAFAreas;               /**< Maximum number of AF areas. In case client has set to zero when calling 
                                           GetConfig component updates to the max number of focus regions and  
                                           no other fields are updated by component. In case client
                                           has set this to non-zero when calling GetConfig component does not update 
                                           it and only reports AF regions upto this number. */
    OMX_U32 nAFAreas;                 /**< Actual number of AF regions returned.
                                           Component updates this field to indicate the number of AF regions returned
                                           when upon GetConfig call. Any changes done by client are ignored.*/
    OMX_SYMBIAN_AFROITYPE sAFROIs[1]; /**< List of focus statuses and other info for all regions.
                                           List filled by component when calling GetConfig. */
} OMX_SYMBIAN_CONFIG_EXTFOCUSSTATUSTYPE;

/** 
 * Enumeration of possible depth of field choices
 */
typedef enum OMX_SYMBIAN_DOFHINTTYPE {
    OMX_SYMBIAN_DoFNotSpecified = 0, /**< Not specified, component decides by itself */
    OMX_SYMBIAN_DoFSmall,            /**< Small DoF */
    OMX_SYMBIAN_DoFMedium,           /**< Medium DoF */  
    OMX_SYMBIAN_DoFLarge,            /**< Large DoF */
    OMX_SYMBIAN_DoFVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_DoFMax = 0x7FFFFFFF
} OMX_SYMBIAN_DOFHINTTYPE;

/** 
 * Depth of field hint provided by IL client to implementation.
 *
*/
typedef struct OMX_SYMBIAN_CONFIG_HINTDOFTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_DOFHINTTYPE eDoFHint; /**< DoF hint */
} OMX_SYMBIAN_CONFIG_HINTDOFTYPE;

/** 
 * HW information retrieval struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_HWINFOTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nVersion1;          /**< For camera sensor filled like this:                                    
                                     Bits 0...7 Sensor Manufacturer (8 bits). 
                                     Bits 8...15 Sensor Version/Revision (8 bit). 
                                     Bits 16...31 Sensor Model (16 bits).
                                     For flashgun filled like this:
                                     Bits 0-7    Flash IC Info
                                     Bits 8-15   Flash IC revision
                                     Bits 16-17  Flash Module Info A as present in Module Info Reg.A of flash HW 
                                     (Only valid for some Xenon flash, leave as 0 for LED and even for Xenon if not available)
                                     Bits 24-31  Flash Module Info B as present in Module Info Reg.B of flash HW 
                                     (Only valid for some Xenon flash, leave as 0 for LED and even for Xenon if not available) */
    OMX_U32 nVersion2;          /**< Not filled for camera
                                     For flashgun filled in same way as nVersion1 but containin information for secondary flash IC connected to same camera. 
                                     Leave as 0s if there is only single flash IC*/
    OMX_STRING cInfoString;     /**< Additional information string. OMX client is responsible for allocating
                                     char buffer that can hold 256 characters plus zero termination*/
} OMX_SYMBIAN_CONFIG_HWINFOTYPE;

/** 
 * Capture mode extension.
 * 
 * Capture mode is extended to allow additional capture modes
 * like time nudge
*/
typedef struct OMX_SYMBIAN_CONFIG_EXTCAPTUREMODETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nFrameBefore;       /**< Number of frames to be stored before capture bit is set */ 
    OMX_BOOL bPrepareCapture;   /**< Enables capturing frames into ring buffer before capture bit is set */
}OMX_SYMBIAN_CONFIG_EXTCAPTUREMODETYPE;

/** 
 * Enumeration of possible rotation angles
 */
typedef enum OMX_SYMBIAN_ROTATIONANGLETYPE {
    OMX_SYMBIAN_RotationCW0 = 0, /**< Rotation angle zero degrees clockwise */
    OMX_SYMBIAN_RotationCW90,    /**< Rotation angle 90 degrees clockwise */
    OMX_SYMBIAN_RotationCW180,   /**< Rotation angle 180 degrees clockwise */
    OMX_SYMBIAN_RotationCW270,   /**< Rotation angle 270 degrees clockwise */
    OMX_SYMBIAN_RotationVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_RotationMax = 0x7FFFFFFF
} OMX_SYMBIAN_ROTATIONANGLETYPE;

/** 
 * Enumeration of possible image orientation values 
 * The orientation values are same as the ones defined
 * for Exif orientation tag.
 */
typedef enum OMX_SYMBIAN_ORIENTATIONORIGINTYPE
{
    OMX_SYMBIAN_OrientationNotSpecified = 0x0,    /**< Orientatio of frame is not specified */
    OMX_SYMBIAN_OrientationRowTopColumnLeft,      /**< Frame is in normal orientation */
    OMX_SYMBIAN_OrientationRowTopColumnRight,     /**< Frame is mirrored */
    OMX_SYMBIAN_OrientationRowBottomColumnRight,  /**< Frame is 180 degrees CW rotated (=mirrored and flipped) */
    OMX_SYMBIAN_OrientationRowBottomColumnLeft,   /**< Frame is flipped */
    OMX_SYMBIAN_OrientationRowLeftColumnTop,      /**< Frame is 90 degrees CW rotated and mirrored */
    OMX_SYMBIAN_OrientationRowRightColumnTop,     /**< Frame is 270 degrees CW rotated */
    OMX_SYMBIAN_OrientationRowRightColumnBottom,  /**< Frame is 270 degrees CW rotated and mirrored */
    OMX_SYMBIAN_OrientationRowLeftColumnBottom,   /**< Frame is 90 degrees CW rotated */
    OMX_SYMBIAN_OrientationVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_OrientationMax = 0x7FFFFFFF
} OMX_SYMBIAN_ORIENTATIONORIGINTYPE;

/** 
 * Orientation Configuration.
*/
typedef struct OMX_SYMBIAN_CONFIG_ORIENTATIONTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_ORIENTATIONORIGINTYPE eOrientation; /**< Frame orientation */
} OMX_SYMBIAN_CONFIG_ORIENTATIONTYPE;

/**
 * Maximum number of ROIs in config
 */
#define OMX_SYMBIAN_MAX_NUMBER_OF_ROIS 10

/** 
 * Tracking object type classification.
*/
typedef enum OMX_SYMBIAN_ROIOBJECTTYPE {
    OMX_SYMBIAN_RoiObjectNone = 0,   /**< Unspecified object type */
    OMX_SYMBIAN_RoiObjectTypeFace,   /**< Object type face */
    OMX_SYMBIAN_RoiObjectTypeObject, /**< Object type generic object */
    OMX_SYMBIAN_RoiObjectVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_RoiObjectMax = 0x7FFFFFFF
} OMX_SYMBIAN_ROIOBJECTTYPE;

/**
 * Pricipal 3D orientation type
 */
typedef struct OMX_SYMBIAN_3DORIENTATIONYTYPE {
    OMX_U32 nYaw;   /**< Yaw rotation angle in degrees, 0xFFFFFFFF if unknown */
    OMX_U32 nPitch; /**< Pitch rotation angle in degrees, 0xFFFFFFFF if unknown */
    OMX_U32 nRoll;  /**< Roll rotation angle in degrees, 0xFFFFFFFF if unknown */
} OMX_SYMBIAN_3DORIENTATIONYTYPE;

/**
 * Object information structure
 */
typedef struct OMX_SYMBIAN_ROIOBJECTINFOTYPE{
    OMX_SYMBIAN_RELATIVERECTTYPE sROI;           /**< Region where the object is located in the scene */
    OMX_U32 nROIID;                              /**< Identification number for ROI if available, zero by default */ 
    OMX_U32 nPriority;                           /**< ROI priority, 0 being the highest priority */
    OMX_SYMBIAN_ROIOBJECTTYPE eObjectType;       /**< Type of object in this ROI */
    OMX_SYMBIAN_3DORIENTATIONYTYPE nOrientation; /**< 3D orientation of the object in the ROI */
} OMX_SYMBIAN_ROIOBJECTINFOTYPE;

/** 
 * Regions of interest config
 * 
 * Used to indicate number of regions of interest within the image
 * Also some additional information about the object in the ROI
 * can be embedded but is not mandatory
*/
typedef struct OMX_SYMBIAN_CONFIG_ROITYPE {    
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nNumberOfROIs;      /**< Number of ROIs included in this config */
    OMX_SYMBIAN_ROIOBJECTINFOTYPE sROIs[OMX_SYMBIAN_MAX_NUMBER_OF_ROIS]; /**< Array of ROIs */ 
} OMX_SYMBIAN_CONFIG_ROITYPE; 

/** 
 * Regions of interest structure
 * 
 * Used to indicate number of regions of interest within the image
 * Also some additional information about the object in the ROI
 * can be embedded but is not mandatory
*/
typedef struct OMX_SYMBIAN_ROITYPE {    
    OMX_U32 nNumberOfROIs;      /**< Number of ROIs included in this config */
    OMX_SYMBIAN_ROIOBJECTINFOTYPE sROIs[OMX_SYMBIAN_MAX_NUMBER_OF_ROIS]; /**< Array of ROIs */ 
} OMX_SYMBIAN_ROITYPE;

/** 
 * Enumeration of possible flash status values 
 */
typedef enum OMX_SYMBIAN_FLASHSTATUSTYPE
{
    OMX_SYMBIAN_FlashUnknown  = 0x0,   /**< Flash status is unknown */
    OMX_SYMBIAN_FlashOff,              /**< Flash is discharged (xenon) or off */
    OMX_SYMBIAN_FlashCharging,         /**< Flash is charging (xenon) */
    OMX_SYMBIAN_FlashReady,            /**< Flash is ready to be used */
    OMX_SYMBIAN_FlashNotAvailable,     /**< Flash can not be used at the moment
                                            (temporarily overheated etc...) */
    OMX_SYMBIAN_FlashInsufficientCharge,  /**< Flash can not be used at the moment
                                               since there is not enough power to fire it*/
    OMX_SYMBIAN_FlashVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_FlashMax = 0x7FFFFFFF
} OMX_SYMBIAN_FLASHSTATUSTYPE;

/** 
 * Flash status Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_FLASHSTATUSTYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_FLASHSTATUSTYPE eFlashStatus; /**< Flash status */
} OMX_SYMBIAN_CONFIG_FLASHSTATUSTYPE;


/** 
 * Enumeration of possible pre capture exposure time estimates
 * 
 * This enumeration is used to implement pre-capture warning about
 * low or high exposure time which could lead to bad quality images
 */
typedef enum OMX_SYMBIAN_PRECAPTUREEXPOSURETIMETYPE
{
    OMX_SYMBIAN_PreCaptureExposureNoneOrOngoing = 0x0, /**< Exposure time not known or exposure already started */
    OMX_SYMBIAN_PreCaptureExposureNormal,              /**< Exposure time is within normal limits */
    OMX_SYMBIAN_PreCaptureExposureShort,               /**< Exposure time is short, risk of underexposing the images */
    OMX_SYMBIAN_PreCaptureExposureLong,                /**< Exposure time is long, risk of blurry image */
    OMX_SYMBIAN_PreCaptureExposureVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_PreCaptureExposureMax = 0x7FFFFFFF
} OMX_SYMBIAN_PRECAPTUREEXPOSURETIMETYPE;

/** 
 * Pre-Capture Exposure Time Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_PRECAPTUREEXPOSURETIMETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_PRECAPTUREEXPOSURETIMETYPE eExposureTime; /**< Estimated exposure time */
} OMX_SYMBIAN_CONFIG_PRECAPTUREEXPOSURETIMETYPE;

/** 
 * Enumeration of possible bracket mode values 
 */
typedef enum OMX_SYMBIAN_BRACKETMODETYPE {
    OMX_SYMBIAN_BracketExposureRelativeInEV = 0,/**< Exposure value is changed relative to the value set by 
                                                      automatic exposure. nBracketValues are in Q16. */
    OMX_SYMBIAN_BracketExposureAbsoluteMs,      /**< Exposure value is changed in absolute value in ms. 
                                                     nBracketValues are in Q16. */
    OMX_SYMBIAN_BracketFocusRelative,           /**< Focus is adjusted relative around the current focus position. 
                                                     The value is S32 integer, and is the same as adjusting nFocusSteps 
                                                     of OMX_IMAGE_CONFIG_FOCUSCONTROLTYPE relatively. */
    OMX_SYMBIAN_BracketFocusAbsolute,           /**< Focus position is adjusted absolutely. It is the same as setting 
                                                     nFocusSteps of OMX_IMAGE_CONFIG_FOCUSCONTROLTYPE relatively for 
                                                     each captures. The value should be interpreted as an U32 value. */
    OMX_SYMBIAN_BracketFlashPower,              /**< Power of flash in percentages. Only 0%(no flash) and 100% (flash fired) 
                                                     shall be supported, other values may be supported. The value should be 
                                                     interpreted as an U32 value. If for any asked brackets the requested 
                                                     level of flash power cannot be reached, the capturing sequence will not 
                                                     be interrupted or delayed, and this situation will be reported using 
                                                     the status of the flash. */
    OMX_SYMBIAN_BracketAperture,                /**< Aperture number relative to the automatic setting. Data in Q16 format. */
    OMX_SYMBIAN_BracketWhiteBalance,            /**< White Balance is adjusted using required values relatively to  
                                                     current color temperature resulted from automatic WB calculation. 
                                                     Data is in Q16 format. */
    OMX_SYMBIAN_BracketVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_BracketMax = 0x7FFFFFFF
} OMX_SYMBIAN_BRACKETMODETYPE;

/** 
 * Bracketing Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_BRACKETINGTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_BRACKETMODETYPE eBracketMode;   /**< Bracketing mode - what values are changed during bracket capture */
    OMX_BOOL bCumulativeBracketing;             /**< When set true allows an IL client to accumulate bracketing settings. 
                                                     If cumulative bracketing is supported, it would mean all subsequent 
                                                     calls of this setting with bCumulativeBracketing true will accumulate 
                                                     settings for different bracketing types. Any bracketing setting call 
                                                     with bCumulativeBracketing false resets the accumulation of settings. 
                                                     If the IL client uses different numbers of bracketings for different 
                                                     types, the last defined value is repeated as many times as needed. */
    OMX_U32 nNbrBracketingValues;               /**< Specifies how many different bracketing values are used. 
                                                     The default value is 0, so by default bracketing is disabled. */
    OMX_U32 nBracketValues[1];                  /**< An array specifying the bracket values to be used. Bracket values and their 
                                                     formats are actually dependent on the type of bracketing, not always in
                                                     U32 format. Different representations of data on 32 bits for different types
                                                     of bracketing are being used. The actual data format representation needs to
                                                     be clarified for each type of bracketing when the type of bracketing is defined */
} OMX_SYMBIAN_CONFIG_BRACKETINGTYPE;

/** 
 * Enumeration of possible levels of RAW processing 
 */
typedef enum OMX_SYMBIAN_RAWIMAGEPRESETTYPE {
    OMX_SYMBIAN_RawImageUnprocessed = 0,/**< RAW data is unprocessed */
    OMX_SYMBIAN_RawImageProcessed,      /**< RAW data has some processing enabled */
    OMX_SYMBIAN_RawImageVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_RawImageMax = 0x7FFFFFFF
} OMX_SYMBIAN_RAWIMAGEPRESETTYPE;

/** 
 * RAW Preset Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_RAWPRESETTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_RAWIMAGEPRESETTYPE ePreset; /**< RAW processing preset */
} OMX_SYMBIAN_CONFIG_RAWPRESETTYPE;

/** 
 * Camera Parameters Lock Generic Enum.
*/
typedef enum OMX_SYMBIAN_LOCKTYPE {
    OMX_SYMBIAN_LockOff = 0,   /**< No lock applied */
    OMX_SYMBIAN_LockImmediate, /**< Lock is applied immediately */
    OMX_SYMBIAN_LockAtCapture, /**< Lock is applied at next capture */
    OMX_SYMBIAN_LockVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_LockMax = 0x7FFFFFFF 
} OMX_SYMBIAN_LOCKTYPE;

/** 
 * Image Parameters Lock.
*/
typedef struct OMX_SYMBIAN_CONFIG_LOCKTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_LOCKTYPE eImageLock;      /**< The lock setting */
} OMX_SYMBIAN_CONFIG_LOCKTYPE;

/** 
 * Image Flash Control Config.
 * 
 * Extended because in standard flash control is a param
*/
typedef struct OMX_SYMBIAN_CONFIG_FLASHCONTROLTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_IMAGE_FLASHCONTROLTYPE eFlashControl; /**< Flash mode setting */
}OMX_SYMBIAN_CONFIG_FLASHCONTROLTYPE;

/** 
 * Enumeration of possible focus region control values
 */
typedef enum OMX_SYMBIAN_FOCUSREGIONCONTROL {
    OMX_SYMBIAN_FocusRegionAuto = 0,       /**< Focus region decided by AF algorithm */
    OMX_SYMBIAN_FocusRegionManual,         /**< Manual focus region selected by user */
    OMX_SYMBIAN_FocusRegionFacePriority,   /**< ROI with priority face (if available) should be used as focus region, otherwise automatically selected by AF algorithm */
    OMX_SYMBIAN_FocusRegionObjectPriority, /**< ROI with priority object (if available) should be used as focus region, otherwise automatically selected by AF algorithm */
    OMX_SYMBIAN_FocusRegionVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_FocusRegionMax = 0x7FFFFFFF
} OMX_SYMBIAN_FOCUSREGIONCONTROL;

/** 
 * Focus Region Control Config.
*/
typedef struct OMX_SYMBIAN_CONFIG_FOCUSREGIONTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_FOCUSREGIONCONTROL eFocusRegionControl; /**< Focus region selection */
    OMX_SYMBIAN_RELATIVERECTTYPE sFocusRegion;          /**< Focus region if focus region control is manual */
} OMX_SYMBIAN_CONFIG_FOCUSREGIONTYPE;

/**
 * Enumeration of possible camera self tests
 */
typedef enum OMX_SYMBIAN_CAMERASELFTESTTYPE
{
    OMX_SYMBIAN_CameraSelfTestNone = 0,
    OMX_SYMBIAN_CameraSelfTestSensor,               /**< Verifies command and data bus connections of camera sensor */
    OMX_SYMBIAN_CameraSelfTestHwa,                  /**< Verifies command bus and data bus connections between host and camera HWA */
    OMX_SYMBIAN_CameraSelfTestFocus,                /**< Verifies that focus lens is able to move. This test typically requires HW's 
                                                         ability to measure lens position. Test can be done e.g. by moving lens to 
                                                         two different positions and reading back the lens position */

    OMX_SYMBIAN_CamearSelfTestOpticalZoom,          /**< Verifies that optical zoom lens is able to move. This test typically requires 
                                                         HW's ability to measure lens position. Test can be done e.g. by moving lens to 
                                                         two different positions and reading back the lens position */
    OMX_SYMBIAN_CameraSelfTestFlashDriverPrimaryL,  /**< tests connections to primary flash driver IC */
    OMX_SYMBIAN_CameraSelfTestFlashDriverSecondaryL,/**< tests connections to secondary flash driver IC */
    OMX_SYMBIAN_CameraSelfTestFlashL,               /**< tests capture flash without using strobe signal from camera */
    OMX_SYMBIAN_CameraSelfTestFlashWithStrobe,      /**< tests capture flash using strobe signal from camera: ONLY 
                                                         this one needs to be done in idle state from flash tests cases */
    OMX_SYMBIAN_CameraSelfTestVideoLightL,          /**< tests video light */
    OMX_SYMBIAN_CameraSelfTestAfLightL,             /**< tests AF assistance light */
    OMX_SYMBIAN_CameraSelfTestIndicatorLightL,      /**< tests capture indicator light */
    OMX_SYMBIAN_CameraSelfTestFlashTorchL,          /**< tests flash in torch mode */
    OMX_SYMBIAN_CameraSelfTestHwaControl,           /**< Verifies HWA command bus connection */
    OMX_SYMBIAN_CameraSelfTestHwaOTP,               /**< Sanity check for HWA OTP programming */
    OMX_SYMBIAN_CameraSelfTestHwaSDRAM,             /**< Verifies HWA SDRAM bus connections */
    OMX_SYMBIAN_CameraSelfTestHwaSleep,             /**< Verifies HWA ability to sleep/wakeup */
    OMX_SYMBIAN_CameraSelfTestHwaPmControl,         /**< Verifies HWA power managment control */
    OMX_SYMBIAN_CameraSelfTestSensorControl,        /**< Verifies sensor command bus connection */
    OMX_SYMBIAN_CameraSelfTestSensorNVM,            /**< Verifies sensor non-volatile memory is readable */
    OMX_SYMBIAN_CameraSelfTestLensDrvControl,       /**< Verifies lens driver IC command bus connection */
    OMX_SYMBIAN_CameraSelfTestShutter,              /**< Verifies shutter operation */
    OMX_SYMBIAN_CameraSelfTestNDFilter,             /**< Verifies ND filter */
    OMX_SYMBIAN_CameraSelfTestGlobalReset,          /**< Verifies global reset signal (from camera to host) connection */
    OMX_SYMBIAN_CameraSelfTestFlashTxEnabledL,      /**< Verifies the TXP masking line connection (from Modem to Flash HW) by detecting that it is in  enabled state. */
    OMX_SYMBIAN_CameraSelfTestFlashTxDisabledL,     /**< Verifies the TXP masking line connection (from Modem to Flash HW) by detecting that it is in  disabled state. */
    OMX_SYMBIAN_CameraSelfTestVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_CameraSelfTestMax = 0x7FFFFFFF
} OMX_SYMBIAN_CAMERASELFTESTTYPE;

/**
 * Self test type and result struct
 */
typedef struct OMX_SYMBIAN_CAMERASELFTESTRESULTTYPE {
    OMX_SYMBIAN_CAMERASELFTESTTYPE eTestId;   /**< Test id */
    OMX_ERRORTYPE eResult;                    /**< Test result */
} OMX_SYMBIAN_CAMERASELFTESTRESULTTYPE;

#define OMX_SYMBIAN_MAX_SELF_TESTS_PER_REQUEST 20

/**
 * Self test select config 
 */
typedef struct OMX_SYMBIAN_CONFIG_CAMERASELFTESTSELECTTYPE { 
    OMX_U32 nSize;                  /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_U32 nPortIndex;             /**< Port that this structure applies to */
    OMX_U32 nFlashHwFaultRegister1; /**< Flash fault register 1 contents unaltered */
    OMX_U32 nFlashHwFaultRegister2; /**< Flash fault register 2 contents unaltered */
    OMX_U16 nTestSelectMaxSizeUsed; /**< Number of test results in array */
    OMX_SYMBIAN_CAMERASELFTESTRESULTTYPE eSelfTests[OMX_SYMBIAN_MAX_SELF_TESTS_PER_REQUEST]; /**< Self test results */
} OMX_SYMBIAN_CONFIG_CAMERASELFTESTSELECTTYPE; 

/**
 * Enumeration of functional tests
 */
typedef enum OMX_SYMBIAN_CAMERAHWFUNCTEST {
    OMX_SYMBIAN_CameraFuncTestNone = 0,
    OMX_SYMBIAN_CameraFuncTestDeadPixels,
    OMX_SYMBIAN_CameraFuncTestCouplet,
    OMX_SYMBIAN_CameraFuncTestBrightness,
    OMX_SYMBIAN_CameraFuncTestVignetting,
    OMX_SYMBIAN_CameraFuncTestNdFilter,
    OMX_SYMBIAN_CameraFuncTestFocus,
    OMX_SYMBIAN_CameraFuncTestIris,
    OMX_SYMBIAN_CameraFuncTestFlash,
    OMX_SYMBIAN_CameraFuncTestIndicatorLed,
    OMX_SYMBIAN_CameraFuncTestVideoLight,
    OMX_SYMBIAN_CameraFuncTestBlemish,
    OMX_SYMBIAN_CameraFuncTestShutter,
    OMX_SYMBIAN_CameraFuncTestVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_CameraFuncTestMax = 0x7FFFFFFF
} OMX_SYMBIAN_CAMERAHWFUNCTEST;

#define OMX_SYMBIAN_MAX_FUNC_TESTS_PER_REQUEST 20

/**
 * Functional test selection config
 */
typedef struct OMX_SYMBIAN_CONFIG_CAMERAHWFUNCTESTSELECTTYPE { 
    OMX_U32 nSize;                  /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_U32 nPortIndex;             /**< Port that this structure applies to */
    OMX_U16 nTestSelectMaxSizeUsed; /**< Number of entries in eFuncTests */
    OMX_SYMBIAN_CAMERAHWFUNCTEST eFuncTests[OMX_SYMBIAN_MAX_FUNC_TESTS_PER_REQUEST]; /**< Array of selected tests */ 
    OMX_BOOL bTestImageOutput;      /**< If set OMX_TRUE, NRW format test image is outputted in addition to test results */
}OMX_SYMBIAN_CONFIG_CAMERAHWFUNCTESTSELECTTYPE;

/**
 * Enumeration of scene modes
 */
typedef enum OMX_SYMBIAN_SCENEMODETYPE {
    OMX_SYMBIAN_SceneAuto = 0,      /**< Camera settings automatically selected by camera */
    OMX_SYMBIAN_ScenePortrait,      /**< Use camera settings suitable for taking portrait images */
    OMX_SYMBIAN_SceneLandscape,     /**< Use camera settings suitable for taking landscae images */
    OMX_SYMBIAN_SceneNight,         /**< Use camera settings suitable for taking low light images */
    OMX_SYMBIAN_SceneNightPortrait, /**< Use camera settings suitable for taking portrait images in low light */
    OMX_SYMBIAN_SceneSport,         /**< Use camera settings suitable for taking images when there is fast movement in the scene */
    OMX_SYMBIAN_SceneMacro,         /**< Use camera settings suitable for taking close-up images */
    OMX_SYMBIAN_SceneVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_SceneMax = 0x7FFFFFFF
} OMX_SYMBIAN_SCENEMODETYPE;

/**
 * Scene mode config
 */
typedef struct OMX_SYMBIAN_CONFIG_SCENEMODETYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_SCENEMODETYPE eSceneType; /**< Scene mode selection */
} OMX_SYMBIAN_CONFIG_SCENEMODETYPE;

/**
 * RGB histogram returned using this config
 */
typedef struct OMX_SYMBIAN_CONFIG_RGBHISTOGRAM {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nRed[256];          /**< Histogram for red color */
    OMX_U32 nGreen[256];        /**< Histogram for green color */
    OMX_U32 nBlue[256];         /**< Histogram for blue color */
} OMX_SYMBIAN_CONFIG_RGBHISTOGRAM;

/**
 * Histogram control config
 */
typedef struct OMX_SYMBIAN_CONFIG_HISTOGRAMCONTROLTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_BOOL bMeasure;          /**< Set OMX_TRUE to start histogram measurement */
    OMX_U32 nBins;              /**< Number of bins */
    OMX_U32 nBytesPerBin;       /**< Number of bytes / bin */
} OMX_SYMBIAN_CONFIG_HISTOGRAMCONTROLTYPE;

/**
 * Region of interest selection config 
 */
typedef struct OMX_SYMBIAN_CONFIG_ROISELECTIONTYPE {
    OMX_U32 nSize;                   /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;        /**< OMX specification version information */
    OMX_U32 nPortIndex;              /**< Port that this structure applies to */
    OMX_BOOL bReset;                 /**< OMX_TRUE = reset all selections done. Rest of the fields are not used */ 
    OMX_BOOL bSelect;                /**< OMX_TRUE = select / OMX_FALSE = deselect */
    OMX_SYMBIAN_RELATIVERECTTYPE sROI;  /**< Indicates the region where the object to be selected is located in the scene */
    OMX_U32 nROIID;                     /**< This field can contain the ID of object in order to assist  
                                             object selection. 0 means undefined and is the default value. */
    OMX_SYMBIAN_ROIOBJECTTYPE eObjectType; /**< Indicates the type of object to be selected. */
} OMX_SYMBIAN_CONFIG_ROISELECTIONTYPE;

/** 
 * High level control type classification.
*/
typedef enum OMX_SYMBIAN_HIGHLEVELCONTROLTYPE {
    OMX_SYMBIAN_ControlOff = 0, /**< Explicitly turn feature off */ 
    OMX_SYMBIAN_ControlOn,      /**< Explicitly turn feature on */
    OMX_SYMBIAN_ControlAuto,    /**< Let feature to be turned on or off automatically */
    OMX_SYMBIAN_ControlVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_ControlMax = 0x7FFFFFFF
} OMX_SYMBIAN_HIGHLEVELCONTROLTYPE;

/** 
 * Generic high level control config struct.
 *
*/
typedef struct OMX_SYMBIAN_CONFIG_HIGHLEVELCONTROLTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_HIGHLEVELCONTROLTYPE eControl; /**< High level control for given feature */
} OMX_SYMBIAN_CONFIG_HIGHLEVELCONTROLTYPE;

/** 
 * Enumeration of possible user choices of power vs. quality values
 */
typedef enum OMX_SYMBIAN_HINTPOWERVSQUALITYTYPE {
    OMX_SYMBIAN_HintNotSpecified = 0,   /**< Not specified, component decides by itself */
    OMX_SYMBIAN_HintOverallLowPower,    /**< Overall power consumption priorized over quality */
    OMX_SYMBIAN_HintOverallHighQuality, /**< Overall high quality priorized over power consumption */
    OMX_SYMBIAN_HintVendorStartUnused = 0x70000000,
    OMX_SYMBIAN_HintMax = 0x7FFFFFFF
} OMX_SYMBIAN_HINTPOWERVSQUALITYTYPE;

/** 
 * Power versus quality hint provided by IL client to implementation.
*/
typedef struct OMX_SYMBIAN_CONFIG_HINTPOWERVSQUALITYTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_HINTPOWERVSQUALITYTYPE eHint; /**< Power vs quality hint */
} OMX_SYMBIAN_CONFIG_HINTPOWERVSQUALITYTYPE;

/** 
 * Enumeration of possible analyzer feedback types
 */
typedef enum OMX_SYMBIAN_FEEDBACKTYPE {
    OMX_SYMBIAN_FeedbackFaceTracker = 0,/**< Face tracker analyzer feedback 
                                             nFeedbackValue contains OMX_SYMBIAN_ROITYPE structure */
    OMX_SYMBIAN_FeedbackObjectTracker,  /**< Object tracker analyzer feedback 
                                             nFeedbackValue contains OMX_SYMBIAN_ROITYPE structure */
    OMX_SYMBIAN_FeedbackMotionLevel,    /**< Motion level analyzer feedback 
                                             nFeedbackValue contains OMX_U32 type*/
    OMX_SYMBIAN_FeedbackAutomaticSceneDetection, /**< ASD analyzer feedback 
                                                      nFeedbackValue contains OMX_SYMBIAN_SCENEMODETYPE type*/
    OMX_SYMBIAN_FeedbackVendorStart = 0x70000000, /**< Vendor specific feedback range start */
    OMX_SYMBIAN_FeedbackMax = 0x7FFFFFFF
} OMX_SYMBIAN_FEEDBACKTYPE;

/**
 * Generic, opaque config for passing feedback values from analyzers to other components
 * like the camera. Adding new analyzer to the graph should be easy since all analyzers
 * can use the same config for passing their analysis results. 
 */
typedef struct OMX_SYMBIAN_CONFIG_FEEDBACKTYPE {
    OMX_U32 nSize;                              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;                   /**< OMX specification version information */
    OMX_U32 nPortIndex;                         /**< The port that this structure applies to */
    OMX_TICKS nOriginatorTimestamp;             /**< Timestamp of the frame the analysis was performed on */
    OMX_U32 nOriginatorFrameNum;                /**< Frame number on which the analysis was performed on*/
    OMX_SYMBIAN_FEEDBACKTYPE nFeedbackType;     /**< An enum to differentiate among different feedback types*/
    OMX_U32 nFeedbackValueSize;                 /**< The size of provided feedback data (in bytes)
                                                     In case client has set this to zero when calling 
                                                     GetConfig component updates to the number of bytes needed for nFeedbackValue 
                                                     and no other fields are updated by component. In case client
                                                     has set this to non-zero it means the number of bytes client has
                                                     allocated for nFeedbackValue field. In case not enough space has been
                                                     allocated for nFeedbackValue component may return an error. */
    OMX_U8 nFeedbackValue[1];                   /**< Feedback data in byte buffer*/
} OMX_SYMBIAN_CONFIG_FEEDBACKTYPE;

/**
 * Enumeration of possible frame aspect ratios
 */
typedef enum OMX_SYMBIAN_ASPECTRATIOTYPE {
    OMX_SYMBIAN_AspectRatioUnspecified = 0, /**< Unspecified AR, all sizes are returned no matter what the AR */
    OMX_SYMBIAN_AspectRatio4To3,            /**< Only sizes with 4:3 AR returned */
    OMX_SYMBIAN_AspectRatio16To9,           /**< Only sizes with 16:9 AR returned */
    OMX_SYMBIAN_AspectRatio1To1,            /**< Only sizes with 1:1 AR returned */
    OMX_SYMBIAN_AspectRatio3To2,            /**< Only sizes with 3:2 AR returned */
    OMX_SYMBIAN_AspectRatioNonStandard,     /**< Sizes with AR other than than 4:3, 16:9, 1:1 or 3:2 returned */
    OMX_SYMBIAN_AspectRatioVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_SYMBIAN_AspectRatioMax = 0x7FFFFFFF
} OMX_SYMBIAN_ASPECTRATIOTYPE;

/**
 * Config returning supported resolutions of particular port. Only used with OMX_GetConfig.
 */
typedef struct OMX_SYMBIAN_CONFIG_SUPPORTEDRESOLUTIONS {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_ASPECTRATIOTYPE eAspectRatio; /**< Aspect ratio filter filled in by client */
    OMX_U32 nNumResolutions;    /**< If set to 0 by the IL client component returns here the number of resolutions that are supported
                                     no resolutions are returned. If set to a non zero value (N) by the IL client this means the 
                                     IL client queries the N largest resolutions and the whole struct must be dimensioned for N OMX_SYMBIAN_SIZETYPE.
                                     Once GetConfig call returns nNumResolutions has been updated to reflect the number of resolutions returned. */
    OMX_SYMBIAN_SIZETYPE sResolutions[1];    /**< List of nNumResolutions resolutions sorted in decreasing size order */
} OMX_SYMBIAN_CONFIG_SUPPORTEDRESOLUTIONS;

/**
 * Param for telling a component which values it needs to put in Exif make and model fields
 */
typedef struct OMX_SYMBIAN_PARAM_DEVICEMAKEMODEL {
    OMX_U32 nSize;            /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    OMX_U32 nPortIndex;       /**< Port that this structure applies to */
    OMX_U8 cMake[128];        /**< String containing make of the device 
                                   (client allocates buffer for 128 characters including NULL termination) */
    OMX_U8 cModel[128];       /**< String containing model of the device
                                   (client allocates buffer for 128 characters including NULL termination) */ 
} OMX_SYMBIAN_PARAM_DEVICEMAKEMODEL;

/**
 * Param for telling a component which values it needs to put in Exif make and model fields
 */
typedef struct OMX_SYMBIAN_PARAM_SOFTWARE {
    OMX_U32 nSize;            /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    OMX_U32 nPortIndex;       /**< Port that this structure applies to */
    OMX_U8 cSoftware[128];        /**< String containing make of the device
                                   (client allocates buffer for 128 characters including NULL termination) */
} OMX_SYMBIAN_PARAM_SOFTWARE;


/**
 * Read-only config used to retrive camera sensor NVM data
 */
typedef struct OMX_SYMBIAN_CONFIG_SENSORNVMDATATYPE {
    OMX_U32 nSize;                    /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;         /**< OMX specification version information */
    OMX_U32 nNVMBytes;                /**< Amount of NVM data in bytes. In case client has set to zero when calling GetConfig,
                                           component updates the number NVM bytes and no other fields. In case client has set
                                           this to non-zero when calling GetConfig, component fills the number of nNVMBytes to NVMData. */
    OMX_U8 NVMData[1];                /**< NVM data. */
} OMX_SYMBIAN_CONFIG_SENSORNVMDATATYPE;



/** 
 * Extradata extensions
 */
typedef enum OMX_SYMBIAN_CAMERA_EXTRADATATYPE {
    OMX_SYMBIAN_CameraExtraDataCameraUserSettings = OMX_ExtraDataVendorStartUnused + OMX_SYMBIAN_CAMERA_EXTENSIONS_START_OFFSET,
    OMX_SYMBIAN_CameraExtraDataEXIFAppMarker,
    OMX_SYMBIAN_CameraExtraDataFrameDescription,
    OMX_SYMBIAN_CameraExtraDataCaptureParameters,
    OMX_SYMBIAN_CameraExtraDataROI,
    OMX_SYMBIAN_CameraExtraDataHistogram, 
    OMX_SYMBIAN_CameraExtraDataFeedback,
    OMX_SYMBIAN_CameraExtraData3AVendorSpecific,
    OMX_Symbian_CameraExtraDataAFStatus,
    OMX_SYMBIAN_CameraExtraDataMax = 0x7FFFFFFF
} OMX_SYMBIAN_CAMERA_EXTRADATATYPE; 

typedef struct OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE {
    OMX_S32 nX; /**< User selection for coordinates of the Center Field Of View */
    OMX_S32 nY;
    OMX_BU32 xDigitalZoomFactor; /**< User selection for digital zoom */
    OMX_BU32 xOpticalZoomFactor; /**< User selection of optical zoom */

    //Drive mode user settings
    OMX_BOOL bOneShot;      /**< User selection of still vs video UC */
    OMX_BOOL bContinuous;   /**< User selection of continuous or not */
    OMX_BOOL bFrameLimited; /**< User selection of limited or not */
    OMX_U32 nFrameLimit;    /**< User selection of number of after capture button press frames in case of limited*/
    OMX_U32 nFramesBefore;  /**< User selection of number of before capture button press frames in case of limited*/

    OMX_SYMBIAN_SCENEMODETYPE nSceneModePreset;    /**< User selection of scene mode */
    OMX_WHITEBALCONTROLTYPE eWhiteBalControl; /**< User selection for white balance */
    OMX_EXPOSURECONTROLTYPE eExposureControl; /**< User selection for exposure */
    OMX_IMAGE_FLASHCONTROLTYPE eFlashControl; /**< User selection for flash */
    OMX_IMAGE_FOCUSCONTROLTYPE eFocusControl; /**< User selection for focus */
    OMX_U32 nFocusSteps;
    OMX_U32 nFocusStepIndex;
    
    OMX_U32 nLockingStatus; /**< Bitfield containing user selection for locks: 0 - no locks, 1 - exposure lock, 2 - WB lock, 4 - focus lock */

    OMX_METERINGTYPE eMetering; /**< User selection for metering */
    OMX_S32 xEVCompensation;    /**< User selection for EV compensation */
    OMX_U32 nApertureFNumber;   /**< User selection for aperture */
    OMX_BOOL bAutoAperture;     /**< OMX_TRUE indicates that aperture value is 
                                     automatically selected, user selection does not apply */
    OMX_U32 nShutterSpeedMsec;  /**< User selection of shutterspeed in milliseconds */                                 
    OMX_BOOL bAutoShutterSpeed; /**< OMX_TRUE indicates that shutter speed value is 
                                     automatically selected, user selection does not apply */
    OMX_U32 nSensitivity;       /**< User selected sensitivity value */
    OMX_BOOL bAutoSensitivity;  /**< OMX_TRUE indicates that sensitivity value is 
                                     automatically selected, user selection does not apply */
    OMX_IMAGEFILTERTYPE eImageFilter; /**< User selection for image filter */

    OMX_S32 nContrast;      /**< User selection for contrast */
    OMX_U32 nBrightness;    /**< User selection for brighness */
    OMX_S32 nSaturation;    /**< User selection for saturation */
    OMX_S32 nSharpness;     /**< User selection for sharpness */
    OMX_BOOL bStab;         /**< User selection for stabilization */

    OMX_SYMBIAN_DOFHINTTYPE eDoFhint; /**< User selection for depth of field */
    
    OMX_SYMBIAN_FOCUSREGIONCONTROL eFocusRegionControl; /**< User selection for focus region selection */
    OMX_SYMBIAN_RELATIVERECTTYPE sFocusRegion;          /**< User selection for focus region if focus region control is manual */ 
} OMX_SYMBIAN_CAMERAUSERSETTINGSTYPE;


typedef struct OMX_SYMBIAN_EXIFAPPMARKERTYPE {    
    OMX_U32 nSize;                       /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nExtraDataVersion;   /**< Extradata version information */
    OMX_U8 nAppMarkerData[1];            /**< Exif data. nSize is affected by its size */
} OMX_SYMBIAN_EXIFMARKERTYPE;

typedef struct OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE {
    OMX_VERSIONTYPE nExtraDataVersion;      /**< Extradata version information */
    OMX_U8 nPixelFormat;                    /**< The used pixel format: 0 - uncompressed and unpacked, 
                                                 1 –uncompressed and packed, 2 –compressed and unpacked, 3 –compressed and packed */
    OMX_U8 nDataEndianess;                  /**< Data endianess: 0 - little endian, 1 - big endian */
    OMX_SYMBIAN_RAWIMAGEPRESETTYPE eRawPreset; /**< RAW preset */
    OMX_U8 nColorOrder;                     /**< The used color order, as in SMIA spec Table 44, e.g. 0 - GRBG, …*/
    OMX_U8 nBitDepth;                       /**< The bit depth: 8, 10, 12, ... bit pixel data */
    OMX_S32 nDataPedestal;                  /**< The data pedestal, -1 if N.A. */
    OMX_U8 nFrameCounter;                   /**< Frame counter value */
    OMX_U8 nImageDataReadoutOrder;          /**< The sensor module readout order of the image data */

    OMX_U8 nIslLinesTop;    /**< The number of top ISL lines */      
    OMX_U8 nIslLinesBottom; /**< The number of bottom ISL lines */      
   
    OMX_U16 nVisColsStart;  /**< Visible pixels sensor coordinates  providing also extras information */
    OMX_U16 nVisCols;
    OMX_U16 nVisRowsStart;
    OMX_U16 nVisRows;
    
    OMX_U16 nSensorRoiColsStart; /**< First column of pixels included in region of interest streamed from sensor 
                                      relative to full sensor columns */
    OMX_U16 nSensorRoiCols;      /**< Number of pixel columns streamed from sensor 
                                      relative to full sensor columns */
    OMX_U16 nSensorRoiRowsStart; /**< First row of pixels included in region of interest streamed from sensor 
                                      relative to full sensor rows */
    OMX_U16 nSensorRoiRows;      /**< Number of pixel rows streamed from sensor 
                                      relative to full sensor rows */
    
    OMX_U16 nSensorCols;         /**< Number of pixel columns sensor has */
    OMX_U16 nSensorRows;         /**< Number of pixel rows sensor has */

    OMX_U16 nSpatialSampling;    /**< It is 0 for Bayer sampling, 1 for co-sited,…*/
    OMX_U16 nScalingMode;        /**< It is 0 for None, 1 horizontal, 2 full, ... */

} OMX_SYMBIAN_FRAMEDESCRIPTIONTYPE;

typedef struct OMX_SYMBIAN_CAPTUREPARAMETERSTYPE {
    OMX_VERSIONTYPE nExtraDataVersion;  /**< Extradata version information */

    //Scene characterization
    OMX_SYMBIAN_ORIENTATIONORIGINTYPE eSceneOrientation; /**< The actual scene orientation */
    OMX_U32 nMotionLevel; /**< The detected motion level */
    OMX_S32 nLuxAmbient;  /**< The actual estimated lux value for ambient light, -1 if if N.A. */

    //AEW 
    OMX_S32 nTargetTotalExposure;   /**< Total target exposure in microseconds (expo x AG), -1 if N.A. */
    OMX_S8  nExposureStatus;        /**< Within range [-100, 100]: 0 exposed, negative underexposed, positive overexposed. 
                                         Indicates gap between target and actual settings */
    OMX_S32 xAnalogGain;        /**< Analog gain multiplier in Q16 format, -1 if N.A. */
    OMX_S32 xDigitalGain;       /**< Digital gain information in Q16 format, -1 if N.A. 
                                     It indicates how much brightness amplification is included into AWB gains */
    OMX_U32 nExposureTime;      /**< Actual used exposure time in microseconds, 0 if N.A. */
    OMX_U32 xApertureFNumber;   /**< Actual aperture in Q16 format, 0 if N.A. */
    OMX_BOOL bNDFilter;         /**< True if ND filter was used */
    OMX_U32 nSensitivity;       /**< Actual ISO setting, 0 if if N.A. */

    //flash
    OMX_BOOL bFlashFired;               /**< True if flash was fired */
    OMX_U16 nFlashPowerDuringCapture;   /**< Used flash power */
    OMX_U16 nFlashCtrlPulseInUsec;      /**< Used flash length */

    //other capture related data
    OMX_S32 xGamma;                /**< Gamma value in Q16 format */
    OMX_U8  nGammaLUT[256];        /**< Gamma LUT table*/
    OMX_S32 nRGB2RGBColorConversion[9];     /**< Color conversion matrix */
    OMX_S32 xGainG;             /**< WB Gain for G in Q16 format*/
    OMX_S32 xGainR;             /**< WB Gain for R in Q16 format*/
    OMX_S32 xGainB;             /**< WB Gain for B in Q16 format*/
    OMX_S32 nOffsetG;           /**< G offset */
    OMX_S32 nOffsetR;           /**< R offset */
    OMX_S32 nOffsetB;           /**< B offset */
    OMX_S32 nColorTemperature;  /**< Color temperature, -1 if N.A. */
    OMX_S32 xRG;                /**< RG data in Q16 format*/
    OMX_S32 xBG;                /**< BG data in Q16 format*/
    OMX_S32 nContrast;          /**< Actual contrast value */
    OMX_U32 nBrightness;        /**< Actual brightness value */
    OMX_S32 nSaturation;        /**< Actual saturation value */
    OMX_S32 nSharpness;         /**< Actual sharpness value */
    
    OMX_TICKS nTimestamp;   /**< Timestamp of the frame - reference time: moving camera to executing state */
    OMX_U32 nFrameNum;      /**< Frame number - number of frames streamed out from given port */

} OMX_SYMBIAN_CAPTUREPARAMETERSTYPE;


/**
 * Generic, opaque extra data for passing feedback values from analyzers to other components
 * downstream from camera. 
 */
typedef struct OMX_SYMBIAN_EXTRADATA_FEEDBACKTYPE {
    OMX_VERSIONTYPE nExtraDataVersion;          /**< Extradata version information */
    OMX_U32 nSize;                              /**< Size of the structure in bytes */
    OMX_TICKS nOriginatorTimestamp;             /**< Timestamp of the frame the analysis was performed on */
    OMX_U32 nOriginatorFrameNum;                /**< Frame number on which the analysis was performed on*/
    OMX_SYMBIAN_FEEDBACKTYPE nFeedbackType;     /**< An enum to differentiate among different feedback types*/
    OMX_U32 nFeedbackValueSize;                 /**< The size of provided feedback data (in bytes)*/
    OMX_U8 nFeedbackValue[1];                   /**< Feedback data in byte buffer*/
} OMX_SYMBIAN_EXTRADATA_FEEDBACKTYPE;


/** 
 * Focus Status extra data.
*/
typedef struct OMX_SYMBIAN_EXTRADATA_EXTFOCUSSTATUSTYPE { 
    OMX_VERSIONTYPE nExtraDataVersion;/**< Extradata version information */
    OMX_TICKS nTimestamp;             /**< Timestamp of the frame - reference time: moving camera to executing state */
    OMX_U32 nFrameNum;                /**< Frame number - number of frames streamed out from given port */
    OMX_BOOL bFocused;                /**< Boolean indicating whether any of the focus 
                                           regions is currently succesfully focused */
    OMX_U32 nAFAreas;                 /**< Number of AF ROIs included in this extra data*/
    OMX_SYMBIAN_AFROITYPE sAFROIs[1]; /**< List of focus statuses and other info for AF regions.*/
} OMX_SYMBIAN_EXTRADATA_EXTFOCUSSTATUSTYPE;

/* Structures for passing data to EXIF builder */
typedef enum {
   OMX_SYMBIAN_ResolutionUnit_NoUnit,
   OMX_SYMBIAN_ResolutionUnit_Inch,
   OMX_SYMBIAN_ResolutionUnit_Centimeter
} OMX_SYMBIAN_RESOLUTION_UNITTYPE;

typedef enum {
   OMX_SYMBIAN_YCbCrPositioning_Unknown,
   OMX_SYMBIAN_YCbCrPositioning_Centered,
   OMX_SYMBIAN_YCbCrPositioning_CoSited
} OMX_SYMBIAN_YCBCR_POSITIONINGTYPE;

typedef enum {
   OMX_SYMBIAN_ComponentsConfiguration_RGB,
   OMX_SYMBIAN_ComponentsConfiguration_YCbCr
} OMX_SYMBIAN_COMPONENTS_CONFIGURATIONTYPE;

typedef enum {
   OMX_SYMBIAN_ColorSpace_Uncalibrated,
   OMX_SYMBIAN_ColorSpace_sRGB
} OMX_SYMBIAN_COLOR_SPACETYPE;

typedef struct OMX_SYMBIAN_CONFIG_EXIFSETTINGSTYPE {
    OMX_U32 nSize;                       /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;            /**< Version information */
    OMX_BUFFERHEADERTYPE* pOmxBufHdr;    /**< Input buffer to be read */
	const char*                       pManufacturer;
	const char*                       pModel;
	const char*			              pSoftware;
	OMX_SYMBIAN_DATE_TIMETYPE         sDateTime;
	OMX_SYMBIAN_ORIENTATIONORIGINTYPE eOrientation;
	OMX_U32                           nXResolution;
	OMX_U32                           nYResolution;
	OMX_SYMBIAN_RESOLUTION_UNITTYPE   eResolutionUnit;
	OMX_SYMBIAN_YCBCR_POSITIONINGTYPE eYCbCrPositioning;
	OMX_U32                           nSensitivity;
	OMX_SYMBIAN_COMPONENTS_CONFIGURATIONTYPE eComponentsConfig;
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
	OMX_SYMBIAN_COLOR_SPACETYPE       eColorSpace;
} OMX_SYMBIAN_CONFIG_EXIFSETTINGSTYPE;

typedef struct OMX_SYMBIAN_SUPPORTED_ZOOM_RESOLUTIOINS {
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nSize;
    OMX_U32 nPreviewHeight;
    OMX_U32 nPreviewWidth;
    OMX_U32 nPictureHeight;
    OMX_U32 nPictureWidth;
} OMX_SYMBIAN_SUPPORTED_ZOOM_RESOLUTIOINS;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_CameraExt_h */
