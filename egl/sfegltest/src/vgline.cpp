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

#include "vgline.h"

_LIT(KVGLineName, "vgline");

CEGLRendering* CVGLine::NewL(RWindow& aWindow)
    {
    CVGLine* self = new (ELeave) CVGLine(aWindow);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

const TDesC& CVGLine::Name()
	{
	return KVGLineName;
	}

CVGLine::CVGLine(RWindow& aWindow)
    :   CEGLRendering(aWindow, EGL_OPENVG_API)
	{
    }

void CVGLine::KhrSetup()
    {
    static VGubyte const Segments[] =
        {
        VG_MOVE_TO_ABS,
        VG_LINE_TO_REL,
        VG_CLOSE_PATH
        };

    static VGfloat const Coords[] =
        {
        110, 35,
        50, 160,
        };

    VGfloat strokeColor[4]  = {1.f, 0.f, 0.f, 1.f};

    RDebug::Printf("[EBT] CVGLine::KhrSetup vgCreatePaint");
    iVGPaint = vgCreatePaint();
    VGCheckError();

    RDebug::Printf("[EBT] CVGLine::KhrSetup vgSetParameterX");
    vgSetParameteri(iVGPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
    VGCheckError();
    vgSetParameterfv(iVGPaint, VG_PAINT_COLOR, 4, strokeColor);
    VGCheckError();

    RDebug::Printf("[EBT] CVGLine::KhrSetup vgCreatePath");
    iVGPath = vgCreatePath(VG_PATH_FORMAT_STANDARD,
                            VG_PATH_DATATYPE_F,
                            1.0f, // scale
                            0.0f, // bias
                            3,    // segmentCapacityHint
                            4,    // coordCapacityHint
                            VG_PATH_CAPABILITY_ALL);
    VGCheckError();

    RDebug::Printf("[EBT] CVGLine::KhrSetup vgAppendPathData");
    vgAppendPathData(iVGPath, sizeof(Segments), Segments, Coords);
    VGCheckError();
    }

void CVGLine::KhrPaint()
    {
    RDebug::Printf("[EBT] CVGLine::KhrPaint vgSetPaint");
    vgSetPaint(iVGPaint, VG_STROKE_PATH);
    VGCheckError();

    RDebug::Printf("[EBT] CVGLine::KhrPaint vgDrawPath");
    vgDrawPath(iVGPath, VG_STROKE_PATH);
    VGCheckError();
    }

