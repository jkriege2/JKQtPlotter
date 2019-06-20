import argparse
import os

from fontTools.ttLib import TTFont

def makeWeb(args):
    font = TTFont(args.file)

    base, ext = os.path.splitext(args.file)
    font.flavor = "woff"
    font.save(os.path.join(args.dir, base + ".woff"))
    font.close()


def main():
    parser = argparse.ArgumentParser(description="Create web fonts.")
    parser.add_argument("file", help="input font to process")
    parser.add_argument("dir", help="output directory to write fonts to")

    args = parser.parse_args()

    makeWeb(args)

if __name__ == "__main__":
    main()
