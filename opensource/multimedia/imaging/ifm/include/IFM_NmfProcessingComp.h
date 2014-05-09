/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _IFM_NMFPROCESSINGCOMP_H_
#define _IFM_NMFPROCESSINGCOMP_H_

#include "NmfMpc_ProcessingComponent.h"
#include "ifmport.h"


/// Abstract base class for  IFM Nmf Processing components
#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(IFM_NmfProcessingComp);
#endif
class IFM_NmfProcessingComp: public NmfMpc_ProcessingComponent {
    public:

        IFM_NmfProcessingComp(ENS_Component &enscomp) 
            : NmfMpc_ProcessingComponent(enscomp) 
            {
                int portCount = enscomp.getPortCount();
                DBC_ASSERT(portCount !=0);
                mPoolidArray = new OMX_U32[portCount];
                mBytesPerPixel = 0;
            }
        virtual ~IFM_NmfProcessingComp() {
            delete [] mPoolidArray;
        }
        virtual OMX_ERRORTYPE construct(void);
        virtual OMX_ERRORTYPE destroy(void);

    protected:

        virtual OMX_ERRORTYPE instantiate(void);
        virtual OMX_ERRORTYPE deInstantiate(void);
        virtual OMX_ERRORTYPE start(void);
        virtual OMX_ERRORTYPE stop(void);
        virtual OMX_ERRORTYPE configure(void);


        virtual OMX_ERRORTYPE doBufferAllocation(OMX_U32 nPortIndex,OMX_U32 nBufferIndex,
                OMX_U32 nSizeBytes, OMX_U8 **ppData, void **bufferAllocInfo);

        virtual OMX_ERRORTYPE doBufferDeAllocation(OMX_U32 nPortIndex,OMX_U32 nBufferIndex, void *bufferAllocInfo);

        virtual OMX_U32 getBufferPhysicalAddress(void *bufferAllocInfo, OMX_U8 *,OMX_U32);
        virtual OMX_U32 getBufferMpcAddress(void *bufferAllocInfo) {
			return 0; // TODO : check this
        }

        virtual void doSpecificEmptyBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) {
        
        }
        virtual void doSpecificFillBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) {
        
        }

        
 	   // pure virtual methods of NmfProcessingComponent
        virtual OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port *port) {return OMX_ErrorNone;};
        virtual OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port *port) {return OMX_ErrorNone;};
        
        
        virtual char * getNmfSuffix(void)const;   

    private:

        float           getPixelDepth(OMX_U32 format);

		float     mBytesPerPixel;
		OMX_U32*     mPoolidArray;



};

#endif // _IFM_NMFPROCESSINGCOMP_H_
