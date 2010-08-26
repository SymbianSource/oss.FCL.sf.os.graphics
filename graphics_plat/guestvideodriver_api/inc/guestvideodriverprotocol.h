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
// Header file for the guest video driver


#ifndef __GUESTVIDEODRIVERPROTOCOL_H
#define __GUESTVIDEODRIVERPROTOCOL_H

#include <e32cmn.h>
#include "guestvideodrivercommon.h"

/**
 * Namespace for Platform Simulator Video Driver.
 * The user API - kernel LDD messaging protocol is defined within this
 * namespace.
 */
namespace GuestVideoDriver
    {

    /**
     * Enumeration of different requests
     */
    enum TRequestId
	{
	ERequestExecuteCommand = 0x1001, 	// Asynchronous (DoRequest)
	ERequestMapAddress = 0x1002, 		// Synchronous  (DoControl)
	ERequestLoadCommands = 0x1004, 		// Synchronous  (DoRequest)
	ERequestSgHandles = 0x1005, 		// Synchronous  (DoControl)
	ERequestFrameBaseAddress = 0x1006, 	// Synchronous  (DoControl)
	};

    }


#endif // __GUESTVIDEODRIVERPROTOCOL_H
