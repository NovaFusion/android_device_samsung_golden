/*
  OMX_Symbian_OtherExt.h
  
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
@brief Symbian OpenMAX IL Extension Data Structures for the 'Other' Domain.

This file contains the extension structures for the Symbian IL extensions in the
'Other' domain.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_OtherExt_h
#define OMX_Symbian_OtherExt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully 
 */
#ifdef __SYMBIAN32__
#include <openmax/il/khronos/v1_x/OMX_Core.h>
#else
#include <OMX_Core.h>
#endif


/** @name Extradata extension */

/*@{*/

/** Extradata base type.

This is a helper struct that can be used as part of the definitions of
Symbian-specific extra data types. It is exactly the same as the standard
OMX_OTHER_EXTRADATATYPE save for the missing data field.
*/
typedef struct OMX_SYMBIAN_OTHER_EXTRADATABASETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_EXTRADATATYPE eType;
    OMX_U32 nDataSize;
} OMX_SYMBIAN_OTHER_EXTRADATABASETYPE;

/*@}*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_OtherExt_h */
