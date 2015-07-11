#!/usr/bin/env python
import sys
from fontTools import ttLib

font = ttLib.TTFont(sys.argv[1])

MS_Platform_ID  = 3
MS_Enc_ID       = 1
MS_Lang_US_ID   = 0x409

FullName_ID = 4

name = font["name"]
cff  = font["CFF "]

psname = cff.cff.fontNames[0]

# set MS full name to psname
# per name table spec
fullname = name.getName(FullName_ID, MS_Platform_ID, MS_Enc_ID, MS_Lang_US_ID)
fullname.string = psname.encode("utf_16_be")

font.save(sys.argv[1] + ".post")
