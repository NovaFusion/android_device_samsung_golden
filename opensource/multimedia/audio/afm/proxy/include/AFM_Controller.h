/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_Controller.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_CONTROLLER_H_
#define _AFM_CONTROLLER_H_

#include <OMX_Core.h>
#include "ENS_Nmf.h"

class ENS_Component;

class AFM_Controller {
public:
  AFM_Controller(ENS_Component &enscomp) : mENSComponent(enscomp){}
  
  virtual OMX_ERRORTYPE   configure(void) = 0;
  virtual OMX_ERRORTYPE   start(void) = 0;
  virtual OMX_ERRORTYPE   stop(void) = 0;
  virtual OMX_ERRORTYPE   deInstantiate(void) = 0;

  virtual OMX_ERRORTYPE   bindMain(NMF::Composite * mainComp) = 0;
  virtual OMX_ERRORTYPE   bindMain(t_cm_instance_handle mainComp) = 0;
  virtual OMX_ERRORTYPE   unbindMain(NMF::Composite * mainComp) = 0;
  virtual OMX_ERRORTYPE   unbindMain(t_cm_instance_handle mainComp) = 0;
  
  virtual OMX_ERRORTYPE   bindBinding(NMF::Composite * bindingComp, int portIdx) = 0;
  virtual OMX_ERRORTYPE   bindBinding(t_cm_instance_handle bindingComp, int portIdx) = 0;
  virtual OMX_ERRORTYPE   unbindBinding(NMF::Composite * bindingComp, int portIdx) = 0;
  virtual OMX_ERRORTYPE   unbindBinding(t_cm_instance_handle bindingComp, int portIdx) = 0;

protected:
  ENS_Component &       mENSComponent;
};

#endif // _HOSTCONTROLLER_H_
