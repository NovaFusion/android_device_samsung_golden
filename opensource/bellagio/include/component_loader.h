/**
  src/component_loader.h

  OpenMAX Component loader APIs. This header file specifies the entry point
  that a component loader must provide.

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

#ifndef __COMPONENT_LOADER_H__
#define __COMPONENT_LOADER_H__

#include <OMX_Core.h>

/** @brief Component loader entry points
 *
 * The component loader generic structure contains the entry points for
 * each component loader. The list of component loaders is filled using
 * a special function, called AddComponentLoader.
 */
typedef struct BOSA_COMPONENTLOADER
{
  /** @brief The constructor of the component loader
   *
   * The component loader creator is called by the OMX_Init function. It is
   * implemented by the specific component loader. It must allocate any
   * resource needed by the component loader.
   *
   * @param loader A private data structure, if needed by the component loader.
   * This data structure is passed every time a function of this loader is called.
   * @return OMX_ErrorInsufficientResources if the component loader can not be constructed
   */
  OMX_ERRORTYPE (*BOSA_InitComponentLoader)(struct BOSA_COMPONENTLOADER *loader);

  /** @brief The destructor of the component loader
   *
   * The component loader destructor is called by the OMX_Deinit function. It is
   * implemented by the specific component loader. It must free every specific
   * resource used by the component loader.
   *
   * @param loader the specific component loader. This parameter is also specific
   * to the component loader, and its structure is defined by each loader.
   * @return OMX_ErrorNone
   */
  OMX_ERRORTYPE (*BOSA_DeInitComponentLoader)(struct BOSA_COMPONENTLOADER *loader);

  /** @brief The component constructor of the current component loader
   *
   * This function implements the OMX_GetHandle function for the
   * specific component loader. Its interface is the same as the
   * standard GetHandle function, except that the first parameter
   * that contains the private data of the specific component loader.
   *
   * @param loader Private data of the component loader
   * @param pHandle the openmax handle returned by the function, or NULL
   * in case of failure.
   * @param cComponentName A string that contains the standard
   * component's name
   * @param pAppData private data of the component (if needed)
   * @param pCallBacks IL client callback function pointers passed
   * to the component
   *
   * @return OMX_ErrorNone if the component is correctly constructed and returned
   * @return OMX_ErrorComponentNotFound if the component is not found
   * @return OMX_ErrorInsufficientResources if the component exists but can not be allocated.
   */
  OMX_ERRORTYPE (*BOSA_CreateComponent)(
    struct BOSA_COMPONENTLOADER *loader,
    OMX_HANDLETYPE* pHandle,
    OMX_STRING cComponentName,
    OMX_PTR pAppData,
    OMX_CALLBACKTYPE* pCallBacks);

  /** @brief The component destructor of the current component loader
   *
   * This function implements the OMX_FreeHandle function for the
   * specific component loader. Its interface is the same as the
   * standard FreeHandle function, except that the first parameter
   * that contains the private data of the specific component loader.
   *
   * @param loader Private data of the component loader
   * @param pHandle the openmax handle returned by the function, or NULL
   * in case of failure.
   * @param cComponentName A string that contains the standard
   * component's name
   * @param pAppData private data of the component (if needed)
   * @param pCallBacks IL client callback function pointers passed
   * to the component
   *
   * @return OMX_ErrorNone if the component is correctly constructed and returned
   * @return OMX_ErrorComponentNotFound if the component is not found
   * @return OMX_ErrorInsufficientResources if the component exists but can not be allocated.
   */
  OMX_ERRORTYPE (*BOSA_DestroyComponent)(
      struct BOSA_COMPONENTLOADER *loader,
      OMX_HANDLETYPE hComponent);

  /** @brief An enumerator of the components handled by the current component loader.
   *
   * This function implements the OMX_ComponentNameEnum function
   * for the specific component loader
   *
   * @param loader Private data of the component loader
   * @param cComponentName A pointer to a null terminated string
   * with the component name.  The names of the components are
   * strings less than 127 bytes in length plus the trailing null
   * for a maximum size of 128 bytes (OMX_MAX_STRINGNAME_SIZE).
   * @param nNameLength The number of characters in the
   * cComponentName string.  With all component name strings
   * restricted to less than 128 characters (including the trailing null)
   * it is recommended that the caller provide a input string for the
   * cComponentName of 128 characters.
   * @param nIndex A number containing the enumeration index
   * for the component. Multiple calls to OMX_ComponentNameEnum
   * with increasing values of nIndex will enumerate through the
   * component names in the system until OMX_ErrorNoMore is returned.
   * The value of nIndex is 0 to (N-1), where N is the number of
   * valid installed components in the system.
   *
   * @return OMX_ErrorNone If the command successfully executes
   * @return OMX_ErrorNoMore If the value of nIndex exceeds the
   * number of components handled by the component loader minus 1
   */
  OMX_ERRORTYPE (*BOSA_ComponentNameEnum)(
    struct BOSA_COMPONENTLOADER *loader,
    OMX_STRING cComponentName,
    OMX_U32 nNameLength,
    OMX_U32 nIndex);

  /** @brief This function implements the OMX_GetRolesOfComponent standard function for the current component loader
   *
   * This function will return the number of roles supported by
   * the given component and (if the roles field is non-NULL)
   * the names of those roles. The call will fail if an insufficiently
   * sized array of names is supplied.
   * To ensure the array is sufficiently sized the client should:
   * - first call this function with the roles field NULL to
   *   determine the number of role names
   * - second call this function with the roles field pointing to
   *   an array of names allocated according to the number
   *   returned by the first call.
   *
   * @param loader Private data of the component loader
   * @param compName The name of the component being queried about.
   * @param pNumRoles This parameter is used both as input and output.
   * If roles is NULL, the input is ignored and the output specifies
   * how many roles the component supports. If compNames is not NULL,
   * on input it bounds the size of the input structure and on output,
   * it specifies the number of roles string names listed within
   * the roles parameter.
   * @param roles If NULL this field is ignored. If non-NULL this points
   * to an array of 128-byte strings which accepts a list of the names of
   * all standard components roles implemented on the specified
   * component name. numComps indicates the number of names.
   */
  OMX_ERRORTYPE (*BOSA_GetRolesOfComponent)(
    struct BOSA_COMPONENTLOADER *loader,
    OMX_STRING compName,
    OMX_U32 *pNumRoles,
    OMX_U8 **roles);

  /** @brief This function implements the OMX_GetComponentsOfRole
   * standard function for the current component loader
   *
   * This function will return the number of components that support
   * the given role and (if the compNames field is non-NULL) the names
   * of those components. The call will fail if an insufficiently
   * sized array of names is supplied. To ensure the array is
   * sufficiently sized the client should:
   * - first call this function with the compNames field NULL to
   *   determine the number of component names
   * - second call this function with the compNames field pointing
   *   to an array of names allocated according to the number
   *   returned by the first call.
   *
   * @param loader Private data of the component loader
   * @param role This is generic standard component name consisting
   * only of component class name and the type within that class
   * (e.g. 'audio_decoder.aac').
   * @param pNumComps This is used both as input and output. If compNames
   * is NULL, the input is ignored and the output specifies how many
   * components support the given role. If compNames is not NULL,
   * on input it bounds the size of the input structure and on output,
   * it specifies the number of components string names listed within
   * the compNames parameter.
   * @param compNames If NULL this field is ignored. If non-NULL this points
   * to an array of 128-byte strings which accepts a list of the names of
   * all physical components that implement the specified standard component
   * name. Each name is NULL terminated. numComps indicates the number of names.
   */
  OMX_ERRORTYPE (*BOSA_GetComponentsOfRole) (
    struct BOSA_COMPONENTLOADER *loader,
    OMX_STRING role,
    OMX_U32 *pNumComps,
    OMX_U8  **compNames);

  /** @brief The reference to the current component loader private data
   *
   * The current loader specified by this structure is described with this
   * generic pointer that contains the private data of the loader.
   */
  void *loaderPrivate;

} BOSA_COMPONENTLOADER;

#endif
