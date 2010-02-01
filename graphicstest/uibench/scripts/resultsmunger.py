# Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
# # Scans the working directory for files and processes them
# # assuming they are the output from the te_uibench component
# # Outputs a results.csv file that contains:-
# # filename, testid1, result1, testid2, result2 etc
# 
#

import os
import dircache
import re

out_file = open("results.csv","w")

files=dircache.listdir(os.getcwd())

bitbltfinder = re.compile('.* ((.?)BitBlt(.?)) o: (.*), (.+)bit->(.*) mode - for (.*) bitmaps, per bitmap=(.*) .*')
bitmapcreatefinder = re.compile('.* Bitmap->Create - for (.*) bitmaps, per bitmap=(.*) .*')
bitmapdupfinder = re.compile('.* Bitmap->Duplicate - for (.*) bitmaps, per bitmap (.*) .*')
fontdupfinder = re.compile('.* Font->Duplicate - for (.*) fonts, per font (.*) .*')
solidcolourfinder = re.compile('.* (Rect->Fill) o: (.*), (.*) mode - colour: (.*) - per fill (.*) .*')

count = 0
while count < len(files):
    print 'files[',count,']=',files[count]

    if files[count] != "results.csv":
        
        out_file.write(files[count]+", ")
        
        in_file = open(files[count],"r")
        text = " "
        while text!="":
            text = in_file.readline()

            bcresult = bitmapcreatefinder.match(text)
            if bcresult:
                print "BitmapCreate "+bcresult.group(2)
                out_file.write("BitmapCreate, ")
                out_file.write(bcresult.group(2)+", ")
            
            bdresult = bitmapdupfinder.match(text)
            if bdresult:
                print "BitmapDuplicate "+bdresult.group(2)
                out_file.write("BitmapDuplicate, ")
                out_file.write(bdresult.group(2)+", ")
            
            fdresult = fontdupfinder.match(text)
            if fdresult:
                print "FontDuplicate "+fdresult.group(2)
                out_file.write("FontDuplicate, ")
                out_file.write(fdresult.group(2)+", ")

            scresult = solidcolourfinder.match(text)
            if scresult:
                test = scresult.group(1)+"-"+scresult.group(2)+"o-"+scresult.group(3)+"m"+scresult.group(4)+"c"
                print "Solid Colour "+scresult.group(5)
                out_file.write(test+", ")
                out_file.write(scresult.group(5)+", ")
            
            result = bitbltfinder.match(text)
            if result:
                test = result.group(1)+"-"+result.group(4)+"o-"+result.group(5)+"bit-"+result.group(6)+"m"
                print test
                print result.group(8)
                out_file.write(test+", ")
                out_file.write(result.group(8)+", ")
        in_file.close()
        out_file.write("\n")
        
    count = count + 1

out_file.close()    