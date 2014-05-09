/*****************************************************************************/
/**
*  © ST-Ericsson, 2011 - All rights reserved

*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \file    CNGNmfMpc.h
* \brief   Comfort Noise Generatoion MPC NMF processing class header
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _CN_NMF_MPC_H_
#define _CN_NMF_MPC_H_

#include "AFMNmfMpc_PcmProcessing.h"
#include "host/comfortnoise/nmfil/effect/configure.hpp"

/// @defgroup cng
/// @ingroup  afmpcmprocessing
/// @{


/**
 * \brief Class that manages the Comfort Noise Generation NMF component for MCP (DSP).
 */
class CNGNmfMpc : public AFMNmfMpc_PcmProcessing
{
    public:
        CNGNmfMpc(ENS_Component &enscomp): AFMNmfMpc_PcmProcessing(enscomp) {}

        virtual OMX_ERRORTYPE  construct(void);
        virtual OMX_ERRORTYPE  destroy(void);

        /**
          * \brief Apply settings to the NMF component. 
          * Called as a consequence of a setConfig call to the component.
        */
        virtual OMX_ERRORTYPE applyConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure);

    protected:
        virtual OMX_ERRORTYPE instantiateAlgo(void);
        virtual OMX_ERRORTYPE configureAlgo(void);
        virtual OMX_ERRORTYPE startAlgo(void);
        virtual OMX_ERRORTYPE stopAlgo(void);
        virtual OMX_ERRORTYPE deInstantiateAlgo(void);

    private:
    
	    /**
          * \brief Handle of the NMF component which provides the effect interface
        */

        //Comfort noise library
        t_cm_instance_handle                         mNmfCNGLib;
        Icomfortnoise_nmfil_effect_configure         mIconfigure;

	    void applyConfigCNG(void);
        void registerStubsAndSkels(void);
        void unregisterStubsAndSkels(void);
};

/// @}

#endif //  _CNG_NMF_MPC_H_
