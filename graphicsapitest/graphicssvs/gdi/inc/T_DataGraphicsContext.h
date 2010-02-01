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

/**
@test
@internalComponent

This contains CT_DataGraphicsContext
*/

#if (!defined __T_DATA_GRAPHICS_CONTEXT_H__)
#define __T_DATA_GRAPHICS_CONTEXT_H__

//	epoc32 includes
#include <gdi.h>

//	User Includes
#include "DataWrapperBase.h"

class CT_DataGraphicsContext : public CDataWrapperBase
	{
public:
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

protected:
	CT_DataGraphicsContext();
	virtual CGraphicsContext*	GetGraphicsContext() const = 0;

	void	DoCmdCancelClippingRect();
	void	DoCmdCancelClippingRegion();
	void	DoCmdDeviceL(const TDesC& aSection);
	void	DoCmdDiscardBrushPattern();
	void	DoCmdDiscardFont();
	void	DoCmdDrawArc(const TDesC& aSection);
	void	DoCmdDrawBitmapL(const TDesC& aSection);
	void	DoCmdDrawBitmapMaskedL(const TDesC& aSection);
	void	DoCmdDrawEllipse(const TDesC& aSection);
	void	DoCmdDrawLine(const TDesC& aSection);
	void	DoCmdDrawLineBy(const TDesC& aSection);
	void	DoCmdDrawLineTo(const TDesC& aSection);
	void	DoCmdDrawPie(const TDesC& aSection);
	void	DoCmdDrawPolygonL(const TDesC& aSection);
	void	DoCmdDrawPolyLineL(const TDesC& aSection);
	void	DoCmdDrawRect(const TDesC& aSection);
	void	DoCmdDrawRoundRect(const TDesC& aSection);
	void	DoCmdDrawText(const TDesC& aSection);
	void	DoCmdDrawTextExtendedL(const TDesC& aSection);
	void	DoCmdDrawTextVertical(const TDesC& aSection);
	void	DoCmdGetShadowColor(const TDesC& aSection);
	void	DoCmdGetUnderlineMetrics(const TDesC& aSection);
	void	DoCmdJustificationInPixels(const TDesC& aSection);
	void	DoCmdMapColorsL(const TDesC& aSection);
	void	DoCmdMoveBy(const TDesC& aSection);
	void	DoCmdMoveTo(const TDesC& aSection);
	void	DoCmdPlot(const TDesC& aSection);
	void	DoCmdReserved();
	void	DoCmdReset();
	void	DoCmdSetBrushColor(const TDesC& aSection);
	void	DoCmdSetBrushOrigin(const TDesC& aSection);
	void	DoCmdSetBrushStyle(const TDesC& aSection);
	void	DoCmdSetClippingRegion(const TDesC& aSection);
	void	DoCmdSetCharJustification(const TDesC& aSection);
	void	DoCmdSetClippingRect(const TDesC& aSection);
	void	DoCmdSetDrawMode(const TDesC& aSection);
	void	DoCmdSetOrigin(const TDesC& aSection);
	void	DoCmdSetPenColor(const TDesC& aSection);
	void	DoCmdSetPenSize(const TDesC& aSection);
	void	DoCmdSetPenStyle(const TDesC& aSection);
	void	DoCmdSetShadowColor(const TDesC& aSection);
	void	DoCmdSetStrikethroughStyle(const TDesC& aSection);
	void	DoCmdSetUnderlineStyle(const TDesC& aSection);
	void	DoCmdSetWordJustification(const TDesC& aSection);
	void	DoCmdUseBrushPatternL(const TDesC& aSection);
	void	DoCmdUseFontL(const TDesC& aSection);
	};

#endif /* __T_DATA_GRAPHICS_CONTEXT_H__ */
