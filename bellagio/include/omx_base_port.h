/**
  src/base/omx_base_port.h

  Base class for OpenMAX ports to be used in derived components.

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

#include "tsemaphore.h"
#include "queue.h"
#include "omx_classmagic.h"

#ifndef __OMX_BASE_PORT_H__
#define __OMX_BASE_PORT_H__

#define TUNNEL_USE_BUFFER_RETRY 20
#define TUNNEL_USE_BUFFER_RETRY_USLEEP_TIME 50000

/**
 * Port Specific Macro's
 */
#define PORT_IS_BEING_FLUSHED(pPort)                             (pPort->bIsPortFlushed == OMX_TRUE)
#define PORT_IS_BEING_DISABLED(pPort)                            (pPort->bIsTransientToDisabled == OMX_TRUE)
#define PORT_IS_ENABLED(pPort)                                   (pPort->sPortParam.bEnabled == OMX_TRUE)
#define PORT_IS_POPULATED(pPort)                                 (pPort->sPortParam.bPopulated == OMX_TRUE)
#define PORT_IS_TUNNELED(pPort)                                  (pPort->nTunnelFlags & TUNNEL_ESTABLISHED)
#define PORT_IS_DEEP_TUNNELED(pPort)                             (pPort->nTunnelFlags & PROPRIETARY_COMMUNICATION_ESTABLISHED)
#define PORT_IS_BUFFER_SUPPLIER(pPort)                           (pPort->nTunnelFlags & TUNNEL_IS_SUPPLIER)
#define PORT_IS_TUNNELED_N_BUFFER_SUPPLIER(pPort)                ((pPort->nTunnelFlags & TUNNEL_ESTABLISHED) && (pPort->nTunnelFlags & TUNNEL_IS_SUPPLIER))

/** The following enum values are used to characterize each buffer
  * allocated or assigned to the component. A buffer list is
  * created for each port of the component. The buffer can be assigned
  * to the port, or owned by the port. The buffer flag are applied for each buffer
  * in each port buffer list. The following use cases are currently implemented:
  * - When the IL Client asks the component to allocate a buffer
  *   for a given port, with the call to AllocateBuffer, the
  *   buffer created is characterizeed by the flag BUFFER_ALLOCATED
  * - When the IL Client asks the component to use a buffer allocated
  *   by the client itself, the buffer flag is BUFFER_ASSIGNED
  * - When the component is tunneled by another component, and the first
  *   is supplier of the buffer, the buffer is marked with the
  *   BUFFER_ALLOCATED flag.
  * - When the component is tunneled by another component, and the second
  *   is supplier of the buffer, the buffer is marked with the
  *   BUFFER_ASSIGNED flag.
  * - The case of a buffer supplied by the first component but allocated by another
  *   component or another port inside the same component, as in the case
  *   of shared buffers, is not yet implemented in these components
  * - During hte deallocation phase each buffer is marked with the BUFFER_FREE
  *   flag, so that the component can check if all the buffers have been deallocated
  *   before switch the component state to Loaded, as specified by
  *   the OpenMAX specs
  */
typedef enum BUFFER_STATUS_FLAG {
  BUFFER_FREE = 0,
  BUFFER_ALLOCATED = 0x0001,  /**< This flag is applied to a buffer when it is allocated
                 by the given port of the component */
  BUFFER_ASSIGNED = 0x0002, /**< This flag is applied to a buffer when it is assigned
                from another port or by the IL client */
  HEADER_ALLOCATED = 0x0004 /**< This flag is applied to a buffer when buffer header is allocated
                by the given port of the component */
  } BUFFER_STATUS_FLAG;

/** @brief the status of a port related to the tunneling with another component
 */
typedef enum TUNNEL_STATUS_FLAG {
  NO_TUNNEL = 0, /**< No tunnel established */
  TUNNEL_ESTABLISHED = 0x0001, /**< the TUNNEL_ESTABLISHED specifies if a port is tunneled.
                              * It is assigned to a private field of the port if it is tunneled
                              */
  TUNNEL_IS_SUPPLIER = 0x0002, /**< the TUNNEL_IS_SUPPLIER specifies if a tunneled port is the supplier.
                              * It is assigned to a private field of the port if it is tunneled and also it is the buffer supplier for the tunnel.
                              */
  PROPRIETARY_COMMUNICATION_ESTABLISHED = 0x0004 /** The tunnel established is created between two components of the same
                                                  * vendor. These components can take advantage from a vendor specific
                                                  * communication
                                                  */
} TUNNEL_STATUS_FLAG;


/**
 * @brief the base structure that describes each port.
 *
 * The data structure that describes a port contains the basic elements used in the
 * base component. Other elements can be added in the derived components structures.
 */
CLASS(omx_base_PortType)
#define omx_base_PortType_FIELDS \
  OMX_HANDLETYPE hTunneledComponent; /**< @param hTunneledComponent Handle to the tunnelled component */\
  OMX_U32 nTunnelFlags; /**< This field contains one or more tags that describe the tunnel status of the port */\
  OMX_U32 nTunneledPort; /**< @param nTunneledPort Tunneled port number */ \
  OMX_BUFFERSUPPLIERTYPE eBufferSupplier; /**< @param eBufferSupplier the type of supplier in case of tunneling */\
  OMX_U32 nNumTunnelBuffer; /**< @param nNumTunnelBuffer Number of buffer to be tunnelled */\
  tsem_t* pAllocSem; /**< @param pFlushSem Semaphore that locks the execution until the buffers have been flushed, if needed */ \
  pthread_mutex_t exitMutex; /** This mutex synchronizes the access to the boolean variable bIsDestroying */ \
  OMX_BOOL bIsDestroying; /** This variable is set to true when the port has been selected for destruction */ \
  OMX_U32 nNumBufferFlushed; /**< @param nNumBufferFlushed Number of buffer Flushed */\
  OMX_BOOL bIsPortFlushed;/**< @param bIsPortFlushed Boolean variables indicate port is being flushed at the moment */ \
  queue_t* pBufferQueue; /**< @param pBufferQueue queue for buffer to be processed by the port */\
  tsem_t* pBufferSem; /**< @param pBufferSem Semaphore for buffer queue access synchronization */\
  OMX_U32 nNumAssignedBuffers; /**< @param nNumAssignedBuffers Number of buffer assigned on each port */\
  OMX_PARAM_PORTDEFINITIONTYPE sPortParam; /**< @param sPortParam General OpenMAX port parameter */\
  OMX_BUFFERHEADERTYPE **pInternalBufferStorage; /**< This array contains the reference to all the buffers hadled by this port and already registered*/\
  BUFFER_STATUS_FLAG *bBufferStateAllocated; /**< @param bBufferStateAllocated The State of the Buffer whether assigned or allocated */\
  OMX_COMPONENTTYPE *standCompContainer;/**< The OpenMAX component reference that contains this port */\
  OMX_BOOL bIsTransientToEnabled;/**< It indicates that the port is going from disabled to enabled */ \
  OMX_BOOL bIsTransientToDisabled;/**< It indicates that the port is going from enabled to disabled */ \
  OMX_BOOL bIsFullOfBuffers; /**< It indicates if the port has all the buffers needed */ \
  OMX_BOOL bIsEmptyOfBuffers;/**< It indicates if the port has no buffers*/ \
  OMX_ERRORTYPE (*PortConstructor)(OMX_COMPONENTTYPE *openmaxStandComp,omx_base_PortType **openmaxStandPort,OMX_U32 nPortIndex, OMX_BOOL isInput); /**< The contructor of the port. It fills all the other function pointers */ \
  OMX_ERRORTYPE (*PortDestructor)(omx_base_PortType *openmaxStandPort); /**< The destructor of the port*/ \
  OMX_ERRORTYPE (*Port_DisablePort)(omx_base_PortType *openmaxStandPort); /**< Disables the port */ \
  OMX_ERRORTYPE (*Port_EnablePort)(omx_base_PortType *openmaxStandPort); /**< Enables the port */ \
  OMX_ERRORTYPE (*Port_SendBufferFunction)(omx_base_PortType *openmaxStandPort, OMX_BUFFERHEADERTYPE* pBuffer);  /**< Holds the EmptyThisBuffer of FillThisBuffer function, if the port is input or output */ \
  OMX_ERRORTYPE (*Port_AllocateBuffer)(omx_base_PortType *openmaxStandPort, OMX_BUFFERHEADERTYPE** pBuffer, OMX_U32 nPortIndex, OMX_PTR pAppPrivate, OMX_U32 nSizeBytes);/**< Replaces the AllocateBuffer call for the base port. */   \
  OMX_ERRORTYPE (*Port_UseBuffer)(omx_base_PortType *openmaxStandPort,OMX_BUFFERHEADERTYPE** ppBufferHdr,OMX_U32 nPortIndex,OMX_PTR pAppPrivate,OMX_U32 nSizeBytes,OMX_U8* pBuffer);/**< The standard use buffer function applied to the port class */ \
  OMX_ERRORTYPE (*Port_FreeBuffer)(omx_base_PortType *openmaxStandPort,OMX_U32 nPortIndex,OMX_BUFFERHEADERTYPE* pBuffer); /**< The standard free buffer function applied to the port class */ \
  OMX_ERRORTYPE (*Port_AllocateTunnelBuffer)(omx_base_PortType *openmaxStandPort,OMX_U32 nPortIndex);/**< AllocateTunnelBuffer call for the base port. */   \
  OMX_ERRORTYPE (*Port_FreeTunnelBuffer)(omx_base_PortType *openmaxStandPort,OMX_U32 nPortIndex); /**< The free buffer function used to free tunnelled buffers */ \
  OMX_ERRORTYPE (*BufferProcessedCallback)(OMX_HANDLETYPE hComponent, OMX_PTR pAppData, OMX_BUFFERHEADERTYPE* pBuffer);/**< Holds the EmptyBufferDone or FillBufferDone callback, if the port is input or output port */ \
  OMX_ERRORTYPE (*FlushProcessingBuffers)(omx_base_PortType *openmaxStandPort); /**< release all the buffers currently under processing */ \
  OMX_ERRORTYPE (*ReturnBufferFunction)(omx_base_PortType* openmaxStandPort,OMX_BUFFERHEADERTYPE* pBuffer); /**< Call appropriate function to return buffers to peer or IL Client*/ \
  OMX_ERRORTYPE (*ComponentTunnelRequest)(omx_base_PortType* openmaxStandPort, OMX_HANDLETYPE hTunneledComp, OMX_U32 nTunneledPort, OMX_TUNNELSETUPTYPE* pTunnelSetup); /**< Setup tunnel with the port */
ENDCLASS(omx_base_PortType)

/**
 * @brief The base contructor for the generic OpenMAX ST port
 *
 * This function is executed by the component that uses a port.
 * The parameter contains the info about the component.
 * It takes care of constructing the instance of the port and
 * every object needed by the base port.
 *
 * @param openmaxStandPort the ST port to be initialized
 *
 * @return OMX_ErrorInsufficientResources if a memory allocation fails
 */
OMX_ERRORTYPE base_port_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,omx_base_PortType **openmaxStandPort,OMX_U32 nPortIndex, OMX_BOOL isInput);

/** @brief The base destructor for the generic OpenMAX ST port
 *
 * This function is executed by the component that uses a port.
 * The parameter contains the info about the component.
 * It takes care of destructing the instance of the port and
 * every object used by the base port.
 *
 * @param openmaxStandPort the ST port to be disposed
  */
OMX_ERRORTYPE base_port_Destructor(omx_base_PortType *openmaxStandPort);

/** @brief Disables the port.
 *
 * This function is called due to a request by the IL client
 *
 * @param openmaxStandPort the reference to the port
 *
 */
OMX_ERRORTYPE base_port_DisablePort(omx_base_PortType *openmaxStandPort);

/** @brief Enables the port.
 *
 * This function is called due to a request by the IL client
 *
 * @param openmaxStandPort the reference to the port
 *
 */
OMX_ERRORTYPE base_port_EnablePort(omx_base_PortType *openmaxStandPort);

/** @brief The entry point for sending buffers to the port
 *
 * This function can be called by the EmptyThisBuffer or FillThisBuffer. It depends on
 * the nature of the port, that can be an input or output port.
 */
OMX_ERRORTYPE base_port_SendBufferFunction(
  omx_base_PortType *openmaxStandPort,
  OMX_BUFFERHEADERTYPE* pBuffer);

/** @brief Called by the standard allocate buffer, it implements a base functionality.
 *
 * This function can be overriden if the allocation of the buffer is not a simply malloc call.
 * The parameters are the same as the standard function, except for the handle of the port
 * instead of the handler of the component
 * When the buffers needed by this port are all assigned or allocated, the variable
 * bIsFullOfBuffers becomes equal to OMX_TRUE
 */
OMX_ERRORTYPE base_port_AllocateBuffer(
  omx_base_PortType *openmaxStandPort,
  OMX_BUFFERHEADERTYPE** pBuffer,
  OMX_U32 nPortIndex,
  OMX_PTR pAppPrivate,
  OMX_U32 nSizeBytes);

/** @brief Called by the standard use buffer, it implements a base functionality.
 *
 * This function can be overriden if the use buffer implicate more complicated operations.
 * The parameters are the same as the standard function, except for the handle of the port
 * instead of the handler of the component
 * When the buffers needed by this port are all assigned or allocated, the variable
 * bIsFullOfBuffers becomes equal to OMX_TRUE
 */
OMX_ERRORTYPE base_port_UseBuffer(
  omx_base_PortType *openmaxStandPort,
  OMX_BUFFERHEADERTYPE** ppBufferHdr,
  OMX_U32 nPortIndex,
  OMX_PTR pAppPrivate,
  OMX_U32 nSizeBytes,
  OMX_U8* pBuffer);

/** @brief Called by the standard function.
 *
 * It frees the buffer header and in case also the buffer itself, if needed.
 * When all the bufers are done, the variable bIsEmptyOfBuffers is set to OMX_TRUE
 */
OMX_ERRORTYPE base_port_FreeBuffer(
  omx_base_PortType *openmaxStandPort,
  OMX_U32 nPortIndex,
  OMX_BUFFERHEADERTYPE* pBuffer);

/** @brief Releases buffers under processing.
 *
 * This function must be implemented in the derived classes, for the
 * specific processing
 */
OMX_ERRORTYPE base_port_FlushProcessingBuffers(omx_base_PortType *openmaxStandPort);

/** @brief Returns buffers when processed.
 *
 * Call appropriate function to return buffers to peer or IL Client
 */

OMX_ERRORTYPE base_port_ReturnBufferFunction(
  omx_base_PortType* openmaxStandPort,
  OMX_BUFFERHEADERTYPE* pBuffer);

/** @brief Setup Tunnel with the port
 */

OMX_ERRORTYPE base_port_ComponentTunnelRequest(
  omx_base_PortType* openmaxStandPort,
  OMX_HANDLETYPE hTunneledComp,
  OMX_U32 nTunneledPort,
  OMX_TUNNELSETUPTYPE* pTunnelSetup);

/** @brief Allocate Buffers for tunneling use
 */
OMX_ERRORTYPE base_port_AllocateTunnelBuffer(
  omx_base_PortType *openmaxStandPort,
  OMX_U32 nPortIndex);

/** @brief Free buffers used in tunnel
 */
OMX_ERRORTYPE base_port_FreeTunnelBuffer(
  omx_base_PortType *openmaxStandPort,
  OMX_U32 nPortIndex);


#endif
