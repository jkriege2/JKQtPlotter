# Fira Math

[![Build Status](https://github.com/firamath/firamath/workflows/build/badge.svg)](https://github.com/firamath/firamath/actions/workflows/main.yml)
[![CTAN](https://img.shields.io/ctan/v/firamath.svg)](https://www.ctan.org/pkg/firamath)
[![GitHub release](https://img.shields.io/github/release/firamath/firamath/all.svg)](https://github.com/firamath/firamath/releases/latest)

Fira Math is a sans-serif font with Unicode math support. This font is a fork of

- [FiraSans](https://github.com/bBoxType/FiraSans)
- [FiraGO](https://github.com/bBoxType/FiraGO)

## Showcase

<p align="center">
  <img src="docs/svg/firamath-demo-2.svg" alt="firamath-demo-2" width=49%>
  <img src="docs/svg/firamath-demo-3.svg" alt="firamath-demo-3" width=49%>
  <img src="docs/svg/firamath-demo-4.svg" alt="firamath-demo-4" width=49%>
  <img src="docs/svg/firamath-demo-5.svg" alt="firamath-demo-5" width=49%>
</p>

## Installation

### On your system

Download `FiraMath-Regular.otf` from the [GitHub release](https://github.com/firamath/firamath/releases) page, then:

- Windows
  1. Quit related programs, e.g. Microsoft Word.
  1. Double-click the font file to open it in the Font Previewer.
  1. Click `Install` bottom to install.
  1. You can also directly copy all the `.otf` font files into `%WINDOWS%\Fonts\` to install them.
- macOS
  1. Double-click on the font files in Finder.
  1. It will open in the font previewer. Click Install Font. It will open in the Font Book.
  1. Close the Font Book. The fonts should now be available.
  1. Details can be found in [this article](http://support.apple.com/kb/HT2509).
- Linux
  1. Copy the `.otf` font files to your fonts directory.
  1. Run `sudo fc-cache`.

### On the TeX distribution

- If there are no special reasons, you should always install Fira Math via the package manager:

  - TeX Live: run `tlmgr install firamath`
  - MiKTeX: run `mpm --admin --install=firamath`

  Both distributions have GUI interfaces, you may enjoy them as well.

- **[NOT RECOMMENDED]** Download `firamath.tds.zip` from [CTAN](https://ctan.org/pkg/firamath) or [GitHub release page](https://github.com/firamath/firamath/releases), extract it in the root of one of your TDS trees, then run `mktexlsr` and `fc-cache` to update the filename database and font cache.

## Usage

Fira Math can be used in LaTeX, ConTeXt and Microsoft Word after installed on your OS (or the TeX distribution).

### LaTeX

```latex
% Compiled with XeLaTeX or LuaLaTeX
\documentclass{article}
\usepackage{amsmath}
\usepackage[mathrm=sym]{unicode-math}
\setmathfont{Fira Math}

\begin{document}
\[
  \int_0^{\mathrm{\pi}} \sin x \, \mathrm{d}x = 2
\]
\end{document}
```

You may try the [`firamath-otf`](https://ctan.org/pkg/firamath-otf) package as well. - Thanks [Herbert Voß](https://github.com/hvoss49)!

### ConTeXt

```tex
% Compiled with ConTeXt MkIV
\definefontfamily [mainface] [rm] [FiraGO]  % Require FiraGO fonts
\definefontfamily [mainface] [mm] [Fira Math]
\setupbodyfont    [mainface]

\starttext
\startformula
  \int_0^{\mathrm{\pi}} \sin x \, \mathrm{d}x = 2
\stopformula
\stoptext
```

See <https://tex.stackexchange.com/q/429621>.

### Microsoft Word

1. Create a new equation. Then select the little *additional settings* corner.
1. In the menu, change the *Default font* to Fira Math.
1. In order for the changes to take effect, you will have to create a new equation environment (the current one will not be changed).
1. To change the font in existing equations, select the equation text and select an OpenType math-enabled font (such as Fira Math).

See <https://superuser.com/q/1114697>.

## Building from source

To build Fira Math, you may

```sh
python -m pip install -U pip
pip install -r requirements.txt
patch $(python -c "import os, glyphsLib; print(os.path.join(glyphsLib.__path__[0], 'builder', 'axes.py'))") scripts/axes.py.diff
python scripts/build.py
```

Note that Python 3.9+ is required. Since we are using [the dev version of glyphsLib](https://github.com/googlefonts/glyphsLib/pull/652), it's better to use a Python virtual environment.

To edit the source files, [Glyphs 3](https://glyphsapp.com/) is required.

### Development builds

If you just want to download the latest development version, please try the artifacts in [GitHub Actions](https://github.com/firamath/firamath/actions).

## Contributing

[Issues](https://github.com/firamath/firamath/issues) and [pull requests](https://github.com/firamath/firamath/pulls) are always welcome.

## License

This Font Software is licensed under the [SIL Open Font License](http://scripts.sil.org/OFL), Version 1.1.

-----

Copyright (C) 2018&ndash;2021 by Xiangdong Zeng.
