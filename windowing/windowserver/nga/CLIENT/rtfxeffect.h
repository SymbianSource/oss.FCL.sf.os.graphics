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
// Description: Wserv client side local header file.
//


#if !defined(__RTFXEFFECTS_H__)
#define __RTFXEFFECTS_H__

#include <w32std.h>
#include "../SERVER/w32cmd.h"

/** 
Class is used for passing data sent to TFX APIs RegisterEffect and OverrideEffect
from client to server of wserv. Another purpose of this class is to avoid code 
duplication, when these APIs are called from RWsSession and RWindowBase.
*/
class RTFXEffect : public MWsClientClass
	{
public:
	/**
	TFXEffect Enum is used to distinguish when OverrideEffect API is called from 
	Session class or Window class. 
	*/
	enum TFXEffect
		{
		ENone,
		/**
		Used in OverrideTFXEffect() when called from RWsSession
		*/
		ETFXSession,
		/**
		Used in OverrideTFXEffect() when called from RWindowBase
		*/
		ETFXWindow
		};
	RTFXEffect(TInt aHandle, RWsBuffer* aBuffer);
	void CheckFileNameAndSetSizes(const TFileName& aResourceDir, const TFileName& aFilenamePhase1, const TFileName& aFilenamePhase2);
	void RegisterTFXEffect(TInt aAction, const TFileName& aResourceDir, const TFileName& aFilenamePhase1, const TFileName& aFilenamePhase2, TUint aAppUid);
	void OverrideTFXEffect(TBool aOneShot, TInt aAction, const TFileName& aResourceDir, const TFileName& aFilenamePhase1, const TFileName& aFilenamePhase2, TFXEffect aCalledFor);
	void AppendFileNameData(const TFileName& aResourceDir, const TFileName& aFilenamePhase1, const TFileName& aFilenamePhase2);
	void WriteDataUsingIPC(TWsClCmdRegisterEffect* aForRegister, TWsClCmdOverrideEffect* aForOverride, 
		const TFileName& aResourceDir, const TFileName& aFilenamePhase1, const TFileName& aFilenamePhase2, TFXEffect aCalledFrom);
	TBool CheckCombinedSizeWithCurrentBuffer(TInt aSize) const;
	
private:
	TInt iDirPathSizePaded;
	TInt iFileName1SizePaded;
	TInt iFileName2SizePaded;
	TInt iCombSizePaded;
	};

#endif
