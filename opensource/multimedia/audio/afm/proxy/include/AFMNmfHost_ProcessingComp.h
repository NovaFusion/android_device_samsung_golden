//*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfHost_ProcessingComp.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_HOSTNMFPROCESSINGCOMP_H_
#define _AFM_HOSTNMFPROCESSINGCOMP_H_

#include "NmfHost_ProcessingComponent.h"
#include "AFM_HostController.h"
#include "AFM_Port.h"
#include "AFM_PcmPort.h"

class ENS_Component;
class HostBindings;

#ifndef HOST_ONLY
class AFMNmfHostMpc_ProcessingComp;
#endif

/// Abstract base class for  AFM Nmf Processing components
class AFMNmfHost_ProcessingComp: public NmfHost_ProcessingComponent {
    public:

        AFMNmfHost_ProcessingComp(ENS_Component &enscomp) 
  	  : NmfHost_ProcessingComponent(enscomp),
  	    mNmfMain(0),mController(enscomp, *this), mIsPortInstantiated(0) {}    

        AFM_API_IMPORT virtual OMX_ERRORTYPE construct(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE destroy(void);
        
        bool isMainInstantiated() const {
            return mNmfMain;
        }

        bool isPortInstantiated(OMX_U32 nPortIndex) const {
            if(mIsPortInstantiated == 0) return false;
            return mIsPortInstantiated[nPortIndex];

        }

        ENS_Component & getENSComponent() const {
            return mENSComponent;
        }
  
        OMX_U32 getPortPriorityLevel(OMX_U32 portIndex) const {
            return ((static_cast<AFM_Port *>(mENSComponent.getPort(portIndex)))->getPortPriorityLevel());
        }

        virtual OMX_U32 getPriorityLevel() const { return mMyPriority; };
        virtual void setPriorityLevel(OMX_U32 priority) { mMyPriority = priority; };

#ifndef HOST_ONLY
		friend class AFMNmfHostMpc_ProcessingComp;
#endif
    protected:

        AFM_API_IMPORT virtual OMX_ERRORTYPE instantiate(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiate(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE start(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE stop(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE configure(void);

        AFM_API_IMPORT virtual OMX_ERRORTYPE doBufferAllocation(OMX_U32 nBufferIndex,
                OMX_U32 nSizeBytes, OMX_U8 **ppData, void **bufferAllocInfo);

        AFM_API_IMPORT virtual OMX_ERRORTYPE doBufferDeAllocation(
                OMX_U32 nBufferIndex, void *bufferAllocInfo);

        virtual bool useBufferNeedsMemcpy(void) const { return true; }

        virtual void doSpecificEmptyBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) { return;}
        virtual void doSpecificFillBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) { return;}

        AFM_API_IMPORT virtual OMX_ERRORTYPE doSpecificEventHandler_cb(OMX_EVENTTYPE event, OMX_U32 nData1, OMX_U32 nData2, OMX_BOOL &deferEventHandler);

        AFM_API_IMPORT virtual OMX_ERRORTYPE doSpecificSendCommand(OMX_COMMANDTYPE eCmd, OMX_U32 nData, OMX_BOOL &bDeferredCmd);

        AFM_API_IMPORT virtual OMX_ERRORTYPE allocateBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex, 
                OMX_U32 nSizeBytes, 
                OMX_U8 **ppData, 
                void **bufferAllocInfo, 
                void **portPrivateInfo);

        AFM_API_IMPORT virtual OMX_ERRORTYPE useBufferHeader(
                OMX_DIRTYPE dir,
                OMX_BUFFERHEADERTYPE* pBuffer);

        AFM_API_IMPORT virtual OMX_ERRORTYPE useBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex, 
                OMX_BUFFERHEADERTYPE* pBufferHdr,
                void **portPrivateInfo);

        AFM_API_IMPORT virtual OMX_ERRORTYPE freeBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex, 
                OMX_BOOL bBufferAllocated, 
                void *bufferAllocInfo,
                void *portPrivateInfo);


        virtual OMX_ERRORTYPE instantiateMain(void) = 0;
        virtual OMX_ERRORTYPE startMain(void) = 0;
        virtual OMX_ERRORTYPE configureMain(void) = 0;
        virtual OMX_ERRORTYPE stopMain(void) = 0;
        virtual OMX_ERRORTYPE deInstantiateMain(void) = 0;

        /// allow derived classes to do specific stuffs on disablePort command complete event. 
        virtual OMX_ERRORTYPE disablePortNotification(OMX_U32 nPortIndex) {return OMX_ErrorNone;}

        AFM_API_IMPORT virtual NMF::Composite * getNmfHandle(OMX_U32 portidx) const;
        AFM_API_IMPORT virtual const char * getNmfSuffix(OMX_U32 portidx) const;   

        NMF::Composite *            mNmfMain;
        OMX_BUFFERSUPPLIERTYPE *    mNmfBufferSupplier;

    private:
        HostBindings **		mBindings;
        AFM_HostController	mController;
        bool *              mIsPortInstantiated;
        OMX_U32             mMyPriority;

        OMX_ERRORTYPE   instantiateBindingComponent(OMX_U32 portIdx);
        OMX_ERRORTYPE   deInstantiateBindingComponent(OMX_U32 portIdx);
        OMX_ERRORTYPE   createNmfShmPcm(AFM_Port *port);
        OMX_ERRORTYPE   createNmfShm(AFM_Port *port);
        OMX_ERRORTYPE   addBindingComponentOutputPortProprietaryCom(AFM_PcmPort *port);
        OMX_ERRORTYPE   addBindingComponentInputPortProprietaryCom(AFM_PcmPort *port);

        OMX_ERRORTYPE   specificSendEnablePortCommand(OMX_U32 portIndex);
        OMX_ERRORTYPE   disablePortCommandComplete(OMX_U32 portIndex);

        const char * getNmfPortSuffix(OMX_U32 portIdx) const;

        OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port *port,AFMNmfHost_ProcessingComp & pTunneledProcessingComp);
        OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port *port,AFMNmfHost_ProcessingComp & pTunneledProcessingComp);
#ifndef HOST_ONLY
        OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port *port,AFMNmfHostMpc_ProcessingComp & pTunneledProcessingComp);
        OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port *port,AFMNmfHostMpc_ProcessingComp & pTunneledProcessingComp);
#endif

        NMF::Composite *      getNmfPortHandle(OMX_U32 portIdx) const;
};

#endif // _AFM_HOSTNMFPROCESSINGCOMP_H_
