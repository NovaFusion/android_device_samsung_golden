/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_Types.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_TYPES_H_
#define _AFM_TYPES_H_

#include "OMX_Types.h"
#include "OMX_Audio.h"
#include "ENS_Index.h"

#define DEFAULT_PCM_BLOCKSIZE   240
#define DEFAULT_PCM_BLOCKDURATION   5

typedef struct AFM_PARAM_PCMMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nChannels;
    OMX_U32 nPortIndex;
    OMX_ENDIANTYPE eEndian;
    OMX_U32 nSamplingRate;
    OMX_U32 nBitPerSample;
    OMX_BOOL nFrameAligned;
} AFM_PARAM_PCMMODETYPE;

typedef struct AFM_PARAM_BITSTREAMMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL nFrameAligned;
} AFM_PARAM_BITSTREAMMODETYPE;

typedef struct AFM_PARAM_PCMLAYOUTTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nBlockSize;
    OMX_U32 nChannels;
    OMX_U32 nBitsPerSample;
    OMX_U32 nMaxChannels;
    OMX_U32 nNbBuffers;
} AFM_PARAM_PCMLAYOUTTYPE;

/*struct AFM_AUDIO_CONFIG_CSCALL_TIMING_REQ_MARGINS
 * @nbMargin for margins to be used for narrow band codecs
 * @wbMArgin for margins to be used for wide band codecs
 * */
typedef struct AFM_AUDIO_CONFIG_CSCALL_TIMING_REQ_MARGINS {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nbMargin;
    OMX_U32 wbMargin;
}AFM_AUDIO_CONFIG_CSCALL_TIMING_REQ_MARGINS;

/*struct AFM_AUDIO_CONFIG_CSCALL_DATA_WAIT_TIME
 * @threshold is the threshold until source won't be reconfigured
 * @targetWaitTime is the time data are wanted to wait in cscall before beeing sent to modem
 * @waitBeforeReconfigure How many time we get over the threshold before reconfiguring
 * */
typedef struct AFM_AUDIO_CONFIG_CSCALL_DATA_WAIT_TIME {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U16 threshold;
    OMX_U16 targetWaitTime;
    OMX_U16 waitBeforeReconfigure;
}AFM_AUDIO_CONFIG_CSCALL_DATA_WAIT_TIME;
/*
struct AFM_AUDIO_CSCALL_Protocol
@AFM_AUDIO_PROTOCOL_ISI protocol type chosen to ISI
@AFM_AUDIO_PROTOCOL_MAI  protocol type chosen to MAI
*/
typedef enum AFM_AUDIO_CSCALL_SPEECH_PROTOCOL_TYPE{
    AFM_AUDIO_PROTOCOL_AUTODETECT,
    AFM_AUDIO_PROTOCOL_ISI,
    AFM_AUDIO_PROTOCOL_MAI,
    AFM_AUDIO_PROTOCOL_ISI_TD
}AFM_AUDIO_SPEECH_PROTOCOL_TYPE;

/*
Enum used to select the type of loop on Modem side
*/
typedef enum AFM_MODEM_LOOP_TYPE{
    AFM_MODEM_LOOP_OFF,
    AFM_MODEM_LOOP_RAW,
    AFM_MODEM_LOOP_SPEECH_CODEC
}AFM_MODEM_LOOP_TYPE;

typedef struct AFM_PARAM_CSCALLMODETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nULRealTimeMode;
    OMX_U32 nDLRealTimeMode;
    AFM_AUDIO_SPEECH_PROTOCOL_TYPE eProtocol;
} AFM_PARAM_CSCALLMODETYPE;

typedef struct AFM_PARAM_AMRNBENCODERTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bNoHeader;
    OMX_U16  ePayloadFormat;
    OMX_BOOL bEfr_on;
} AFM_PARAM_AMRNBENCODERTYPE;

typedef struct AFM_CONFIG_AMRNBENCODERTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bDtxEnable;
    OMX_U16 nBitRate;
} AFM_CONFIG_AMRNBENCODERTYPE;

typedef struct AFM_PARAM_AMRNBDECODERTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bNoHeader;
    OMX_BOOL bErrorConcealment;
    OMX_U16  ePayloadFormat;
        OMX_BOOL bEfr_on;
} AFM_PARAM_AMRNBDECODERTYPE;

typedef struct AFM_PARAM_CAPTURERTYPE {
  OMX_U32              nSize;
  OMX_VERSIONTYPE      nVersion;
  OMX_U32              nBufferSize;
} AFM_PARAM_CAPTURERTYPE;

typedef struct AFM_CONFIG_LOWPOWERTYPE {
    OMX_U32              nSize;
    OMX_VERSIONTYPE      nVersion;
    OMX_BOOL             nLowPower;
} AFM_CONFIG_LOWPOWERTYPE;

// Index used to force standard tunneling instead
// of proprietary communication (debug only!)
typedef struct AFM_PARAM_FORCE_STANDARD_TUNNELING {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bIsStandardTunneling;
} AFM_PARAM_FORCE_STANDARD_TUNNELING;

typedef struct AFM_PARAM_PRIORITY {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPriority;
} AFM_PARAM_PRIORITY; //FIXME:ProcessingComp?

typedef struct AFM_PARAM_AACDECTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U16 nSyntax;
    OMX_U16 nObjectType;
    OMX_U16 nFrequency;
    OMX_U16 ncrc_ignore;
    OMX_U16 nDownSample;
    OMX_U16 nEnableSBR;
    OMX_U16 nErrorConcealment;
    OMX_U16 nbsac_on;
    OMX_U16 nbsac_nch;
    OMX_U16 nbsac_layer;
    OMX_U16 nbsac_usemaxlayer;
} AFM_PARAM_AACDECTYPE;

//Param for TransducerEqualizer,
typedef struct AFM_AUDIO_PARAM_TRANSDUCEREQUALIZERTYPE {
    OMX_U32 nSize;             /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
    OMX_U16  MemoryPreset;
    OMX_BOOL stereo;
    OMX_U16  nb_alloc_biquad_cells_per_channel;
    OMX_U16  nb_alloc_FIR_coefs_per_channel;
} AFM_AUDIO_PARAM_TRANSDUCEREQUALIZERTYPE;

//Config for TransducerEqualizer,
typedef struct AFM_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE {
    OMX_U32 nSize;             /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
    OMX_BOOL        same_biquad_l_r;
    OMX_BOOL        same_FIR_l_r;
    OMX_BOOL        biquad_first;
    OMX_S16         biquad_gain_exp_l;
    OMX_S32         biquad_gain_mant_l;
    OMX_S16         biquad_gain_exp_r;
    OMX_S32         biquad_gain_mant_r;
    OMX_S16         FIR_gain_exp_l;
    OMX_S32         FIR_gain_mant_l;
    OMX_S16         FIR_gain_exp_r;
    OMX_S32         FIR_gain_mant_r;
    OMX_U16         nb_biquad_cells_per_channel;
    OMX_U16         nb_FIR_coefs_per_channel;
    struct
    {
        OMX_S16 b_exp;
        OMX_S32 b0;
        OMX_S32 b1;
        OMX_S32 b2;
        OMX_S32 a1;
        OMX_S32 a2;
    }               biquad_cell[40];
    OMX_S32         FIR_coef[400];
    OMX_BOOL        bEnable;
    OMX_BOOL        bAutomaticTransitionGain;
    OMX_S16         transition_gain_exp_l;
    OMX_S32         transition_gain_mant_l;
    OMX_S16         transition_gain_exp_r;
    OMX_S32         transition_gain_mant_r;
    OMX_S32         smoothing_delay_ms;
} AFM_AUDIO_CONFIG_TRANSDUCEREQUALIZERTYPE;


//Config for Legacy Graphic Equalizer : same structure as OMX_AUDIO_CONFIG_EQUALIZERTYPE
typedef struct AFM_AUDIO_CONFIG_LEGACYGRAHICEQUALIZERTYPE {
    OMX_U32 nSize;             /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
    OMX_U32 nPortIndex;        /**< port that this structure applies to */
    OMX_BOOL bEnable;          /**< Enable/disable for equalizer */
    OMX_BU32 sBandIndex;       /**< Band number to be set.  Upper Limit is
                                    N-1, where N is the number of bands, lower limit is 0 */
    OMX_BU32 sCenterFreq;      /**< Center frequecies in Hz.  This is a
                                    read only element and is used to determine
                                    the lower, center and upper frequency of
                                    this band.  */
    OMX_BS32 sBandLevel;       /**< band level in millibels */
} AFM_AUDIO_CONFIG_LEGACYGRAHICEQUALIZERTYPE;

//Param for Sound Pressure Limiter,
typedef struct AFM_AUDIO_PARAM_SPLTYPE {
    OMX_U32 nSize;             /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
    OMX_U16  MemoryPreset;
    OMX_U16  nType;            /**< Limiter type: 0 sound pressure, 1 peak */
    OMX_U16  nAttackTime;      /**< Gain adaptation attack time in peak limiter in ms */
    OMX_U16  nReleaseTime;     /**< Gain adaptation release time in peak limiter in ms*/
} AFM_AUDIO_PARAM_SPLTYPE;

/*
struct AFM_AUDIO_CSCALL_SPEECH_NETWORK_TYPE:
@AFM_AUDIO_NETWORK_NONE network type chosen to NONE
@AFM_AUDIO_NETWORK_GSM network type chosen to GSM
@AFM_AUDIO_NETWORK_WCDMA network type chosen to WCDMA
*/
typedef enum AFM_AUDIO_CSCALL_SPEECH_NETWORK_TYPE{
    AFM_AUDIO_NETWORK_NONE,
    AFM_AUDIO_NETWORK_GSM,
    AFM_AUDIO_NETWORK_WCDMA,
	AFM_AUDIO_NETWORK_CS_O_HS
}AFM_AUDIO_SPEECH_NETWORK_TYPE;

/*
struct AFM_AUDIO_CSCALL_CONFIG_SPEECH_ENCODER:
@bAAC When GSM AMR/WB-AMR codecs are in use, this field contains the NoiseSuppression Control Bit (NSCB) as defined in 3GPP 44.018. It toggles the state of the Noise Suppression in the audio component. This should be ignored if the Speech Codec is set to non GSM AMR modes.
@bNsynch Defines whether the NSYNC synchronisation procedure should be used within 2G AMR calls to temporarily disable DTX after handover. When set ON, the encoder should disable DTX and send only speech for the next 12 UL frames. This should be ignored if the Speech Codec is set to non GSM AMR modes.
@nSFN Used by the modem to synchronise codec rate changes on a specified speech frame during 2G AMR calls. The value given specifies the Speech Frame Number of the next CMI frame at which the rate change should be applied. NOTE: The current Speech Frame Number is provided to the modem in UL GSM AMR frames (see section 4.8). This should be ignored if the Speech Codec is set to non GSM AMR modes.
@nBandModeType Defines the AMR codec rate to be configured for uplink audio data. This should be ignored if the Speech Codec is set to non AMR modes.
@bDTX Defines whether DTX may be used for the selected codec
*/
typedef struct AFM_AUDIO_CSCALL_CONFIG_SPEECH_ENCODER {
    OMX_BOOL bAAC;//Audio Activity Control
    OMX_BOOL bNsynch;//NSYNC synchronisation
    OMX_U8 nSFN;//Speech Frame Number
    OMX_AUDIO_AMRBANDMODETYPE nBandModeType;//AMR Rate
    OMX_BOOL bPCMcodecIn16K; // OMX_TRUE :means that the PCM codecs is setup in 16K, otherwise it is in 8K.
    OMX_BOOL bDTX;//DTX used
    OMX_BOOL bNoData; // send AMT nodata frame to modem
    OMX_BOOL bNSCB; //Noice suppression control bit
} AFM_AUDIO_CSCALL_CONFIG_SPEECH_ENCODER;

/*
struct AFM_AUDIO_CONFIG_CSCALL_SPEECH_CODEC_REQ:
@nNetworkType network type used for the cs call (AFM_AUDIO_NETWORK_NONE/AFM_AUDIO_NETWORK_GSM/AFM_AUDIO_NETWORK_WCDMA)
@nCodingType Defines the codec to be configured (AMR, GSM FR, GSM HR note: in case of AMR NB and AMR WB use AMR, distinction will be made on the basis of nBandModeType)
@encoderconfig Defines the config sent to encoder.(ref explanation of struct AFM_AUDIO_CSCALL_CONFIG_SPEECH_ENCODER)
@nBandModeTypeDecoder Defines the AMR codec rate to be configured for downlink audio data.This should be ignored if the Speech Codec is set to non AMR modes.
*/
typedef struct AFM_AUDIO_CONFIG_CSCALL_SPEECH_CODEC_REQ {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    AFM_AUDIO_SPEECH_NETWORK_TYPE nNetworkType;//User of Speech Codec(Network)
    OMX_AUDIO_CODINGTYPE nCodingType;//Speech Codec
    AFM_AUDIO_CSCALL_CONFIG_SPEECH_ENCODER encoderconfig;//Encoder Configuration
    OMX_AUDIO_AMRBANDMODETYPE nBandModeTypeDecoder;//AMR codec rate for downlink
    OMX_BOOL bPCMcodecIn16K;// OMX_TRUE means that the PCM codec is in 16K otherwise it is in 8K
    OMX_BOOL codecInitialization;//Codec Initialisation
} AFM_AUDIO_CONFIG_CSCALL_SPEECH_CODEC_REQ ;//corresponding index AFM_IndexConfigCscallSpeechCodecReq


/*
struct AFM_AUDIO_CONFIG_CSCALL_UPLINK_TIMING_REQ:
@nDeliveryTime Delivery schedule for subsequent uplink audio frames in microseconds.
@nModemProcessingTime Time taken for modem to process and transmit the data (Tencode+Trftx), ref 43.050.
*/
typedef struct AFM_AUDIO_CONFIG_CSCALL_UPLINK_TIMING_REQ {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nDeliveryTime;
    OMX_U16 nModemProcessingTime;
    OMX_U16 nCause; // 0-too late  1-too early  2-discard
} AFM_AUDIO_CONFIG_CSCALL_UPLINK_TIMING_REQ; //corresponding index AFM_IndexConfigCscallSpeechTimingReq


/*
struct AFM_AUDIO_CONFIG_CSCALL_VOICE_CALL_STATUS
@bConnected is voice call connected or not
@eProtocol protocol to use
*/
typedef struct AFM_AUDIO_CONFIG_CSCALL_VOICE_CALL_STATUS {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bConnected;
} AFM_AUDIO_CONFIG_CSCALL_VOICE_CALL_STATUS; //corresponding index AFM_IndexConfigCscallVoiceCallStatus


/*
struct AFM_AUDIO_CONFIG_CSCALL_MREP_REQ:
@nNextDLTiming Delivery schedule for next DL audio frames in microseconds.
@pDLData Pointer on DL modem trace input.
*/
typedef struct AFM_AUDIO_CONFIG_CSCALL_MREP_REQ {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
	OMX_U32 nNextDLTiming;
    OMX_U16 *pDLData;
	OMX_U32 nOutputULSize;
    OMX_U16 *pULData;
} AFM_AUDIO_CONFIG_CSCALL_MREP_REQ; //corresponding index AFM_IndexConfigCscallMRePReq

/*
struct AFM_AUDIO_CONFIG_MODEM_LOOPBACK:
@nLoopBackMode can be equal to 'Off', 'Raw' or 'SpeechCodec'
@pDLData Pointer on DL modem trace input.
*/
typedef struct AFM_AUDIO_CONFIG_MODEM_LOOPBACK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    AFM_MODEM_LOOP_TYPE nLoopBackMode;
	OMX_AUDIO_CODINGTYPE nSpeechCodec;
    OMX_AUDIO_AMRBANDMODETYPE nAMRrate;
} AFM_AUDIO_CONFIG_MODEM_LOOPBACK; //corresponding index AFM_IndexConfigModemLoopBack


/*
struct AFM_AUDIO_PARAM_MSP_NB_CHANNEL
@nNbChannel limit number of msp channels
*/
typedef struct AFM_AUDIO_PARAM_MSP_NB_CHANNEL {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nNbChannel;
} AFM_AUDIO_PARAM_MSP_NB_CHANNEL; //corresponding index AFM_IndexParamMspNbChannel


/*
  Enum used to distinguish various form of Proprietary Communication between AFM components
*/
typedef enum AFM_PROPRIETARY_COMMUNICATION{
    AFM_NO_PROPRIETARY_COMMUNICATION	     = 0,
    AFM_MPC_PROPRIETARY_COMMUNICATION	     = 0x1,
    AFM_HYBRIDMPC_PROPRIETARY_COMMUNICATION  = 0x11,
    AFM_HOST_PROPRIETARY_COMMUNICATION	     = 0x2,
    AFM_HYBRIDHOST_PROPRIETARY_COMMUNICATION = 0x12,
}AFM_PROPRIETARY_COMMUNICATION;

typedef struct AFM_PARAM_PROPRIETARY_COMMUNICATION {
  OMX_U32 nSize;
  OMX_VERSIONTYPE nVersion;
  OMX_U32 nPortIndex;
  AFM_PROPRIETARY_COMMUNICATION eCommType;
} AFM_PARAM_PROPRIETARY_COMMUNICATION;

typedef struct AFM_AUDIO_PARAM_DMA_CHANNEL_TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL isPhysical; // OMX_TRUE: DMA used in physical mode, OMX_FALSE:DMA used in logical mode
} AFM_AUDIO_PARAM_DMA_CHANNEL_TYPE; //corresponding index AFM_IndexParamDmaChannelType

typedef enum OMX_PORT_PRIORITY {
    OMX_PriorityBackground = 0,
    OMX_PriorityNormal,
    OMX_PriorityUrgent,
    OMX_PriorityTelephonyDL,
    OMX_PriorityTelephonyUL,
    OMX_PriorityTelephonyUL_2,
}OMX_PORT_PRIORITY;

typedef struct  OMX_PARAM_LINEPRIORITYTYPE{
  OMX_U32 nSize;
  OMX_VERSIONTYPE nVersion;
  OMX_U32 nPortIndex;
  OMX_PORT_PRIORITY ePortPriority;
} OMX_PARAM_LINEPRIORITYTYPE;

//Config for Xprot
typedef struct AFM_AUDIO_CONFIG_LSPROTECTIONTYPE {
    OMX_U32 nSize;             /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
    // Mono or left channel of stereo
    OMX_S16 x_lm;         // displacement limit
    OMX_S16 sigma_dp;     // displacement predictor coefficient
    OMX_S16 a_1_t;        // displacement predictor coefficient
    OMX_S16 a_2_t;        // displacement predictor coefficient
    OMX_S16 t_r;          // peak detector release coefficient
    OMX_S16 t_av1;        // LFSN coefficient update smoothing factor
    OMX_S16 t_av2;        // LFSN coefficient update smoothing factor
    OMX_S16 pa1n_asnd[5]; // a_1_r estimator polynome coefficients
    OMX_S16 pa2n_asnd[5]; // a_2_r estimator polynome coefficients
    OMX_S16 s_pa1n;       // a_1_r estimator scaling factor
    OMX_S16 s_pa2n;       // a_2_r estimator scaling factor
    OMX_S16 b_d;          // sigma_c_0 estimator scaling factor
    OMX_S16 a_1_r;        // LFSN filter coefficient
    OMX_S16 a_2_r;        // LFSN filter coefficient
    OMX_S16 b_1_c;        // LFSN filter coefficient
    OMX_S16 b_2_c;        // LFSN filter coefficient
    OMX_S16 sigma_c_0;    // LFSN filter coefficient
    OMX_S16 t_lm;         // temperature limit
    OMX_S16 sigma_T_amb;  // scaling factor for ambient temperature
    OMX_S16 b_tv;         // voice coil thermal model coefficient
    OMX_S16 a_tv;         // voice coil thermal model coefficient
    OMX_S16 b_tm;         // magnet thermal model coefficient
    OMX_S16 a_tm;         // magnet thermal model coefficient
    OMX_S16 frame_length; // length of audio frame
    OMX_S16 t_mp;         // scaling factor for other temperatures
    OMX_S16 a_1_x_d;
    OMX_S16 a_2_x_d;
    OMX_S16 b_2_x_d;
    OMX_S16 b_1_u_d;
    OMX_S16 b_2_u_d;
    OMX_S32 alfa;
    OMX_S32 beta;
    OMX_S16 ext_gain_offset;
    OMX_S16 low_boost_comp_gain;
    OMX_S16 therm_prediction_rate;
    OMX_S16 dummy1;		// assure even addresses for mmdsp
    OMX_S32 t_rav_l;      // Reserved for possible future use
    OMX_S16 t_att;        // T_ATT
    OMX_S16 t_att1;       // T_ATT1
    OMX_S16 t_rel1;       // T_REL1
    OMX_S16 t_rel2;       // T_REL2
    OMX_S16 t_rel3;       // T_REL3
    OMX_S16 t_ch_lim;     // Replaces hardcoded number 25
    // Right channel
    OMX_S16 x_lm_r;         // displacement limit
    OMX_S16 sigma_dp_r;     // displacement predictor coefficient
    OMX_S16 a_1_t_r;        // displacement predictor coefficient
    OMX_S16 a_2_t_r;        // displacement predictor coefficient
    OMX_S16 t_r_r;          // peak detector release coefficient
    OMX_S16 t_av1_r;        // LFSN coefficient update smoothing factor
    OMX_S16 t_av2_r;        // LFSN coefficient update smoothing factor
    OMX_S16 pa1n_asnd_r[5]; // a_1_r estimator polynome coefficients
    OMX_S16 pa2n_asnd_r[5]; // a_2_r estimator polynome coefficients
    OMX_S16 s_pa1n_r;       // a_1_r estimator scaling factor
    OMX_S16 s_pa2n_r;       // a_2_r estimator scaling factor
    OMX_S16 b_d_r;          // sigma_c_0 estimator scaling factor
    OMX_S16 a_1_r_r;        // LFSN filter coefficient
    OMX_S16 a_2_r_r;        // LFSN filter coefficient
    OMX_S16 b_1_c_r;        // LFSN filter coefficient
    OMX_S16 b_2_c_r;        // LFSN filter coefficient
    OMX_S16 sigma_c_0_r;    // LFSN filter coefficient
    OMX_S16 t_lm_r;         // temperature limit
    OMX_S16 sigma_T_amb_r;  // scaling factor for ambient temperature
    OMX_S16 b_tv_r;         // voice coil thermal model coefficient
    OMX_S16 a_tv_r;         // voice coil thermal model coefficient
    OMX_S16 b_tm_r;         // magnet thermal model coefficient
    OMX_S16 a_tm_r;         // magnet thermal model coefficient
    OMX_S16 frame_length_r; // length of audio frame
    OMX_S16 t_mp_r;         // scaling factor for other temperatures
    OMX_S16 a_1_x_d_r;
    OMX_S16 a_2_x_d_r;
    OMX_S16 b_2_x_d_r;
    OMX_S16 b_1_u_d_r;
    OMX_S16 b_2_u_d_r;
    OMX_S32 alfa_r;
    OMX_S32 beta_r;
    OMX_S16 ext_gain_offset_r;
    OMX_S16 low_boost_comp_gain_r;
    OMX_S16 therm_prediction_rate_r;
    OMX_S16 dummy2;		  // assure even addresses for mmdsp
    OMX_S32 t_rav_l_r;      // Reserved for possible future use
    OMX_S16 t_att_r;        // T_ATT
    OMX_S16 t_att1_r;       // T_ATT1
    OMX_S16 t_rel1_r;       // T_REL1
    OMX_S16 t_rel2_r;       // T_REL2
    OMX_S16 t_rel3_r;       // T_REL3
    OMX_S16 t_ch_lim_r;     // Replaces hardcoded number 25
    //generic
    OMX_S16 imode;
    OMX_S16 ivolume;
    OMX_S16 itemperature;
} AFM_AUDIO_CONFIG_LSPROTECTIONTYPE;


typedef enum AFM_SRC_MODE {
    AFM_SRC_MODE_NORMAL,
    AFM_SRC_MODE_CUSTOM1,
    AFM_SRC_MODE_CUSTOM2,
    AFM_SRC_MODE_CUSTOM3,
    AFM_SRC_MODE_CUSTOM4,
    AFM_SRC_MODE_CUSTOM5,
    AFM_SRC_MODE_CUSTOM6,
    AFM_SRC_MODE_CUSTOM7,
    AFM_SRC_MODE_CUSTOM8,
}AFM_SRC_MODE;


typedef struct  AFM_AUDIO_PARAM_SRC_MODE {
  OMX_U32 nSize;
  OMX_VERSIONTYPE nVersion;
  OMX_U32 nPortIndex;
  AFM_SRC_MODE nSrcMode;
} AFM_AUDIO_PARAM_SRC_MODE;


// Alsasink

typedef enum OMX_AUDIO_ALSASINK_CT {
	OMX_AUDIO_ALSASINK_CT_REFER,
	OMX_AUDIO_ALSASINK_CT_IEC60958_PCM,
	OMX_AUDIO_ALSASINK_CT_AC3,
	OMX_AUDIO_ALSASINK_CT_MPEG1,
	OMX_AUDIO_ALSASINK_CT_MP3,
	OMX_AUDIO_ALSASINK_CT_MPEG2,
	OMX_AUDIO_ALSASINK_CT_AAC,
	OMX_AUDIO_ALSASINK_CT_DTS,
	OMX_AUDIO_ALSASINK_CT_ATRAC,
	OMX_AUDIO_ALSASINK_CT_ONE_BIT_AUDIO,
	OMX_AUDIO_ALSASINK_CT_DOLBY_DIGITAL,
	OMX_AUDIO_ALSASINK_CT_DTS_HD,
	OMX_AUDIO_ALSASINK_CT_MAT,
	OMX_AUDIO_ALSASINK_CT_DST,
	OMX_AUDIO_ALSASINK_CT_WMA_PRO,
} OMX_AUDIO_ALSASINK_CT;

typedef struct  OMX_AUDIO_PARAM_ALSASINK {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_AUDIO_ALSASINK_CT coding_type;
} OMX_AUDIO_PARAM_ALSASINK;


typedef struct AFM_PARAM_HOST_MPC_SYNC_TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL enabled;
} AFM_PARAM_HOST_MPC_SYNC_TYPE;


typedef enum AFM_MEMORY_TYPE {
    AFM_MEMORY_TYPE_SDRAM,
    AFM_MEMORY_TYPE_ESRAM,
    AFM_MEMORY_TYPE_TCMX,
    AFM_MEMORY_TYPE_TCMY
} AFM_MEMORY_TYPE;

/**
* Desired memory type for the binding associated
* with this port, if any. This property needs to be
* set on the component that owns the binding, in case
* two components are connected using deep tunneling.
* If enough memory of the specified type is not available,
* SDRAM is silently used instead.
*/
typedef struct AFM_PARAM_BINDING_MEMORY_TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    AFM_MEMORY_TYPE memoryType;
} AFM_PARAM_BINDING_MEMORY_TYPE;

typedef struct AFM_AUDIO_PARAM_CSCALL_MPC {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U16 nDuration; //in micro seconds
    OMX_U16 nSamplingRate;
    OMX_U16 nLoopbackMode;
    OMX_U16 nMspMode;
    OMX_U16 nRefPortStatus;
} AFM_AUDIO_PARAM_CSCALL_MPC;

typedef struct AFM_AUDIO_PARAM_MDRC {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BU32 nFilterType;
} AFM_AUDIO_PARAM_MDRC;

typedef struct AFM_AUDIO_GLOBAL_CONFIG_MDRC {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32  nPortIndex;
    OMX_U32  nMode;
    OMX_BOOL bLimiterRmsMeasure;
    OMX_S32  sLimiterThresh;
    OMX_BU32 nBiquadOrder;              // meaningless if nFilterType != Butterworth
    OMX_BU32 nFIRSize;                  // meaningless if nFilterType != FIR
    OMX_BOOL bFIRGainOptimization;      // meaningless if nFilterType != FIR
} AFM_AUDIO_GLOBAL_CONFIG_MDRC;

typedef struct AFM_AUDIO_PARAM_RENDERER {
  OMX_U32              nSize;
  OMX_VERSIONTYPE      nVersion;
  OMX_U16 nBoardRC;
} AFM_AUDIO_PARAM_RENDERER;

typedef struct AFM_AUDIO_PARAM_SPEECH_PROC {
  OMX_U32              nSize;
  OMX_VERSIONTYPE      nVersion;
  OMX_BOOL bBypassUplink;
  OMX_BOOL bBypassDownlink;
  OMX_BOOL bRealTimeReference;
  OMX_BOOL bOutputReference;
} AFM_AUDIO_PARAM_SPEECH_PROC;

typedef struct AFM_AUDIO_CONFIG_PCM_PROBE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bEnable;
    OMX_U32  nProbeIdx;
    OMX_U32  nPortIndex;
} AFM_AUDIO_CONFIG_PCM_PROBE;

typedef struct AFM_AUDIO_CONFIG_STEREO_CAPTURE 
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bMdrcEnable;
    OMX_BOOL bNoiseReductionEnable;
    OMX_BOOL bStereoWidenerEnable;
} AFM_AUDIO_CONFIG_STEREO_CAPTURE;

#endif // _AFM_TYPES_H_


