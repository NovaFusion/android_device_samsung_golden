/*****************************************************************************/
/**
*  © ST-Ericsson, 2010 - All rights reserved
*
*  Reproduction and Communication of this document is strictly prohibited
*  unless specifically authorized in writing by ST-Ericsson.
*
* \brief   Virtual Surround Extension
* \author  ST-Ericsson
*/
/*****************************************************************************/
#ifndef _OMX_STE_VIRTUAL_SURROUND_EXTENSION_H_
#define _OMX_STE_VIRTUAL_SURROUND_EXTENSION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "AFM_Index.h"
#include <OMX_Types.h>

/**
 * Virtual Surround specific config index
 */

typedef enum OMX_STE_VIRTUALSURROUND_INDEXTYPE
{
   OMX_STE_IndexStartVirtualSurroundExtensions = AFM_IndexStartComponentSpecificExtensions + 589,
   OMX_STE_IndexConfigAudioVirtualSurround,                     /**< reference: OMX_STE_AUDIO_CONFIG_VIRTUALSURROUNDTYPE */
   OMX_STE_IndexEndVirtualSurroundExtensions

}OMX_STE_VIRTUALSURROUND_INDEXTYPE;


/** The virtual surround effect
 *  Virtual Surround is multichannel to binaural converter with the purpose to position and
 *  enhance the multichannel audio in such a way that the user listening (through headphones)
 *  experiences an externalization and a spatial localization similar to listening to a 5.1 
 *  surround system.
 *  \ingroup effects
 */
typedef enum OMX_STE_AUDIO_VIRTUALSURROUNDMODETYPE
{
    OMX_STE_AUDIO_VirtualSurroundStandardDownmix = 1,          /**< Matrix Downmix without left and right surround mixing */
    OMX_STE_AUDIO_VirtualSurroundSurroundDownmix,              /**< Matrix Downmix with left and right surround mixing */
    OMX_STE_AUDIO_VirtualSurroundSurroundVirtualization        /**< 5.1 channel virtualization */ 
} OMX_STE_AUDIO_VIRTUALSURROUNDMODETYPE; 



typedef enum OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONMODETYPE
{
    OMX_STE_AUDIO_VirtualSurroundRoomSimulationMixed = 1,      /**< Room Simulation based on non-processed front/surround audio signals */
    OMX_STE_AUDIO_VirtualSurroundRoomSimulationVirtualized,    /**< Room Simulation based on virtualized front/surround audio signals*/
} OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONMODETYPE;



typedef enum OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONROOMTYPE
{  
    OMX_STE_AUDIO_VirtualSurroundRoomDefault = 1,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeRoom,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeLivingRoom,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeAuditorium,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeConcertHall,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeArena,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeSmallRoom,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeMediumRoom,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeLargeRoom,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeMediumHall,
    OMX_STE_AUDIO_VirtualSurroundRoomTypeLargeHall
} OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONROOMTYPE;



typedef struct OMX_STE_AUDIO_CONFIG_VIRTUALSURROUNDTYPE
{
    OMX_U32 nSize;                                                                   /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;                                                        /**< OMX specification version information */
    OMX_U32 nPortIndex;                                                              /**< Port that this structure applies to */
    OMX_BOOL bEnable;                                                                /**< Enable/disable for Virtual Surround control */
    OMX_STE_AUDIO_VIRTUALSURROUNDMODETYPE eVirtualSurroundMode;                      /**< Virtual Surround algorithm type */
    OMX_BOOL bRoomSimulationFrontEnable;                                             /**< Enable/disable room simulation processing for
                                                                                          the front speaker pair (LF and RF) */
    OMX_BOOL bRoomSimulationSurroundEnable;                                          /**< Enable/disable room simulation processing for
                                                                                          the surround speaker pair (LS and RS) */
    OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONMODETYPE eRoomSimulationMode;         /**< Select the room simulation mode */
    OMX_STE_AUDIO_VIRTUALSURROUNDROOMSIMULATIONROOMTYPE eRoomSimulationRoomType;     /**< Select the room type where audio is virtualized */
    OMX_BS32 nVirtualSurroundLeftFrontGain;                                          /**< Left front gain value in mB (1/100 dB) */
    OMX_BS32 nVirtualSurroundRightFrontGain;                                         /**< Right front gain value in mB (1/100 dB) */
    OMX_BS32 nVirtualSurroundCenterFrontGain;                                        /**< Center front gain value in mB (1/100 dB) */
    OMX_BS32 nVirtualSurroundLeftSurroundGain;                                       /**< Left surround gain value in mB (1/100 dB) */
    OMX_BS32 nVirtualSurroundRightSurroundGain;                                      /**< Right surround gain value in mB (1/100 dB) */
    OMX_BS32 nVirtualSurroundLowFrequencyEffectsGain;                                /**< LFE gain value in mB (1/100 dB) */
    OMX_BS32 nVirtualSurroundFrontRoomSimulationGain;                                /**< Front room gain value in mB (1/100 dB) */
    OMX_BS32 nVirtualSurroundSurroundRoomSimulationGain;                             /**< Surroud room gain value in mB (1/100 dB) */
} OMX_STE_AUDIO_CONFIG_VIRTUALSURROUNDTYPE;


#ifdef __cplusplus
}
#endif

#endif /* _OMX_STE_VIRTUAL_SURROUND_EXTENSION_H_ */
