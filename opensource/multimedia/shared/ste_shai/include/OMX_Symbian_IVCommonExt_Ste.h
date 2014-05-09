/*
  OMX_Symbian_IVCommonExt.h
  
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
@brief Symbian OpenMAX IL Extension Data Structures Common for Imaging and Video.

This file contains the extension structures for the Symbian IL extensions that
are common to Imaging and Video domains.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_IVCommonExt_h
#define OMX_Symbian_IVCommonExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#include <openmax/il/khronos/v1_x/OMX_IVCommon.h>
#include <openmax/il/khronos/v1_x/OMX_Image.h>
#else
#include <OMX_Types.h>
#include <OMX_Core.h>
#include <OMX_IVCommon.h>
#include <OMX_Image.h>
#endif

/* Extensions to standard enums */
/* ---------------------------- */

 
/**
 * Offset to Symbian extensions
 */
#define OMX_SYMBIAN_IV_COMMON_EXTENSIONS_START_OFFSET 0x00A00000
#define OMX_SYMBIAN_CAMERA_EXTENSIONS_START_OFFSET 0x00B00000
#define OMX_SYMBIAN_VIDEO_EXTENSIONS_START_OFFSET 0x00C00000

/* Extended generic types */
/* ---------------------- */
 

/** 
 * Generic point type
 * Point coordinates in 2D space
*/
typedef struct OMX_SYMBIAN_POINTTYPE {
    OMX_S32 nX; 
    OMX_S32 nY;
} OMX_SYMBIAN_POINTTYPE;

/** 
 * Generic size type
 * Size of rectangle in 2D space
*/
typedef struct OMX_SYMBIAN_SIZETYPE {
    OMX_S32 nWidth;
    OMX_S32 nHeight;
} OMX_SYMBIAN_SIZETYPE;

/** 
 * Generic rectangle type.
 * Rectangle represented by top left corner coordinates
 * and the size of the rectangle
 * There exist also a rectangle config defined in the OMX IL standard.
 * The purpose of this addition is to provide a generic rectangle type to 
 * be used by other configs and params.
*/
typedef struct OMX_SYMBIAN_RECTTYPE {
    OMX_SYMBIAN_POINTTYPE sTopLeft;
    OMX_SYMBIAN_SIZETYPE sSize;    
} OMX_SYMBIAN_RECTTYPE;

/** 
 * Relative rectangle type.
 * The purpose of this type is to represent a rectangular
 * region relative to certain reference size.
 * 
 * Example:
 * The location of an object detected in camera scene can be represendted
 * with relative rect. Let the object be located in rectangular are which has
 * top left coordintes (10,20) and size (90,120). The object detection algorithm
 * has used 320x240 sized image for analysis and that will be used as the 
 * reference size in the relative rect. The object location in 2 megapixel 
 * (1600x1200 pixels) captured image can be calculated by scaling the top left 
 * X-coordinate and rectangle width with reference size's width's faction 1600/320=5. 
 * Likewise for the Y-coordinate and height 1200/240=5. In the captured image 
 * the object is located inside a rectangular region that has top left coordinates 
 * (10*5, 20*5) = (50, 100) and size (90*5, 120*5) = (450, 600).      
*/
typedef struct OMX_SYMBIAN_RELATIVERECTTYPE {
    OMX_SYMBIAN_RECTTYPE sRect;      /**< Rectangle represented as concerete top left coordinates and size */
    OMX_SYMBIAN_SIZETYPE sReference; /**< Reference size. Since sRect is relative to this reference size
                                          it can be scaled up or down to represent the same rectangular 
                                          region in relation to bigger or smaller reference sizes */
} OMX_SYMBIAN_RELATIVERECTTYPE;



/* Extended generic config and param structs */
/* ----------------------------------------- */
 

/** 
 * Generic signed 32-bit integer config struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_S32TYPE { 
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_S32 nValue;             /**< signed 32-bit value */
} OMX_SYMBIAN_CONFIG_S32TYPE; 

/** 
 * Generic unsigned 32-bit integer param struct.
*/
typedef OMX_SYMBIAN_CONFIG_S32TYPE OMX_SYMBIAN_PARAM_S32TYPE; 

/** 
 * Generic unsigned 32-bit integer config struct.
*/
typedef struct OMX_SYMBIAN_CONFIG_U32TYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_U32 nValue;             /**< Unsigned 32-bit value */
} OMX_SYMBIAN_CONFIG_U32TYPE; 

/** 
 * Generic unsigned 32-bit integer param struct.
*/
typedef OMX_SYMBIAN_CONFIG_U32TYPE OMX_SYMBIAN_PARAM_U32TYPE; 

/** 
 * Port specific generic boolean config struct
 * OMX spec already has boolean config but
 * without port index
*/
typedef struct OMX_SYMBIAN_CONFIG_BOOLEANTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_BOOL bEnabled;          /**< Boolean value */
} OMX_SYMBIAN_CONFIG_BOOLEANTYPE; 

/** 
 * Generic point config struct
*/
typedef struct OMX_SYMBIAN_CONFIG_POINTTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_POINTTYPE sPoint; /**< Point coordinates */
} OMX_SYMBIAN_CONFIG_POINTTYPE; 



/* Extended params and configs */
/* --------------------------- */

/** 
 * Enumeration of possible flicker removal values 
 */
typedef enum OMX_SYMBIAN_FLICKERREMOVALTYPE {
    OMX_SYMBIAN_FlickerRemovalOff,  /**< Flicker removal disabled */
    OMX_SYMBIAN_FlickerRemovalAuto, /**< Automatically detected AC frequency */
    OMX_SYMBIAN_FlickerRemoval50,   /**< Flicker removal for 50 Hz AC frequency */
    OMX_SYMBIAN_FlickerRemoval60,   /**< Flicker removal for 60 Hz AC frequency */
    OMX_SYMBIAN_FlickerRemovalMax = 0x7FFFFFFF
} OMX_SYMBIAN_FLICKERREMOVALTYPE;

/** 
 * Flicker Removal.
*/
typedef struct OMX_SYMBIAN_CONFIG_FLICKERREMOVALTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
   OMX_SYMBIAN_FLICKERREMOVALTYPE eFlickerRemoval; /**< Flicker removal mode */
}OMX_SYMBIAN_CONFIG_FLICKERREMOVALTYPE;

/**
 * Classification of algorithm comlexity values
 */
typedef enum OMX_SYMBIAN_ALGORITHMCOMPLEXITYTYPE {
    OMX_SYMBIAN_AlgComplexityLow,
    OMX_SYMBIAN_AlgComplexityMedium,
    OMX_SYMBIAN_AlgComplexityHigh,
    OMX_SYMBIAN_AlgComplexityMax = 0x7FFFFFFF
} OMX_SYMBIAN_ALGORITHMCOMPLEXITYTYPE;

/**
 * Algorithm complexity config
 */
typedef struct OMX_SYMBIAN_CONFIG_ALGORITHMCOMPLEXITYTYPE {
    OMX_U32 nSize;              /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< Port that this structure applies to */
    OMX_SYMBIAN_ALGORITHMCOMPLEXITYTYPE eComplexity; /**< Algorithm complexity to use */
} OMX_SYMBIAN_CONFIG_ALGORITHMCOMPLEXITYTYPE;

/** Color format extensions. */
typedef enum OMX_SYMBIAN_COLOR_FORMATTYPE {
    OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar = OMX_COLOR_FormatVendorStartUnused + OMX_SYMBIAN_IV_COMMON_EXTENSIONS_START_OFFSET,
    OMX_SYMBIAN_COLOR_FormatRawBayer12bit,
    OMX_STE_COLOR_FormatYUV420MBSwap64bPackedSemiPlanar,
    OMX_STE_COLOR_FormatYUV420PackedSemiPlanar_NV21,
    OMX_STE_COLOR_FormatYUV420PackedSemiPlanar_NV12,
    OMX_STE_COLOR_FormatRawData,
    OMX_SYMBIAN_COLOR_FormatMax = 0x7FFFFFFF
} OMX_SYMBIAN_COLOR_FORMATTYPE;

/** Primary color enumeration. */
typedef enum OMX_SYMBIAN_COLORPRIMARYTYPE {
	OMX_SYMBIAN_ColorPrimaryUnknownRange,
    OMX_SYMBIAN_ColorPrimaryBT601FullRange,
    OMX_SYMBIAN_ColorPrimaryBT601LimitedRange,
    OMX_SYMBIAN_ColorPrimaryBT709FullRange,
    OMX_SYMBIAN_ColorPrimaryBT709LimitedRange,
    OMX_SYMBIAN_ColorMax = 0x7FFFFFFF
} OMX_SYMBIAN_COLORPRIMARYTYPE;

/** YUV data color range.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Param.Common.ColorPrimary".
*/
typedef struct OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_SYMBIAN_COLORPRIMARYTYPE eColorPrimary;
} OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE;


/** Extradata type extensions. 

The index used to query or enable the generation of this payload is retrieved using
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Common.ExtraVideoStabilizationData".
*/
typedef enum OMX_SYMBIAN_EXTRADATATYPE {
    OMX_SYMBIAN_ExtraDataVideoStabilization = OMX_ExtraDataVendorStartUnused + OMX_SYMBIAN_IV_COMMON_EXTENSIONS_START_OFFSET,
    OMX_SYMBIAN_ExtraDataMax = 0x7FFFFFFF
} OMX_SYMBIAN_EXTRADATATYPE;

/** Video stabilization extradata content struct */
typedef struct OMX_SYMBIAN_DIGITALVIDEOSTABTYPE {
    OMX_BOOL bState;   			//**< The state of digital image stabilization */
    OMX_U32 nTopLeftCropVectorX; 	//**< Horizontal coordinate of the crop vector from top left angle, absolute wrt overscanning width  */
    OMX_U32 nTopLeftCropVectorY; 	//**< Vertical coordinate of the crop vector from top left angle, absolute wrt overscanning height  */
    OMX_U32 nMaxOverscannedWidth; 	//**<  Max W+30% */
    OMX_U32 nMaxOverscannedHeight; 	//**<  Max H+30% */
} OMX_SYMBIAN_DIGITALVIDEOSTABTYPE;


/** Pixel Aspect Ratio */
typedef struct OMX_SYMBIAN_PARAM_PIXELASPECTRATIOTYPE {
	OMX_U32 nSize;				/**< Size of the structure in bytes */
	OMX_VERSIONTYPE nVersion;	/**< OMX specification version information */
	OMX_U32 nPortIndex;			/**< Port that this structure applies to */
	OMX_U8 nHorizontal;			/**< Width of pixel. nHorizontal is 4 is For 4:3 PAR*/
	OMX_U8 nVertical;			/**< Height of pixel. nVertical is 3 is For 4:3 PAR */
} OMX_SYMBIAN_PARAM_PIXELASPECTRATIOTYPE;

typedef struct {
	OMX_U16 year;
	OMX_U8  month;
	OMX_U8  day;
	OMX_U8  hour;
	OMX_U8  minutes;
	OMX_U8  seconds;
} OMX_SYMBIAN_DATE_TIMETYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_IVCommonExt_h */
