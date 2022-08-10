# Example (JKQTMathText): Command-Line Utility jkqtmathtext_render           {#JKQTMathTextRenderCmdLineTool}
JKQTMathText is a hand-written LaTeX-renderer for Qt (implemented in native C++, using Qt). It supports a large set of standard LaTeX markup and can render it to a QPainter.

This project (see `./examples/jkqtmathtext_render/`) is a command-line utility that accepts a LaTeX markup string and a filename for the generated image.
It then renders the string into the image.
The source code of the main application can be found in [`jkqtmathtext_render.cpp`](https://github.com/jkriege2/JKQtPlotter/tree/master/examples/jkqtmathtext_render/jkqtmathtext_render.cpp). 
The major rendering portion look like this:

First we generate dummy QPixmap that is needed to use the QPainter, that is required for determining the size of the rendering.

```.cpp
    QPixmap pix(10,10);
```

Now we create a JKQTMathText object and configure it

```.cpp
    QPainter painter;
    JKQTMathText mathText;
    if (useXITS) mathText.useXITS();
    mathText.setFontSize(fontsize);
```

Now we parse some LaTeX code and thus generate its memory representation.

```.cpp
    mathText.parse(latex);
```


Finally we can generate a QImage with the output of the rendering algorithm and save it as a file.

```.cpp
    const QImage pix=mathText.drawIntoImage(drawBoxes, backgroundColor, sizeincrease);
    pix.save(outputFilename);
```


calling this utility with the LaTeX code:

```.sh
> jkqtmathtext_render "$x_{1/2}=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$" jkqtmathtext_render_output.png
```

results in this output:

![jkqtmathtext_render_output](https://raw.githubusercontent.com/jkriege2/JKQtPlotter/master/screenshots/jkqtmathtext_render_output.png)

The tool supports these command-line options:
- *command-line mode*: call `jkqtmathtext_render LATEX OUTPUTFILE`
    - The extension of the `OUTPUTFILE` determines the file type. The tool supports `.png`, `.bmp`, `.jpg`, `.ppm`, `.xbm` and `.xpm`
- *file-mode*:  call `jkqtmathtext_render --inputfile=INPUTFILE.jkmt --outputdir=OUTPUTDIR`
    - The file `INPUTFILE.jkmt` is a text file with several "render jobs", deparated by `---` lines.
    - The first line in each job defines the output filename (relative to `OUTPUTDIR`)
    - The second line is optional and contains a list of altered command-line options, e.g. `--fontsize=24 --fontmathroman=XITS`, Note however that only options concerning formatting are  allowed, `--verbose` or the file/directory-options will not be processed!
    - The third and further lines is concatenated to form the LaTeX markup to be rendered.
- All modes support these command-line options:
    - `--verbose`: verbose output of the tool
    - `--sizeincrease=SIZE_PIXELS`: set the width of the additional margin around the rendering result 
    - `--drawboxes`: flag that enables drawing of rectangles around each box
    - `--font=FONT_SERIF`: sets text- and math-mode serif font to `FONT_SERIF`
    - `--font=FONT_SERIF,FONT_SANS`: sets text- and math-mode serif font to `FONT_SERIF` and sans font to `FONT_SANS`
    - `--font=FONT_TEXT_SERIF,FONT_TEXT_SANS,FONT_MATH_SERIF,FONT_MATH_SANS`: sets text- and math-mode serif font to `FONT_TEXT_SERIF` and `FONT_MATH_SERIF` and sans fonts to `FONT_TEXT_SANS` and `FONT_MATH_SANS`
    - `--font=...+XITS|STIX|ASANA`: set fonts as given above and then use XITS-/STIX- or ASANA-fonts for math-roman 
    - `--fontsize=SIZE_PT`: set the font-size in pt
    - `--fontroman=FONT`: set the text-mode roman font 
    - `--fontsans=FONT`: set the text-mode sans font 
    - `--fontmathroman=FONT`: set the math-mode roman font 
    - `--fontmathsans=FONT`: set the math-mode sans font 
    - `--fontblackboard=FONT`: use the given font as blackboard-font and de-activate the simulate-feature 
    - `--fontblackboardmode=default|font_directly|simulate|unicode_or_font_directly|unicode_or_simulate`: use the given drawing mode for blackboard fonts
    - `--fonttypewriter=FONT`: set the typewriter font 
    - `--fontscript=FONT`: set the script font 
    - `--fontcaligraphic=FONT`: set the caligraphic font 
    - `--fontfraktur=FONT`: set the fraktur font 
    - `--fontfallbacksymbol=FONT`: set the fallback symbol font, using unicode encoding 
    - `--fontfallbacksymbol_symbolencoding=FONT`: set the fallback symbol font, using WinSymbol encoding 
    - `--background=COLOR`: set the background color of the output image
    - `--textcolor=COLOR`: set the text color of the output image

