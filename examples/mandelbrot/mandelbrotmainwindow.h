#ifndef MANDELBROTMAINWINDOW_H
#define MANDELBROTMAINWINDOW_H

#include <QMainWindow>
#include "jkqtplotter/graphs/jkqtpimage.h"

namespace Ui {
class MandelbrotMainWindow;
}

class MandelbrotMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MandelbrotMainWindow(QWidget *parent = nullptr);
    ~MandelbrotMainWindow();
protected slots:
    void paletteChanged(JKQTPMathImageColorPalette pal);
    void plotZoomChangedLocally(double newxmin, double newxmax, double newymin, double newymax, JKQTPlotter* sender);
    void maxIterationsChanged(int maxIter);
    void logScalingChanged(bool en);
    void plotResized(int new_width, int new_height, JKQTPlotter* sender);
    void resetView();
protected:
    void calculateMandelSet(double rmin, double rmax, double imin, double imax, size_t width, size_t height, unsigned int max_iterations = 1000);
private:
    Ui::MandelbrotMainWindow *ui;
    // graph representing Mandelbrot Set
    JKQTPColumnMathImage* graph;
    // column for Mandelbrot Set image data, used for calculations (double-buffering with mandelbrot_col_display)
    size_t mandelbrot_col;
    // column for Mandelbrot Set image data, used for display (double-buffering with mandelbrot_col)
    size_t mandelbrot_col_display;
};

#endif // MANDELBROTMAINWINDOW_H
