/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   CSA_Index.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/

#ifndef CSA_Index_h
#define CSA_Index_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header must include all required header files to allow the
 *  header to compile without errors.  The includes below are required
 *  for this header file to compile successfully
 */
#include <OMX_Index.h>

typedef enum CSA_INDEXTYPE {

    CSA_IndexStartExtensions = OMX_IndexVendorStartUnused,

    CSA_IndexAudioStartUnused = CSA_IndexStartExtensions + 0x1000,
    CSA_IndexVideoStartUnused = CSA_IndexStartExtensions + 0x2000,
    CSA_IndexImageStartUnused = CSA_IndexStartExtensions + 0x3000,

    /* Chipset Supplier specific area */
    CSA_IndexChipsetSupplierStartUnused = CSA_IndexStartExtensions + 0x4000

} CSA_INDEXTYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/* File EOF */
