/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 */

#ifndef _IGMTEST_DRAW_H_
#define _IGMTEST_DRAW_H_


/* All relative positions are expressed in FOV % x 1000
 * */

typedef enum {
    IMGTEST_DRAWCOLORFMT_NONE,
    IMGTEST_DRAWCOLORFMT_RGB565,       // 16b packed, Little-endian
    IMGTEST_DRAWCOLORFMT_RGB888,       // 24b packed
    IMGTEST_DRAWCOLORFMT_CbYCrY422     // Coplanar
} e_imgTestC_colorFormat;

typedef enum {
    IMGTEST_DRAWOBJECT_SPOT,
    IMGTEST_DRAWOBJECT_LINE,
    IMGTEST_DRAWOBJECT_RECT
} e_imgTest_drawObjectType;

typedef struct {
    unsigned long x;
    unsigned long y;
} imgTest_drawSpot;

typedef struct {
    unsigned long x0;
    unsigned long y0;
    unsigned long x1;
    unsigned long y1;
} imgTest_drawLine;

typedef struct {
    unsigned long x0;       // Topleft column
    unsigned long y0;       // Topleft line
    unsigned long x1;       // Bottomright column
    unsigned long y1;       // Bottomright line
} imgTest_drawRect;



typedef struct {
    e_imgTest_drawObjectType type;
    unsigned long RGBColor; // [0RGB] Blue is the LSB
    union {
        imgTest_drawSpot spot;
        imgTest_drawLine line;
        imgTest_drawRect rect;
    } coords;
} imgTest_drawObject;


long imgtest_drawRel2Abs(long pos, long reference);
void imgtest_drawPixel(unsigned char *pu8_buffer, long x, long y, long stride, long height, e_imgTestC_colorFormat colorFmt, unsigned long RGBColor);
void imgtest_drawLine(unsigned char *pu8_buffer, long width, long height, long stride, e_imgTestC_colorFormat colorFmt, imgTest_drawObject *pObject);
void imgTest_draw(unsigned char *pu8_buffer, long width, long height, long stride, e_imgTestC_colorFormat colorFmt, imgTest_drawObject *aObjects, int objCount);


#endif /* _IGMTEST_DRAW_H_ */
