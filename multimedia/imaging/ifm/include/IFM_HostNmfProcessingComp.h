/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _IFM_HostNmfProcessingComp_H_
#define _IFM_HostNmfProcessingComp_H_

#undef OMXCOMPONENT
#define OMXCOMPONENT "IFM_HostNmfProcessingComp"

#include "NmfHost_ProcessingComponent.h"
#include "ifmport.h"


/// Abstract base class for  IFM Nmf Processing components
#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(IFM_HostNmfProcessingComp);
#endif
class IFM_HostNmfProcessingComp: public NmfHost_ProcessingComponent {
    public:

        IFM_HostNmfProcessingComp(ENS_Component &enscomp)
            : NmfHost_ProcessingComponent(enscomp)
        {
            int portCount = enscomp.getPortCount();
            DBC_ASSERT(portCount !=0);
            mPoolidArray = new OMX_U32[portCount];
            mBytesPerPixel = 0;
        }
        virtual ~IFM_HostNmfProcessingComp() {
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


        virtual OMX_ERRORTYPE allocateBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex,
                OMX_U32 nSizeBytes,
                OMX_U8 **ppData,
                void **bufferAllocInfo,
                void **portPrivateInfo);

        virtual OMX_ERRORTYPE useBufferHeader(OMX_DIRTYPE dir,
        		OMX_BUFFERHEADERTYPE* pBufferHdr);

        virtual OMX_ERRORTYPE useBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex,
                OMX_BUFFERHEADERTYPE* pBufferHdr,
                void **portPrivateInfo);

        virtual OMX_ERRORTYPE freeBuffer(
                OMX_U32 nPortIndex,
                OMX_U32 nBufferIndex,
                OMX_BOOL bBufferAllocated,
                void *bufferAllocInfo,
                void *portPrivateInfo);

        OMX_U32 getBufferPhysicalAddress(void *bufferAllocInfo, OMX_U8 *,OMX_U32);


        virtual void doSpecificEmptyBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) {

        }
        virtual void doSpecificFillBufferDone_cb(OMX_BUFFERHEADERTYPE* pBuffer) {

        }


 	   // pure virtual methods of NmfProcessingComponent
        virtual OMX_ERRORTYPE bindPortForProprietaryCom(ENS_Port *port) {return OMX_ErrorNone;};
        virtual OMX_ERRORTYPE unBindPortForProprietaryCom(ENS_Port *port) {return OMX_ErrorNone;};


        virtual char * getNmfSuffix(void)const;


    private:



		float     mBytesPerPixel;
		OMX_U32*     mPoolidArray;

		 float           getPixelDepth(OMX_U32 format);

};

#endif // _IFM_HostNmfProcessingComp_H_
