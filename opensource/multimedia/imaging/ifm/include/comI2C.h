/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _COMI2C_H
#define _COMI2C_H

#include <cm/inc/cm_type.h>

typedef enum {
	I2C_Control_None = 0,
	I2C_Control_ReadByte,
	I2C_Control_ReadWord,
	I2C_Control_ReadDWord,
	I2C_Control_WriteByte,
	I2C_Control_WriteWord,
	I2C_Control_MAX,
} e_I2C_Control_Type;

typedef struct
{
    t_uint16 I2CAddr;
    t_uint16 I2CValue;
}ts_I2CReg;

typedef struct
{
	ts_I2CReg *pReg;
	t_uint32 aNb_Element;
	t_uint32 currentNb_Element;
	t_uint32 coin;
	t_uint32 step;
	e_I2C_Control_Type controlType;
}ts_I2CCommsStruct;

#ifdef __SYMBIAN32__
NONSHARABLE_CLASS(CComI2C);
#endif
class CComI2C
{
    public:
        CComI2C();
        virtual ~CComI2C();
		ts_I2CCommsStruct I2CComStruct;
		void configure(ts_I2CReg *pRegister, t_uint32 nbElement, e_I2C_Control_Type ctlType);
};
#endif // _COMI2C_H
