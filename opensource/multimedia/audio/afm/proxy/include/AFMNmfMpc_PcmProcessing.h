/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfMpc_PcmProcessing.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_PCMPROCNMFPROCESSINGCOMP_H_
#define _AFM_PCMPROCNMFPROCESSINGCOMP_H_

#include "AFMNmfMpc_ProcessingComp.h"

#include "host/pcmprocessings/wrapper/configure.hpp"
#include "host/malloc/setheap.hpp"

class AFMNmfMpc_PcmProcessing: public AFMNmfMpc_ProcessingComp {
public:

    AFMNmfMpc_PcmProcessing(ENS_Component &enscomp) 
            : AFMNmfMpc_ProcessingComp(enscomp), mNmfil(0), mNmfSyncLib(0) {
        }

    AFM_API_IMPORT virtual OMX_ERRORTYPE construct(void);

protected:
    // handle of the NMF component which provides the pcmprocessing(effect) interface
    t_cm_instance_handle        mNmfil;

    AFM_API_IMPORT virtual OMX_ERRORTYPE doSpecificSendCommand(OMX_COMMANDTYPE eCmd, OMX_U32 nData, OMX_BOOL &bDeferredCmd);

	AFM_API_IMPORT virtual OMX_ERRORTYPE instantiateMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE startMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE configureMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE stopMain(void);
    AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiateMain(void);

    /// virtual methods called for algo specific stuff
    virtual OMX_ERRORTYPE instantiateAlgo(void) = 0;
    virtual OMX_ERRORTYPE startAlgo(void) = 0;
    virtual OMX_ERRORTYPE stopAlgo(void) = 0;
    virtual OMX_ERRORTYPE configureAlgo(void) = 0;
    virtual OMX_ERRORTYPE deInstantiateAlgo(void) = 0;
    virtual bool          supportsPcmProbe() const { return true; };
    virtual OMX_U32       getNmfPortIndex(OMX_U32 omxPortIdx) const { return omxPortIdx; };

private:
    //Host to Dsp NMF interfaces
    Ipcmprocessings_wrapper_configure       mIconfigure;
    t_cm_instance_handle                    mNmfSyncLib;
};

#endif // _AFM_PCMPROCNMFPROCESSINGCOMP_H_
