/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   NmfComponent.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIO_NMF_COMPONENT_A_H_
#define _AUDIO_NMF_COMPONENT_A_H_

#include <OMX_Types.h>
#include <OMX_Core.h>
#include <nmf/inc/component_type.h>
#include "AFM_macros.h"

class NmfComponent {
    public:

        //constructor
        NmfComponent(void) : mMyPriority(0), mNmfMain(0), mDomainHandle(0) {} 
            
        //destructor
        virtual ~NmfComponent(void){};

        // instantiate the NMF components
        virtual OMX_ERRORTYPE instantiate(OMX_U32 domainId, OMX_U32 priority) = 0;

        // deInstantiate the NMF components
        virtual OMX_ERRORTYPE deInstantiate(void) = 0;
        
        // start the NMF components
        virtual OMX_ERRORTYPE start(void) = 0;
        
        // stop the NMF components
        virtual OMX_ERRORTYPE stop(void) = 0;

        inline void * getNmfHandle(void) const{
            return mNmfMain;
        }
       
		AFM_API_IMPORT t_cm_instance_handle & getNmfMpcHandle(void) const;
        
        OMX_U32 getNMFDomainHandle(void) const{
            return mDomainHandle;
        }
        
        void setNMFDomainHandle(OMX_U32 domainHandle) {
            mDomainHandle = domainHandle;
        }

    protected:
        OMX_U32                 mMyPriority;

        void *                  mNmfMain;

    private :
        OMX_U32                 mDomainHandle;
};

#endif // _AUDIO_NMF_COMPONENT_A_H_
