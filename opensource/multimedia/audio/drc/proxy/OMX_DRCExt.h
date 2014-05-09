/*****************************************************************************/
/**
*  © ST-Ericsson, 2010 - All rights reserved

*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \brief   DRC Extension
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _DRC_OMX_DRCEXTENSION_H_
#define _DRC_OMX_DRCEXTENSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "AFM_Index.h"
#include <OMX_Types.h>

/**
 * DRC specific config and param indexes
 */

typedef enum OMX_DRC_INDEXTYPE
{
   OMX_IndexStartDRCExtensions = AFM_IndexStartComponentSpecificExtensions + 579,
   OMX_IndexParamAudioDRC,                               /**< reference: OMX_AUDIO_PARAM_DRCTYPE */
   OMX_IndexConfigAudioDRC,                              /**< reference: OMX_AUDIO_CONFIG_DRCTYPE */
   OMX_IndexEndDRCExtensions

}OMX_DRC_INDEXTYPE;


/** The dynamic range control (DRC) effect
 *  DRC varies the output level of a signal, based on its input level, in a non-linear fashion.
 *  It can gate, expand, compress, and limit the dynamic range of an audio signal. 
 *  Typically it is used to increase the perceived loudness of a signal. 
 *  @ingroup effects
 */
typedef enum OMX_AUDIO_DRCACCEPTANCELEVELTYPE
{
   OMX_AUDIO_DRCAcceptanceLevel90Percent = 1,            /**< 90 percent of the desired gain must be reached
                                                              within the attack or release time  */
   OMX_AUDIO_DRCAcceptanceLevel99Percent,                /**< 99 percent of the desired gain must be reached
                                                              within the attack or release time */
   OMX_AUDIO_DRCAcceptanceLevel999Percent                /**< 99.9 percent of the desired gain must be reached
                                                              within the attack or release time */
}OMX_AUDIO_DRCACCEPTANCELEVELTYPE;


typedef struct OMX_AUDIO_CONFIG_DRCTYPE
{
   OMX_U32   nSize;                                      /**< size of the structure in bytes */
   OMX_VERSIONTYPE nVersion;                             /**< OMX specification version information */
   OMX_U32   nPortIndex;                                 /**< Port index indicating which port to set */
   OMX_BOOL  bEnable;                                    /**< Enable/disable for DRC */
   OMX_BOOL  bUseSeparateChannelSettings;                /**< Enable/disable separate channel settings */
   OMX_BU32  sLookAheadTime;                             /**< Look Ahead Time, in ms expressed in Q14.1*/

   OMX_BOOL  bUsePeakMeasure;                            /**< If true, Peak Detector will be used by the algorithm.
                                                              Otherwise, Root Mean Square (RMS) value is used. RMS
                                                              is recommended */
   OMX_BS32  sStaticCurvePointX[8];                      /**< Point x in Q7.8. Range [-128..0] dB  */
   OMX_BS32  sStaticCurvePointY[8];                      /**< Point y in Q7.8. Range [-128..0] dB. y may not introduce a
                                                              gain outside the range [-48..48] dB (include nOffsetGain) */
   OMX_BS32  sInputGain;                                 /**< Input gain to the Level Detector, in dB expressed in Q7.8,
                                                              range[-128..128] dB */
   OMX_BS32  sOffsetGain;                                /**< Offset to the gain calculated Gain Processor, in dB expressed
                                                              in Q7.8, range [-48..48] dB including the gain introduced by
                                                              the Static Curve, is increased by this offset gain */
   OMX_BU32  sLevelDetectorAttackTime;                   /**< Attack time for the Level Detector, in ms
                                                              expressed in Q14.1, range[0..16000] ms */
   OMX_BU32  sLevelDetectorReleaseTime;                  /**< Release time for the Level Detector, in ms
                                                              expressed in Q14.1, range[0..16000] ms */
   OMX_BU32  sGainProcessorAttackTime;                   /**< Attack time for the Gain Processor, in ms
                                                              expressed in Q14.1 range[0..16000] ms */
   OMX_BU32  sGainProcessorReleaseTime;                  /**< Release time for Gain Processor, in ms
                                                              expressed in Q14.1 range[0..16000] ms */
   OMX_AUDIO_DRCACCEPTANCELEVELTYPE  eAcceptanceLevel;   /**< This value tells how much of the specified gain that must
                                                              have been reached before the attack or release time elapsed.
                                                              OMX_Audio_DRCAcceptanceLevel90Percent is the recommended seting*/


   OMX_BOOL  bUsePeakMeasureChannel2;                            /**< If true, Peak Detector will be used by the algorithm.
                                                                      Otherwise, Root Mean Square (RMS) value is used. RMS
                                                                      is recommended */
   OMX_BS32  sStaticCurvePointXChannel2[8];                      /**< Point x in Q7.8. Range [-128..0] dB  */
   OMX_BS32  sStaticCurvePointYChannel2[8];                      /**< Point y in Q7.8. Range [-128..0] dB. y may not introduce a
                                                                      gain outside the range [-48..48] dB (include nOffsetGain) */
   OMX_BS32  sInputGainChannel2;                                 /**< Input gain to the Level Detector, in dB expressed in Q7.8,
                                                                      range[-128..128] dB */
   OMX_BS32  sOffsetGainChannel2;                                /**< Offset to the gain calculated Gain Processor, in dB expressed
                                                                      in Q7.8, range [-48..48] dB including the gain introduced by
                                                                      the Static Curve, is increased by this offset gain */
   OMX_BU32  sLevelDetectorAttackTimeChannel2;                   /**< Attack time for the Level Detector, in ms
                                                                      expressed in Q14.1, range[0..16000] ms */
   OMX_BU32  sLevelDetectorReleaseTimeChannel2;                  /**< Release time for the Level Detector, in ms
                                                                      expressed in Q14.1, range[0..16000] ms */
   OMX_BU32  sGainProcessorAttackTimeChannel2;                   /**< Attack time for the Gain Processor, in ms
                                                                      expressed in Q14.1 range[0..16000] ms */
   OMX_BU32  sGainProcessorReleaseTimeChannel2;                  /**< Release time for Gain Processor, in ms
                                                                      expressed in Q14.1 range[0..16000] ms */
   OMX_AUDIO_DRCACCEPTANCELEVELTYPE  eAcceptanceLevelChannel2;   /**< This value tells how much of the specified gain that must
                                                                      have been reached before the attack or release time elapsed.
                                                                      OMX_Audio_DRCAcceptanceLevel90Percent is the recommended seting*/
}OMX_AUDIO_CONFIG_DRCTYPE;


#ifdef __cplusplus
}
#endif

#endif /* _DRC_OMX_DRCEXTENSION_H_ */
