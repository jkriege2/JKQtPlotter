#include <QObject>
#include <QtTest>
#include "jkqtcommon/jkqtpcsstools.h"
#include "jkqtcommon/jkqtpcachingtools.h"

#ifndef QCOMPARE_EQ
#define QCOMPARE_EQ(A,B) if (!static_cast<bool>((A)==(B))) {qDebug()<<QTest::toString(A)<< "!=" << QTest::toString(B); } QVERIFY((A)==(B))
#endif
#ifndef QVERIFY_THROWS_NO_EXCEPTION
#define QVERIFY_THROWS_NO_EXCEPTION(B) B
#endif
#ifndef QVERIFY_THROWS_EXCEPTION
#define QVERIFY_THROWS_EXCEPTION(type, A) QVERIFY_EXCEPTION_THROWN(A, type)
#endif

namespace QTest {
    template<>
    char *toString(const JKQTPCSSParser::NumberWithUnit &n)
    {
        const QByteArray ba("JKQTPCSSParser::NumberWithUnit(" +
                            QByteArray::number(n.number) + ", '" +
                            n.unit.toLatin1() + "')");
        return qstrdup(ba.data());
    }
}


class JKQTPCSSParserTest : public QObject
{
    Q_OBJECT

public:
    inline JKQTPCSSParserTest() {
    }
    
    inline ~JKQTPCSSParserTest() {
    }

private slots:
    inline void test_JKQTPCSSParser_readNumberWithUnit() {
        JKQTPCSSParser::NumberWithUnit n;
        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readNumberWithUnit("100"));
        QCOMPARE_EQ(n, JKQTPCSSParser::NumberWithUnit(100.0, ""));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readNumberWithUnit("100%"));
        QCOMPARE_EQ(n, JKQTPCSSParser::NumberWithUnit(100.0, "%"));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readNumberWithUnit("100grad"));
        QCOMPARE_EQ(n, JKQTPCSSParser::NumberWithUnit(100.0, "grad"));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readNumberWithUnit("-100grad"));
        QCOMPARE_EQ(n, JKQTPCSSParser::NumberWithUnit(-100.0, "grad"));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readNumberWithUnit("-100.08grad"));
        QCOMPARE_EQ(n, JKQTPCSSParser::NumberWithUnit(-100.08, "grad"));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readNumberWithUnit("+100.08grad"));
        QCOMPARE_EQ(n, JKQTPCSSParser::NumberWithUnit(100.08, "grad"));
    }

    inline void test_JKQTPCSSParser_readColor() {
        QColor n;

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("black"));
        QCOMPARE_EQ(n, QColor(Qt::black));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("red"));
        QCOMPARE_EQ(n, QColor(Qt::red));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("salmon"));
        QCOMPARE_EQ(n, QColor("salmon"));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("transparent"));
        QCOMPARE_EQ(n.alpha(), 0);

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("window"));
        QCOMPARE_EQ(n, QGuiApplication::palette().color(QPalette::Window));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("#F0F"));
        QCOMPARE_EQ(n, QColor::fromRgb(255,0,255));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("#FF00FF"));
        QCOMPARE_EQ(n, QColor::fromRgb(255,0,255));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("#F0F1"));
        QCOMPARE_EQ(n, QColor::fromRgb(0xFF,0,0xFF,0x11));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("#FF00FF11"));
        QCOMPARE_EQ(n, QColor::fromRgb(255,0,255,17));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("rgb(50,100,200)"));
        QCOMPARE_EQ(n, QColor::fromRgb(50,100,200));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("rgb(100%,50,50%)"));
        QCOMPARE(n, QColor::fromRgbF(1,50.0/255.0,0.5));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("rgb(100% 0 50% / 10%)"));
        QCOMPARE_EQ(n, QColor::fromRgbF(1,0,0.5, 0.1));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("rgb(0,0,0)"));
        QCOMPARE_EQ(n, QColor::fromRgb(0,0,0));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("hsv(180deg 127 127)"));
        QCOMPARE_EQ(n, QColor::fromHsv(180,127,127));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("hsv(0.5turn 127 127)"));
        QCOMPARE_EQ(n, QColor::fromHsv(180,127,127));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("hsv(200grad 127 127)"));
        QCOMPARE_EQ(n, QColor::fromHsv(180,127,127));

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readColor("hsv(180 127 127)"));
        QCOMPARE_EQ(n, QColor::fromHsv(180,127,127));

    }

    inline void test_JKQTPCSSParser_readGradient() {
        QGradient n, g;
        QLinearGradient lg;

        QVERIFY_THROWS_EXCEPTION(std::exception, n=JKQTPCSSParser::readGradient("wa__flame"));

#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readGradient("warmflame"));
        g = QGradient(QGradient::WarmFlame);
        g.setCoordinateMode(QGradient::ObjectBoundingMode);
        QCOMPARE_EQ(n, g);
#endif

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readGradient("linear-gradient(to left, red, blue)"));
        lg = QLinearGradient(1,0.5,0,0.5);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor("red")), QGradientStop(1, QColor("blue"))});
        QCOMPARE_EQ(n, lg);

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readGradient("linear-gradient(to right, red, green 20%, blue)"));
        lg = QLinearGradient(0,0.5,1,0.5);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor("red")), QGradientStop(0.2, QColor("green")), QGradientStop(1, QColor("blue"))});
        QCOMPARE_EQ(n, lg);

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readGradient("linear-gradient(to top, red, blue)"));
        lg = QLinearGradient(0.5,1,0.5,0);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor("red")), QGradientStop(1, QColor("blue"))});
        QCOMPARE_EQ(n, lg);

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readGradient("linear-gradient(to bottom, red, blue)"));
        lg = QLinearGradient(0.5,0,0.5,1);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor("red")), QGradientStop(1, QColor("blue"))});
        QCOMPARE_EQ(n, lg);

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readGradient("linear-gradient(to bottom, rgb(255,0,0), rgb(0,0,255))"));
        lg = QLinearGradient(0.5,0,0.5,1);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor("red")), QGradientStop(1, QColor("blue"))});
        QCOMPARE_EQ(n, lg);

        QVERIFY_THROWS_NO_EXCEPTION(n=JKQTPCSSParser::readGradient("linear-gradient(to bottom, rgb(100%,0,0/90%), rgb(0,0,100%/10%))"));
        lg = QLinearGradient(0.5,0,0.5,1);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor::fromRgbF(1,0,0,0.9)), QGradientStop(1, QColor::fromRgbF(0,0,1,0.1))});
        QCOMPARE_EQ(n, lg);


    }

    inline void test_JKQTPCSSParser_jkqtp_String2QBrushStyleExt() {
        Qt::BrushStyle bs;
        QGradient n, g;
        QLinearGradient lg;


#if (QT_VERSION >= QT_VERSION_CHECK(5, 12, 0))
        QVERIFY_THROWS_NO_EXCEPTION(bs=jkqtp_String2QBrushStyleExt("warmflame", &n, nullptr));
        g = QGradient(QGradient::WarmFlame);
        g.setCoordinateMode(QGradient::ObjectBoundingMode);
        QCOMPARE_EQ(n, g);
        QCOMPARE_EQ(bs, Qt::LinearGradientPattern);
#endif

        QVERIFY_THROWS_NO_EXCEPTION(bs=jkqtp_String2QBrushStyleExt("d1", &n, nullptr));
        QCOMPARE_EQ(bs, Qt::Dense1Pattern);


        QVERIFY_THROWS_NO_EXCEPTION(bs=jkqtp_String2QBrushStyleExt("linear-gradient(to left, red, blue)", &n, nullptr));
        lg = QLinearGradient(1,0.5,0,0.5);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor("red")), QGradientStop(1, QColor("blue"))});
        QCOMPARE_EQ(n, lg);
        QCOMPARE_EQ(bs, Qt::LinearGradientPattern);

    }

    inline void test_JKQTPCSSParser_jkqtp_String2QColor() {
        QColor n;

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("black"));
        QCOMPARE_EQ(n, QColor(Qt::black));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("red"));
        QCOMPARE_EQ(n, QColor(Qt::red));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("salmon"));
        QCOMPARE_EQ(n, QColor("salmon"));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("transparent"));
        QCOMPARE_EQ(n.alpha(), 0);

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("window"));
        QCOMPARE_EQ(n, QGuiApplication::palette().color(QPalette::Window));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("#F0F"));
        QCOMPARE_EQ(n, QColor::fromRgb(255,0,255));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("#FF00FF"));
        QCOMPARE_EQ(n, QColor::fromRgb(255,0,255));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("#F0F1"));
        QCOMPARE_EQ(n, QColor::fromRgb(0xFF,0,0xFF,0x11));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("#FF00FF11"));
        QCOMPARE_EQ(n, QColor::fromRgb(255,0,255,17));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("rgb(50,100,200)"));
        QCOMPARE_EQ(n, QColor::fromRgb(50,100,200));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("grey50"));
        QCOMPARE_EQ(n, QColor::fromRgbF(0.5,0.5,0.5));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("grey52"));
        QCOMPARE_EQ(n, QColor::fromRgbF(0.52,0.52,0.52));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("grey52,20%"));
        QCOMPARE_EQ(n, QColor::fromRgbF(0.52,0.52,0.52,0.8));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("grey52,a20%"));
        QCOMPARE_EQ(n, QColor::fromRgbF(0.52,0.52,0.52,0.2));

        QVERIFY_THROWS_NO_EXCEPTION(n=jkqtp_String2QColor("grey52,a240"));
        QCOMPARE_EQ(n, QColor::fromRgbF(0.52,0.52,0.52,240.0/255.0));
    }


};


QTEST_APPLESS_MAIN(JKQTPCSSParserTest)

#include "JKQTPCSSParser_test.moc"
