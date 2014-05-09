/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef A2DP_DATA_SINK_API_H
#define A2DP_DATA_SINK_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <OMX_Core.h>
#include "a2dpsink_component.hpp"

#ifdef __SYMBIAN32__
    #define A2DP_CTRL_DATA_API IMPORT_C
#else

    #define A2DP_CTRL_DATA_API
#endif


  /*sampling frequency of the data to be sent to A2DP chip*/
    typedef enum A2DP_SAMPLING_FREQUENCY // in KHZ
    {
    	A2DP_FREQ_UNKNOWN= 0,
        A2DP_FREQ_8KHZ = 8000,
        A2DP_FREQ_11_025KHZ = 11025,
        A2DP_FREQ_12KHZ = 12000,
        A2DP_FREQ_16KHZ = 16000,
        A2DP_FREQ_22_05KHZ = 22050,
        A2DP_FREQ_24KHZ = 24000,
        A2DP_FREQ_32KHZ = 32000,
        A2DP_FREQ_44_1KHZ = 44100,
        A2DP_FREQ_48KHZ = 48000
    } A2DP_SAMPLING_FREQUENCY;


    /*bit depth of the data transmitted, bits in a word containing valid data*/
    typedef enum A2DP_BIT_DEPTH
    {
    	A2DP_SIZE_UNDEF=0,
    	A2DP_8BITS,
    	A2DP_16BITS,
    	A2DP_24BITS,
   	A2DP_32BITS
     } A2DP_BIT_DEPTH;

    typedef enum A2DP_POWER_STATE
    {
    	A2DP_POWER_DOWN,
	A2DP_POWER_UP
    } A2DP_POWER_STATE;

    typedef enum A2DP_CHANNEL_MODE
    {
    	A2DP_MODE_UNDEF = 0,
    	A2DP_MODE_MONO,
    	A2DP_MODE_STEREO 
    }A2DP_CHANNEL_MODE;

    /*a2dp chip being master or slave to MSP*/
    typedef struct A2DP_CONF {
    	OMX_U32			nTunnelId;
    	OMX_U32			nBufferSizeMs;
    	A2DP_BIT_DEPTH		nSampleSize;
    	A2DP_CHANNEL_MODE 	nChanMode;
    	A2DP_SAMPLING_FREQUENCY	nSamplingFreq;       
     } A2DP_CONF;
	     
    /*
    opens/closes the provider handle to the audio streaming driver
    Channel.
    @returns OMX_ERRORTYPE
    */
    A2DP_CTRL_DATA_API OMX_ERRORTYPE stericsson_a2dpsink_open_ctrl_data();
    A2DP_CTRL_DATA_API void stericsson_a2dpsink_close_ctrl_data();
    A2DP_CTRL_DATA_API OMX_ERRORTYPE stericsson_a2dpsink_config_provider(A2DP_CONF Config);
    //A2DP_CTRL_DATA_API OMX_ERRORTYPE stericsson_a2dpsink_config_provider();
    A2DP_CTRL_DATA_API void stericsson_a2dpsink_set_owner(a2dpsink_component *owner);

    /*start the transmit of a2dp buffer corresponding to the id
    @buffer_id: carries the unique identification for the buffer
    returned at the time of creation.
    @returns OMX_ERRORTYPE
    */
    A2DP_CTRL_DATA_API OMX_ERRORTYPE stericsson_a2dpsink_start();
    A2DP_CTRL_DATA_API void stericsson_a2dpsink_stop();
    A2DP_CTRL_DATA_API void stericsson_a2dpsink_buffer_processed(OMX_BUFFERHEADERTYPE_p sinkBuffer);

    /*queries if transmit of a2dp buffer corresponding to the id
    is pending
    @buffer_id: carries the unique identification for the buffer
    returned at the time of creation.
    @returns true/false.
    */
    A2DP_CTRL_DATA_API void stericsson_a2dpsink_buffer_filled(bool status);

    /*cancels the  transmit of a2dp buffer corresponding to the id.
    @buffer_id: carries the unique identification for the buffer
    returned at the time of creation.
    */
    A2DP_CTRL_DATA_API bool stericsson_a2dpsink_is_buffer_requested();

    /* Gets the format supported by A2DP Device, can be compressed or pcm*/
    /*A2DP_DATA_TYPE format_supported();*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //A2DP_CTRL_DATA_API_H


