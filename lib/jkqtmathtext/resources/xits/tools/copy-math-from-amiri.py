import sys

import fontforge


amiri = fontforge.open(sys.argv[1])
xits = fontforge.open(sys.argv[2])

amiri.em = 1000
amiri.layers[1].is_quadratic = 0
amiri.selection.all()
amiri.unlinkReferences()

names = []
alts = []
for aglyph in amiri.glyphs():
    u = aglyph.unicode
    if (u in range(0x1EE00, 0x1EF00) or
        u in range(0x0660, 0x066E) or
        u in range(0x06F0, 0x06FA) or
        u in range(0x0608, 0x060B) or
        u == 0x060C):
        names.append(aglyph.name)

for aglyph in amiri.glyphs():
    for name in names:
        if aglyph.name != name and aglyph.name.startswith(name + ".alt"):
            alts.append(aglyph.name)

for name in names + alts:
    aglyph = amiri[name]
    if aglyph.name not in xits:
        xits.createChar(aglyph.unicode, aglyph.name)
    xglyph = xits[aglyph.name]
    aglyph.draw(xglyph.glyphPen())
    xglyph.width = aglyph.width
    xglyph.round()
    xglyph.autoHint()

for name in alts:
    base, ext = name.split(".")
    if ext.startswith("alt"):
        xits[base].addPosSub("'cv01' Alternate Arabic Math symbols-1", name)
    elif ext.startswith("display"):
        xits[base].verticalVariants = (xits[base], xits[name])
    else:
        print "Unknown alternate glyph:", name

xits.save()
