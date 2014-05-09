/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file
 * \brief   header for servies for openmax OSAL
 * \author  ST-Ericsson
 *
 ***************************************************************************
 */

#ifndef _OMXILOSALSERVICES_H
#define _OMXILOSALSERVICES_H

#include <sys/cdefs.h>
#include <OMX_Types.h>
#include <OMX_Core.h>

/** Maximum size of a property string - size of string to be passed to GetProperty */
#define OMXIL_OSAL_PROPERY_VALUE_MAX 92

__BEGIN_DECLS

/**
 * Key properties definition
 */
typedef enum {

    EPropertyKeyVideoDecoderMaxResolution,
    EPropertyKeyVideoDecoderH264MaxLevel,
    EPropertyKeyVideoEncoderOutputBufferCount,
    EPropertyKeyVideoDecoderMPEG4InputSize,
    EPropertyKeyVideoDecoderRecyclingDelay,
    EPropertyKeyVideoDecoderDisplayOrder,
    EPropertyKeyVideoDecoderH264DefaultLevel,

} OMXIL_OSAL_PROPERTY_KEY;

/**
 * Resource ID definition
 */
typedef enum {
    /**
     * Resource to indicate the Soc DVFS OPP value
     */
    ESocDvfsOpp = 0,
    /**
     * Resource to indicate the DDR OPP value
     */
    ESocDDROpp = 1,
    /**
     * Resource to indicate the maximum latency tolerated
     */
    EPwrResLatency = 2,

} OMXIL_OSAL_RESOUCE_ID;

/**
 * Product ID definition
 */
typedef enum {
    EProductIdUnknown,
    EProductId8400,
    EProductId8500,
    EProductId9500,
    EProductId5500,
    EProductId7400,
    EProductId8500C,
    EProductId8500A,
    EProductId8520F,
    EProductId8520H,
} OMXIL_OSAL_PRODUCT_ID;

__END_DECLS

#ifdef  __cplusplus

namespace omxilosalservices {

class OmxILOsalMutex
{

  public:

    // Mutex creation
    static OMX_ERRORTYPE MutexCreate(OmxILOsalMutex* &aOmxILOsalMutex);

    // Mutex destruction
    static void MutexFree(OmxILOsalMutex* &aOmxILOsalMutex);

    // Take the mutex
    virtual void MutexLock()=0;

    // Release the mutex
    virtual void MutexUnlock()=0;

    // Try to lock the Mutex. If the lock have been performed, ENone is returned.
    // In case the mutex was already locked, then EFailed is returned
    virtual OMX_ERRORTYPE MutexTryLock()=0;

    virtual ~OmxILOsalMutex(){};
};


class OmxILOsalTimer
{

  public:

    // Returns the system time in microseconds
    static OMX_U64 GetSystemTime(void);
};

class OmxILOsalPrm
{

  public:
    static OMX_ERRORTYPE ChangeResourceState(OMXIL_OSAL_RESOUCE_ID aResourceId,
                                             OMX_U32 aValue,
                                             OMX_HANDLETYPE aClientHandle);

    static OMX_ERRORTYPE ReleaseResourceState(OMXIL_OSAL_RESOUCE_ID aResourceId,
                                              OMX_HANDLETYPE aClientHandle);

};

class OmxILOsalProductId
{

  public:

    static OMXIL_OSAL_PRODUCT_ID GetProductId();
};

class OmxILOsalProperties
{
public:

    static OMX_ERRORTYPE GetProperty(OMXIL_OSAL_PROPERTY_KEY key, char *value);
};

class OmxIlOsalDateTime {
  public:
    typedef struct {
        OMX_U16 year;       // years
        OMX_U8  month;      // range from 1 (January) to 12
        OMX_U8  day;        // from 1..31
        OMX_U8  hour;       // hours since midnight 0..23
        OMX_U8  minutes;    // minutes after hour 0..59
        OMX_U8  seconds;    // seconds after minute 0..59
    } OmxIlOsalDateTimeType;

    // Get the date and time information.
    static void OmxIlOsalGetDateTime(OmxIlOsalDateTimeType *);
};


} // end of namespace
#endif //ifdef  __cplusplus
#endif //_OMXILOSALSERVICES_H
