// Copyright (c) 1995-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <fntstore.h>
#include <bitmap.h>
#include <openfont.h>
#include "fbsmessage.h"
#include "SERVER.H"
#include "BackGroundCompression.h"
#include <shapeinfo.h>
#include <graphics/shaperparams.h>

/** Helper function for converting a pointer to an offset from the passed
heap base. Use OffsetToPointer() to convert the returned offset back to a
useable pointer.
@param aAny A pointer to be converted to an offset.
@param aHeapBase The heap base of the current process.
@return An offset representing the passed pointer that can be converted
back to a pointer using the function OffsetToPointer(). 
@see OffsetToPointer()
 */
LOCAL_C TInt PointerToOffset(const TAny* aAny, TInt aHeapBase)
	{
	if (aAny && aHeapBase)
		{
		return reinterpret_cast<TInt>(aAny) - aHeapBase;
		}
	return 0;
	}

/** Helper function for converting an offset (that was calculated using
PointerToOffset()) back to a pointer relative to the passed heap base.
@param aOffset The offset to be converted to a pointer.
@param aHeapBase The heap base of the current process.
@return A pointer relative to the passed heap base.
@see PointerToOffset()
 */
LOCAL_C TAny* OffsetToPointer(TInt aOffset, TInt aHeapBase)
	{
	if (aOffset && aHeapBase)
		{
		return reinterpret_cast<TAny*>(aOffset + aHeapBase);
		}
	return NULL;
	}

CFbClient::CFbClient(RHeap* aHeap):
	CSession2(),
	iConnectionHandle(0),
	iIx(NULL),
	iResourceCount(0),
	iHeap(aHeap)
#ifdef _DEBUG
	,iOwnHeapFailNumber(-1),
	iSharedHeapFailNumber(-1)
#endif	
	{
	}

CFbClient* CFbClient::NewL(RHeap* aHeap)
	{
	CFbClient* c = new(ELeave) CFbClient(aHeap);
	c->iOpenFontGlyphData = TOpenFontGlyphData::New(aHeap,4 * 1024);
	if (!c->iOpenFontGlyphData)
		{
		delete c;
		User::Leave(KErrNoMemory);
		}
	return c;
	}

CFbClient::~CFbClient()
	{
	/*
	Don't call any CFontStore functions if CFbClient::NewL has left, or if FBSERV has already deleted the
	font store, which happens in test programs like TFBS when FBSERV is closed before the client(s).
	*/
	CFontStore* font_store = NULL;
	CFbTop* fbTop = TopLevelStore();
	if (fbTop)
		{
		font_store = fbTop->FontStore();
		}
	
	if (font_store)
		font_store->DeleteSessionCache(iSessionHandle);
	
	// If the font store doesn't exist, neither will the shared heap owned by FBSERV.
	if (font_store)
		iHeap->Free(iOpenFontGlyphData);
	
	// delete fonts hold by the client
	delete iIx;
	
	// delete font files hold by the client
    if (iFontFileIndex)
        {
        TInt count = iFontFileIndex->Count();
        for (TInt index = 0;index < count; index++)
            {
            if (font_store)
                font_store->RemoveFile(iFontFileIndex->At(0).iUid);
            iFontFileIndex->Delete(0);
            }
        delete iFontFileIndex;
        }

	// Close the buffer used to hold the text thats needs shaping.
	iTextToShape.Close();
	}

void CFbClient::Init(TUint aConnectionHandle)
	{
	iConnectionHandle=aConnectionHandle;
	}

void CFbClient::CreateL()
	{
	iIx=CObjectIx::NewL();
	iFontFileIndex=new(ELeave) CArrayFixFlat<TFontFileIndex>(1);
	}

CFontBitmapServer* CFbClient::FontBitmapServer()
	{
	return((CFontBitmapServer*)Server());
	}

CFbTop* CFbClient::TopLevelStore()
	{
	CFontBitmapServer* server = FontBitmapServer();
	if (server)
		return server->TopLevelStore();
	else
		return NULL;
	}

void CFbClient::CopyFontInfo(CFontObject* aFontObjPtr,TInt aHandle,TFontInfo& aFontInfo)
	{
	CBitmapFont* font=aFontObjPtr->iAddressPointer;
	aFontInfo.iHandle = aHandle;
	aFontInfo.iServerHandle=(TInt)aFontObjPtr;
	aFontInfo.iAddressOffset=TInt(font)-TopLevelStore()->HeapBase();
	}

void CFbClient::ServiceL(const RMessage2& aMessage)
	{
#ifdef _DEBUG
	TBool resetOwnHeap=EFalse;
	TBool resetSharedHeap=EFalse;
	
	//the memory tests have been mostly written to have start and end memory
	//function calls (or macros) at the start of this function call (ServiceL)
	//and the matching call (or macro) is at the end of this function.
	
	if (iOwnHeapFailNumber!=-1)
		{
		//if this not -1 then know to set the heap to fail on
		//the given number of allocations
		
		resetOwnHeap=ETrue;
		__UHEAP_SETFAIL(RHeap::EDeterministic,iOwnHeapFailNumber);
		}
	if (iSharedHeapFailNumber!=-1)
		{
		resetSharedHeap=ETrue;
		__RHEAP_SETFAIL (iHeap, RHeap::EDeterministic,iSharedHeapFailNumber);
		}	

	if (iOwnHeapCheck)
		{
		__UHEAP_MARK;
		}					
	if (iHeapCheck)
		{
		__RHEAP_MARK(iHeap);
		}		
#endif		
	
	switch(aMessage.Function())
		{
// client messages
	case EFbsMessInit:
		Init(FontBitmapServer()->Init());
		iSessionHandle = (TInt)this;
		aMessage.Complete(iSessionHandle);
#ifdef _DEBUG
		iRet=KErrNone;
#endif		
		break;
	case EFbsMessClose:
		{
		TInt localhandle=aMessage.Int0();
		if(!iIx->At(localhandle))
			{
			aMessage.Panic(KFBSERVPanicCategory,KErrNotFound);
			break;
			}
		iIx->Remove(localhandle);
		iResourceCount--;
		aMessage.Complete(KErrNone);
#ifdef _DEBUG
		iRet=KErrNone;
#endif				
		break;
		}
	case EFbsMessResourceCount:
		aMessage.Complete(iResourceCount);
#ifdef _DEBUG
		iRet=iResourceCount;
#endif			
		break;
// server messages
	case EFbsMessShutdown:
	case EFbsMessNumTypefaces:
	case EFbsMessTypefaceSupport:
	case EFbsMessFontHeightInTwips:
	case EFbsMessFontHeightInPixels:
	case EFbsMessSetPixelHeight:
	case EFbsMessDefaultAllocFail:
	case EFbsMessDefaultMark:
	case EFbsMessDefaultMarkEnd:
	case EFbsMessUserAllocFail:
	case EFbsMessUserMark:
	case EFbsMessUserMarkEnd:
	case EFbsMessHeapCheck:
	case EFbsMessSetDefaultGlyphBitmapType:
	case EFbsMessGetDefaultGlyphBitmapType:
	case EFbsMessFontNameAlias:
	case EFbsMessGetHeapSizes:
	case EFbsMessDefaultLanguageForMetrics:
	case EFbsCompress:
	case EFbsMessFetchLinkedTypeface:
	case EFbsMessRegisterLinkedTypeface:
	case EFbsMessUpdateLinkedTypeface:
#ifdef _DEBUG
		FontBitmapServer()->ProcMessage(aMessage,iSessionHandle,iRet);
#else
		FontBitmapServer()->ProcMessage(aMessage,iSessionHandle);
#endif			
		break;
// font messages
	case EFbsMessFontDuplicate:
	case EFbsMessGetNearestFontToDesignHeightInTwips:
	case EFbsMessGetNearestFontToDesignHeightInPixels:
	case EFbsMessGetNearestFontToMaxHeightInTwips:
	case EFbsMessGetNearestFontToMaxHeightInPixels:
	case EFbsMessGetFontById:
	case EFbsMessInstallFontStoreFile:
	case EFbsMessAddFontStoreFile:
	case EFbsMessRemoveFontStoreFile:
	case EFbsMessRasterize:
	case EFbsMessFaceAttrib:
	case EFbsMessHasCharacter:
	case EFbsMessShapeText:
	case EFbsMessShapeDelete:
	case EFbsMessSetTwipsHeight:
	case EFbsMessGetTwipsHeight:
	case EFbsSetSystemDefaultTypefaceName:
#if (_DEBUG)
	case EFbsMessSetDuplicateFail:
#endif
		ProcFontMessage(aMessage);
		break;
// bitmap messages
	case EFbsMessBitmapCreate:
	case EFbsMessBitmapResize:
	case EFbsMessBitmapDuplicate:
	case EFbsMessBitmapLoad:
	case EFbsMessBitmapCompress:
	case EFbsMessBitmapBgCompress:
	case EFbsMessBitmapClean:
	case EFbsGetAllBitmapHandles:
	case EFbsMessBitmapLoadFast:
	case EFbsMessBitmapNotifyDirty:
	case EFbsMessBitmapCancelNotifyDirty:
		ProcBitmapMessage(aMessage);
		break;
//Memory messages		
	case EFbsMessSetHeapFail:
	case EFbsMessHeapCount:
	case EFbsMessSetHeapReset:
	case EFbsMessSetHeapCheck:
	case EFbsMessHeap:
#ifdef _DEBUG	
		ProcMemMessage(aMessage);
#else
		aMessage.Complete(KErrNone);
#endif	
		break;
	default:
		aMessage.Panic(KFBSERVPanicCategory,KErrArgument);
		break;
		}
#ifdef _DEBUG

	//do not want a memory panic if the return code is OK
	//__UHEAP_MARKEND does this
	
	if (iOwnHeapCheck)
		{
		TUint32 badCell=User::Heap().__DbgMarkEnd(0);
		if (iRet<0)
			{
			//if the function call was a success, there may have been valid memory allocations,
			//so only panic if the call failed, and the memory is not the same before and after the
			//function call
			__ASSERT_DEBUG (badCell==NULL,User::Panic(KFBSERVPanicCategory, KErrGeneral));
			}
		}					
	if (iHeapCheck)
		{
		TUint32 badCell2=0;
		badCell2= __RHEAP_MARKEND(iHeap);
		//the above function call does not panic if there is a failure, this is
		//only for the user heap
		if (iRet<0)
			{
			__ASSERT_DEBUG (badCell2==NULL,User::Panic(KFBSERVPanicCategory, KErrGeneral));
			}
		}
	//change the state of the memory check?	
	if (iOwnHeapCheckFlip)
		{
		//change the state of the memory testing
		//if the mark was set still need to have a mark end
		iOwnHeapCheckFlip=EFalse;
		iOwnHeapCheck=!iOwnHeapCheck;
		}
	if (iHeapCheckFlip)
		{
		iHeapCheckFlip=EFalse;
		iHeapCheck=!iHeapCheck;
		}
		
	if (resetOwnHeap)
		{
		//previously set failures, reset
		__UHEAP_RESET;
		}
	if (resetSharedHeap)
		{		
		__RHEAP_RESET (iHeap);
		}		
#endif		
	}


/** Handler for EFbsMessFontDuplicate message
 @param aMessage input parameters
 @param aPanicRequired flag that is set if a client panic is required
 @return KErrNone if successful, otherwise a system wide error code
 */
TInt CFbClient::HandleMesgFontDuplicate(const RMessage2& aMessage, TBool& aPanicRequired)
	{
#if _DEBUG
	if (iFontDuplicateToFail)
		return KErrNoMemory; //return with this error since this error is possible
#endif
	CFontObject* fontptr = (CFontObject*) aMessage.Int0();
	if(!TopLevelStore()->ValidFontHandle((TInt)fontptr))
		{
		return KErrUnknown;
		}

	TPckgBuf<TFontInfo> foninfo;
	TInt localhandle = 0;
	TInt ret = fontptr->Open();
	if (ret != KErrNone)
		{
		return ret;
		}
	TRAP(ret, localhandle=iIx->AddL(fontptr));
	if (ret != KErrNone)
		{
		fontptr->Close();
		return ret;
		}
	CopyFontInfo(fontptr,localhandle,foninfo());
	fontptr->iHeightInTwips = ((fontptr->iAddressPointer->HeightInPixels() * fontptr->iFontStore->iKPixelHeightInTwips) + 667) / 1000;
	ret = aMessage.Write(1, foninfo);
	if(ret != KErrNone)
		{
		iIx->Remove(localhandle);
		aPanicRequired = ETrue;
		return ret;
		}

	// success
	iResourceCount++;
	return KErrNone;
	}


/** Handler for EFbsMessGetNearestFontToDesignHeightInTwips, EFbsMessGetNearestFontToDesignHeightInPixels, 
 EFbsMessGetNearestFontToMaxHeightInTwips or EFbsMessGetNearestFontToMaxHeightInPixels messages
 @param aMessage input and output parameters
 @param aPanicRequired flag that is set if a client panic is required
 @return KErrNone if successful, otherwise a system wide error code
 */
TInt CFbClient::HandleMesgGetNearestFont(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	CFontObject* fontptr = NULL;
	TPckgBuf<TFontSpec> pckgFontSpec;
	TInt pckgMaxHeight;
	TPckgBuf<TSizeInfo> info;

	const TFbsMessage	fbsMessage = static_cast<TFbsMessage>(aMessage.Function());

	TInt ret = aMessage.Read(0, pckgFontSpec);
	TFontSpec& fontSpec = pckgFontSpec();
	if (ret == KErrNone )
		{
		TInt length = fontSpec.iTypeface.iName.Length();
		if(length < 0 || length > TOpenFontFaceAttribBase::ENameLength)
			{
			aPanicRequired = ETrue;
			return KErrArgument;
			}	
		
		ret = aMessage.Read(2, info);
		if (ret == KErrNone)
			{
			pckgMaxHeight = info().iMaxHeight;
			TopLevelStore()->FontStore()->iKPixelHeightInTwips=info().iDevSize.iHeight;
			TopLevelStore()->FontStore()->iKPixelWidthInTwips=info().iDevSize.iWidth;
			}
		}

	if (KErrNone != ret)
		{
		aPanicRequired = ETrue;
		return ret;
		}
	if ( (EFbsMessGetNearestFontToMaxHeightInTwips == fbsMessage) || (EFbsMessGetNearestFontToMaxHeightInPixels == fbsMessage) )
		{
		ret = TopLevelStore()->GetNearestFont(fontptr, fbsMessage, fontSpec, pckgMaxHeight);
		}
	else
		{
		ret = TopLevelStore()->GetNearestFont(fontptr, fbsMessage, fontSpec);
		}

	if (ret == KErrNone)
		{ // package up the result
		ret = CopyFontInfoIntoReturnMessage(aMessage, aPanicRequired, fontptr, 1);
		}

	return ret;
	}


 /** package up the font that was found - includes cleanup handling in case of error
 @param aMessage input and output parameters
 @param aPanicRequired flag that is set if a client panic is required
 @param aFontObj the object that is to be returned
 @param aWritePosition the slot position in the message pointing to the receiving object
 @return KErrNone if successful, otherwise a system wide error code
 */
TInt CFbClient::CopyFontInfoIntoReturnMessage(const RMessage2& aMessage, TBool& aPanicRequired, CFontObject* aFontObj, TInt aWritePosition)
	{
	TInt localhandle = 0;
	TRAPD(ret, localhandle = iIx->AddL(aFontObj));
	if (KErrNone != ret)
		{
		aFontObj->Close();
		return ret;
		}
	TPckgBuf<TFontInfo> pckgFontInfo;
	CopyFontInfo(aFontObj, localhandle, pckgFontInfo());
	ret = aMessage.Write(aWritePosition, pckgFontInfo);
	if (KErrNone != ret)
		{
		iIx->Remove(localhandle);
		aPanicRequired = ETrue;
		return ret;
		}
	// success
	iResourceCount++;
	return KErrNone;
	}


/** Handler for EFbsMessGetFontById message
 Works for Bitmap fonts only, see implementation in CFbTop::GetFontById() & CFontStore::GetFontById().
 @param aMessage input and output parameters
 @param aPanicRequired flag that is set if a client panic is required
 @return KErrNone if successful, otherwise a system wide error code
 */
TInt CFbClient::HandleMesgGetFontById(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	CFontObject* fontptr = NULL;
	TPckgBuf<TAlgStyle> algstyle;
	TPckgBuf<TSize> size;

	TInt ret = aMessage.Read(1, algstyle);
	if (ret == KErrNone)
		{ // get font size
		ret = aMessage.Read(3, size);
		if (ret == KErrNone)
			{
			TopLevelStore()->FontStore()->iKPixelHeightInTwips = size().iHeight;
			TopLevelStore()->FontStore()->iKPixelWidthInTwips = size().iWidth;
			}
		}
	if (ret != KErrNone)
		{
		aPanicRequired = ETrue;
		return ret;
		}
	TUid fileid;
	fileid.iUid = aMessage.Int2();
	ret = TopLevelStore()->GetFontById(fontptr,fileid,algstyle());

	if (ret == KErrNone)
		{ // package up the result
		ret = CopyFontInfoIntoReturnMessage(aMessage, aPanicRequired, fontptr, 0);
		}

	return ret;
	}


/** Handler for EFbsMessInstallFontStoreFile or EFbsMessAddFontStoreFile messages
 @param aMessage input and output parameters
 @param aPanicRequired flag that is set if a client panic is required
 @return KErrNone if successful, otherwise a system wide error code
 */
TInt CFbClient::HandleMesgAddOrInstallFontFile(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	TPckgBuf<TIntParcel> id;
	TInt length=aMessage.Int1();
	
	if(length < 0 || length * sizeof(TText) >= KMaxTInt/2)
		{
		aPanicRequired = ETrue;
		return KErrArgument;
		}
	
	TText* buffer=(TText*)User::Alloc(length*sizeof(TText));
	if(buffer==NULL)
		{
		return KErrNoMemory;
		}
	TPtr filename(buffer,length,length);
	TInt ret = aMessage.Read(0,filename);
	if(ret!=KErrNone)
		{
		User::Free(buffer);
		aPanicRequired = ETrue;
		return ret;
		}
	TRAP(ret, id().iInt = TopLevelStore()->FontStore()->AddFileL(filename).iUid);
	User::Free(buffer);
	if (ret != KErrNone)
		{
		return ret;
		}
	TUid uid;
	uid.iUid=id().iInt;
	if (aMessage.Function() == EFbsMessAddFontStoreFile)
		{
		TRAP(ret, AddFontFileIndexL(uid));
		if (ret!=KErrNone)
			{
			TopLevelStore()->FontStore()->RemoveFile(uid);
			return ret;
			}
		}
	ret = aMessage.Write(2,id);
	if (ret!=KErrNone)
		{
		RemoveFontFileIndex(uid);
		aPanicRequired = ETrue;
		}
	return ret;
	}


/** Handler for EFbsMessRemoveFontStoreFile message
 @param aMessage input parameters
 @return KErrNone if successful
 */
TInt CFbClient::HandleMesgRemoveFontFile(const RMessage2& aMessage)
	{
	TUid uid;
	uid.iUid=aMessage.Int0();
	RemoveFontFileIndex(uid);
	return KErrNone;
	}


/** Handler for EFbsMessRasterize message
 @param aMessage input parameters
 @param aPanicRequired flag that is set if a client panic is required
 @return ETrue if successful, EFalse or any system-wide error code if not successful.
 */
TInt CFbClient::HandleMesgRasterize(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	CFbTop* fbtop = TopLevelStore();
	CFontObject* fontptr = static_cast<CFontObject*>(iIx->At(aMessage.Int0(), fbtop->FontConUniqueID()));
	if(!fontptr)
		{
		aPanicRequired = ETrue;
		return KErrArgument;
		}
	CBitmapFont* bitmapFont = fontptr->iAddressPointer;
	const TUint charCode = aMessage.Int1();

	if ( (bitmapFont != NULL) && (bitmapFont->Rasterize(iSessionHandle, charCode, iOpenFontGlyphData)) )
		{
		// Convert all pointers to be passed back to the client to offsets from
		// the heap base so that they can be recreated client side relative to the
		// client's heap base
		TInt heapbase = fbtop->HeapBase();
		TPckgBuf<TRasterizeParams> params;
		params().iMetricsOffset = PointerToOffset(iOpenFontGlyphData->Metrics(), heapbase);
		params().iBitmapPointerOffset = PointerToOffset(iOpenFontGlyphData->BitmapPointer(), heapbase);;
		TInt err = aMessage.Write(2, params);
		if (KErrNone != err)
			{
			aPanicRequired = ETrue;
			return err;
			}
		return ETrue;
		}
	return EFalse;
	}


/** Handler for EFbsMessFaceAttrib message
 @param aMessage Input and output parameters
 @param aPanicRequired Flag that is set to ETrue if a client panic is required
 @return ETrue if successful, EFalse or any system-wide error code if not successful.
	 An error code is only returned if aPanicRequired is set to ETrue.
 */
TInt CFbClient::HandleMesgFaceAttrib(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	CFontObject* fontptr = static_cast<CFontObject*>(iIx->At(aMessage.Int0(), TopLevelStore()->FontConUniqueID()));
	if(!fontptr)
		{
		aPanicRequired = ETrue;
		return KErrArgument;
		}
	CBitmapFont* bitmapFont = fontptr->iAddressPointer;

	TPckgBuf<TOpenFontFaceAttrib> package;
	if ( (bitmapFont != NULL) && (bitmapFont->GetFaceAttrib(package())) )
		{
		TInt ret = aMessage.Write(1,package);
		if (ret == KErrNone)
			{
			return ETrue;
			}
		else
			{
			aPanicRequired = ETrue;
			return ret;
			}
		}
	return EFalse;
	}


/** Handler for EFbsMessHasCharacter message
 @param aMessage Input parameters
 @param aPanicRequired Flag that is set to ETrue if a client panic is required
 @return ETrue if the font has the character, EFalse if the font does not have
  the character, or any system-wide error code if not successful. An error code
  is only returned if aPanicRequired is set to ETrue.
 */
TInt CFbClient::HandleMesgHasCharacter(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	CFontObject* fontptr = static_cast<CFontObject*>(iIx->At(aMessage.Int0(), TopLevelStore()->FontConUniqueID()));
	if(!fontptr)
		{
		aPanicRequired = ETrue;
		return KErrArgument;
		}
	CBitmapFont* bitmapFont = fontptr->iAddressPointer;

	TInt ret = 0;
	TRAPD(error, ret = bitmapFont->HasCharacterL(aMessage.Int1()));
	if (error != KErrNone)
		{
		return EFalse;
		}
	return ret;
	}


/** Handler for EFbsMessShapeText message
 @param aMessage Input and output parameters
 @param aPanicRequired Flag that is set to ETrue if a client panic is required
 @return An offset from the heap base where the pointer to the shape is located
	if successful, otherwise 0 or any system-wide error code. An error code is
	only returned if aPanicRequired is set to ETrue.
 */
TInt CFbClient::HandleMesgShapeText(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	TInt error = KErrNone;
	TShapeHeader* shape = 0;
	TPckgBuf<TShapeMessageParameters> sp;
	if (aMessage.GetDesLength(2) != sizeof(TShapeMessageParameters))
		{
		aPanicRequired = ETrue;
		return KErrArgument;
		}

	CFbTop* fbtop = TopLevelStore();
	CFontObject* fontptr = static_cast<CFontObject*>(iIx->At(aMessage.Int0(), fbtop->FontConUniqueID()));
	if(!fontptr)
		{
		aPanicRequired = ETrue;
		return KErrArgument;
		}
	CBitmapFont* bitmapFont = fontptr->iAddressPointer;

	TInt inputTextLength = aMessage.GetDesLength(1);
	if (inputTextLength < 0)
		{
		error = inputTextLength;
		aPanicRequired = ETrue;
		return error;
		}
	else
		{
		iTextToShape.Zero();
		if (iTextToShape.MaxLength() < inputTextLength)
			error = iTextToShape.ReAlloc(inputTextLength);
		}
	if (error == KErrNone)
		{
		error = aMessage.Read(1, iTextToShape);
		if (error != KErrNone)
			{
			aPanicRequired = ETrue;
			return error;
			}
		error = aMessage.Read(2, sp);
		if (error != KErrNone)
			{
			aPanicRequired = ETrue;
			return error;
			}
		TRAP(error, shape = bitmapFont->ShapeTextL(iTextToShape, iSessionHandle, sp()) );
		if (error == KErrNone)
			{
			// Convert the pointer to be passed back to the client to an offset from
			// the heap base so that it can be recreated client side relative to the
			// client's heap base
			return PointerToOffset(shape, fbtop->HeapBase());
			}
		}
	return 0;
	}


/** Handler for EFbsMessShapeDelete message
 @param aMessage input and output parameters
 @param aPanicRequired flag that is set to ETrue if a client panic is required
 @return KErrNone if successful, otherwise a system wide error code
	 An error code is only returned if aPanicRequired is set to ETrue.
 */
TInt CFbClient::HandleMesgShapeDelete(const RMessage2& aMessage, TBool& aPanicRequired)
	{
	CFbTop* fbtop = TopLevelStore();
	CFontObject* fontptr = static_cast<CFontObject*>(iIx->At(aMessage.Int0(), fbtop->FontConUniqueID()));
	if(!fontptr)
		{
		aPanicRequired = ETrue;
		return KErrArgument;
		}
	CBitmapFont* bitmapFont = fontptr->iAddressPointer;

	// Combine the passed shape offset with the current heap base to get
	// a valid pointer to a shape header for use in this process			
	TShapeHeader* shapeheader = reinterpret_cast<TShapeHeader*>(OffsetToPointer(aMessage.Int1(), fbtop->HeapBase()));

	bitmapFont->DeleteShape(iSessionHandle,shapeheader);
	return KErrNone;
	}


void CFbClient::ProcFontMessage(const RMessage2& aMessage)
	{
	TInt ret = KErrUnknown;
	TBool panicRequired = EFalse;

	switch(aMessage.Function())
		{
		case EFbsMessFontDuplicate:
			ret = HandleMesgFontDuplicate(aMessage, panicRequired);
			break;

		case EFbsMessGetNearestFontToDesignHeightInTwips:
		case EFbsMessGetNearestFontToDesignHeightInPixels:
		case EFbsMessGetNearestFontToMaxHeightInTwips:
		case EFbsMessGetNearestFontToMaxHeightInPixels:
			ret = HandleMesgGetNearestFont(aMessage, panicRequired);
			break;

		case EFbsMessGetFontById:
			ret = HandleMesgGetFontById(aMessage, panicRequired);
			break;

		case EFbsMessInstallFontStoreFile:
		case EFbsMessAddFontStoreFile:
			ret = HandleMesgAddOrInstallFontFile(aMessage, panicRequired);
			break;

		case EFbsMessRemoveFontStoreFile:
			ret = HandleMesgRemoveFontFile(aMessage);
			break;

		case EFbsMessRasterize:
			ret = HandleMesgRasterize(aMessage, panicRequired);
			break;

		case EFbsMessFaceAttrib:
			ret = HandleMesgFaceAttrib(aMessage, panicRequired);
			break;

		case EFbsMessHasCharacter:
			ret = HandleMesgHasCharacter(aMessage, panicRequired);
			break;

		case EFbsMessShapeText:
			ret = HandleMesgShapeText(aMessage, panicRequired);
			break;

		case EFbsMessShapeDelete:
			ret = HandleMesgShapeDelete(aMessage, panicRequired);
			break;

		case EFbsMessSetTwipsHeight:
			{
			TInt localhandle=aMessage.Int0();
			CFontObject* fontptr = static_cast<CFontObject*>(iIx->At(localhandle, TopLevelStore()->FontConUniqueID()));
			if(!fontptr)
				{
				panicRequired = ETrue;
				ret = KErrArgument;
				break;
				}
			fontptr->iHeightInTwips = aMessage.Int1();
			ret = KErrNone;
			break;	
			}
		case EFbsMessGetTwipsHeight:
			{
			TInt localhandle=aMessage.Int0();
			CFontObject* fontptr = static_cast<CFontObject*>(iIx->At(localhandle, TopLevelStore()->FontConUniqueID()));
			if(!fontptr)
				{
				panicRequired = ETrue;
				ret = KErrArgument;
				break;
				}
			TPckgBuf<TInt> height;
			height() = fontptr->iHeightInTwips;
			ret = aMessage.Write(1,height);
			if (KErrNone != ret)
				{
				panicRequired = ETrue;
				}
			break;
			}
		case EFbsSetSystemDefaultTypefaceName:
			{
			TBuf<KMaxTypefaceNameLength> fontTypefaceName;
			ret = aMessage.GetDesLength(0);
			if (ret < 0)
				{
				panicRequired = ETrue;
				break;
				}
			if (ret <= KMaxTypefaceNameLength) // Size in characters i.e. 2 bytes for unicode
				{
				ret = aMessage.Read(0, fontTypefaceName);
				if (ret == KErrNone)
					{
					TopLevelStore()->SetSystemDefaultTypefaceName(fontTypefaceName);
					}
				}
			else
				{
				panicRequired = ETrue;
				ret = KErrTooBig;
				}

			break;
			}

#ifdef _DEBUG
		case EFbsMessSetDuplicateFail:
			TInt argument =aMessage.Int0();
			if (argument)
				{
				iFontDuplicateToFail = ETrue;
				}
			else
				{
				iFontDuplicateToFail = EFalse;
				}
			ret=KErrNone;
			break;
#endif
		}

	// either have a result or an error code to panic the client with
	if (panicRequired)
		{
		aMessage.Panic(KFBSERVPanicCategory, ret);
		}
	else
		{
		if(!aMessage.IsNull())
			{
			aMessage.Complete(ret);
			}		
		}
#ifdef _DEBUG
	iRet=ret;
#endif	
	}

void CFbClient::ProcBitmapMessage(const RMessage2 &aMessage)
	{
	CBitmapObject* bmpptr=NULL;
	TInt localhandle=0;
	TInt ret=KErrUnknown;
	switch(aMessage.Function())
		{
	case EFbsMessBitmapCreate:
		{
		TPckgBuf<TBmpSpec> bs;
		ret = aMessage.Read(0,bs);
		if(ret!=KErrNone)
			{
			aMessage.Panic(KFBSERVPanicCategory,ret);
			return;
			}
			
		TBmpSpec& bmpSpec = bs();	
		
		if(!TDisplayModeUtils::IsDisplayModeValid(bmpSpec.iDispMode))
			{
			aMessage.Panic(KFBSERVPanicCategory,KErrArgument);
			return;
			}

		// client uses iHandle to pass UID and iServerHandle to pass data size
		TRAP(ret, bmpptr = TopLevelStore()->CreateBitmapL(bmpSpec.iSizeInPixels, bmpSpec.iDispMode, TUid::Uid(bmpSpec.iHandle), EFalse, bmpSpec.iServerHandle));
		if(ret!=KErrNone)
			break;
		TRAP(ret,localhandle=iIx->AddL(bmpptr));
		if(ret!=KErrNone)
			{
			bmpptr->Close();
			break;
			}
		bmpSpec.iHandle=localhandle;
		bmpSpec.iServerHandle = bmpptr->Handle();
		bmpSpec.iAddressOffset=TInt(bmpptr->Address())-TopLevelStore()->HeapBase();
		ret = aMessage.Write(0,bs);
		if(ret!=KErrNone)
			{
			iIx->Remove(localhandle);
			aMessage.Panic(KFBSERVPanicCategory,ret);
			return;
			}
		iResourceCount++;
		break;
		}

	case EFbsMessBitmapLoad:
	case EFbsMessBitmapLoadFast:
		{
		TPckgBuf<TLoadBitmapArg> loadBitmapArg;
		ret = aMessage.Read(1,loadBitmapArg);
		if(ret!=KErrNone)
			{
			aMessage.Panic(KFBSERVPanicCategory,ret);
			return;
			}
		const TInt32 id=loadBitmapArg().iBitmapId;
		const TBool shareifloaded=loadBitmapArg().iShareIfLoaded;
		const TUint fileOffset = loadBitmapArg().iFileOffset;
		if(aMessage.Function() == EFbsMessBitmapLoad)
			{
			RFile file;
			ret=file.AdoptFromClient(aMessage,2,3);
			if (ret!=KErrNone)
				{
				break;
				}
			TFileName filename;
			ret=file.FullName(filename);
			if (ret!=KErrNone)
				{
				break;
				}

			if(shareifloaded)
				{
				TRAP(ret, bmpptr=TopLevelStore()->ShareBitmapL(filename, id, fileOffset,&file, iSessionHandle));
				}
			else
				{
				TRAP(ret, bmpptr=TopLevelStore()->LoadBitmapL(filename, id, fileOffset, &file, iSessionHandle));
				}
			file.Close();
			}
		else
			{
			TFileName filename;
			ret = aMessage.Read(2,filename);
			if (ret!=KErrNone)
				{
				aMessage.Panic(KFBSERVPanicCategory,ret);
				return;
				}
			_LIT(KZDrive, "z:");
			if (filename.Left(2).CompareF(KZDrive))
				{
				// File is not in the Z: drive.
				// So open the file and pass the file handle to LoadBitmapL() or ShareBitmapL() and close it afterwards.
				// The reason is that the cache cannot be used for files that are writable,
				// since they can't be kept open in the cache.
				RFile file;
				ret = file.Open(TopLevelStore()->FileSession(),filename,EFileShareReadersOnly);
				if (ret!=KErrNone)
					{
					break;
					}
				if(shareifloaded)
					{
					TRAP(ret, bmpptr=TopLevelStore()->ShareBitmapL(filename, id, fileOffset,&file, iSessionHandle));
					}
				else
					{
					TRAP(ret, bmpptr=TopLevelStore()->LoadBitmapL(filename, id, fileOffset, &file, iSessionHandle));
					}
				file.Close();
				}
			else
				{
				if(shareifloaded)
					{
					TRAP(ret, bmpptr=TopLevelStore()->ShareBitmapL(filename, id, fileOffset, NULL, iSessionHandle));
					}
				else
					{
					TRAP(ret, bmpptr=TopLevelStore()->LoadBitmapL(filename, id, fileOffset, NULL, iSessionHandle));
					}
				}
			}
		if(ret!=KErrNone)
			{
			break;
			}
		TRAP(ret,localhandle=iIx->AddL(bmpptr));
		if(ret!=KErrNone)
			{
			bmpptr->Close();
			break;
			}
		TPckgBuf<TBmpHandles> handlebuffer;
		handlebuffer().iHandle=localhandle;
		handlebuffer().iServerHandle = bmpptr->Handle();
		handlebuffer().iAddressOffset=TInt(bmpptr->Address())-TopLevelStore()->HeapBase();
		ret = aMessage.Write(0,handlebuffer);
		if(ret!=KErrNone)
			{
			iIx->Remove(localhandle);
			aMessage.Panic(KFBSERVPanicCategory,ret);
			return;
			}
		iResourceCount++;
		break;
		}
	case EFbsMessBitmapResize:
		{
		localhandle=aMessage.Int0();
		CFbTop* fbtop = TopLevelStore();
		bmpptr = static_cast<CBitmapObject*>(iIx->At(localhandle, fbtop->BitmapConUniqueID()));
		if(!bmpptr)
			{
			ret=KErrUnknown;
			break;
			}
		ret = fbtop->GetCleanBitmap(bmpptr);
		if (ret != KErrNone)
			break;
		TSize newsize(aMessage.Int1(),aMessage.Int2());
 		const TBool compressedInRam = bmpptr->Address()->IsCompressedInRAM();  //It must be set before the resizing is done.
		const TDisplayMode dispMode = bmpptr->Address()->DisplayMode();
		CBitmapObject* newbmpptr = NULL;
		TRAP(ret, newbmpptr = fbtop->CreateBitmapL(newsize, dispMode, KUidCFbsBitmapCreation, ETrue));
		if (ret != KErrNone)
			break;
		ret = newbmpptr->Address()->CopyData(*bmpptr->Address());
		if (ret != KErrNone)
			{
			newbmpptr->Close();
			break;
			}
 		if (compressedInRam)
			{
			// if the header says PaletteCompression specify.
			TBitmapfileCompressionScheme scheme = ERLECompression;
			if (bmpptr->Address()->iHeader.iCompression == EGenericPaletteCompression)
				scheme = EPaletteCompression;
			ret = newbmpptr->Address()->CompressData(scheme); // re-compress
			if (ret != KErrNone)
				{
				newbmpptr->Close();
				break;
				}
			}
		TInt newlocalhandle = 0;
		TRAP(ret, newlocalhandle = iIx->AddL(newbmpptr));
		if (ret != KErrNone)
			{
			newbmpptr->Close();
			break;
			}
		ret = newbmpptr->Open();
		if (ret != KErrNone)
			{
			iIx->Remove(newlocalhandle);
			break;
			}
		bmpptr->SetCleanBitmap(newbmpptr);
		if (bmpptr->AccessCount() >= 2)
			fbtop->NotifyDirtyBitmap(*bmpptr, this);
		iIx->Remove(localhandle);
		TPckgBuf<TBmpHandles> handlebuffer;
		handlebuffer().iHandle = newlocalhandle;
		handlebuffer().iServerHandle = newbmpptr->Handle();
		handlebuffer().iAddressOffset = TInt(newbmpptr->Address()) - fbtop->HeapBase();
		ret = aMessage.Write(3, handlebuffer);
		if (ret != KErrNone)
			{
			iIx->Remove(newlocalhandle);
			aMessage.Panic(KFBSERVPanicCategory, ret);
			return;
			}
		break;
		}
	case EFbsMessBitmapDuplicate:
		{
		bmpptr = TopLevelStore()->FindBitmap(aMessage.Int0());
		if (bmpptr == NULL)
			{
			ret=KErrUnknown;
			break;
			}
		//coverity [check_return]
		//coverity [unchecked_value]
		TopLevelStore()->GetCleanBitmap(bmpptr);
		ret = bmpptr->Open();
		if (ret != KErrNone)
			break;
		TPckgBuf<TBmpHandles> handlebuffer;
		TRAP(ret,localhandle=iIx->AddL(bmpptr));
		if(ret!=KErrNone)
			{
			bmpptr->Close();
			break;
			}
		handlebuffer().iHandle = localhandle;
		handlebuffer().iServerHandle = bmpptr->Handle();
		handlebuffer().iAddressOffset = TInt(bmpptr->Address()) - TopLevelStore()->HeapBase();
		ret = aMessage.Write(1, handlebuffer);
		if(ret!=KErrNone)
			{
			iIx->Remove(localhandle);
			aMessage.Panic(KFBSERVPanicCategory,ret);
			return;
			}
		iResourceCount++;
		break;
		}
	case EFbsMessBitmapCompress:
		{
		localhandle = aMessage.Int0();
		CFbTop* fbtop = TopLevelStore();
		bmpptr = static_cast<CBitmapObject*>(iIx->At(localhandle, fbtop->BitmapConUniqueID()));
		if(!bmpptr)
			{
			ret = KErrUnknown;
			break;
			}
		ret = fbtop->GetCleanBitmap(bmpptr);
		if (ret != KErrNone)
			break;
		const TSize size = bmpptr->Address()->SizeInPixels();
		const TDisplayMode dispMode = bmpptr->Address()->DisplayMode();
		CBitmapObject* newbmpptr = NULL;
		TRAP(ret, newbmpptr = fbtop->CreateBitmapL(size, dispMode, KUidCFbsBitmapCreation, ETrue));
		if (ret != KErrNone)
			break;
		ret = newbmpptr->Address()->CopyData(*bmpptr->Address());
		if (ret != KErrNone)
			{
			newbmpptr->Close();
			break;
			}
		ret = newbmpptr->Address()->CompressData((TBitmapfileCompressionScheme)aMessage.Int1());
		if (ret != KErrNone)
			{
			newbmpptr->Close();
			break;
			}
		TInt newlocalhandle = 0;
		TRAP(ret, newlocalhandle = iIx->AddL(newbmpptr));
		if (ret != KErrNone)
			{
			newbmpptr->Close();
			break;
			}
		ret = newbmpptr->Open();
		if (ret != KErrNone)
			{
			iIx->Remove(newlocalhandle);
			break;
			}
		bmpptr->SetCleanBitmap(newbmpptr);
		if (bmpptr->AccessCount() >= 2)
			fbtop->NotifyDirtyBitmap(*bmpptr, this);
		iIx->Remove(localhandle);
		TPckgBuf<TBmpHandles> handlebuffer;
		handlebuffer().iHandle = newlocalhandle;
		handlebuffer().iServerHandle = newbmpptr->Handle();
		handlebuffer().iAddressOffset = TInt(newbmpptr->Address()) - fbtop->HeapBase();
		ret = aMessage.Write(2, handlebuffer);
		if (ret != KErrNone)
			{
			iIx->Remove(newlocalhandle);
			aMessage.Panic(KFBSERVPanicCategory, ret);
			return;
			}
		break;
		}
	case EFbsMessBitmapBgCompress:
		{
		localhandle = aMessage.Int0();
		TBitmapfileCompressionScheme scheme = (TBitmapfileCompressionScheme)aMessage.Int1();
		TBool async = aMessage.Int2();
		CFbTop* fbtop = TopLevelStore();
		bmpptr = static_cast<CBitmapObject*>(iIx->At(localhandle, fbtop->BitmapConUniqueID()));
		if(!bmpptr)
			{
			ret = KErrUnknown;
			break;
			}
		ret = fbtop->GetCleanBitmap(bmpptr);
		if (ret != KErrNone)
			{
			if (!async)
				ret = KErrNone;
			break;
			}
		ret = bmpptr->Address()->CheckBackgroundCompressData();
		if (KErrNone == ret)
			{
			ret = fbtop->BackgroundCompression()->AddToCompressionQueue(bmpptr, scheme, async ? &aMessage : NULL);
			if (ret == KErrNone && async)
				return; // do not complete the client's request - that will be done by the background compression thread
			}
		if (KErrAlreadyExists == ret)
			ret = KErrNone;
		break;
		}
	case EFbsMessBitmapClean:
		{
		TInt localhandle = aMessage.Int0();
		CFbTop* fbtop = TopLevelStore();
		bmpptr = static_cast<CBitmapObject*>(iIx->At(localhandle, fbtop->BitmapConUniqueID()));
		if(!bmpptr)
			{
			ret = KErrUnknown;
			break;
			}
		ret = fbtop->GetCleanBitmap(bmpptr);
		if (ret != KErrNone)
			break;
		ret = bmpptr->Open();
		if (ret != KErrNone)
			break;
		TInt cleanlocalhandle = 0;
		TRAP(ret, cleanlocalhandle = iIx->AddL(bmpptr));
		if (ret != KErrNone)
			{
			bmpptr->Close();
			break;
			}
		iIx->Remove(localhandle);
		TPckgBuf<TBmpHandles> handlebuffer;
		handlebuffer().iHandle = cleanlocalhandle;
		handlebuffer().iServerHandle = bmpptr->Handle();
		handlebuffer().iAddressOffset = TInt(bmpptr->Address()) - fbtop->HeapBase();
		ret = aMessage.Write(1, handlebuffer);
		if (ret != KErrNone)
			{
			iIx->Remove(cleanlocalhandle);
			aMessage.Panic(KFBSERVPanicCategory, ret);
			return;
			}
		break;
		}
	case EFbsGetAllBitmapHandles:
		{
		ret = TopLevelStore()->GetAllBitmapHandles(aMessage);		
		break;	
		}	

	case EFbsMessBitmapNotifyDirty:
		{
		if (iHelper == NULL)
			{
			iHelper = new TFbClientHelper(*this);
			if (iHelper == NULL)
				{
				ret = KErrNoMemory;
				break;
				}
			TopLevelStore()->AddClientHelper(*iHelper);
			}
		if (!iHelper->iMessage.IsNull())
			{
			aMessage.Panic(KFBSERVPanicCategory, KErrAlreadyExists);
			return;
			}
		if (!iHelper->iDirty)
			{
			iHelper->iMessage = aMessage;
			return; // do not complete the client's request yet - that will be done when a bitmap becomes dirty
			}
		ret = KErrNone;
		iHelper->iDirty = EFalse;
		}
		break;
	case EFbsMessBitmapCancelNotifyDirty:
		{
		if (iHelper != NULL && !iHelper->iMessage.IsNull())
			iHelper->iMessage.Complete(KErrCancel);
		ret = KErrNone;
		}
		break;
		}
		
	if(!aMessage.IsNull())
		{
		aMessage.Complete(ret);
		}
	
#ifdef _DEBUG
	iRet=ret;
#endif		
	}

void CFbClient::NotifyDirtyBitmap(CBitmapObject& aBmpObj)
	{
	if (iHelper != NULL && iIx->At(&aBmpObj) != KErrNotFound)
		{
		iHelper->iDirty = ETrue;
		if (!iHelper->iMessage.IsNull())
			{
			iHelper->iMessage.Complete(KErrNone);
			iHelper->iDirty = EFalse;
			}
		}
	}

void CFbClient::AddFontFileIndexL(TUid aId)
	{
	TInt count=iFontFileIndex->Count();
	for (TInt index=0;index<count;index++)
		{
		if (iFontFileIndex->At(index).iUid==aId)
			{
			iFontFileIndex->At(index).iAccessCount++;
			TopLevelStore()->FontStore()->RemoveFile(aId);
			return;
			}
		}

	TFontFileIndex fontFileIndex;
	fontFileIndex.iUid=aId;
	fontFileIndex.iAccessCount=1;
	iFontFileIndex->AppendL(fontFileIndex);
	}

void CFbClient::RemoveFontFileIndex(TUid aId)
	{
	TInt count=iFontFileIndex->Count();
	for (TInt index=0;index<count;index++)
		{
		TFontFileIndex* fontFileIndex=&iFontFileIndex->At(index);
		if (fontFileIndex->iUid==aId)
			{
			fontFileIndex->iAccessCount--;
			if (fontFileIndex->iAccessCount<1)
				{
				TopLevelStore()->FontStore()->RemoveFile(fontFileIndex->iUid);
				iFontFileIndex->Delete(index);
				}
			return;
			}
		}
	// not found - must be an installed file or rubbish, so try anyway
	TopLevelStore()->FontStore()->RemoveFile(aId);
	}

void CFbClient::Disconnect(const RMessage2 &aMessage)
	{
	// if any bitmaps are in the background compression queue with a to-be-completed RMessage2 from this session,
	// the RMessage2 must be completed now as it is only possible to complete messages on existing sessions
	TopLevelStore()->BackgroundCompression()->CompleteOutstandingRequests(this);

	// if there is a to-be-completed request for dirty bitmap notification complete it now
	if (iHelper)
		{
		if (!iHelper->iMessage.IsNull())
			iHelper->iMessage.Complete(KErrDisconnected);
		iHelper->Deque();
		delete iHelper;
		iHelper = NULL;
		}

	// Clear the mbm file store cache resources that corresponds to this session
	TopLevelStore()->CloseFileStores(iSessionHandle);

	CSession2::Disconnect(aMessage);
	}

#ifdef _DEBUG
void CFbClient::ProcMemMessage(const RMessage2 &aMessage)
	{
	TInt ret=KErrNone;
	TInt parameterForFunctionCall;
	TInt cells = User::Heap().Available(parameterForFunctionCall);
	switch(aMessage.Function())
		{
		case EFbsMessSetHeapFail:
			if (aMessage.Int0()==RFbsSession::EHeapFailTypeServerMemory)
				{
				iOwnHeapFailNumber=aMessage.Int1();
				}
			else
				{
				iSharedHeapFailNumber=aMessage.Int1();
				}
			break;
		case EFbsMessHeapCount:
			if (aMessage.Int0()==RFbsSession::EHeapFailTypeServerMemory)
				{
				ret=User::CountAllocCells();
				}
			else
				{
				ret=iHeap->Count();
				}	
			break;
		case EFbsMessSetHeapReset:
			if (aMessage.Int0()==RFbsSession::EHeapFailTypeServerMemory)
				{
				iOwnHeapFailNumber=-1;
				}
			else
				{
				iSharedHeapFailNumber=-1;
				}				
			break;
		case EFbsMessSetHeapCheck:
			if (aMessage.Int0()==RFbsSession::EHeapFailTypeServerMemory)
				{
				iOwnHeapCheckFlip=ETrue;
				}
			else
				{
				iHeapCheckFlip=ETrue;
				}
			break;
		case EFbsMessHeap:
			ret=(TInt)iHeap;
			break;			
		}
	aMessage.Complete(ret);
	iRet=ret;
	}
#endif
