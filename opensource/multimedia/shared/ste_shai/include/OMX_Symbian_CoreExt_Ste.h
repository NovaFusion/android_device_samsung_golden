/*
  OMX_Symbian_CoreExt.h
  
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
@brief Symbian OpenMAX IL Extension Data Structures for the 'Core' types.

This file contains the extension structures for the Symbian IL extensions that
pertain to core functionality.

@publishedDeviceAbstraction
*/

#ifndef OMX_Symbian_CoreExt_h
#define OMX_Symbian_CoreExt_h

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


/** Extensions to the standard IL errors. */
typedef enum OMX_SYMBIAN_ERRORTYPE
{
    OMX_Symbian_ErrorInvalidMode = (OMX_S32) (OMX_ErrorVendorStartUnused + 0x00A00001),
    OMX_Symbian_ErrorPreemptResourcesRequest, /** Component signals its resources have been pre-empted */
    OMX_Symbian_ErrorMax = 0x7FFFFFFF
} OMX_SYMBIAN_ERRORTYPE;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_Symbian_CoreExt_h */
