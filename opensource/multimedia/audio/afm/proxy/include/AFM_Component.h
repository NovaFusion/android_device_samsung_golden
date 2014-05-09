/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_Component.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_COMPONENT_H_
#define _AFM_COMPONENT_H_

#include "ENS_Component.h"
#include "AFM_PcmPort.h"
#include "AFM_Types.h"
#include "AFM_Index.h"
#include "AFM_macros.h"

class AFM_Component_RDB: public ENS_ResourcesDB {
    public:
        AFM_Component_RDB(OMX_U32 nbOfDomains);
};

/// Abstract base class for  AFM components
class AFM_Component: public ENS_Component {
    public:

        AFM_API_IMPORT virtual ~AFM_Component() = 0;

        AFM_API_IMPORT virtual OMX_ERRORTYPE getExtensionIndex(
                OMX_STRING cParameterName,
                OMX_INDEXTYPE* pIndexType) const;

        AFM_API_IMPORT virtual OMX_BOOL isPortSpecificParameter(OMX_INDEXTYPE nParamIndex) const;

        AFM_API_IMPORT virtual OMX_ERRORTYPE createResourcesDB();

        AFM_API_IMPORT virtual void NmfPanicCallback(void *contextHandler, t_nmf_service_type serviceType, t_nmf_service_data *serviceData);

        AFM_API_IMPORT virtual OMX_ERRORTYPE setParameter(OMX_INDEXTYPE nParamIndex, OMX_PTR pComponentParameterStructure);

        AFM_API_IMPORT virtual OMX_ERRORTYPE getParameter(OMX_INDEXTYPE nParamIndex, OMX_PTR pComponentParameterStructure) const;

        virtual bool prioritiesSetForAllPorts() const {return true;};

        virtual OMX_ERRORTYPE setConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure);
        virtual OMX_ERRORTYPE getConfig(OMX_INDEXTYPE nConfigIndex, OMX_PTR pComponentConfigStructure) const;

        virtual AFM_AUDIO_CONFIG_PCM_PROBE * getProbeConfig(OMX_U32 portIdx) const;
        virtual OMX_ERRORTYPE setProbeConfig(AFM_AUDIO_CONFIG_PCM_PROBE * pConfig);

    protected:
        
        AFM_API_IMPORT OMX_ERRORTYPE   construct(OMX_U32 nPortCount, OMX_U32 nRoleCount=1, bool ArmOnly=false);

#ifndef BC_BREAK
        AFM_API_IMPORT OMX_ERRORTYPE   virtual createPcmPort(
                OMX_U32 nIndex,
                OMX_DIRTYPE eDir,
                OMX_U32 nBufferSizeMin,
                const OMX_AUDIO_PARAM_PCMMODETYPE &defaultPcmSettings);
#endif

   private:
        AFM_AUDIO_CONFIG_PCM_PROBE * mProbeConfig;

};

#endif // _AFM_COMPONENT_H_
