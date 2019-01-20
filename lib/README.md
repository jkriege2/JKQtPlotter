# JKQTPlotter: LIB subdirectory

This directory contains all files that belong to the JKQTPlotter library (including the files of the JKQTFastPlotter and JKMathText). There are several ways to add these to your program:

### simply include all necessary files
If you use QMake and simply want to include all necessary files into your project, include one of these `PRI`-files into your QMake Project:
- `jkqtplotter.pri` contains all files in this library, including `JKQTFastPlotter` and `JKQTMathText`
- `jkqtfastplotter.pri` contains only those files from this directory which are necessary to build `JKQTFastPlotter`
- `jkqtmathtext.pri` contains only those files from this directory which are necessary to build `JKQTMathText`

### Build static libraries
In the directory [`../staticlib`](../staticlib) you will find several Project files that build a static library of the full JKQTPlotter (including JKQTFastPlotter and JKMathText), or of subsets of this library. You can then link against these libs in your project.

### Build shared libraries (DLLs, SOs, ...)
In the directory [`../sharedlib`](../sharedlib) you will find several Project files that build a dynamic shared library of the full JKQTPlotter (including JKQTFastPlotter and JKMathText), or of subsets of this library. You can then link against these libs in your project.

