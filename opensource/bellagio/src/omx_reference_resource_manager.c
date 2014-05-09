/**
  src/omx_reference_resource_manager.c

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

#include <string.h>
#include "omx_reference_resource_manager.h"
#include "omx_base_component.h"
#include "queue.h"

/**
 * This is the static base pointer of the list
 */
static int globalTimestamp = 0;

/**
 * Components specific section
 * Each component that has some resources to be handled by this
 * resource manager should add some fields here
 */
/* Max allowable volume component instance */
static ComponentListType *volumeComponentList = NULL;
static ComponentListType *volumeWaitingList = NULL;
#define MAX_RESOURCE_VOLUME 10

/* Max allowable mixer component instance */
static ComponentListType *mixerComponentList = NULL;
static ComponentListType *mixerWaitingList = NULL;
#define MAX_RESOURCE_MIXER 10

/* Max allowable video scheduler component instance */
static ComponentListType *videoschedComponentList = NULL;
static ComponentListType *videoschedWaitingList = NULL;
#define MAX_RESOURCE_VIDEOSCHED 10

/* Max allowable video scheduler component instance */
static ComponentListType *clockComponentList = NULL;
static ComponentListType *clockWaitingList = NULL;
#define MAX_RESOURCE_CLOCK 5

/**
 * This function initializes the Resource manager. In the current implementation
 * it does not perform any operation
 */
OMX_ERRORTYPE RM_Init() {
	return OMX_ErrorNone;
}

/**
 * This function de-initializes the resource manager.
 * In the current implementation its responsibility is to clean up any queue
 * that can be left pending at the end of usage. With a correct use of the
 * resource manager it won't happen, but it is safer to clean up everything
 * this these lists are global and alive for all the life of IL client,
 * beyond the usual OMX_Init - Deinit scope.
 */
OMX_ERRORTYPE RM_Deinit() {
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	clearList(&volumeComponentList);
	clearList(&volumeWaitingList);
	clearList(&mixerComponentList);
	clearList(&mixerWaitingList);
	clearList(&videoschedComponentList);
	clearList(&videoschedWaitingList);
	clearList(&clockComponentList);
	clearList(&clockWaitingList);
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function adds a new element to a given list.
 * If it does not yet exists, this function also allocates the list.
 */
OMX_ERRORTYPE addElemToList(ComponentListType **list, OMX_COMPONENTTYPE *openmaxStandComp) {
	ComponentListType *componentTemp;
	ComponentListType *componentNext;
	omx_base_component_PrivateType* omx_base_component_Private;
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
	if (!*list) {
		*list = malloc(sizeof(ComponentListType));
		if (!*list) {
			DEBUG(DEB_LEV_ERR, "In %s OMX_ErrorInsufficientResources\n", __func__);
			return OMX_ErrorInsufficientResources;
		}
		(*list)->openmaxStandComp = openmaxStandComp;
		(*list)->timestamp = globalTimestamp;
		globalTimestamp++;
		(*list)->nGroupPriority = omx_base_component_Private->nGroupPriority;
		(*list)->next = NULL;
		DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
		return OMX_ErrorNone;
	}
	componentTemp = *list;
	while(componentTemp->next) {
		componentTemp = componentTemp->next;
	}
	componentNext = malloc(sizeof(ComponentListType));
	if (!componentNext) {
		DEBUG(DEB_LEV_ERR, "In %s OMX_ErrorInsufficientResources\n", __func__);
		return OMX_ErrorInsufficientResources;
	}
	componentTemp->next = componentNext;
	componentNext->next = NULL;
	componentNext->openmaxStandComp = openmaxStandComp;
	componentNext->timestamp = globalTimestamp;
	globalTimestamp++;
	componentNext->nGroupPriority = omx_base_component_Private->nGroupPriority;
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function removes the given element from the list, if present.
 * If the list is empty, this function cleans up everything.
 */
OMX_ERRORTYPE removeElemFromList(ComponentListType **list, OMX_COMPONENTTYPE *openmaxStandComp) {
	ComponentListType *componentTemp;
	ComponentListType *componentPrev;
	OMX_BOOL bFound = OMX_FALSE;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s list %x\n", __func__, (int)*list);
	if (!*list) {
		DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
		return OMX_ErrorUndefined;
	}
	componentTemp = *list;
	componentPrev = *list;
	while(componentTemp) {
		if (componentTemp->openmaxStandComp == openmaxStandComp) {
			if (componentTemp == *list) {
				*list = (*list)->next;
				free(componentTemp);
			} else {
				componentPrev->next = componentTemp->next;
				free(componentTemp);
			}
			 bFound = OMX_TRUE;
			 break;
		} else {
			if (componentTemp != *list) {
				componentPrev = componentPrev->next;
			}
			componentTemp = componentTemp->next;
		}
	}
	if(!bFound) {
		DEBUG(DEB_LEV_ERR, "In %s, the specified component does not exist\n", __func__);
		return OMX_ErrorComponentNotFound;
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;

}


/**
 * This function returns the number of elements present in the
 * list. If the list does not exists, this function returns 0 elements
 * without further warnings
 */
int numElemInList(ComponentListType *list) {
	ComponentListType *componentTemp;
	int numElem = 0;
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	if (!list) {
		DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s, no list no elements\n", __func__);
		return 0;
	}
	componentTemp = list;
	while(componentTemp) {
		numElem++;
		componentTemp = componentTemp->next;
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return numElem;
}

/**
 * This function deallocate any remaining element in a list
 * and dispose it
 */
OMX_ERRORTYPE clearList(ComponentListType **list) {
	ComponentListType *componentTemp;
	ComponentListType *componentPrev;
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	if (!*list) {
		DEBUG(DEB_LEV_FUNCTION_NAME, "In %s, no list no elements\n", __func__);
		return OMX_ErrorNone;
	}
	componentTemp = *list;
	while(componentTemp) {
		componentPrev = componentTemp;
		componentTemp = componentTemp->next;
		free(componentPrev);
	}
	*list = NULL;
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This debug function is capable of printing the full list
 * actually stored
 */
void RM_printList(ComponentListType *list, int viewFlag) {
	ComponentListType *componentTemp = list;
	omx_base_component_PrivateType* omx_base_component_Private;
	int index;

	if (!list) {
		printf("The list is empty\n");
		return;
	}
	index = 0;
	while (componentTemp) {
		omx_base_component_Private = (omx_base_component_PrivateType*)componentTemp->openmaxStandComp->pComponentPrivate;
		if ((viewFlag & RM_SHOW_NAME) == RM_SHOW_NAME) {
			printf("Name %s ", omx_base_component_Private->name);
		}
		if ((viewFlag & RM_SHOW_ADDRESS) == RM_SHOW_ADDRESS) {
			printf("Address %x ", (int)componentTemp->openmaxStandComp);
		}
		printf("\n");
		index++;
		componentTemp = componentTemp->next;
	}
}

/**
 * This function returns the number of components that have a lower priority
 * than the value specified, and the lowest among all possibles.
 * If the number returned is 0, no component is preemptable. if it is 1 or more,
 * the oldest_component_preemptable will contain the reference to the preemptable
 * component with the oldest time stamp.
 */
int searchLowerPriority(ComponentListType *list, int current_priority, ComponentListType **oldest_component_preemptable) {
	ComponentListType *componentTemp;
	ComponentListType *componentCandidate;
	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	int nComp = 0;
	if (!list) {
		DEBUG(DEB_LEV_ERR, "In %s no list\n", __func__);
		return OMX_ErrorUndefined;
	}
	componentTemp = list;
	componentCandidate = NULL;
	while (componentTemp) {
		if (componentTemp->nGroupPriority > current_priority) {
			nComp++;
		}
		if (nComp>0) {
			if (componentCandidate) {
				if (componentCandidate->timestamp > componentTemp->timestamp) {
					componentCandidate = componentTemp;
				}
			} else {
				componentCandidate = componentTemp;
			}
		}
		componentTemp = componentTemp->next;
	}
	*oldest_component_preemptable = componentCandidate;
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return nComp;
}

/**
 * This function tries to preempt the given component, that has been detected as
 * the candidate by the default policy defined in the OpenMAX spec.
 */
OMX_ERRORTYPE preemptComponent(OMX_COMPONENTTYPE *openmaxStandComp) {
	OMX_ERRORTYPE err;
	omx_base_component_PrivateType* omx_base_component_Private = openmaxStandComp->pComponentPrivate;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);

	if (omx_base_component_Private->state == OMX_StateIdle) {
        (*(omx_base_component_Private->callbacks->EventHandler))
          (openmaxStandComp, omx_base_component_Private->callbackData,
            OMX_EventError, OMX_ErrorResourcesLost, 0, NULL);
        err = OMX_SendCommand(openmaxStandComp, OMX_CommandStateSet, OMX_StateLoaded, NULL);
        if (err != OMX_ErrorNone) {
        	DEBUG(DEB_LEV_ERR, "In %s, the state cannot be changed\n", __func__);
        	return OMX_ErrorUndefined;
        }
    } else if ((omx_base_component_Private->state == OMX_StateExecuting) || (omx_base_component_Private->state == OMX_StatePause)) {
    	// TODO fill also this section that cover the preemption of a running component
    	// send OMX_ErrorResourcesPreempted
    	// change state to Idle
    	// send OMX_ErrorResourcesLost
    	// change state to Loaded
    }
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function is executed by a component when it changes state from Loaded to Idle.
 * If it return ErrorNone the resource is granted and it can transit to Idle.
 * In case the resource is already busy, the resource manager preempt another component
 * with a lower priority and a oldest time flag if it exists. Differently it returns OMX_ErrorInsufficientResources
 */
OMX_ERRORTYPE RM_getResource(OMX_COMPONENTTYPE *openmaxStandComp) {
	ComponentListType *componentCandidate;
	omx_base_component_PrivateType* omx_base_component_Private;
	int candidates;
	OMX_ERRORTYPE err;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
	if (!strcmp(omx_base_component_Private->name, "OMX.st.volume.component")) {
		if (numElemInList(volumeComponentList) >= MAX_RESOURCE_VOLUME) {
			candidates = searchLowerPriority(volumeComponentList, omx_base_component_Private->nGroupPriority, &componentCandidate);
			if (candidates) {
				DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s candidates %i winner %x\n", __func__, candidates, (int)componentCandidate->openmaxStandComp);
				err = preemptComponent(componentCandidate->openmaxStandComp);
				if (err != OMX_ErrorNone) {
					DEBUG(DEB_LEV_ERR, "In %s the component cannot be preempted\n", __func__);
					return OMX_ErrorInsufficientResources;
				} else {
					err = removeElemFromList(&volumeComponentList, componentCandidate->openmaxStandComp);
					err = addElemToList(&volumeComponentList, openmaxStandComp);
					if (err != OMX_ErrorNone) {
						DEBUG(DEB_LEV_ERR, "In %s memory error\n", __func__);
						return OMX_ErrorInsufficientResources;
					}
				}
			} else {
				DEBUG(DEB_LEV_ERR, "Out of %s\n", __func__);
				return OMX_ErrorInsufficientResources;
			}
		} else {
			err = addElemToList(&volumeComponentList, openmaxStandComp);
		}
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.audio.mixer")) {
		if (numElemInList(mixerComponentList) >= MAX_RESOURCE_MIXER) {
			candidates = searchLowerPriority(mixerComponentList, omx_base_component_Private->nGroupPriority, &componentCandidate);
			if (candidates) {
				DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s candidates %i winner %x\n", __func__, candidates, (int)componentCandidate->openmaxStandComp);
				err = preemptComponent(componentCandidate->openmaxStandComp);
				if (err != OMX_ErrorNone) {
					DEBUG(DEB_LEV_ERR, "In %s the component cannot be preempted\n", __func__);
					return OMX_ErrorInsufficientResources;
				} else {
					err = removeElemFromList(&mixerComponentList, componentCandidate->openmaxStandComp);
					err = addElemToList(&mixerComponentList, openmaxStandComp);
					if (err != OMX_ErrorNone) {
						DEBUG(DEB_LEV_ERR, "In %s memory error\n", __func__);
						return OMX_ErrorInsufficientResources;
					}
				}
			} else {
				DEBUG(DEB_LEV_ERR, "Out of %s\n", __func__);
				return OMX_ErrorInsufficientResources;
			}
		} else {
			err = addElemToList(&mixerComponentList, openmaxStandComp);
		}
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.video.scheduler")) {
		if (numElemInList(videoschedComponentList) >= MAX_RESOURCE_VIDEOSCHED) {
			candidates = searchLowerPriority(videoschedComponentList, omx_base_component_Private->nGroupPriority, &componentCandidate);
			if (candidates) {
				DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s candidates %i winner %x\n", __func__, candidates, (int)componentCandidate->openmaxStandComp);
				err = preemptComponent(componentCandidate->openmaxStandComp);
				if (err != OMX_ErrorNone) {
					DEBUG(DEB_LEV_ERR, "In %s the component cannot be preempted\n", __func__);
					return OMX_ErrorInsufficientResources;
				} else {
					err = removeElemFromList(&videoschedComponentList, componentCandidate->openmaxStandComp);
					err = addElemToList(&videoschedComponentList, openmaxStandComp);
					if (err != OMX_ErrorNone) {
						DEBUG(DEB_LEV_ERR, "In %s memory error\n", __func__);
						return OMX_ErrorInsufficientResources;
					}
				}
			} else {
				DEBUG(DEB_LEV_ERR, "Out of %s\n", __func__);
				return OMX_ErrorInsufficientResources;
			}
		} else {
			err = addElemToList(&videoschedComponentList, openmaxStandComp);
		}
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.clocksrc")) {
		if (numElemInList(clockComponentList) >= MAX_RESOURCE_CLOCK) {
			candidates = searchLowerPriority(clockComponentList, omx_base_component_Private->nGroupPriority, &componentCandidate);
			if (candidates) {
				DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s candidates %i winner %x\n", __func__, candidates, (int)componentCandidate->openmaxStandComp);
				err = preemptComponent(componentCandidate->openmaxStandComp);
				if (err != OMX_ErrorNone) {
					DEBUG(DEB_LEV_ERR, "In %s the component cannot be preempted\n", __func__);
					return OMX_ErrorInsufficientResources;
				} else {
					err = removeElemFromList(&clockComponentList, componentCandidate->openmaxStandComp);
					err = addElemToList(&clockComponentList, openmaxStandComp);
					if (err != OMX_ErrorNone) {
						DEBUG(DEB_LEV_ERR, "In %s memory error\n", __func__);
						return OMX_ErrorInsufficientResources;
					}
				}
			} else {
				DEBUG(DEB_LEV_ERR, "Out of %s\n", __func__);
				return OMX_ErrorInsufficientResources;
			}
		} else {
			err = addElemToList(&clockComponentList, openmaxStandComp);
		}
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function is called by a component when it transit from Idle to Loaded and can release
 * its used resource handled by the resource manager
 */
OMX_ERRORTYPE RM_releaseResource(OMX_COMPONENTTYPE *openmaxStandComp){
	omx_base_component_PrivateType* omx_base_component_Private;
	OMX_COMPONENTTYPE *openmaxWaitingComp;
	OMX_ERRORTYPE err;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
	if (!strcmp(omx_base_component_Private->name, "OMX.st.volume.component")) {
		if (!volumeComponentList) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
			return OMX_ErrorUndefined;
		}
		err = removeElemFromList(&volumeComponentList, openmaxStandComp);
		if (err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource cannot be released\n", __func__);
			return OMX_ErrorUndefined;
		}
		if(numElemInList(volumeWaitingList)) {
			openmaxWaitingComp = volumeWaitingList->openmaxStandComp;
			removeElemFromList(&volumeWaitingList, openmaxWaitingComp);
	        err = OMX_SendCommand(openmaxWaitingComp, OMX_CommandStateSet, OMX_StateIdle, NULL);
	        if (err != OMX_ErrorNone) {
	        	DEBUG(DEB_LEV_ERR, "In %s, the state cannot be changed\n", __func__);
	        }
		}
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.audio.mixer")) {
		if (!mixerComponentList) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
			return OMX_ErrorUndefined;
		}
		err = removeElemFromList(&mixerComponentList, openmaxStandComp);
		if (err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource cannot be released\n", __func__);
			return OMX_ErrorUndefined;
		}
		if(numElemInList(mixerWaitingList)) {
			openmaxWaitingComp = mixerWaitingList->openmaxStandComp;
			removeElemFromList(&mixerWaitingList, openmaxWaitingComp);
	        err = OMX_SendCommand(openmaxWaitingComp, OMX_CommandStateSet, OMX_StateIdle, NULL);
	        if (err != OMX_ErrorNone) {
	        	DEBUG(DEB_LEV_ERR, "In %s, the state cannot be changed\n", __func__);
	        }
		}
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.video.scheduler")) {
		if (!videoschedComponentList) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
			return OMX_ErrorUndefined;
		}
		err = removeElemFromList(&videoschedComponentList, openmaxStandComp);
		if (err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource cannot be released\n", __func__);
			return OMX_ErrorUndefined;
		}
		if(numElemInList(videoschedWaitingList)) {
			openmaxWaitingComp = videoschedWaitingList->openmaxStandComp;
			removeElemFromList(&videoschedWaitingList, openmaxWaitingComp);
	        err = OMX_SendCommand(openmaxWaitingComp, OMX_CommandStateSet, OMX_StateIdle, NULL);
	        if (err != OMX_ErrorNone) {
	        	DEBUG(DEB_LEV_ERR, "In %s, the state cannot be changed\n", __func__);
	        }
		}
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.clocksrc")) {
		if (!clockComponentList) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource manager is not initialized\n", __func__);
			return OMX_ErrorUndefined;
		}
		err = removeElemFromList(&clockComponentList, openmaxStandComp);
		if (err != OMX_ErrorNone) {
			DEBUG(DEB_LEV_ERR, "In %s, the resource cannot be released\n", __func__);
			return OMX_ErrorUndefined;
		}
		if(numElemInList(clockWaitingList)) {
			openmaxWaitingComp = clockWaitingList->openmaxStandComp;
			removeElemFromList(&clockWaitingList, openmaxWaitingComp);
	        err = OMX_SendCommand(openmaxWaitingComp, OMX_CommandStateSet, OMX_StateIdle, NULL);
	        if (err != OMX_ErrorNone) {
	        	DEBUG(DEB_LEV_ERR, "In %s, the state cannot be changed\n", __func__);
	        }
		}
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of  %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function adds the given component to the waiting queue for
 * the given resource. When a resource becomes available through the
 * RM_releaseResource function the first element in the queue is taken
 * off the list and it receives the resource just released.
 */
OMX_ERRORTYPE RM_waitForResource(OMX_COMPONENTTYPE *openmaxStandComp) {
	omx_base_component_PrivateType* omx_base_component_Private;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
	if (!strcmp(omx_base_component_Private->name, "OMX.st.volume.component")) {
		addElemToList(&volumeWaitingList, openmaxStandComp);
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.audio.mixer")) {
		addElemToList(&mixerWaitingList, openmaxStandComp);
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.video.scheduler")) {
		addElemToList(&videoschedWaitingList, openmaxStandComp);
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.clocksrc")) {
		addElemToList(&clockWaitingList, openmaxStandComp);
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}

/**
 * This function removes a component from the waiting queue
 * if the IL client decides that the component should not wait any
 * more for the resource
 */
OMX_ERRORTYPE RM_removeFromWaitForResource(OMX_COMPONENTTYPE *openmaxStandComp) {
	omx_base_component_PrivateType* omx_base_component_Private;

	DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
	omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
	if (!strcmp(omx_base_component_Private->name, "OMX.st.volume.component")) {
		removeElemFromList(&volumeWaitingList, openmaxStandComp);
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.audio.mixer")) {
		removeElemFromList(&mixerWaitingList, openmaxStandComp);
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.video.scheduler")) {
		removeElemFromList(&videoschedWaitingList, openmaxStandComp);
	} else if (!strcmp(omx_base_component_Private->name, "OMX.st.clocksrc")) {
		removeElemFromList(&clockWaitingList, openmaxStandComp);
	}
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorNone;
}
