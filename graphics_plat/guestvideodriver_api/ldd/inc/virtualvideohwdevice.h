// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef VIRTUALVIDEOHWDEVICE_H
#define VIRTUALVIDEOHWDEVICE_H

#include <kernel/kernel.h>
#include <kernel/kern_priv.h>
#include "virtualvideohwdeviceinterface.h"

class DVirtualVideoHwInterface;
class DDeviceReqHandler;

_LIT( KVirtualVideoHwDevicePanic, "DVVideoHwDevice" );

/**
 * Logical device driver for the virtual Video peripheral used in the
 * platform simulator.
 */
class DVirtualVideoHwDevice : public DLogicalDevice,
                              public MVirtualVideoHwDeviceInterface
    {
public:

    /**
     * Default constructor.
     */
    DVirtualVideoHwDevice();

    /**
     * Destructor.
     */
    virtual ~DVirtualVideoHwDevice();

public: // From DLogicalDevice

    /**
     * Second stage constructor.
     * @see DLogicalDevice::Install
     */
    virtual TInt Install();
    
    /**
     * Gets the driver's capabilities.
     * @see DLogicalDevice::GetCaps
     */
    virtual void GetCaps( TDes8& aDes ) const;

    /**
     * Called by the kernel's device driver framework to create
     * a Logical Channel.
     * @see DLogicalDevice::Create
     */
    virtual TInt Create( DLogicalChannelBase*& aChannel );
    
private: // From MVirtualVideoHwDeviceInterface
    
private:

    // Interface to the hardware
    DVirtualVideoHwInterface* iHwInterface;
    
    // DFC queue
    TDfcQue* iDfcQ;
    
    // Request handler
    DDeviceReqHandler* iDeviceReqHandler;     
    };

#endif // VIRTUALVIDEOHWDEVICE_H
