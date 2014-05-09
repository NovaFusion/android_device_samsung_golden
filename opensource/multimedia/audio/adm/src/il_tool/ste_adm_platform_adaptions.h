/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */
/*! \file ste_adm_cmd.h
*   \brief Platform specific adaptions.

    This header exposes functionality for handling platform specific adaptions.
*/
#ifndef STE_ADM_PLATFORM_ADAPTIONS_GUARD_H
#define STE_ADM_PLATFORM_ADAPTIONS_GUARD_H

#include "ste_adm_omx_tool.h"

OMX_INDEXTYPE adaptions_string_to_index(const char* str); // TODO: Rename
#define platform_adaptions_string_to_index string_to_index

/**
 *  Sets all platform specific configurations required for a specific
 *  component, by name.
 *  Those can be overridden in database if required.
 */
ste_adm_res_t adm_adaptions_setup_comp(
    il_comp_t pHandle,
    const char* cComponentName);


//
// IL components
//
// mixer:    port 0 is output, port 1-8 are inputs
// splitter: port 0 is input, port 1-8 are outputs

typedef enum {
    ADM_ARM_MIXER_NAME,
    ADM_ARM_SPLITTER_NAME,
    ADM_DSP_MIXER_NAME,
    ADM_DSP_SPLITTER_NAME,
    ADM_VC_DL_SPLITTER_NAME,
    ADM_VC_DL_MIXER_NAME,
    ADM_VC_UL_SPLITTER_NAME,
    ADM_VC_UL_MIXER_NAME,
    ADM_VC_DICT_SPLITTER_NAME,
    ADM_VC_DICT_MIXER_NAME,
    ADM_MODEM_SOURCE_NAME,
    ADM_MODEM_SINK_NAME,
    ADM_DEFAULT_APP_MIXER_NAME,
    ADM_DEFAULT_COMMON_MIXER_NAME,
    ADM_DEFAULT_SPLITTER_NAME,
    ADM_CSCALL_NAME,
    ADM_SPEECHPROC_NAME,
	ADM_COMMON_MIXER_NAME
} comp_name_id_t;

const char* adaptions_comp_name(comp_name_id_t id);

int adaptions_internal_mixsplit(void);

const char* adaptions_get_mixer_internal_effect_name(const char* name);

int adaptions_is_dsp_component(const char* name);


// 0) uses AFM mixers/splitters for playback chain
// 1) uses ADM mixers/splitters for playback chain
#define ADM_INTERNAL_MIXSPLIT_DEFAULT_VALUE 1

//
// CSCall component. Communicates with modem.
//
#define ADM_CSCALL_DL_PORT 0
#define ADM_CSCALL_UL_PORT 1
#define ADM_CSCALL_CTRL_PORT 2
#define ADM_CSCALL_MPC_REF_PORT 2
//
// Speech processing component
//
#define ADM_SPEECHPROC_UL_IN_PORT        0   // Uplink input port
#define ADM_SPEECHPROC_DL_IN_PORT        1   // Downlink input port
#define ADM_SPEECHPROC_REF_PORT          2   // Echo reference input
#define ADM_SPEECHPROC_UL_OUT_PORT       3   // Uplink output port
#define ADM_SPEECHPROC_DL_OUT_PORT       4   // Downlink output port


#define MIXER_OUTPUT_PORT   0
#define MIXER_INPUT_PORT   1
#define SPLITTER_INPUT_PORT 0
#define SPLITTER_OUTPUT_PORT 1
#define SPLITTER_DICT_OUTPUT_PORT 2


#endif // STE_ADM_PLATFORM_ADAPTIONS_GUARD_H

