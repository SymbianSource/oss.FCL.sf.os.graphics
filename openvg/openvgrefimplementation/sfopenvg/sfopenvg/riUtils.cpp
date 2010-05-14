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
#   include "riUtils.h"
#endif

#include <string.h>

namespace OpenVGRI
{

/**
 * \brief   Sets mem areas to byte(s) in c.
 * \param   dst     Destination pointer.
 * \param   c       Data to set into dst.
 * \param   nElements   Amount of elements to set.
 * \param   nBytesPerElement    Amount of bytes for each element.
 * \note    This is moslty an image-settings support function. It is assumed that several
 *          bytes / elements can be set at once, especially in 3-byte case.
 */
void riMemSet32(void* dst, RIuint32 c, size_t nElements, size_t nBytesPerElement)
{
    // \todo This function should be called from a function that handles npot element sizes.
    // \todo Investigate the status of (open) std::fill implementations. Some of that code 
    // did not _seem_ to bundle sets or use SSE, etc.
    // \todo Use SSE instructions on Intel? 
    
    RI_ASSERT(dst);
    RI_ASSERT(nElements);

    switch(nBytesPerElement)
    {
    case 4:
    {
        RIuint32* ptr = (RIuint32*)dst;
        do {
            *ptr++ = c;
        } while(--nElements);
        break;
    }
    case 3:
    {
        // \todo Endianness.
        RIuint8* ptr = (RIuint8*)dst;
        RIuint8 b[3];
        b[0] = c & 0xff;
        b[1] = (c >> 8)&0xff;
        b[2] = (c >> 16)&0xff;
        do {
            *ptr++ = b[0];
            *ptr++ = b[1];
            *ptr++ = b[2];
        } while(--nElements);
        break;
    }
    case 2:
    {
        size_t dws = nElements / 2; 
        if (dws)
        {
            RIuint32* ptr32 = (RIuint32*)dst;
            dst = (void*)(((RIuint8*)dst + dws * 4));
            RIuint32 dw = c | (c<<16);
            do {
                *ptr32++ = dw;
            } while(--dws);
            nElements &= 0x01;
        }
        if (nElements)
        {
            RIuint16 *ptr16 = (RIuint16*)dst;
            const RIuint16 w = (RIuint16)c;
            do {
                *ptr16++ = w;
            } while(--nElements);
        }
    }
    case 1:
    {
        memset(dst, c, nElements);
        break;
    }
    default:
        RI_ASSERT(false);
    }

}

}

