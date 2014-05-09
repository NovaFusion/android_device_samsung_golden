/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef HDMI_CTRL_SINK_API_H
#define HDMI_CTRL_SINK_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <OMX_Core.h>

#ifdef __SYMBIAN32__
    #define HDMI_CTRL_API IMPORT_C
#else
    #define HDMI_CTRL_API
#endif
    /*
    underlying interface used for data transfer from MSP to AV8100*/
    typedef enum HDMI_DATA_INTERFACE
    {
        HDMI_I2s=0,
        HDMI_I2sdelay,
        HDMI_TDM,
    } HDMI_DATA_INTERFACE;

    /*number of channels containing valid data*/
    
    typedef enum HDMI_NUMBER_OF_CHANNELS
    {
    	EOneI2sChan=1,
    	ETwoI2sChan,
    	EThreeI2sChan,
    	EFourI2sChan
    } HDMI_NUMBER_OF_CHANNELS;


    /*sampling frequency of the data to be sent to HDMI chip*/
    typedef enum HDMI_SAMPLING_FREQUENCY // in KHZ
    {
        /*HDMI_FREQ_8KHZ,
        HDMI_FREQ_11_025KHZ,
        HDMI_FREQ_12KHZ,
        HDMI_FREQ_16KHZ,
        HDMI_FREQ_22_05KHZ,
        HDMI_FREQ_24KHZ,*/
        HDMI_FREQ_32KHZ,
        HDMI_FREQ_44_1KHZ,
        HDMI_FREQ_48KHZ,
        HDMI_FREQ_UNKNOWN = 0xffffff
    } HDMI_SAMPLING_FREQUENCY;


    /*bit depth of the data transmitted, bits in a word containing valid data*/
    typedef enum HDMI_BIT_DEPTH
    {
        HDMI_16BITS,
        HDMI_20BITS,
        HDMI_24BITS
    } HDMI_BIT_DEPTH;

    /*data sent is pcm or compressed*/
    typedef enum HDMI_DATA_TYPE
    {
        HDMI_PCM,
        HDMI_COMPRESSED
    } HDMI_DATA_TYPE;

    /*mute unmute hdmi*/
    typedef enum HDMI_MUTE_STATE
    {
        HDMI_MUTE_DISABLED,
        HDMI_MUTE_ENABLED
    } HDMI_MUTE_STATE;

    /*hdmi chip being master or slave to MSP*/
    typedef enum HDMI_CHIP_MODE
    {
    	HDMI_SLAVE,
    	HDMI_MASTER,
        
    } HDMI_CHIP_MODE;

    typedef enum HDMI_POWER_STATE
    {
        HDMI_POWER_DOWN,
        HDMI_POWER_UP
    } HDMI_POWER_STATE;
    
    typedef enum HDMI_SPEAKER_PLACEMENT
    {
    	HDMI_N_N_N_N_N_N_FR_FL               = 0,
    	HDMI_N_N_N_N_N_LFE_FR_FL             = 1,
    	HDMI_N_N_RR_RL_FC_LFE_FR_FL          = 11,
    	HDMI_FRH_FLH_RR_RL_FC_LFE_FR_FL      = 19
    	//To be completed after checking at forum
    } HDMI_SPEAKER_PLACEMENT;
    
    /*hdmi chip being master or slave to MSP*/
    typedef struct HDMI_CONF {
        HDMI_DATA_INTERFACE     data_if;
        HDMI_NUMBER_OF_CHANNELS channels;
        HDMI_SAMPLING_FREQUENCY frequency;
        HDMI_BIT_DEPTH          bit_depth;
        HDMI_DATA_TYPE          data_type;
        HDMI_CHIP_MODE          mode;
        HDMI_MUTE_STATE         muteSwitch;
        HDMI_SPEAKER_PLACEMENT  spkr_placement;
        
    } HDMI_CONF;
    

    /*
    opens/closes the access handle to the ctrl channel of the hdmi driver
    @returns success/failure
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_open_ctrlchannel();
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_close_ctrlchannel();

    /*
    configures the underlying sw and hw with the configuration asked in HDMI_CONF
    @conf hdmi_conf to be filled by the caller.
    returns OMX_ERRORTYPE.
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_configure(HDMI_CONF conf);

	/*
    creates(if not created earlier)physically contiguous, non cacheable buffers
    and initialzes the allocation length.
    @buffer_hdr: requires a pointer to OMX_BUFFERHEADERTYPE that will be returned with
    buffer address(pBuffer) as well the size of contiguous memory allocated. This is double the size of one buffer
    @returns OMX_ERRORTYPE
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_allocate_buffers(OMX_BUFFERHEADERTYPE* buffer_hdr, OMX_U32 nb_buffers);

    /*
    frees earlier created buffer through call stericsson_hdmisink_audio_allocate_buffers
    @buffer_id: carries the unique identification for the buffer returned at the time of creation.
    @returns OMX_ERRORTYPE
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_free_buffers();

    /*
    Acquires/Releases the clock for MSP and powerup/down the HDMI.
    returns OMX_ERRORTYPE
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_powerctrl(HDMI_POWER_STATE state);

    /*
    Starts the data transmission using DMA to MSP2
    data already posted through previous stericsson_hdmisink_audio_transmit_buffers calls.
    returns OMX_ERRORTYPE
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_start();

    /*
    Set the state of underlying HDMI sw as STOPPED.
    Doesnot take any more buffers from client through transmit_hdmi_buffer
    Also on getting cancel_transmit_hdmi_buffer in future on data channel,
    stops the ongoing transmission, and flushing the data.
    returns OMX_ERRORTYPE*/

    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_stop();


    /*
    Set the state of underlying HDMI sw as PAUSED.
    Doesnot take any more buffers from client through transmit_hdmi_buffer
    Also on getting cancel_transmit_hdmi_buffer in future on data channel,
    stops the ongoing transmission, and retain those buffers for future transmission.

    returns OMX_ERRORTYPE
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_pause();
    
        /*
    Set the state of underlying HDMI sw as MUTE/UNMUTE.
    
    returns OMX_ERRORTYPE
    */
    HDMI_CTRL_API OMX_ERRORTYPE stericsson_hdmisink_audio_mute(HDMI_MUTE_STATE muteSwitch);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //HDMI_CTRL_SINK_API_H

