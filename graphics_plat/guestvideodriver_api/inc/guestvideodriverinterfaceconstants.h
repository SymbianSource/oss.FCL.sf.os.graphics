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

#ifndef VirtualVideoInterfaceConstants_h_
#define VirtualVideoInterfaceConstants_h_

//      virtualhw.irq => ic.input[391];
//        pvbus.pvbus_m_range[0x60000000..0x62ffffff] => graphicsvirtualhw.mbus; //=>Reserve own memory area later
//        pvbus.pvbus_m_range[0x63000000..0x63000fff] => graphicsvirtualhw.pbus; //=>Reserve own memory area later                

#ifdef PLATSIM_CONFIG
#define VVI_BASE 0x60000000
#else
#define VVI_BASE 0x00000000
#endif // PLATSIM_CONFIG

// base address
#define VVI_REGISTERS_BASE_ADDRESS VVI_BASE + 0x03000000 
#define VVI_REGISTERS_MEMORY_SIZE 0x1000
#define VVI_REGISTERS_MASK 0x0FFF

#define VVI_PARAMETERS_INPUT_BASE_ADDRESS VVI_BASE + 0x00000000
#define VVI_PARAMETERS_INPUT_MEMORY_SIZE  0x01000000

#define VVI_PARAMETERS_OUTPUT_BASE_ADDRESS VVI_BASE + 0x01000000
#define VVI_PARAMETERS_OUTPUT_MEMORY_SIZE  0x01000000

#define VVI_SURFACEBUFFER_BASE_ADDRESS VVI_BASE + 0x02000000
#define VVI_SURFACEBUFFER_MEMORY_SIZE  0x01000000

// register indices, TODO: cleanup needed, some are not used
#define VVI_R_ID                                0x0000
#define VVI_R_IRQ_ENABLE                        0x0004
#define VVI_R_IRQ_STATUS                        0x0008
#define VVI_R_COMMAND                           0x000c
#define VVI_R_PARAMETER_LOAD                    0x0010
#define VVI_R_ERROR                             0x0014
#define VVI_R_INPUT_BUFFER_TAIL                 0x0018
#define VVI_R_INPUT_BUFFER_HEAD                 0x001c
#define VVI_R_INPUT_BUFFER_READ_COUNT           0x0020
#define VVI_R_INPUT_BUFFER_WRITE_COUNT          0x0024
#define VVI_R_INPUT_BUFFER_MAX_TAIL             0x0028
#define VVI_R_REQUEST_ID                        0x002c
#define VVI_R_SHARED_CMD_MEMORY_BASE            0x0030
#define VVI_R_SHARED_SURFACEBUFFER_MEMORY_BASE  0x0034
#define VVI_R_LASTREG                           0x0038  // not a register, address of last register

// COMMAND register values
#define VVI_EXECUTE 0

//IRQ number
#ifdef PLATSIM_CONFIG
#define VVI_IRQ 391
#else
// Jani - This is defined in syborg.h "EIntGraphics" - should we get it from there?
#define VVI_IRQ 13
#endif // PLATSIM_CONFIG

#endif
