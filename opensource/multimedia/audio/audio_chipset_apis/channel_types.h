/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \brief   Includes Audio Channel types enum
 * \author  ST-Ericsson
 */
/*****************************************************************************/

#ifndef __CHANNEL_TYPE_H__
#define __CHANNEL_TYPE_H__

enum TAudioCodecChannelType
        {
        EMinAudioCodecChannelTypes=-1,
        EAudioCodecEarChannel=0,
        EAudioCodecHSChannel,
        EAudioCodecIHFChannel,
        EAudioCodecVibLChannel,
        EAudioCodecVibRChannel,
        EAudioCodecMic1AChannel,
        EAudioCodecMic1BChannel,
        EAudioCodecMic2Channel,
        EAudioCodecLinChannel,
        EAudioCodecDMic12Channel,
        EAudioCodecDMic34Channel,
        EAudioCodecDMic56Channel,
        EAudioCodecMultiMicChannel,
        EAudioCodecFMRxChannel,
        EAudioCodecFMTxChannel,
        //EBluetooth_HDMIChannel,
        EConfigBluetoothChannel,
        EAudioModemChannel,
        EMaxAudioCodecChannelTypes
        //EAccessory
        };

    
            
#endif

/* End of d32audioio.h */
