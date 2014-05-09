/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfMpc_Decoder.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_DECNMFPROCESSINGCOMP_H_
#define _AFM_DECNMFPROCESSINGCOMP_H_

#include "AFMNmfMpc_ProcessingComp.h"

#include "host/malloc/setheap.hpp"
#include "host/decoders/streamed/configure.hpp"

/// Abstract base class for AFM decoder processing components
class AFMNmfMpc_Decoder: public AFMNmfMpc_ProcessingComp {
public:

    AFMNmfMpc_Decoder(ENS_Component &enscomp) 
            : AFMNmfMpc_ProcessingComp(enscomp), mNmfil(0), mNmfBitstreamLib(0), 
            mNmfSyncLib(0), mMemory_buffer_in(0), mMemory_buffer_out(0) {
    }

    AFM_API_IMPORT virtual OMX_ERRORTYPE construct(void);

protected:
    // handle of the NMF component which provides the decoder interface
    t_cm_instance_handle  mNmfil;

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
    t_cm_instance_handle                mNmfBitstreamLib;      
    t_cm_instance_handle                mNmfSyncLib;

    t_cm_memory_handle                  mMemory_buffer_in;
    t_cm_memory_handle                  mMemory_buffer_out;

    Idecoders_streamed_configure        mIconfigure;
    
    // Private methods
    OMX_ERRORTYPE   instantiateLibraries();
    OMX_ERRORTYPE   deInstantiateLibraries();
    OMX_ERRORTYPE   startLibraries();
    OMX_ERRORTYPE   stopLibraries();
};

#endif // _AFM_DECNMFPROCESSINGCOMP_H_
