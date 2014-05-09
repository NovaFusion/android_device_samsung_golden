/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 *
 * 
 *
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   audio_hw_control_chipset_api.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
/*
  audio_hw_control_chipset_api.h

  Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
  All rights reserved.

  This program and the accompanying materials are made available 
  under the terms of the Eclipse Public License v1.0 which accompanies 
  this distribution, and is available at 
  http://www.eclipse.org/legal/epl-v10.html

  Initial Contributors:
  Nokia Corporation - initial contribution.

  Contributors:
*/
#ifndef AUDIO_HWCTRL_CHIPSET_API_H
#define AUDIO_HWCTRL_CHIPSET_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully 
 */

#include <OMX_Audio.h>

#ifdef __SYMBIAN32__
    #define AUDIO_HWCTRL_API IMPORT_C
#else
    #define AUDIO_HWCTRL_API
#endif

/* structures and enums used by OMX interface */
 typedef enum OMX_AUDIO_HWREGISTERTYPE { 
     OMX_AUDIO_IO = 0,
     OMX_AUDIO_I2C,
     OMX_AUDIO_SLIMBUS,
     OMX_AUDIO_CBUS = 0xFFFF     
 } OMX_AUDIO_HWREGISTERTYPE;

 /*
 typedef struct OMX_AUDIO_REGBANKDESCRIPTORTYPE { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nRegBankIndex;   
     OMX_U32 nLogicalStartAddr;
     OMX_U32 nLogicalEndAddr;
     OMX_AUDIO_HWREGISTERTYPE eRegBankType;
       
 } OMX_AUDIO_REGBANKDESCRIPTORTYPE;
*/

 typedef enum OMX_AUDIO_HWLOOPSTYPE {
  OMX_AUDIO_NOLOOP =                      0,
 OMX_AUDIO_SIDETONE_LOOP =               0x01,
 OMX_AUDIO_LOOP_MIC1B_TO_HFL =           0X02,
 OMX_AUDIO_LOOP_MIC1B_TO_HFR = 	         0x04,
 OMX_AUDIO_LOOP_MIC1B_TO_EAR =           0x08,
 OMX_AUDIO_LOOP_MIC1B_TO_HSL_HSR =       0x10,
 OMX_AUDIO_LOOP_MIC1A_TO_HSL=            0x20,
 OMX_AUDIO_LOOP_MIC1A_TO_HSR=            0X40,          
 OMX_AUDIO_LOOP_MIC1A_TO_HSR_HSL=        0X80,
 OMX_AUDIO_LOOP_LINEIN_TO_HSR_HSL=       0x100,
 OMX_AUDIO_LOOP_DMIC1_TO_HSL=            0x200,
 OMX_AUDIO_LOOP_DMIC1_TO_HSR=            0x400,
 OMX_AUDIO_LOOP_DMIC2_TO_HSL =           0x800,
 OMX_AUDIO_LOOP_DMIC2_TO_HSR =           0x1000,
 OMX_AUDIO_LOOP_DMIC12_TO_HSR_HSL =      0x2000,
 OMX_AUDIO_LOOP_DMIC3_TO_HSL =           0x4000,
 OMX_AUDIO_LOOP_DMIC3_TO_HSR =           0x8000,
 OMX_AUDIO_LOOP_DMIC4_TO_HSL =           0x10000,
 OMX_AUDIO_LOOP_DMIC4_TO_HSR =           0x20000,
 OMX_AUDIO_LOOP_DMIC34_TO_HSR_HSL =      0x40000,
 OMX_AUDIO_LOOP_DMIC5_TO_HSL =           0x80000,
 OMX_AUDIO_LOOP_DMIC5_TO_HSR =           0x100000,
 OMX_AUDIO_LOOP_DMIC6_TO_HSL =           0x200000,
 OMX_AUDIO_LOOP_DMIC6_TO_HSR =           0x400000,
 OMX_AUDIO_LOOP_DMIC56_TO_HSR_HSL =      0x800000,
 OMX_AUDIO_LOOP_FMRX_TO_HSR_HSL =        0x1000000
     /* rest of HW loops are to be defined on a vendor specific basis */
 } OMX_AUDIO_HWLOOPSTYPE;

 typedef enum OMX_AUDIO_HWOPERATIONTYPE { 
     OMX_AUDIO_NOOP = 0,
     OMX_AUDIO_16BIT_SINGLE_READ,
     OMX_AUDIO_16BIT_SINGLE_WRITE,
     OMX_AUDIO_32BIT_SINGLE_READ,
     OMX_AUDIO_32BIT_SINGLE_WRITE,
     OMX_AUDIO_16BIT_MULTI_READ,
     OMX_AUDIO_16BIT_MULTI_WRITE,
     OMX_AUDIO_32BIT_MULTI_READ,
     OMX_AUDIO_32BIT_MULTI_WRITE,
     OMX_AUDIO_16BIT_BURST_MULTI_READ,
     OMX_AUDIO_16BIT_BURST_MULTI_WRITE,
     OMX_AUDIO_32BIT_BURST_MULTI_READ,
     OMX_AUDIO_32BIT_BURST_MULTI_WRITE = 0xFFFF     
 } OMX_AUDIO_HWOPERATIONTYPE;

 
 typedef enum OMX_AUDIO_HWCTRL_STATUSTYPE { 
     OMX_AUDIO_OK = 0,
     OMX_AUDIO_FAIL,
     /* if callback facility is somehow solved in OpenMAX IL for getconfig/setconfig parameter calls, it is expected */
     /* to have here extra status information that can be returned for e.g. multi-read/write commands */
     OMX_AUDIO_NOT_SUPPORTED = 0xFFFF     
 } OMX_AUDIO_HWCTRL_STATUSTYPE;



/* Configuration parameters */

typedef struct OMX_AUDIO_CONFIG_POWERTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;            
     OMX_U32 nChannel;
     OMX_BOOL bPower;
} OMX_AUDIO_CONFIG_POWERTYPE;

/*
typedef struct OMX_AUDIO_CONFIG_VOLUMERAMPTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;            
     OMX_U32 nChannel;
     OMX_BOOL bLinear;
     OMX_BS32 sStartVolume;
     OMX_BS32 sEndVolume;
     OMX_TICKS nRampDuration;
     OMX_BOOL bRampTerminate;
     OMX_BS32 sCurrentVolume;
     OMX_TICKS nRampCurrentTime;
     OMX_TICKS nRampMinDuration;
     OMX_TICKS nRampMaxDuration;
     OMX_U32 nVolumeStep;
 } OMX_AUDIO_CONFIG_VOLUMERAMPTYPE;
*/

 typedef struct OMX_AUDIO_CONFIG_BURSTCONTROLTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;            
     OMX_BOOL bBurstControl;
     OMX_BOOL bStatus;
 } OMX_AUDIO_CONFIG_BURSTCONTROLTYPE;

 typedef struct OMX_AUDIO_CONFIG_REGISTERBANKQUERYTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;   
     OMX_U16 nNumRegisterBanks;           
 } OMX_AUDIO_CONFIG_REGISTERBANKQUERYTYPE;

/*
 typedef enum OMX_AUDIO_HWREGISTERTYPE { 
     OMX_AUDIO_IO = 0,
     OMX_AUDIO_I2C,
     OMX_AUDIO_SLIMBUS,
     OMX_AUDIO_CBUS = 0xFFFF     
 } OMX_AUDIO_HWREGISTERTYPE;
 */
/* 
 typedef struct OMX_AUDIO_REGBANKDESCRIPTORTYPE { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nRegBankIndex;   
     OMX_U32 nLogicalStartAddr;
     OMX_U32 nLogicalEndAddr;
     OMX_AUDIO_HWREGISTERTYPE eRegBankType;
       
 } OMX_AUDIO_REGBANKDESCRIPTORTYPE;
*/
/*
 typedef struct OMX_AUDIO_CONFIG_REGISTERBANKTABLETYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;   
     OMX_U16 nNumRegisterBanks; 
     OMX_AUDIO_REGBANKDESCRIPTORTYPE RegBankDescriptorTable;     
 } OMX_AUDIO_CONFIG_REGISTERBANKTABLETYPE;
*/

 typedef struct OMX_AUDIO_CONFIG_CUSTOMHWCONTROLTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;   
     OMX_AUDIO_HWCTRL_STATUSTYPE eStatus;
     OMX_AUDIO_HWOPERATIONTYPE eHwOperation;
     OMX_AUDIO_HWREGISTERTYPE eRegisterType;
     OMX_U16 nDataCount;
     OMX_U32 nLogicalAddress;
     OMX_U16 nData;     
 } OMX_AUDIO_CONFIG_CUSTOMHWCONTROLTYPE;

typedef struct OMX_AUDIO_CONFIG_HWLOOPSUPPORTTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;
     OMX_U32 nSupportedLoops;    
     OMX_AUDIO_HWLOOPSTYPE eLoopIndex;
         
 } OMX_AUDIO_CONFIG_HWLOOPSUPPORTTYPE;

typedef struct OMX_AUDIO_CONFIG_HWLOOPCONTROLTYPE  { 
     OMX_U32 nSize;                                    
     OMX_VERSIONTYPE nVersion;                   
     OMX_U32 nPortIndex;   
     OMX_AUDIO_HWLOOPSTYPE eLoopIndex;
     OMX_U32 nChannel;
     OMX_BOOL bControlSwitch;
     OMX_BOOL bLinear;
     OMX_BS32 sLoopVolume;
     
 } OMX_AUDIO_CONFIG_HWLOOPCONTROLTYPE;



/* End of OMX part */

/************  non-OMX interface definition **************/  

/* types and constants used by non-OMX APIs */

/* channel ID denoting special case of all channels being selected where possible */       
#define AUDIO_COMMON_CHANNEL_ALL	0x7FFFFFFF

typedef enum AUDIO_COMMON_SWITCH { 
     AUDIO_COMMON_OFF = 0,
     AUDIO_COMMON_ON,  
     AUDIO_COMMON_ALLCHANNEL_UNSUPPORTED = 0xFFFF     
 } AUDIO_COMMON_SWITCH;


typedef enum AUDIO_COMMON_STATUS { 
     AUDIO_COMMON_OK = 0,
     AUDIO_COMMON_FAIL,  
     AUDIO_COMMON_UNSUPPORTED = 0xFFFF     
} AUDIO_COMMON_STATUS;
        
typedef enum AUDIO_HAL_HW_LOOPS { 
AUDIO_NOLOOP                    =        0,
AUDIO_SIDETONE_LOOP             =        0x01,             
AUDIO_LOOP_MIC1B_TO_HFL         =        0X02,                 
AUDIO_LOOP_MIC1B_TO_HFR         = 		 0x04,    	                    
AUDIO_LOOP_MIC1B_TO_EAR         =        0x08,                
AUDIO_LOOP_MIC1B_TO_HSL_HSR     =        0x10,                 
AUDIO_LOOP_MIC1A_TO_HSL         =        0x20,                       
AUDIO_LOOP_MIC1A_TO_HSR         =        0X40,                     
AUDIO_LOOP_MIC1A_TO_HSR_HSL     =        0X80,
AUDIO_LOOP_LINEIN_TO_HSR_HSL    =        0x100,
AUDIO_LOOP_DMIC1_TO_HSL         =        0x200,
AUDIO_LOOP_DMIC1_TO_HSR         =        0x400,
AUDIO_LOOP_DMIC2_TO_HSL         =        0x800,   
AUDIO_LOOP_DMIC2_TO_HSR         =        0x1000,  
AUDIO_LOOP_DMIC12_TO_HSR_HSL    =        0x2000,  
AUDIO_LOOP_DMIC3_TO_HSL         =        0x4000,  
AUDIO_LOOP_DMIC3_TO_HSR         =        0x8000,  
AUDIO_LOOP_DMIC4_TO_HSL         =        0x10000, 
AUDIO_LOOP_DMIC4_TO_HSR         =        0x20000, 
AUDIO_LOOP_DMIC34_TO_HSR_HSL    =        0x40000, 
AUDIO_LOOP_DMIC5_TO_HSL         =        0x80000,
AUDIO_LOOP_DMIC5_TO_HSR         =        0x100000,
AUDIO_LOOP_DMIC6_TO_HSL         =        0x200000,
AUDIO_LOOP_DMIC6_TO_HSR         =        0x400000,
AUDIO_LOOP_DMIC56_TO_HSR_HSL    =        0x800000,
AUDIO_LOOP_FMRX_TO_HSR_HSL      =        0x1000000
     /* rest of HW loops are to be defined on a vendor specific basis */
} AUDIO_HAL_HW_LOOPS;
        
typedef enum AUDIO_HAL_HW_OPERATION{ 
     AUDIO_HAL_NOOP = 0,
     AUDIO_HAL_16BIT_SINGLE_READ,
     AUDIO_HAL_16BIT_SINGLE_WRITE,
     AUDIO_HAL_32BIT_SINGLE_READ,
     AUDIO_HAL_32BIT_SINGLE_WRITE,
     AUDIO_HAL_16BIT_MULTI_READ,
     AUDIO_HAL_16BIT_MULTI_WRITE,
     AUDIO_HAL_32BIT_MULTI_READ,
     AUDIO_HAL_32BIT_MULTI_WRITE,
     AUDIO_HAL_16BIT_BURST_MULTI_READ,
     AUDIO_HAL_16BIT_BURST_MULTI_WRITE,
     AUDIO_HAL_32BIT_BURST_MULTI_READ,
     AUDIO_HAL_32BIT_BURST_MULTI_WRITE = 0xFFFF     
} AUDIO_HAL_HW_OPERATION;

typedef enum AUDIO_HAL_HW_REGISTER_TYPE { 
     AUDIO_HAL_IO = 0,
     AUDIO_HAL_I2C,
     AUDIO_HAL_SLIMBUS,
     AUDIO_HAL_CBUS = 0xFFFF      
} AUDIO_HAL_HW_REGISTER_TYPE;


/** 16 bit unsigned quantity that is 16 bit word aligned */
typedef unsigned short uint16;

/** 16 bit signed quantity that is 16 bit word aligned */
typedef signed short int16;

/** 32 bit unsigned quantity that is 32 bit word aligned */
typedef unsigned long uint32;

/** signed quantity that is 32 bit word aligned */
typedef signed long int32;

typedef struct AUDIO_HAL_REGBANK_DESCRIPTOR{ 
     uint32 reg_bank_index;   
     uint32 logical_start_addr;
     uint32 logical_end_addr;
     AUDIO_HAL_HW_REGISTER_TYPE reg_bank_type;
} AUDIO_HAL_REGBANK_DESCRIPTOR;

typedef struct AUDIO_HAL_GAIN_DESCRIPTOR  { 
     uint32 channel_index; 
     uint16 gain_index;
     uint32 linear;
     int32 min_gain;
     int32 max_gain;
     uint32 gain_step;
     uint32 analog_gain;
     
} AUDIO_HAL_GAIN_DESCRIPTOR;

typedef struct AUDIO_TIMED_RAMP_INFO  { 

      uint32 linear;
      int32 ramp_start_volume;
      int32 ramp_end_volume;
      int32 current_volume;
      uint32 current_time_hi;
      uint32 current_time_lo;
      uint32 ramp_min_duration_hi;
      uint32 ramp_min_duration_lo;
      uint32 ramp_max_duration_hi;
      uint32 ramp_max_duration_lo;
      int32 min_volume;
      int32 max_volume;
      uint32 volume_step;
     
} AUDIO_TIMED_RAMP_INFO;


/* API functions */

AUDIO_HWCTRL_API void audio_start_fsbitclk(void);
AUDIO_HWCTRL_API void audio_stop_fsbitclk(void);
AUDIO_HWCTRL_API void audio_switch_to_burst(int32 frame_nbr);
AUDIO_HWCTRL_API void audio_switch_to_normal(void);

AUDIO_HWCTRL_API void audio_dumpregs(void);



/* The convention for the functions will be:       */
/* <supplier>_<component>_<API function name(...)>    */

/* Nokia AV Sink APIs */
//AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_nokiaavsink_audio_open_channel(void* ptr);
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_nokiaavsink_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_close_channel(void);

/* HAL layer */

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, 
                             AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_nokiaavsink_audio_hal_mute_status (uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, AUDIO_COMMON_SWITCH power_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_nokiaavsink_audio_hal_power_status(uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_gain_capabilities_query(uint32* num_channels, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_gains_descriptor_query(AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_supported_loops_query(uint16* supported_loop_indexes);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsink_audio_hal_gain_control(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32 gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsink_audio_hal_gain_query(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32* gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsink_audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value);

															             
AUDIO_HWCTRL_API uint16 stericsson_nokiaavsink_audio_hal_registerbank_query(void);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_regbank_descriptors_query(AUDIO_HAL_REGBANK_DESCRIPTOR* register_banks);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_hal_custom_hw_control (void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status, uint32 extra_status_info), 
                                 AUDIO_HAL_HW_OPERATION hw_operation, AUDIO_HAL_HW_REGISTER_TYPE register_type,
                                 uint16 num_data, uint32* address_data_sequence);

                                 
/* Sequencer layer */
AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_sequencer_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_sequencer_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH power_control);
                                  
AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsink_audio_sequencer_burst_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                                  AUDIO_COMMON_SWITCH burst_control);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_sequencer_volume_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                     uint32 channel_index, uint32 linear, int32 volume);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_sequencer_volume_query(uint32 channel_index, uint32 linear, int32* min_volume,
                                   int32* max_volume, int32* current_volume);

/* Timing critical layer */
AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_timed_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                              uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);
                              
AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_timed_volume_ramp_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, uint32 linear, int32 start_volume,
                             int32 end_volume, uint32 ramp_duration_hi, uint32 ram_duration_lo,
                             AUDIO_COMMON_SWITCH ramp_terminate);

AUDIO_HWCTRL_API void stericsson_nokiaavsink_audio_timed_volume_ramp_query(uint32 channel_index, AUDIO_TIMED_RAMP_INFO* volume_ramp_info);


/* IHF Sink APIs */
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_ihf_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_ihf_audio_close_channel(void);

/* HAL layer */

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, 
                             AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_ihf_audio_hal_mute_status (uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, AUDIO_COMMON_SWITCH power_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_ihf_audio_hal_power_status(uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_gain_capabilities_query(uint32* num_channels, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_gains_descriptor_query(AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_supported_loops_query(uint16* supported_loop_indexes);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_ihf_audio_hal_gain_control(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32 gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_ihf_audio_hal_gain_query(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32* gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_ihf_audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value);

															             
AUDIO_HWCTRL_API uint16 stericsson_ihf_audio_hal_registerbank_query(void);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_regbank_descriptors_query(AUDIO_HAL_REGBANK_DESCRIPTOR* register_banks);

AUDIO_HWCTRL_API void stericsson_ihf_audio_hal_custom_hw_control (void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status, uint32 extra_status_info), 
                                 AUDIO_HAL_HW_OPERATION hw_operation, AUDIO_HAL_HW_REGISTER_TYPE register_type,
                                 uint16 num_data, uint32* address_data_sequence);

                                 
/* Sequencer layer */
AUDIO_HWCTRL_API void stericsson_ihf_audio_sequencer_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API void stericsson_ihf_audio_sequencer_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH power_control);
                                  
AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_ihf_audio_sequencer_burst_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                                  AUDIO_COMMON_SWITCH burst_control);

AUDIO_HWCTRL_API void stericsson_ihf_audio_sequencer_volume_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                     uint32 channel_index, uint32 linear, int32 volume);

AUDIO_HWCTRL_API void stericsson_ihf_audio_sequencer_volume_query(uint32 channel_index, uint32 linear, int32* min_volume,
                                   int32* max_volume, int32* current_volume);

/* Timing critical layer */
AUDIO_HWCTRL_API void stericsson_ihf_audio_timed_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                              uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);
                              
AUDIO_HWCTRL_API void stericsson_ihf_audio_timed_volume_ramp_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, uint32 linear, int32 start_volume,
                             int32 end_volume, uint32 ramp_duration_hi, uint32 ram_duration_lo,
                             AUDIO_COMMON_SWITCH ramp_terminate);

AUDIO_HWCTRL_API void stericsson_ihf_audio_timed_volume_ramp_query(uint32 channel_index, AUDIO_TIMED_RAMP_INFO* volume_ramp_info);


/* Earpiece Sink APIs */
//AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_earpiece_audio_open_channel(void* ptr);
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_earpiece_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_earpiece_audio_close_channel(void);

/* HAL layer */

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, 
                             AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_earpiece_audio_hal_mute_status (uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, AUDIO_COMMON_SWITCH power_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_earpiece_audio_hal_power_status(uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_gain_capabilities_query(uint32* num_channels, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_gains_descriptor_query(AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_supported_loops_query(uint16* supported_loop_indexes);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_earpiece_audio_hal_gain_control(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32 gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_earpiece_audio_hal_gain_query(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32* gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_earpiece_audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value);

															             
AUDIO_HWCTRL_API uint16 stericsson_earpiece_audio_hal_registerbank_query(void);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_regbank_descriptors_query(AUDIO_HAL_REGBANK_DESCRIPTOR* register_banks);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_hal_custom_hw_control (void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status, uint32 extra_status_info), 
                                 AUDIO_HAL_HW_OPERATION hw_operation, AUDIO_HAL_HW_REGISTER_TYPE register_type,
                                 uint16 num_data, uint32* address_data_sequence);

                                 
/* Sequencer layer */
AUDIO_HWCTRL_API void stericsson_earpiece_audio_sequencer_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_sequencer_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH power_control);
                                  
AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_earpiece_audio_sequencer_burst_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                                  AUDIO_COMMON_SWITCH burst_control);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_sequencer_volume_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                     uint32 channel_index, uint32 linear, int32 volume);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_sequencer_volume_query(uint32 channel_index, uint32 linear, int32* min_volume,
                                   int32* max_volume, int32* current_volume);

/* Timing critical layer */
AUDIO_HWCTRL_API void stericsson_earpiece_audio_timed_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                              uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);
                              
AUDIO_HWCTRL_API void stericsson_earpiece_audio_timed_volume_ramp_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, uint32 linear, int32 start_volume,
                             int32 end_volume, uint32 ramp_duration_hi, uint32 ram_duration_lo,
                             AUDIO_COMMON_SWITCH ramp_terminate);

AUDIO_HWCTRL_API void stericsson_earpiece_audio_timed_volume_ramp_query(uint32 channel_index, AUDIO_TIMED_RAMP_INFO* volume_ramp_info);


//NokiaAvSource
/* NokiaAvSource Source APIs */
//AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_nokiaavsource_audio_open_channel(void* ptr);
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_nokiaavsource_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_close_channel(void);

/* HAL layer */

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, 
                             AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_nokiaavsource_audio_hal_mute_status (uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, AUDIO_COMMON_SWITCH power_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_nokiaavsource_audio_hal_power_status(uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_gain_capabilities_query(uint32* num_channels, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_gains_descriptor_query(AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_supported_loops_query(uint16* supported_loop_indexes);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsource_audio_hal_gain_control(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32 gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsource_audio_hal_gain_query(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32* gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsource_audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value);

															             
AUDIO_HWCTRL_API uint16 stericsson_nokiaavsource_audio_hal_registerbank_query(void);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_regbank_descriptors_query(AUDIO_HAL_REGBANK_DESCRIPTOR* register_banks);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_custom_hw_control (void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status, uint32 extra_status_info), 
                                 AUDIO_HAL_HW_OPERATION hw_operation, AUDIO_HAL_HW_REGISTER_TYPE register_type,
                                 uint16 num_data, uint32* address_data_sequence);

                                 
/* Sequencer layer */
AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_sequencer_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_sequencer_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH power_control);
                                  
AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_nokiaavsource_audio_sequencer_burst_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                                  AUDIO_COMMON_SWITCH burst_control);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_sequencer_volume_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                     uint32 channel_index, uint32 linear, int32 volume);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_sequencer_volume_query(uint32 channel_index, uint32 linear, int32* min_volume,
                                   int32* max_volume, int32* current_volume);

/* Timing critical layer */
AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_timed_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                              uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);
                              
AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_timed_volume_ramp_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, uint32 linear, int32 start_volume,
                             int32 end_volume, uint32 ramp_duration_hi, uint32 ram_duration_lo,
                             AUDIO_COMMON_SWITCH ramp_terminate);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_timed_volume_ramp_query(uint32 channel_index, AUDIO_TIMED_RAMP_INFO* volume_ramp_info);
            
                                    
//Multi Mic
/* MultiMic Source APIs */
//AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_multimic_audio_open_channel(void* ptr);
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_multimic_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_multimic_audio_close_channel(void);

/* HAL layer */

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, 
                             AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_multimic_audio_hal_mute_status (uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, AUDIO_COMMON_SWITCH power_control);

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_multimic_audio_hal_power_status(uint32 channel_index);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_gain_capabilities_query(uint32* num_channels, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_gains_descriptor_query(AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_supported_loops_query(uint16* supported_loop_indexes);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_multimic_audio_hal_gain_control(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32 gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_multimic_audio_hal_gain_query(uint32 channel_index, uint16 gain_index,
                                    uint32 linear, int32* gain_value);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_multimic_audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value);

															             
AUDIO_HWCTRL_API uint16 stericsson_multimic_audio_hal_registerbank_query(void);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_regbank_descriptors_query(AUDIO_HAL_REGBANK_DESCRIPTOR* register_banks);

AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_custom_hw_control (void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status, uint32 extra_status_info), 
                                 AUDIO_HAL_HW_OPERATION hw_operation, AUDIO_HAL_HW_REGISTER_TYPE register_type,
                                 uint16 num_data, uint32* address_data_sequence);

                                 
/* Sequencer layer */
AUDIO_HWCTRL_API void stericsson_multimic_audio_sequencer_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);

AUDIO_HWCTRL_API void stericsson_multimic_audio_sequencer_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                  uint32 channel_index, AUDIO_COMMON_SWITCH power_control);
                                  
AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_multimic_audio_sequencer_burst_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                                  AUDIO_COMMON_SWITCH burst_control);

AUDIO_HWCTRL_API void stericsson_multimic_audio_sequencer_volume_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                                     uint32 channel_index, uint32 linear, int32 volume);

AUDIO_HWCTRL_API void stericsson_multimic_audio_sequencer_volume_query(uint32 channel_index, uint32 linear, int32* min_volume,
                                   int32* max_volume, int32* current_volume);

/* Timing critical layer */
AUDIO_HWCTRL_API void stericsson_multimic_audio_timed_mute_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                              uint32 channel_index, AUDIO_COMMON_SWITCH mute_control);
                              
AUDIO_HWCTRL_API void stericsson_multimic_audio_timed_volume_ramp_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), 
                             uint32 channel_index, uint32 linear, int32 start_volume,
                             int32 end_volume, uint32 ramp_duration_hi, uint32 ram_duration_lo,
                             AUDIO_COMMON_SWITCH ramp_terminate);

AUDIO_HWCTRL_API void stericsson_multimic_audio_timed_volume_ramp_query(uint32 channel_index, AUDIO_TIMED_RAMP_INFO* volume_ramp_info);
                                    
AUDIO_HWCTRL_API void audio_hwctrl_setPseudoBurstMode(void);
AUDIO_HWCTRL_API void audio_hwctrl_unsetPseudoBurstMode(void);

AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_fmrx_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_fmrx_audio_close_channel(void);
AUDIO_HWCTRL_API void stericsson_fmrx_audio_hal_power_control      (void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), uint32 channel_index,AUDIO_COMMON_SWITCH power_control);
AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_fmrx_audio_hal_power_status(uint32 channel_index);
AUDIO_HWCTRL_API void stericsson_fmrx_audio_sequencer_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), uint32 channel_index, AUDIO_COMMON_SWITCH power_control);

AUDIO_HWCTRL_API void stericsson_fmrx_audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_fmrx_audio_hal_supported_loops_query(uint16* supported_loop_indexes);

AUDIO_HWCTRL_API void stericsson_fmrx_audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_fmrx_audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value);
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_fmtx_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_fmtx_audio_close_channel(void);
AUDIO_HWCTRL_API void stericsson_fmtx_audio_hal_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), uint32      channel_index,AUDIO_COMMON_SWITCH power_control);
AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_fmtx_audio_hal_power_status(uint32 channel_index);
AUDIO_HWCTRL_API void stericsson_fmtx_audio_sequencer_power_control(void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status), uint32 channel_index, AUDIO_COMMON_SWITCH power_control);
 //------------------------------------------------------------------------
 //      LineIn
 //------------------------------------------------------------------------
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_linein_audio_open_channel(void);
AUDIO_HWCTRL_API void stericsson_linein_audio_close_channel(void);
AUDIO_HWCTRL_API void stericsson_linein_audio_hal_loop_gain_capabilities_query(uint16* num_loops, uint16* max_num_gains);

AUDIO_HWCTRL_API void stericsson_linein_audio_hal_supported_loops_query(uint16* supported_loop_indexes);

AUDIO_HWCTRL_API void stericsson_linein_audio_hal_loop_gains_descriptor_query(AUDIO_HAL_HW_LOOPS loop_index,
                                    AUDIO_HAL_GAIN_DESCRIPTOR* gain_descriptor_table);

AUDIO_HWCTRL_API AUDIO_COMMON_STATUS stericsson_linein_audio_hal_loop_control(AUDIO_HAL_HW_LOOPS loop_index, AUDIO_COMMON_SWITCH loop_switch,
                                    uint16 channel_index, uint16 gain_index, uint32 linear,
                                    int32 loop_gain_value);
//---------------------------------------------------------------------------
//          BT TX
//---------------------------------------------------------------------------
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_bttx_audio_open_channel(void);
AUDIO_HWCTRL_API void          stericsson_bttx_audio_close_channel(void);
AUDIO_HWCTRL_API void          stericsson_bttx_audio_hal_power_control( 
        void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
        uint32 channel_index,
        AUDIO_COMMON_SWITCH power_control
        );
AUDIO_HWCTRL_API void          stericsson_bttx_audio_sequencer_power_control( 
        void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
        uint32 channel_index,
        AUDIO_COMMON_SWITCH power_control
        );

AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_bttx_audio_hal_power_status(uint32 channel_index);

//---------------------------------------------------------------------------
//          BT RX
//---------------------------------------------------------------------------
AUDIO_HWCTRL_API OMX_ERRORTYPE stericsson_btrx_audio_open_channel(void);
AUDIO_HWCTRL_API void          stericsson_btrx_audio_close_channel(void);
AUDIO_HWCTRL_API void          stericsson_btrx_audio_hal_power_control( 
        void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
        uint32 channel_index,
        AUDIO_COMMON_SWITCH power_control
        );
AUDIO_HWCTRL_API void          stericsson_btrx_audio_sequencer_power_control( 
        void (*call_back_fn_ptr)(AUDIO_COMMON_STATUS status),
        uint32 channel_index,
        AUDIO_COMMON_SWITCH power_control
        );
AUDIO_HWCTRL_API AUDIO_COMMON_SWITCH stericsson_btrx_audio_hal_power_status(uint32 channel_index);



AUDIO_HWCTRL_API void stericsson_multimic_audio_hal_loop_SideTone_FIRFilterCoefficients(uint32 FIRAddress, uint32* FIRCoefficient);

AUDIO_HWCTRL_API void stericsson_nokiaavsource_audio_hal_loop_SideTone_FIRFilterCoefficients(uint32 FIRAddress, uint32* FIRCoefficient);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif


