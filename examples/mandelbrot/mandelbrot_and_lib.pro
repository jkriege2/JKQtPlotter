TEMPLATE = subdirs

SUBDIRS +=  jkqtplotterlib mandelbrot

jkqtplotterlib.file = ../../qmake/staticlib/jkqtplotterlib/jkqtplotterlib.pro

mandelbrot.file=$$PWD/mandelbrot.pro
mandelbrot.depends = jkqtplotterlib
