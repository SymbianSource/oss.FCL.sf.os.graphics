/* Copyright (c) 2009-2010 The Khronos Group Inc.
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

#include <WF/wfc.h>
#include <e32debug.h>

#include "wfcstructs.h"
#include "wfcdevice.h"
#include "owfmutex.h"
#include "owfdisplaycontext.h"
#include "owfdisplaycontextgeneral.h"
#include <graphics/symbianstream.h>

static const TInt updateReceiverPriorityTable[] = {10, 9, 8, 7, 6, 5, 4, 3};

/* Arbitrary internal version number for SymbianStream registration. */
static const TInt KInternalVersion = 4;

OWF_DISPCTX OWF_DisplayContext_Create(TInt screenNum)
    {
    TInt priorityTableSize = sizeof(updateReceiverPriorityTable) / sizeof(updateReceiverPriorityTable[0]);
    TInt priority = 0;
    
    OWFDisplayContext *pDispCtx = new OWFDisplayContext;
    if (!pDispCtx)
        {
        RDebug::Printf("Not enough memory to create OWFDisplayContext object");
        return NULL;
        }
    
    pDispCtx->iEventServerUpdate  = 0;
    pDispCtx->iCompositionOngoing = WFC_FALSE;
    pDispCtx->iPreviousCommit     = WFC_FALSE;
    pDispCtx->iSerialNumber       = INITIAL_CONTEXT_SERIAL_NUMBER;
    
    if (screenNum != OWF_RESERVED_BAD_SCREEN_NUMBER)
        {
        if((screenNum < 0) || (screenNum >= priorityTableSize))
            {
            RDebug::Printf("Can't retrieve backend priority for the screen number: %d\n", screenNum);
            }
        else
            {
            priority = updateReceiverPriorityTable[screenNum];
            }
		// False positive leaving function is in a TRAPD
		// coverity[leave_without_push]
        TInt err = SymbianStreamRegisterScreenNotifications(screenNum, priority, KInternalVersion);
        if (!(err == KErrNone || err == KErrNotReady || err == KErrAlreadyExists))
            {
            RDebug::Printf("Register with Surface Update Server failed for screen: %d err=%d\n", screenNum, err);
            delete pDispCtx;
            return NULL;
            }
        if (err == KErrNotReady)
            {
            RDebug::Printf("Surface Update Server is not ready for screen: %d\n", screenNum);
            }
        if (err == KErrAlreadyExists)
            {
            RDebug::Printf("Backend object already exists for screen: %d\n", screenNum);
            }
        }
    
    if (screenNum >= 0)
        {
        pDispCtx->iDisplayContext = COwfcDisplayDevice::SingletonInstance();
        if (!pDispCtx->iDisplayContext)
            {
            RDebug::Printf("Failed to initialise display singleton\n");
            delete pDispCtx;
            return NULL;
            }

        TInt err = pDispCtx->iDisplayContext->OpenScreen(screenNum);
        if (err != KErrNone)
            {
            RDebug::Printf("Failed to initialise display driver on screen: %d err=%d\n", screenNum, err);
            delete pDispCtx;
            return NULL;
            }
        }
    else
        {
        // Off-screen context
        pDispCtx->iDisplayContext = NULL;
        }
    
    return static_cast<OWF_DISPCTX>(pDispCtx);
    }

void OWF_DisplayContext_Destroy(TInt screenNum, OWF_DISPCTX dc)
    {
    if (dc != OWF_INVALID_HANDLE)
        {
        OWFDisplayContext *pDispCtx = _OWF_DISPLAYCONTEXT(dc);
        if (screenNum >= 0)
            {
            OWFboolean canDelete = OWF_FALSE;
            pDispCtx->iDisplayContext->CloseScreen(screenNum, &canDelete);
            if (canDelete == OWF_TRUE)
                {
                delete pDispCtx->iDisplayContext;
                }
            }
        delete pDispCtx;
        }
    
    if (screenNum != OWF_RESERVED_BAD_SCREEN_NUMBER)
        {
        TInt err = SymbianStreamUnregisterScreenNotifications(screenNum);
        if (!(err == KErrNone || err == KErrNotReady))
            {
            RDebug::Printf("Unregister with Surface Update Server failed for screen: %d\n", screenNum);
            }
        }
    }

OWFDisplayContext_::~OWFDisplayContext_()
    {
    }

void* COwfScratchBuffers::Allocate(TInt aMinSize)
/**
Create a chunk for a new scratch buffer that is at least aMinSize bytes large.
@param  aMinSize    The minimum size of the buffer required.
@return A pointer to the base of the new buffer if successful; otherwise null is returned. 
*/
    {
    TBufferAddress buf;
    if (buf.iChunk.CreateLocal(aMinSize, aMinSize) != KErrNone) 
        {
        return 0;
        }    
    buf.iAddress = buf.iChunk.Base();
    buf.iOffset = 0;
    return ((iBuffers.Append(buf) == KErrNone) ? buf.iAddress : 0);
    }

void COwfScratchBuffers::Destroy(void* aAddress)
/**
Destroys the buffer with base address aAddress
@param  aAddress    The address of the buffer to destroy
*/
    {
    for (TInt i = iBuffers.Count() - 1; i >= 0; --i)
        {
        if (iBuffers[i].iAddress == static_cast<TUint8*>(aAddress))
            {
            iBuffers[i].iChunk.Close();
            iBuffers.Remove(i);
            break;
            }
        }
    }
    
COwfScratchBuffers::~COwfScratchBuffers()
/**
Destructor
Close all the chunks.
*/
    {
    for (TInt i = iBuffers.Count() - 1; i >= 0; --i)
        {
        iBuffers[i].iChunk.Close();
        }
    iBuffers.Close();
    }    

void* OWF_DisplayContext_ScratchBuffer_Allocate(OWF_DISPCTX dc, int size)
    {
    if (!dc)
        {
        return NULL;
        }
    OWFDisplayContext *pDispCtx = _OWF_DISPLAYCONTEXT(dc);
    return pDispCtx->iBuffers.Allocate(size);
    }

void OWF_DisplayContext_ScratchBuffer_Destroy(OWF_DISPCTX dc, void* buffer)
    {
    if (!dc)
        {
        return;
        }
    OWFDisplayContext *pDispCtx = _OWF_DISPLAYCONTEXT(dc);
    pDispCtx->iBuffers.Destroy(buffer);
    }

void OWF_DisplayContext_IncrementSerialNumber(OWF_DISPCTX dc)
    {
    if (!dc)
        {
        return;
        }
    OWFDisplayContext *pDispCtx = _OWF_DISPLAYCONTEXT(dc);
    pDispCtx->iSerialNumber++;
    if (pDispCtx->iSerialNumber == INITIAL_CONTEXT_SERIAL_NUMBER)
        {
        pDispCtx->iSerialNumber++;
        }
    }

void OWF_DisplayContext_SetCompositionOngoing(OWF_DISPCTX dc, WFCboolean val)
    {
    if (!dc)
        {
        return;
        }
    OWFDisplayContext *pDispCtx = _OWF_DISPLAYCONTEXT(dc);
    pDispCtx->iCompositionOngoing = val;    
    }
