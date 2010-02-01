/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#if (!defined __T_DATA_FBS_BIT_GC_H__)
#define __T_DATA_FBS_BIT_GC_H__

//	User includes
#include "T_DataBitmapContext.h"

class CT_DataFbsBitGc : public CT_DataBitmapContext
	{
public:
	static CT_DataFbsBitGc*	NewL();
	
	~CT_DataFbsBitGc();

	//	CTPData implementation
	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	virtual TAny*	GetObject();
	virtual void	SetObjectL(TAny* aObject);
	virtual void	DisownObjectL();
	void			DestroyData();

	//	CT_DataGraphicsContext implementation
	virtual CGraphicsContext*	GetGraphicsContext() const;

	//	CT_DataBitmapContext implementation
	virtual CBitmapContext*		GetBitmapContext() const;

protected:
	CT_DataFbsBitGc();
	void	ConstructL();

private:
	void	DoCmdNewL();
	void	DoCmdDestructor();
	void	DoCmdActivateL(const TDesC& aSection);
	void	DoCmdActivateNoJustAutoUpdateL(const TDesC& aSection);
	TBool	DoCmdAlphaBlendBitmapsL(const TDesC& aSection);
	void    DoCmdDrawPolyLineNoEndPointL(const TDesC& aSection);
	
	void    DoCmdCancelClipping(const TDesC& aSection);
	void    DoCmdCopySettingsL(const TDesC& aSection);
	TBool   DoCmdBitBltL(const TDesC& aSection);
	
	void	DoCmdDrawTextL(const TDesC& aSection);
	void	DoCmdDrawTextVerticalL(const TDesC& aSection);
	void	DoCmdUpdateJustificationL(const TDesC& aSection);
	void	DoCmdUpdateJustificationVerticalL(const TDesC& aSection);

	void    DoCmdFadeArea(const TDesC& aSection);
	void    DoCmdIsBrushPatternUsed(const TDesC& aSection);
	void    DoCmdIsFontUsed(const TDesC& aSection);
	void    DoCmdOrientationsAvailable(const TDesC& aSection);
	void    DoCmdRectDrawnTo(const TDesC& aSection);
	void    DoCmdResized(const TDesC& aSection);
    void    DoCmdSetDitherOrigin(const TDesC& aSection);
    void    DoCmdSetFadeMode(const TDesC& aSection);
    TBool   DoCmdSetFadingParameters(const TDesC& aSection);
    void    DoCmdSetOrientation(const TDesC& aSection);
    void    DoCmdSetShadowMode(const TDesC& aSection);
    void    DoCmdSetUserDisplayMode(const TDesC& aSection);
    void    DoCmdShadowArea(const TDesC& aSection);
    void    DoCmdUseBrushPatternL(const TDesC& aSection);
    void    DoCmdUseFontL(const TDesC& aSection);
    void    DoCmdUseFontNoDuplicateL(const TDesC& aSection);
    void    DoCmdPromptMessageL(const TDesC& aSection);
protected:
	/** test step reference*/
	CFbsBitGc*	iFbsBitGc;
	  
	};

#endif /* __T_DATA_FBS_BIT_GC_H__ */
