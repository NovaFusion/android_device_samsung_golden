/*****************************************************************************/
/**
*  © ST-Ericsson, 2010 - All rights reserved
*
*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson.
*
* \brief   Virtual Surround PCM processor proxy
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _VIRTUAL_SURROUND_PCM_PROCESSING_H_
#define _VIRTUAL_SURROUND_PCM_PROCESSING_H_

#include "AFM_PcmProcessing.h"
#include "OMX_STE_VirtualSurroundExt.h"


#ifdef AFM_VIRTUAL_SURROUND_DEBUG
  #include <stdio.h>
  #define AFM_PRINT        printf("[AFM_VSURROUND] "); printf
#else
  #define AFM_PRINT(x,...)
#endif


#define AFM_VIRTUAL_SURROUND_MIN_GAIN     -7800
#define AFM_VIRTUAL_SURROUND_MAX_GAIN      1200
#define AFM_VIRTUAL_SURROUND_DEFAULT_GAIN  0


/**
 * \brief Factory method for the Virtual Surround ENS component
 *
 * This method creates a Virtual Surround ENS component.
 * It is used by the ENS Core to create a Virtual Surround OMX IL component.
 * Register this method to ENS Core will enable Virtual Surround OMX IL support.
 */
OMX_ERRORTYPE virtualsurroundPcmProcFactoryMethod(ENS_Component_p* ppENSComponent);


/**
 * \brief Class that implements the generic part of the Virtual Surround ENS/AFM component
 */
class VirtualSurroundPcmProcessing : public AFM_PcmProcessing
{
   public:
       OMX_ERRORTYPE construct(void);
       virtual ~VirtualSurroundPcmProcessing() {}
	   	   
       /**
        * \brief Get config struct for Virtual Surround.
        * getConfig as specified in the OMX IL spec.
        * Index OMX_STE_IndexConfigAudioVirtualSurround maps to the OMX_AUDIO_CONFIG_VIRTUALSURROUNDTYPE struct.
        */
       virtual OMX_ERRORTYPE getConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure) const;
       
       /**
        * \brief Set config struct for Virtual Surround.
        * getConfig as specified in the OMX IL spec.
        * Index OMX_STE_IndexConfigAudioVirtualSurround maps to the OMX_AUDIO_CONFIG_VIRTUALSURROUNDTYPE struct.
        */
        virtual OMX_ERRORTYPE setConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure);

       /** \brief Returns sample bit size. Virtual Surround only supports 16bit samples. */
       virtual OMX_U32 getSampleBitSize(void) const {return 16;}

       OMX_BOOL getEnable(void) const {return mEnable;}
       OMX_BOOL getRoomSimulationFrontEnable(void) const {return mRoomSimulationFrontEnable;}
       OMX_BOOL getRoomSimulationSurroundEnable(void) const {return mRoomSimulationSurroundEnable;}
       OMX_U16  getVirtualSurroundMode(void) const {return mVirtualSurroundMode;}
       OMX_U16  getRoomSimulationMode(void) const {return mRoomSimulationMode;}
       OMX_U16  getRoomSimulationRoomType(void) const {return mRoomSimulationRoomType;}
       OMX_S32  getLetfFrontGain(void) const {return mLeftFrontGain;}
       OMX_S32  getRightFrontGain(void) const {return mRightFrontGain;}
       OMX_S32  getCenterFrontGain(void) const {return mCenterFrontGain;}
       OMX_S32  getLeftSurroundGain(void) const {return mLeftSurroundGain;}
       OMX_S32  getRightSurroundGain(void) const {return mRightSurroundGain;}
       OMX_S32  getLowFrequencyEffectsGain(void) const {return mLowFrequencyEffectsGain;}
       OMX_S32  getFrontRoomSimulationGain(void) const {return mFrontRoomSimulationGain;}
       OMX_S32  getSurroundRoomSimulationGain(void) const {return mSurroundRoomSimulationGain;}

   private:
       OMX_BOOL mEnable;
       OMX_BOOL mRoomSimulationFrontEnable;
       OMX_BOOL mRoomSimulationSurroundEnable;
       OMX_STE_AUDIO_VIRTUALSURROUNDMODETYPE mVirtualSurroundMode;
       OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONMODETYPE mRoomSimulationMode;
       OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONROOMTYPE mRoomSimulationRoomType;
       OMX_S32 mLeftFrontGain;
       OMX_S32 mRightFrontGain;
       OMX_S32 mCenterFrontGain;
       OMX_S32 mLeftSurroundGain;
       OMX_S32 mRightSurroundGain;
       OMX_S32 mLowFrequencyEffectsGain;
       OMX_S32 mFrontRoomSimulationGain;
       OMX_S32 mSurroundRoomSimulationGain;

};

#endif /* _VIRTUAL_SURROUND_PCM_PROCESSING_H_ */
