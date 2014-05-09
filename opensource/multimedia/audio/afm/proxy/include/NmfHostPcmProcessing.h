/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   NmfHostPcmProcessing.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_NMFHOSTPCMPROCESSING_H_
#define _AFM_NMFHOSTPCMPROCESSING_H_

#include "NmfComponent.h"
#include "AFM_macros.h"

#include "hst/pcmprocessings/wrapper/configure.hpp"

class NmfHostPcmProcessing: public NmfComponent {
    public:
        AFM_API_IMPORT NmfHostPcmProcessing(t_host_pcmprocessing_config config);

        AFM_API_IMPORT virtual OMX_ERRORTYPE instantiate(OMX_U32 domainId, OMX_U32 priority);
        AFM_API_IMPORT virtual OMX_ERRORTYPE start(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE stop(void);
        AFM_API_IMPORT virtual OMX_ERRORTYPE deInstantiate(void);

        AFM_API_IMPORT OMX_ERRORTYPE configure(void);

#ifndef __SYMBIAN32__             
	void print_fmt(t_host_effect_format * fmt);
#endif

        virtual OMX_ERRORTYPE instantiateAlgo(void) = 0;
        virtual OMX_ERRORTYPE startAlgo(void) = 0;
        virtual OMX_ERRORTYPE stopAlgo(void) = 0;
        virtual OMX_ERRORTYPE configureAlgo(void) = 0;
        virtual OMX_ERRORTYPE deInstantiateAlgo(void) = 0;


     private:
        t_host_pcmprocessing_config             mConfig;

        Ihst_pcmprocessings_wrapper_configure   mIconfigure;

    protected:
	NMF::Composite *                        mNmfAlgo;
};

#endif // _AFM_NMFHOSTPCMPROCESSING_H_
