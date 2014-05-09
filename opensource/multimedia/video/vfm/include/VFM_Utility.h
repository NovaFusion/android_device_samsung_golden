/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _VFM_UTILITY_H_
#define _VFM_UTILITY_H_

/*+CR 343565 runtime OSI traces */
#include "osi_trace.h"
/*-CR 343565 runtime OSI traces */

/// @ingroup VFMUtilities
/// @{

/// @brief Endianess conversion:  little endian <--> big endian, used
/// to pass int32 value from/to the ARM to/from the DSP
/// @hideinitializer
#define ENDIANESS_CONVERSION(number) ((t_uint32)(number)>>16 | ((t_uint32)(number) & 0xffff)<<16)

/*+CR 343565 runtime OSI traces */
#if 0
/// @brief Utility macros to return errors
/// @hideinitializer
#define RETURN_XXX_IF_WRONG(_x, _error)                { if (!(_x)) { OUT0(""); return (_error); } }

/// @brief Utility macros to return errors
/// @hideinitializer
#define RETURN_OMX_ERROR_IF_ERROR(_x)                { OMX_ERRORTYPE _error; _error = (_x); if (_error!=OMX_ErrorNone) { OUT0(""); return _error; } }
#else
//#include <stdio.h>
#define VFM_TO_STR(x) #x
#define RETURN_XXX_IF_WRONG(_x, _error)                { if (!(_x)) { DBGT_CRITICAL("RETURN_XXX_IF_WRONG, condition=%s, error=%d(0x%x)(%s)", VFM_TO_STR(_x),  _error, _error, VFM_TO_STR(_error)); OUT0("Error"); return (_error); } }
#define RETURN_OMX_ERROR_IF_ERROR(_x)                { OMX_ERRORTYPE _error; _error = (_x); if (_error!=OMX_ErrorNone) { DBGT_CRITICAL("RETURN_OMX_ERROR_IF_ERROR, condition=%s, error=%d(0x%x)(%s)", VFM_TO_STR(_x), _error,_error, VFM_TO_STR(_error)); OUT0("Error"); return (_error); } }
#endif
/*-CR 343565 runtime OSI traces */

/// @brief Bit to indicate to use MTF on this memory
/// (to be set on the ARM before passing to the DSP)
/// @hideinitializer
#define VFM_EXT_BIT       1

/// @brief Macro to have the upper address that fits an alignement
/// (mask should be something like 0xF to get an alignement of 16)
/// @hideinitializer
#define VFM_ROUND_UPPER(value, mask) (((t_uint32)(value) + mask ) & ~mask)

/// @brief Image buffer alignement constraint
/// @hideinitializer
#define VFM_IMAGE_BUFFER_ALIGN_MASK               0xff

/// @brief Address buffer alignement constraint
/// @hideinitializer
#define VFM_ADDRESS_ALIGN_MASK                    0xf

/// @}

#endif
