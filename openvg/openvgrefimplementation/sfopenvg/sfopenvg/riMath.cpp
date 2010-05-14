/*------------------------------------------------------------------------
 *
 * OpenVG 1.1 Reference Implementation
 * -----------------------------------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 * Portions Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief	Implementation of non-inline matrix functions.
 * \note	
 *//*-------------------------------------------------------------------*/

#include "riDefs.h"
#include "riMath.h"

#if 0
#include <stdio.h>

static void printMatrix(const Matrix3x3& m)
{
    // For tracing a bug in matrix inverse in release-builds.
    for(int i = 0; i < 3; i++)
    {
        printf("[%.4f %.4f %.4f]\n", m[i][0], m[i][1], m[i][2]);
    }
}

#endif

namespace OpenVGRI
{

/*-------------------------------------------------------------------*//*!
* \brief	Inverts a 3x3 matrix. Returns false if the matrix is singular.
* \param	
* \return	
* \note		
*//*-------------------------------------------------------------------*/

bool Matrix3x3::invert()
{
    // \todo Save computation on affine matrices?
	bool affine = isAffine();
	RIfloat det00 = matrix[1][1]*matrix[2][2] - matrix[2][1]*matrix[1][2];
	RIfloat det01 = matrix[2][0]*matrix[1][2] - matrix[1][0]*matrix[2][2];
	RIfloat det02 = matrix[1][0]*matrix[2][1] - matrix[2][0]*matrix[1][1];

	RIfloat d = matrix[0][0]*det00 + matrix[0][1]*det01 + matrix[0][2]*det02;
	if( d == 0.0f ) return false;	//singular, leave the matrix unmodified and return false
	d = 1.0f / d;

	Matrix3x3 t;

    // \note There is some bug (in GCC?) in accessing matrix elements: If data
    // is accessed like: t[i][j], then the following will produce incorrect
    // resulst on optimized builds. If the data is accessed through t.matrix,
    // then the output is correct. Debug build works correctly, and if print
    // calls are inserted, the code also works correctly. The context to get
    // this bug appear are fill paints (linear and radial gradient test
    // functions).

	t.matrix[0][0] = d * det00;
	t.matrix[1][0] = d * det01;
	t.matrix[2][0] = d * det02;
    //printf("t\n");
    //printMatrix(t);
	t.matrix[0][1] = d * (matrix[2][1]*matrix[0][2] - matrix[0][1]*matrix[2][2]);
	t.matrix[1][1] = d * (matrix[0][0]*matrix[2][2] - matrix[2][0]*matrix[0][2]);
	t.matrix[2][1] = d * (matrix[2][0]*matrix[0][1] - matrix[0][0]*matrix[2][1]);
	t.matrix[0][2] = d * (matrix[0][1]*matrix[1][2] - matrix[1][1]*matrix[0][2]);
	t.matrix[1][2] = d * (matrix[1][0]*matrix[0][2] - matrix[0][0]*matrix[1][2]);
	t.matrix[2][2] = d * (matrix[0][0]*matrix[1][1] - matrix[1][0]*matrix[0][1]);
	if(affine)
		t[2].set(0,0,1);	//affine matrix stays affine
	*this = t;
	return true;
}

//==============================================================================================

}	//namespace OpenVGRI
