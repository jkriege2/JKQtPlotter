#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea : public QWidget
{
public:
    enum Shape { Line, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                 Chord, Pie, Path, Text, Pixmap, TiledPixmap };

    RenderArea(QWidget *parent = 0);
    void draw(QPainter *painter);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif
