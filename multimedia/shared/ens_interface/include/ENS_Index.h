/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   ENS_Index.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _ENS_INDEX_H_
#define _ENS_INDEX_H_

#include "ENS_macros.h"
#include "CSA_Index.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @defgroup ens_index_class ENS Extended Indexes
/// @ingroup ens_classes
/// @{
/** ENS eXtended Indexes */
typedef enum ENS_INDEXTYPE {
    ENS_IndexStartExtensions = CSA_IndexChipsetSupplierStartUnused,     /**< Base of ENS Index Extensions */
    ENS_IndexConfigNMFComponentList,     /** returns an ENS_List_p containing a list of t_cm_instance_handle instantiated by this ENS component */
    ENS_IndexConfigTraceSetting,         /**< reference: ENS_CONFIG_TRACESETTINGTYPE */
    ENS_IndexConfigTraceMpc,             /**< reference: ENS_CONFIG_TRACEMODEMPC */

    /// Start of index range for use in the DRe
    ENS_IndexDReStartUnused = (ENS_IndexStartExtensions + 0x1000),

    /// Start of index range for use in the Audio Framework Manager
    ENS_IndexAFMStartUnused = (ENS_IndexStartExtensions + 0x2000),

    /// Start of index range for use in the Video Framework Manager
    ENS_IndexVFMStartUnused = (ENS_IndexStartExtensions + 0x3000),

    /// Start of index range for use in the Imaging Framework Manager
    ENS_IndexIFMStartUnused = (ENS_IndexStartExtensions + 0x4000),

    /// Start of index range for use in the Val_Source_Sink
    ENS_IndexVSSStartUnused = (ENS_IndexStartExtensions + 0x5000),

} ENS_INDEXTYPE;

#define PV_OMX_COMPONENT_CAPABILITY_TYPE_INDEX 0xFF7A347

//The OMX_GetParameter call expects the following structure to be filled for this index:

typedef struct PV_OMXComponentCapabilityFlagsType {
////////////////// OMX COMPONENT CAPABILITY RELATED MEMBERS
    OMX_BOOL iIsOMXComponentMultiThreaded;
    OMX_BOOL iOMXComponentSupportsExternalOutputBufferAlloc;
    OMX_BOOL iOMXComponentSupportsExternalInputBufferAlloc;
    OMX_BOOL iOMXComponentSupportsMovableInputBuffers;
    OMX_BOOL iOMXComponentSupportsPartialFrames;
    OMX_BOOL iOMXComponentUsesNALStartCode;
    OMX_BOOL iOMXComponentCanHandleIncompleteFrames;
    OMX_BOOL iOMXComponentUsesFullAVCFrames;
} PV_OMXComponentCapabilityFlagsType;

/// @}

//Structure associated to ENS_IndexConfigTraceSetting
typedef struct {
    OMX_U32         nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U16         nTraceEnable; // tracegroup bitfields to enable/disable traces
    OMX_U32         nParentHandle; // OMX component ID
} ENS_CONFIG_TRACESETTINGTYPE;

//Structure associated to ENS_IndexConfigTraceMpc
typedef struct {
    OMX_U32         nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL        bEnable; /* sends MPC traces to ARM when true */
} ENS_CONFIG_TRACEMODEMPC;

#ifdef __cplusplus
}
#endif

#endif
