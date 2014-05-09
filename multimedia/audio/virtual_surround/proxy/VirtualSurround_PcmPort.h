/*****************************************************************************/
/**
*  © ST-Ericsson, 2010 - All rights reserved
*
*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson.
*
* \brief   Virtual surround PCM port definition
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _VIRTUAL_SURROUND_PCM_PORT_H_
#define _VIRTUAL_SURROUND_PCM_PORT_H_

#include "AFM_PcmPort.h"

#ifdef AFM_VIRTUAL_SURROUND_PORT_DEBUG
  #include <stdio.h>
  #define AFM_PORT_PRINT        printf("[VSURROUND_PCMPORT] "); printf
#else
  #define AFM_PORT_PRINT(x,...)
#endif


/**
 * \brief Class that implements specific port handling in the Virtual Surround OMX component
 */
class VirtualSurround_PcmPort : public AFM_PcmPort
{
   public:

       /**
	    * \brief Base class constructor
		*/
       VirtualSurround_PcmPort(OMX_U32 nIndex, OMX_U32 nBufferSize,
                                          const OMX_AUDIO_PARAM_PCMMODETYPE &defaultPcmSettings,
                                          ENS_Component &enscomponent);

       /**
        * \brief Check specific PCM settings in the effect.
        * checkPcmSettings overrides the implementation from the base class AFM_PcmPort.
        * It allowes upmixing or downmixing in the OMX IL Component which it is not possible
        * in the base class for setting an i/o PCM input port.
        */
       virtual OMX_ERRORTYPE checkPcmSettings(const OMX_AUDIO_PARAM_PCMMODETYPE &pcmSettings) const ;
};

#endif /* _VIRTUAL_SURROUND_PCM_PORT_H_ */
