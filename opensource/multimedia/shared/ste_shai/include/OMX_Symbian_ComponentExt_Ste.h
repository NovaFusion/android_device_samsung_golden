/*
  OMX_Symbian_ComponentExt.h
  
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
@brief Symbian OpenMAX IL Extension Data Structures for the 'Component' types.

This file contains the extension structures for the Symbian IL extensions that
pertain to common component functionality.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_ComponentExt_h
#define OMX_Symbian_ComponentExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#else
#include <OMX_Types.h>
#endif

/** @name Deferred mode control */

/*@{*/

/** Commit mode selection struct. 

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.CommitMode".
*/
typedef struct OMX_SYMBIAN_CONFIG_COMMITMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bDeferred;
} OMX_SYMBIAN_CONFIG_COMMITMODETYPE;

/** Explicit commit struct.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Commit".
*/
typedef struct OMX_SYMBIAN_CONFIG_COMMITTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
} OMX_SYMBIAN_CONFIG_COMMITTYPE;

/*@}*/

/** @name Thread Priority Control */

/*@{*/

/** Symbian OS thread priority control.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.SymbianThreadPriority".
*/
typedef struct OMX_SYMBIAN_CONFIG_SYMBIANTHREADPRIORITYTYPE {
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nThreadID;          /**< Implementation specific thread ID */
    OMX_S32 nThreadPriority;    /**< The actual priority value */
} OMX_SYMBIAN_CONFIG_SYMBIANTHREADPRIORITYTYPE;

/*@}*/

/** @name Shared Chunk Metadata Extension */

/*@{*/

/** Symbian OS shared chunk metadata config type.

The index specified for this structure is retrieved using 
OMX_GetExtensionIndex() with the extension string 
"OMX.Symbian.Index.Config.Sharedchunkmetadata".
*/
typedef struct OMX_SYMBIAN_CONFIG_SHAREDCHUNKMETADATATYPE
{
    OMX_U32 nSize;             /**< Size of this structure, in Bytes */
    OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
    OMX_U32 nPortIndex;        /**< Port that this structure applies to */
    OMX_U32 nHandleId;         /**< This is a handle id of the shared chunk */
    OMX_U64 nOwnerThreadId;    /**< This is thread that contains the handle */
} OMX_SYMBIAN_CONFIG_SHAREDCHUNKMETADATATYPE;

/*@}*/

/** @name GPS Positioning Extension */

/*@{*/

/** GPS location config.
*/
/* New structure of GPS being added here for ER348736
This new structure is currently used only in MBL environment and
NOT in MBS so it is added here.
Change involves addition of new field timestamp
This structure is not part of SHAI but as it is common structure between
imaging and vidoe so it has to be defined here
OMX_CONFIG_EXTENDEDGPSLOCATIONTYPE new structure for MBL
exsting OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE for MBS
*/
#ifdef __SYMBIAN32__
typedef struct OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE {    
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U64 nLatitudeDegrees;
    OMX_U64 nLatitudeMinutes;
    OMX_U64 nLatitudeSeconds;
    OMX_U64 nLongitudeDegrees;
    OMX_U64 nLongitudeMinutes;
    OMX_U64 nLongitudeSeconds;
    OMX_U64 nAltitudeMeters;
    OMX_BOOL bLatitudeRefNorth;
    OMX_BOOL bLongitudeRefEast;
    OMX_BOOL bAltitudeRefAboveSea;
    OMX_BOOL bLocationKnown;  
} OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE;
#else

/* new enum added for ER 361637 */
typedef enum OMX_KEY_GPS_PROCESSING_METHODTYPE {
    OMX_Undefined_Format = 0x00000000,
	OMX_ASCII_Format,
    OMX_JIS_Format,
    OMX_Unicode_Format
} OMX_KEY_GPS_PROCESSING_METHODTYPE;

/* Two new members processingValue and nPrMethodDataValue have been added
to support GPS Tag KEY_GPS_PROCESSING_METHOD - ER 361637 */

typedef struct OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U64 nLatitudeDegrees;
    OMX_U64 nLatitudeMinutes;
    OMX_U64 nLatitudeSeconds;
    OMX_U64 nLongitudeDegrees;
    OMX_U64 nLongitudeMinutes;
    OMX_U64 nLongitudeSeconds;
    OMX_U64 nAltitudeMeters;
    OMX_BOOL bLatitudeRefNorth;
    OMX_BOOL bLongitudeRefEast;
    OMX_BOOL bAltitudeRefAboveSea;
    OMX_U64  nHours;
    OMX_U64  nMinutes;
    OMX_U64  nSeconds;
    OMX_KEY_GPS_PROCESSING_METHODTYPE processingValue;
    OMX_U8 nPrMethodDataValue[32];
    //These three fields nYear,nMonth and nDay are added for ER 429860
    OMX_U64 nYear;
    OMX_U64 nMonth;
    OMX_U64 nDay;
    OMX_BOOL bLocationKnown;
} OMX_SYMBIAN_CONFIG_GPSLOCATIONTYPE;

#endif

/*@}*/

/** @name Pre-emption Policy Extensions */

/*@{*/

/** Pre-emption Policy enum.
*/
typedef enum OMX_SYMBIAN_PREEMPTIONPOLICYTYPE {
    OMX_SYMBIAN_PreemptionRequestDisabled = 0x00000000, /**< Default behavior: No preemption request event; standard v1.0 behavior to be followed */
    OMX_SYMBIAN_PreemptionRequestEnabled = 0x00000001,  /**< Preemption request sent by component, assumed that there are no automatic state transitions */
    OMX_SYMBIAN_PreemptionPolicyMax = 0x7fffffff
} OMX_SYMBIAN_PREEMPTIONPOLICYTYPE;

/** Pre-emption Policy parameter structure

The index specified for this structure is retrieved using
OMX_GetExtensionIndex() with the extension string
"OMX.Symbian.Index.Param.PreemptionPolicy".
*/
typedef struct OMX_SYMBIAN_PARAM_PREEMPTIONPOLICYTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_SYMBIAN_PREEMPTIONPOLICYTYPE ePolicy;
} OMX_SYMBIAN_PARAM_PREEMPTIONPOLICYTYPE;

/*@}*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_ComponentExt_h */
