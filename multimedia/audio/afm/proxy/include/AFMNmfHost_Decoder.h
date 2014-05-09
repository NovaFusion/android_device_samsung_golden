/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfHost_Decoder.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_HOSTDECNMFPROCESSINGCOMP_H_
#define _AFM_HOSTDECNMFPROCESSINGCOMP_H_

#include "AFMNmfHost_ProcessingComp.h"

// commnent this if you want to use the streamed wrapper
//#define USE_FA_WRP

#ifdef USE_FA_WRP
#include "hst/decoders/framealigned/configure.hpp"
#else
#include "hst/decoders/streamed/configure.hpp"
#endif

/// Abstract base class for AFM decoder processing components
class AFMNmfHost_Decoder: public AFMNmfHost_ProcessingComp {
public:

    AFMNmfHost_Decoder(ENS_Component &enscomp) 
      : AFMNmfHost_ProcessingComp(enscomp) {bufInPtr = NULL; bufOutPtr = NULL; mNmfil = 0;}

    AFM_API_IMPORT virtual OMX_ERRORTYPE construct(void);

protected:
    // handle of the NMF component which provides the decoder interface
    NMF::Composite *  mNmfil;

    AFM_API_IMPORT virtual OMX_ERRORTYPE instantiateMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE startMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE configureMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE stopMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiateMain(void);

    /// virtual methods called for algo specific stuff
    virtual OMX_ERRORTYPE instantiateAlgo() = 0;
    virtual OMX_ERRORTYPE startAlgo() = 0;
    virtual OMX_ERRORTYPE stopAlgo() = 0;
    virtual OMX_ERRORTYPE configureAlgo() = 0;
    virtual OMX_ERRORTYPE deInstantiateAlgo() = 0;

private:
    // Libraries
#ifdef USE_FA_WRP
    Ihst_decoders_framealigned_configure        mIconfigure;
#else
    Ihst_decoders_streamed_configure        mIconfigure;
#endif
    // buffers alloated by the proxy for the streamed decoder
    OMX_U8 * bufInPtr, * bufOutPtr;
};

#endif // _AFM_HOSTDECNMFPROCESSINGCOMP_H_
