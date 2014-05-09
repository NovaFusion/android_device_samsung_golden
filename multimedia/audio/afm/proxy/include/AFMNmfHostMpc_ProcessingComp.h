/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfHostMpc_ProcessingComp.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_HOSTMPCNMFPROCESSINGCOMP_H_
#define _AFM_HOSTMPCNMFPROCESSINGCOMP_H_

#include "NmfHostMpc_ProcessingComponent.h"
#include "AFM_HostController.h"
#include "AFM_PcmPort.h"

class Bindings;
class AFMNmfMpc_ProcessingComp;
class AFMNmfHost_ProcessingComp;

/// Abstract base class for AFM Nmf Host/Mpc Processing components
class AFMNmfHostMpc_ProcessingComp: public NmfHostMpc_ProcessingComponent {
  ////////////////////////
  // Constructor
  ////////////////////////
public:
  
  AFMNmfHostMpc_ProcessingComp(ENS_Component &enscomp) 
	: NmfHostMpc_ProcessingComponent(enscomp),
	  mNmfMain(0),
	  mBindings(0),
	  mController(enscomp, *this),
	  mIsPortInstantiated(0),
	  mNmfBufferSupplier(0) {}    
  
  friend class AFMNmfMpc_ProcessingComp;
  friend class AFMNmfHost_ProcessingComp;

  ////////////////////////
  // Component life cycle
  ////////////////////////
public :
  
  AFM_API_IMPORT virtual OMX_ERRORTYPE construct(void);
  AFM_API_IMPORT virtual OMX_ERRORTYPE destroy(void);
  
protected:
  
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE instantiate(void);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiate(void);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE start(void);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE stop(void);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE configure(void);
  
  virtual OMX_ERRORTYPE instantiateMain(void)   = 0;
  virtual OMX_ERRORTYPE startMain(void)         = 0;
  virtual OMX_ERRORTYPE configureMain(void)     = 0;
  virtual OMX_ERRORTYPE stopMain(void)          = 0;
  virtual OMX_ERRORTYPE deInstantiateMain(void) = 0;

  ////////////////////////
  // Command - event 
  ////////////////////////
protected:
  
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE doSpecificEventHandler_cb(OMX_EVENTTYPE event, OMX_U32 nData1, OMX_U32 nData2, OMX_BOOL &deferEventHandler);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE doSpecificSendCommand(OMX_COMMANDTYPE eCmd, OMX_U32 nData, OMX_BOOL &bDeferredCmd);


private:
  OMX_ERRORTYPE specificSendEnablePortCommand(OMX_U32 portIndex);
  OMX_ERRORTYPE disablePortCommandComplete(OMX_U32 portIndex);


  ////////////////////////////////
  // Buffer allocation - callback
  ////////////////////////////////
protected:
  
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE allocateBuffer(OMX_U32 nPortIndex,OMX_U32 nBufferIndex, OMX_U32 nSizeBytes, 
                                                 OMX_U8 **ppData, void **bufferAllocInfo, void **portPrivateInfo);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE freeBuffer(OMX_U32 nPortIndex, OMX_U32 nBufferIndex, OMX_BOOL bBufferAllocated,
                                             void *bufferAllocInfo, void *portPrivateInfo);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE useBufferHeader(OMX_DIRTYPE dir, OMX_BUFFERHEADERTYPE* pBuffer);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE useBuffer(OMX_U32 nPortIndex, OMX_U32 nBufferIndex,
                                            OMX_BUFFERHEADERTYPE* pBufferHdr, void **portPrivateInfo);
  
  /*final*/ virtual void doSpecificEmptyBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) { return;}
  /*final*/ virtual void doSpecificFillBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) { return;}

  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE doBufferAllocation(OMX_U32 nPortIndex, OMX_U32 nBufferIndex,OMX_U32 nSizeBytes,
                                                     OMX_U8 **ppData, void **bufferAllocInfo);
  /*final*/ AFM_API_IMPORT virtual OMX_ERRORTYPE doBufferDeAllocation(OMX_U32 nPortIndex, OMX_U32 nBufferIndex, void *bufferAllocInfo);
  
  /*final*/ AFM_API_IMPORT virtual OMX_U32 getBufferPhysicalAddress(void *bufferAllocInfo, OMX_U8 *,OMX_U32);
  /*final*/ AFM_API_IMPORT virtual OMX_U32 getBufferMpcAddress(void *bufferAllocInfo);
  
  /*final*/ virtual bool useBufferNeedsMemcpy(void) const { return true; }
  

  
  ////////////////////////////////
  // Bindings - Proprietary Comm
  ////////////////////////////////
public:

  ENS_Component & getENSComponent() const { return mENSComponent; }
  bool            isMainInstantiated() const { return mNmfMain;}
  bool            isPortInstantiated(OMX_U32 nPortIndex) const {
    if(mIsPortInstantiated == 0) return false;
    return mIsPortInstantiated[nPortIndex];
  }
  OMX_U32 getPortPriorityLevel(OMX_U32 portIndex) const {
          return ((static_cast<AFM_Port *>(mENSComponent.getPort(portIndex)))->getPortPriorityLevel());
  }

protected:

  // This is a hybrid component at least one of its port is a NMF Mpc component
  virtual t_cm_instance_handle getNmfMpcHandle(OMX_U32 portidx) const = 0;

  // To be implemented by derived class, if more that one input (resp. output) port.
  virtual const char *         getNmfSuffix(OMX_U32 portidx) const;

  NMF::Composite *             mNmfMain;
  
private:
  Bindings **              mBindings;
  AFM_HostController       mController;
  bool *                   mIsPortInstantiated;
  OMX_BUFFERSUPPLIERTYPE * mNmfBufferSupplier;
  OMX_U32                  mMyPriority;

  NMF::Composite *      getNmfHostHandle(void) const;
  NMF::Composite *      getNmfHostPortHandle(OMX_U32 portIdx) const;
  t_cm_instance_handle  getNmfMpcPortHandle(OMX_U32 portIdx) const;
  const char *          getNmfPortSuffix(OMX_U32 portIdx) const;
  
  OMX_ERRORTYPE instantiateBindingComponent(OMX_U32 portIdx);
  OMX_ERRORTYPE deInstantiateBindingComponent(OMX_U32 portIdx);
  OMX_ERRORTYPE createNmfShmPcm(AFM_Port *port);
  OMX_ERRORTYPE createNmfShm(AFM_Port *port);

  OMX_ERRORTYPE setupProprietaryCom(AFM_Port *port);
  OMX_ERRORTYPE cancelProprietaryCom(AFM_Port *port);
  OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port * port, AFMNmfMpc_ProcessingComp & pTunneledProcessingComp);
  OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port * port, AFMNmfHostMpc_ProcessingComp & pTunneledProcessingComp);
  OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port * port, AFMNmfHost_ProcessingComp & pTunneledProcessingComp);
  OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port * port, AFMNmfMpc_ProcessingComp & pTunneledProcessingComp);
  OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port * port, AFMNmfHostMpc_ProcessingComp & pTunneledProcessingComp);
  OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port * port, AFMNmfHost_ProcessingComp & pTunneledProcessingComp);
  OMX_ERRORTYPE addBindingComponentInputPortProprietaryCom(AFM_PcmPort *port);
  OMX_ERRORTYPE addBindingComponentOutputPortProprietaryCom(AFM_PcmPort *port);
};

#endif // _AFM_HOSTMPCNMFPROCESSINGCOMP_H_
