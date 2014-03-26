/**
  src/extension_struct.h

  This file will include all the extension structures defined in Bellagio
  and not present in the OpenMAX spec. These structure will be used
  with the function getExtensionIndex defined in the standard.

  Copyright (C) 2009 STMicroelectronics
  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).

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

#ifndef EXTENSION_STRUCT_H_
#define EXTENSION_STRUCT_H_

/** This structure is threaded like a parameter with the
 * extension index OMX_IndexParameterThreadsID
 */
typedef struct OMX_PARAM_BELLAGIOTHREADS_ID {
    OMX_U32 nSize;                 /**< Size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;      /**< OMX specification version information */
	long int nThreadBufferMngtID; /**< @param nThreadBufferMngtID the linux thread ID of the buffer management thread*/\
	long int nThreadMessageID; /**< @param nThreadMessageID the linux thread ID of the message handler thread*/\
} OMX_PARAM_BELLAGIOTHREADS_ID;

typedef struct multiResourceDescriptor {
	int CPUResourceRequested;
	int MemoryResourceRequested;
} multiResourceDescriptor;

#endif
