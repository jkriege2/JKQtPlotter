import sys
try:
    from sortsmill import ffcompat as fontforge
except ImportError:
    import fontforge

font = fontforge.open(sys.argv[1])

if len(sys.argv) > 4:
  font.mergeFeature(sys.argv[4])

font.version = sys.argv[3]
font.generate(sys.argv[2], flags=("round", "opentype"))
