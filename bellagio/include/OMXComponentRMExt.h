/**
  src/base/OMXComponentRMExt.h

  This extension of the core provides functions for the resource manager used to retrieve
	the quality levels of the components available.

  Copyright (C) 2008-2010 STMicroelectronics

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#ifndef __OMXCOMPONENTRMEXT_H__
#define __OMXCOMPONENTRMEXT_H__

#include <OMX_Core.h>
/**
 * @file
 * @brief Extended OMXIL component interface for quality-of-service
 */

/**
 * @brief Returns the currently installed quality level of a OMXIL component
 * instance
 * @param hComponent [IN] Component handle
 * @param pQualityLevel [OUT] Current quality level
 * @return OMX_ErrorNone
 * @remark This function should move to a OMX_GetParameter call with appropriate
 * structure.
 */
OMX_ERRORTYPE getQualityLevel(OMX_HANDLETYPE hComponent, OMX_U32* pQualityLevel);
/**
 * @brief Sets the quality level on a OMXIL component instance
 * @param hComponent [IN] Component handle
 * @param nQualityLevel [IN] New quality level
 * @return OMX_ErrorNone
 * @remark This function should move to a OMX_GetParameter call with appropriate
 * structure.
 */
OMX_ERRORTYPE setQualityLevel(OMX_HANDLETYPE hComponent, OMX_U32 nQualityLevel);


#endif

