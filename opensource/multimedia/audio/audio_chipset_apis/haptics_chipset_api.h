/* ---------------------------------------------------------------------------*
 *                                                                            *
 *----------------------------------------------------------------------------*
 * MODULE NAME                                                                *
 *----------------------------------------------------------------------------*
 * Haptics Chipset API                                                        *
 *                                                                            *
 *----------------------------------------------------------------------------*
 * DETAILED DESCRIPTION                                                       *
 * ---------------------------------------------------------------------------*
 * Haptics API header file. This file in addition with standard OpenMAX IL    *
 * 1.1.2 header files constitute complete header files for Nokia Haptics      *
 * Chipset API.                                                               *
 *																			  *
 * Copyright (c) 2007 Nokia                                                   *
 *                                                                            *
 *                                                                            *
 * Version History:                                                           *
 * ---------------------------------------------------------------------------*
 *   Date                Author                Description                    *
 *   ----                ------                -----------                    *
 *																			  *
 *----------------------------------------------------------------------------*/

#ifndef HAPTICS_CHIPSET_API_H
#define HAPTICS_CHIPSET_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully
 */

#include <OMX_Audio.h>

#include <audio_hw_control_chipset_api.h>

    typedef struct OMX_AUDIO_PARAM_HAPTICSDRIVERPROPERTIESTYPE
    {
        OMX_U32 nSize;
        OMX_VERSIONTYPE nVersion;
        OMX_U32 nPortIndex;
        OMX_U32 nNativeSamplingRate;
        OMX_U32 nHapticsDriverNominalVoltage;
        OMX_U32 nHapticsDriverImpedance;
    } OMX_AUDIO_PARAM_HAPTICSDRIVERPROPERTIESTYPE;


//---------------------------------------------------------------------------
//          Vibra 1
//---------------------------------------------------------------------------
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_vibra1_audio_open_channel (void);

AUDIO_HWCTRL_API void          stericsson_vibra1_audio_close_channel(void);

AUDIO_HWCTRL_API void          stericsson_vibra1_audio_hal_power_control( 
                                void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
                                uint32 channel_index,
                                AUDIO_COMMON_SWITCH power_control
                                );

AUDIO_HWCTRL_API void         stericsson_vibra1_audio_sequencer_power_control( 
                                void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
                                uint32 channel_index,
                                AUDIO_COMMON_SWITCH power_control
                                );

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_vibra1_audio_hal_gain_control(
                                uint32 channel_index,
                                uint16 gain_index,
                                uint32 linear,
                                int32 gain_value
                                );

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_vibra1_audio_hal_power_status(uint32 channel_index);

//---------------------------------------------------------------------------
//          Vibra 2
//---------------------------------------------------------------------------
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_vibra2_audio_open_channel (void);

AUDIO_HWCTRL_API void          stericsson_vibra2_audio_close_channel(void);

AUDIO_HWCTRL_API void          stericsson_vibra2_audio_hal_power_control( 
                                void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
                                uint32 channel_index,
                                AUDIO_COMMON_SWITCH power_control
                                );

AUDIO_HWCTRL_API void          stericsson_vibra2_audio_sequencer_power_control( 
                                void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
                                uint32 channel_index,
                                AUDIO_COMMON_SWITCH power_control
                                );

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_vibra2_audio_hal_gain_control(
                                uint32 channel_index,
                                uint16 gain_index,
                                uint32 linear,
                                int32 gain_value
                                );

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_vibra2_audio_hal_power_status(uint32 channel_index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

