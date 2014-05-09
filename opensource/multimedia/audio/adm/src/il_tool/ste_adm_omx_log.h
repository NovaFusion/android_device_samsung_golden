#ifndef INCLUSION_GUARD_STE_ADM_OMX_LOG_H
#define INCLUSION_GUARD_STE_ADM_OMX_LOG_H


#include "OMX_Core.h"
#include "OMX_Component.h"
#include "ste_adm_omx_tool.h" // adm_il_appdata_t


/**
* Save log in a circular buffer that is displayed when a fatal error
* has occured, to be able to see what happened when the 'normal' prints
* showing the path to the error were not enabled.
*
* ste_adm_debug_logbuf_init() should be called before any IL activity
* ste_adm_debug_logbuf_show() should be called when a fatal error
* has occured (typically; an IL callback has not arrived, or an IL error
* has been received).
*/
void ste_adm_debug_logbuf_show();
void ste_adm_debug_logbuf_init();

OMX_API OMX_ERRORTYPE OMX_APIENTRY il_tool_log_Init(void);

OMX_ERRORTYPE il_tool_log_CreateComponent(const char* c,
    il_comp_t comp_h);

OMX_API OMX_ERRORTYPE OMX_APIENTRY il_tool_log_FreeHandle(
    il_comp_t comp_h);

OMX_ERRORTYPE il_tool_log_SetConfig(
    il_comp_t comp_h,
    OMX_INDEXTYPE nIndex,
    const void* pComponentConfigStructure);

OMX_ERRORTYPE il_tool_log_SetParameter(
    il_comp_t comp_h,
    OMX_INDEXTYPE nIndex,
    const void* pComponentParameterStructure);

OMX_ERRORTYPE il_tool_log_SendCommand(
    il_comp_t comp_h,
    OMX_COMMANDTYPE Cmd,
    OMX_U32 nParam1,
    OMX_PTR pCmdData);

OMX_ERRORTYPE il_tool_log_GetParameter(
    il_comp_t hComponent,
    OMX_INDEXTYPE nParamIndex,
    OMX_PTR pComponentParameterStructure);

OMX_ERRORTYPE il_tool_log_GetConfig(
    il_comp_t hComponent,
    OMX_INDEXTYPE nIndex,
    OMX_PTR pComponentConfigStructure);

OMX_ERRORTYPE il_tool_log_GetComponentVersion(
    il_comp_t hComponent,
    OMX_STRING pComponentName,
    OMX_VERSIONTYPE* pComponentVersion,
    OMX_VERSIONTYPE* pSpecVersion,
    OMX_UUIDTYPE* pComponentUUID);

OMX_ERRORTYPE il_tool_log_GetState(
    il_comp_t hComponent,
    OMX_STATETYPE* pState);

OMX_ERRORTYPE il_tool_log_SetupTunnel(
    il_comp_t hOutput,
    OMX_U32 nPortOutput,
    il_comp_t hInput,
    OMX_U32 nPortInput);

OMX_ERRORTYPE il_tool_log_GetExtensionIndex(
    il_comp_t hComponent,
    const char* cParameterName,
    OMX_INDEXTYPE* pIndexType);


OMX_ERRORTYPE il_tool_log_EmptyThisBuffer(
            il_comp_t hComponent,
            OMX_IN  OMX_BUFFERHEADERTYPE* pBuffer);

OMX_ERRORTYPE il_tool_log_FillThisBuffer(
            il_comp_t hComponent,
            OMX_IN  OMX_BUFFERHEADERTYPE* pBuffer);


OMX_ERRORTYPE il_tool_log_AllocateBuffer(
            il_comp_t hComponent,
            OMX_INOUT OMX_BUFFERHEADERTYPE** ppBuffer,
            OMX_IN OMX_U32 nPortIndex,
            OMX_IN OMX_PTR pAppPrivate,
            OMX_IN OMX_U32 nSizeBytes);

OMX_ERRORTYPE il_tool_log_UseBuffer(
            il_comp_t comp_h,
            OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr,
            OMX_IN OMX_U32 nPortIndex,
            OMX_IN OMX_PTR pAppPrivate,
            OMX_IN OMX_U32 nSizeBytes,
            OMX_IN OMX_U8* pBuffer);

OMX_ERRORTYPE il_tool_log_FreeBuffer(
            il_comp_t hComponent,
            OMX_IN  OMX_U32 nPortIndex,
            OMX_IN  OMX_BUFFERHEADERTYPE* pBuffer);



#ifdef __cplusplus
extern "C" {
#endif
    const char* adm_log_index2str(OMX_INDEXTYPE idx);
    const char* adm_log_state2str(OMX_STATETYPE state);
    const char* adm_log_event2str(OMX_EVENTTYPE event);
    const char* adm_log_cmd2str(OMX_COMMANDTYPE cmd);
    const char* adm_log_err2str(OMX_ERRORTYPE res);
#ifdef __cplusplus
}
#endif







#endif // INCLUSION_GUARD_STE_ADM_OMX_LOG_H
