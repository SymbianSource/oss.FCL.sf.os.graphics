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


#ifndef VIRTUALVIDEOHWINTERFACE_H
#define VIRTUALVIDEOHWINTERFACE_H

#include <kernel/kernel.h>
#include <graphics/guestvideodriverinterfaceconstants.h>

/**
 * Class that encapsulates the Virtual Video Hw interface behind a function
 * interface. Intended to be instantiated only once in the device driver.
 */
class DVirtualVideoHwInterface : public DBase
    {
    public: // Data types
	
	/**
	 * Enumeration for different interrupts.
	 */
	enum TInterrupt
	    {
	    EInterruptNewDataAvailable  = 0x01 ///< Video HW has new data available
	    };
	/**
	 * Enumeration for different registers.
	 */
	enum TRegister
	    {
	    ERegId,			     ///< Id register
	    ERegIrqEnable,		      ///< IRQ enabling register
	    ERegIrqStatus,		      ///< IRQ status register
	    ERegCommand,			///< Command register
	    ERegParameterLoad,
	    ERegError,			 ///< 
	    ERegInputBufferTail,
	    ERegInputBufferHead,
	    ERegInputBufferReadCount,
	    ERegInputBufferWriteCount,
	    ERegInputBufferMaxTail,
	    ERegRequestId,
		ERegSharedCmdMemBase,
		ERegSharedFramebufferMemBase,
	    ENumRegisters		       ///< Number of registers						
	    
	    };

    public: // Construction and destruction

	/**
	 * Constructor.
	 */
	DVirtualVideoHwInterface();

	/**
	 * Destructor.
	 * NOTE: Enters and leaves critical section.
	 */
	virtual ~DVirtualVideoHwInterface();

    public: // Functions


	/**
	 * Initializes the Virtual Video HW's memory mapped I/O model.
	 * NOTE: Enters and leaves critical section.
	 *
	 * @return System-wide error code
	 */
	TInt InitParametersInputMemory();
	TInt InitParametersOutputMemory();
	
	/**
	 * Enables interrupts in the Virtual Video HW.
	 *
	 * @param aChannel The channel, whose interrupts are enabled
	 * @param aInterruptBitField A bitmask of TInterrupt enumeration.
	 *      Enabled interrupt bits should be set to 1 and disabled interrupt
	 *      bits to 0.
	 */
	void EnableInterrupts( TUint32 aInterruptBitField );
	
	/**
	 * Initializes the Virtual Video HW's memory mapped I/O model.
	 * NOTE: Enters and leaves critical section.
	 *
	 * @return System-wide error code
	 */
	 TInt InitRegisterMemory();
	/**
	 * Retrieves interrupt enabled state in the Virtual Video HW.
	 *
	 * @param aChannel The channel, whose interrupt states are queried
	 * @param aInterruptBitField A reference parameter, where a bitmask of
	 *      TInterrupt enumeration will be written. Enabled interrupt bits
	 *      are set to 1 and disabled interrupt bits to 0.
	 */
	void GetInterruptEnabledState( TUint32& aInterruptBitField );

	/**
	 * Retrieves interrupt status in the Virtual Video HW.
	 *
	 * @param aChannel The channel, whose interrupt status is queried
	 * @param aInterruptBitField A reference parameter, where a bitmask of
	 *      TInterrupt enumeration will be written. Those interrupt lines
	 *      are active, whose bits are set to 1 in the bitmask.
	 */
	void GetInterruptStatus( TUint32& aInterruptBitField );

	/**
	 * Resets interrupt status in the Virtual Video HW.
	 *
	 * @param aChannel The channel, whose interrupt statuses are being reset
	 * @param aInterruptBitField The bitmask of TInterrupt enumeration. Those
	 *      interrupts are reset, whose bits are set to 1 in the bitfield.
	 */
	void ResetInterruptStatus( TUint32 aInterruptBitField );
	
	/**
	 * Issues a command to the Virtual Video HW.
	 *
	 * @param aCommand One of TCommand enumeration commands.
	 */
	void IssueCommand( TUint32 aCommand );

	/**
	 * Set shared command memory base address.
	 *
	 * @param aPhysicalAddress Physical address of the contiguous memory area reserved for video device.
	 */
	void SetSharedCmdMemBase( TUint32 aPhysicalAddress );
		
	/**
	 * Set shared framebuffer memory base address.
	 *
	 * @param aPhysicalAddress Physical address of the contiguous memory area reserved for video device.
	 */

	void SetSharedFramebufferMemBase( TUint32 aPhysicalAddress );
	
	/**
	 * Address to input parameter buffer
	 */
	TLinAddr InputParametersAddress(){ return iInputParametersChunkKernelAddress; }
	
	/**
	 * Address to output parameter buffer
	 */
	TLinAddr OutputParametersAddress(){ return iOutputParametersChunkKernelAddress; }

	
	/**
	 * Gets a register value.
	 * 
	 * @param aRegisterRange The register range
	 * @param aRegister The register in the given register range, whose value is being read
	 * @param aValue A reference parameter, where the register's value is written
	 */
	void GetRegisterValue(
	    TRegister aRegister,
	    TUint32& aValue );

	/**
	 * Sets a register value.
	 * 
	 * @param aRegisterRange The register range
	 * @param aRegister The register in the given register range, whose value is being written
	 * @param aValue A new value to be written into register
	 */
	void SetRegisterValue( TRegister aRegister,
	    TUint32 aValue );

	IMPORT_C static TPhysAddr GetFrameBase();

	public:
        TPhysAddr iFrameRamPhys;
        static DVirtualVideoHwInterface * pVVHIf;

    protected:

	/**
	 * Maps a virtual address range to physical address space
	 * 
	 * @param aPhysicalAddress 
	 * @param aMaxSize 
	 * @param aChunk
	 * @param aLinAddr 
	 * @return System wide error code
	 */
	TInt InitPhysicalMemory( TUint32 aPhysicalAddress, 
		TInt aMaxSize, DChunk*& aChunk, TLinAddr& aLinAddr );

    private: // Data

	/// A memory chunk that is mapped to the Virtual Video HW's physical memory area
	DChunk* iRegisterMemoryChunk;	
	/// The virtual start address of the Virtual Video HW's memory map
	TLinAddr iRegisterChunkKernelAddress;

	/// A memory chunk that is mapped to the Virtual Video HW's physical memory area
	DChunk* iInputParametersMemoryChunk;	
	/// The virtual start address of the Virtual Video HW's memory map
	TLinAddr iInputParametersChunkKernelAddress;

	/// A memory chunk that is mapped to the Virtual Video HW's physical memory area
	DChunk* iOutputParametersMemoryChunk;	
	/// The virtual start address of the Virtual Video HW's memory map
	TLinAddr iOutputParametersChunkKernelAddress;
    TPhysAddr iVideoRamPhys;
    };


#endif // VIRTUALVIDEOHWINTERFACE_H
