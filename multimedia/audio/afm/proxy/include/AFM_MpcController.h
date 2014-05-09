/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_MpcController.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_MPCCONTROLLER_H_
#define _AFM_MPCCONTROLLER_H_

#include "AFM_Controller.h"
#include "host/misc/controller/configure.hpp"

#define NB_MAX_PORTS  10

class AFMNmfMpc_ProcessingComp;

class AFM_MpcController : public AFM_Controller {
public:
  AFM_MpcController(ENS_Component &enscomp, AFMNmfMpc_ProcessingComp &afmProcessingComp) : 
    AFM_Controller(enscomp), mAFMProcessingComp(afmProcessingComp), mNmfCtrl(0), mNmfGenericFsm(0) {
  }

  OMX_ERRORTYPE   instantiate(OMX_U32 priority);
  virtual OMX_ERRORTYPE   configure(void);
  virtual OMX_ERRORTYPE   start(void);
  virtual OMX_ERRORTYPE   stop(void);
  virtual OMX_ERRORTYPE   deInstantiate(void);
	         
  virtual OMX_ERRORTYPE   bindMain(NMF::Composite * mainComp)    { DBC_ASSERT(0);return OMX_ErrorNotImplemented; }
  virtual OMX_ERRORTYPE   bindMain(t_cm_instance_handle mainComp);
  virtual OMX_ERRORTYPE   unbindMain(NMF::Composite * mainComp)  { DBC_ASSERT(0);return OMX_ErrorNotImplemented; }
  virtual OMX_ERRORTYPE   unbindMain(t_cm_instance_handle mainComp);
  
  virtual OMX_ERRORTYPE   bindBinding(NMF::Composite * bindingComp, int portIdx)     { DBC_ASSERT(0);return OMX_ErrorNotImplemented; }
  virtual OMX_ERRORTYPE   bindBinding(t_cm_instance_handle bindingComp, int portIdx);
  virtual OMX_ERRORTYPE   unbindBinding(NMF::Composite * bindingComp, int portIdx)   { DBC_ASSERT(0);return OMX_ErrorNotImplemented; }
  virtual OMX_ERRORTYPE   unbindBinding(t_cm_instance_handle bindingComp, int portIdx);
	        
private:
  AFMNmfMpc_ProcessingComp &    mAFMProcessingComp;

  // NMF stuff
  t_cm_instance_handle          mNmfCtrl;
  t_cm_instance_handle          mNmfGenericFsm;
  Imisc_controller_configure    mIconfigure;
};

#endif // _AFM_MPCCONTROLLER_H_
