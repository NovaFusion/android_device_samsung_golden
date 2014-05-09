/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _SENSOR_H
#define _SENSOR_H

#ifndef __SYMBIAN32__
#include <string.h>
#endif
#include "comI2C.h"
#include <cm/inc/cm_type.h>
#include "host/ispctl_types.idt.h"
#include "osi_trace.h"
#include "ENS_DBC.h"

#define MAX_PREBOOT_PE          32
#define MAX_POSTBOOT_PE         32
#define MAX_SPECIFIC_PE         32
#define PATCH_SIZE_SEQUENCE     16

/* aspect ratio filters */
#define ASPECT_RATIO_4TO3   0x1
#define ASPECT_RATIO_16TO9  0x2
#define ASPECT_RATIO_1TO1   0x4
#define ASPECT_RATIO_3TO2   0x8
#define ASPECT_RATIO_OTHER  0x10
#define ASPECT_RATIO_ALL    0xffffffff

enum enumCameraSlot
{
	ePrimaryCamera,
	eSecondaryCamera,
};


enum enumSensorAlign
{
	e_align_none,
	e_align_raw_r_g,
	e_align_raw_g_r,
	e_align_raw_b_g,
	e_align_raw_g_b,
	e_align_LastEnum,
};

typedef struct sMaxZoom
{
	t_uint16 X_resolution;
	t_uint16 Xplus_maxResolution;   
	t_uint16 Y_resolution;
	t_uint16 Yplus_maxResolution;   
	float maxZoom;
	float rotated_maxZoom;
	float plus_maxZoom;
	float rotated_plus_maxZoom;  
} sMaxZoom;

/** Sensor ID as it should eventually be provided by the FW before boot. */
typedef struct
{
    t_uint16 model;
    t_uint8  manufacturer;
    t_uint8  revisionNumber;
    t_uint8  Fuse_Id[3]; // FuseId 1 to 3
    t_uint32 Fuse_Id4;  // FuseID 4
} t_sensorDesciptor;

#define MAX_SENSOR_RESOLUTIONS 30

typedef struct
{
  t_uint16 fov_x;
  t_uint16 fov_y;
  t_uint16 x;
  t_uint16 y;
} t_sensorResolution;

typedef struct
{
  t_sensorResolution res[MAX_SENSOR_RESOLUTIONS];
  int count;
} t_sensorResolutions;

typedef struct
{
    t_uint32                FocalLength;
    t_uint16                HorizontolViewAngle;
    t_uint16                VerticalViewAngle;
} t_LensParameters;
/*****************************/
/* Sensor object declaration */
/*****************************/

/** \brief abstract class implementing Sensor specifics parameters */
#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CSensor);
#endif
class CSensor
{
    public:
        CSensor(enumCameraSlot slot);   /* Standard Constructor*/
        virtual ~CSensor();             /* Standard destructor */

        ts_PageElement*         GetSensorPreBootTable(t_uint32* aNb_Element);
        ts_PageElement*         GetSensorPostBootTable(t_uint32* aNb_Element);
        t_uint16                GetSensorPeByIndex(t_uint16 index);
        ts_I2CReg*              GetSensorPatchTable(t_uint32* aNb_Element);
        ts_PageElement*         GetSensorSpecific  (t_uint32* aNb_Element);
        enumCameraSlot          GetCameraSlot() { return i_CamSlot; }
        void                    SetFocalLengthMm(float);
        float                   GetFocalLengthMm();
        void                    SetSensorDescriptor(t_sensorDesciptor *pDesc );
        t_uint8                 GetFuseId1(void);
        t_uint8                 GetFuseId2(void);
        t_uint8                 GetFuseId3(void);
        t_uint32                GetFuseId4(void);
        t_uint16                GetSensorModel(void);
        t_uint8                 GetSensorManufacturer(void);
        t_uint8                 GetSensorRevNumber(void);

        virtual t_uint32        getBayerWidth(void);
        virtual t_uint32        getBayerHeight(void);
        virtual t_uint8         getNberOfNonActiveLines(void);
        void                    setBayerWidth(t_uint32 width);
        void                    setBayerHeight(t_uint32 height);
        void                    setNberOfNonActiveLines(t_uint8 nber);
        virtual t_sint32 		GetSensorId(void)=0;
        virtual const char*     GetPeName(const t_uint32 aAddr)=0;
        void                    GetLensParameters(t_LensParameters *LensParameters);

        void                    setSensorModesPointer(t_uint8* pSensorMode);
        t_uint8*                getSensorModesPointer(void);
        void                    setSensorModesNber(t_uint16 SensorModesNber);
        t_uint16                getSensorModesNber(void);

	virtual bool            getResolutions(t_sensorResolutions *resolutions);

	static void             filterResolutionsAspectRatio(t_sensorResolutions *resolutions, int filter);

    protected:
        float                   iFocalLengthMm;
        t_uint32                iSensorPreBootNbElement;
        t_uint32                iSensorPostBootNbElement;
        t_uint32                iSensorSpecificNbElement;
        t_uint32                iSensorPatchSequenceNbElement;

        ts_PageElement          iSensorPreBootPEs[MAX_PREBOOT_PE];
        ts_PageElement          iSensorPostBootPEs[MAX_POSTBOOT_PE];
        ts_PageElement          iSensorSpecificPEs[MAX_SPECIFIC_PE];

        ts_I2CReg               iSensorPatchSequence[PATCH_SIZE_SEQUENCE];

        enumCameraSlot          i_CamSlot;                // Set to primary or secondary camera
        t_sensorDesciptor       i_SensorDescriptor;
        t_uint32                i_BayerWidth;
        t_uint32                i_BayerHeight;            // bayer height, including i_NberofNonActiveLines
        t_uint8                 i_NberofNonActiveLines;
        
        t_uint8*                pSensorModes;
        t_uint16                iSensorModesNber;
        t_LensParameters        iLensParameters;
};
#endif
