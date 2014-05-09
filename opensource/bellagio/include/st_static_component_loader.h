/**
  src/st_static_component_loader.h

  ST specific component loader for local components.

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

#ifndef __ST_STATIC_COMPONENT_LOADER_H__
#define __ST_STATIC_COMPONENT_LOADER_H__

#include "omxcore.h"
#include "extension_struct.h"

/** @brief the private data structure handled by the ST static loader that described
 * an OpenMAX component
 *
 * This structure contains all the fields that the loader must use to support
 * the loading  unloading functions of the component, that are not described by the
 * standard.
 */
typedef struct stLoaderComponentType{
  OMX_VERSIONTYPE componentVersion; /**< the version of the component in the OpenMAX standard format */
  char* name; /**< String that represents the name of the component, ruled by the standard */
  unsigned int name_specific_length;/**< this field contains the number of roles of the component */
  char** name_specific; /**< Strings those represent the names of the specific format components */
  char** role_specific; /**< Strings those represent the names of the specific format components */
  char* name_requested; /**< This parameter is used to send to the component the string requested by the IL Client */
  OMX_ERRORTYPE (*constructor)(OMX_COMPONENTTYPE*,OMX_STRING cComponentName); /**< constructor function pointer for each Linux ST OpenMAX component */
  OMX_U32 nqualitylevels;/**< number of available quality levels */
  multiResourceDescriptor** multiResourceLevel;
} stLoaderComponentType;

/** @brief The initialization of the ST specific component loader.
 */
void st_static_setup_component_loader(BOSA_COMPONENTLOADER * st_static_loader);

/** @brief The constructor of the ST specific component loader.
 *
 * It is the component loader developed under linux by ST, for local libraries.
 * It is based on a registry file, like in the case of GStreamer. It reads the
 * registry file, and allows the components to register themself to the
 * main list templateList.
 */
OMX_ERRORTYPE BOSA_ST_InitComponentLoader(BOSA_COMPONENTLOADER *loader);

/** @brief The destructor of the ST specific component loader.
 *
 * This function deallocates the list of available components.
 */
OMX_ERRORTYPE BOSA_ST_DeInitComponentLoader(BOSA_COMPONENTLOADER *loader);

/** @brief creator of the requested openmax component
 *
 * This function searches for the requested component in the internal list.
 * If the component is found, its constructor is called,
 * and the standard callback are assigned.
 * A pointer to a standard openmax component is returned.
 */
OMX_ERRORTYPE BOSA_ST_CreateComponent(
    BOSA_COMPONENTLOADER *loader,
    OMX_HANDLETYPE* pHandle,
    OMX_STRING cComponentName,
    OMX_PTR pAppData,
    OMX_CALLBACKTYPE* pCallBacks);

/** @brief destructor of the requested OpenMAX component
 *
 */
OMX_ERRORTYPE BOSA_ST_DestroyComponent(
    BOSA_COMPONENTLOADER *loader,
    OMX_HANDLETYPE hComponent);

/** @brief This function search for the index from 0 to end of the list
 *
 * This function searches in the list of ST static components and enumerates
 * both the class names and the role specific components.
 */
OMX_ERRORTYPE BOSA_ST_ComponentNameEnum(
    BOSA_COMPONENTLOADER *loader,
    OMX_STRING cComponentName,
    OMX_U32 nNameLength,
    OMX_U32 nIndex);

/** @brief The specific version of OMX_GetRolesOfComponent
 *
 * This function replicates exactly the behavior of the
 * standard OMX_GetRolesOfComponent function for the ST static
 * component loader
 */
OMX_ERRORTYPE BOSA_ST_GetRolesOfComponent(
    BOSA_COMPONENTLOADER *loader,
    OMX_STRING compName,
    OMX_U32 *pNumRoles,
    OMX_U8 **roles);

/** @brief The specific version of OMX_GetComponentsOfRole
 *
 * This function replicates exactly the behavior of the
 * standard OMX_GetComponentsOfRole function for the ST static
 * component loader
 */
OMX_ERRORTYPE BOSA_ST_GetComponentsOfRole (
    BOSA_COMPONENTLOADER *loader,
    OMX_STRING role,
    OMX_U32 *pNumComps,
    OMX_U8  **compNames);

#endif
