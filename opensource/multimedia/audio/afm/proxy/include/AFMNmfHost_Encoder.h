/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFMNmfHost_Encoder.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_HOSTENCNMFPROCESSINGCOMP_H_
#define _AFM_HOSTENCNMFPROCESSINGCOMP_H_

#include "AFMNmfHost_ProcessingComp.h"

#include "hst/encoders/framealigned/configure.hpp"

/// Abstract base class for AFM encoder processing components
class AFMNmfHost_Encoder: public AFMNmfHost_ProcessingComp {
public:

    AFMNmfHost_Encoder(ENS_Component &enscomp) 
            : AFMNmfHost_ProcessingComp(enscomp) {mNmfil = 0;}

    AFM_API_IMPORT virtual OMX_ERRORTYPE construct(void);

protected:
    // handle of the NMF component which provides the encoder interface
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
    Ihst_encoders_framealigned_configure        mIconfigure;
    
};

#endif // _AFM_HOSTENCNMFPROCESSINGCOMP_H_
