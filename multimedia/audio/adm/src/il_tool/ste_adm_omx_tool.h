/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */
#ifndef INCLUSION_GUARD_STE_ADM_OMX_TOOL_H
#define INCLUSION_GUARD_STE_ADM_OMX_TOOL_H

#include "OMX_Component.h"
#include "OMX_Types.h"
#include "ste_adm_util.h"
#include "ste_adm_msg.h"
#include "ste_adm_dbg.h"
#include "audio_chipset_api_index.h"
#include "audio_render_chipset_api.h"
#include <pthread.h>
#include "OMX_CoreExt.h"

extern adm_sem_t g_adm_event_sem;

#define ADM_SEM_WAIT_WITH_PRINT(...) \
  do { adm_sem_wait_and_printf(&g_adm_event_sem, __FILE__, __LINE__, __VA_ARGS__); } while(0)

// All OMX AppData should be of this type

#define ADM_MAX_IO_PER_COMPONENT (9) // TODO name



#ifndef OMX_IO_MAX_BUFFERS
  #define OMX_IO_MAX_BUFFERS 16
#endif

// Drop first buffers corresponding to IL_TOOL_DROP_DATA_MS ms
// for Mic and HSetIn to avoid click/pop sound caused by mic regulator
#define IL_TOOL_DROP_DATA_MS 200

struct adm_il_appdata_s;
typedef struct adm_il_appdata_s* il_comp_t;

#include "ste_adm_omx_io.h"

// Return the OMX_HANDLETYPE wrapped in il_comp_t.
// This is useful for debug prints, since the component handle
// is the same as that used by e.g. ENS prints. However,
// internally in the ADM, il_comp_t should almost always be used - never
// the 'raw' IL handle.

ste_adm_res_t il_tool_set_buffer_time(il_comp_t comp, OMX_U32 port, unsigned int buffer_time, unsigned buffer_count);

OMX_HANDLETYPE il_tool_raw_handle(const il_comp_t comp);

int il_tool_io_is_idle(il_comp_t comp, OMX_U32 port);

void il_tool_io_feed(il_comp_t      comp,
                        OMX_U32        port,
                        msg_data_t* msg_p);

void il_tool_io_request_data(il_comp_t comp, OMX_U32 port, msg_data_t* msg_p);

void il_tool_io_wait_until_idle(il_comp_t comp, OMX_U32 port);

typedef void (*io_idle_callback_t)(void* param);
void il_tool_io_subscribe_idle_callback_once(il_comp_t comp, OMX_U32 port, io_idle_callback_t cb_fp, void* param);

int il_tool_io_alloc(il_comp_t comp, OMX_U32 port, int shm_fd_to_reuse);
int il_tool_io_dealloc(il_comp_t comp, OMX_U32 port);

unsigned int il_tool_get_bufidx(il_comp_t comp, OMX_U32 port);
void il_tool_set_bufidx(il_comp_t comp, OMX_U32 por, unsigned int idx);

typedef void (*il_tool_callback_t)(void* callback_param, il_comp_t hComponent,
        OMX_EVENTEXTTYPE eEvent,
        OMX_U32 Data1, OMX_U32 Data2, OMX_PTR pEventData);


/**
* Information about OMX buffers. Only ste_adm_omx_io.c should access
* the members of this struct.
*/
typedef struct {
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Only functions in ste_adm_omx_io.c should access the contents
    // of this struct!!!
    //
    // Try to keep the attributes here related to IL io only, and keep
    // other ADM data types out of here. We want this file to provide
    // nothing except I/O functionality.
    //
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    unsigned int            magic;

    pthread_cond_t  cond_idle; // associated mutex: adm_il_tool_state_t::mutex

    // Shared memory
    int                     shm_size;
    int                     shm_fd;
    char*                   shm_mmap_buf;

    int                     bufsz;

    OMX_BUFFERHEADERTYPE*   hdr_p[OMX_IO_MAX_BUFFERS];      /**< The list of buffer headers */
    unsigned int            buf_count;                     /**< The number of buffers */

    unsigned int            owned_by_adm;  // number of buffers owned by ADM
    unsigned int            next_buf_idx;  // empty: next buffer to send to IL
                                           // fill: next buffer that will be returned
    int                     fill_active;   // If the buffers in ADM are full
    int                     has_error;     // An error has occured when it could not be reported
    int                     eos_queued;

    msg_data_t* msg_p;         // current pending request. NULL if no request pending.
    int                     is_src_port;    // true if port is src (delivers data from it using FillThisBuffer)
    int                     idle_state_wanted;

    int                     drop_buf_enabled;   // true if buffers should be dropped to
                                                // avoid click/pop sound caused by mic regulator
    int                     drop_buf_cnt;       // number of dropped buffers
    int                     drop_buf_finished;  // true when all buffers are dropped

    io_idle_callback_t      idle_callback_fp;
    void*                   idle_callback_param;

    // Smoothing of data feed duration
    unsigned int            data_rate;      // Audio rate (bytes per second)
    uint64_t                cur_response_time_ms;
    uint64_t                time_entering_lpa_mpde;
} ste_adm_omx_ioinfo_t;



typedef struct
{
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // Only functions in ste_adm_omx_tool.c should access the contents
    // of this struct!!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // 0 if the component is not busy, != 0 if it is busy (=processing
    // command sent via OMX_SendCommand - change state or enable/disable port
    int cmd_in_progress;

    // The last error given to the component via the event callback, or
    // OMX_ErrorNone if no error has occured.
    OMX_ERRORTYPE last_error;
    OMX_U32       last_error_Data2;

    // Used for subscribed callbacks that should arrive in ADM context
    // if any (when not NULL)
    il_tool_callback_t event_callback_fp;
    void*              event_callback_param;

    int event_callback_must_be_serialized;

    int                     waiting_for_event;
    int                     waiting_for_event_idx;


    // OMX_MAX_STRINGNAME_SIZE includes terminating \0
    char component_name[OMX_MAX_STRINGNAME_SIZE];
    OMX_HANDLETYPE omx_handle;
} adm_il_tool_state_t;



// TODO: mutex?

// Struct common for all IL components created by the ADM.
typedef struct adm_il_appdata_s
{
    pthread_mutex_t mutex;

    // State related to callbacks (e.g. CmdDisablePort) and
    // error events. Must only be accessed by IL tool!!!!
    adm_il_tool_state_t callback_state; // TODO: Rename from callback_state

    // State of buffers used for FillThisBuffer/EmptyThisBuffer IO
    // Only access using functions defined in ste_adm_omx_io.h!!!!
    // Each is tied to a port; e.g. bufinfo[2] belongs to port 2
    ste_adm_omx_ioinfo_t bufinfo[ADM_MAX_IO_PER_COMPONENT];
} adm_il_appdata_t;


// WARNING: Do not use this function to subscribe on events from a component
// that also processes buffers - with the current implementation, all kinds of events
// are lost when il_tool_unsubscribe_events() is called.
void il_tool_subscribe_events(il_comp_t comp_h, il_tool_callback_t cb, void* callback_param, int must_be_serialized);
void il_tool_unsubscribe_events(il_comp_t comp_h);

void il_tool_io_init(ste_adm_omx_ioinfo_t *bufinfo_p);

#include "ste_adm_srv.h"



//
// IL_TOOL_-macros will try to execute a single IL command, or a sequence
// of IL commands.
//
// The macros use the symbol 'res', and will set it to an error code
// if the IL command fails. Also, if an error occurs, the IL_TOOL_-macro
// will goto the label 'cleanup'.
//
// The IL_TOOL_-macros excpect to be invoced as a worker thread.
//
// The IL_TOOL_INIT_CONFIG_STRUCT macro is different; it does not execute any
// IL commands (only sets the size and version of the supplied structure)
//

//
//
//
//
#define IL_TOOL_CREATE(__comp_ptr, __comp_name, __dbg_type) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_create_component(__comp_ptr, __comp_name, __dbg_type); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("TRY_IL_CREATE of %s failed\n", __comp_name); \
            goto cleanup; \
        } \
    } while(0)


//
//
//
#define IL_TOOL_DESTROY(__comp_ptr) \
    do { \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        il_tool_destroy_component(__comp_ptr); \
    } while(0)


//
//
//
#define IL_TOOL_DISABLE_PORT(__comp_handle, __port_index) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_change_port_state(__comp_handle, __port_index, 0, 0); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_DISABLE_PORT of %X.%d failed\n", __comp_handle, __port_index); \
            goto cleanup; \
        } \
    } while(0)

//
//
//
//
#define IL_TOOL_ENABLE_PORT(__comp_handle, __port_index) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_change_port_state(__comp_handle, __port_index, 1, 0); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_ENABLE_PORT of %X.%d failed\n", __comp_handle, __port_index); \
            goto cleanup; \
        } \
    } while(0)

//
//
//
//
#define IL_TOOL_DISABLE_ALL_PORTS_EXCEPT(__comp_handle, ...) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        OMX_U32 ports_to_not_disable[] = { __VA_ARGS__ }; \
        res = il_tool_disable_all_except(__comp_handle, ports_to_not_disable, sizeof(ports_to_not_disable) / sizeof(int)); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_DISABLE_ALL_EXCEPT of %X failed\n", __comp_handle); \
            goto cleanup; \
        } \
    } while(0)

//
//
//
//
#define IL_TOOL_DISABLE_ALL_PORTS(__comp_handle) \
    IL_TOOL_DISABLE_ALL_PORTS_EXCEPT(__comp_handle)

//
//
//
//
#define IL_TOOL_CONNECT_SRC_CFG(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_connect_and_enable(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port, 1, 1, 0, OMX_BufferSupplyUnspecified); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_CONNECT_SRC_CFG of %X failed\n", __comp1_handle); \
            goto cleanup; \
        } \
    } while(0)

//
//
//
//
#define IL_TOOL_CONNECT_DST_CFG(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_connect_and_enable(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port, 1, 1, 1, OMX_BufferSupplyUnspecified); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_CONNECT_DST_CFG of failed\n"); \
            goto cleanup; \
        } \
    } while(0)


//
// Disconnect the tunnel. Send both DisablePort commands, and wait for the semaphore.
// The first component should be the source and the second the sink, i.e. in the same
// order as SetupTunnel (and IL_CONNECT) expects them.
//
#define IL_TOOL_DISCONNECT(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_disconnect(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_DISCONNECT of %X.%d -> %X.%d failed\n", __comp1_handle, __comp1_port, __comp2_handle, __comp2_port); \
            goto cleanup; \
        } \
    } while(0)



//
// The component MUST be listed in order Source to Sink. The macro will internally
// make sure the state transitions are activated in the correct order depending
// on what state transition to make.
//
#define IL_TOOL_ENTER_STATE(__target_state, ...) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        il_comp_t components_to_transition[] = { __VA_ARGS__ }; \
        res = il_tool_enter_state(__target_state, components_to_transition, sizeof(components_to_transition) / sizeof(OMX_HANDLETYPE)); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_ENTER_STATE failed\n"); \
            goto cleanup; \
        } \
    } while(0)

//
//
//
//
#define IL_TOOL_CONNECT_AND_ENABLE_WITH_SRC_CFG(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_connect_and_enable(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port, 1, 1, 0, OMX_BufferSupplyUnspecified); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_CONNECT_AND_ENABLE_WITH_SRC_CFG failed\n"); \
            goto cleanup; \
        } \
    } while(0)

//
//
//
//
#define IL_TOOL_CONNECT_AND_ENABLE_WITH_DST_CFG(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_connect_and_enable(__comp1_handle, __comp1_port, __comp2_handle, __comp2_port, 1, 1, 1, OMX_BufferSupplyUnspecified); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_CONNECT_AND_ENABLE_WITH_DST_CFG failed\n"); \
            goto cleanup; \
        } \
    } while(0)

//
//
//
//
#define IL_TOOL_DISABLE_PORT_WITH_IO(__comp_handle, __comp_port) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_change_port_state(__comp_handle, __comp_port, 0, 1); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_DISABLE_PORT_WITH_IO failed\n"); \
            goto cleanup; \
        } \
    } while(0)


//
//
//
//
#define IL_TOOL_ENABLE_PORT_WITH_IO(__comp_handle, __comp_port) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        res = il_tool_change_port_state(__comp_handle, __comp_port, 1, 1); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_ENABLE_PORT_WITH_IO failed\n"); \
            goto cleanup; \
        } \
    } while(0)


//
//
//
//
#define IL_TOOL_SET_PARAM(__comp_handle, __port_index, __param) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
        res = il_tool_SetParameter(__comp_handle, __port_index, __param); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_SET_PARAM failed\n"); \
            goto cleanup; \
        } \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
    } while(0)

//
//
//
//
#define IL_TOOL_GET_PARAM(__comp_handle, __port_index, __param) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
        res = il_tool_GetParameter(__comp_handle, __port_index, __param); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_GET_PARAM failed\n"); \
            goto cleanup; \
        } \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
    } while(0)

//
//
//
//
#define IL_TOOL_SET_CONFIG(__comp_handle, __port_index, __param) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
        res = il_tool_SetConfig(__comp_handle, __port_index, __param); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_SET_CONFIG failed\n"); \
            goto cleanup; \
        } \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
    } while(0)

//
//
//
//
#define IL_TOOL_GET_CONFIG(__comp_handle, __port_index, __param) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
        res = il_tool_GetConfig(__comp_handle, __port_index, __param); \
        if (res != STE_ADM_RES_OK) { \
            ALOG_ERR("IL_TOOL_GET_CONFIG failed\n"); \
            goto cleanup; \
        } \
        ADM_ASSERT((__param)->nVersion.nVersion == OMX_VERSION); \
        ADM_ASSERT((__param)->nSize == sizeof(*__param)); \
    } while(0)


//
//
//
//
#define IL_TOOL_INIT_CONFIG_STRUCT(__il_config_struct) \
    do { \
        memset(&__il_config_struct, 0, sizeof(__il_config_struct)); \
        __il_config_struct.nSize             = sizeof(__il_config_struct); \
        __il_config_struct.nVersion.nVersion = OMX_VERSION; \
    } while(0)


#define IL_TOOL_SET_PORT_BUFFER_CONFIG(__comp_handle, __port_index, __buf_count, __buf_size) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        OMX_PARAM_PORTDEFINITIONTYPE portdef; \
        IL_TOOL_INIT_CONFIG_STRUCT(portdef); \
        portdef.nPortIndex = __port_index; \
        IL_TOOL_GET_PARAM(__comp_handle, OMX_IndexParamPortDefinition, &portdef); \
        if ((__buf_size) != 0)  { portdef.nBufferSize = (size_t) (__buf_size); } \
        if ((__buf_count) != 0) { portdef.nBufferCountActual = (size_t) (__buf_count); } \
        IL_TOOL_SET_PARAM(__comp_handle, OMX_IndexParamPortDefinition, &portdef); \
    } while(0)

#define IL_TOOL_SET_PORT_SYNCHRONIZED(__comp_handle, __port_index, __enabled) \
    do { \
        ADM_ASSERT(res == STE_ADM_RES_OK); \
        SRV_DBG_ASSERT_IS_WORKER_THREAD; \
        OMX_AUDIO_PARAM_SYNCHRONIZEDTYPE param; \
        IL_TOOL_INIT_CONFIG_STRUCT(param); \
        param.nPortIndex = __port_index; \
        param.bIsSynchronized = __enabled; \
        IL_TOOL_SET_PARAM(__comp_handle, OMX_IndexParamAudioSynchronized, &param); \
     } while(0)



ste_adm_res_t il_tool_create_component(il_comp_t* comp_hp, const char* name, const char* dbg_type);
void il_tool_destroy_component(il_comp_t* comp_hp);
ste_adm_res_t il_tool_change_port_state(il_comp_t comp_h, OMX_U32 port, int enabled, int alloc_bufs);
ste_adm_res_t il_tool_disable_all_except(il_comp_t comp_h, OMX_U32* ports_to_keep_enabled, int num_port_to_keep);

ste_adm_res_t il_tool_connect_and_enable(il_comp_t src_comp,
                                         OMX_U32 src_port, il_comp_t dst_comp,
                                         OMX_U32 dst_port, int enable_as_well, int propagate_cfg,
                                         int use_dst_cfg, OMX_BUFFERSUPPLIERTYPE supplier);
ste_adm_res_t il_tool_disconnect(il_comp_t src_comp, OMX_U32 src_port, il_comp_t dst_comp, OMX_U32 dst_port);

ste_adm_res_t il_tool_enter_state(OMX_STATETYPE target_state, il_comp_t* components, int num_components);
void il_tool_get_buffers(il_comp_t comp, OMX_U32 port, int* fd, int *bufsize, int* num_bufs);
void il_tool_reset_bufidx(il_comp_t comp, OMX_U32 port);

void il_tool_enable_trace(il_comp_t handle, OMX_U16 traceEnable, OMX_U32 parentHandle);

int ste_adm_omx_tool_handle_event_must_be_serialized(il_comp_t hComponent);
ste_adm_res_t il_tool_set_buffer_time(il_comp_t comp, OMX_U32 port, unsigned int buffer_time, unsigned buffer_count);

// Wait for OMX_EventIndexSettingChanged of specified index
// Data1 == <port> && Data2 == nIndex
ste_adm_res_t il_tool_set_config_and_wait_config_event(il_comp_t hComponent, OMX_INDEXTYPE nIndex, void* cfgData, OMX_INDEXTYPE nEventIndex);

void ste_adm_omx_tool_handle_event(il_comp_t hComponent,
        OMX_EVENTTYPE eEvent,
        OMX_U32 Data1, OMX_U32 Data2,
        OMX_PTR pEventData);

void il_tool_io_transfer_done(il_comp_t comp, OMX_U32 port, OMX_BUFFERHEADERTYPE* buf_p);

// Set last opened device
void il_tool_io_set_last_opened_dev(const char* last_dev);
// Get last opened device
const char* il_tool_io_get_last_opened_dev();

// Must be followed by waiting for the idle callback
// (il_tool_io_subscribe_idle_callback_once)
void il_tool_io_eos(il_comp_t comp, OMX_U32 port);
void il_tool_io_move_towards_idle(il_comp_t comp, OMX_U32 port);
void il_tool_save_state();
ste_adm_res_t il_tool_revert();

// Return != 0 if the IL component is a
// DSP component, 0 otherwise
int il_tool_is_dsp_component(il_comp_t comp);

// Causes an error to be returned if an IL state transition
// takes longer than the specified time. If timeout is
// 0 (the boot default), the ADM waits indefinitely, or
// until the recovery mechanism reboot the ADM.
void il_tool_set_state_transition_timeout(int timeout_sec);

// Per thread



ste_adm_res_t il_tool_CreateComponent(const char* c,
    il_comp_t* comp_hp);

void il_tool_DestroyComponent(
    il_comp_t comp_h);

ste_adm_res_t il_tool_SetConfig(
    il_comp_t comp_h,
    OMX_INDEXTYPE nIndex,
    const void* pComponentConfigStructure);

ste_adm_res_t il_tool_SetParameter(
    il_comp_t comp_h,
    OMX_INDEXTYPE nIndex,
    const void* pComponentParameterStructure);

ste_adm_res_t il_tool_SendCommand(
    il_comp_t comp_h,
    OMX_COMMANDTYPE Cmd,
    OMX_U32 nParam1,
    OMX_PTR pCmdData);

ste_adm_res_t il_tool_GetParameter(
    il_comp_t hComponent,
    OMX_INDEXTYPE nParamIndex,
    OMX_PTR pComponentParameterStructure);

ste_adm_res_t il_tool_GetConfig(
    il_comp_t hComponent,
    OMX_INDEXTYPE nIndex,
    OMX_PTR pComponentConfigStructure);

ste_adm_res_t il_tool_GetComponentVersion(
    il_comp_t hComponent,
    OMX_STRING pComponentName,
    OMX_VERSIONTYPE* pComponentVersion,
    OMX_VERSIONTYPE* pSpecVersion,
    OMX_UUIDTYPE* pComponentUUID);

ste_adm_res_t il_tool_GetState(
    il_comp_t hComponent,
    OMX_STATETYPE* pState);

ste_adm_res_t il_tool_SetupTunnel(
    il_comp_t hOutput,
    OMX_U32 nPortOutput,
    il_comp_t hInput,
    OMX_U32 nPortInput);

ste_adm_res_t il_tool_GetExtensionIndex(
    il_comp_t hComponent,
    const char* cParameterName,
    OMX_INDEXTYPE* pIndexType);


// Internal use

void il_tool_event_cb(il_comp_t hComponent,
        OMX_EVENTTYPE eEvent,
        OMX_U32 Data1, OMX_U32 Data2,
        OMX_PTR pEventData);
void il_tool_cb_empty_buffer_done(
                            il_comp_t           component_handle,
                            OMX_BUFFERHEADERTYPE*    buffer_done_p);
void il_tool_cb_fill_buffer_done(
                            il_comp_t          component_handle,
                            OMX_BUFFERHEADERTYPE*   buffer_done_p);


ste_adm_res_t il_tool_commit_mixer_effects(il_comp_t comp_h, OMX_U32 port);

ste_adm_res_t il_tool_setConfig_mixer_effect(
        il_comp_t comp_h,
        int position,
        OMX_U32 port,
        OMX_INDEXTYPE nIndex,
        const void* pComponentConfigStructure);

ste_adm_res_t il_tool_add_mixer_effect(
        il_comp_t comp_h,
        int position,
        OMX_U32 port,
        const char* name);

ste_adm_res_t il_tool_remove_mixer_effect(
        il_comp_t comp_h,
        int position,
        OMX_U32 port);


#endif // INCLUSION_GUARD_STE_ADM_OMX_TOOL_H










