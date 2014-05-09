/*****************************************************************************/
/**
*  © ST-Ericsson, 2010 - All rights reserved
*
*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson.
*
* \brief   Virtual Surround Host NMF processing class header
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _VIRTUAL_SURROUND_NMF_HOST_PCM_PROCESSING_H_
#define _VIRTUAL_SURROUND_NMF_HOST_PCM_PROCESSING_H_

#include "AFMNmfHost_PcmProcessing.h"
#include "virtual_surround/nmfil/host/effect/configure.hpp"

/**
 * \brief Class that manages the Virtual Surround NMF component for the host CPU.
 */
class VirtualSurroundNmfHost_PcmProcessing: public AFMNmfHost_PcmProcessing
{
   public:
       VirtualSurroundNmfHost_PcmProcessing(ENS_Component &enscomp):AFMNmfHost_PcmProcessing(enscomp) {}

       virtual OMX_ERRORTYPE construct(void);
       virtual OMX_ERRORTYPE destroy(void);
	   
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
       virtual OMX_U32       nbBitPerSampleProcessed(){return 16;};


   private:
       Ivirtual_surround_nmfil_host_effect_configure  mIConfig;

       void applyConfigVirtualSurround(void);

};

#endif /* _VIRTUAL_SURROUND_NMF_HOST_PCM_PROCESSING_H_ */
