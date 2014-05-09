/**
  src/OMXComponentRMExt.c

  This extension of the core provides functions for the resoruce manager used to retrieve
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

#include "omxcore.h"
#include "omx_base_component.h"
#include "OMXComponentRMExt.h"

OMX_ERRORTYPE getQualityLevel(OMX_HANDLETYPE hComponent, OMX_U32* pQualityLevel) {
	omx_base_component_PrivateType* omx_base_component_Private;
	OMX_COMPONENTTYPE *openmaxStandComp = (OMX_COMPONENTTYPE *)hComponent;
	OMX_ERRORTYPE err;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
	if (omx_base_component_Private == NULL) {
		DEBUG(DEB_LEV_ERR, "The function %s should be called on a Bellagio OpenMAX component\n", __func__);
		return OMX_ErrorNotImplemented;
	}
	err = omx_base_component_Private->getQualityLevel(openmaxStandComp, pQualityLevel);
	if (err != OMX_ErrorNone) {
		DEBUG(DEB_LEV_ERR, "The get Quality levels fails\n");
		return err;
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE setQualityLevel(OMX_HANDLETYPE hComponent, OMX_U32 nQualityLevel) {
	omx_base_component_PrivateType* omx_base_component_Private;
	OMX_COMPONENTTYPE *openmaxStandComp = (OMX_COMPONENTTYPE *)hComponent;
	OMX_ERRORTYPE err;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
	if (omx_base_component_Private == NULL) {
		DEBUG(DEB_LEV_ERR, "The function %s should be called on a Bellagio OpenMAX component\n", __func__);
		return OMX_ErrorNotImplemented;
	}
	err = omx_base_component_Private->setQualityLevel(openmaxStandComp, nQualityLevel);
	if (err != OMX_ErrorNone) {
		DEBUG(DEB_LEV_ERR, "The set Quality levels fails\n");
		return err;
	}
	return OMX_ErrorNone;
}

