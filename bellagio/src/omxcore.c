/**
  src/omxcore.c

  OpenMAX Integration Layer Core. This library implements the OpenMAX core
  responsible for environment setup, components tunneling and communication.

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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <errno.h>
#include <assert.h>

#include <OMX_Core.h>
#include <OMX_ContentPipe.h>

#include "omxcore.h"
#include "omx_create_loaders.h"
#include "tsemaphore.h"

extern CPresult file_pipe_Constructor(CP_PIPETYPE* pPipe, CPstring szURI);
extern CPresult inet_pipe_Constructor(CP_PIPETYPE* pPipe, CPstring szURI);

/** The static field initialized is equal to 0 if the core is not initialized.
 * It is equal to 1 when the OMX_Init has been called
 */
static int initialized;

/** The semaphore to protect the initialized variable against race condition
 */
static tsem_t initSem;

static void start(void) __attribute__ ((constructor));
static void stop(void) __attribute__ ((destructor));

void start(void) 
{
  tsem_init(&initSem,1);
  return;
}

void stop(void)
{
  tsem_deinit(&initSem);
  return;
}

/** The int bosa_loaders contains the number of loaders available in the system.
 */
static int bosa_loaders;

/** The pointer to the loaders list. This list contains the all the different component loaders
 * present in the system or added by the IL Client with the BOSA_AddComponentLoader function.
 * The component loader is a implementation specific way to handle a set of components. The implementation
 * of the IL core accesses to the loaders in a standard way, but the different loaders can handle
 * different types of components, or handle in different ways the same components. It can be used also
 * to create a multi-OS support
 */
static volatile BOSA_COMPONENTLOADER **loadersList = NULL;

OMX_ERRORTYPE BOSA_AddComponentLoader(BOSA_COMPONENTLOADER *pLoader)
{
  BOSA_COMPONENTLOADER **newLoadersList = NULL;
  assert(pLoader);

  newLoadersList = realloc(loadersList, bosa_loaders * sizeof(BOSA_COMPONENTLOADER *));

  if (!newLoadersList)
    return OMX_ErrorInsufficientResources;

  bosa_loaders++;
  loadersList = (volatile BOSA_COMPONENTLOADER **) newLoadersList;

  loadersList[bosa_loaders - 1] = pLoader;

  DEBUG(DEB_LEV_SIMPLE_SEQ, "Loader added at index %d\n", bosa_loaders - 1);

  return OMX_ErrorNone;
}

/** @brief The OMX_Init standard function
 *
 * This function calls the init function of each component loader added. If there
 * is no component loaders present, the ST default component loader (static libraries)
 * is loaded as default component loader.
 *
 * @return OMX_ErrorNone
 */
OMX_ERRORTYPE OMX_Init() {
  int i = 0;
  OMX_ERRORTYPE err = OMX_ErrorNone, errLoader = OMX_ErrorInsufficientResources;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);

  /* Get a lock in case multiple thread are calling OMX_Init at the same time */
  tsem_down(&initSem);
  
  if(initialized == 0) {
    
    if (createComponentLoaders()) {
      tsem_up(&initSem);
      return OMX_ErrorInsufficientResources;
    }
    
    for (i = 0; i < bosa_loaders; i++) {
      err = loadersList[i]->BOSA_InitComponentLoader((struct BOSA_COMPONENTLOADER *) loadersList[i]);
      if (err != OMX_ErrorNone) {
	
	DEBUG(DEB_LEV_ERR, "Component loader %d constructor fails. Error= 0x%08x \n",i, err);
	
	/* The loader failed to initialize itself -> discarding it for the session */
	free((void *) loadersList[i]);
	loadersList[i] = 0;
      }
      
      // Returning OMX_ErrorNone if at least one loader has been able to initialize itself
      if (err == OMX_ErrorNone) {
	errLoader = OMX_ErrorNone;
      }
    }
  } else {
    /* In case core already initialized things are fine */
    errLoader = OMX_ErrorNone;
  }
  initialized ++;
  tsem_up(&initSem);
  
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
  return errLoader;
}

/** @brief The OMX_Deinit standard function
 *
 * In this function the Deinit function for each component loader is performed
 */
OMX_ERRORTYPE OMX_Deinit() {
  int i = 0;
  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
  tsem_down(&initSem);
  initialized --;
  if(initialized == 0) {
    for (i = 0; i < bosa_loaders; i++) {
      if(loadersList[i]) {
	loadersList[i]->BOSA_DeInitComponentLoader((struct BOSA_COMPONENTLOADER *) loadersList[i]);
	free((void *) loadersList[i]);
	loadersList[i] = 0;
      }
    }
    free(loadersList);
    loadersList = 0;
    bosa_loaders = 0;
  }
  tsem_up(&initSem);
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
  return OMX_ErrorNone;
}

/** @brief the OMX_GetHandle standard function
 *
 * This function will scan inside any component loader to search for
 * the requested component. If there are more components with the same name
 * the first component is returned. The existence of multiple components with
 * the same name is not contemplated in OpenMAX specification. The assumption is
 * that this behavior is NOT allowed.
 *
 * @return OMX_ErrorNone if a component has been found
 *         OMX_ErrorComponentNotFound if the requested component has not been found
 *                                    in any loader
 */
OMX_ERRORTYPE OMX_GetHandle(OMX_HANDLETYPE* pHandle,
  OMX_STRING cComponentName,
  OMX_PTR pAppData,
  OMX_CALLBACKTYPE* pCallBacks) {

  OMX_ERRORTYPE err = OMX_ErrorNone;
  int i;
  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s for %s\n", __func__, cComponentName);

  for (i = 0; i < bosa_loaders; i++) {
    if(loadersList[i]) {
      err = loadersList[i]->BOSA_CreateComponent(
						 (struct BOSA_COMPONENTLOADER *) loadersList[i],
						 pHandle,
						 cComponentName,
						 pAppData,
						 pCallBacks);
      if (err == OMX_ErrorNone) {
	// the component has been found
	return OMX_ErrorNone;
      }
    }
  }
  /*Required to meet conformance test: do not remove*/
  if (err == OMX_ErrorInsufficientResources) {
    return OMX_ErrorInsufficientResources;
  }
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
  return OMX_ErrorComponentNotFound;
}

/** @brief The OMX_FreeHandle standard function
 *
 * This function executes the BOSA_DestroyComponent of the component loaders
 *
 * @param hComponent the component handle to be freed
 *
 * @return The error of the BOSA_DestroyComponent function or OMX_ErrorNone
 */
OMX_ERRORTYPE OMX_FreeHandle(OMX_HANDLETYPE hComponent) {
	int i;
    OMX_ERRORTYPE err;
    DEBUG(DEB_LEV_FUNCTION_NAME, "In %s for %x\n", __func__, (int)hComponent);

    for (i = 0; i < bosa_loaders; i++) {
      if(loadersList[i]) {
    	err = loadersList[i]->BOSA_DestroyComponent((struct BOSA_COMPONENTLOADER *) loadersList[i],
						    hComponent);
	
    	if (err == OMX_ErrorNone) {
	  // the component has been found and destroyed
	  return OMX_ErrorNone;
    	}
      }
    }
    DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
    return OMX_ErrorComponentNotFound;
}

/** @brief the OMX_ComponentNameEnum standard function
 *
 * This function build a complete list of names from all the loaders.
 * For each loader the index is from 0 to max, but this function must provide a single
 * list, with a common index. This implementation orders the loaders and the
 * related list of components.
 */
OMX_ERRORTYPE OMX_ComponentNameEnum(
		OMX_STRING cComponentName,
		OMX_U32 nNameLength,
		OMX_U32 nIndex)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  int i = 0;
    int index = 0;
    int offset = 0;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);

  for (i = 0; i < bosa_loaders; i++) {
    offset = 0;
    if(loadersList[i]) {
      while((err = loadersList[i]->BOSA_ComponentNameEnum((struct BOSA_COMPONENTLOADER *) loadersList[i],
							  cComponentName,
							  nNameLength,
							  offset)) != OMX_ErrorNoMore) {
	if (index == nIndex) {
	  return err;
	}
	offset++;
	index++;
      }
    }
  }
  
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
  return OMX_ErrorNoMore;
}

/** @brief the OMX_SetupTunnel standard function
 *
 * The implementation of this function is described in the OpenMAX spec
 *
 * @param hOutput component handler that controls the output port of the tunnel
 * @param nPortOutput index of the output port of the tunnel
 * @param hInput component handler that controls the input port of the tunnel
 * @param nPortInput index of the input port of the tunnel
 *
 * @return OMX_ErrorBadParameter, OMX_ErrorPortsNotCompatible, tunnel rejected by a component
 * or OMX_ErrorNone if the tunnel has been established
 */
OMX_ERRORTYPE OMX_SetupTunnel(
  OMX_HANDLETYPE hOutput,
  OMX_U32 nPortOutput,
  OMX_HANDLETYPE hInput,
  OMX_U32 nPortInput) {

  OMX_ERRORTYPE err;
  OMX_COMPONENTTYPE* component;
  OMX_TUNNELSETUPTYPE* tunnelSetup;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s the output port is:%x/%i, the input port is %x/%i\n",
		  __func__, (int)hOutput, (int)nPortOutput, (int)hInput, (int)nPortInput);
  tunnelSetup = malloc(sizeof(OMX_TUNNELSETUPTYPE));
  component = (OMX_COMPONENTTYPE*)hOutput;
  tunnelSetup->nTunnelFlags = 0;
  tunnelSetup->eSupplier = OMX_BufferSupplyUnspecified;

  if (hOutput == NULL && hInput == NULL)
        return OMX_ErrorBadParameter;
  if (hOutput){
    err = (component->ComponentTunnelRequest)(hOutput, nPortOutput, hInput, nPortInput, tunnelSetup);
    if (err != OMX_ErrorNone) {
    DEBUG(DEB_LEV_ERR, "Tunneling failed: output port rejects it - err = %x\n", err);
    free(tunnelSetup);
    tunnelSetup = NULL;
    return err;
    }
  }
  DEBUG(DEB_LEV_PARAMS, "First stage of tunneling acheived:\n");
  DEBUG(DEB_LEV_PARAMS, "       - supplier proposed = %i\n", tunnelSetup->eSupplier);
  DEBUG(DEB_LEV_PARAMS, "       - flags             = %i\n", (int)tunnelSetup->nTunnelFlags);

  component = (OMX_COMPONENTTYPE*)hInput;
  if (hInput) {
    err = (component->ComponentTunnelRequest)(hInput, nPortInput, hOutput, nPortOutput, tunnelSetup);
    if (err != OMX_ErrorNone) {
      DEBUG(DEB_LEV_ERR, "Tunneling failed: input port rejects it - err = %08x\n", err);
      // the second stage fails. the tunnel on poutput port has to be removed
      component = (OMX_COMPONENTTYPE*)hOutput;
      err = (component->ComponentTunnelRequest)(hOutput, nPortOutput, NULL, 0, tunnelSetup);
      if (err != OMX_ErrorNone) {
        // This error should never happen. It is critical, and not recoverable
        free(tunnelSetup);
        tunnelSetup = NULL;
        DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s with OMX_ErrorUndefined\n", __func__);
        return OMX_ErrorUndefined;
      }
      free(tunnelSetup);
      tunnelSetup = NULL;
      DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s with OMX_ErrorPortsNotCompatible\n", __func__);
      return OMX_ErrorPortsNotCompatible;
    }
  }
  DEBUG(DEB_LEV_PARAMS, "Second stage of tunneling acheived:\n");
  DEBUG(DEB_LEV_PARAMS, "       - supplier proposed = %i\n", (int)tunnelSetup->eSupplier);
  DEBUG(DEB_LEV_PARAMS, "       - flags             = %i\n", (int)tunnelSetup->nTunnelFlags);
  free(tunnelSetup);
  tunnelSetup = NULL;
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
  return OMX_ErrorNone;
}

/** @brief the OMX_GetRolesOfComponent standard function
 */
OMX_ERRORTYPE OMX_GetRolesOfComponent (
  OMX_STRING CompName,
  OMX_U32 *pNumRoles,
  OMX_U8 **roles) {
  OMX_ERRORTYPE err = OMX_ErrorNone;
  int i;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
  for (i = 0; i < bosa_loaders; i++) {
    if(loadersList[i]) {
      err = loadersList[i]->BOSA_GetRolesOfComponent((struct BOSA_COMPONENTLOADER *) loadersList[i],
						     CompName,
						     pNumRoles,
						     roles);
      if (err == OMX_ErrorNone) {
	return OMX_ErrorNone;
      }
    }
  }
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
  return OMX_ErrorComponentNotFound;
}

/** @brief the OMX_GetComponentsOfRole standard function
 *
 * This function searches in all the component loaders any component
 * supporting the requested role
 *
 * @param role See spec
 * @param pNumComps See spec
 * @param compNames See spec
 *
 */
OMX_ERRORTYPE OMX_GetComponentsOfRole (
  OMX_STRING role,
  OMX_U32 *pNumComps,
  OMX_U8  **compNames) {
  OMX_ERRORTYPE err = OMX_ErrorNone;
  int i,j;
  int only_number_requested = 0, full_number=0;
  OMX_U32 temp_num_comp = 0;

  OMX_U8 **tempCompNames;
  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
  if (compNames == NULL) {
    only_number_requested = 1;
  } else {
    only_number_requested = 0;
  }
  for (i = 0; i < bosa_loaders; i++) {
    temp_num_comp = *pNumComps;
    if(loadersList[i]) {
      err = loadersList[i]->BOSA_GetComponentsOfRole(
          (struct BOSA_COMPONENTLOADER *) loadersList[i],
          role,
          &temp_num_comp,
          NULL);
      if (err != OMX_ErrorNone) {
	DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	return OMX_ErrorComponentNotFound;
      }
      if (only_number_requested == 0) {
	tempCompNames = malloc(temp_num_comp * sizeof(OMX_STRING));
	for (j=0; j<temp_num_comp; j++) {
	  tempCompNames[j] = malloc(OMX_MAX_STRINGNAME_SIZE * sizeof(char));
	}
	err = loadersList[i]->BOSA_GetComponentsOfRole(
						       (struct BOSA_COMPONENTLOADER *) loadersList[i],
						       role,
						       &temp_num_comp,
						       tempCompNames);
	if (err != OMX_ErrorNone) {
	  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	  return OMX_ErrorComponentNotFound;
	}

	for (j = 0; j<temp_num_comp; j++) {
	  if (full_number + j < *pNumComps) {
	    strncpy((char *)compNames[full_number + j], (const char *)tempCompNames[j], 128);
	  }
	}
      }
      full_number += temp_num_comp;
    }
  }
  *pNumComps = full_number;
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
  return OMX_ErrorNone;
}

OMX_ERRORTYPE OMX_GetContentPipe(
    OMX_HANDLETYPE *hPipe,
    OMX_STRING szURI) {
	  OMX_ERRORTYPE err = OMX_ErrorContentPipeCreationFailed;
	  CPresult res;
	  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);

	  if(strncmp(szURI, "file", 4) == 0) {
	    res = file_pipe_Constructor((CP_PIPETYPE*) hPipe, szURI);
	    if(res == 0x00000000)
	      err = OMX_ErrorNone;
	  }

	  else if(strncmp(szURI, "inet", 4) == 0) {
	    res = inet_pipe_Constructor((CP_PIPETYPE*) hPipe, szURI);
	    if(res == 0x00000000)
	      err = OMX_ErrorNone;
	  }
	  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s\n", __func__);
	  return err;
}

