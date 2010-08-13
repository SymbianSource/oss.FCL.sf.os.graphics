// Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef DEVICEREQHANDLER_H
#define DEVICEREQHANDLER_H

#include <kernel/kernel.h>
#include <kernel/kern_priv.h>
#include <e32cmn.h>

#include <sgresource/sgcommon.h>//TSgImageMetaData
#include "sgextension.h"//For SgExtension

#include "guestvideodriverprotocol.h"
#include "virtualvideohwinterface.h"
#include "remotefunctioncall.h"
#include "requestbuffer.h"
#include "reqhandlerextension.h"

#include <EGL/egltypes.h>//For EGLint

class RSgImage;//we only need pointers anyways

using namespace GuestVideoDriver;


//Container class for the mapping from pbuffers to sgimages
class TPbSId
    {
    public:
    TInt iPbuffer;
    TUint64 iSgId;
    TPbSId( TInt aP, TUint64 aId ) : iPbuffer( aP ), iSgId( aId ){}
    };
class TVGISId
    {
    public:
    TInt iVGImage;
    TUint64 iSgId;
    TVGISId( TInt aI, TUint64 aId ) : iVGImage( aI ), iSgId( aId ){}
    };

/**
 * Class for handling requests to the Video driver.
 */
class DDeviceReqHandler : public DBase, MReqHandlerCallback , protected MRequestBufferBookKeepingWriter
    {
public:
    enum TRequestMode
        {
        ENormal, //to be issued by the normal route
        EHandled, //already completed (in the scheduler)
        EQueued //queued into another outstanding request 
        };
    enum TOpChainType
        {//for performing a series of operations in sequence
        EOpInit = 1
        };
    enum { KMaxFreeRequests = 20 }; 
    
    struct SgImageMetaInfo
    	{
    	TUint32 iSgImagePointer;
    	};
    /*
     * Stores data belonging to a client thread
     */
    struct TPerThreadData
        {
        TRequestStatus* iStatus;
        DThread* iAsyncClient;//asynchronous client
        TUint32 iProcessId;
        TUint32 iThreadId;
        
        TUint32 iCurrApiUid;//5.2 in "Implementing sgImage on Guest EGL"
        TUint32 iCurrOpCode;//Not required
        
        TBool   iErrUpdatedGLES;
        TBool   iErrUpdatedVG;
        TBool   iErrUpdatedEGL;
        TUint32  iErrorGLES;//The error for the previous GLES call
        TUint32  iErrorVG;//The error for the previous VG call
        TUint32  iErrorEGL;//The error for the previous EGL call
        
        EGLContext iContext;//The last set context

        
        RArray<SgImageMetaInfo> iImageInfo;
        

        TBool sameThread( TUint32 aProcId, TUint32 aThreadId )
            {
            return ( ( aProcId == iProcessId && aThreadId == iThreadId ) ? ETrue : EFalse );
            }
        TPerThreadData( TRequestStatus* aStatus, DThread* aAsyncClient, TUint32 aProcId, TUint32 aThreadId ) :
            iStatus( aStatus),
            iAsyncClient(aAsyncClient),
            iProcessId( aProcId ),
            iThreadId( aThreadId ),
            iErrUpdatedGLES(EFalse),
            iErrUpdatedVG(EFalse),
            iErrUpdatedEGL(EFalse)
            {}
        };
    
    struct TTransactionInfo
        {
    public:
        TUint32 iTransactionId;
        //this must be valid for the whole duration of the transaction, and is now owned here
        TAny* iTransactionData;
        TTransactionInfo( TUint32 aTransactionId, TAny* aTransactionData = 0 ) :
            iTransactionId( aTransactionId ),
            iTransactionData( aTransactionData )
            {}
        };
    
    /*
     * Stores an async request before serialisation
     */
    struct TAsyncRequest
        {
        enum TRequestType
            {
            ERTDriver,
            ERTRequest,
            ERTCommandLoad
            };
        
        TAsyncRequest( TRequestStatus* aStatus,
                        DThread* aAsyncClient,
                        TAny* aA1,
                        TAny* aA2,
                        TRequestType aCommandLoadRequest ):
        iStatus( aStatus ),
        iAsyncClient( aAsyncClient ),
        iA1( aA1 ),
        iA2( aA2 ),
        iCommandLoadRequest( aCommandLoadRequest ),
        iNext( NULL )
            {    
            }

        void Init(TRequestStatus* aStatus,
                        DThread* aAsyncClient,
                        TAny* aA1,
                        TAny* aA2,
                        TRequestType aCommandLoadRequest )
            {
            iStatus = aStatus;
            iAsyncClient = aAsyncClient;
            iA1 = aA1;
            iA2 = aA2;
            iCommandLoadRequest = aCommandLoadRequest;
            iNext = NULL;
            }
        
        void AppendToList( TAsyncRequest* iPrev )
            {
            if ( iPrev->iNext )
                {
                this->iNext = iPrev->iNext;
                }
            iPrev->iNext = this;
            }
        
        void RemoveFromList( TAsyncRequest* iPrev )
            {
            if ( iPrev->iNext == this )
                {
                iPrev->iNext = this->iNext;
                }
            this->iNext = NULL;
            }
        
        //Current request status
        TRequestStatus* iStatus;
        
        //Current asynchronous client
        DThread* iAsyncClient;
        
        TAny* iA1;
        TAny* iA2;

        TRequestType iCommandLoadRequest;
        
        RemoteFunctionCallData iRemoteFunctionCall;
        
        //Link for linked list
        TAsyncRequest* iNext;
        };
    
    struct TAsyncRequestRoot: public TAsyncRequest
        {
        TAsyncRequestRoot():
        TAsyncRequest( NULL, NULL, NULL, NULL, ERTRequest )
            {
            }
        
        void AppendToLast( TAsyncRequest* aReq )
            {
            TAsyncRequest* last( this );
            TAsyncRequest* next( iNext );            
            while ( next )
                {
                last = next;
                next = next->iNext;
                }
            aReq->AppendToList( last );
            }

        void DeleteAll()
            {
            TAsyncRequest* next( iNext );
            while ( next )
                {
                TAsyncRequest* nextToDel( next );
                next = next->iNext;
                delete nextToDel;
                }
            }
        };

    /**
     * Constructor.
     * 
     * @param aHwInterface
     *   Interface to the hardware
     * @param aQueue
     *   Deferred function call queue
     * @param aMdaifSvTable
     *   MDAIF shared variable table
     */
    DDeviceReqHandler(
            DVirtualVideoHwInterface& aHwInterface,
            TDfcQue* aQueue );

    /**
     * Destructor.
     */
    virtual ~DDeviceReqHandler();
    
    /**
     * Handles a command execution
     * 
     * @param aUserThread
     *   Thread making the request
     * @param aStatus
     *   Request status to be used in request completion.
     * @param a1
     *   Request parameter 1
     */
    TInt HandleExecuteAsync(
            DThread*& aUserThread,
            TRequestStatus* aStatus,
            TAny* a1);

    /**
     * Handles a command buffering
     * 
     * @param aUserThread
     *   Thread making the request
     * @param a1
     *   Request parameter 1
     */
    TInt HandleLoadCommands(
            DThread*& aUserThread,
            TRequestStatus* aStatus,
            TAny* a1 );
    
    TInt HandleClientShutdown( TUint aProcessId, TUint aThreadId );
    
public: // Called from Dfc context
    
    /**
     * Directs hardware interrupts to appropriate functions.
     */
    void ProcessDfc();
            
public: // Inline functions

    /**
     * Returns current interrupt ID.
     * 
     * @return
     *   Interrupt ID.
     */
    inline TInt InterruptId() { return iInterruptId; }
        
    /**
     * Returns hardware interface.
     * 
     * @return
     *   Hardware interface reference.
     */
    inline DVirtualVideoHwInterface& HwInterface() { return iHwInterface; }
    
    /**
     * Returns DFC.
     * 
     * @return
     *   Reference to TDfc object.
     */
    inline TDfc& Dfc() { return iDfc; }

    /**
     * Returns current interrupt ID.
     * 
     * @param aInterrupt
     *   Interrupt to be reset.
     * @return
     *   KErrNone - Successful
     *   System wide errorcodes in error situations.
     */
    inline TInt ResetAndEnableInterrupt(
            DVirtualVideoHwInterface::TInterrupt aInterrupt )
        {
        iHwInterface.ResetInterruptStatus( aInterrupt );
        Interrupt::Clear( InterruptId() );
        return Interrupt::Enable( iInterruptId );
        }

    /**
     * Process next request from ring buffer
     */ 
    TInt ProcessNextPendingRequest();
    
protected: // Ring buffer book keaping routines
    void IncrementInputWriteCount( TUint32 aWriteCount )
        {
        TUint32 inputBufferWriteCount = GetWriteCount() + aWriteCount;
        iHwInterface.SetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferWriteCount,
                inputBufferWriteCount );
        }
    
    TUint32 GetInputWriteCount()
        {
        TUint32 inputBufferWriteCount;
        iHwInterface.GetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferWriteCount,
                inputBufferWriteCount );
        return inputBufferWriteCount;
        }
    
    TUint32 GetInputReadCount()
        {
        TUint32 inputBufferReadCount;
        iHwInterface.GetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferReadCount,
                inputBufferReadCount );
        return inputBufferReadCount;
        }
    
    TUint32 InputBufferTail()
        {
        TUint32 val;
        iHwInterface.GetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferTail,
                val );
        return val;
        }
    
    TUint32 InputBufferHead()
        {
        TUint32 val;
        iHwInterface.GetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferHead,
                val );
        return val;        
        }
    
    void SetInputBufferTail( TUint32 aIndex )
        {
        iHwInterface.SetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferTail,
                aIndex );    
        }
    
    void SetInputBufferHead( TUint32 aIndex )
        {
        iHwInterface.SetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferHead,
                aIndex );
        }

protected: // From MRequestBufferBookKeeping
    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    void IncrementWriteCount( TUint32 aWriteCount ){ IncrementInputWriteCount( aWriteCount ); }

    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    TUint32 GetWriteCount(){ return GetInputWriteCount(); }
    
    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    TUint32 GetReadCount(){ return GetInputReadCount(); }

    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    TUint32 BufferTail(){ return InputBufferTail(); }

    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    TUint32 BufferHead(){ return InputBufferHead(); }

    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    void SetBufferHead( TUint32 aIndex ){ SetInputBufferHead( aIndex ); }

    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    TUint32 MaxTailIndex()
        {
        TUint32 val;
        iHwInterface.GetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferMaxTail,
                val );
        return val;        
        }
    
    /**
     * See MRequestBufferBookKeepingWriter
     */ 
    void SetMaxTailIndex( TUint32 aIndex )
        {
        iHwInterface.SetRegisterValue( DVirtualVideoHwInterface::ERegInputBufferMaxTail,
                        aIndex );
        }
    
protected: //Command Scheduling functions
    /**
     * DDeviceReqHandler::InterpretRequest
     * This function is the focal point of the implementation
     * of the 1st paragraph of 3.4 in the "Implementing SgImage" design doc.
     * This function therefore intercepts and interprets SOME EGL,VG (and, in the future GLES)
     * calls.
     * 
     * @param aReq the request
     * 
     * @return TBool has the call already been processed? If true, then no further processing
     * is required for that call (doesn't have to be sent host-side).
     */
    DDeviceReqHandler::TRequestMode InterpretRequest( TAsyncRequest* aReq );
    
    
    void ConsumeSchedulerInitiatedRequestResult( TAsyncRequest* aReq );
    
    /**
     * RemoveClientData
     * @param aProcId
     * @param aThreadId
     */
    void RemoveClientData( const TUint32 aProcId, const TUint32 aThreadId );
    /**
     * Initiate a request to the host side from the scheduler (this)
     * This is needed so that the scheduler can make requests that are not
     * directly client-originated.
     */
    TBool InitiateRequestWithReply( TAsyncRequest* req, TAny* aTransactionData = 0 );

    /**
     * Helper function for getting the stored client data for the proc/thread, if present
     * @return TPerThreadData object, or NULL, if no object was found with the right proc/thread id
     */
    TPerThreadData* GetClientData( const TUint32 aProcId, const TUint32 aThreadId );
    
    /**
     * getVGSync
     * Check if the VGImage is sg image backing, and if it's sync flag is set to dirty.
     * If both above conditions are met, add the required parameters to the hostside call
     * so that the hostside can proceed to making the sync.
     * 
     */
    DDeviceReqHandler::TRequestMode DDeviceReqHandler::getVGSync( VGImage aImg, TAsyncRequest* aReq );
    
    /**
     * 
     * @param aReq the request to add the additional info to
     * @param aImageIndexInReq the index of the vg image in the rfc call. This will
     *        be used to determine what the sgImage is, and find the sync bit
     * @param aSetBufferDirty: set the buffer dirty in any case
     * @param aAddFalseForOther. All sync operations are prepared to handle two vgimages, thus need to have another
     *        value to read. Set this to ETrue if you are not going to add another real sync operation.
     */
    void getVGSyncInOp( TAsyncRequest* aReq, TInt aSgHandleIndexInReq, TBool aSetBufferDirty = EFalse );
    
public:
    HBuf8* OpenSgImageMetaData( const TUint64 aId, DSgResource*& aResource );
	void CreateSgImagePbuffer( const TSgImageMetaData& aInfo, TRequestStatus* aStatus, DThread* aThread );
	void CreateSgImageVGImage( const TSgImageMetaData& aInfo, TRequestStatus* aStatus, DThread* aThread );
	TInt DestroySgImage( const TUint64 aId );
    void ContinueInit( TAsyncRequest* aReq );

    


private: //Request management
    TInt iOpType; 
    TInt iOpChainPhase;//just a running nummber
    TInt iOpReturn;//return value from host side calls
    TRequestStatus iOpStatus;//dummy request status
    /*
     * Allocates a new request data structure
     */
    TAsyncRequest* AllocRequest( TRequestStatus* aStatus,
            DThread* aAsyncClient,
            TAny* aA1,
            TAny* aA2,
            TAsyncRequest::TRequestType aRequestType = TAsyncRequest::ERTRequest );
    
    

    /*
     * Releases a request data structure
     */
    void ReleaseRequest( TAsyncRequest* aReq );

    /*
     * Free request data structures
     */
    TAsyncRequestRoot iFreeListRoot;
    
    /*
     * Number of free request data structures
     */
    TInt iFreeCount;
    
private:
    
    TLinearOrder<TPbSId> iPbufferSgMapOrder;
    TLinearOrder<TVGISId> iVGImageSgMapOrder;
    //map pbuffer surface handles -> sgImage ID
    RArray<TPbSId> iPbufferSgMap;
    RArray<TVGISId> iVGImageSgMap;
    //The data structs for storing per process&thread information
    RPointerArray<TPerThreadData>   iClientData;
    //The current client
    TPerThreadData* iCurrClientData;
    RArray<TTransactionInfo> iSchedulerInitiatedTransactionIds;
    TAny* iSchedulerInitiatedTransactionData;
    // The process- and thread ids to use in driver rfcs
    // This is needed as the calling thread may be different, while we still want the same
    // state to remain on the hostside.
    TUint32 iDriverRfcProcessId;
    TUint32 iDriverRfcThreadId;
    //Is egl initialized or not. If not, then it needs to be done before processing any other command
    TBool iInitialized;
    
    /// Interface to the hardware.
    DVirtualVideoHwInterface& iHwInterface;

    /// DFC.
    TDfc iDfc;
    
    /// Current interrupt ID.
    TInt iInterruptId;
    
    /// Kernel address of the shared chunk. 
    TLinAddr iChunkKernelAddress;
    
    /// Size of the chunk.
    TUint32 iChunkSize;
    
    //Request lists
    TAsyncRequestRoot iSerializedRequestRoot;
    TAsyncRequestRoot iPendingRequestRoot;

    /*
     * Current transaction id 
     */
    TUint32 iTransactionId;
    
    /*
     * Ring buffer for requests
     */    
    RequestBufferWriter iRequestBuffer;
    
    };

#endif // DEVICEREQHANDLER_H
