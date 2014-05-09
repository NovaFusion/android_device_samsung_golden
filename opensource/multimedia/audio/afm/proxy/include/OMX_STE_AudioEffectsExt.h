/*****************************************************************************/
/**
*  © ST-Ericsson, 2011 - All rights reserved

*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson
*
* \brief   Mixer/Splitter extension
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _OMX_STE_AUDIOEFFECTSEXT_H_
#define _OMX_STE_AUDIOEFFECTSEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "AFM_Index.h"
#include "AFM_Types.h"
#include "OMX_Symbian_AudioExt_Ste.h"
#include "audio_transducer_chipset_api.h"

/**
 * Specific config indexes for Audio Effects integrated in Mixer/Splitter OMX components
 */

typedef enum OMX_STE_AUDIOEFFECTS_INDEXTYPE
{
    OMX_IndexConfigAudioEffectsFirstIndex = AFM_IndexStartComponentSpecificExtensions + 850,
    OMX_IndexConfigAudioEffectAddPosition,              /**<  reference: OMX_AUDIO_CONFIG_EFFECTADDPOSITIONTYPE             */
    OMX_IndexConfigAudioEffectRemovePosition,           /**<  reference: OMX_AUDIO_CONFIG_EFFECTREMOVEPOSITIONTYPE          */
    OMX_IndexConfigAudioEffectCommit,                   /**<  reference: OMX_AUDIO_CONFIG_EFFECTCOMMITTYPE                  */
    OMX_IndexConfigAudioEffectConfigure,                /**<  reference: OMX_AUDIO_CONFIG_EFFECTCONFIGURETYPE               */
}OMX_STE_AUDIOEFFECTS_INDEXTYPE;

typedef struct OMX_AUDIO_CONFIG_EFFECTADDPOSITIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nPositionIndex;     /*effect's position in the port (incremented in same direction of data flow) */
    OMX_STRING cEffectName;     /*name of the effect to be added */
} OMX_AUDIO_CONFIG_EFFECTADDPOSITIONTYPE;

typedef struct OMX_AUDIO_CONFIG_EFFECTREMOVEPOSITIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nPositionIndex;     /*effect's position in the port (incremented in same direction of data flow) */
} OMX_AUDIO_CONFIG_EFFECTREMOVEPOSITIONTYPE;

typedef struct OMX_AUDIO_CONFIG_EFFECTCOMMITTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
} OMX_AUDIO_CONFIG_EFFECTCOMMITTYPE;

typedef struct OMX_AUDIO_CONFIG_EFFECTPOSITIONTYPE {
    OMX_U32         nPortIndex;             /**< index of the port of OMX mixer/splitter where the effect has been added */
    OMX_U32         nPositionIndex;         /**< position of the effect inside the port */
    OMX_INDEXTYPE   nEffectSpecificIndex;   /**< index associated with the specific effect configuration structure*/
} OMX_AUDIO_CONFIG_EFFECTPOSITIONTYPE;

typedef struct OMX_AUDIO_CONFIG_EFFECTCONFIGURETYPE {
    OMX_U32                             nSize;      /**< size of the structure in bytes */
    OMX_VERSIONTYPE                     nVersion;   /**< OMX specification version information */
    OMX_AUDIO_CONFIG_EFFECTPOSITIONTYPE base;
    union {
        AFM_AUDIO_CONFIG_PCM_PROBE                  probe;              /**< for pcmprobe configuration */
        OMX_AUDIO_CONFIG_VOLUMETYPE                 volume;             /**< volume configuration (supported by spl effect) */
        AFM_AUDIO_PARAM_SPLTYPE                     spl;                /**< spl params */
        OMX_AUDIO_CONFIG_SPLLIMITTYPE               splLimit;           /**< spl configuration */
        AFM_AUDIO_PARAM_TRANSDUCEREQUALIZERTYPE     teq;                /**< teq params */
        AFM_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE    teqConfig;          /**< teq configuration */
        AFM_AUDIO_PARAM_MDRC                        mdrc;               /**< mdrc params */
        AFM_AUDIO_GLOBAL_CONFIG_MDRC                mdrcGlobalConfig;   /**< mdrc global configuration */
        OMX_SYMBIAN_AUDIO_CONFIG_MDRCTYPE           mdrcConfig;         /**< mdrc configuration */
    } sConfig;
} OMX_AUDIO_CONFIG_EFFECTCONFIGURETYPE;

#ifdef __cplusplus
}
#endif

#endif /* _OMX_STE_AUDIOEFFECTSEXT_H_ */
