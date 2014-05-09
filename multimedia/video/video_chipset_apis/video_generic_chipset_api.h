/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef VIDEO_GENERIC_CHIPSET_API_H
#define VIDEO_GENERIC_CHIPSET_API_H


#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Component.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include <openmax/il/khronos/v1_x/OMX_IVCommon.h>
#include <openmax/il/shai/OMX_Symbian_IVCommonExt.h>
#include <openmax/il/shai/OMX_Symbian_ExtensionNames.h>
#include <openmax/il/shai/OMX_Symbian_ComponentExt.h>
#include <openmax/il/shai/OMX_Symbian_CameraExt.h>
#else
#include "OMX_Component.h"
#include "OMX_Types.h"
#include "OMX_IVCommon.h"
#include "OMX_Symbian_IVCommonExt_Ste.h"
#include "OMX_Symbian_ExtensionNames_Ste.h"
#include "OMX_Symbian_ComponentExt_Ste.h"
#include "OMX_Symbian_CameraExt_Ste.h"
#endif
#include "OMX_Symbian_VideoExt_Ste.h"

// FIXME: SHAI_FIX should be removed soon. It is there because the SHAI is not uptodate
// with respect to the doc of Laurent
#define SHAI_FIX
#ifdef SHAI_FIX

// FIXME: use OMX_SYMBIAN_COLORPRIMARYTYPE instead
typedef enum OMX_COLORPRIMARYTYPE  {
    OMX_ColorPrimaryUnknown=0,          // Default value
    OMX_ColorPrimaryBT601FullRange,     // YUV range from 0 to 255 with BT.601 defined YUV to RGB color conversion matrix
    OMX_ColorPrimaryBT601LimitedRange,  // YUV range from 16 to 235 or 240 for chrominance with BT.601 defined YUV to RGB color conversion matrix
    OMX_ColorPrimaryBT709FullRange,     // YUV range from 0 to 255 with BT.709 defined YUV to RGB color conversion matrix
    OMX_ColorPrimaryBT709LimitedRange   // YUV range from 16 to 235 or 240 for chrominance with BT.709 defined YUV to RGB color conversion matrix
} OMX_COLORPRIMARYTYPE;

#define OMX_CONFIG_COLORPRIMARY OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE
#define OMX_CONFIG_GPSLOCATIONTYPE OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE
#define OMX_VIDEO_PARAM_SEQUENCEHEADER  OMX_SYMBIAN_VIDEO_PARAM_SEQUENCEHEADERTYPE
#define OMX_VIDEO_DIGITALVIDEOSTABTYPE OMX_SYMBIAN_DIGITALVIDEOSTABTYPE
#define OMX_PARAM_COLORPRIMARYTYPE OMX_SYMBIAN_PARAM_COLORPRIMARYTYPE
// To be included once it is part of SHAI
//#define OMX_PARAM_PIXELASPECTRATIO OMX_SYMBIAN_PARAM_PIXELASPECTRATIO


typedef enum VFM_COLOR_FORMATTYPE {
    /// @brief 420 YUV MacroBlock Tiled ST Proprietary format
    /// @hideinitializer
    //OMX_COLOR_FormatYUV420MBPackedSemiPlanar  = OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar,
    // Done as a Patch to support bith values of MBtiled Planar format
    OMX_COLOR_FormatYUV420MBPackedSemiPlanar = (0x7FFFFFFF-1),
} VFM_COLOR_FORMATTYPE;
    // OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar should be used instead of OMX_COLOR_FormatYUV420MBPackedSemiPlanar

// OMX_PARAM_PIXELASPECTRATIO is added here
// It should be part of OMX_Symbian_IVCommonExt.h and OMX_Symbian_IVCommonExt_Ste.h file
/**<Reserved android opaque colorformat. Tells the encoder that the actual colorformat will be  relayed by the Gralloc Buffers.
FIXME: In the process of reserving some enum values for Android-specific OMX IL colorformats>**/

#define OMX_COLOR_FormatAndroidOpaque 0x7F000789

typedef struct OMX_PARAM_PIXELASPECTRATIO
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U8 nHorizontal;
    OMX_U8 nVertical;
} OMX_PARAM_PIXELASPECTRATIO;




#endif  // SHAI_FIX


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully
 */

/*---------------------------------------------------------*
		              ENUMERATIONS
 *---------------------------------------------------------*/

typedef enum OMX_3DFORMATTYPE {
    /*Currently used in h264dec SEI*/
    OMX_3DLeftView                 = 0x1,
    OMX_3DRightView                = 0x2,
    OMX_3DInterleaveLeftViewFirst  = 0x4,
    OMX_3DInterleaveRightViewFirst = 0x8,
    OMX_3DTopBottomLeftViewFirst   = 0x10,
    OMX_3DTopBottomRightViewFirst  = 0x20,
    OMX_3DSideBySideLeftViewFirst  = 0x40,
    OMX_3DSideBySideRightViewFirst = 0x80,
    /*Buffer format defined in PortDefinitionParam*/
    OMX_COLOR_Format3DInterleaveLeftViewFirst = OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+1,
    OMX_COLOR_Format3DInterleaveRightViewFirst = OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+2,
    OMX_COLOR_Format3DTopBottomHalfLeftViewFirst =OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+3,
    OMX_COLOR_Format3DTopBottomHalfRightViewFirst =OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+4,
    OMX_COLOR_Format3DTopBottomFullLeftViewFirst =OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+5,
    OMX_COLOR_Format3DTopBottomFullRightViewFirst =OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+6,
    OMX_COLOR_Format3DSideBySideLeftViewFirst =OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+7,
    OMX_COLOR_Format3DSideBySideRightViewFirst =OMX_SYMBIAN_COLOR_FormatYUV420MBPackedSemiPlanar+8
} OMX_3DFORMATTYPE;



/** End of Dummy component **/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

