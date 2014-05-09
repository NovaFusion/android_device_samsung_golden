/*****************************************************************************/
/**
*  © ST-Ericsson, 2011 - All rights reserved

*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \file    Cng.h
* \brief   Comfort Noise Generation pcm processor proxy
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _CNG_H_
#define _CNG_H_

#include "OMX_CNGExt.h"
#include "AFM_PcmProcessing.h"

/// @defgroup cng
/// @ingroup  afmpcmprocessing
/// @{


#ifdef AFM_CNG_DEBUG
  #include <stdio.h>
  #define AFM_PRINT        printf("[AFM_CNG] "); printf
#else
  #define AFM_PRINT(x,...)
#endif

#define AFM_CNG_LEVEL_MAX     -50
#define AFM_CNG_LEVEL_MIN     -91


/**
 * \brief Factory method for the Comfort Noise Generaation ENS component
 *
 * This method creates a Comfort Noise Generation ENS component.
 * It is used by the ENS Core to create a Comfort Noise Generation OMX IL component.
 * Register this method to ENS Core will enable CNG OMX IL support.
 */
OMX_ERRORTYPE comfortnoisePcmProcFactoryMethod(ENS_Component_p* ppENSComponent);


/**
 * \brief Class that implements the generic part of the Comfort Noise Generation ENS/AFM component
 */
class CNGPcmProcessing : public AFM_PcmProcessing
{
    public:
        OMX_ERRORTYPE construct(void);
        virtual ~CNGPcmProcessing() {}

        /**
         * \brief Get config struct for Comfort Noise Generation.
         * getConfig as specified in the OMX IL spec.
         * Index OMX_IndexConfigAudioComfortNoise maps to the OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE struct.
         */
        virtual OMX_ERRORTYPE getConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure) const;

        /**
         * \brief Set config struct for Comfort Noise Generation.
         * setConfig as specified in the OMX IL spec.
         * Index OMX_IndexConfigAudioCNG maps to the OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE struct.
         */
        virtual OMX_ERRORTYPE setConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure);

        /** getExtensionIndex for Comfort Noise Generation. Set configuration index for Audio Effect */
        virtual OMX_ERRORTYPE getExtensionIndex(OMX_STRING cParameterName, OMX_INDEXTYPE* pIndexType) const;


        OMX_BOOL getEnable(void) const {return mEnable;}
        OMX_S16  getLevel(void) const {return mLevel;}

    private:
        OMX_BOOL mEnable;
        OMX_S16  mLevel;

};

/// @}

#endif /* _COMFORT_NOISE_H_ */
