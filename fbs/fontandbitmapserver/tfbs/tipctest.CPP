// Copyright (c) 1996-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "fbsmessage.h"
#include "../sfbs/UTILS.H"
#include "tipctest.h"
#include "fbsdefs.h"


CTIPCTest::CTIPCTest(CTestStep* aStep):
	CTGraphicsBase(aStep),
	iFbs(NULL),
	iFs(NULL)
	{}

CTIPCTest::~CTIPCTest()
	{
	if(iShutdownFbs)
		RFbsSession::GetSession()->SendCommand(EFbsMessShutdown);
	RFbsSession::Disconnect();
	delete iFs;
	iHeap->Close();
	}

void CTIPCTest::ConstructL()
	{
	INFO_PRINTF1(_L("Font/Bitmap Server IPC Messages Testing"));

	if(RFbsSession::Connect()==KErrNone)
		{
		RFbsSession::Disconnect();
		iShutdownFbs = EFalse;
		}
	else
		{
		FbsStartup();
		iShutdownFbs = ETrue;
		}
	RFbsSession::Connect();
	iFbs=RFbsSession::GetSession();
	
	if(!iShutdownFbs)
		{
		if(iClient.Connect() == KErrNone)
			{
			iShutdownFbs = EFalse;
			}
		else
			{
			iShutdownFbs = ETrue;
			}
		}
		
	iHeap=UserHeap::ChunkHeap(NULL,0x10000,0x10000);
	TRAPD(ret,iFs=CFontStore::NewL(iHeap));
	TEST(ret==KErrNone);
	TEST(iFs != NULL);
	
	}

/**
	@SYMTestCaseID
	GRAPHICS-FBSERV-0603

	@SYMTestCaseDesc
	Tests the sending of commands to a FbsSession 
	server thru Inter Process Communication. Sends
	both valid and invalid commands.

	@SYMTestActions
	Commands sent to the server:
	1. EFbsMessShutdown
	2. EFbsMessFontHeightInTwips
	3. EFbsMessFontHeightInTwips
	4. EFbsMessFontHeightInPixels
	5. EFbsMessFontHeightInPixels
	6. EFbsMessAddFontStoreFile
	7. EFbsMessAddFontStoreFile
	8. EFbsMessInstallFontStoreFile
	9. EFbsMessInstallFontStoreFile
	10. EFbsMessInstallFontStoreFile
	11. EFbsMessBitmapCreate
	12. EFbsMessRasterize
	13. EFbsMessFaceAttrib
	14. EFbsMessHasCharacter
	15. EFbsMessFontNameAlias
	16. EFbsMessGetNearestFontToDesignHeightInTwips
	17. EFbsMessGetNearestFontToMaxHeightInTwips
	18. EFbsMessGetNearestFontToDesignHeightInPixels
	19. EFbsMessGetNearestFontToMaxHeightInPixels
	20. EFbsMessShapeText
	21. EFbsMessShapeDelete
	22. EFbsMessSetTwipsHeight
	23. EFbsMessGetTwipsHeight	
	
	@SYMTestExpectedResults
	Test should pass
*/
void CTIPCTest::RunTestCaseL(TInt aCurTestCase)
	{
	_LIT(KCaseNumber, "CaseNumber");
	
	TBool res = iStep->GetIntFromConfig(iStep->ConfigSection(), KCaseNumber, aCurTestCase);
	
	if(!res)
		{
		return;
		}
		
	switch(aCurTestCase)
		{
	case 1:
		INFO_PRINTF1(_L("Shutdown\r\n"));
		iFbs->SendCommand(EFbsMessShutdown);
		TestComplete();
		break;
	case 2:
		INFO_PRINTF1(_L("Height in twips with negative typeface index\r\n"));
		iFbs->SendCommand(EFbsMessFontHeightInTwips, -1);	
		TEST(EFalse); // Previous line should have paniced 
		TestComplete();
		break;	
	case 3:
		INFO_PRINTF1(_L("Height in twips with invalid size\r\n"));
		iClient.SendInvalidSize(EFbsMessFontHeightInTwips);
		TEST(EFalse);
		TestComplete();
		break;
	case 4:
		INFO_PRINTF1(_L("Height in pixels with negative typeface index\r\n"));
		iFbs->SendCommand(EFbsMessFontHeightInPixels, -1);
		TEST(EFalse);
		TestComplete();
		break;
	case 5:
		INFO_PRINTF1(_L("Height in pixels with invalid size\r\n"));
		iClient.SendInvalidSize(EFbsMessFontHeightInPixels);
		TEST(EFalse);
		TestComplete();
		break;
	case 6:
		INFO_PRINTF1(_L("Add font store file with negative length\r\n"));
		iFbs->SendCommand(EFbsMessAddFontStoreFile, 0, -1);
		TEST(EFalse);
		TestComplete();
		break;
	case 7:
		INFO_PRINTF1(_L("Add font store file with huge length\r\n"));
		iFbs->SendCommand(EFbsMessAddFontStoreFile, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 8:
		INFO_PRINTF1(_L("Install font store file with negative length\r\n"));
		iFbs->SendCommand(EFbsMessInstallFontStoreFile, 0, -1);
		TEST(EFalse);
		TestComplete();
		break;
	case 9:
		INFO_PRINTF1(_L("Install font store file with huge length\r\n"));
		iFbs->SendCommand(EFbsMessInstallFontStoreFile, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 10:
		INFO_PRINTF1(_L("Remove font store file with active references\r\n"));
		iFbs->SendCommand(EFbsMessRemoveFontStoreFile, 11);
		// With PDEF121246 (INC120690) fix in place, this test will always pass. Without the fix there will be a KERN 4 panic.
		TEST(ETrue); 
		TestComplete();
		break;
	case 11:
		INFO_PRINTF1(_L("Bitmap create with invalid display mode\r\n"));
		iClient.SendInvalidDisplayMode(EFbsMessBitmapCreate);
		TEST(EFalse);
		TestComplete();
		break;
	case 12:
		INFO_PRINTF1(_L("Rasterize with invalid bitmap handle\r\n"));
		iFbs->SendCommand(EFbsMessRasterize, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 13:
		INFO_PRINTF1(_L("Face Attribute with invalid bitmap handle\r\n"));
		iFbs->SendCommand(EFbsMessFaceAttrib, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 14:
		INFO_PRINTF1(_L("Has Character with invalid bitmap handle\r\n"));
		iFbs->SendCommand(EFbsMessHasCharacter, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 15:
		INFO_PRINTF1(_L("Name Alias with huge alias name length\r\n"));
		iFbs->SendCommand(EFbsMessFontNameAlias, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 16:
		INFO_PRINTF1(_L("Get nearest font to design height in twips with invalid typeface name length\r\n"));
		iClient.SendInvalidNameLength(EFbsMessGetNearestFontToDesignHeightInTwips);
		TEST(EFalse);
		TestComplete();
		break;
	case 17:
		INFO_PRINTF1(_L("Get nearest font to max height in twips with invalid typeface name length\r\n"));
		iClient.SendInvalidNameLength(EFbsMessGetNearestFontToMaxHeightInTwips);
		TEST(EFalse);
		TestComplete();
		break;
	case 18:
		INFO_PRINTF1(_L("Get nearest font to design height in pixels with invalid typeface name length\r\n"));
		iClient.SendInvalidNameLength(EFbsMessGetNearestFontToDesignHeightInPixels);
		TEST(EFalse);
		TestComplete();
		break;
	case 19:
		INFO_PRINTF1(_L("Get nearest font to max height in pixels with invalid typeface name length\r\n"));
		iClient.SendInvalidNameLength(EFbsMessGetNearestFontToMaxHeightInPixels);
		TEST(EFalse);
		TestComplete();
		break;
	case 20:
		INFO_PRINTF1(_L("Shape text with invalid bitmap font handle\r\n"));
		iFbs->SendCommand(EFbsMessShapeText, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 21:
		INFO_PRINTF1(_L("Shape delete with invalid bitmap font handle\r\n"));
		iFbs->SendCommand(EFbsMessShapeDelete, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 22:
		INFO_PRINTF1(_L("Set twips height with invalid font handle\r\n"));
		iFbs->SendCommand(EFbsMessSetTwipsHeight, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;
	case 23:
		INFO_PRINTF1(_L("Get twips height with invalid font handle\r\n"));
		((CTIPCTestStep*)iStep)->SetOverallTestStepID(_L("GRAPHICS-FBSERV-0603"));
		((CTIPCTestStep*)iStep)->RecordTestResultL();
		((CTIPCTestStep*)iStep)->CloseTMSGraphicsStep();
		iFbs->SendCommand(EFbsMessGetTwipsHeight, 963955448, 963955368, 963955300, 963955232);
		TEST(EFalse);
		TestComplete();
		break;	
	case 24:
		TestComplete();
		break;
		}
	}
	
RIPCTestFbservClient::RIPCTestFbservClient()
	{
	
	}
	
RIPCTestFbservClient::~RIPCTestFbservClient()
	{
	
	}
	
TInt RIPCTestFbservClient::Connect(void)
	{
	_LIT(KFbServerName, "!Fontbitmapserver");
	TVersion version(0, 0, 0);
	TInt ret = CreateSession(KFbServerName, version);
	return ret;
	}
	
void RIPCTestFbservClient::SendInvalidSize(TInt aFunctionNumber)
	{
	TPckgBuf<TSize> size;
	size().iHeight = 0;
	size().iWidth = 0;
	TIpcArgs args;
	args.Set(0, 1);
	args.Set(2, &size);
	
	SendReceive(aFunctionNumber, args);
	}
	
void RIPCTestFbservClient::SendInvalidDisplayMode(TInt aFunctionNumber)
	{
	TPckgBuf<TBmpSpec> bmpSpec;
	bmpSpec().iDispMode = EColorLast;
	TIpcArgs args = TIpcArgs();
	args.Set(0, &bmpSpec);
	
	SendReceive(aFunctionNumber, args);
	}

void RIPCTestFbservClient::SendInvalidNameLength(TInt aFunctionNumber)	
	{
	TIpcArgs args = TIpcArgs();
	_LIT(KDefault, "Default");
	TInt size = 7;
	HBufC8 *buf1 = HBufC8::NewLC(size);
	TPtr8 ptr1 = buf1->Des();
	ptr1.Copy(KDefault);
	CleanupStack::PopAndDestroy(buf1);
	args.Set(0, &ptr1);	
	SendReceive(aFunctionNumber, args);
	}
	
	
void CTIPCTest::LoadFontsL(const TDesC& aFontsDir)
	{
	RFs fileSys;
	User::LeaveIfError(fileSys.Connect());

	TFindFile fileFinder(fileSys);
	CDir* foundFileList = NULL;
	
	_LIT(KFBSERVFontFilePattern, "*");
	TInt findFileComplete = fileFinder.FindWildByDir(KFBSERVFontFilePattern,aFontsDir,foundFileList);

	TRAPD(addFileError, LoadFontFromFilelistL(findFileComplete, foundFileList, fileFinder));
	TEST(addFileError == KErrNone);
		
	fileSys.Close();
	}

void CTIPCTest::LoadFontFromFilelistL(TInt aFindFileComplete, CDir* aFoundFileList, TFindFile& aFileFinder)
	{
	TUid id1 = TUid::Uid(0);
	while (!aFindFileComplete)
		{
		CleanupStack::PushL(aFoundFileList);

		const TInt foundFileCount = aFoundFileList->Count();
		for (TInt i = 0; i < foundFileCount; i++)
			{			
			TParse parse;
			if (parse.Set((*aFoundFileList)[i].iName,&aFileFinder.File(),NULL) == KErrNone)
				{
				// Get filename and extension of font proposing to be loaded
				TPtrC fontFilename = parse.NameAndExt();
				id1=iFs->AddFileL(parse.FullName());
				
				iFs->RemoveFile(KNullUid);
				
				}
			}
		CleanupStack::PopAndDestroy(aFoundFileList); 
		aFindFileComplete = aFileFinder.FindWild(aFoundFileList);
		}
	}

//--------------
__CONSTRUCT_STEP__(IPCTest)

