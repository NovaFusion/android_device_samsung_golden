/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   ENS_HwRm.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef ENS_HWRM_H_
#define ENS_HWRM_H_

#include <OMX_Component.h>

#include <rme.h>
using namespace rme;

// Base class for using the Ressource Manager
class ENS_HwRm {

public:

    // Synchronous methods
    IMPORT_C static RM_STATUS_E HWRM_Register(const OMX_PTR comphdl,
        const RM_CBK_T *pcbks);

    IMPORT_C static RM_STATUS_E HWRM_Unregister(const OMX_PTR comphdl);

    IMPORT_C static OMX_U32 HWRM_Get_DefaultDDRDomain(OMX_IN RM_NMFD_E nDomainType);

    // Asynchronous calls
    IMPORT_C static OMX_ERRORTYPE HWRM_Notify_NewStatesOMXRES(OMX_IN const OMX_PTR pCompHdl,
        OMX_IN OMX_STATETYPE eOMXILState,
        OMX_IN RM_RESOURCESTATE_E eResourceState,
        OMX_IN OMX_U32 nCounter);

    IMPORT_C static OMX_ERRORTYPE HWRM_Notify_NewPolicyCfg(OMX_IN const OMX_PTR pCompHdl,
        OMX_IN OMX_U32 nGroupPriority, OMX_IN OMX_U32 nGroupID,
        OMX_IN OMX_U32 eQoSbitmap);

    IMPORT_C static OMX_ERRORTYPE HWRM_Notify_RMConfigApplied(OMX_IN const OMX_PTR pCompHdl,
        OMX_IN RM_RC_E eType,
        OMX_IN OMX_U32 nData,
        OMX_IN RM_STATUS_E nErrorCode);

    IMPORT_C static OMX_ERRORTYPE HWRM_Notify_Tunnel(
        OMX_IN const OMX_PTR pCompHdlOut, OMX_IN OMX_U32 nPortIndexOut,
        OMX_IN const OMX_PTR pCompHdlIn, OMX_IN OMX_U32 nPortIndexIn);

    IMPORT_C static OMX_ERRORTYPE HWRM_Notify_ESRAM_Sharing(
        OMX_IN OMX_BOOL bEsramResourceRequired);
};

#endif /*ENS_HWRM_H_*/
