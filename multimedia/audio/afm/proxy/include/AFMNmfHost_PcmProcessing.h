/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfHost_PcmProcessing.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_NMF_HOST_PCM_PROCESSING_H_
#define _AFM_NMF_HOST_PCM_PROCESSING_H_

#include "AFMNmfHost_ProcessingComp.h"

#include "hst/pcmprocessings/wrapper/configure.hpp"

/// Abstract base class for AFM PCM Processor processing components
class AFMNmfHost_PcmProcessing: public AFMNmfHost_ProcessingComp {
public:

  AFMNmfHost_PcmProcessing(ENS_Component &enscomp);
  virtual OMX_ERRORTYPE construct(void);

protected:
    // handle of the NMF component which provides the algo interface
    NMF::Composite *  mNmfil;

    virtual OMX_ERRORTYPE instantiateMain(void);
    virtual OMX_ERRORTYPE startMain(void);
    virtual OMX_ERRORTYPE configureMain(void);
    virtual OMX_ERRORTYPE stopMain(void);
    virtual OMX_ERRORTYPE deInstantiateMain(void);

    /// virtual methods called for algo specific stuff
    virtual OMX_ERRORTYPE instantiateAlgo() = 0;
    virtual OMX_ERRORTYPE startAlgo() = 0;
    virtual OMX_ERRORTYPE stopAlgo() = 0;
    virtual OMX_ERRORTYPE configureAlgo() = 0;
    virtual OMX_ERRORTYPE deInstantiateAlgo() = 0;
    virtual OMX_U32       nbBitPerSampleProcessed() = 0; // Should be 16 or 32

private:
    // Libraries
    Ihst_pcmprocessings_wrapper_configure        mIconfigure;
    t_host_pcmprocessing_config                  mPcmProcConfig;
    
};

#endif // _AFM_NMF_HOST_PCM_PROCESSING_H_
