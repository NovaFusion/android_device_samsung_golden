/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */
/*! \file ste_adm_client.h
*   \brief Exports STE ADM client functionality.

    This header exports all functionality needed for clients to
    communicate with the STE ADM server process.
*/
#ifndef STE_ADM_CLIENT_INCLUSION_GUARD_H
#define STE_ADM_CLIENT_INCLUSION_GUARD_H

#include <stdint.h>

#ifndef STE_ADM_EXPORT
  #define STE_ADM_EXPORT __attribute__((visibility("default")))
#endif

/* Clients should be able to use C++ as well as C */
#ifdef __cplusplus
extern "C" {
#endif

/**
* Type of comfort tone according to 3GPP and GSM standards.
* Comfort tones are further described in ETSI TS 100 512 V7.0.1
* (GSM 02.40 version 7.0.1 Release 1998).
*/
typedef enum {
  STE_ADM_COMFORT_TONE_DIAL = 0x20,                 /** See ETSI TS 100 512 V7.0.1 */
  STE_ADM_COMFORT_TONE_SUBSCRIBER_BUSY,             /** See ETSI TS 100 512 V7.0.1 */
  STE_ADM_COMFORT_TONE_CONGESTION,                  /** See ETSI TS 100 512 V7.0.1 */
  STE_ADM_COMFORT_TONE_RADIO_PATH_ACKNOWLEDGE,      /** See ETSI TS 100 512 V7.0.1 */
  STE_ADM_COMFORT_TONE_RADIO_PATH_NOT_AVAILABLE,    /** See ETSI TS 100 512 V7.0.1 */
  STE_ADM_COMFORT_TONE_ERROR_SPECIAL_INFORMATION,   /** See ETSI TS 100 512 V7.0.1 */
  STE_ADM_COMFORT_TONE_CALL_WAITING,                /** See ETSI TS 100 512 V7.0.1 */
  STE_ADM_COMFORT_TONE_RINGING                      /** See ETSI TS 100 512 V7.0.1 */
  } ste_adm_comfort_tone_t;

/**
* These types indicate different standards according to
* 3GPP TS 22.001 V5.0.0 for how comfort tones should be played.
*/
typedef enum {
  STE_ADM_COMFORT_TONE_STANDARD_JAPAN = 0x30, /** Japanese standard for comfort tones.  */
  STE_ADM_COMFORT_TONE_STANDARD_ANSI,         /** ANSI standard for comfort tones.  */
  STE_ADM_COMFORT_TONE_STANDARD_CEPT          /** CEPT standard for comfort tones.  */
  } ste_adm_comfort_tone_standard_t;

/**
 * TTY modes
 */
typedef enum {
  STE_ADM_TTY_MODE_OFF = 0,
  STE_ADM_TTY_MODE_FULL,
  STE_ADM_TTY_MODE_HCO,
  STE_ADM_TTY_MODE_VCO,
  } ste_adm_tty_mode_t;

/**
 * Dictaphone modes
 */
typedef enum {
  STE_ADM_DICTAPHONE_UPLINK_DOWNLINK = 0,
  STE_ADM_DICTAPHONE_UPLINK,
  STE_ADM_DICTAPHONE_DOWNLINK,
  } ste_adm_dictaphone_mode_t;

/**
* Format of audio data sent to the ADM.
*
* The following numerical values are part of the interface (so a direct cast
* can be used).
*   1=mono 16-bit
*   2=stereo 16-bit interleaved, order L R
*   6=5.1 audio, order LF RF LFE CF LS RS
*
*/
typedef enum
{
  STE_ADM_FORMAT_INVALID = 0,
  STE_ADM_FORMAT_MONO    = 1,
  STE_ADM_FORMAT_STEREO  = 2,
  STE_ADM_FORMAT_HDMI5_1 = 6,
  STE_ADM_FORMAT_FIRST_CODED = 0x1000,
  STE_ADM_FORMAT_AC3     = 0x1000,
  STE_ADM_FORMAT_MPEG1   = 0x1001,
  STE_ADM_FORMAT_MP3     = 0x1002,
  STE_ADM_FORMAT_MPEG2   = 0x1003,
  STE_ADM_FORMAT_AAC     = 0x1004,
  STE_ADM_FORMAT_DTS     = 0x1005,
  STE_ADM_FORMAT_ATRAC   = 0x1006,
  STE_ADM_FORMAT_OBA     = 0x1007,
  STE_ADM_FORMAT_DDPLUS  = 0x1008,
  STE_ADM_FORMAT_DTS_HD  = 0x1009,
  STE_ADM_FORMAT_MAT     = 0x100A,
  STE_ADM_FORMAT_DST     = 0x100B,
  STE_ADM_FORMAT_WMA_PRO = 0x100C
} ste_adm_format_t;


// ADM error codes.
// Clients must be prepared to handle errors NOT listed in this
// table, to make it possible to add errors as need arises through
// error corrections, change requests, changes in lower layers,
// or other reasons.
typedef enum
{
  STE_ADM_RES_OK = 0,
  STE_ADM_RES_UNRECOVERABLE_ERROR = -1,
  STE_ADM_RES_IL_ERROR = -2,
  STE_ADM_RES_INTERNAL_ERROR = -3,
  STE_ADM_RES_ALL_IN_USE = 4,
  STE_ADM_RES_DEV_LIMIT_REACHED = -5,
  STE_ADM_RES_DEV_PER_SESSION_LIMIT_REACHED = -6,
  STE_ADM_RES_ERR_MALLOC = -7,   // malloc()/calloc()/realloc() failed (out of mem or address space exhausted or limit reached)
  STE_ADM_RES_ERR_BASIC_RESOURCE = -8, // something like pthread_mutex_init() has failed
  STE_ADM_RES_ERR_MSG_IO = -9,   // A write to a fifo failed, or msg corrupt, or something like epoll failed. Sometimes this can be reported.
  STE_ADM_RES_ERR_TOO_MANY_CLIENTS = -10,
  STE_ADM_RES_CHAIN_TOO_LONG = -11, // The chain length was exceeded
  STE_ADM_RES_UNKNOWN_IL_INDEX_NAME = -12, // The index name in the DB could not be resolved to numerical value
  STE_ADM_RES_IL_UNSUPPORTED_INDEX = -13, // SetParam / SetConfig returned OMX_ErrorUnsupportedIndex
  STE_ADM_RES_IL_UNKNOWN_ERROR = -14, // TODO: How to do with these? Return most of them instead...
  STE_ADM_RES_IL_INCONSISTENT = -15, // Assumption about IL component not met
  STE_ADM_RES_UNKNOWN_DATABASE_ERROR = -16,
  STE_ADM_RES_DEVICE_NAME_TOO_LONG = -17,
  STE_ADM_RES_INVALID_PARAMETER = -18,  // Invalid parameter
  STE_ADM_RES_DB_FATAL = -19, // Fatal db error, retrying probably wont help, ADM should be rebooted
  STE_ADM_RES_DB_INCONSISTENT = -20, // Database inconsistency
  STE_ADM_RES_NO_SUCH_DEVICE = -21,  // The requested device is not available in the database
  STE_ADM_RES_INVALID_CHANNEL_CONFIG = -22, // The specified number of channels are not supported
  STE_ADM_RES_NO_SUCH_SPEECH_CONFIG = -23, // the given in-dev, out-dev and sample rate does not exist
  STE_ADM_RES_INCORRECT_STATE = -24,
  STE_ADM_RES_INVALID_BUFFER_SIZE = -25,
  STE_ADM_RES_PARTIAL_CSCALL_NOT_SUPPORTED = -26, // can't have cscall with just input, or just output device.
  STE_ADM_RES_IL_INSUFFICIENT_RESOURCES = -27,
  STE_ADM_RES_CSCALL_DISABLED = -28, // CSCall functionality has been disabled, likely due to modem problems
  STE_ADM_RES_ALSA_ERROR = -29,
  STE_ADM_RES_NO_SUCH_COMPONENT = -30, // The specified component was not found
  STE_ADM_RES_DEVICE_CLOSED = -31,     // The device you were monitoring has been closed
  STE_ADM_RES_DEVICE_ALREADY_OPEN = -32,
  STE_ADM_RES_CSCALL_NOT_ACTIVE = -33,  // When trying to read/write from dictaphone when cscall is not up and running,
                                // or trying other operation that depends on cscall being up.
  STE_ADM_RES_CSCALL_FINISHED = -34,    // CSCall terminated, dictaphone record/playback done
  STE_ADM_RES_SETTINGS_NOT_COMPATIBLE = -35, // Tried to do live remap with too big differences between device settings
  STE_ADM_RES_DEVICE_RECONFIGURING = -36, // Tried to access device which is currently being reconfigured

  // Return values for INTERNAL USE ONLY
  STE_ADM_RES_NO_MORE = 1      // INTERNAL USE - NOT AN ERROR: No more db entries available
} ste_adm_res_t;

STE_ADM_EXPORT const char* ste_adm_res_to_str(ste_adm_res_t res);


/**
* Device identifier string for Speaker.
*/
#define STE_ADM_DEVICE_STRING_SPEAKER "Speaker"
/**
* Device identifier string for Handset out.
*/
#define STE_ADM_DEVICE_STRING_HSOUT "HSetOut"
/**
* Device identifier string for analog USB out.
*/
#define STE_ADM_DEVICE_STRING_AUSBOUT "AUSBOut"
/**
* Device identifier string for Carkit out.
*/
#define STE_ADM_DEVICE_STRING_CARKITOUT "CarKitOut"
/**
* Device identifier string for Earpiece.
*/
#define STE_ADM_DEVICE_STRING_EARP "EarPiece"
/**
* Device identifier string for Bluetooth out.
*/
#define STE_ADM_DEVICE_STRING_BTOUT "BTOut"
/**
* Device identifier string for Bluetooth Wideband (16kHz) out.
*/
#define STE_ADM_DEVICE_STRING_BTWBOUT "BTWBOut"
/**
* Device identifier string for A2DP out.
*/
#define STE_ADM_DEVICE_STRING_A2DP "A2DPOut"
/**
* Device identifier string for FM transmitter.
*/
#define STE_ADM_DEVICE_STRING_FMTX "FMTX"
/**
* Device identifier string for HDMI.
*/
#define STE_ADM_DEVICE_STRING_HDMI "HDMI"
/**
* Device identifier string for Vibra Left Output.
*/
#define STE_ADM_DEVICE_STRING_VIBL "VibraL"
/**
* Device identifier string for Vibra Right Output.
*/
#define STE_ADM_DEVICE_STRING_VIBR "VibraR"
/**
* Device identifier string for TTYOut.
*/
#define STE_ADM_DEVICE_STRING_TTYOUT "TTYOut"
/**
* Device identifier string for Microphone.
*/
#define STE_ADM_DEVICE_STRING_MIC "Mic"
/**
* Device identifier string for Speaker
*/
#define STE_ADM_DEVICE_STRING_HSIN "HSetIn"
/**
* Device identifier string for Bluetooth in.
*/
#define STE_ADM_DEVICE_STRING_BTIN "BTIn"
/**
* Device identifier string for Bluetooth Wideband (16kHz) in.
*/
#define STE_ADM_DEVICE_STRING_BTWBIN "BTWBIn"
/**
* Device identifier string for FM receiver.
*/
#define STE_ADM_DEVICE_STRING_FMRX "FMRX"
/**
* Device identifier string for TTYIn.
*/
#define STE_ADM_DEVICE_STRING_TTYIN "TTYIn"
/**
* Device identifier string for dictaphone recording.
*/
#define STE_ADM_DEVICE_STRING_DICT_REC "x-ste-adm-dict-rec"
/**
* Device identifier string for dictaphone playback.
*/
#define STE_ADM_DEVICE_STRING_DICT_PLAY "x-ste-adm-dict-play"
/**
* Device identifier string for VoIP input.
*/
#define STE_ADM_DEVICE_STRING_VOIPIN "VoIPIn"
/**
* Device identifier string for VoIP output.
*/
#define STE_ADM_DEVICE_STRING_VOIPOUT "VoIPOut"
/**
* Device identifier string for USBHeadset input.
*/
#define STE_ADM_DEVICE_STRING_USBIN "USBIn"
/**
* Device identifier string for USBHeadset output.
*/
#define STE_ADM_DEVICE_STRING_USBOUT "USBOut"
/**
* Device identifier string for Analog input.
*/
#define STE_ADM_DEVICE_STRING_LINEIN "LineIn"

/**
* Maximum number of characters in device string. The length
* does *NOT* include the terminating \0, so the define
* a string variable, you need to write:
* char name[STE_ADM_MAX_DEVICE_NAME_LENGTH+1];
*
*/
#define STE_ADM_MAX_DEVICE_NAME_LENGTH (63)

/**
* Flags for which level of audio routing shall be used for the device to device connection.
* Only one of LOGICAL, BUS or HW should be chosen. The routing choice is interpreted as a hint only
* unless the FORCED flag is set.
*/
typedef enum {
    STE_ADM_D2D_FLAG_LOGICAL = (1<<0),
    /**<Route connection at a logical level OMX level. */
    STE_ADM_D2D_FLAG_BUS = (1<<1),
    /**<Route connection at Data BUS level. */
    STE_ADM_D2D_FLAG_HW = (1<<2),
    /**<Route connection on the same HW chip. */
    STE_ADM_D2D_FLAG_FORCED = (1<<3),
    /**<Don't accept any other routing than the one flagged */
    STE_ADM_D2D_FLAG_FORCED_LOGICAL = (STE_ADM_D2D_FLAG_LOGICAL | STE_ADM_D2D_FLAG_FORCED),
    /**<Route connection at a logical level OMX level. Other routings not acceptable. */
    STE_ADM_D2D_FLAG_FORCED_BUS = (STE_ADM_D2D_FLAG_BUS | STE_ADM_D2D_FLAG_FORCED),
    /**<Route connection at Data BUS level. Other routings not acceptable. */
    STE_ADM_D2D_FLAG_FORCED_HW = (STE_ADM_D2D_FLAG_HW | STE_ADM_D2D_FLAG_FORCED),
    /**<Route connection on the same HW chip. Other routings not acceptable. */
    STE_ADM_D2D_FLAG_ROUTING_IMPOSSIBLE = (1<<4),
    /**< The routing is not possible for any logical level, used for output only. */
} ste_adm_dev2dev_flags_t;


/**
 * Effect chain identifiers.
 */
typedef enum {
    STE_ADM_APPLICATION_CHAIN,
    STE_ADM_VOICE_CHAIN,
    STE_ADM_COMMON_CHAIN
} ste_adm_effect_chain_type_t;

typedef struct
{
    char                    dev_name[STE_ADM_MAX_DEVICE_NAME_LENGTH+1];
    int                     voice_active;
    int                     app_active;
    int                     voicecall_samplerate;
} active_device_t;

typedef struct
{
    int in_samplerate;
    int in_channels;
    int out_samplerate;
    int out_channels;
} ste_adm_voip_mode_t;


/**
* Establish a connection to the STE ADM server process. This call must be
* successfully completed before any communication takes place. Once a connection
* is established, the STE ADM server process guarantees that it will not be
* broken unless requested by the client.
*
* @return client id to be used in further communication, negative error
          code if not successful.
*/
int STE_ADM_EXPORT ste_adm_client_connect(void);

/**
* Disconnect from the server. It is very important to do this when done with the
* connection. This is due to the fact that the server has a limit on the number
* of concurrently connected clients.
*
* @param client_id ID of the client to disconnect. This ID is returned from
*        ste_adm_client_connect.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_connect
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_disconnect(int client_id);

/**
* Send PCM audio data to the ADM server.
*
* @param client_id    ID of the client from which the PCM audio data is sent.
*                     This ID is returned from ste_adm_client_connect.
* @param buf_idx      Index of the buffer to transfer. This will let ADM know
*                     where in the shared memory buffer the data shall be read.
*                     Valid values are: 0 <= buf_idx < num_bufs, where num_bufs
*                     is specified when calling ste_adm_client_open_device().
* @param data_size    The size of the PCM audio data that should be
*                     transferred.
* @param lpa_mode     Out parameter, returns true if LPA is active.
*
* @return 0 on success, negative error code on failure.
* @see ste_adm_client_connect
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_send(int client_id, int buf_idx,
                                                 int data_size, int *lpa_mode);

/**
* Receive PCM audio data from the ADM server.
*
* @param client_id      ID of the client that has requested the PCM audio data.
*                       This ID is returned from ste_adm_client_connect.
* @param buf_idx        Out parameter, index of the returned buffer. Its value
*                       is only valid in case the call is successfull.
*
* @return 0 on success, negative error code on failure.
* @see ste_adm_client_connect
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_receive(int client_id, int* buf_idx);

/**
* Open an ADM device.
*
* A pointer to a shared memory buffer will be returned in bufp. The size
* of this shared memory is bufsz * num_bufs. The first buffer is at
* bufp, the second is at bufp + 1 * bufsz, etc.
* When you're finished with the device, you must unmap the shared memory
* by calling munmap(bufp, bufsz * num_bufs), otherwise the shared memory
* region will remain in your process.
*
* @param client_id    ID of the client for which the device should be opened.
*                     This ID is returned from ste_adm_client_connect.
* @param device_name  Name of device, matching a device name in the database.
* @param samplerate   Desired sampelrate in Hz
* @param format       Sample format, number of channels, channel order etc
* @param actual_samplerate Actual samplerate in Hz.
* @param bufp         Pointer to the shared memory pointer.
* @param bufsz        Size of each buffer.
* @param num_bufs     Number of buffers
*
* @return negative error code on failure.
*           ID that identifies the connection on the server on succes.
*
* @see ste_adm_client_connect
*
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_open_device(int client_id, const char* device_name,
    int samplerate, ste_adm_format_t format, int* actual_samplerate, char** bufp, int bufsz, int num_bufs);



/**
* Tell the ADM to close a device previously opened with
* ste_adm_client_open_device.
*
* @param client_id  ID of the client for which the device should be closed.
*                   This ID is returned from ste_adm_client_connect.
*
* @return 0 on success, negative error code on failure.
* @see ste_adm_client_connect
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_close_device(int client_id, const char* device_name);


typedef enum
{
    STE_ADM_VC_MODEM_STATUS_DISCONNECTED = 0,
    STE_ADM_VC_MODEM_STATUS_CONNECTED,
} ste_adm_vc_modem_status_t;

/**
* Start playing the specified comfort tone. The tone will be played through Android,
* using the android::AudioSystem::NOTIFICATION audio type. If the tone has unlimited
* length it will play indefinitely until a call to ste_adm_stop_comfort_tone() is made.
* If a call to ste_adm_start_comfort_tone() is made when a comfort tone is
* already active, the new comfort tone will directly replace the current comfort tone.
* There is no stacking of calls to ste_adm_start_comfort_tone() - the only ADM state
* is which tone is currently playing, if any. Even if you make several calls to
* ste_adm_start_comfort_tone(), you just need to call ste_adm_stop_comfort_tone()
* once. When the ADM starts, no comfort tone is playing.
* The type of tone standard to use must match the setting in gsm.operator.iso-country.
* This interface will change in the future to use a built-in tone generator
* in the ADM.
*
* The call is blocking. If the ADM is not active, the call will block.
*
* @param tone_standard  Comfort tone standard to use
* @param tone_type      Type of tone to play
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_stop_comfort_tone
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_start_comfort_tone(ste_adm_comfort_tone_standard_t tone_standard,
                               ste_adm_comfort_tone_t          tone_type);

/**
* Stop the current comfort tone, if any. Calling this function without any
* ongoing comfort tone playback will not cause it to fail. For more info,
* see ste_adm_start_comfort_tone(). The active comfort tone will stop
* immediately - it does not keep playing in order to finish the current
* 'tone sequence' at a specific point.
*
* The call is blocking. If the ADM is not active, the call will block.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_start_comfort_tone
*/
int STE_ADM_EXPORT ste_adm_stop_comfort_tone();


/**
* Set the cscall devices that will be used for voicecall whenever
* the modem reports that a voicecall is available.
*
* The modem voicecall start/stop mechanism is asynchronous with
* this call. There is no mechanism to report errors that occur
* while setting up the graph.
*
* To move the voicecall, use this API with a new set of devices.
* To hang up, use this API with both devices NULL or "".
* On ADM boot, both devices are set to NULL.
*
* Specifying one device as NULL/"" and the other as a proper
* device name is not supported.
*
* Note that while a set of devices is specified, the voicecall
* audio paths will be opened/closed multiple times as the modem
* reports that the voicecall is activated/deactivated.
*
* @return ste_adm_res_t
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_set_cscall_devices(int client_id, const char* indev, const char* outdev);

/**
* Mute the cscall upstream (e.g. microphone end on local headset).
*
* The mute setting defaults to OFF when the ADM is booted. The setting is
* not persistent - i.e. it does not survive an ADM restart. The call can
* be made even if voicecall is not active. The call is blocking and will
* not return until completed. The mute operation is done at the start of the
* vc chain - other components connected to the same audio device
* are not affected.
*
* The mute status controlled by this call is not affected by the
* ste_adm_client_set_cscall_upstream_volume() call. The reason for two
* separate calls is that there are two clients in Android that are unaware
* of each other that set voicecall volume and mute status.
*
* @param enable_mute  if 0, mute is disabled. if 1, mute is enabled.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_get_cscall_upstream_mute
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_upstream_mute(int enable_mute);


/**
* Mute the cscall downstream (e.g. speaker end of local headset).
*
* The mute setting defaults to OFF when the ADM is booted. The setting is
* not persistent - i.e. it does not survive an ADM restart. The call can
* be made even if voicecall is not active. The call is blocking and will
* not return until completed. The mute operation is done at the end of the
* vc chain - other components connected to the same audio device
* are not affected.
*
* Comfort tones requested via ste_adm_start_comfort_tone() are not affected
* by mute settings.
*
* NOTE: The mute status controlled by this call is not affected by the
* ste_adm_client_set_cscall_downstream_volume() call. The reason for two
* separate calls is that there are two clients in Android that are unaware
* of each other that set voicecall volume and mute status.
*
* @param enable_mute  if 0, mute is disabled. if 1, mute is enabled.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_get_cscall_downstream_mute
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_downstream_mute(int enable_mute);


/**
* Get the mute status for upstream voicecall. For details, see
* ste_adm_client_set_cscall_upstream_mute(). The call is blocking
* and will not return until completed.
*
* @param mute_enabled  If 0, mute is disabled. if 1, mute is enabled.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_set_cscall_upstream_mute
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_cscall_upstream_mute(int* mute_enabled);

/**
* Get the mute status for downstream voicecall. For details, see
* ste_adm_client_set_cscall_downstream_mute(). The call is blocking
* and will not return until completed.
*
* @param mute_enabled  If 0, mute is disabled. if 1, mute is enabled.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_set_cscall_upstream_mute
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_cscall_downstream_mute(int* mute_enabled);


/**
* Set the cscall downstream (e.g. speaker end of local headset) volume.
*
* The volume setting defaults to max volume (0 dB) when the ADM is booted.
* The setting is not persistent - i.e. it does not survive an ADM restart.
* The call can be made even if voicecall is not active. The call is
* blocking and will not return until completed. The volume operation is
* done at the end of the vc chain - other components connected to
* the same audio device are not affected.
*
* Comfort tones requested via ste_adm_start_comfort_tone() are not affected
* by volume settings.
*
* If the underlaying device does not support the full volume range, the
* requested value will be capped. ste_adm_client_get_cscall_downstream_volume
* will still return the uncapped value (note that a different VC device
* in the future might support a larger volume range).
*
* NOTE: The volume status controlled by this call is not affected by the
* ste_adm_client_set_cscall_downstream_mute() call. The reason for two
* separate calls is that there are two clients in Android that are unaware
* of each other that set voicecall volume and mute status.
*
* @param volume     Volume, in millibells. 100 mB = 1 dB. Range
*                   is [-10000, 0]. In addition INT_MIN can be used;
*                   it will result in a mute.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_get_cscall_downstream_mute
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_downstream_volume(int volume);

ste_adm_res_t STE_ADM_EXPORT ste_adm_client_init_cscall_downstream_volume(int min, int max);

/**
* Set the cscall upstream volume (e.g. microphone end on local headset).
*
* The volume setting defaults to max volume (0 dB) when the ADM is booted.
* The setting is not persistent - i.e. it does not survive an ADM restart.
* The call can be made even if voicecall is not active. The call is
* blocking and will not return until completed. The volume operation is
* done at the start of the vc chain - other components connected to
* the same audio device are not affected.
*
* Comfort tones requested via ste_adm_start_comfort_tone() are not affected
* by volume settings.
*
* If the underlaying device does not support the full volume range, the
* requested value will be capped. ste_adm_client_get_cscall_upstream_volume
* will still return the uncapped value (note that a different VC device
* in the future might support a larger volume range).
*
* NOTE: The volume status controlled by this call is not affected by the
* ste_adm_client_set_cscall_downstream_mute() call. The reason for two
* separate calls is that there are two clients in Android that are unaware
* of each other that set voicecall volume and mute status.
*
* @param volume     Volume, in millibells. 100 mB = 1 dB. Range
*                   is [-10000, 0]. In addition INT_MIN can be used;
*                   it will result in a mute.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_get_cscall_downstream_mute
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_upstream_volume(int volume);

/**
* Get the voicecall upstream volume.
*
* @param volume   Volume, in millibells. 100 mB = 1 dB. Range
*                 is [-10000, 0]. INT_MIN is returned if the
*                 volume is muted.
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_cscall_upstream_volume(int* volume);


/**
* Get the voicecall downstream volume.
*
* @param volume   Volume, in millibells. 100 mB = 1 dB. Range
*                 is [-10000, 0]. INT_MIN is returned if the
*                 volume is muted.
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_cscall_downstream_volume(int* volume);


/**
* Enable/disable the TTY.
* The TTY setting defaults to disabled when the ADM is booted. The setting is
* not persistent - i.e. it does not survive an ADM restart. The call can
* be made even if voicecall is not active. The call is blocking and will
* not return until completed.
*
* @param tty_mode      TTY mode to activate
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
* @see ste_adm_client_get_cscall_tty_mode
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_tty_mode(ste_adm_tty_mode_t tty_mode);

/**
* Get the TTY mode. See ste_adm_client_set_cscall_tty_mode for details.
* The call is blocking and will not return until completed.
*
* @param tty_mode      Current TTY mode
*
* @return 0 on success, negative error code on failure.
* @see ste_adm_client_set_cscall_tty_mode
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_cscall_tty_mode(ste_adm_tty_mode_t* tty_mode);

/**
* Enable/disable the VoIP mode.
* If VoIP is enabled the cscall graph is setup without connecting to the cscall
* component.
*
* @param enabled   VoIP mode enabled/disabled
* @param mode      samplerate and channels to use in the VoIP call
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_voip_mode(int enabled, ste_adm_voip_mode_t *mode);

/**
* Enable/disable cscall module loopback mode.
* Must not be called when voicecall is active.
*
* @param mode      loopback mode enable/disable
* @param codec      used to specify codec type when enabling loopback. not used when disabling loopback
*
* @return STE_ADM_RES_OK if ok. For more info, see ste_adm_res_t.
* @see ste_adm_client_set_cscall_loopback_mode
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_set_cscall_loopback_mode(int mode, int codec);

/**
* Set dictaphone mode.
* If dictaphone is enable during voice call, the graph will be set according to
* the mode speficed by the user. It will possible to capture the speech only in
* uplink or downlink or both uplink/downlink.
*
* @param mode       dictaphone mode uplink/downlink - 0, uplink - 1, downlink - 2.
*                   By default uplink/downlink are recorded.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_dictaphone_mode(ste_adm_dictaphone_mode_t mode);

/**
* Set dictaphone mute.
*
* @param enable_mute  if 0, mute is disabled. if 1, mute is enabled.
*                   by default dictaphone is unmuted.
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_cscall_dictaphone_mute(int enable_mute);

/**
* Get dictaphone mute status.
* @param mute_enabled  If 0, mute is disabled. if 1, mute is enabled.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_cscall_dictaphone_mute(int *mute_enabled);

/**
* Set the application chain master volume for the specified
* device. This does not affect the volume of the voicecall
* chain.
*
* The value is not persistent, and the call can only be issued when
* the device is opened. If the device is closed, the volume
* is reset to the default value.
*
* @param volume     Volume in millibells (1 dB = 100 mB). Range
*                   is [-10000,0] (but an even smaller range may be
*                   accepted by lower layers; value will be capped).
*                   In addition INT_MIN can be used; it will result in
*                   a mute.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_app_volume(const char* dev_name, int volume);

/**
* Get the application chain master volume for the specified
* device (see ste_adm_client_set_app_volume).
*
* The retrieved value represents the value that is actually applied
* by lower layers, and may differ from the value set by
* ste_adm_client_set_app_volume (e.g. it may be capped, or moved to
* a step value allowed by the lower layers).
*
* @param volume     Volume in millibells (1 dB = 100 mB). Range
*                   is [-10000,0]. In addition, INT_MIN can be
*                   returned if the volume is muted.
*
* @return STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_app_volume(const char* dev_name, int* volume);

/**
* Get the maximum output latency (ms) of all the devices opened by the specified
* client.
*
* @param     dev_name         Name of the device for which to retrieve the latency.
* @param     latency          The maximum output latency in milliseconds.
*
* @return     STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_max_out_latency(const char *device, int *latency);

/**
* Drain and stop all ouput devices associated with the specified client.
* @param     client_id        ID of the client for which to retrieve the latency.
* @return     STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_drain(int client_id);

/**
* Get the mapping from a given toplevel device to an actual device.
* @param    toplevel_device     NULL terminate string containing the name of
                                the toplevel device to get the mapping for.
* @param    actual_device       Pointer to a string where the name of the actual
                                device associated with the toplevel device will
                                be placed. Must have room for at least
                                STE_ADM_MAX_DEVICE_NAME_LENGTH+1 chars.
* @return   STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_toplevel_map(const char *toplevel_device, char *actual_device);

/**
* Set the mapping from a given toplevel device to an actual device.
* @param    toplevel_device     NULL terminate string containing the name of
                                the toplevel device to change the mapping for.
* @param    actual_device       NULL terminate string containing the name of
                                the actual device to associate the toplevel
                                device with.
* @return   STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_toplevel_map(const char *toplevel_device, const char *actual_device);

/**
* Set the mapping from a given toplevel device to an actual device, and
* rescan the effect settings from the database if the devices are active.
* Voicecall settings (speech proc) will also be re-scanned, if needed.
*
* The graph of the previous device and the new device must be compatible
* to allow update without disconnecting the graph. This mean that, for
* both devices (old device and new device), the following must be true:
*  - the settings for the sink/source must be the same in both devices
*  - both devices must contain the same effects, in the same order, in the
*    voicecall-, app- and common chains.
*  - for each effect:
*    - the set of parameters of OMX_SetParameter kind must be the same
*      for both devices
*    - the contents of the parameters of OMX_SetParameter kind must be
*      the same for both devices
*    - the set of parameters of OMX_SetConfig kind must be the same for
*      both devices
*
* For changes involving voicecall, it is sometimes necessary change
* two devices at the same time, since the intermediate states are
* not valid. For this purpose, the parameters toplevel_device2 and
* new_actual_device2 can be used. This is optional; these parameters can
* be set to NULL if you only want to change one device mapping.
*
* If the graphs of the previous device and the new device are not
* compatible the effect chains must be disconnected before setting up
* the graph of the new device. Since the graph must be disconnected,
* smooth transition between the device configurations is not supported
* in this case.
*
* @param    toplevel_device     Zero terminated string containing the name of
*                               the toplevel device to change the mapping for.
* @param    new_actual_device   Zero terminated string containing the name of
*                               the actual device to associate the toplevel
*                               device with.
* @param    toplevel_device2    Zero terminated string containing the name of
*                               the toplevel device to change the mapping for.
*                               If not used, set to NULL or "".
* @param    new_actual_device2  Zero terminated string containing the name of
*                               the actual device to associate the toplevel
*                               device with.
*                               If not used, set to NULL or "".
* @return   STE_ADM_RES_OK if ok. STE_ADM_RES_SETTINGS_NOT_COMPATIBLE will be
*           returned if the devices are found to not be liveremap-compatible
*           with the current settings. See ste_adm_res_t for other error codes.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_set_toplevel_map_live(const char *toplevel_device,  const char *new_actual_device,
                                                           const char *toplevel_device2, const char *new_actual_device2);


/**
* Get the toplevel device associated with the given actual device.
* @param    actual_device       NULL terminated string containing the name of
*                               the actual device for which the toplevel device
*                               should be retrieved.
* @param    toplevel_device     Pointer to a string where the name of the toplevel
*                               device associated with the actual device will
*                               be placed. Must have room for at least
*                               STE_ADM_MAX_DEVICE_NAME_LENGTH+1 chars.
* @return   STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_toplevel_device(const char *actual_device, char *toplevel_device);

/**
* Setup a device to device connection from the src to dst device
* using the flags supplied.
*
* @param src_dev The source device for the connection.
* @param dst_dev The destination device for the connection.
* @param flags Routing flags, will contain the resulting routing on output.
*
* @return client id to be used in further communication, negative error
          code if not successful.
*/
int STE_ADM_EXPORT ste_adm_dev2dev_connect(const char* src_dev, const char* dst_dev,
    ste_adm_dev2dev_flags_t *flags);

/**
* Disconnect a device to device connection.
*
* @param client_id The client handle
*
* @return client id to be used in further communication, negative error
          code if not successful.
*/
int STE_ADM_EXPORT ste_adm_dev2dev_disconnect(int client_id);

/**
* Set external delay for a device.
* The device is specified by Name in table Device of the database.
*
* @param    device          NULL terminated string containing name of the device
*                           for which the external delay should be changed.
*
* @param    external_delay  The new external delay value to be set.
*
* @return   STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_external_delay(const char *device, int external_delay);

/**
* Get the current sink latency.
*
* @param    dev_name        NULL terminated string containing name of the device
*                           for which to retrieve the sink latency.
*
* @param    latencyMs       Sink latency in milliseconds.
*
* @return   STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_sink_latency(const char *dev_name, uint32_t *latencyMs);

/**
* Set the pcm probe id state.
* @param    probe_id            The pcm probe id
* @param    enabled             The probe id state (1=enabled, 0=disabled)
* @return   STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_pcm_probe(int probe_id, int enabled);

/**
* Rescan database for the specified component.
* Experimental API; ***NOT SUPPORTED***
*
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_rescan_config(const char* dev_name,
    ste_adm_effect_chain_type_t chain_id, const char *component_name);

/**
* Rescan database for speechproc.
* Experimental API; ***NOT SUPPORTED***
*
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_rescan_speechproc();

/**
* Tear down the current voicecall devices and reolad them wiht new settings.
* No app device may be open on same physical device when this is done.
* Experimental API; ***NOT SUPPORTED***
*
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_reload_device_settings();

/**
 * Executes the SQL string in the ADM database.
 */
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_execute_sql(const char *sql);

/**
 * Sends a request for the active device list.
 * This function is not blocking. The client have to wait on the socket (client_id)
 * and then call ste_adm_client_read_active_device_list when data is available on the socket.
 */
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_request_active_device_list(int client_id);

/**
 * Reads the active device list. See ste_adm_client_request_active_device_list.
 */
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_read_active_device_list(int client_id, active_device_t **device_list_pp, int *nbr_devices_p);


/**
* Releases all 'static' resources in the ADM (it does not force close
* current sessions). Used during verification with Valgrind to check
* that there are no memory leaks.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_debug_shutdown();

/**
* Prints debug information to logcat.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_debug_dump_state();

/**
 * Reguest update for current modem voice call state. This function is not blocking.
 * The client have to use the function ste_adm_client_read_modem_vc_state for update of voice call state
 */
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_request_modem_vc_state(int client_id);

/**
 * Reads the modem vc_state, connected or disconnected. First call returns the current status, sequental calls
 * blocks until status is updated.
 */
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_read_modem_vc_state(int client_id, ste_adm_vc_modem_status_t *vc_status);

/**
 * Experimental.
 *
 */
ste_adm_res_t STE_ADM_EXPORT ste_adm_reopen_db();

typedef enum
{
    ADM_SLIM_MODEM                  // encoder/decoder Speech proc in APE side
    ,ADM_HALF_SLIM_MODEM            // encoder/decoder in Modem side and Speech proc in APE side
    ,ADM_FAT_MODEM                  // encoder/decoder Speech proc in Modem side
    ,ADM_FULL_FAT_MODEM             // no ape.
    ,ADM_UNKNOWN_MODEM_TYPE
} ste_adm_modem_type_t;

/**
* Get the modem type
*
* @param     modem_type          The modem type of type ste_adm_modem_type_t.
*
* @return     STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_get_modem_type(ste_adm_modem_type_t *modem_type);

/**
* Set tuning mode
*
* @param     enabled          Enables/disables tuning mode.
*
* @return     STE_ADM_RES_OK on success, negative error code on failure.
*/
ste_adm_res_t STE_ADM_EXPORT ste_adm_client_set_tuning_mode(int enabled);

#ifdef __cplusplus
}
#endif

#endif /* STE_ADM_CLIENT_INCLUSION_GUARD_H */

