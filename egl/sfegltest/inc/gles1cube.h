// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Accenture - initial contribution.
//
// Contributors:
//
// Description:

#ifndef __GLES1CUBE_H__
#define __GLES1CUBE_H__

#include "eglrendering.h"

class CGLES1Cube : public CEGLRendering
    {
public:
    static CEGLRendering* NewL(RWindow& aWindow);
    ~CGLES1Cube();
    static const TDesC& Name();

private:
    CGLES1Cube(RWindow& aWindow);
    void KhrSetup();
    void KhrPaint();

private:
    TReal iAngle;
    GLuint iCoordinateColorBuffer;
    GLuint iIndexBuffer;
    };

#endif

