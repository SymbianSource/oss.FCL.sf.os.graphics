// Copyright (c) 1995-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <fbs.h>
#include "fbsdefs.h"
#include "UTILS.H"
#include "FBSVER.H"
#include "FbsRalc.h"
#include "fbshelper.h"
#include "FbsMessage.H"

GLDEF_C void Panic(TFbsPanic aPanic)
	{
	_LIT(KFBSERVClientPanicCategory,"FBSCLI");
	User::Panic(KFBSERVClientPanicCategory,aPanic);
	}

EXPORT_C TInt FbsStartup()
	{
	RChunk sharedchunk;
	TInt ret = sharedchunk.OpenGlobal(KFBSERVSharedChunkName,EFalse);
	if (ret == KErrNone)
		{
		sharedchunk.Close();
		return KErrNone;
		}

	RProcess fbs;
	_LIT(KFBSERVServerExe,"z:\\sys\\bin\\fbserv.exe");
	ret = fbs.Create(KFBSERVServerExe,KNullDesC);

	if (ret!=KErrNone)
		return ret;
	
  	
	TRequestStatus stat;
	fbs.Rendezvous(stat);
	if (stat!=KRequestPending)
		fbs.Kill(0);		// abort startup
	else
		fbs.Resume();	// logon OK - start the server
	User::WaitForRequest(stat);		// wait for start or death
	// we can't use the 'exit reason' if the server panicked as this
	// is the panic 'reason' and may be '0' which cannot be distinguished
	// from KErrNone
	ret=(fbs.ExitType()==EExitPanic) ? KErrGeneral : stat.Int();
	fbs.Close();
	return ret;
	}


//
// Fontbitmap server client side session
//

/** 
@publishedAll 
@released
*/
EXPORT_C RFbsSession::RFbsSession():
	RSessionBase(),
	iConnections(0),
	iCallBack(),
	iSharedChunk(),
	iHelper(NULL),
	iRomFileAddrCache(NULL),
	iDecompressionBuffer(NULL),
	iScanLineBuffer(NULL),
	iSpare(NULL)
	{}

/** Creates a session with the Font and Bitmap server.
@param aFileServer A fuly constructed file server session
@return KErrNone, if successful; KErrNoMemory if there is not enough memory 
to create the session; otherwise another of the system-wide error codes. 
@publishedAll 
@released
*/
EXPORT_C TInt RFbsSession::Connect(RFs& aFileServer)
	{
	RFbsSession* thisptr = (RFbsSession*)Dll::Tls();
	if(thisptr)
		{
		thisptr->iConnections++;
		return KErrNone;
		}
	TInt ret = RFbsSession::DoAlloc(thisptr);
	if(ret!=KErrNone)
		return ret;
	return thisptr->DoConnect(aFileServer);
	}

/** Creates a session with the Font and Bitmap server.
@return KErrNone, if successful; KErrNoMemory if there is not enough memory 
to create the session; otherwise another of the system-wide error codes. 
@publishedAll 
@released
*/
EXPORT_C TInt RFbsSession::Connect()
	{
	RFbsSession* thisptr = (RFbsSession*)Dll::Tls();
	if(thisptr)
		{
		thisptr->iConnections++;
		return KErrNone;
		}
	TInt ret = RFbsSession::DoAlloc(thisptr);
	if (ret!=KErrNone)
		return ret;
	ret = thisptr->iFileServer.Connect();
	if(ret!=KErrNone)
		{
		thisptr->Disconnect();
		return ret;
		}
	return thisptr->DoConnect(thisptr->iFileServer);
	}

/** Closes the session with the Font and Bitmap server. 
@publishedAll 
@released
*/
EXPORT_C void RFbsSession::Disconnect()
	{
	RFbsSession* thisptr=(RFbsSession*)Dll::Tls();
	if(thisptr==NULL) return;
	if(thisptr->iConnections>0)
		{
		thisptr->iCallBack.iPtr=NULL;
		thisptr->iCallBack.CallBack();
		// Destructor of CFbsSessionHelper may call SendCommand to cancel an
		// outstanding request, therefore destruction must be done before
		// iConnections is 0 to avoid an assertion going off.
		if(thisptr->iConnections==1)
			{
			delete thisptr->iHelper;
			}
		thisptr->iConnections--;
		}
	if(thisptr->iConnections==0)
		{
		thisptr->iSharedChunk.Close();
		thisptr->iLargeBitmapChunk.Close();
		// Call close on the iFileServer regardless of whether this session owns it: 
		// if we don't own it, close will do nothing if there are still open files, 
		// so always calling close introduces extra safety
		thisptr->iFileServer.Close();
		delete thisptr->iRomFileAddrCache; 
		delete thisptr->iScanLineBuffer;
		delete thisptr->iDecompressionBuffer;
		thisptr->Close();
		delete thisptr;
		Dll::FreeTls();
		}
	}

/**  Gets the current Font and Bitmap server session.
@return  A pointer to the current session or NULL if Connect() has not been 
called yet. 
@publishedAll 
@released
*/
EXPORT_C RFbsSession* RFbsSession::GetSession()
    {
	return((RFbsSession*)Dll::Tls());
	}

/** Triggers the most recently registered callback. This is mainly called by 
bitmaps when their twips size changes and when any FBSERV objects are closed 
to notify clients of a change that may affect their operation. 
@publishedAll 
@released
*/
EXPORT_C void RFbsSession::CallBack()
    {
	iCallBack.CallBack();
	iCallBack.iPtr=NULL;
	}

void RFbsSession::SetCallBackPtr(TInt* aBitmapHandle)const
	{
	iCallBack.iPtr=aBitmapHandle;
	}
	
/** Sets the callback.
@param aCallBack callback object to be called by CallBack(). Only one may be 
in use at a time and subsequent calls will displace previous calls. 
@publishedAll 
@released
*/	
EXPORT_C void RFbsSession::SetCallBack(TCallBack aCallBack)
    {
	iCallBack=aCallBack;
	}

/** Resets the callback. 
@publishedAll 
@released
*/	
EXPORT_C void RFbsSession::ResetCallBack()
	{
	TCallBack cb(NULL);
	iCallBack=cb;
	}

/**  Returns the number of Font and Bitmap Server objects currently in 
use by this session.
@return The number of resources in use: bitmaps, fonts, typeface stores. 
@publishedAll 
@released
*/	
EXPORT_C TInt RFbsSession::ResourceCount()
    {
	return(SendCommand(EFbsMessResourceCount));
	}

/** Utility function for passing commands to the server.
@param aMessage Integer code for the message to pass - see TFbsMessage.
@param aInt0 Parameter 0 for the message.
@param aInt1 Parameter 1 for the message.
@param aInt2 Parameter 2 for the message.
@param aInt3 Parameter 3 for the message.
@return Return code from RSessionBase::SendReceive(). 
@internalComponent
*/
EXPORT_C TInt RFbsSession::SendCommand(TInt aMessage,TInt aInt0,TInt aInt1,TInt aInt2,TInt aInt3) const
   {
	__ASSERT_ALWAYS(iConnections>0,Panic(EFbsPanicBadConnection));
	switch(aMessage)
		{
	case EFbsMessShutdown:
	case EFbsMessClose:
		SetCallBackPtr(aMessage==EFbsMessClose ? &aInt1 : 0);
		iCallBack.CallBack();
	default:
		break;
		}
	TInt ret = SendReceive(aMessage, TIpcArgs(aInt0,aInt1,aInt2,aInt3));
	return(ret);
	}

TInt RFbsSession::SendCommand(TInt aMessage, const TIpcArgs& aArgs) const
	{
	__ASSERT_ALWAYS(iConnections>0,Panic(EFbsPanicBadConnection));
	return SendReceive(aMessage,aArgs);
	}

void RFbsSession::SendCommand(TInt aMessage, const TIpcArgs& aArgs, TRequestStatus &aStatus) const
	{
	__ASSERT_ALWAYS(iConnections>0,Panic(EFbsPanicBadConnection));
	SendReceive(aMessage,aArgs,aStatus);
	}
	
/** Gets the current Font and Bitmap server version.
@return The current version of the server. 
@publishedAll 
@released
*/
EXPORT_C TVersion RFbsSession::Version()
	{
	TVersion v(KFbsMajorVersionNumber,KFbsMinorVersionNumber,KFbsBuildVersionNumber);
	return(v);
	}

/** Gets the address of first location in the global shared heap containing 
fonts and bitmaps.
@return Pointer to the base of the shared heap. 
@publishedAll 
@released
*/	
EXPORT_C TUint8* RFbsSession::HeapBase() const
	{
	return(iSharedChunk.Base());
	}

TBool RFbsSession::LookupBitmapInROM(const TDesC& aFilename, TAny*& aAddr)
	{
	aAddr = iRomFileAddrCache->Lookup(aFilename);
	if (aAddr)
		return ETrue;
	return EFalse;
   	}

TInt RFbsSession::DoAlloc(RFbsSession*& aNewSession)
	{
	aNewSession = (RFbsSession*)User::Alloc(sizeof(RFbsSession));
	if(!aNewSession) 
		return KErrNoMemory;
	new(aNewSession) RFbsSession;
	aNewSession->iConnections = 1;
	TInt ret = Dll::SetTls(aNewSession);
	if(ret!=KErrNone)
		{
		delete aNewSession;
		aNewSession = NULL;
		}
	return ret;
	}

/**  
Do actual connect as common to both Connect() and Connect(RFs& aFileServer). Store fully constructed
file server session to iSpare member variable
@see Connect()
@return KErrNone, if successful; KErrNoMemory if there is not enough memory 
to create the session; otherwise another of the system-wide error codes. 
@internalComponent
*/
TInt RFbsSession::DoConnect(RFs& aFileServer)
	{
	//Allow this session to be accessed by other process to lead bitmap in server
	TInt ret = aFileServer.ShareProtected();
	if (ret!=KErrNone)
		{
		Disconnect();
		return ret;
		}
	iRomFileAddrCache = CFbsRalCache::New(4, aFileServer);
	if (!iRomFileAddrCache)
		{
		Disconnect();
		return KErrNoMemory;
		}

	ret = CreateSession(KFBSERVGlobalThreadName,Version(),-1);
	if(ret!=KErrNone)
		{
		Disconnect();
		return ret;
		}
	const TInt serverAssignedHandle = SendReceive(EFbsMessInit);
	if(serverAssignedHandle <= 0)
		{
		Disconnect();
		return serverAssignedHandle;
		}
	ret = iSharedChunk.OpenGlobal(KFBSERVSharedChunkName,EFalse);
	if(ret!=KErrNone) 
		Panic(EFbsPanicChunkError);
	iHelper = new CFbsSessionHelper(*this);
	if (!iHelper)
		{
		Disconnect();
		return KErrNoMemory;
		}
	iHelper->iServerSessionHandle = serverAssignedHandle;
	ret = iLargeBitmapChunk.OpenGlobal(KFBSERVLargeChunkName,EFalse);
	if(ret!=KErrNone) 
		Panic(EFbsPanicChunkError);
	
	iSpare = (TUint32*)&aFileServer;
	return KErrNone;
	}
	
/**  
Allocates the buffer for decoding compressed rom bitmaps.
 
Internal use only.

@param 	aSize Minimum size of the buffer required.  
		If the buffer is too small an attempt to resize it will be made.
@return KErrNone if successful, 
		KErrNoMemory if the buffer was too small and could 
		not be expanded.
@publishedAll 
@released
*/
TInt RFbsSession::AllocScanLineBuffer(TInt aSize)
    {
	if (iScanLineBuffer && iScanLineBuffer->Des().MaxSize() >= aSize)
		{
		return KErrNone;
		}

	HBufC8* newBuffer = HBufC8::New(aSize);

	if (newBuffer)
		{
		delete iScanLineBuffer;
		iScanLineBuffer = newBuffer;
		return KErrNone;
		}

	return KErrNoMemory;
	}

/**  
Gets a reference to the buffer currently in use for decoding
compressed rom bitmaps. 

Internal use only.

@return  The buffer area currently in use,
@publishedAll 
@released
*/
HBufC8* RFbsSession::GetScanLineBuffer()
    {
	return iScanLineBuffer;
	}

/**  
Gets a pointer to the buffer currently in use for decoding
compressed mask bitmaps. 

Internal use only.
@param 	aSize Minimum size of the buffer required.  
		If the buffer is too small an attempt to resize it will be made.
@return  The buffer area currently in use or NULL if there is no memory.
@internalComponent 
*/
HBufC8* RFbsSession::GetDecompressionBuffer(TInt aSize)
	{
	if (iDecompressionBuffer)
		{
		if (iDecompressionBuffer->Des().MaxSize() < aSize)
			{
			HBufC8* tempBuffer = iDecompressionBuffer->ReAlloc(aSize);
			if (!tempBuffer)
				{
				return NULL;
				}
			iDecompressionBuffer = tempBuffer;
			}
		}
	else
		{
		iDecompressionBuffer = HBufC8::New(aSize);
		}

	return iDecompressionBuffer;
	}

/**  
Gets a pointer to an extra buffer for general use. 

Internal use only.
@param 	aSize Minimum size of the buffer required.  
		If the buffer is too small an attempt to resize it will be made.
@return  The buffer area currently in use or NULL if there is no memory.
@internalComponent 
*/
HBufC8* RFbsSession::GetExtraBuffer(TInt aSize)
	{
	if (iHelper->iExtraBuffer)
		{
		if (iHelper->iExtraBuffer->Des().MaxSize() < aSize)
			{
			HBufC8* tempBuffer = iHelper->iExtraBuffer->ReAlloc(aSize);
			if (!tempBuffer)
				{
				return NULL;
				}
			iHelper->iExtraBuffer = tempBuffer;
			}
		}
	else
		{
		iHelper->iExtraBuffer = HBufC8::New(aSize);
		}

	return iHelper->iExtraBuffer;
	}

/**
Returns a handle assigned to this session by the server upon connection.
This method should be used instead of SessionHandle() when passing a session 
handle to FbServ APIs that require Session handles. 
@return A handle representing this session on the server. 
@pre The session has successfully connected to the server.
 */
TInt RFbsSession::ServerSessionHandle() const
	{
	__ASSERT_ALWAYS(iConnections>0,Panic(EFbsPanicBadConnection));
	return iHelper->iServerSessionHandle;
	}

/**
Returns the current sizes of the FBServ default heap, the heap for large bitmaps, 
and the heap for small bitmaps.

Not supported in release builds.

@internalComponent
@test
@param aDefaultHeapSize A reference to an integer supplied by the caller. On return from this function, contains the size of the FBServ default heap.
@param aSmallBmpHeapSize A reference to an integer supplied by the caller. On return from this function, contains the size of the FBServ heap for small bitmaps
@param aBigBmpHeapSize A reference to an integer supplied by the caller. On return from this function, contains the size of the FBServ heap for large bitmaps
@return KErrNone or one of the system wide error codes in debug mode, or KErrNotSupported in release mode.
*/
#ifdef _DEBUG
EXPORT_C TInt RFbsSession::GetHeapSizes(TInt& aDefaultHeapSize, TInt& aSmallBmpHeapSize, TInt& aBigBmpHeapSize)
 	{
 	TPckgBuf<THeapSizes> data;
 	TIpcArgs args(&data);
 	TInt ret = SendReceive(EFbsMessGetHeapSizes, args);
 	if(ret == KErrNone)
 		{
 		aDefaultHeapSize = data().iDefault;
 		aSmallBmpHeapSize = data().iSmall;
 		aBigBmpHeapSize = data().iBig;
 		}
 	return ret;
 	}
#else
EXPORT_C TInt RFbsSession::GetHeapSizes(TInt&, TInt&, TInt&)
	{
	return KErrNotSupported;
	}
#endif
