/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   ENS_DBC.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _ENS_DBC_H_
#define _ENS_DBC_H_

/// @ingroup ens_classes
typedef enum {
    DBC_ASSERTION_FAILED,
    DBC_PRECONDITION_FAILED,
    DBC_POSTCONDITION_FAILED,
    DBC_INVARIANT_FAILED
} DBCError_e;

extern "C" {
    void dbc_assert(int a, DBCError_e errorId, const char *file, int line);
}

/// @ingroup ens_classes
#define DBC_ASSERT(a) dbc_assert((int)(a), DBC_ASSERTION_FAILED, __FILE__, __LINE__);

/// @ingroup ens_classes
#define DBC_PRECONDITION(a) dbc_assert((int)(a), DBC_PRECONDITION_FAILED, __FILE__, __LINE__);

/// @ingroup ens_classes
#define DBC_POSTCONDITION(a) dbc_assert((int)(a), DBC_POSTCONDITION_FAILED, __FILE__, __LINE__);

/// @ingroup ens_classes
#define DBC_INVARIANT(a) dbc_assert((int)(a), DBC_INVARIANT_FAILED, __FILE__, __LINE__);

#endif /* _ENS_DBC_H_ */
