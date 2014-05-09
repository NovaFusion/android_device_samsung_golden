#ifndef INCLUSION_GUARD_STE_ADM_OMX_CORE_H
#define INCLUSION_GUARD_STE_ADM_OMX_CORE_H

#include "OMX_Core.h"
#include "AFM_Index.h"




typedef enum ADM_CORE_INDEXTYPE
{
    ADM_IndexStartExtensions = AFM_IndexStartComponentSpecificExtensions,
    ADM_IndexConfigLatencySettings,
    ADM_IndexConfigStartupJitterMargin,
    ADM_IndexParamDictRecMode,
    ADM_IndexParamDictRecMute
} ADM_CORE_INDEXTYPE;

typedef struct ADM_CONFIG_LATENCYSETTINGTYPE {
    OMX_U32              nSize;
    OMX_VERSIONTYPE      nVersion;
    OMX_U32              nPortIndex;
    OMX_U32              nbrBuffersToUse;
    OMX_U32              bufferFillTime;
} ADM_CONFIG_LATENCYSETTINGTYPE;

typedef struct ADM_CONFIG_STARTUPJITTERMARGINTYPE {
    OMX_U32              nSize;
    OMX_VERSIONTYPE      nVersion;
    OMX_U32              nPortIndex;
    OMX_U32              startupJitterMargin;
} ADM_CONFIG_STARTUPJITTERMARGINTYPE;

typedef struct ADM_PARAM_DICTRECMODE {
    OMX_U32              nSize;
    OMX_VERSIONTYPE      nVersion;
    OMX_U32              nMode;
} ADM_PARAM_DICTRECMODETYPE;

typedef struct ADM_PARAM_DICTRECMUTE {
    OMX_U32              nSize;
    OMX_VERSIONTYPE      nVersion;
    OMX_U32              nPortIndex;
    OMX_U32              nMute;
} ADM_PARAM_DICTRECMUTETYPE;

OMX_API OMX_ERRORTYPE adm_omxcore_OMX_GetHandle(
    OMX_OUT OMX_HANDLETYPE* pHandle,
    OMX_IN  OMX_STRING cComponentName,
    OMX_IN  OMX_PTR pAppData,
    OMX_IN  OMX_CALLBACKTYPE* pCallBacks);

// OK to call with invalid handle, returns OMX_ErrorBadParameter
OMX_ERRORTYPE adm_omxcore_OMX_FreeHandle(OMX_HANDLETYPE hComponent);
void ste_adm_omx_core_init(void);
void ste_adm_omx_core_deinit(void);
void ste_adm_omx_core_dump(void);
#endif
