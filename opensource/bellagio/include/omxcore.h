/**
  src/omxcore.h

  OpenMAX Integration Layer Core. This library implements the OpenMAX core
  responsible for environment setup, components tunneling and communication.

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

#ifndef __ST_OMXCORE_H__
#define __ST_OMXCORE_H__

#include <OMX_Component.h>
#include <OMX_Types.h>
#include <pthread.h>

#include "utils.h"
#include "component_loader.h"
#include "omx_comp_debug_levels.h"

//forward decl
struct BOSA_COMPONENTLOADER;

OMX_ERRORTYPE BOSA_AddComponentLoader(struct BOSA_COMPONENTLOADER *pLoader);

/** Defines the major version of the core */
#define SPECVERSIONMAJOR  1
/** Defines the minor version of the core */
#define SPECVERSIONMINOR  1
/** Defines the revision of the core */
#define SPECREVISION      0
/** Defines the step version of the core */
#define SPECSTEP          0

#endif
