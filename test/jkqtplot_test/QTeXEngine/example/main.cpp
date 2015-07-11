#include <QApplication>
#include "renderarea.h"
#include <QTeXEngine.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RenderArea area;

	area.setGeometry(100, 100, 600, 500);
    area.show();

    QTeXPaintDevice tex(QString("example.tex"), QSize(600, 500));
    tex.setDocumentMode();
	QPainter paint;
	paint.begin(&tex);
	area.draw(&paint);
	paint.end();

    return app.exec();
}
