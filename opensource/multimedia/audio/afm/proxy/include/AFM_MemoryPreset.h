/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_MemoryPreset.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_MEMORYPRESET_H_
#define _AFM_MEMORYPRESET_H_

#include "host/memorypreset.idt.h"
#include "host/memorybank.idt.h"

typedef struct t_afm_memory_need {
  t_uint32 size[MEM_BANK_COUNT];
} t_afm_memory_need;

typedef struct t_afm_mempreset {
    t_memory_preset     mempreset;
    t_afm_memory_need   memory_needs; 
} t_afm_mempreset;

#endif
