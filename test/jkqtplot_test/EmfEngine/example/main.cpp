#include <QApplication>
#include "renderarea.h"
#include <EmfEngine.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    RenderArea area;

	area.setGeometry(100, 100, 600, 500);
    area.show();

    EmfPaintDevice emf(QSize(600, 500), QString("example.emf"));
	QPainter paint;
	paint.begin(&emf);
	area.draw(&paint);
	paint.end();

    return app.exec();
}
