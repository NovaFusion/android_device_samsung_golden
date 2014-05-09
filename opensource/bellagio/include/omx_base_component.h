/**
  src/base/omx_base_component.h

  OpenMAX base component. This component does not perform any multimedia
  processing.It is used as a base component for new components development.

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
#ifndef _OMX_BASE_COMPONENT_H_
#define _OMX_BASE_COMPONENT_H_

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <asm/unistd.h>

#include "omxcore.h"
#include "OMXComponentRMExt.h"
#include "tsemaphore.h"
#include "queue.h"
#include "omx_classmagic.h"
#include "omx_base_port.h"
#include "extension_struct.h"

/** Default size of the internal input buffer */
#define DEFAULT_IN_BUFFER_SIZE  4 * 1024
/** Default size of the internal output buffer */
#define DEFAULT_OUT_BUFFER_SIZE 32 * 1024 /*16 * 1024 */ // TODO - check this size is ok
/** Default MIME string length */
#define DEFAULT_MIME_STRING_LENGTH 128

#define NUM_DOMAINS 4

#define OMX_BUFFERFLAG_KEY_FRAME 0x11000000

typedef struct OMX_VENDOR_EXTRADATATYPE  {
  OMX_U32 nPortIndex;
  OMX_U32 nDataSize;   // Size of the supporting data to follow
  OMX_U8  *pData;     // Supporting data hint
} OMX_VENDOR_EXTRADATATYPE;

typedef struct OMX_VENDOR_PROP_TUNNELSETUPTYPE  {
  OMX_U32 nPortIndex;
  OMX_TUNNELSETUPTYPE nTunnelSetup;   // Tunnel setup flags
} OMX_VENDOR_PROP_TUNNELSETUPTYPE;

/** this is the list of custom vendor index */
typedef enum OMX_INDEXVENDORTYPE {
	/** only one index for file reader component input file */
	OMX_IndexVendorInputFilename          = 0xFF000001,
	OMX_IndexVendorOutputFilename         = 0xFF000002,
	OMX_IndexVendorCompPropTunnelFlags    = 0xFF000003, /* Will use OMX_TUNNELSETUPTYPE structure*/
	OMX_IndexParameterThreadsID           = 0xFF000004
} OMX_INDEXVENDORTYPE;

/** This enum defines the transition states of the Component*/
typedef enum OMX_TRANS_STATETYPE {
    OMX_TransStateInvalid,
    OMX_TransStateLoadedToIdle,
    OMX_TransStateIdleToPause,
    OMX_TransStatePauseToExecuting,
    OMX_TransStateIdleToExecuting,
    OMX_TransStateExecutingToIdle,
    OMX_TransStateExecutingToPause,
    OMX_TransStatePauseToIdle,
    OMX_TransStateIdleToLoaded,
    OMX_TransStateMax = 0X7FFFFFFF
} OMX_TRANS_STATETYPE;

/** @brief Enumerates all the possible types of messages
 * handled internally by the component
 */
typedef enum INTERNAL_MESSAGE_TYPE {
  SENDCOMMAND_MSG_TYPE = 1,/**< this flag specifies that the message send is a command */
  ERROR_MSG_TYPE,/**< this flag specifies that the message send is an error message */
  WARNING_MSG_TYPE /**< this flag specifies that the message send is a warning message */
} INTERNAL_MESSAGE_TYPE;

/** @brief The container of an internal message
 *
 * This structure contains a generic OpenMAX request (from send command).
 * It is processed by the internal message handler thread
 */
typedef struct internalRequestMessageType {
  int messageType; /**< the flag that specifies if the message is a command, a warning or an error */
  int messageParam; /**< the second field of the message. Its use is the same as specified for the command in OpenMAX spec */
  OMX_PTR pCmdData; /**< This pointer could contain some proprietary data not covered by the standard */
} internalRequestMessageType;

/**
 * @brief the base descriptor for a ST component
 */
CLASS(omx_base_component_PrivateType)
#define omx_base_component_PrivateType_FIELDS \
	OMX_COMPONENTTYPE *openmaxStandComp; /**< The OpenMAX standard data structure describing a component */ \
	omx_base_PortType **ports; /** @param ports The ports of the component */ \
	OMX_PORT_PARAM_TYPE sPortTypesParam[NUM_DOMAINS]; /** @param sPortTypesParam OpenMAX standard parameter that contains a short description of the available ports */ \
	char uniqueID; /**< ID code that identifies an ST static component*/ \
	char* name; /**< component name */\
	OMX_STATETYPE state; /**< The state of the component */ \
	OMX_TRANS_STATETYPE transientState; /**< The transient state in case of transition between \
                              Loaded/waitForResources - Idle. It is equal to  \
                              Invalid if the state or transition are not corect \
                              Loaded when the transition is from Idle to Loaded \
                              Idle when the transition is from Loaded to Idle */ \
	OMX_CALLBACKTYPE* callbacks; /**< pointer to every client callback function, \
                                as specified by the standard*/ \
	OMX_PTR callbackData;/**< Private data that can be send with \
                        the client callbacks. Not specified by the standard */ \
	queue_t* messageQueue;/**< the queue of all the messages recevied by the component */\
	tsem_t* messageSem;/**< the semaphore that coordinates the access to the message queue */\
	OMX_U32 nGroupPriority; /**< @param nGroupPriority Resource management field: component priority (common to a group of components) */\
	OMX_U32 nGroupID; /**< @param nGroupID ID of a group of components that share the same logical chain */\
	OMX_PARAM_BELLAGIOTHREADS_ID* bellagioThreads;\
	OMX_BOOL bIsEOSReached; /** @param bIsEOSReached boolean flag is true when EOS has been reached */ \
	OMX_MARKTYPE pMark; /**< @param pMark This field holds the private data associated with a mark request, if any */\
	pthread_mutex_t flush_mutex;  /** @param flush_mutex mutex for the flush condition from buffers */ \
	tsem_t* flush_all_condition;  /** @param flush_all_condition condition for the flush all buffers */ \
	tsem_t* flush_condition;  /** @param The flush_condition condition */ \
	tsem_t* bMgmtSem;/**< @param bMgmtSem the semaphore that control BufferMgmtFunction processing */\
	tsem_t* bStateSem;/**< @param bMgmtSem the semaphore that control BufferMgmtFunction processing */\
	pthread_t messageHandlerThread; /** @param  messageHandlerThread This field contains the reference to the thread that receives messages for the components */ \
	int bufferMgmtThreadID; /** @param  bufferMgmtThreadID The ID of the pthread that process buffers */ \
	pthread_t bufferMgmtThread; /** @param  bufferMgmtThread This field contains the reference to the thread that process buffers */ \
	void *loader; /**< pointer to the loader that created this component, used for destruction */ \
	void* (*BufferMgmtFunction)(void* param); /** @param BufferMgmtFunction This function processes input output buffers */ \
	OMX_ERRORTYPE (*messageHandler)(OMX_COMPONENTTYPE*,internalRequestMessageType*);/** This function receives messages from the message queue. It is needed for each Linux ST OpenMAX component */ \
	OMX_ERRORTYPE (*DoStateSet)(OMX_COMPONENTTYPE *openmaxStandComp, OMX_U32); /**< @param DoStateSet internal function called when a generic state transition is requested*/ \
	OMX_ERRORTYPE (*destructor)(OMX_COMPONENTTYPE *openmaxStandComp); /** Component Destructor*/ \
	OMX_ERRORTYPE (*getQualityLevel)(OMX_COMPONENTTYPE *openmaxStandComp, OMX_U32* pQualityLevel); /** RM entry point. By default is not implemented */ \
	OMX_ERRORTYPE (*setQualityLevel)(OMX_COMPONENTTYPE *openmaxStandComp, OMX_U32 nQualityLevel); /** RM entry point. By default is not implemented */ \
	OMX_U32 nqualitylevels;/**< number of available quality levels */ \
	multiResourceDescriptor** multiResourceLevel; \
	int currentQualityLevel; /** if supported the current quality level set for this component */
ENDCLASS(omx_base_component_PrivateType)

void base_constructor_remove_garbage_collected(omx_base_component_PrivateType* omx_base_component_Private);

/**
 * @brief The base constructor for the OpenMAX ST components
 *
 * This function is executed by the ST static component loader.
 * It takes care of constructing the instance of the component.
 * For the base_component component, the following is done:
 *
 * 1) Fills the basic OpenMAX structure. The fields can be overwritten
 *    by derived components.
 * 2) Allocates (if needed) the omx_base_component_PrivateType private structure
 *
 * @param openmaxStandComp the ST component to be initialized
 * @param cComponentName the OpenMAX string that describes the component
 *
 * @return OMX_ErrorInsufficientResources if a memory allocation fails
 */
OMX_ERRORTYPE omx_base_component_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,OMX_STRING cComponentName);

/** @brief the base destructor for ST OpenMAX components
 *
 * This function is called by the standard function ComponentDeInit()
 * that is called by the IL core during the execution of the  FreeHandle()
 *
 * @param openmaxStandComp the ST OpenMAX component to be disposed
 */
OMX_ERRORTYPE omx_base_component_Destructor(OMX_COMPONENTTYPE *openmaxStandComp);

OMX_ERRORTYPE omx_base_getQualityLevel(OMX_COMPONENTTYPE *openmaxStandComp, OMX_U32* pQualityLevel);
OMX_ERRORTYPE omx_base_setQualityLevel(OMX_COMPONENTTYPE *openmaxStandComp, OMX_U32 nQualityLevel);

/** Changes the state of a component taking proper actions depending on
 * the transition requested. This base function cover only the state
 * changes that do not involve any port
 *
 * @param openmaxStandComp the OpenMAX component which state is to be changed
 * @param destinationState the requested target state
 *
 * @return OMX_ErrorNotImplemented if the state change is noty handled
 * in this base class, but needs a specific handling
 */
OMX_ERRORTYPE omx_base_component_DoStateSet(
  OMX_COMPONENTTYPE *openmaxStandComp,
  OMX_U32 destinationState);

/** @brief Checks the header of a structure for consistency
 * with size and spec version
 *
 * @param header Pointer to the structure to be checked
 * @param size Size of the structure. it is in general obtained
 * with a sizeof call applied to the structure
 *
 * @return OMX error code. If the header has failed the check,
 * OMX_ErrorVersionMismatch is returned.
 * If the header fails the size check OMX_ErrorBadParameter is returned
 */
OMX_ERRORTYPE checkHeader(OMX_PTR header, OMX_U32 size);

/** @brief Simply fills the first two fields in any OMX structure
 * with the size and the version
 *
 * @param header pointer to the structure to be filled
 * @param size size of the structure. It can be obtained with
 * a call to sizeof of the structure type
 */
void setHeader(OMX_PTR header, OMX_U32 size);

/** @brief standard openmax function
 *
 * it returns the version of the component. See OMX_Core.h
 */
OMX_ERRORTYPE omx_base_component_GetComponentVersion(
  OMX_HANDLETYPE hComponent,
  OMX_STRING pComponentName,
  OMX_VERSIONTYPE* pComponentVersion,
  OMX_VERSIONTYPE* pSpecVersion,
  OMX_UUIDTYPE* pComponentUUID);

/** @brief Enumerates all the role of the component.
 *
 * This function is intended to be used only by a core. The ST static core
 * in any case does not use this function, because it can not be used before the
 * creation of the component, but uses a static list.
 * It is implemented only for API completion, and it will be not overriden
 * by a derived component.
 *
 * @param hComponent handle of the component
 * @param cRole the output string containing the n-role of the component
 * @param nIndex the index of the role requested
 */
OMX_ERRORTYPE omx_base_component_ComponentRoleEnum(
  OMX_HANDLETYPE hComponent,
  OMX_U8 *cRole,
  OMX_U32 nIndex);

/** @brief standard OpenMAX function
 *
 * it sets the callback functions given by the IL client.
 * See OMX_Component.h
 */
OMX_ERRORTYPE omx_base_component_SetCallbacks(
  OMX_HANDLETYPE hComponent,
  OMX_CALLBACKTYPE* pCallbacks,
  OMX_PTR pAppData);

/** @brief Part of the standard OpenMAX function
 *
 * This function return the parameters not related to any port.
 * These parameters are handled in the derived components
 * See OMX_Core.h for standard reference.
 */
OMX_ERRORTYPE omx_base_component_GetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure);

/** @brief part of the standard openmax function
 *
 * This function return the parameters not related to any port,
 * These parameters are handled in the derived components
 * See OMX_Core.h for standard reference.
 *
 * @return OMX_ErrorUnsupportedIndex if the index is not supported or not handled here
 */
OMX_ERRORTYPE omx_base_component_SetParameter(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nParamIndex,
  OMX_PTR ComponentParameterStructure);

/** @brief base GetConfig function
 *
 * This base function is not implemented. If a derived component
 * needs to support any config, it must implement a derived
 * version of this function and assign it to the correct pointer
 * in the private component descriptor.
 */
OMX_ERRORTYPE omx_base_component_GetConfig(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nIndex,
  OMX_PTR pComponentConfigStructure);

/** @brief base SetConfig function
 *
 * This base function is not implemented. If a derived component
 * needs to support any config, it must implement a derived
 * version of this function and assign it to the correct pointer
 * in the private component descriptor.
 */
OMX_ERRORTYPE omx_base_component_SetConfig(
  OMX_HANDLETYPE hComponent,
  OMX_INDEXTYPE nIndex,
  OMX_PTR pComponentConfigStructure);

/** @brief base function not implemented
 *
 * This function can be eventually implemented by a
 * derived component if needed.
 */
OMX_ERRORTYPE omx_base_component_GetExtensionIndex(
  OMX_HANDLETYPE hComponent,
  OMX_STRING cParameterName,
  OMX_INDEXTYPE* pIndexType);

/** @returns the state of the component
 *
 * This function does not need any override by derived components.
 */
OMX_ERRORTYPE omx_base_component_GetState(
  OMX_HANDLETYPE hComponent,
  OMX_STATETYPE* pState);

/** @brief standard SendCommand function
 *
 * In general this function does not need a overwrite, but
 * a special derived component could do it.
 */
OMX_ERRORTYPE omx_base_component_SendCommand(
  OMX_HANDLETYPE hComponent,
  OMX_COMMANDTYPE Cmd,
  OMX_U32 nParam,
  OMX_PTR pCmdData);

/** @brief This standard functionality is called when the component is
 * destroyed in the FreeHandle standard call.
 *
 * In this way the implementation of the FreeHandle is standard,
 * and it does not need a support by a specific component loader.
 * The implementation of the ComponentDeInit contains the
 * implementation specific part of the destroying phase.
 */
OMX_ERRORTYPE omx_base_component_ComponentDeInit(
  OMX_HANDLETYPE hComponent);

/** @brief Component's message handler thread function
 *
 * Handles all messages coming from components and
 * processes them by dispatching them back to the
 * triggering component.
 */
void* compMessageHandlerFunction(void*);

/** This is called by the component message entry point.
 * In the base version this function is named compMessageHandlerFunction
 *
 * A request is made by the component when some asynchronous services are needed:
 * 1) A SendCommand() is to be processed
 * 2) An error needs to be notified
 * 3) ...
 *
 * @param openmaxStandComp the component itself
 * @param message the message that has been passed to core
 */
OMX_ERRORTYPE omx_base_component_MessageHandler(OMX_COMPONENTTYPE *openmaxStandComp,internalRequestMessageType* message);

/**
 * This function verify Component State and Structure header
 */
OMX_ERRORTYPE omx_base_component_ParameterSanityCheck(
  OMX_HANDLETYPE hComponent,
  OMX_U32 nPortIndex,
  OMX_PTR pStructure,
  size_t size);

OMX_ERRORTYPE omx_base_component_AllocateBuffer(
  OMX_HANDLETYPE hComponent,
  OMX_BUFFERHEADERTYPE** ppBuffer,
  OMX_U32 nPortIndex,
  OMX_PTR pAppPrivate,
  OMX_U32 nSizeBytes);

OMX_ERRORTYPE omx_base_component_UseBuffer(
  OMX_HANDLETYPE hComponent,
  OMX_BUFFERHEADERTYPE** ppBufferHdr,
  OMX_U32 nPortIndex,
  OMX_PTR pAppPrivate,
  OMX_U32 nSizeBytes,
  OMX_U8* pBuffer);

OMX_ERRORTYPE omx_base_component_UseEGLImage (
  OMX_HANDLETYPE hComponent,
  OMX_BUFFERHEADERTYPE** ppBufferHdr,
  OMX_U32 nPortIndex,
  OMX_PTR pAppPrivate,
  void* eglImage);

OMX_ERRORTYPE omx_base_component_FreeBuffer(
  OMX_HANDLETYPE hComponent,
  OMX_U32 nPortIndex,
  OMX_BUFFERHEADERTYPE* pBuffer);

OMX_ERRORTYPE omx_base_component_EmptyThisBuffer(
  OMX_HANDLETYPE hComponent,
  OMX_BUFFERHEADERTYPE* pBuffer);

OMX_ERRORTYPE omx_base_component_FillThisBuffer(
  OMX_HANDLETYPE hComponent,
  OMX_BUFFERHEADERTYPE* pBuffer);

OMX_ERRORTYPE omx_base_component_ComponentTunnelRequest(
  OMX_HANDLETYPE hComp,
  OMX_U32 nPort,
  OMX_HANDLETYPE hTunneledComp,
  OMX_U32 nTunneledPort,
  OMX_TUNNELSETUPTYPE* pTunnelSetup);

#endif
