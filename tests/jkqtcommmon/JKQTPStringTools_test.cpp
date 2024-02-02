#include <QObject>
#include <QtTest>
#include "jkqtcommon/jkqtpstringtools.h"

#ifndef QCOMPARE_EQ
#define QCOMPARE_EQ(A,B) if (!static_cast<bool>((A)==(B))) {qDebug()<<QTest::toString(A)<< "!=" << QTest::toString(B); } QVERIFY((A)==(B))
#endif
#ifndef QVERIFY_THROWS_NO_EXCEPTION
#define QVERIFY_THROWS_NO_EXCEPTION(B) B
#endif
#ifndef QVERIFY_THROWS_EXCEPTION
#define QVERIFY_THROWS_EXCEPTION(type, A) QVERIFY_EXCEPTION_THROWN(A, type)
#endif



class JKQTPStringToolsTest : public QObject
{
    Q_OBJECT

public:
    inline JKQTPStringToolsTest() {
    }
    
    inline ~JKQTPStringToolsTest() {
    }

private slots:


    inline void test_jkqtp_String2QBrushStyleExt() {
        Qt::BrushStyle bs;
        QGradient n, g;
        QLinearGradient lg;


        QVERIFY_THROWS_NO_EXCEPTION(bs=jkqtp_String2QBrushStyleExt("warmflame", &n, nullptr));
        g = QGradient(QGradient::WarmFlame);
        g.setCoordinateMode(QGradient::ObjectBoundingMode);
        QCOMPARE_EQ(n, g);
        QCOMPARE_EQ(bs, Qt::LinearGradientPattern);

        QVERIFY_THROWS_NO_EXCEPTION(bs=jkqtp_String2QBrushStyleExt("d1", &n, nullptr));
        QCOMPARE_EQ(bs, Qt::Dense1Pattern);


        QVERIFY_THROWS_NO_EXCEPTION(bs=jkqtp_String2QBrushStyleExt("linear-gradient(to left, red, blue)", &n, nullptr));
        lg = QLinearGradient(1,0.5,0,0.5);
        lg.setCoordinateMode(QGradient::ObjectBoundingMode);
        lg.setStops({QGradientStop(0, QColor("red")), QGradientStop(1, QColor("blue"))});
        QCOMPARE_EQ(n, lg);
        QCOMPARE_EQ(bs, Qt::LinearGradientPattern);

    }

    inline void test_jkqtp_String2QColor() {
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

    inline void test_jkqtp_floattounitstr() {
        QCOMPARE_EQ(jkqtp_floattounitstr(0, 1, true), "0");
        QCOMPARE_EQ(jkqtp_floattounitstr(0, 1, false), "0");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 1, true), "1");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 1, false), "1.0");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.2e3, 1, false), "1.2k");
        QCOMPARE_EQ(jkqtp_floattounitstr(-1.2e6, 1, false), "-1.2M");
        QCOMPARE_EQ(jkqtp_floattounitstr(-1.2e30, 1, false), "-1.2Q");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.2e30, 1, false), "1.2Q");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.2e-3, 1, false), "1.2m");
        QCOMPARE_EQ(jkqtp_floattounitstr(-1.2e-6, 1, false), "-1.2\xB5");
        QCOMPARE_EQ(jkqtp_floattounitstr(-1.2e-30, 1, false), "-1.2q");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.2e-30, 1, false), "1.2q");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.234e-30, 1, false), "1.2q");
    }

    inline void test_jkqtp_floattolatexunitstr() {
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(0, 1, true), "0");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(0, 1, false), "0");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 1, true), "1");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 1, false), "1.0");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.2e3, 1, false), "1.2\\;\\mathrm{k}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-1.2e6, 1, false), "-1.2\\;\\mathrm{M}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-1.2e30, 1, false), "-1.2\\;\\mathrm{Q}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.2e30, 1, false), "1.2\\;\\mathrm{Q}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.2e-3, 1, false), "1.2\\;\\mathrm{m}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-1.2e-6, 1, false), "-1.2\\;\\mathrm{\\mu}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-1.2e-30, 1, false), "-1.2\\;\\mathrm{q}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.2e-30, 1, false), "1.2\\;\\mathrm{q}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.234e-30, 1, false), "1.2\\;\\mathrm{q}");
    }

};


QTEST_APPLESS_MAIN(JKQTPStringToolsTest)

#include "JKQTPStringTools_test.moc"
