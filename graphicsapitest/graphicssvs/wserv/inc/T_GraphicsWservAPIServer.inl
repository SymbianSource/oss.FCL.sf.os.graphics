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

This contains CT_GraphicsWservAPIServer inline functions
*/


/*@{*/
/*@}*/

inline CT_GraphicsWservAPIServer::CT_GraphicsWservAPIBlock::CT_GraphicsWservAPIBlock()
	{
	}

inline CT_GraphicsWservAPIServer::CT_GraphicsWservAPIBlock::~CT_GraphicsWservAPIBlock()
	{
	}

inline CT_GraphicsWservAPIServer::CT_GraphicsWservAPIServer()
	{
	}

inline CT_GraphicsWservAPIServer::~CT_GraphicsWservAPIServer()
	{
	}

inline CTestBlockController*	CT_GraphicsWservAPIServer::CreateTestBlock()
	{
	return new CT_GraphicsWservAPIBlock();
	}
