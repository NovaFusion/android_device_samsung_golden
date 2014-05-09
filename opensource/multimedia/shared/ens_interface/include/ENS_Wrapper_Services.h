/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   ENS_Wrapper_Services.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _ENS_WRAPPER_SERVICES_H_
#define _ENS_WRAPPER_SERVICES_H_

#ifndef HOST_ONLY
  #include <cm/inc/cm.hpp>
#else
  #include <inc/type.h>
  #include <share/inc/macros.h>
#endif //HOST_ONLY

#undef OMX_TRACE_UID
/* depends on OpenMax IL */
#include "OMX_Component.h"

class ENS_IOMX;

ENS_IOMX * EnsWrapper_getEnsComponent(OMX_HANDLETYPE hComp);

typedef enum {
    ENS_ASIC_ID_INVALID  = -1,
    ENS_ASIC_ID_DB8500ED = 0,
    ENS_ASIC_ID_DB8500V1 = 1,
    ENS_ASIC_ID_DB8500V2 = 2,
} ENS_ASIC_ID;

ENS_ASIC_ID EnsWrapper_getASIC_ID();

#include "cpp.hpp"

/**
 * In a thread based model, NMF calls need to go through the wrapper
 */

#ifndef HOST_ONLY

/**
 * Create a full context to use the CM driver (including the NMF event handler).
 * By default  "Shared channel" attribut is used.
 */
t_cm_error EnsWrapper_local_cm_CreateFullContext(OMX_PTR &aCtx,
    t_nmf_channel_flag flag);

t_cm_error EnsWrapper_local_cm_DestroyFullContext(OMX_PTR &aCtx);

t_cm_error EnsWrapper_local_cm_bindComponentToUser(OMX_PTR aCtx,
    t_cm_instance_handle component,
    const char *itfname,
    NMF::InterfaceDescriptor *itfref,
    t_uint32 fifosize);

t_cm_error EnsWrapper_local_cm_unbindComponentToUser(OMX_PTR aCtx,
    const t_cm_instance_handle client,
    const char *itfname);

t_cm_error EnsWrapper_cm_bindComponentToUser(OMX_HANDLETYPE hComp,
    t_cm_instance_handle component,
    const char *itfname,
    NMF::InterfaceDescriptor *itfref,
    t_uint32 fifosize);

t_cm_error EnsWrapper_cm_unbindComponentToUser(OMX_HANDLETYPE hComp,
    const t_cm_instance_handle client,
    const char *itfname);

#endif //HOST_ONLY

t_nmf_error EnsWrapper_bindToUser(OMX_HANDLETYPE hComp,
    NMF::Composite *component,
    const char *itfname,
    NMF::InterfaceDescriptor *itfref,
    t_uint32 fifosize);

t_nmf_error EnsWrapper_unbindToUser(OMX_HANDLETYPE hComp,
    NMF::Composite *component,
    const char *itfname);

#endif /* _ENS_WRAPPER_SERVICES_H_ */
