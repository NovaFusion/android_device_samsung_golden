/**
  src/omx_create_loaders.h

  Define a generic entry point for component loaders creation.
  Each component loader should provide an implementation of the
  createComponentLoaders function.

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

#ifndef __OMX_CREATE_LOADERS_H__
#define __OMX_CREATE_LOADERS_H__

#ifdef __cplusplus
extern "C" {
#endif

int
createComponentLoaders(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __OMX_CREATE_LOADERS_H__
