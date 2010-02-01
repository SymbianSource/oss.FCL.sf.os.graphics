@rem
@rem Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem

copy ..\release\armv5\urel\t_fbservapi.exe .
copy ..\release\armv5\urel\t_graphicswservapi.exe .
copy ..\release\armv5\urel\t_graphicsbitgdiapi.exe .
copy ..\release\armv5\urel\t_graphicsfntstoreapi.exe .
copy ..\release\armv5\urel\t_animplugin.dll .
copy ..\release\armv5\urel\TWsGraphicShare.exe .
copy ..\release\armv5\urel\TWsGraphicUnshare.exe .

md t_fbservapi
copy ..\data\z\graphics\t_fbservapi\empty_file.txt .\t_fbservapi
copy ..\data\z\graphics\t_fbservapi\testfontfb.gdr .\t_fbservapi
copy ..\data\z\graphics\t_fbservapi\test_bitmap.mbm .\t_fbservapi
copy ..\data\z\graphics\t_fbservapi\test_bitmap_offset8.mbm .\t_fbservapi
copy ..\data\z\graphics\t_fbservapi\scaniline_10bytes.txt .\t_fbservapi
copy ..\data\z\graphics\t_fbservapi\test_bitmap_24bit_400x300.mbm .\t_fbservapi
copy ..\data\z\graphics\t_fbservapi\test_bitmap_32x8.mbm .\t_fbservapi
copy ..\data\z\graphics\t_fbservapi\test_bitmap_rom.rbm .\t_fbservapi

md wserv
copy ..\data\z\graphics\wserv\test_alpha.mbm .\wserv
copy ..\data\z\graphics\wserv\test_board.mbm .\wserv
copy ..\data\z\graphics\wserv\test_mask.mbm .\wserv

md bitgdi
copy ..\data\z\graphics\bitgdi\test_rwssprite_bitmap.mbm .\bitgdi

md fntstore
copy ..\data\z\graphics\fntstore\dummy .\fntstore
copy ..\data\z\graphics\fntstore\dummy_2 .\fntstore
copy ..\data\z\graphics\fntstore\testfontws.gdr .\fntstore

call makesis t_fbservapi.pkg t_fbservapi.sis
call makesis t_wservapi.pkg t_wservapi.sis
call makesis t_bitgdiapi.pkg t_bitgdiapi.sis
call makesis t_fntstoreapi.pkg t_fntstoreapi.sis

call signsis -s t_fbservapi.sis t_fbservapi.sis zeus.cer SymbianACS.key caforstat
call signsis -s t_wservapi.sis t_wservapi.sis zeus.cer SymbianACS.key caforstat
call signsis -s t_bitgdiapi.sis t_bitgdiapi.sis zeus.cer SymbianACS.key caforstat
call signsis -s t_fntstoreapi.sis t_fntstoreapi.sis zeus.cer SymbianACS.key caforstat

