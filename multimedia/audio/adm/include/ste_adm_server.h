/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */
/*! \file ste_adm_server.h
*   \brief
*/

#ifndef STE_ADM_SERVER_INCLUSION_GUARD_H
#define STE_ADM_SERVER_INCLUSION_GUARD_H

#ifndef STE_ADM_EXPORT
  #define STE_ADM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
* Start the ADM server. It will run in its own thread.
* Returns 0 if ok, negative error value otherwise
*/
int STE_ADM_EXPORT ste_adm_server_create(void);

#ifdef __cplusplus
}
#endif

#endif /* STE_ADM_SERVER_INCLUSION_GUARD_H */


