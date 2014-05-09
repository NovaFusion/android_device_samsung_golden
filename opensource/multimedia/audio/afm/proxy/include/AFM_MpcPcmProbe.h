/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_MpcPcmProbe.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_MPCPCMPROBE_H_
#define _AFM_MPCPCMPROBE_H_

#include "AFM_MpcController.h"
#include "host/pcmdump/complete.hpp"
#include "host/pcmdump/configure.hpp"
#include "AFM_Types.h"

typedef struct {
    OMX_BOOL bEnable;
    OMX_U32  nProbeIdx;
    t_cm_memory_handle   SharedBufMemHdl;
    void *   pSharedBufAddr;
    OMX_U32  nBufferSize;
} PcmProbe_t;

typedef struct {
    t_cm_instance_handle nmfHdl;
    OMX_BOOL             bIsBoundToPcmDump;
} NmfPortHdl_t;

#define PCM_PROBE_NB_BUFFER 10

class AFM_MpcPcmProbe : public pcmdump_completeDescriptor {

    public:
        AFM_MpcPcmProbe(ENS_Component &enscomp, AFMNmfMpc_ProcessingComp &afmProcessingComp) :
            mENSComponent(enscomp), mAFMProcessingComp(afmProcessingComp), mNmfDump(0) {
                OMX_U32 portCount = mENSComponent.getPortCount();
                mConfigPcmProbe = new PcmProbe_t[portCount];
                mNmfPortHdl = new NmfPortHdl_t[portCount];
                mIconfigure = new Ipcmdump_configure[portCount];

                for (OMX_U32 i = 0; i < portCount; i++) {
                    mConfigPcmProbe[i].bEnable = OMX_FALSE;
                    mNmfPortHdl[i].nmfHdl = 0;
                    mNmfPortHdl[i].bIsBoundToPcmDump = OMX_FALSE;
                }
            }

        ~AFM_MpcPcmProbe()
        {
            delete [] mConfigPcmProbe;
            delete [] mNmfPortHdl;
            delete [] mIconfigure;
        }

        OMX_ERRORTYPE   instantiate(OMX_U32 priority);
        OMX_ERRORTYPE   configure(void);
        OMX_ERRORTYPE   start(void);
        OMX_ERRORTYPE   stop(void);
        OMX_ERRORTYPE   deInstantiate(void);

        PcmProbe_t * getProbeConfig(OMX_U32 portIdx) const;

        OMX_ERRORTYPE applyConfig(AFM_AUDIO_CONFIG_PCM_PROBE *pConfig);
        OMX_ERRORTYPE retrieveConfig(AFM_AUDIO_CONFIG_PCM_PROBE * pConfig);

    private:
        // method inherited from misc_pcmdump_completeDescriptor
        virtual void pcmdump_complete(PcmDumpCompleteInfo_t sCompleteInfo);

        Ipcmdump_configure *        mIconfigure;
        PcmProbe_t *                mConfigPcmProbe;

        ENS_Component &               mENSComponent;
        AFMNmfMpc_ProcessingComp &    mAFMProcessingComp;
        NmfPortHdl_t *                mNmfPortHdl;
        t_cm_instance_handle          mNmfDump;
};
#endif //_AFM_MPCPCMPROBE_H_

