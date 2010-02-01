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

#ifndef OWFDISPLAYCONTEXT_H
#define OWFDISPLAYCONTEXT_H

#include <e32base.h>

#include <dispchannel.h>
#include "owfmemory.h"
#include "owfscreen.h"
#include "owfdisplaycontextgeneral.h"

#define _OWF_DISPLAYCONTEXT(x)          (static_cast<OWFDisplayContext *>(x))

struct TBufferAddress
    {
    RChunk  iChunk;
    TInt    iOffset;
    TUint8* iAddress;
    };

/**
Container that allocates and destroys the chunks used by the scratch buffers.
*/
class COwfScratchBuffers : public CBase
    {
public:
    void* Allocate(TInt aMinSize);
    void Destroy(void* aAddress);
    ~COwfScratchBuffers();
private:
    RArray <TBufferAddress> iBuffers;
    };

class COWFScreenDisplayInfo : public CBase
    {
public:
    ~COWFScreenDisplayInfo();
    static COWFScreenDisplayInfo* NewL(TInt aScreen);
    TInt ScreenNumber() const;
    void GetScreenInfo(OWF_SCREEN *header) const;
    OWFboolean Blit(void* buffer, OWF_ROTATION rotation);
    
private:
    COWFScreenDisplayInfo(TInt aScreen);
    void ConstructL();
private:
    OWF_SCREEN iScreenInfo;
    mutable RDisplayChannel iDispChan;
    TInt iScreenNumber;
    RArray <TBufferAddress*> iCompositionBuffer;
    RDisplayChannel::TDisplayRotation iDefaultRotation;
    TInt iRotationOffset;
    };
    
class COwfcDisplayDevice : public CBase
    {
private:
    static COwfcDisplayDevice* gDisplayDeviceInstancePtr;
public:
    static COwfcDisplayDevice* SingletonInstance();
    TInt OpenScreen(TInt aScreen);
    TInt CloseScreen(TInt aScreen, OWFboolean *aCanDelete);
    ~COwfcDisplayDevice();
public:
    static TBool ScreenCompare(const TInt* k, COWFScreenDisplayInfo* const & t)
    {return (*k == t->ScreenNumber());}
    RArray <COWFScreenDisplayInfo*> iScreenDisplayInfo;
    };

typedef struct OWFDisplayContext_
    {
public:
    COwfcDisplayDevice *iDisplayContext;
    COwfScratchBuffers iBuffers;
    WFCint iEventServerUpdate;
    WFCboolean iCompositionOngoing;
    WFCint iPreviousCommit;
    WFCint iSerialNumber;
public:
    virtual ~OWFDisplayContext_();
    } OWFDisplayContext;

#endif //OWFDISPLAYCONTEXT_H
