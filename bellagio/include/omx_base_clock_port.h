/**
  src/base/omx_base_clock_port.h
    
  Base Clock Port class for OpenMAX clock ports to be used in derived components.

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

#include "omx_classmagic.h"
#include "omx_base_port.h"

#ifndef __OMX_BASE_CLOCK_PORT_H__
#define __OMX_BASE_CLOCK_PORT_H__

/**
 * @brief the base clock domain structure that describes each port. 
 * 
 * The data structure is derived from base port class and contain clock 
 * domain specific parameters.
 * Other elements can be added in the derived components structures. 
 *
 * @param sOtherParam Domain specific (other) OpenMAX port parameter 
 */

DERIVEDCLASS(omx_base_clock_PortType, omx_base_PortType)
#define omx_base_clock_PortType_FIELDS omx_base_PortType_FIELDS \
  OMX_TIME_CONFIG_TIMESTAMPTYPE        sTimeStamp;  /**< General OpenMAX configuration time stamp parameter */ \
  OMX_TIME_MEDIATIMETYPE               sMediaTime; \
  OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE sMediaTimeRequest; \
  OMX_OTHER_PARAM_PORTFORMATTYPE       sOtherParam; /**< Domain specific (other) OpenMAX port parameter */
ENDCLASS(omx_base_clock_PortType)

/** 
  * @brief the base contructor for the generic openmax ST clock port
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

OMX_ERRORTYPE base_clock_port_Constructor(
  OMX_COMPONENTTYPE *openmaxStandComp,
  omx_base_PortType **openmaxStandPort,
  OMX_U32 nPortIndex, 
  OMX_BOOL isInput);

/** 
  * @brief the base clock port destructor for the generic openmax ST clock port
  * 
  * This function is executed by the component that uses a port.
  * The parameter contains the info about the port.
  * It takes care of destructing the instance of the port
  * 
  * @param openmaxStandPort the ST port to be destructed
  * 
  * @return OMX_ErrorNone 
  */

OMX_ERRORTYPE base_clock_port_Destructor(
  omx_base_PortType *openmaxStandPort);

OMX_ERRORTYPE base_clock_port_SendBufferFunction(
  omx_base_PortType *openmaxStandPort,
  OMX_BUFFERHEADERTYPE* pBuffer);

#endif
