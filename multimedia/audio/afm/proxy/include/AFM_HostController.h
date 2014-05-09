/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_HostController.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_HOSTCONTROLLER_H_
#define _AFM_HOSTCONTROLLER_H_

#include "AFM_Controller.h"
#include "hst/composite/controller.hpp"

#define NB_MAX_PORTS  10

class NmfHost_ProcessingComponent;

class AFM_HostController : public AFM_Controller{
public:
 AFM_HostController(ENS_Component &enscomp, NmfHost_ProcessingComponent &hostProcessingComp) :
   AFM_Controller(enscomp), mHostProcessingComp(hostProcessingComp), mNmfCtrl(0) {}
  
  OMX_ERRORTYPE   instantiate(OMX_U32 priority);
  virtual OMX_ERRORTYPE   configure(void);
  virtual OMX_ERRORTYPE   start(void);
  virtual OMX_ERRORTYPE   stop(void);
  virtual OMX_ERRORTYPE   deInstantiate(void);

  virtual OMX_ERRORTYPE   bindMain(NMF::Composite * mainComp);
  virtual OMX_ERRORTYPE   bindMain(t_cm_instance_handle mainComp)   { DBC_ASSERT(0);return OMX_ErrorNotImplemented; }
  virtual OMX_ERRORTYPE   unbindMain(NMF::Composite * mainComp);
  virtual OMX_ERRORTYPE   unbindMain(t_cm_instance_handle mainComp) { DBC_ASSERT(0);return OMX_ErrorNotImplemented; }
  
  virtual OMX_ERRORTYPE   bindBinding(NMF::Composite * bindingComp, int portIdx);
  virtual OMX_ERRORTYPE   bindBinding(t_cm_instance_handle bindingComp, int portIdx);
  virtual OMX_ERRORTYPE   unbindBinding(NMF::Composite * bindingComp, int portIdx);
  virtual OMX_ERRORTYPE   unbindBinding(t_cm_instance_handle bindingComp, int portIdx);
	
private:
  NmfHost_ProcessingComponent &   mHostProcessingComp;

  // NMF controller object
  NMF::Composite *                mNmfCtrl;

  Ihst_misc_afmcontroller_configure mIconfigure;
};

#endif // _AFM_HOSTCONTROLLER_H_
