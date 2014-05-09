/*****************************************************************************/
/**
*  © ST-Ericsson, 2011 - All rights reserved

*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \brief   CNG Extension
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _OMX_CNG_CNGEXTENSION_H_
#define _OMX_CNG_CNGEXTENSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "AFM_Index.h"
#include <OMX_Types.h>

/**
 * Comfort Noise Generaition specific config index
 */

typedef enum OMX_COMFORT_NOISE_GENERATION_INDEXTYPE
{
    OMX_IndexConfigAudioComfortNoiseFirstIndex = AFM_IndexStartComponentSpecificExtensions + 823,
    OMX_IndexConfigAudioComfortNoise,                          /**< reference: OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE */
    OMX_IndexConfigAudioComfortNoiseEndIndex

}OMX_COMFORT_NOISE_GENERATION_INDEXTYPE;


/** The comfort noise generation effect
 *  
 *  @ingroup effects
 */
typedef struct OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE
{
    OMX_U32 nSize;                    /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;         /**< OMX specification version information */
    OMX_U32  nPortIndex;              /**< Port index indicating which port to set */
    OMX_BOOL bEnable;                 /**< Enable/disable Comfort Noise Generation algorithm */
    OMX_BS32 sLevel;                  /**< Noise Generation RMS power level, in dBovl, range [-90..-70]dBovl */

} OMX_AUDIO_CONFIG_COMFORTNOISEGENERATIONTYPE;


#ifdef __cplusplus
}
#endif

#endif /* _OMX_CNG_CNGEXTENSION_H_ */
