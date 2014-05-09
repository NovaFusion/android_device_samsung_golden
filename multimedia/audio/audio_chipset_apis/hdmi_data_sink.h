/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef HDMI_DATA_SINK_API_H
#define HDMI_DATA_SINK_API_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <OMX_Core.h>
#include "hdmisink_component.hpp"

#ifdef __SYMBIAN32__
    #define HDMI_DATA_API IMPORT_C
#else
    #define HDMI_DATA_API
#endif
    /*
    opens/closes the data access handle to the driver
    Channel.
    @returns OMX_ERRORTYPE
    */
    HDMI_DATA_API OMX_ERRORTYPE stericsson_hdmisink_audio_open_datachannel(hdmisink_component *owner);
    HDMI_DATA_API OMX_ERRORTYPE stericsson_hdmisink_audio_close_datachannel();

    /*start the transmit of hdmi buffer corresponding to the id
    @buffer_id: carries the unique identification for the buffer
    returned at the time of creation.
    @returns OMX_ERRORTYPE
    */
    HDMI_DATA_API OMX_ERRORTYPE stericsson_hdmisink_audio_transmit_buffers(int buffer_id);


    /*queries if transmit of hdmi buffer corresponding to the id
    is pending
    @buffer_id: carries the unique identification for the buffer
    returned at the time of creation.
    @returns true/false.
    */
    HDMI_DATA_API bool stericsson_hdmisink_audio_is_tx_pending(int buffer_id);

    /*cancels the  transmit of hdmi buffer corresponding to the id.
    @buffer_id: carries the unique identification for the buffer
    returned at the time of creation.
    */
    HDMI_DATA_API void stericsson_hdmisink_audio_cancel_transmit(int buffer_id);

    /* Gets the format supported by HDMI Device, can be compressed or pcm*/
    /*HDMI_DATA_TYPE format_supported();*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //HDMI_DATA_SINK_API_H
