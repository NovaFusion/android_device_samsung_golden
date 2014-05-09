/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_macros.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_MACRO_H_
#define _AFM_MACRO_H_


#ifndef AFM_API_IMPORT
	#ifdef __SYMBIAN32__
		#define AFM_API_IMPORT IMPORT_C
	#else
		#define AFM_API_IMPORT
	#endif
#endif

#ifndef AFM_API_EXPORT
	#ifdef __SYMBIAN32__
		#define AFM_API_EXPORT EXPORT_C
	#else
		#define AFM_API_EXPORT
	#endif
#endif


#endif /* _AFM_MACRO_H_ */
