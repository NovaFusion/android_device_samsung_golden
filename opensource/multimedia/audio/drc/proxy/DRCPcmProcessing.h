/*****************************************************************************/
/**
*  © ST-Ericsson, 2010 - All rights reserved

*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \brief   DRC pcm processor proxy
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _DRC_PCM_PROCESSING_H_
#define _DRC_PCM_PROCESSING_H_

#include "OMX_DRCExt.h"
#include "AFM_PcmProcessing.h"


#ifdef AFM_DRC_DEBUG
  #include <stdio.h>
  #define AFM_PRINT        printf("[AFM_DRC] "); printf
#else
  #define AFM_PRINT(x,...)
#endif


#define AFM_DRC_LOOK_AHEAD_TIME_MAX            2000
#define AFM_DRC_STATIC_CURVE_POINT_MAX         0
#define AFM_DRC_STATIC_CURVE_POINT_MIN        (-128 << 8)
#define AFM_DRC_ATTACK_RELEASE_TIME_MAX       (16000 << 1)
#define AFM_DRC_LEVEL_DETECTOR_TIME_DEFAULT    16
#define AFM_DRC_GAIN_PROCESSOR_TIME_DEFAULT    400
#define AFM_DRC_NUMBER_OF_STATIC_CURVE_POINTS  8


/**
 * \brief Factory method for the DRC ENS component
 *
 * This method creates a DRC ENS component.
 * It is used by the ENS Core to create a DRC OMX IL component.
 * Register this method to ENS Core will enable DRC OMX IL support.
 */
OMX_ERRORTYPE drcPcmProcFactoryMethod(ENS_Component_p* ppENSComponent);


/**
 * \brief Class that implements the generic part of the DRC ENS/AFM component
 */
class DRCPcmProcessing : public AFM_PcmProcessing
{
   public:
       OMX_ERRORTYPE construct(void);
       virtual ~DRCPcmProcessing() {}

       /**
        * \brief Get config struct for DRC.
        * getConfig as specified in the OMX IL spec.
        * Index OMX_IndexConfigAudioDRC maps to the \a OMX_AUDIO_CONFIG_DRCTYPE struct.
        */
       virtual OMX_ERRORTYPE getConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure) const;

       /**
        * \brief Set config struct for DRC.
        * setConfig as specified in the OMX IL spec.
        * Index OMX_IndexConfigAudioDRC maps to the \a OMX_AUDIO_CONFIG_DRCTYPE struct.
        */
       virtual OMX_ERRORTYPE setConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure);

       /** Returns sample bit size. DRC only supports 16bit samples. */
       virtual OMX_U32 getSampleBitSize(void) const {return 16;}

       OMX_BOOL getEnable(void) const {return mEnable;}
       OMX_BOOL getUseSeparateChannelSettings(void) const {return mUseSeparateChannelSettings;}
       OMX_U16  getLookAheadTime(void) const {return mLookAheadTime;}

       OMX_BOOL getUsePeakMeasure(void) const {return mUsePeakMeasure;}
       OMX_S16 getInputGain(void) const {return mInputGain;}
       OMX_S16 getOffsetGain(void) const {return mOffsetGain;}
       OMX_S16 getStaticCurvePointX(OMX_U32 nPointIndex) const {return mStaticCurvePointX[nPointIndex];}
       OMX_S16 getStaticCurvePointY(OMX_U32 nPointIndex) const {return mStaticCurvePointY[nPointIndex];}

       OMX_U16 getAcceptanceLevel(void) const {return mAcceptanceLevel;}
       OMX_U16 getLevelDetectorAttackTime(void) const {return mLevelDetectorAttackTime;}
       OMX_U16 getGainProcessorAttackTime(void) const {return mGainProcessorAttackTime;}
       OMX_U16 getLevelDetectorReleaseTime(void) const {return mLevelDetectorReleaseTime;}
       OMX_U16 getGainProcessorReleaseTime(void) const {return mGainProcessorReleaseTime;}

       OMX_BOOL getUsePeakMeasureChannel2(void) const {return mUsePeakMeasureChannel2;}
       OMX_S16 getInputGainChannel2(void) const {return mInputGainChannel2;}
       OMX_S16 getOffsetGainChannel2(void) const {return mOffsetGainChannel2;}
       OMX_S16 getStaticCurvePointXChannel2(OMX_U32 nPointIndex) const {return mStaticCurvePointXChannel2[nPointIndex];}
       OMX_S16 getStaticCurvePointYChannel2(OMX_U32 nPointIndex) const {return mStaticCurvePointYChannel2[nPointIndex];}


       OMX_U16 getAcceptanceLevelChannel2(void) const {return mAcceptanceLevelChannel2;}
       OMX_U16 getLevelDetectorAttackTimeChannel2(void) const {return mLevelDetectorAttackTimeChannel2;}
       OMX_U16 getGainProcessorAttackTimeChannel2(void) const {return mGainProcessorAttackTimeChannel2;}
       OMX_U16 getLevelDetectorReleaseTimeChannel2(void) const {return mLevelDetectorReleaseTimeChannel2;}
       OMX_U16 getGainProcessorReleaseTimeChannel2(void) const {return mGainProcessorReleaseTimeChannel2;}


   private:
       OMX_BOOL mEnable;
       OMX_BOOL mUseSeparateChannelSettings;
       OMX_U16  mLookAheadTime;

       OMX_BOOL mUsePeakMeasure;
       OMX_S16  mStaticCurvePointX[AFM_DRC_NUMBER_OF_STATIC_CURVE_POINTS];
       OMX_S16  mStaticCurvePointY[AFM_DRC_NUMBER_OF_STATIC_CURVE_POINTS];
       OMX_S16  mInputGain;
       OMX_S16  mOffsetGain;
       OMX_U16  mLevelDetectorAttackTime;
       OMX_U16  mLevelDetectorReleaseTime;
       OMX_U16  mGainProcessorAttackTime;
       OMX_U16  mGainProcessorReleaseTime;
       OMX_AUDIO_DRCACCEPTANCELEVELTYPE  mAcceptanceLevel;

       OMX_BOOL mUsePeakMeasureChannel2;
       OMX_S16  mStaticCurvePointXChannel2[AFM_DRC_NUMBER_OF_STATIC_CURVE_POINTS];
       OMX_S16  mStaticCurvePointYChannel2[AFM_DRC_NUMBER_OF_STATIC_CURVE_POINTS];
       OMX_S16  mInputGainChannel2;
       OMX_S16  mOffsetGainChannel2;
       OMX_U16  mLevelDetectorAttackTimeChannel2;
       OMX_U16  mLevelDetectorReleaseTimeChannel2;
       OMX_U16  mGainProcessorAttackTimeChannel2;
       OMX_U16  mGainProcessorReleaseTimeChannel2;
       OMX_AUDIO_DRCACCEPTANCELEVELTYPE  mAcceptanceLevelChannel2;
};

#endif /* _DRC_PCM_PROCESSING_H_ */
