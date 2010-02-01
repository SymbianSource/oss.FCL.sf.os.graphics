rem Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
rem All rights reserved.
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem 
rem 
rem

@echo off
set _main_ver=00490
set _main_ver_sub=114869
set _ver=151
set _prefix=GT
set _prefixN=%_prefix%_%_main_ver%
set ZipPrefix=%_prefix%
set _ve32toolp=113
set _ve32tools=123
set _ve32utl=035
set _vgditools=052
set _vsdk=014

set _ve32=189
set _vf32=148
set _vstore=062
set _vgdi=071
set _vfntstore=060
set _vfbserv=093
set _vbitgdi=097
if "%1"=="?" goto help
if "%1"=="" goto help
if "%EPOCROOT%"=="" echo error: Variable Epocroot must be defined
if "%EPOCROOT%"=="" goto help
goto %1
goto help

rem ================ Install ==============

:install
echo Backing up EPOC.INI.
if not exist %EPOCROOT%epoc32\data\epoc.bak copy %EPOCROOT%epoc32\data\epoc.ini %EPOCROOT%epoc32\data\epoc.bak >nul

rem add line to epoc.ini so that the system starter will start the emulator in the specified display mode
echo SYMBIAN_GRAPHICS_USE_GCE ON >> %EPOCROOT%epoc32\data\epoc.ini
echo SYMBIAN_BASE_USE_GCE ON >> %EPOCROOT%epoc32\data\epoc.ini
echo ColorDepth Color16M >> %EPOCROOT%epoc32\data\epoc.ini

echo === New EPOC.INI -- %EPOCROOT%epoc32\data\epoc.ini ===
type %EPOCROOT%epoc32\data\epoc.ini
echo ======================================================

goto :EOF


rem ================ Installonb ==============
:installonb
echo Backing up EPOC.INI.
if not exist %EPOCROOT%epoc32\data\epoc.bak copy %EPOCROOT%epoc32\data\epoc.ini %EPOCROOT%epoc32\data\epoc.bak >nul

rem For ONB system only add line to epoc.ini so that the system starter will start the emulator in the specified display mode
echo. >> %EPOCROOT%epoc32\data\epoc.ini
echo ColorDepth Color16M >> %EPOCROOT%epoc32\data\epoc.ini

echo === New EPOC.INI -- %EPOCROOT%epoc32\data\epoc.ini ===
type %EPOCROOT%epoc32\data\epoc.ini
echo ======================================================

goto :EOF

rem ================ Uninstall ==============

:uninstall
echo Restoring EPOC.INI.
if exist %EPOCROOT%epoc32\data\epoc.bak xcopy /r /y %EPOCROOT%epoc32\data\epoc.bak %EPOCROOT%epoc32\data\epoc.ini >nul
goto :EOF




rem ================ HELP! =================

:help
echo Parameters are:
echo BACKUP    to backup to U:\wserv
echo GETZIP    to get the zip files for the current baseline
echo GETLINE   to extract all the components we depend on
echo GETSRC {comp}  to get the source of another component
rem echo -----------------------------------------------------------------------
echo DELBLD    to delete all work files for the current build
echo CHECK     to check that all the files in the group are in a .PRJ file
echo ARMUDEB   to build unicode debug code for ARM debug
goto end

:endgroup
call cont wserv group
:end
set _main_ver=
set _main_ver_sub=
set _prefix=
set _prefixN=
set _ver=
set _ve32tools=
set _ve32toolp=
set _ve32utl=
set _vgditools=
set _ve32=
set _vf32=
set _vgdi=
set _vfntstore=
set _vfbserv=
set _vbitgdi=
set _vstore=
set _vsdk=
set _par1=
set _par2=
