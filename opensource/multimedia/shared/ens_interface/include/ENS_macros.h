/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   ENS_macros.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _ENS_MACRO_H_
#define _ENS_MACRO_H_

#ifndef ENS_API_IMPORT
  #ifdef __SYMBIAN32__
    #define ENS_API_IMPORT IMPORT_C
  #else
    #define ENS_API_IMPORT
  #endif
#endif

#ifndef ENS_API_EXPORT
  #ifdef __SYMBIAN32__
    #define ENS_API_EXPORT EXPORT_C
  #else
    #define ENS_API_EXPORT
  #endif
#endif

#endif /* _ENS_MACRO_H_ */
