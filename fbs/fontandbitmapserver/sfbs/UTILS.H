// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifndef __UTILS_H__
#define __UTILS_H__

#include <e32std.h>
#include <gdi.h>
#include <fntstore.h>
#include <bitmap.h>



/**
@internalComponent
*/ 
_LIT(KFBSERVPanicCategory,"FBSERV");
_LIT(KFBSERVGlobalThreadName,"!Fontbitmapserver");
_LIT(KFBSERVSharedChunkName,"FbsSharedChunk");
_LIT(KFBSERVLargeChunkName,"FbsLargeChunk");
#ifdef SYMBIAN_DEBUG_FBS_LOCKHEAP
_LIT(KFBSERVDebugMutexName,"FbsDebugMutex");
#endif

/**
@internalComponent
*/
const TUid KUidCFbsBitmapCreation={0x0009A2C};
/**
@internalComponent
*/
enum TFbsPanic
	{
	EFbsPanicNoConnection=1,
	EFbsPanicBadConnection,
	EFbsPanicUnknownMessage,
	EFbsPanicActiveSchedulerError,
	EFbsPanicStartupFailed,
	EFbsTypefaceStoreError,
	EFbsFontCreateFailed,
	EFbsFontAddressViolation,
	EFbsBitmapInvalidFormat,
	EFbsBitmapInvalidMode,
	EFbsBitmapInvalidCompression,
	EFbsBitmapDecompressionError,
	EFbsPanicChunkError,
	EFbsColor256UtilError,
	EFbsHardwareBitmapError,
	EFbsNotSupportedForCompression,
	EFbsBitmapAlignment,
	EFbsBitmapInvalidScanLinePtr,
	EFbsBitmapInvalidMode2,
	EFbsBitmapInvalidMode3,
	EFbsBitmapSwappingImpossible,
	EFbsPanicBadHeapLock,
	EFbsShaperNotYetCalled,
	EFbsShaperInvalidArguments,
	EFbsInvalidCompressionThreshold,
	EFbsTypefaceIndexOutOfRange,
	EFbsPanicBitmapDataCopy,
	EFbsPanicBitmapReadOnly,
	EFbsPanicInvalidBitmapType
	};

class TScanLine
/**
@internalComponent
*/
	{
public:
	TPoint iPoint;
	TInt iLength;
	TDisplayMode iDispMode;
	};

class TBmpSpec
/**
@internalComponent
*/
	{
public:
	TSize iSizeInPixels;
	TDisplayMode iDispMode;
	TInt iHandle;
	TInt iServerHandle;
	TInt iAddressOffset;
	};

class TBmpHandles
/**
@internalComponent
*/
	{
public:
	TInt iHandle;
	TInt iServerHandle;
	TInt iAddressOffset;
	};

class TFontInfo
/**
@internalComponent
*/
	{
public:
	TInt iHandle;
	TInt iAddressOffset;
	TInt iServerHandle;
	};

class TSizeInfo
/**
@internalComponent
*/
	{
	public:
		TSizeInfo(){}
		TSizeInfo(TInt aMaxHeight,TSize aDevSize):iMaxHeight(aMaxHeight),iDevSize(aDevSize) {}

	TInt iMaxHeight;
	TSize iDevSize;
	};

class TIntParcel
/**
@internalComponent
*/
	{
public:
	TInt iInt;
	};

class TLoadBitmapArg
/**
@internalComponent
*/
	{
public:
	TInt iBitmapId;
	TInt iShareIfLoaded;
	TInt iFileOffset;
	};


IMPORT_C extern const TInt KFbServLargeChunkGrowByShifter;
IMPORT_C extern const TInt KFbServLargeChunkSizeShifter;
IMPORT_C extern const TInt KFbServLargeChunkMinPhysicalSize;
IMPORT_C extern const TInt KFbServLargeChunkMinVirtualSize;
IMPORT_C extern const TInt KFbServLargeChunkMaxVirtualSize;


class CChunkPile : public CBase
/**
@internalComponent
*/
	{
public:
	IMPORT_C ~CChunkPile();
	IMPORT_C static CChunkPile* NewL(const RChunk& aChunk);
	IMPORT_C TUint8* Alloc(TInt aSize);
	IMPORT_C void Free(TAny* aCell);
	IMPORT_C TUint8* ChunkBase() const;
	IMPORT_C static TInt VirtualSize();
private:
	CChunkPile(const RChunk& aChunk);
	void ConstructL();
	TInt DoAlloc(TUint8*& aCell, TInt aSize, RPointerArray<TUint8>& aCells, RArray<TInt>& aFreeCellLinks, TBool aLarge);
	void DoFree(TUint8* aCell, RPointerArray<TUint8>& aCells, RArray<TInt>& aFreeCellLinks, TBool aLarge);
	TInt GrowSmallSection(TInt aSize, TInt aLastFreeCell);
	void ShrinkSmallSection(TInt aShrinkBy);
private:
	RChunk iChunk;
	TInt iPageSize, iPageMask;
	RPointerArray<TUint8> iSmallCells; // last address is not a cell but the top of the small section
	RArray<TInt> iFreeSmallCellLinks; // index of Nth free small cell is element 0 + element 1 + ... + element N-1
	TInt iLargeSectionBottom; // boundary between small & large sections
	RPointerArray<TUint8> iLargeCells; // last address is not a cell but the top of the large section
	RArray<TInt> iFreeLargeCellLinks; // index of Nth free large cell is element 0 + element 1 + ... + element N-1
	RCriticalSection iLock;
	};

/**
Used to pass sizes of FBServs default heap, heap for large bitmaps, and heap for small bitmaps.

@internalComponent
@test
*/
struct THeapSizes
	{
	TInt iDefault;
	TInt iBig;
	TInt iSmall;
	};

/**
Used when the EFbsMessRasterize message is sent from client to server. Allows pointers to items
created on the shared FBServ heap to be passed from the server to the client as offsets from the
heap base. Valid pointers are recreated client side using these offsets combined with the client's
heap base.

@internalComponent 
 */
struct TRasterizeParams
	{
	TInt iMetricsOffset;
	TInt iBitmapPointerOffset;
	};

IMPORT_C extern const TInt KFbServWritableDataPagingMode;

/**
Used to specify the writable data paging mode of fbserv.
This may be set using the patchable constant KFbServWritableDataPagingMode at rom-build time via the definitions: 
	SYMBIAN_GRAPHICS_FBSERV_PAGEDATA -request that fbserv writable data is paged.
	SYMBIAN_GRAPHICS_FBSERV_UNPAGEDDATA -request that fbserv writable data is unpaged.
	SYMBIAN_GRAPHICS_FBSERV_PAGE_BITMAP_DATA_ONLY -request that only bitmap data is paged.
	SYMBIAN_GRAPHICS_FBSERV_PAGE_BITMAP_DATA_AND_SHARED_HEAP_ONLY -request that only bitmap data and shared heap are paged.
@internalComponent
*/
enum TFbsWritableDataPagingMode
	{
	EFbsWdpDefault=0x00, //No change to the rom-build writable data paging configuration (nothing was specified).
	EFbsWdpPagedData=0x01, //SYMBIAN_GRAPHICS_FBSERV_PAGEDATA was specified.
	EFbsWdpUnpagedData=0x02, //SYMBIAN_GRAPHICS_FBSERV_UNPAGEDDATA was specified.
	EFbsWdpPageBitmapDataChunkOnly=0x03, //SYMBIAN_GRAPHICS_FBSERV_PAGE_BITMAP_DATA_ONLY was specified.
	EFbsWdpPageBitmapDataAndSharedHeapChunksOnly=0x04, //SYMBIAN_GRAPHICS_FBSERV_PAGE_BITMAP_DATA_AND_SHARED_HEAP_ONLY was specified.
	};


/*
Structure used to pass parameters from FBS client to FBS server.
Used when opening/closing an outline iterator.
@internalTechnology
*/
class TFBSGlyphOutlineParam 
    {
public:
    TInt iHandle;
    TInt iCount;
    TBool iHinted;
    };

/*
Structure used to pass font table/glyph outlines from FBS to FBS client.
Used when opening an font table or an outline iterator.
@internalTechnology
*/
class TOffsetLen 
    {
public:
    TInt iOffset;
    TInt iLen;
    };

#endif


