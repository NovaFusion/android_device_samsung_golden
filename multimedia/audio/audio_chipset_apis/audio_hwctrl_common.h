/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   audio_hwctrl_common.h
 * \brief  Common Audio HW Ctrl API for all sources and sinks
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIO_HWCTRL_COMMON_H_
#define _AUDIO_HWCTRL_COMMON_H_

#include "OMX_Types.h"
#include "OMX_Core.h"
#include <audio_hw_control_chipset_api.h>

typedef void (*fn_ptr1)(AUDIO_COMMON_STATUS status);
typedef void (*fn_ptr2)(AUDIO_COMMON_STATUS status, uint32 extra_status_info);


typedef enum AUDIO_COMPONENT_TYPE
	{
	EARPIECE,// = 0,
	NOKIA_AV_SINK,// = 1,//HEADSET
	HAC,
	IHF,// = 2,
	VIBL,
	VIBR,
	NOKIA_AV_SOURCE,// = 5,//Mic1A
	MIC1B,
	MIC2,
	LIN,
	DMIC12,
	DMIC34,
	DMIC56,
	MULTI_MIC,// = 12,
	MULTI_MIC_EANC,
	FM_RX_SOURCE,
	FM_TX_SINK,
	MONO_BT_SOURCE,
	MONO_BT_SINK,
	A2DP_SOURCE,
	A2DP_SINK,
	USB_SOURCE,
	USB_SINK,
	HDMI_SINK,
	//BLUETOOTH_HDMI = 13//,
	//EAudioModem
	//EAccessory
	} AUDIO_COMPONENT_TYPE;


/* API functions */
/* Added APIs */
//OMX_ERRORTYPE audio_open_channel(int16 channel_type, void* ptr);
OMX_ERRORTYPE audio_open_channel(int16 channel_type);
void audio_close_channel(int16 channel_type);
/* HAL layer */

void audio_hal_mute_control(fn_ptr1 mute_fn,
                             uint32 channel_index,
                             AUDIO_COMMON_SWITCH mute_control, int16 channel_type);

AUDIO_COMMON_SWITCH audio_hal_mute_status(uint32 channel_index, int16 channel_type);

void audio_hal_power_control(fn_ptr1 power_fn,
                             uint32 channel_index, AUDIO_COMMON_SWITCH power_control, int16 channel_type);

AUDIO_COMMON_SWITCH audio_hal_power_status(uint32 channel_index, int16 channel_type);

void audio_hal_gain_capabilities_query(uint32* num_channels, uint16* max_num_gains, int16 channel_type);

void audio_hal_gains_descriptor_query(AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table, int16 channel_type);

void audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains, int16 channel_type);

void audio_hal_supported_loops_query(uint16* supported_loop_indexes, int16 channel_type);

void audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table, int16 channel_type);

AUDIO_COMMON_STATUS audio_hal_gain_control(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32 gain_value, int16 channel_type);

AUDIO_COMMON_STATUS audio_hal_gain_query(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32* gain_value, int16 channel_type);

AUDIO_COMMON_STATUS audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value, int16 channel_type);


uint16 audio_hal_registerbank_query(void);

void audio_hal_regbank_descriptors_query(AUDIO_HAL_REGBANK_DESCRIPTOR* register_banks);

void audio_hal_custom_hw_control(fn_ptr2 customfn,
                                 AUDIO_HAL_HW_OPERATION hw_operation, /*OMX_AUDIO_REGBANKDESCRIPTORTYPE*/AUDIO_HAL_HW_REGISTER_TYPE register_type,
                                 uint16 num_data, uint32* address_data_sequence, int16 channel_type);


/* Sequencer layer */
void audio_sequencer_mute_control(fn_ptr1 seq_mutefn,
                                  uint32 channel_index, AUDIO_COMMON_SWITCH mute_control, int16 channel_type);

void audio_sequencer_power_control(fn_ptr1 seq_powerfn,
                                  uint32 channel_index, AUDIO_COMMON_SWITCH power_control, int16 channel_type);

AUDIO_COMMON_STATUS audio_sequencer_burst_control(fn_ptr1 seq_burstfn,
                                                  AUDIO_COMMON_SWITCH burst_control, int16 channel_type);

void audio_sequencer_volume_control(fn_ptr1 seq_volumefn,
                                     uint32 channel_index, uint32 linear, int32 volume, int16 channel_type);

void audio_sequencer_volume_query(uint32 channel_index, uint32 linear, int32* min_volume,
                                   int32* max_volume, int32* current_volume, int16 channel_type);

/* Timing critical layer */
void audio_timed_mute_control(fn_ptr1 timed_mutefn,
                              uint32 channel_index, AUDIO_COMMON_SWITCH mute_control, int16 channel_type);

void audio_timed_volume_ramp_control(fn_ptr1 timed_ramp,
                             uint32 channel_index, uint32 linear, int32 start_volume,
                             int32 end_volume, uint32 ramp_duration_hi, uint32 ram_duration_lo,
                             AUDIO_COMMON_SWITCH ramp_terminate, int16 channel_type);

void audio_timed_volume_ramp_query(uint32 channel_index, AUDIO_TIMED_RAMP_INFO* volume_ramp_info, int16 channel_type);

void Enable_BT(int16 channel_type);
void Disable_BT(int16 channel_type);

void audio_hal_loop_SideTone_FIRFilterCoefficients(uint32 FIRAddress,uint32* FIRCoefficient,int16 channel_type);

#endif // _AUDIO_HWCTRL_COMMON_H_
