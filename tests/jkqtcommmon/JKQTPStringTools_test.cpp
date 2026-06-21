#include <QObject>
#include <QtTest>
#include "jkqtcommon/jkqtpstringtools.h"
#include <cmath>

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

    // ===== ORIGINAL TEST CASES =====
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

    // ===== EDGE CASE TESTS =====
    inline void test_jkqtp_floattounitstr_boundary_values() {
        // Test boundary between units (just below and just above thresholds)
        // 999 should not get a unit, 1000 should be "1k"
        QCOMPARE_EQ(jkqtp_floattounitstr(999, 1, false), "999.0");
        QCOMPARE_EQ(jkqtp_floattounitstr(1000, 1, false), "1.0k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1001, 1, false), "1.0k");

        // Test boundary for micro
        QCOMPARE_EQ(jkqtp_floattounitstr(0.001, 1, false), "1.0m");
        QCOMPARE_EQ(jkqtp_floattounitstr(0.0009, 1, false), "900.0\xB5");
        QCOMPARE_EQ(jkqtp_floattounitstr(0.0001, 1, false), "100.0\xB5");

        // Test boundary for nano
        QVERIFY((jkqtp_floattounitstr(1e-6, 1, false)== "1.0\xB5") || (jkqtp_floattounitstr(1e-6, 1, false)== "1000.0n"));
        QCOMPARE_EQ(jkqtp_floattounitstr(1e-9, 1, false), "1.0n");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e-8, 1, false), "10.0n");
    }

    inline void test_jkqtp_floattounitstr_negative_values() {
        // Ensure negative values work correctly
        QCOMPARE_EQ(jkqtp_floattounitstr(-1.0, 1, true), "-1");
        QCOMPARE_EQ(jkqtp_floattounitstr(-1.0, 1, false), "-1.0");
        QCOMPARE_EQ(jkqtp_floattounitstr(-999, 1, false), "-999.0");
        QCOMPARE_EQ(jkqtp_floattounitstr(-1000, 1, false), "-1.0k");
        QCOMPARE_EQ(jkqtp_floattounitstr(-1.2e12, 1, false), "-1.2T");
        QCOMPARE_EQ(jkqtp_floattounitstr(-5.5e-9, 1, false), "-5.5n");
    }

    inline void test_jkqtp_floattounitstr_precision_variations() {
        // Test different past_comma values
        QCOMPARE_EQ(jkqtp_floattounitstr(1.23456e3, 0, false), "1k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.23456e3, 2, false), "1.23k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.23456e3, 3, false), "1.235k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.23456e3, 5, false), "1.23456k");

        // Test with remove_trail0=true
        QCOMPARE_EQ(jkqtp_floattounitstr(1.20000e3, 5, true), "1.2k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.00000e3, 5, true), "1k");
        QCOMPARE_EQ(jkqtp_floattounitstr(5.50000e-9, 5, true), "5.5n");
    }

    inline void test_jkqtp_floattounitstr_small_values() {
        // Test values near zero and very small values
        QCOMPARE_EQ(jkqtp_floattounitstr(0.0, 1, true), "0");
        QCOMPARE_EQ(jkqtp_floattounitstr(0.0, 1, false), "0");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e-30, 1, false), "1.0q");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e-27, 1, false), "1.0r");
    }

    inline void test_jkqtp_floattounitstr_large_values() {
        // Test large values across all SI units
        QCOMPARE_EQ(jkqtp_floattounitstr(1e3, 1, false), "1.0k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e6, 1, false), "1.0M");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e9, 1, false), "1.0G");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e12, 1, false), "1.0T");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e15, 1, false), "1.0P");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e18, 1, false), "1.0E");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e21, 1, false), "1.0Z");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e24, 1, false), "1.0Y");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e27, 1, false), "1.0R");
        QCOMPARE_EQ(jkqtp_floattounitstr(1e30, 1, false), "1.0Q");
    }

    inline void test_jkqtp_floattounitstr_no_unit_range() {
        // Values that should not receive a unit (between 1 and 999, or between 0.001 and 0.999)
        QCOMPARE_EQ(jkqtp_floattounitstr(1, 1, true), "1");
        QCOMPARE_EQ(jkqtp_floattounitstr(10, 1, true), "10");
        QCOMPARE_EQ(jkqtp_floattounitstr(100, 1, true), "100");
        QCOMPARE_EQ(jkqtp_floattounitstr(500, 1, true), "500");
        QCOMPARE_EQ(jkqtp_floattounitstr(999, 1, true), "999");
    }

    inline void test_jkqtp_floattounitstr_transition_points() {
        // Test exact transitions between unit scales
        // At 1000, should switch from no unit to "k"
        QCOMPARE_EQ(jkqtp_floattounitstr(999.9, 1, false), "999.9");
        QCOMPARE_EQ(jkqtp_floattounitstr(1000.0, 1, false), "1.0k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1000.1, 1, false), "1.0k");

        // Verify all major transitions work
        QCOMPARE_EQ(jkqtp_floattounitstr(999e3, 1, false), "999.0k");
        QCOMPARE_EQ(jkqtp_floattounitstr(1000e3, 1, false), "1.0M");

        QCOMPARE_EQ(jkqtp_floattounitstr(999e6, 1, false), "999.0M");
        QCOMPARE_EQ(jkqtp_floattounitstr(1000e6, 1, false), "1.0G");
    }

    inline void test_jkqtp_floattounitstr_belowIsZero() {
        // Test belowIsZero parameter
        QCOMPARE_EQ(jkqtp_floattounitstr(1e-100, 1, false, 1e-50), "0");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 1, false, 0.5), "1.0");
        QCOMPARE_EQ(jkqtp_floattounitstr(0.3, 1, false, 0.5), "0");
    }

    // ===== LATEX UNIT STRING EDGE CASES =====
    inline void test_jkqtp_floattolatexunitstr_boundary_values() {
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(999, 1, false), "999.0");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1000, 1, false), "1.0\\;\\mathrm{k}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(0.001, 1, false), "1.0\\;\\mathrm{m}");
        QVERIFY((jkqtp_floattolatexunitstr(1e-6, 1, false) == "1.0\\;\\mathrm{\\mu}") || (jkqtp_floattolatexunitstr(1e-6, 1, false) == "1000.0\\;\\mathrm{n}"));
    }

    inline void test_jkqtp_floattolatexunitstr_negative_values() {
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-1.0, 1, true), "-1");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-1.2e3, 1, false), "-1.2\\;\\mathrm{k}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-1.2e-9, 1, false), "-1.2\\;\\mathrm{n}");
    }

    inline void test_jkqtp_floattolatexunitstr_precision_variations() {
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.23456e3, 0, false), "1\\;\\mathrm{k}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.23456e3, 2, false), "1.23\\;\\mathrm{k}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.23456e3, 5, true), "1.23456\\;\\mathrm{k}");

        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.20000e3, 5, true), "1.2\\;\\mathrm{k}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.00000e3, 5, true), "1\\;\\mathrm{k}");
    }

    inline void test_jkqtp_floattolatexunitstr_all_units() {
        // Test all SI units appear correctly in LaTeX format
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e3, 1, false), "1.0\\;\\mathrm{k}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e6, 1, false), "1.0\\;\\mathrm{M}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e9, 1, false), "1.0\\;\\mathrm{G}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e12, 1, false), "1.0\\;\\mathrm{T}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e15, 1, false), "1.0\\;\\mathrm{P}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e18, 1, false), "1.0\\;\\mathrm{E}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e21, 1, false), "1.0\\;\\mathrm{Z}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e24, 1, false), "1.0\\;\\mathrm{Y}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e27, 1, false), "1.0\\;\\mathrm{R}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e30, 1, false), "1.0\\;\\mathrm{Q}");

        // Negative exponents
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e-3, 1, false), "1.0\\;\\mathrm{m}");
        QVERIFY((jkqtp_floattolatexunitstr(1e-6, 1, false) == "1.0\\;\\mathrm{\\mu}") || (jkqtp_floattolatexunitstr(1e-6, 1, false) == "1000.0\\;\\mathrm{n}"));
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e-9, 1, false), "1.0\\;\\mathrm{n}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e-12, 1, false), "1.0\\;\\mathrm{p}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e-15, 1, false), "1.0\\;\\mathrm{f}");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e-18, 1, false), "1.0\\;\\mathrm{a}");

    }

    // ===== PLATFORM-SPECIFIC REGRESSION TESTS (i386 issue) =====
    inline void test_jkqtp_floattounitstr_i386_regression() {
        // These are the exact test cases that failed on i386
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 1, true), "1");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 1, false), "1.0");

        // Additional i386 regression checks with various precisions
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 2, true), "1");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 3, true), "1");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 5, true), "1");
    }

    inline void test_jkqtp_floattolatexunitstr_i386_regression() {
        // These are the exact test cases that failed on i386
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 1, true), "1");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 1, false), "1.0");

        // Additional i386 regression checks
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 2, true), "1");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 3, true), "1");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 5, true), "1");
    }

    // ===== SPECIAL FLOATING POINT VALUES =====
    inline void test_jkqtp_floattounitstr_special_values() {
        // Test NaN handling (should return something reasonable)
        double nan_val = std::nan("");
        // We just verify it doesn't crash
        std::string result_nan = jkqtp_floattounitstr(nan_val, 1, false);
        QVERIFY(!result_nan.empty());

        // Test very close to zero
        QCOMPARE_EQ(jkqtp_floattounitstr(1e-100, 1, false, std::numeric_limits<double>::min()*4), "0");

        // Test exact zero
        QCOMPARE_EQ(jkqtp_floattounitstr(0.0, 0, true), "0");
        QCOMPARE_EQ(jkqtp_floattounitstr(-0.0, 0, true), "0");
    }

    inline void test_jkqtp_floattolatexunitstr_special_values() {
        // Test NaN handling
        double nan_val = std::nan("");
        std::string result_nan = jkqtp_floattolatexunitstr(nan_val, 1, false);
        QVERIFY(!result_nan.empty());

        // Test very close to zero
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1e-100, 1, false, std::numeric_limits<double>::min()*4), "0");

        // Test exact zero
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(0.0, 0, true), "0");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(-0.0, 0, true), "0");
    }

    // ===== CONSISTENCY TESTS =====
    inline void test_jkqtp_floattounitstr_consistency() {
        // Verify that the standard and latex versions produce similar scaled values
        // (though with different unit formatting)
        for (int exp = -30; exp <= 30; exp += 3) {
            double val = pow(10.0, static_cast<double>(exp));
            std::string standard = jkqtp_floattounitstr(val, 1, false);
            std::string latex = jkqtp_floattolatexunitstr(val, 1, false);

            // Both should have the same numerical part (before the unit)
            // Extract the numeric part by removing non-digit chars (except dot and minus)
            size_t pos_standard = standard.find('k');
            if (pos_standard == std::string::npos) pos_standard = standard.find('M');
            if (pos_standard == std::string::npos) pos_standard = standard.find('G');
            if (pos_standard == std::string::npos) pos_standard = standard.find('m');
            if (pos_standard == std::string::npos) pos_standard = standard.find('\xB5');
            if (pos_standard == std::string::npos) pos_standard = standard.find('n');

            // If there's a unit, they should match numerically
            if (pos_standard != std::string::npos) {
                std::string numeric_standard = standard.substr(0, pos_standard);
                // The latex version should contain the same numeric part
                QVERIFY(latex.find(numeric_standard) != std::string::npos);
            }
        }
    }

    inline void test_jkqtp_floattounitstr_remove_trail_zero_consistency() {
        // When remove_trail0 is true, trailing zeros should be removed
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 5, true), "1");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.00000, 5, true), "1");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.20000, 5, true), "1.2");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.23000, 5, true), "1.23");

        // When remove_trail0 is false, trailing zeros should be kept
        QCOMPARE_EQ(jkqtp_floattounitstr(1.0, 5, false), "1.00000");
        QCOMPARE_EQ(jkqtp_floattounitstr(1.2, 5, false), "1.20000");
    }

    inline void test_jkqtp_floattolatexunitstr_remove_trail_zero_consistency() {
        // When remove_trail0 is true, trailing zeros should be removed
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 5, true), "1");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.00000, 5, true), "1");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.20000, 5, true), "1.2");

        // When remove_trail0 is false, trailing zeros should be kept
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.0, 5, false), "1.00000");
        QCOMPARE_EQ(jkqtp_floattolatexunitstr(1.2, 5, false), "1.20000");
    }

};


QTEST_APPLESS_MAIN(JKQTPStringToolsTest)

#include "JKQTPStringTools_test.moc"
