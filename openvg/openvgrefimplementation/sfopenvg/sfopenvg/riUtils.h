/*------------------------------------------------------------------------
 *
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 *//*-------------------------------------------------------------------*/

#ifndef __RIUTILS_H_
#define __RIUTILS_H_

#ifndef __RIDEFS_H
#   include "riDefs.h"
#endif

// This file contains "utility" functions that did not "fit" into existing RI files.
// Once more functionality is accumulated, the corresponding functions/classes should be
// moved to proper files asap. For example, the memcopy functions could go into file
// "riMemory.xxx".

namespace OpenVGRI
{

void riMemSet32(void* dst, RIuint32 c, size_t nElements, size_t nBytesPerElement);

}

#endif

