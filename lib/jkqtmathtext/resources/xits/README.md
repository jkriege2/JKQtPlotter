[![Build Status](https://travis-ci.com/alif-type/xits.svg?branch=master)](https://travis-ci.com/alif-type/xits)

The XITS font project
======================

XITS is a Times-like typeface for mathematical and scientific publishing, based
on [STIX fonts][1]. The main mission of XITS is to provide a version of STIX
fonts enhanced with the OpenType `MATH` table, making it suitable for high
quality mathematic typesetting with OpenType math-capable layout systems, like
Microsoft Office 2007+, XeTeX and LuaTeX.

XITS fonts are distributed under the [Open Font License][2], version 1.1.

Contributing
------------

The preferred way for modifying the fonts is by editing the SFD files under
`sources` directory in the [source repository][5], using [FontForge][3]. When
submitting patches, please run `make normalize` before committing and avoid
any unrelated or unnecessary changes.

To build the fonts from source you need GNU Make, [FontForge][3] Python module,
and [FontTools][4].

[1]: https://www.stixfonts.org
[2]: https://scripts.sil.org/OFL
[3]: https://fontforge.github.io
[4]: https://github.com/fonttools/fonttools
[5]: https://github.com/alif-type/xits
