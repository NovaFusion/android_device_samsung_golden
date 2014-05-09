/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfMpc_ProcessingComp.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_NMFPROCESSINGCOMP_H_
#define _AFM_NMFPROCESSINGCOMP_H_

#include "NmfMpc_ProcessingComponent.h"
#include "AFM_MpcController.h"
#include "AFM_MemoryPreset.h"
#include "AFM_Port.h"
#include "AFM_PcmPort.h"
#include "AFM_MpcPcmProbe.h"

class MpcBindings;
class AFMNmfHostMpc_ProcessingComp;


/// Abstract base class for  AFM Nmf Processing components
class AFMNmfMpc_ProcessingComp: public NmfMpc_ProcessingComponent {
    public:

        AFMNmfMpc_ProcessingComp(ENS_Component &enscomp)
            : NmfMpc_ProcessingComponent(enscomp),
            mNmfMain(0), mPcmProbe(enscomp, *this),
            mController(enscomp, *this), mIsPortInstantiated(0) {}

        AFM_API_IMPORT virtual ~AFMNmfMpc_ProcessingComp(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE construct(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE destroy(void);

        virtual	OMX_ERRORTYPE applyConfig(
                OMX_INDEXTYPE nConfigIndex,
                OMX_PTR pComponentConfigStructure);

        virtual	OMX_ERRORTYPE retrieveConfig(
                OMX_INDEXTYPE nConfigIndex,
                OMX_PTR pComponentConfigStructure);

        bool isMainInstantiated() const {
            return mNmfMain;
        }

        bool isPortInstantiated(OMX_U32 nPortIndex) const {
            if(mIsPortInstantiated == 0) return false;
            return mIsPortInstantiated[nPortIndex];

        }

        void * getSamplesPlayedPtr() const {return mSamplesPlayedPtr; };
        void setSamplesPlayedPtr(void * ptr) { mSamplesPlayedPtr = ptr; };
        OMX_U32 getPortPriorityLevel(OMX_U32 portIndex) const {
            return ((static_cast<AFM_Port *>(mENSComponent.getPort(portIndex)))->getPortPriorityLevel());
        }

        //Must be implemented by derived class in order to probe internal effects (OMX Mixer/Splitter special case)
        virtual PcmProbe_t * getProbeConfig(OMX_U32 portIdx, OMX_U32 effectPositionIdx) const {
            if(effectPositionIdx != 0) {
                return (PcmProbe_t *)NULL;
            } else {
                return mPcmProbe.getProbeConfig(portIdx);
            };
        }

        friend class AFMNmfHostMpc_ProcessingComp;
        friend class AFM_MpcPcmProbe;
    protected:

        AFM_API_IMPORT virtual OMX_ERRORTYPE instantiate(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiate(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE start(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE stop(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE configure(void);

        AFM_API_IMPORT virtual OMX_ERRORTYPE doBufferAllocation(OMX_U32 nPortIndex, OMX_U32 nBufferIndex,
                OMX_U32 nSizeBytes, OMX_U8 **ppData, void **bufferAllocInfo);

        AFM_API_IMPORT virtual OMX_ERRORTYPE doBufferDeAllocation(OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex, void *bufferAllocInfo);

        AFM_API_IMPORT virtual OMX_U32 getBufferPhysicalAddress(void *bufferAllocInfo, OMX_U8 *,OMX_U32);
        AFM_API_IMPORT virtual OMX_U32 getBufferMpcAddress(void *bufferAllocInfo);

        virtual bool useBufferNeedsMemcpy(void) const { return true; }

        virtual void doSpecificEmptyBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) { return;}
        virtual void doSpecificFillBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) { return;}
        AFM_API_IMPORT virtual OMX_ERRORTYPE doSpecificEventHandler_cb(OMX_EVENTTYPE event, OMX_U32 nData1, OMX_U32 nData2, OMX_BOOL & deferEventHandler);

        AFM_API_IMPORT virtual OMX_ERRORTYPE doSpecificSendCommand(OMX_COMMANDTYPE eCmd, OMX_U32 nData, OMX_BOOL &bDeferredCmd);

        virtual OMX_ERRORTYPE instantiateMain(void) = 0;
        virtual OMX_ERRORTYPE startMain(void) = 0;
        virtual OMX_ERRORTYPE configureMain(void) = 0;
        virtual OMX_ERRORTYPE stopMain(void) = 0;
        virtual OMX_ERRORTYPE deInstantiateMain(void) = 0;

        /// allow derived classes to do specific stuffs on disablePort command complete event.
        virtual OMX_ERRORTYPE disablePortNotification(OMX_U32 nPortIndex) {return OMX_ErrorNone;}

        AFM_API_IMPORT virtual t_cm_instance_handle getNmfHandle(OMX_U32 portIdx) const;
        AFM_API_IMPORT virtual const char * getNmfSuffix(OMX_U32 portIdx) const;

        // this function returns the nmf port index from the
        // omx port index
        virtual OMX_U32 getNmfPortIndex(OMX_U32 omxPortIdx) const { DBC_ASSERT(0); return 0;};

        virtual bool supportsPcmProbe() const { return false; };

        t_cm_instance_handle        mNmfMain;
        t_afm_memory_need           mMemoryNeed;
        t_cm_memory_handle          mHeap[MEM_BANK_COUNT];
        OMX_BUFFERSUPPLIERTYPE *    mNmfBufferSupplier;
        AFM_MpcPcmProbe             mPcmProbe;

    private:
        AFM_MpcController       mController;
        MpcBindings **          mBindings;
        bool *                  mIsPortInstantiated;
        void *                  mSamplesPlayedPtr;

        void            registerStubsAndSkels(void);
        void            unregisterStubsAndSkels(void);

        OMX_ERRORTYPE   instantiateBindingComponent(OMX_U32 portIdx);
        OMX_ERRORTYPE   deInstantiateBindingComponent(OMX_U32 portIdx);
        OMX_ERRORTYPE   createNmfShmPcm(AFM_Port *port);
        OMX_ERRORTYPE   createNmfShm(AFM_Port *port);
        OMX_ERRORTYPE   addBindingComponentOutputPortProprietaryCom(AFM_PcmPort *port);
        OMX_ERRORTYPE   addBindingComponentInputPortProprietaryCom(AFM_PcmPort *port);

        t_cm_instance_handle getNmfPortHandle(OMX_U32 portIdx) const;
        const char * getNmfPortSuffix(OMX_U32 portIdx) const;

        OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port *port,AFMNmfMpc_ProcessingComp & pTunneledProcessingComp);
        OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port *port,AFMNmfHostMpc_ProcessingComp & pTunneledProcessingComp);
        OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port *port,AFMNmfMpc_ProcessingComp & pTunneledProcessingComp);
        OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port *port,AFMNmfHostMpc_ProcessingComp & pTunneledProcessingComp);

        OMX_ERRORTYPE sendEnablePortCommand(OMX_U32 portIndex);
        OMX_ERRORTYPE disablePortCommandComplete(OMX_U32 portIndex);
};

#endif // _AFM_NMFPROCESSINGCOMP_H_

