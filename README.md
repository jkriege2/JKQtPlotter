# JKQtPlotter
This is an extensive library of function/data plotter classes for Qt (>= 4.7, tested with Qt up to 5.11).

This software is licensed under the term of the [GNU Lesser General Public License 2.1 
(LGPL 2.1)](./LICENSE) or above. 

## Continuous Integration Status
[![Build status](https://ci.appveyor.com/api/projects/status/vq2o9pfi97isxm2a/branch/master?svg=true)](https://ci.appveyor.com/project/jkriege2/jkqtplotter/branch/master)

## [Examples](./examples/)
There is a [large set of usage examples (with explanations for each) and tutorials](./examples/) in the folder [`./examples/`](./examples).
All test-projects are Qt-projects that use qmake to build. You can load them into QtCreator easily.

[![EXAMPLES-Page](./screenshots/examplesbanner.png)](./examples/)

## [Screenshots](./screenshots/)
The [Screenshots-page](./screenshots/) contains several screenshots, partly taken from the provided examples, but also from other software using this libarary (e.g. [QuickFit 3.0](https://github.com/jkriege2/QuickFit3))

[![EXAMPLES-Page](./screenshots/screenshotsbanner.png)](./screenshots/)


## Building
Building instructions can be found here:
- include necessary files into QMake project: [`./lib/*.pri`](./lib)
- [build a static library](./staticlib): [`./staticlib/*.pro`](./staticlib)
- [build a shared library (DLL, SO, ...)](./sharedlib): [`./sharedlib/*.pro`](./sharedlib)
