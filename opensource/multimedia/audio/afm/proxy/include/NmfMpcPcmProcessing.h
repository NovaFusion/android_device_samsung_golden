/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   NmfMpcPcmProcessing.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_NMFPCMPROCESSING_H_
#define _AFM_NMFPCMPROCESSING_H_

#include "NmfComponent.h"
#include "AFM_macros.h"

#include "host/pcmprocessings/wrapper/configure.hpp"

class NmfMpcPcmProcessing: public NmfComponent {
    public:
        AFM_API_IMPORT NmfMpcPcmProcessing(t_pcmprocessing_config config);

        AFM_API_IMPORT virtual OMX_ERRORTYPE instantiate(OMX_U32 domainId, OMX_U32 priority);
        AFM_API_IMPORT virtual OMX_ERRORTYPE start(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE stop(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiate(void);

        AFM_API_IMPORT OMX_ERRORTYPE configure(void);        

        virtual OMX_ERRORTYPE instantiateAlgo(void) = 0;
        virtual OMX_ERRORTYPE startAlgo(void) = 0;
        virtual OMX_ERRORTYPE stopAlgo(void) = 0;
        virtual OMX_ERRORTYPE configureAlgo(void) = 0;
        virtual OMX_ERRORTYPE deInstantiateAlgo(void) = 0;

     private:
        t_pcmprocessing_config                  mConfig;
        Ipcmprocessings_wrapper_configure       mIconfigure;
        OMX_BOOL                                mCallback;
        

    protected:
        t_cm_instance_handle                    mNmfSyncLib;
        t_cm_instance_handle                    mNmfAlgo;
        t_cm_instance_handle                    mOstTrace;
};

#endif // _AFM_NMFPCMPROCESSING_H_
