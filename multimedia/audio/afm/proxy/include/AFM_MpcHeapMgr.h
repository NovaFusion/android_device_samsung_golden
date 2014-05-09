/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2012. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_MpcHeapMgr.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef  _AFM_MPCHEAPMGR_H_
#define  _AFM_MPCHEAPMGR_H_

#include "ENS_Nmf.h"
#include "host/libeffects/mpc/libmalloc/init.hpp"

#define SIA_HEAP_INT_MEM_X_SIZE     "ste.sia.int.mem.x.size"
#define SIA_HEAP_INT_MEM_Y_SIZE     "ste.sia.int.mem.y.size"
#define SIA_HEAP_ESRAM_MEM_16_SIZE  "ste.sia.esram.mem.16.size"
#define SIA_HEAP_ESRAM_MEM_24_SIZE  "ste.sia.esram.mem.24.size"
#define SIA_HEAP_EXT_MEM_16_SIZE    "ste.sia.ext.mem.16.size"
#define SIA_HEAP_EXT_MEM_24_SIZE    "ste.sia.ext.mem.24.size"

/*
  allocation log level:
  set to 0: log only errors
  set to 1: log information
  set to 2: log debug info
*/
#define SIA_HEAP_DEBUG             "ste.sia.heap.debug"

typedef enum AFM_MEM_TYPE {
  INT_X_MEM_TYPE,
  INT_Y_MEM_TYPE,
  INT_ESRAM16_MEM_TYPE,
  INT_ESRAM24_MEM_TYPE,
  INT_SDRAM16_MEM_TYPE,
  INT_SDRAM24_MEM_TYPE,
  NB_MEM_TYPE
} AFM_MEM_TYPE;

// number of dsp words
#define DEFAULT_SIA_HEAP_INT_MEM_X_SIZE     4000
#define DEFAULT_SIA_HEAP_INT_MEM_Y_SIZE     2000
#define DEFAULT_SIA_HEAP_ESRAM_MEM_16_SIZE  0
#define DEFAULT_SIA_HEAP_ESRAM_MEM_24_SIZE  0
#define DEFAULT_SIA_HEAP_EXT_MEM_16_SIZE    0
#define DEFAULT_SIA_HEAP_EXT_MEM_24_SIZE    128000

#include "linux_utils.h"
#include <stdlib.h>

#ifndef LOG_TAG
#define LOG_TAG "AFM_MpcHeapMgr"
#endif

#ifdef ANDROID
#include <cutils/log.h>
#else
#define ALOGE printf
#define ALOGD printf
#endif

#define HEAPMGR_LOG_LEVEL(level, ...)                \
    if (AFM_MpcHeapMgr::mSteSiaHeapDebug >= level) { \
      ALOGD(__VA_ARGS__);                             \
    }

//-----------------------------------------------------------------------------
//! \class AFM_MpcHeapMgr
//! \brief class that manages the heap on the SIA mpc
//!
//! This is a singleton shared by all mpc components to initialize the
//! mpc heaps (internal, esram, external memory)
//!
//-----------------------------------------------------------------------------
class AFM_MpcHeapMgr {
 public:
  static AFM_MpcHeapMgr * getHandle(OMX_U32 NMFDomainHandle, t_cm_instance_handle ostTrace);    //!< singleton: creates unique instance on first call
  void                    freeHandle(void);   //!< singleton: destroy unique instance on last call
  t_cm_instance_handle    getNMFHandle() { return mNmfHdl; };

 private:
  static AFM_MpcHeapMgr * mpSgAFM_MpcHeapMgr; //!< singleton: pointer on unique instance
  static int           mRefCount;     //!< singleton: reference counter

  static t_cm_instance_handle mNmfHdl;
  static t_cm_instance_handle mOstTrace;
  static OMX_U32              mNMFDomainHandle;
  static t_cm_memory_handle   mMemHdls[NB_MEM_TYPE];
  static t_uint32             mMemHeapSizes[NB_MEM_TYPE];
  static t_sint32             mSteSiaHeapDebug;
  static const char*          mMemHeapName[NB_MEM_TYPE];
  static const char*          mMemHeapPropName[NB_MEM_TYPE];

  Ilibeffects_mpc_libmalloc_init   mIinit;

  AFM_MpcHeapMgr(OMX_U32 NMFDomainHandle);
  ~AFM_MpcHeapMgr();

};

#endif   // _AFM_MPCHEAPMGR_H_
