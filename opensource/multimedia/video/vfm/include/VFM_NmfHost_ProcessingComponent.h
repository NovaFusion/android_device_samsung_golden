/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef __VFM_NmfHost_ProcessingComponent_H
#define __VFM_NmfHost_ProcessingComponent_H

#if ((!defined __SYMBIAN32__) && (!defined IMPORT_C))
    #define IMPORT_C
#endif
#if ((!defined __SYMBIAN32__) && (!defined EXPORT_C))
    #define EXPORT_C
#endif


#include "NmfHost_ProcessingComponent.h"
#include "common_frameinfo.idt"
#include "VFM_Memory.h"

/// @defgroup VFMHostNmfProcessingComponent Host NMF Processing Component
/// @ingroup VFM

/// @ingroup VFMHostNmfProcessingComponent
/// @brief This is the processing component. Its model is to interacts with a single
/// NMF Composite. This composite has a DDep on ARM that interacts with the proxy.
/// The NMF Composite may contain other NMF component, on ARM and/or on DSP.
/// Hence this component is suitable for full-sw implementation and also for
/// mixed implementation DSP + ARM where both the inputs and the outputs are
/// consumed / generated on the ARM side.
class VFM_NmfHost_ProcessingComponent : public NmfHost_ProcessingComponent
{
    public:
        /// @brief Contructor
        /// @param [in] enscomp the proxy this processing component belongs to
        IMPORT_C VFM_NmfHost_ProcessingComponent(ENS_Component &enscomp);

        /// @brief Destructor
        IMPORT_C virtual ~VFM_NmfHost_ProcessingComponent();

        /// @brief Instantiate the specific interfaces between the proxy and the ddep.
        /// It includes the interface emptyThisBuffer /  FillThisBuffer, as well as specific
        /// interfaces.
        IMPORT_C virtual OMX_ERRORTYPE instantiate(void);

        /// @brief Start the NMF component. This may be empty in the specific implementation,
        /// apart if any specific actions has to be taken.
        IMPORT_C virtual OMX_ERRORTYPE start(void);

        /// @brief Stop the NMF component. This may be empty in the specific implementation,
        /// apart if any specific actions has to be taken.
        IMPORT_C virtual OMX_ERRORTYPE stop(void);

        /// @brief Configure the NMF component. This may be empty in the specific implementation,
        /// apart if any specific actions has to be taken.
        IMPORT_C virtual OMX_ERRORTYPE configure(void);

        /// @brief Deinstantiate the specific interfaces between the proxy and the ddep.
        /// It includes the interface emptyThisBuffer /  FillThisBuffer, as well as specific
        /// interfaces.
        IMPORT_C virtual OMX_ERRORTYPE deInstantiate(void);

        /// @brief Allocate buffers on a port (this is the primary function called by the ENS on
        /// OMX_AllocateBuffers). Main process is done in doBufferAllocation().
        /// @param [in] nPortIndex is the index of the port to deallocate to
        /// @param [in] nBufferIndex is the buffer index (from 0 to countactual-1) of the buffer to deallocate
        /// @param [in] nSizeBytes is the size of buffer, in bytes
        /// @param [in] ppData
        /// @param [in] bufferAllocInfo is a meta data structure that contains memory private attribute
        /// (right now data from the HW Buffer API)
        /// @param [in] portPrivateInfo
        IMPORT_C virtual OMX_ERRORTYPE allocateBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex,
                OMX_U32 nSizeBytes,
                OMX_U8 **ppData,
                void **bufferAllocInfo,
                void **portPrivateInfo);

        /// @brief Indicate buffers on a port (this is the primary function called by the ENS on
        /// OMX_UseBuffers).
        /// @param [in] dir the direction of the port
        /// @param [in] pBuffer the buffer header to use
        IMPORT_C virtual OMX_ERRORTYPE useBufferHeader(
                OMX_DIRTYPE dir,
                OMX_BUFFERHEADERTYPE* pBuffer);

        /// @brief Indicate buffers on a port (this is the primary function called by the ENS on
        /// OMX_UseBuffers).
        /// @param [in] nPortIndex is the index of the port to deallocate to
        /// @param [in] nBufferIndex is the buffer index (from 0 to countactual-1) of the buffer to deallocate
        /// @param [in] pBufferHdr
        /// @param [in] portPrivateInfo
        IMPORT_C virtual OMX_ERRORTYPE useBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex,
                OMX_BUFFERHEADERTYPE* pBufferHdr,
                void **portPrivateInfo);

        /// @brief Deallocate buffers on a port (this is the primary function called by the ENS on
        /// OMX_FreeBuffers). Main process is done in doBufferDeAllocation().
        /// @param [in] nPortIndex is the index of the port to deallocate to
        /// @param [in] nBufferIndex is the buffer index (from 0 to countactual-1) of the buffer to deallocate
        /// @param [in] bBufferAllocated
        /// @param [in] bufferAllocInfo is a meta data structure that contains memory private attribute
        /// (right now data from the HW Buffer API)
        /// @param [in] portPrivateInfo
        IMPORT_C virtual OMX_ERRORTYPE freeBuffer(
        		OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex,
                OMX_BOOL bBufferAllocated,
                void *bufferAllocInfo,
                void *portPrivateInfo);

        /// @brief Create the interface of name nameInput (from the proxy to the ddep - typically
        /// "emptyThisBuffer") and the corresponding callback of name nameCB, associated to a
        /// given port of the VFM. This is intended to be used in the implementation of instantiate()
        /// @param [in] port the port we are interfacing to, for a EmptyThisBuffer or a FillThisBuffer
        /// @param [in] nameInput the name of the port
        /// @param [in] nameCB the name of the associated call-back
        /// @param [in] given_fifo_size the size of the NMF fifo for these interfaces. If 0, the nActualBufferCount is used.
        ///     Note that an assert is raised if the given size is lower then the ActualBufferCount
        IMPORT_C OMX_ERRORTYPE createPortInterface(VFM_Port *port, const char *nameInput, const char *nameCB, OMX_U32 given_fifo_size=0);

        /// @brief Destroy the interface of name nameInput (from the proxy to the ddep - typically
        /// "emptyThisBuffer") and the corresponding callback of name nameCB, associated to a
        /// given port of the VFM. This is intended to be used in the implementation of deInstantiate()
        IMPORT_C OMX_ERRORTYPE destroyPortInterface(VFM_Port *port, const char *nameInput, const char *nameCB);

        /// @brief Creates the dynamic array mPortPoolid[] depending on the number of ports. This is
        /// called from the construct of the proxy
        /// @param [in] nPortCount the number of port to instantiate
        IMPORT_C OMX_ERRORTYPE setPortCount(OMX_U32 nPortCount);

        /// @brief Send a new configuration to the coder. This function is called on a setConfig()
        /// @param [in] nParamIndex the index of the parameter that changed
        /// @param [in] pComponentConfigStructure the new configuration
        IMPORT_C virtual OMX_ERRORTYPE applyConfig(OMX_INDEXTYPE nParamIndex, OMX_PTR pComponentConfigStructure);

        IMPORT_C virtual void doSpecificEmptyBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer);
        IMPORT_C virtual void doSpecificFillBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer);

        inline OMX_BOOL isGoToLoaded() const { return mGoToLoaded; };

   protected:
        /// @brief Register the stubs and skels from ARM to DSP for the internal interfaces of the
        /// NMF Composite that is instantiated. This may be empty for full-sw implementation
        virtual void registerStubsAndSkels(void)=0;

        /// @brief Unregister the stubs and skels from ARM to DSP for the internal interfaces of the
        /// NMF Composite that is instantiated. This may be empty for full-sw implementation
        virtual void unregisterStubsAndSkels(void)=0;

        /// @brief Create the codec and initialize the member mCodec
        /// @param[in] domainID the domain ID, as returned by the RME
        /// @return A pointer to the codec, as a NMF on arm component
        virtual OMX_ERRORTYPE codecCreate(OMX_U32 domainId)=0;

        /// @brief Destroy the codec and initialize the member mCodec
        virtual void codecDestroy(void)=0;

        /// @brief Instantiate the specific interfaces between the proxy and the ddep.
        /// It includes the interface emptyThisBuffer /  FillThisBuffer, as well as specific
        /// interfaces.
        virtual OMX_ERRORTYPE codecInstantiate(void)=0;

        /// @brief Start the NMF component. This may be empty in the specific implementation,
        /// apart if any specific actions has to be taken.
        virtual OMX_ERRORTYPE codecStart(void)=0;

        /// @brief Stop the NMF component. This may be empty in the specific implementation,
        /// apart if any specific actions has to be taken.
        virtual OMX_ERRORTYPE codecStop(void)=0;

        /// @brief Configure the NMF component. This may be empty in the specific implementation,
        /// apart if any specific actions has to be taken.
        virtual OMX_ERRORTYPE codecConfigure(void)=0;

        /// @brief Deinstantiate the specific interfaces between the proxy and the ddep.
        /// It includes the interface emptyThisBuffer /  FillThisBuffer, as well as specific
        /// interfaces.
        virtual OMX_ERRORTYPE codecDeInstantiate(void)=0;

        /// @brief This method is called inside sendCommand to allow derived classes to do
        /// some specific stuffs before calling the NMFCALL.
        /// @param [in] eCmd the OMX command to send to the codec
        /// @param [in] nData the OMX parameter of the command
        /// @param [out] bDeferredCmd returns OMX_FALSE if SendCommand interface should be sent to the codec,
        ///    and OMX_TRUE if it should not
        /// @return OMX_ErrorNone if no error occurs
        IMPORT_C virtual OMX_ERRORTYPE doSpecificSendCommand(OMX_COMMANDTYPE eCmd, OMX_U32 nData, OMX_BOOL &bDeferredCmd);

        /// @brief This method is called inside eventHandler to allow derived classes to do
        /// some specific stuffs before calling the real callback.
        /// @param [in] event the event of the callback
        /// @param [in] nData1 the first OMX parameter of the callback
        /// @param [in] nData2 the second OMX parameter of the callback
        /// @param [out] bDeferredCmd returns FALSE if the event handler should be called after doSpecificEventHandler_cb
        /// @return OMX_ErrorNone if no error occurs
        IMPORT_C virtual OMX_ERRORTYPE doSpecificEventHandler_cb(OMX_EVENTTYPE event, OMX_U32 nData1, OMX_U32 nData2, OMX_BOOL &bDeferredCmd);

        /// @brief Pure virtual to be implemented in specific codec implementation of the processing component
        /// in order to send to the codec (ddep) new configuration
        virtual void sendConfigToCodec() = 0;

        /// @brief Generic callback called when the ddep send (usin interface get_config) a new
        /// auto-detected configuration to the codec. This is working only for common frame info.
        /// This function is in charge of storing the new configuration, and is in charge
        /// of raising appropriate callbacks (such as a PortSettingChange).
        /// @param[in] pFrameInfo the new configuration
        IMPORT_C void VFM_getConfigCB(t_common_frameinfo *pFrameInfo);

        /// @brief Retrieve the common configuration of the proxy.
        /// @param[out] pFrameInfo the returned configuration (only the common part)
        IMPORT_C void VFM_retrieveConfig(t_common_frameinfo *pFrameInfo);

        /// @brief Callback raised when the ddep send to the proxy the memory status.
        /// This status can then be retrieved using a GetConfig().
        /// @param[in] pMemoryStatus the memory status
        IMPORT_C void getMemoryCB(VFM_MemoryStatus *pMemoryStatus);

        /// @brief Sends an EmptyThisBuffer to the NMF component
        /// @param[in] pBuffer The buffer to empty
        /// @return OMX_ErrorNone if no errors
        IMPORT_C virtual OMX_ERRORTYPE emptyThisBuffer(OMX_BUFFERHEADERTYPE* pBuffer);


 	    /* +Change start for CR333616 Delay recycling */
        IMPORT_C virtual OMX_ERRORTYPE fillThisBuffer(OMX_BUFFERHEADERTYPE* pBuffer);
        /* -Change end for CR333616 Delay recycling */

        static void vfm_assert_static(int condition, int line, OMX_BOOL isFatal, int param1=0, int param2=0);

    protected:
        /// @brief A pointer to the NMF component that is associated with this
        /// processing component
        NMF::Composite *mCodec;

        /// @brief Indicate whether the codec received a configuration or not
        OMX_BOOL mCodecHasConfig;

        /// @brief Indicate whether the codec is intantiated or not
        OMX_BOOL mCodecIsInstantiated;

        /// @brief Value to indicate if we are going to loaded or not
        OMX_BOOL mGoToLoaded;

    public:
        /// @brief The latest known memory status
        VFM_MemoryStatus mMemoryStatus;
};


/// @brief Some macro definition to ease the porting of the code
#define VFM_NmfHost_ProcessingComponent_SENDCONFIGTOCODEC {                                                             \
    t_frameinfo frame_info;                                                                                             \
    codecRetrieveConfig(&frame_info);                                                                                   \
    if (mCodecIsInstantiated && (!mCodecHasConfig || frameInfoDifferent(&frame_info, &mCodecFrameInfo))) {              \
        mCodecFrameInfo = frame_info;                                                                                   \
        mCodecHasConfig = OMX_TRUE;                                                                                     \
        VFM_Component *pComp = &((VFM_Component &)mENSComponent);                                                       \
        mISetConfig.set_config(pComp->getAllocDdrMemoryDomain(), pComp->getAllocEsramMemoryDomain(), frame_info);       \
    }                                                                                                                   \
}

#define VFM_NmfHost_ProcessingComponent_SENDCONFIGURATION {                                                             \
		VFM_Component *pComp = &((VFM_Component &)mENSComponent);                                                       \
        mISetConfiguration.set_configuration(pComp->getOMXHandle());                             \
}

#define VFM_NmfHost_ProcessingComponent_SENDPARAM {                                                             \
		VFM_Component *pComp = &((VFM_Component &)mENSComponent);                                                       \
        mISetParam.set_param((void *)pComp->pParam);                             \
}

#endif
