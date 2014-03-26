/**
  src/omx_base_video_port.c

  Base Video Port class for OpenMAX ports to be used in derived components.

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#include <string.h>
#include <unistd.h>
#include <omxcore.h>
#include <OMX_Core.h>
#include <OMX_Component.h>
#include "omx_base_component.h"
#include "omx_base_video_port.h"

/**
  * @brief The base contructor for the generic OpenMAX ST Video port
  *
  * This function is executed by the component that uses a port.
  * The parameter contains the info about the component.
  * It takes care of constructing the instance of the port and
  * every object needed by the base port.
  *
  * @param openmaxStandComp pointer to the Handle of the component
  * @param openmaxStandPort the ST port to be initialized
  * @param nPortIndex Index of the port to be constructed
  * @param isInput specifices if the port is an input or an output
  *
  * @return OMX_ErrorInsufficientResources if a memory allocation fails
  */

OMX_ERRORTYPE base_video_port_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,omx_base_PortType **openmaxStandPort,OMX_U32 nPortIndex, OMX_BOOL isInput) {

  omx_base_video_PortType *omx_base_video_Port;

  if (!(*openmaxStandPort)) {
    *openmaxStandPort = calloc(1,sizeof (omx_base_video_PortType));
  }

  if (!(*openmaxStandPort)) {
    return OMX_ErrorInsufficientResources;
  }

  base_port_Constructor(openmaxStandComp,openmaxStandPort,nPortIndex, isInput);

  omx_base_video_Port = (omx_base_video_PortType *)*openmaxStandPort;

  setHeader(&omx_base_video_Port->sVideoParam, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
  omx_base_video_Port->sVideoParam.nPortIndex = nPortIndex;
  omx_base_video_Port->sVideoParam.nIndex = 0;
  omx_base_video_Port->sVideoParam.eCompressionFormat = OMX_VIDEO_CodingUnused;
  omx_base_video_Port->sVideoParam.eColorFormat = OMX_COLOR_FormatUnused;
  omx_base_video_Port->sVideoParam.xFramerate = 15;

  omx_base_video_Port->sPortParam.eDomain = OMX_PortDomainVideo;
  omx_base_video_Port->sPortParam.format.video.cMIMEType = malloc(DEFAULT_MIME_STRING_LENGTH);
  strcpy(omx_base_video_Port->sPortParam.format.video.cMIMEType, "raw/video");
  omx_base_video_Port->sPortParam.format.video.pNativeRender = 0;
  omx_base_video_Port->sPortParam.format.video.bFlagErrorConcealment = OMX_FALSE;
  omx_base_video_Port->sPortParam.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;

  omx_base_video_Port->sPortParam.format.video.nFrameWidth = 0;
  omx_base_video_Port->sPortParam.format.video.nFrameHeight= 0;
  omx_base_video_Port->sPortParam.format.video.nStride = 0;
  omx_base_video_Port->sPortParam.format.video.nSliceHeight = 0;
  omx_base_video_Port->sPortParam.format.video.nBitrate = 0;
  omx_base_video_Port->sPortParam.format.video.xFramerate = 15;
  omx_base_video_Port->sPortParam.format.video.eColorFormat = OMX_COLOR_FormatUnused;
  omx_base_video_Port->sPortParam.format.video.pNativeWindow = NULL;


  omx_base_video_Port->sPortParam.nBufferSize = (isInput == OMX_TRUE)?DEFAULT_IN_BUFFER_SIZE:MAX_VIDEO_OUTPUT_BUF_SIZE ;

  omx_base_video_Port->PortDestructor = &base_video_port_Destructor;

  return OMX_ErrorNone;
}

/**
  * @brief The base video port destructor for the generic OpenMAX ST Video port
  *
  * This function is executed by the component that uses a port.
  * The parameter contains the info about the port.
  * It takes care of destructing the instance of the port
  *
  * @param openmaxStandPort the ST port to be destructed
  *
  * @return OMX_ErrorNone
  */

OMX_ERRORTYPE base_video_port_Destructor(omx_base_PortType *openmaxStandPort){

  if(openmaxStandPort->sPortParam.format.video.cMIMEType) {
    free(openmaxStandPort->sPortParam.format.video.cMIMEType);
    openmaxStandPort->sPortParam.format.video.cMIMEType = NULL;
  }

  base_port_Destructor(openmaxStandPort);

  return OMX_ErrorNone;
}
