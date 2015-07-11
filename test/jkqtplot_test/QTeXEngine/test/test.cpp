#include <QtGui>
#include "QTeXEngine.h"

class PolygonItem : public QGraphicsItem {
   virtual QRectF boundingRect() const { return QRectF(-20, -20, 40, 40); }
   virtual void paint (QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
     painter->setBrush(Qt::blue);
     painter->setPen(Qt::NoPen);
     painter->drawConvexPolygon(QVector<QPointF>() << QPointF(-20, -20) << QPointF(20, 0) << QPointF(-20, 20));
   }
};

class GraphicsView : public QGraphicsView {
  Q_OBJECT
public:
  GraphicsView(){
    QGraphicsScene* s = new QGraphicsScene(this);
    setScene(s);
    setRenderHint(QPainter::Antialiasing);
    translate(0.5, 0.5);
    QGraphicsLineItem* l = new QGraphicsLineItem(10,10, 70, 100);
    QPen lp;
    lp.setDashPattern(QVector<qreal>() << 1.0 << 4.0);
    l->setPen(lp);
    s->addItem(l);

    PolygonItem* p = new PolygonItem();
    p->setPos(60,20);
    s->addItem(p);

    QPixmap pm1("machines.tif");
    QGraphicsPixmapItem* px1 = new QGraphicsPixmapItem(pm1);
    px1->setPos(70, 30);
    s->addItem(px1);

    QPixmap pm2("thermal.tif");
    QGraphicsPixmapItem* px2 = new QGraphicsPixmapItem(pm2);
    px2->setPos(140, 30);
    s->addItem(px2);

  }
};

#include "test.moc"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  GraphicsView view;
  view.show();

  QTeXPaintDevice tex("test.tex", view.scene()->sceneRect().size().toSize());
  tex.setOutputMode(QTeXPaintDevice::Pgf);
  tex.setDocumentMode();

  QPainter painter(&tex);
  view.scene()->render(&painter);
  painter.end();
  return app.exec();
}
