/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _IFM_PORT_H_
#define _IFM_PORT_H_

#include <cm/inc/cm.hpp>
#include "ENS_Port.h"
#include "ENS_List.h"




/* should be removed */
/*
#define ISPPROC_NB_OUT_PORTS 3
#define ISPPROC_NB_IN_PORTS 1
#define ISPPROC_NB_PORTS (ISPPROC_NB_OUT_PORTS+ISPPROC_NB_IN_PORTS)


#define ISPPROC_PORT_OUT2 3
#define ISPPROC_PORT_OUT1 2
#define ISPPROC_PORT_OUT0 1
#define ISPPROC_PORT_IN0 0
*/


#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(ifmport);
#endif
class ifmport: public ENS_Port {
   public:


        ifmport(const EnsCommonPortData& commonPortData, ENS_Component& ensComp)
            : ENS_Port(commonPortData, ensComp){
            mBytesPerPixel = 4 ; //TODO : should be set in child class
            mInternalFrameWidth = 0;
            mInternalFrameHeight = 0;
            bIsOverScanned = false;
            mOverScannedWidth = 0;
            mOverScannedHeight = 0;
      }

      virtual ~ifmport() {};

      virtual OMX_ERRORTYPE setFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE &portdef) ;

      /* TODO : getParamPortDefinition() should be removed */
      /* This should be done instead :
       *
       *     OMX_PARAM_PORTDEFINITIONTYPE mParamPortDefinition;
       *     mParamPortDefinition.nSize              = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
       *     getOmxIlSpecVersion(&mParamPortDefinition.nVersion);
       *     mENSComponent.getPort(0)->getParameter(OMX_IndexParamPortDefinition,&mParamPortDefinition)
       */

      OMX_PARAM_PORTDEFINITIONTYPE getParamPortDefinition(void) const {
         return mParamPortDefinition;
      }

      virtual OMX_ERRORTYPE checkFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE &portdef) {return OMX_ErrorNone;}

      virtual OMX_ERRORTYPE checkCompatibility(
            OMX_HANDLETYPE hTunneledComponent,
            OMX_U32 nTunneledPort) const ;

      virtual OMX_BOOL isAudio() const { return OMX_FALSE; }
      virtual OMX_BOOL isVideo() const { return OMX_FALSE; }
      virtual OMX_BOOL isImage() const { return OMX_TRUE; }


      float getPixelDepth(OMX_U32 format);
      OMX_S32 getStride(OMX_U32 format, OMX_U32 width, bool *isInterleaved);
      OMX_U32 getBufferSize(OMX_U32 format, OMX_S32 stride, OMX_U32 width, OMX_U32 height);

       /* TODO: the following should be put in child class */
      void setPixelDepth(OMX_U32 bytesPerPixel){
         mBytesPerPixel = bytesPerPixel;
      }

      float     mBytesPerPixel;

    /* for handling of image sizes not compliant with HW alignment constraints */
    OMX_U32 mInternalFrameWidth;
    OMX_U32 mInternalFrameHeight;

    bool bIsOverScanned;
    OMX_U32 mOverScannedWidth;
    OMX_U32 mOverScannedHeight;
};

/// Pointer to a ENS_Port
typedef ifmport * IFM_Port_p;


#endif // _IFM_PORT_H_
