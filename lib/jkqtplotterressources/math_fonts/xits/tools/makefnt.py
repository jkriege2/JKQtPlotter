import argparse
import os

import fontforge
from fontTools.ttLib import TTFont


def postProcess(args):
    font = TTFont(args.output)

    os2 = font["OS/2"]
    # FontForge sets this to ord(' ') for OTF fonts, 0 for TTF fonts, we want
    # to keep things consistent.
    os2.usDefaultChar = 0

    # Filter-out useless Macintosh names
    font["name"].names = [n for n in font["name"].names if n.platformID != 1]

    # https://github.com/fontforge/fontforge/pull/3235
    # fontDirectionHint is deprecated and must be set to 2
    font["head"].fontDirectionHint = 2
    # unset bits 6..10
    font["head"].flags &= ~0x7e0

    # Drop useless table with timestamp
    if "FFTM" in font:
        del font["FFTM"]

    font.save(args.output)


def makeFont(args):
    font = fontforge.open(args.input)

    for glyph in font.glyphs():
        glyph.unlinkRmOvrlpSave = True

    if args.features:
        font.mergeFeature(args.features)

    if os.environ.get("SOURCE_DATE_EPOCH") is None:
        os.environ["SOURCE_DATE_EPOCH"] = "0"

    font.appendSFNTName("English (US)", "UniqueID", "%s;%s;%s" % (args.version,
        font.os2_vendor, font.fontname))

    font.version = args.version
    font.generate(args.output, flags=("round", "opentype"))

    postProcess(args)


def main():
    parser = argparse.ArgumentParser(description="Create web fonts.")
    parser.add_argument("input", help="input font file name")
    parser.add_argument("output", help="output font file name")
    parser.add_argument("--version", help="font version", required=True)
    parser.add_argument("--features", help="font features file name")

    args = parser.parse_args()

    makeFont(args)

if __name__ == "__main__":
    main()
