/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   NmfMpcPcmAdapter.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_PCMADAPTER_H_
#define _AFM_PCMADAPTER_H_

#include "NmfComponent.h"
#include "AFM_macros.h"

#include "host/bindings/pcmadapter/configure.hpp"

//FIXME: code duplicated with PcmAdapter binding class!!!!!
//-> should be updated later!!!
class NmfMpcPcmAdapter: public NmfComponent {
    public:
        AFM_API_IMPORT NmfMpcPcmAdapter(OMX_U32 nInputBlockSize,
                OMX_U32 nOutputBlockSize,
                OMX_U32 nChansIn,
                OMX_U32 nChansOut,
                OMX_U32 nNbBuffersIn,
                OMX_U32 nNbBuffersOut,
                OMX_U32 nBitPerSampleIn,
                OMX_U32 nBitPerSampleOut,
                OMX_U32 nSamplingRate);

        AFM_API_IMPORT virtual OMX_ERRORTYPE instantiate(OMX_U32 domainId, OMX_U32 priority);
        AFM_API_IMPORT virtual OMX_ERRORTYPE start(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE stop(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiate(void);

        AFM_API_IMPORT OMX_ERRORTYPE configure(void);

    private:
		t_cm_instance_handle            	mNmfSyncLib;
		t_cm_instance_handle                mOstTrace;
        t_cm_memory_handle                  mBufferPcm;
        
        Ibindings_pcmadapter_configure      mIconfigure;
        PcmAdapterParam_t                   mParams;
		
        // Private methods
        OMX_ERRORTYPE       allocatePcmBuffer(void);
        OMX_ERRORTYPE       freePcmBuffer(void);
        int                 computeBufferSize(void);

        static int          gcd(int a, int b);
};

#endif //_AFM_PCMADAPTER_H_
