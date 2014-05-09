/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_Port.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_PORT_H_
#define _AFM_PORT_H_

#include "ENS_Port.h"
#include "ENS_List.h"
#include "AFM_Types.h"
#include "AFM_macros.h"

/// Concrete class for audio ports.
/// Derived from ENS_Port
class AFM_Port: public ENS_Port {
    public:

        AFM_API_IMPORT AFM_Port(OMX_U32 nIndex,
                OMX_DIRTYPE eDir,
                OMX_U32 nBufferSizeMin,
                const OMX_AUDIO_CODINGTYPE eEncoding,
                ENS_Component &enscomp);

        AFM_API_IMPORT AFM_Port(OMX_U32 nIndex,
                OMX_DIRTYPE eDir,
                OMX_U32 nBufferSizeMin,
                OMX_U32 nbFormatSupported,
                const OMX_AUDIO_CODINGTYPE *pEncodingTable,
                ENS_Component &enscomp);

        AFM_API_IMPORT virtual ~AFM_Port(void) = 0;

        AFM_API_IMPORT virtual OMX_ERRORTYPE setParameter(
                OMX_INDEXTYPE nParamIndex,
                OMX_PTR pComponentParameterStructure);

        AFM_API_IMPORT virtual OMX_ERRORTYPE getParameter(
                OMX_INDEXTYPE nParamIndex,
                OMX_PTR pComponentParameterStructure) const;

        AFM_API_IMPORT virtual OMX_ERRORTYPE setFormatInPortDefinition(
		            const OMX_PARAM_PORTDEFINITIONTYPE& portDef);
        
        AFM_API_IMPORT virtual OMX_ERRORTYPE proprietaryCommunicationRequest(
                OMX_HANDLETYPE hTunneledComponent,  
                OMX_U32 nTunneledPort);
        
        // called on input ports during componentTunnelRequest
        // to check compatibility of tunneled port
        AFM_API_IMPORT virtual OMX_ERRORTYPE checkCompatibility(
                OMX_HANDLETYPE hTunneledComp, OMX_U32 nTunneledPort) const;

        // called by checkCompatibility method 
        // => must check format specific compatibility
        AFM_API_IMPORT virtual OMX_ERRORTYPE checkFormatCompatibility(
                OMX_HANDLETYPE hTunneledComp, OMX_U32 nTunneledPort) const = 0;

        AFM_API_IMPORT OMX_BOOL getFlagErrorConcealment(void);
        OMX_U32                     mNbFormatSupported;
        OMX_AUDIO_CODINGTYPE        *mEncodingTable;

        AFM_PROPRIETARY_COMMUNICATION getProprietaryCommInUse(void) {return mProprietaryCommInUse;}
        void setSupportedProprietaryComm(AFM_PROPRIETARY_COMMUNICATION type) {mSupportedProprietaryComm = type;}

        AFM_API_IMPORT OMX_PORT_PRIORITY getPortPriorityLevel() const;
        void setPortPriorityLevel(OMX_PORT_PRIORITY priority){
            mPortPriorityLevel = priority;
        };

        OMX_BOOL getHostMpcSyncModeEnabled() const { return mHostMpcSyncEnabled; }

        t_cm_mpc_memory_type getBindingMpcMemoryType24() const;

   private:

        AFM_PROPRIETARY_COMMUNICATION mSupportedProprietaryComm;
        AFM_PROPRIETARY_COMMUNICATION mProprietaryCommInUse;
        OMX_PORT_PRIORITY mPortPriorityLevel;
        OMX_BOOL mHostMpcSyncEnabled;
        AFM_MEMORY_TYPE mBindingMemoryType;
};

/// Pointer to a ENS_Port
typedef AFM_Port * AFM_Port_p;


#endif // _AFM_PORT_H_
