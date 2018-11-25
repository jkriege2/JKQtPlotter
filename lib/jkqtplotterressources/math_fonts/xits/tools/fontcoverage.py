import sys
import unicodedata
from fontTools.ttLib import TTFont

# Unicode blocks file from: http://www.unicode.org/Public/UNIDATA/Blocks.txt

blocksfile = open(sys.argv[1], "r")
blocks = [ ]
for line in blocksfile.readlines():
    if not (line.startswith("#") or line == "\n"):
        start, end   = line.split("; ")[0].split("..")
        name         = line.split("; ")[1].strip()
        blocks.append((name, (start, end)))
blocksfile.close()

logfile = open(sys.argv[2], "r")
log = logfile.read()
logfile.close()

for fontfile in sys.argv[3:-1]:
    font = TTFont(fontfile)
    cmap = font['cmap'].getcmap(3, 10)
    if cmap is None:
        cmap = font['cmap'].getcmap(3, 1)
    found = [ ]

    for block in blocks:
        t = f = 0
        name = block[0]
        start, end = int(block[1][0], 16), int(block[1][1], 16)
        i = start
        while (i <= end):
            category = unicodedata.category(chr(i))
            if category != "Cc" and category!= "Cn":
                if i in cmap.cmap:
                    f += 1
                t += 1
            i += 1
        if f:
            found.append((name, (t,f)))

    fullname = str(font['name'].getName(4, 1, 0).string, encoding='ascii')
    coverage  = ""
    coverage += "* %s:\n" %fullname
    for f in found:
        for b in blocks:
            if b[0] == f[0]:
                name = f[0]
                start, end = b[1]
                total, present = f[1]
                percent = present/total*100
                coverage += "  %s (U+%s-%s): %s/%s (%.2f%%)\n" %(name, start, end, present, total, percent)

    log = log.replace("%%{%s}" %fullname, coverage)

outfile = open(sys.argv[-1], "w")
outfile.write(log)
outfile.close()
