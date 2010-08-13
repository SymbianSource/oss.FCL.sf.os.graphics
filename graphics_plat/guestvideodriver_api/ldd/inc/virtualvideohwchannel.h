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


#ifndef VIRTUALVIDEOHWCHANNEL_H
#define VIRTUALVIDEOHWCHANNEL_H

#include <kernel/kernel.h>
#include <kernel/kern_priv.h>
#include "sgextension.h"//For SgExtension

#include "virtualvideohwinterface.h"

class MVirtualVideoHwDeviceInterface;
class DDeviceReqHandler;

_LIT( KVirtualVideoHwChannelPanic, "DVVideoHwChannel" );

/**
 * Logical channel for the ARM virtual video peripheral used in the
 * platform simulator.
 */
class DVirtualVideoHwChannel : public DLogicalChannel
    {
public:

    /**
     * Constructor.
     * 
     * @param aDeviceInterface
     *   Interface to the device driver.
     * @param aHwInterface
     *   Interface to the hardware.
     * @param aDeviceReqHandler
     *   Shared request handler for channels
     * @param aQueue
     *   Deferred function call queue.
     */
    DVirtualVideoHwChannel(
            MVirtualVideoHwDeviceInterface& aDeviceInterface,
            DVirtualVideoHwInterface& aHwInterface,
            DDeviceReqHandler& aDeviceReqHandler,
            TDfcQue* aQueue
            );

    /**
     * Destructor.
     */
    virtual ~DVirtualVideoHwChannel();

protected: // From DLogicalChannelBase

    /**
     * Second phase constructor for DLogicalChannelBase objects.
     * @see DLogicalChannelBase::DoCreate
     */
    virtual TInt DoCreate( TInt aUnit, const TDesC8* aInfo, const TVersion& aVer );

protected: // From DLogicalChannel

    /**
     * Handles a client request in the client context.
     * @see DLogicalChannelBase::Request
     */
    virtual TInt Request( TInt aReqNo, TAny *a1, TAny *a2 );

    /**
     * Processes a message for this logical channel.
     * @see DLogicalChannel::HandleMsg
     */
    void HandleMsg( TMessageBase* aMsgRequest );

protected: // New functions
    
    /**
     * This function is called from HandleMsg(). It is used to handle
     * asynchronous requests.
     * 
     * @param aUserThread
     *   User thread
     * @param aReqNo
     *   Request number
     * @param aStatus
     *   Request status
     * @param a1
     *   Optional parameter
     * @param a2
     *   Optional parameter
     * @return
     *   KErrNone - Successful
     *   KErrArgument - a1 is NULL
     *   KErrNotSupported - Invalid aReqNo
     *   System wide errorcodes in other error situations.
     */
    virtual TInt DoRequest(
            DThread*& aUserThread,
            TInt aReqNo,
            TRequestStatus* aStatus,
            TAny* a1 = NULL,
            TAny* a2 = NULL );
    
    /**
     * This function is called from HandleMsg(). It is used to handle
     * synchronous requests.
     * 
     * @param aUserThread
     *   User thread
     * @param aFunction
     *   Function number
     * @param a1
     *   Required parameter
     * @param a2
     *   Optional parameter
     * @return
     *   KErrNone - Successful
     *   KErrArgument - a1 is NULL
     *   KErrNotReady - Buffer not allocated yet for playback or recording.
     *   KErrInUse - Playback or recording already ongoing.
     *   KErrNotSupported - Invalid aFunction
     *   System wide errorcodes in other error situations.
     */
    virtual TInt DoControl( DThread*& aUserThread, TInt aFunction, TAny* a1, TAny* a2 = NULL );

private:

    /// Interface to the logical device
    MVirtualVideoHwDeviceInterface& iDeviceInterface;
    
    /// Interface to the hardware
    DVirtualVideoHwInterface& iHwInterface;

    TInt GetSgImageMetaData(TSgImageMetaData& aSgInfo, const TUint64 aId);

    TInt iValue;
    
    /// Request handler (Owned)
    DDeviceReqHandler& iReqHandler;
    };

#endif // VIRTUALVIDEOHWCHANNEL_H
