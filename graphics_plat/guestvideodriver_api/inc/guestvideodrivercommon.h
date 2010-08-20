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

#ifndef __GUESTVIDEODRIVERCOMMON_H
#define __GUESTVIDEODRIVERCOMMON_H

#include <e32cmn.h>

/**
 * Namespace for Platform Simulator Audio Driver.
 */
namespace GuestVideoDriver
    {
    /**
     * Name of the DFC thread used by logical channels.
     */
    _LIT( KDfcThreadName, "DfcThreadVirtualVideoHw" );
    
    /**
     * Name of the logical device used to identify the device.
     */
    _LIT( KDeviceName, "VirtualVideoHwDevice" );

    /**
     * Name of the logical device dll file name.
     */
    _LIT( KDeviceFileName, "guestvideohw" );

    /**
     * Current version of the driver.
     */
    const TInt KMajorVer = 1;
    const TInt KMinorVer = 0;
    const TInt KBuildVer = 1;
    }

#endif // __GUESTVIDEODRIVERCOMMON_H
