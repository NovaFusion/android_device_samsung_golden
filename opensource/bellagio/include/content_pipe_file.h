/**
  src/content_pipe_file.h

  This file implements the support for content pipes related to local files.

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

#ifndef __CONTENT_PIPE_FILE_H__
#define __CONTENT_PIPE_FILE_H__

#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>

#include <OMX_Types.h>
#include <OMX_ContentPipe.h>

#include "omx_comp_debug_levels.h"

typedef struct {

  /* public */
  CP_PIPETYPE pipe;

  /* private */
  int fd;

} file_ContentPipe;

#endif
