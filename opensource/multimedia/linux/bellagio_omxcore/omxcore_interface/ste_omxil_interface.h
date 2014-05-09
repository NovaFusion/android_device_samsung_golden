/*****************************************************************************/
/**
*  © ST-Ericsson, 2009 - All rights reserved
*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \file    ste_omxil_interface.h
* \brief   Declare OMXIL Interface
* \author  ST-Ericsson
*/
/*****************************************************************************/

#ifndef __STE_OMX_INTERFACE_H__
#define __STE_OMX_INTERFACE_H__



// get definitions of omx core methods
#ifndef OMX_Core_h
#include "OMX_Core.h"
#endif

#include <stdio.h> 
// ptrs to all omx core methods
// will be set in the derived classes
typedef OMX_ERRORTYPE OMX_APIENTRY(*tpOMX_Init)(void);

typedef OMX_ERRORTYPE OMX_APIENTRY(*tpOMX_Deinit)(void);

typedef OMX_ERRORTYPE OMX_APIENTRY(*tpOMX_ComponentNameEnum)(
    OMX_OUT OMX_STRING cComponentName,
    OMX_IN  OMX_U32 nNameLength,
    OMX_IN  OMX_U32 nIndex);


typedef OMX_ERRORTYPE OMX_APIENTRY(*tpOMX_GetHandle)(
    OMX_OUT OMX_HANDLETYPE* pHandle,
    OMX_IN  OMX_STRING cComponentName,
    OMX_IN  OMX_PTR pAppData,
    OMX_IN  OMX_CALLBACKTYPE* pCallBacks);


typedef OMX_ERRORTYPE OMX_APIENTRY(*tpOMX_FreeHandle)(
    OMX_IN  OMX_HANDLETYPE hComponent);

typedef OMX_ERRORTYPE(*tpOMX_GetComponentsOfRole)(
    OMX_IN      OMX_STRING role,
    OMX_INOUT   OMX_U32 *pNumComps,
    OMX_INOUT   OMX_U8  **compNames);


typedef OMX_ERRORTYPE(*tpOMX_GetRolesOfComponent)(
    OMX_IN      OMX_STRING compName,
    OMX_INOUT   OMX_U32 *pNumRoles,
    OMX_OUT     OMX_U8 **roles);


typedef OMX_ERRORTYPE OMX_APIENTRY(*tpOMX_SetupTunnel)(
    OMX_IN  OMX_HANDLETYPE hOutput,
    OMX_IN  OMX_U32 nPortOutput,
    OMX_IN  OMX_HANDLETYPE hInput,
    OMX_IN  OMX_U32 nPortInput);


typedef OMX_ERRORTYPE(*tpOMX_GetContentPipe)(
    OMX_OUT OMX_HANDLETYPE *hPipe,
    OMX_IN OMX_STRING szURI);


class OMXCoreInterface 
{
    public:
        OMXCoreInterface()
        {
            pOMX_Init = NULL;
            pOMX_Deinit = NULL;
            pOMX_ComponentNameEnum = NULL;
            pOMX_GetHandle = NULL;
            pOMX_FreeHandle = NULL;
            pOMX_GetComponentsOfRole = NULL;
            pOMX_GetRolesOfComponent = NULL;
            pOMX_SetupTunnel = NULL;
            pOMX_GetContentPipe = NULL;
        };

	void release();

        tpOMX_Init GetpOMX_Init()
        {
            return pOMX_Init;
        };

        tpOMX_Deinit GetpOMX_Deinit()
        {
            return  pOMX_Deinit;
        };

        tpOMX_ComponentNameEnum GetpOMX_ComponentNameEnum()
        {
            return pOMX_ComponentNameEnum;
        };

        tpOMX_GetHandle GetpOMX_GetHandle()
        {
            return pOMX_GetHandle;
        };

        tpOMX_FreeHandle GetpOMX_FreeHandle()
        {
            return pOMX_FreeHandle;
        };

        tpOMX_GetComponentsOfRole GetpOMX_GetComponentsOfRole()
        {
            return pOMX_GetComponentsOfRole;
        };

        tpOMX_GetRolesOfComponent GetpOMX_GetRolesOfComponent()
        {
            return pOMX_GetRolesOfComponent;
        };

        tpOMX_SetupTunnel GetpOMX_SetupTunnel()
        {
            return pOMX_SetupTunnel;
        };

        tpOMX_GetContentPipe GetpOMX_GetContentPipe()
        {
            return pOMX_GetContentPipe;
        };

        // ptrs to all omx core methods
        // will be set in the derived classes
        OMX_ERRORTYPE OMX_APIENTRY(*pOMX_Init)(void);

        OMX_ERRORTYPE OMX_APIENTRY(*pOMX_Deinit)(void);

        OMX_ERRORTYPE OMX_APIENTRY(*pOMX_ComponentNameEnum)(
            OMX_OUT OMX_STRING cComponentName,
            OMX_IN  OMX_U32 nNameLength,
            OMX_IN  OMX_U32 nIndex);


        OMX_ERRORTYPE OMX_APIENTRY(*pOMX_GetHandle)(
            OMX_OUT OMX_HANDLETYPE* pHandle,
            OMX_IN  OMX_STRING cComponentName,
            OMX_IN  OMX_PTR pAppData,
            OMX_IN  OMX_CALLBACKTYPE* pCallBacks);


        OMX_ERRORTYPE OMX_APIENTRY(*pOMX_FreeHandle)(
            OMX_IN  OMX_HANDLETYPE hComponent);

        OMX_ERRORTYPE(*pOMX_GetComponentsOfRole)(
            OMX_IN      OMX_STRING role,
            OMX_INOUT   OMX_U32 *pNumComps,
            OMX_INOUT   OMX_U8  **compNames);


        OMX_ERRORTYPE(*pOMX_GetRolesOfComponent)(
            OMX_IN      OMX_STRING compName,
            OMX_INOUT   OMX_U32 *pNumRoles,
            OMX_OUT     OMX_U8 **roles);


        OMX_ERRORTYPE OMX_APIENTRY(*pOMX_SetupTunnel)(
            OMX_IN  OMX_HANDLETYPE hOutput,
            OMX_IN  OMX_U32 nPortOutput,
            OMX_IN  OMX_HANDLETYPE hInput,
            OMX_IN  OMX_U32 nPortInput);


        OMX_ERRORTYPE(*pOMX_GetContentPipe)(
            OMX_OUT OMX_HANDLETYPE *hPipe,
            OMX_IN OMX_STRING szURI);
};

extern "C"
{
    OMXCoreInterface* GetOMXCoreInterface();
}
#endif

