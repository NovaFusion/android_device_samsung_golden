/*
 * Copyright (C) ST-Ericsson SA 2011. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _ENS_IOMX_H_
#define _ENS_IOMX_H_

#ifndef ENS_API_IMPORT
  #ifdef __SYMBIAN32__
    #define ENS_API_IMPORT IMPORT_C
  #else
    #define ENS_API_IMPORT
  #endif
#endif

#ifndef ENS_API_EXPORT
  #ifdef __SYMBIAN32__
    #define ENS_API_EXPORT EXPORT_C
  #else
    #define ENS_API_EXPORT
  #endif
#endif

#include "OMX_Core.h"
#include <inc/typedef.h>
#include "nmf/inc/service_type.h"

/// max length for component name
#define ENS_COMPONENT_NAME_MAX_LENGTH 128

/// Callback structure that wrapper may provide execute asynchronously
/// some processing part in a separate thread for example.
typedef struct ENSWRAPPER_CALLBACKTYPE {
    void (*ConstructENSProcessingPartAsync)(OMX_HANDLETYPE hComponent);
    void (*DestroyENSProcessingPartAsync)(OMX_HANDLETYPE hComponent);
} ENSWRAPPER_CALLBACKTYPE;

class ENS_IOMX {

public:

    ENS_API_IMPORT virtual OMX_ERRORTYPE SendCommand(
        OMX_COMMANDTYPE Cmd,
        OMX_U32 nParam,
        OMX_PTR pCmdData) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE SetParameter(
        OMX_INDEXTYPE nIndex,
        OMX_PTR pComponentParameterStructure) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE GetParameter(
        OMX_INDEXTYPE nIndex,
        OMX_PTR pComponentParameterStructure) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE SetConfig(
        OMX_INDEXTYPE nIndex,
        OMX_PTR pComponentConfigStructure) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE GetConfig(
        OMX_INDEXTYPE nIndex,
        OMX_PTR pComponentConfigStructure) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE GetExtensionIndex(
        OMX_STRING cParameterName,
        OMX_INDEXTYPE *pIndexType) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE GetState(
        OMX_STATETYPE *pState) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE ComponentTunnelRequest(
        OMX_U32 nPort,
        OMX_HANDLETYPE hTunneledComp,
        OMX_U32 nTunneledPort,
        OMX_TUNNELSETUPTYPE *pTunnelSetup) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE SetCallbacks(
        OMX_CALLBACKTYPE *pCallbacks,
        OMX_PTR pAppData) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE EmptyThisBuffer(
        OMX_BUFFERHEADERTYPE *pBuffer) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE FillThisBuffer(
        OMX_BUFFERHEADERTYPE *pBuffer) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE UseBuffer(
        OMX_BUFFERHEADERTYPE **ppBufferHdr,
        OMX_U32 nPortIndex,
        OMX_PTR pAppPrivate,
        OMX_U32 nSizeBytes,
        OMX_U8 *pBuffer) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE AllocateBuffer(
        OMX_BUFFERHEADERTYPE **ppBufferHdr,
        OMX_U32 nPortIndex,
        OMX_PTR pAppPrivate,
        OMX_U32 nSizeBytes) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE FreeBuffer(
        OMX_U32 nPortIndex,
        OMX_BUFFERHEADERTYPE *pBuffer) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE UseEGLImage(
        OMX_BUFFERHEADERTYPE **ppBufferHdr,
        OMX_U32 nPortIndex,
        OMX_PTR pAppPrivate,
        void *eglImage) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE GetComponentVersion(
        OMX_STRING pComponentName,
        OMX_VERSIONTYPE *pComponentVersion,
        OMX_VERSIONTYPE *pSpecVersion,
        OMX_UUIDTYPE *pComponentUUID) = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE ComponentDeInit() = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE ComponentRoleEnum(
        OMX_U8 *cRole,
        OMX_U32 nIndex) = 0;

// TO BE IMPROVED
// Functions that are called by the wrapper

    ENS_API_IMPORT virtual void NmfPanicCallback(
        void *contextHandler,
        t_nmf_service_type serviceType,
        t_nmf_service_data *serviceData) = 0;

    ENS_API_IMPORT virtual void *GetProcessingComponent() = 0;

    /** Initialize the wrapper callback structure if any */
    ENS_API_IMPORT virtual void                      setEnsWrapperCB(ENSWRAPPER_CALLBACKTYPE *ensWrapperCb) = 0;
    ENS_API_IMPORT virtual ENSWRAPPER_CALLBACKTYPE * getEnsWrapperCB()                                      = 0;

    ENS_API_IMPORT virtual OMX_U32      getIndexEventCmdReceived()                                        = 0;
    ENS_API_IMPORT virtual unsigned int getNbCmdReceivedEventsToWait(OMX_COMMANDTYPE Cmd, OMX_U32 nParam) = 0;

    ENS_API_IMPORT virtual const char * getName() const = 0;

    ENS_API_IMPORT virtual OMX_VERSIONTYPE getVersion(void) const = 0;
    ENS_API_IMPORT virtual unsigned short  getTraceEnable() const = 0;

    ENS_API_IMPORT virtual void          setName(const char *cName)                      = 0;
    ENS_API_IMPORT virtual OMX_ERRORTYPE createResourcesDB(void)                         = 0;
    ENS_API_IMPORT virtual OMX_ERRORTYPE setOMXHandleAndRegisterRm(OMX_HANDLETYPE hComp) = 0;
    ENS_API_IMPORT virtual OMX_U32       getUidTopDictionnary(void)                      = 0;

    ENS_API_IMPORT virtual OMX_ERRORTYPE construct(void) = 0;
    ENS_API_IMPORT virtual OMX_ERRORTYPE eventHandlerCB(
            OMX_EVENTTYPE eEvent,
            OMX_U32 nData1,
            OMX_U32 nData2,
            OMX_PTR pEventData) = 0;

    ENS_API_IMPORT virtual ~ENS_IOMX(void) = 0;
};

inline ENS_API_IMPORT ENS_IOMX::~ENS_IOMX(void) { }

/// Function used to indicate the Open Max specification version.
inline void getOmxIlSpecVersion(OMX_VERSIONTYPE *version) {
    version->s.nVersionMajor = OMX_VERSION_MAJOR;
    version->s.nVersionMinor = OMX_VERSION_MINOR;
    version->s.nRevision     = OMX_VERSION_REVISION;
    version->s.nStep         = OMX_VERSION_STEP;
}

#endif      // _ENS_IOMX_H_
