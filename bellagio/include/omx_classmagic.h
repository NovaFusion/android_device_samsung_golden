/**
  src/base/omx_classmagic.h
	  
  This file contains class handling helper macros
  It is left as an exercise to the reader how they do the magic (FIXME)
  
  Usage Rules:
  1) include this file
  2) if your don't inherit, start your class with CLASS(classname)
  3) if you inherit something, start your class with 
  	DERIVEDCLASS(classname, inheritedclassname)
  4) end your class with ENDCLASS(classname)
  5) define your class variables with a #define classname_FIELDS inheritedclassname_FIELDS
  	inside your class and always add a backslash at the end of line (except last)
  6) if you want to use doxygen, use C-style comments inside the #define, and
  	enable macro expansion in doxyconf and predefine DOXYGEN_PREPROCESSING there, etc.
  
  See examples at the end of this file (in #if 0 block)
  
  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/
#ifndef OMX_CLASSMAGIC_H_
#define OMX_CLASSMAGIC_H_


#ifdef DOXYGEN_PREPROCESSING
#define CLASS(a) class a { public:
#define DERIVEDCLASS(a, b) class a : public b { public:
#define ENDCLASS(a) a##_FIELDS };
#else
#define CLASS(a) typedef struct a a; \
 struct a { 
#define DERIVEDCLASS(a, b) typedef struct a a; \
 struct a {
#define ENDCLASS(a) a##_FIELDS };
#endif

#if 0 /*EXAMPLES*/
/**
 * Class A is a nice class
 */
CLASS(A)
#define A_FIELDS \
/** @param a very nice parameter */ \
 	int a; \
/** @param ash another very nice parameter */ \
 	int ash;
ENDCLASS(A)
 
/**
 * Class B is a nice derived class
 */
DERIVEDCLASS(B,A)
#define B_FIELDS A_FIELDS \
/** @param b very nice parameter */ \
	int b;
ENDCLASS(B)

/**
 * Class B2 is a nice derived class
 */
DERIVEDCLASS(B2,A)
#define B2_FIELDS A_FIELDS \
/** @param b2 very nice parameter */ \
	int b2;
ENDCLASS(B2)

/**
 * Class C is an even nicer derived class.
 */
DERIVEDCLASS(C,B)
#define C_FIELDS B_FIELDS \
/** @param c very nice parameter */ \
	int c;
ENDCLASS(C)

#endif /* 0 */

#endif
