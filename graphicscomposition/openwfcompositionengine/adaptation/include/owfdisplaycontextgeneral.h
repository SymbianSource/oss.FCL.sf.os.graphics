/* Copyright (c) 2009 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */
#ifndef OWFDISPLAYCONTEXTGENERAL_H_
#define OWFDISPLAYCONTEXTGENERAL_H_

#include <e32def.h>
#include <WF/wfc.h>
#include <KHR/khrplatform.h>
#include <owfimage.h>

#ifdef __cplusplus
extern "C" {
#endif

// Type OWF_DISPCTX is defined in <owfimage.h>

OWF_DISPCTX OWF_DisplayContext_Create(TInt screenNum);
void OWF_DisplayContext_Destroy(TInt screenNum, OWF_DISPCTX dc);

/**
Create a chunk for a new scratch buffer that is at least size bytes large.
@param  dc      The display context that owns the scratch buffer.
@param  size    The minimum size of the buffer required.
@return A pointer to the base of the new buffer if successful; otherwise null is returned. 
*/
void* OWF_DisplayContext_ScratchBuffer_Allocate(OWF_DISPCTX dc, int size);

/**
Destroys the buffer with base address aAddress
@param  dc      The display context that owns the scratch buffer
@param  buffer  A pointer to the buffer to destroy.
*/
void OWF_DisplayContext_ScratchBuffer_Destroy(OWF_DISPCTX dc, void* buffer);

/**
Increments the serial number of the display context
@param  dc      The display context who's serial number is to be incremented
*/
void OWF_DisplayContext_IncrementSerialNumber(OWF_DISPCTX dc);

/**
Sets the composition ongoing flag
@param  dc      The display context who's composition ongoing flag is to be set
@param val      The boolean value
*/
void OWF_DisplayContext_SetCompositionOngoing(OWF_DISPCTX dc, WFCboolean val);

#ifdef __cplusplus
}
#endif

#endif /* OWFDISPLAYCONTEXTGENERAL_H_ */
