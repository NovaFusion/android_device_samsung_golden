/**
  src/utils.h

  Set of utility functions for debugging purposes

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


#ifndef __ST_UTILS_H__
#define __ST_UTILS_H__

#include <string.h>

#include <OMX_Component.h>
#include <OMX_Types.h>

char *stateName(OMX_STATETYPE state);
char *transientStateName(int state);
char *errorName(OMX_ERRORTYPE error);

/*
 * Copy a block of memory, handling overlap.
 * Method taken from bionic to prevent size check.
 */
void *memcpy_unsafe(void *dst0, const void *src0, size_t length);

#endif
