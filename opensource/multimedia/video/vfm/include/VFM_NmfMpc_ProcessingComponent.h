/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_NmfMpc_ProcessingComponent_H
#define __VFM_NmfMpc_ProcessingComponent_H

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif

#include "NmfMpc_ProcessingComponent.h"

class VFM_NmfMpc_ProcessingComponent : public NmfMpc_ProcessingComponent
{
    public:
        IMPORT_C VFM_NmfMpc_ProcessingComponent(ENS_Component &enscomp);
		IMPORT_C virtual ~VFM_NmfMpc_ProcessingComponent();

        virtual OMX_ERRORTYPE instantiate(void)=0;
        virtual OMX_ERRORTYPE start(void)=0;
        virtual OMX_ERRORTYPE stop(void)=0;
        virtual OMX_ERRORTYPE configure(void)=0;
        virtual OMX_ERRORTYPE deInstantiate(void)=0;
        IMPORT_C virtual OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port *port);
        IMPORT_C virtual OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port *port);


        /// @brief Allocate ports. This function is automatically called by the VFM.
        /// They allow to allocate buffers for standard tunneling or non-tunneled communication.
        /// This allocation is used typically on OMX_AllocateBuffers()
        /// @param [in] nBufferIndex is the index (from 0 to countactual-1) of the buffer to allocate
        /// @param [in] nSizeBytes is the size, in bytes, to allocate
        /// @param [in] ppData is a meta data structure that contains memory private attribute
        /// (right now data from the HW Buffer API)
        /// @param [in] ppBufferMetaData is a meta data structure that contains memory private attribute
        /// (right now data from the HW Buffer API)
        /// @return An error if the allocation fails
        /// @note This is not intended to be used by specific proxies
        /// @note This function can be used directly (implemented in VFM_Port), or can be overloaded
        IMPORT_C virtual OMX_ERRORTYPE doBufferAllocation(OMX_U32 nPortIndex, OMX_U32 nBufferIndex, OMX_U32 nSizeBytes, OMX_U8 **ppData, void **ppBufferMetaData);

        /// @brief Deallocate ports. This function is automatically called by the VFM.
        /// They allow to deallocate buffers for standard tunneling or non-tunneled communication
        /// @param [in] nBufferIndex is the buffer index (from 0 to countactual-1) of the buffer to deallocate
        /// @param [in] ppBufferMetaData is a meta data structure that contains memory private attribute
        /// (right now data from the HW Buffer API)
        /// @note This function can be used directly (implemented in VFM_Port), or can be overloaded
        /// @note This function should be the reverse of the doBufferAllocation()
        IMPORT_C virtual OMX_ERRORTYPE doBufferDeAllocation(OMX_U32 nPortIndex, OMX_U32 nBufferIndex, void *ppBufferMetaData);

        /// @brief Get the physical address associated with a buffer meta data and the port
        /// @param [in] pBufferMetaData the buffer meta data
        /// @return the physical address
        /// @note this is linked to the doBufferAllocation() implementation
        /// @todo Parameter list is wrong. Documentation to be done
        IMPORT_C virtual OMX_U32 getBufferPhysicalAddress(void *bufferAllocInfo, OMX_U8 *,OMX_U32);

        /// @brief Get the DSP address associated with a buffer meta data and the port
        /// @param [in] pBufferMetaData the buffer meta data
        /// @return the DSP address
        /// @note this is linked to the doBufferAllocation() implementation
        IMPORT_C virtual OMX_U32 getBufferMpcAddress(void *bufferAllocInfo);

        /// @brief Internal variable used for allocation of buffers of the ports
        OMX_U32 *    mPortPoolid;

		IMPORT_C OMX_ERRORTYPE setPortCount(OMX_U32 nPortCount);

        virtual void doSpecificEmptyBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer);
        virtual void doSpecificFillBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer);

		IMPORT_C virtual OMX_ERRORTYPE freeBuffer(OMX_U32 nPortIndex,OMX_U32 nBufferIndex,OMX_BOOL bBufferAllocated,void *bufferAllocInfo,void *portPrivateInfo);

        /* +Change start for CR330198 Revert to DTS from PTS */
        /* +Change start for  CR333616 Delay recycling */
        virtual OMX_ERRORTYPE doSpecificSendCommand(OMX_COMMANDTYPE eCmd, OMX_U32 nData, OMX_BOOL &bDeferredCmd) = 0;
        IMPORT_C virtual OMX_ERRORTYPE fillThisBuffer(OMX_BUFFERHEADERTYPE* pBuffer);
        /* -Change end for CR333616 Delay recycling */
        /* -Change end for CR330198 Revert to DTS from PTS */

        /// @brief Sends an EmptyThisBuffer to the NMF component
        /// @param[in] pBuffer The buffer to empty
        /// @return OMX_ErrorNone if no errors
        IMPORT_C virtual OMX_ERRORTYPE emptyThisBuffer(OMX_BUFFERHEADERTYPE* pBuffer);

        static void vfm_assert_static(int condition, int line, OMX_BOOL isFatal, int param1=0, int param2=0);

   protected:
        virtual void registerStubsAndSkels(void)=0;
        virtual void unregisterStubsAndSkels(void)=0;
   };

#endif
