/**
  src/omx_reference_resource_manager.h

  This simple resource manager emulates the behavior of a real RM.
	It applies the rules defined in the OpenMAX spec. It can be replaced in
	the future by a real system.

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

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

#ifndef _OMX_REFERENCE_RESOURCE_MANAGER_H_
#define _OMX_REFERENCE_RESOURCE_MANAGER_H_

#include "omxcore.h"

typedef struct ComponentListType ComponentListType;
struct ComponentListType {
	OMX_COMPONENTTYPE *openmaxStandComp;
	OMX_U32 nGroupPriority;
	OMX_U32 timestamp;
	ComponentListType* next;
};

OMX_ERRORTYPE addElemToList(ComponentListType **list, OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE removeElemFromList(ComponentListType **list, OMX_COMPONENTTYPE *openmaxStandComp);
int numElemInList(ComponentListType *list);
OMX_ERRORTYPE clearList(ComponentListType **list);

/** Debug flags
 */
#define RM_SHOW_NAME 0x01
#define RM_SHOW_ADDRESS 0x02

OMX_ERRORTYPE RM_Init();
OMX_ERRORTYPE RM_Deinit();
OMX_ERRORTYPE RM_getResource(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE RM_releaseResource(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE RM_waitForResource(OMX_COMPONENTTYPE *openmaxStandComp);
OMX_ERRORTYPE RM_removeFromWaitForResource(OMX_COMPONENTTYPE *openmaxStandComp);
void RM_printList(ComponentListType *list, int viewFlag);

#endif
