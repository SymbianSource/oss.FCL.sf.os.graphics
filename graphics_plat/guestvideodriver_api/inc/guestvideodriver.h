// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
// Header file for guest video driver

#ifndef __GUESTVIDEODRIVER_H__
#define __GUESTVIDEODRIVER_H__

#include <e32cmn.h>
#include <e32std.h>
#include <babitflags.h>

#include "guestvideodrivercommon.h"

class RemoteFunctionCallData;

//Local request buffering size
const TInt KRequestBufferSize = 20000;

/**
 * Guest video driver user API. One instance exists per process.
 */
NONSHARABLE_CLASS(RGuestVideoDriver) : public RBusLogicalChannel
    {
public:
	// Default constructor
	IMPORT_C RGuestVideoDriver();

	// Destructor
	IMPORT_C ~RGuestVideoDriver();

	/**
	 * Opens this driver handle. Calls RBusLogicalChannel::DoCreate.
	 * 
	 * @return System wide error code
	 */
	IMPORT_C TInt Open();

	/**
	 * Executes a command 
	 * @param aRequestData Remote function call
	 */
	IMPORT_C void ExecuteCommand(RemoteFunctionCallData& aRequestData);

	/**
	 * Flushes any buffered commands.
	 */
	IMPORT_C void Flush();

	IMPORT_C TInt MapToHWAddress( const TInt aChunkHandle, TUint32& aHWAddress );

	IMPORT_C TInt EglGetSgHandles( const TUint64 aId, TUint64 *aSgHandles );

	/**
     * Gets Base Address of Surface Buffer
     * @param aHWAddress	returned Base Address
     *
	 * @note	A base address of 0 is invalid, and indicates a failure to
     *          obtain the base address.
	 */
	IMPORT_C TInt GetSurfaceBufferBaseAddress( TUint32& aHWAddress );


private:
	/*
	 Buffer, or send if it needs a reply, a command to the virtual video hardware
	 @param aRFC Remote function call to buffer
	 @return ETrue if buffering succeeds, otherwise the aRFC needs to be executed directly
	 */
	TBool BufferCommand( RemoteFunctionCallData& aRFC );

	// Sends buffered commands 
	void SendBuffer();

private:
	TBool     iIsOpen;
	RMutex    iThreadLock;
	/**
	 * Request buffer
	 */
	TUint32                   iProcessId;
	TBuf8<KRequestBufferSize> iBuffer;
    };


#endif // __GUESTVIDEODRIVER_H__
