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

#include "gles1cube.h"

_LIT(KGLES1CubeName, "gles1cube");

CEGLRendering* CGLES1Cube::NewL(RWindow& aWindow)
    {
    CGLES1Cube* self = new (ELeave) CGLES1Cube(aWindow);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

const TDesC& CGLES1Cube::Name()
	{
	return KGLES1CubeName;
	}

CGLES1Cube::CGLES1Cube(RWindow& aWindow)
    :   CEGLRendering(aWindow, EGL_OPENGL_ES_API)
    {

    }

CGLES1Cube::~CGLES1Cube()
	{
	RDebug::Printf("[EBT] CGLES1Cube::~CGLES1Cube");
	glDeleteBuffers(1, &iCoordinateColorBuffer);
	glDeleteBuffers(1, &iIndexBuffer);
	}

/*
    v6----- v4
   /|      /|
  v0------v2|
  | |     | |
  | |v7---|-|v5
  |/      |/
  v1------v3

v0: -1.0, +1.0, +1.0,
v1: -1.0, -1.0, +1.0,
v2: +1.0, +1.0, +1.0,
v3: +1.0, -1.0, +1.0,
v4: +1.0, +1.0, -1.0,
v5: +1.0, -1.0, -1.0,
v6: -1.0, +1.0, -1.0,
v7: -1.0, -1.0, -1.0,
*/

static GLfloat CoordinateData[] =
	{
	-1.0, +1.0, +1.0,	// v0
	-1.0, -1.0, +1.0,	// v1
	+1.0, +1.0, +1.0,	// v2
	+1.0, -1.0, +1.0,	// v3
	+1.0, +1.0, -1.0,	// v4
	+1.0, -1.0, -1.0,	// v5
	-1.0, +1.0, -1.0,	// v6
	-1.0, -1.0, -1.0	// v7
	};

static const TInt CoordinateElementCount = 3; // XYZ

static GLfloat ColorData[] =
	{
	 0.0,  1.0,  1.0,  1.0,	// v0
	 0.0,  0.0,  1.0,  1.0,	// v1
	 1.0,  1.0,  1.0,  1.0,	// v2
	 1.0,  0.0,  1.0,  1.0,	// v3
	 1.0,  1.0,  0.0,  1.0,	// v4
	 1.0,  0.0,  0.0,  1.0,	// v5
	 0.0,  1.0,  0.0,  1.0,	// v6
	 0.0,  0.0,  0.0,  1.0	// v7
	};

static const TInt ColorElementCount = 4; // RGBA

static GLubyte FaceData[] =
	{
	0, 1, 3, 2, // front
	2, 3, 5, 4, // right
	4, 5, 7, 6, // left
	6, 7, 1, 0, // back
	6, 0, 2, 4, // top
	1, 7, 5, 3
	};

static const TInt VerticesPerFace = 4;
static const TInt VertexCount = sizeof(FaceData) / sizeof(GLubyte);
static const TInt FaceCount = VertexCount / VerticesPerFace;

static const TInt TrianglesPerFace = 2;
static const TInt VerticesPerTriangle = 3;

static const TInt IndexCount = FaceCount * TrianglesPerFace * VerticesPerTriangle;

static GLubyte FaceIndexData[] =
	{
	0, 2, 1,
	0, 3, 2
	};

void CGLES1Cube::KhrSetup()
    {
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup");

	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup vertexCount %d", VertexCount);
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup faceCount %d", FaceCount);
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup indexCount %d", IndexCount);

	const GLsizeiptr coordinateSize = CoordinateElementCount * sizeof(GLfloat);
	const GLsizeiptr coordinateDataSize = VertexCount * coordinateSize;
	const GLsizeiptr colorSize = ColorElementCount * sizeof(GLfloat);
	const GLsizeiptr colorDataSize = VertexCount * colorSize;

	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glViewport %d x %d", WindowSize().iWidth, WindowSize().iHeight);
	glViewport(0, 0, WindowSize().iWidth, WindowSize().iHeight);
	GLCheckError();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

	// Create coordinate/color buffer
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glGenBuffers vertex/color");
	glGenBuffers(1, &iCoordinateColorBuffer);
	GLCheckError();
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glBindBuffer vertex/color");
	glBindBuffer(GL_ARRAY_BUFFER, iCoordinateColorBuffer);
	GLCheckError();

	// Allocate memory for coordinate/color buffer
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glBufferData coordinate/color");
	glBufferData(GL_ARRAY_BUFFER, coordinateDataSize + colorDataSize, 0, GL_STATIC_DRAW);
	GLCheckError();

	// Upload coordinate/color data
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup supply coordinate/color data");
	GLubyte indexData[IndexCount];
	for (TInt i=0; i<FaceCount; ++i)
		{
		RDebug::Printf("[EBT] CGLES1Cube::KhrSetup face %d", i);
		// Upload coordinate/color data for this face
		for (TInt j=0; j<VerticesPerFace; ++j)
			{
			const TInt vertexIndex = FaceData[i * VerticesPerFace + j];
			const TInt destIndex = (i * VerticesPerFace) + j;
			const GLfloat *coordSrc = CoordinateData + vertexIndex * CoordinateElementCount;
			const TInt coordDest = destIndex * coordinateSize;
			const GLfloat *colorSrc = ColorData + vertexIndex * ColorElementCount;
			const TInt colorDest = coordinateDataSize + destIndex * colorSize;
			RDebug::Printf("[EBT] CGLES1Cube::KhrSetup vertex %d (%d) destIndex %d coordDest %d colorDest %d",
					       j, vertexIndex, destIndex, coordDest, colorDest);
			RDebug::Printf("[EBT] CGLES1Cube::KhrSetup coord %3.1f %3.1f %3.1f",
						   *(coordSrc), *(coordSrc + 1), *(coordSrc + 2));
			RDebug::Printf("[EBT] CGLES1Cube::KhrSetup color %3.1f %3.1f %3.1f",
						   *(colorSrc), *(colorSrc + 1), *(colorSrc + 2));
			glBufferSubData(GL_ARRAY_BUFFER, coordDest, coordinateSize, coordSrc);
			GLCheckError();
			glBufferSubData(GL_ARRAY_BUFFER, colorDest, colorSize, colorSrc);
			GLCheckError();
			}

		// Store vertex indices for this face
		TInt indexDataOffset = i * TrianglesPerFace * VerticesPerTriangle;
		TInt indexBase = i * VerticesPerFace;
		for (TInt j=0; j<TrianglesPerFace * VerticesPerTriangle; ++j)
			{
			RDebug::Printf("[EBT] CGLES1Cube::KhrSetup index %d offset %d value %d",
						   j, indexDataOffset, indexBase + FaceIndexData[j]);
			indexData[indexDataOffset++] = indexBase + FaceIndexData[j];
			}
		}

	// Tell GL engine how to unpack coordinate/color buffer
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glVertexPointer");
	glVertexPointer(CoordinateElementCount, GL_FLOAT, 0, 0);
	GLCheckError();
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glColorPointer");
	glColorPointer(ColorElementCount, GL_FLOAT, 0, reinterpret_cast<GLvoid*>(coordinateDataSize));
	GLCheckError();

	// Create index buffer
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glGenBuffers index");
	glGenBuffers(1, &iIndexBuffer);
	GLCheckError();
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glBindBuffer index");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iIndexBuffer);
	GLCheckError();

	// Allocate memory for index buffer and upload index data
	RDebug::Printf("[EBT] CGLES1Cube::KhrSetup glBufferData index");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount, indexData, GL_STATIC_DRAW);
	GLCheckError();

	glEnableClientState(GL_VERTEX_ARRAY);
	GLCheckError();
	glEnableClientState(GL_COLOR_ARRAY);
	GLCheckError();

	StartRedrawTimer();
    }

void CGLES1Cube::KhrPaint()
	{
	RDebug::Printf("[EBT] CGLES1Cube::KhrPaint");

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(-1.5f, 1.5f, -2.5f, 2.5f, -10.5f, 10.5f);

	glMatrixMode(GL_MODELVIEW);
	iAngle = (iAngle + 1.0f);
	if (iAngle > 360.0f)
		{
		iAngle -= 360.0f;
		}
	glRotatef(iAngle, 0.0f, 1.0f, 0.5f);

	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_BYTE, 0);
    }

