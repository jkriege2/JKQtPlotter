#include <QObject>
#include <QtTest>
#include "jkqtmath/jkqtpstatisticstools.h"

#ifndef QCOMPARE_EQ
#define QCOMPARE_EQ(A,B) if (!static_cast<bool>((A)==(B))) {qDebug()<<QTest::toString(A)<< "!=" << QTest::toString(B); } QVERIFY((A)==(B))
#endif
#ifndef QVERIFY_THROWS_NO_EXCEPTION
#define QVERIFY_THROWS_NO_EXCEPTION(B) B
#endif
#ifndef QVERIFY_THROWS_EXCEPTION
#define QVERIFY_THROWS_EXCEPTION(type, A) QVERIFY_EXCEPTION_THROWN(A, type)
#endif



class JKQTPStatisticsToolsTest : public QObject
{
    Q_OBJECT

public:
    inline JKQTPStatisticsToolsTest() {
    }
    
    inline ~JKQTPStatisticsToolsTest() {
    }

private slots:
    inline void test_jkqtpstatMedian() {
        const std::vector<double> empty;
        const std::vector<double> oneel = {1.23};
        const std::vector<double> twoel = {2, 1};
        const std::vector<double> threeel = {3, 2, 1};
        const std::vector<double> fourel = {4, 2, 1, 3};
        const std::vector<double> fourelnan = {4, 1, JKQTP_NAN, 2, JKQTP_NAN, JKQTP_NAN, 3};
        QCOMPARE_EQ(JKQTPIsOKFloat(jkqtpstatMedian(empty.begin(), empty.end())),false);
        QCOMPARE_EQ(jkqtpstatMedian(oneel.begin(), oneel.end()),1.23);
        QCOMPARE_EQ(jkqtpstatMedian(twoel.begin(), twoel.end()),(1.0+2.0)/2.0);
        QCOMPARE_EQ(jkqtpstatMedian(threeel.begin(), threeel.end()),2.0);
        QCOMPARE_EQ(jkqtpstatMedian(fourel.begin(), fourel.end()),(2.0+3.0)/2.0);
        QCOMPARE_EQ(jkqtpstatMedian(fourelnan.begin(), fourelnan.end()),(2.0+3.0)/2.0);
    }

    inline void test_jkqtpstatMedianOfSortedVector() {
        const std::vector<double> empty;
        const std::vector<double> oneel = {1.23};
        const std::vector<double> twoel = {1, 2};
        const std::vector<double> threeel = {1, 2, 3};
        const std::vector<double> fourel = {1, 2, 3, 4};
        const std::vector<double> fourelnan = {1, JKQTP_NAN, 2, JKQTP_NAN, 3, 4, JKQTP_NAN};
        QCOMPARE_EQ(JKQTPIsOKFloat(jkqtpstatMedianOfSortedVector(empty)),false);
        QCOMPARE_EQ(jkqtpstatMedianOfSortedVector(oneel),oneel[0]);
        QCOMPARE_EQ(jkqtpstatMedianOfSortedVector(twoel),(twoel[0]+twoel[1])/2.0);
        QCOMPARE_EQ(jkqtpstatMedianOfSortedVector(threeel),threeel[1]);
        QCOMPARE_EQ(jkqtpstatMedianOfSortedVector(fourel),(fourel[1]+fourel[2])/2.0);
        QCOMPARE_EQ(JKQTPIsOKFloat(jkqtpstatMedianOfSortedVector(fourelnan)),false);
    }
    inline void test_jkqtpstatQuantile() {
        const std::vector<double> empty;
        const std::vector<double> oneel = {1.23};
        const std::vector<double> twoel = {2, 1};
        const std::vector<double> threeel = {3, 2, 1};
        const std::vector<double> fourel = {4, 2, 1, 3};
        const std::vector<double> fourelnan = {4, 1, JKQTP_NAN, 2, JKQTP_NAN, JKQTP_NAN, 3};
        QCOMPARE_EQ(JKQTPIsOKFloat(jkqtpstatQuantile(empty.begin(), empty.end(),0.25)), false);
        QCOMPARE_EQ(jkqtpstatQuantile(oneel.begin(), oneel.end(),0.25),1.23);
        QCOMPARE_EQ(jkqtpstatQuantile(twoel.begin(), twoel.end(),0.33),1);
        QCOMPARE_EQ(jkqtpstatQuantile(threeel.begin(), threeel.end(), 0.33),1.0);
        QCOMPARE_EQ(jkqtpstatQuantile(fourel.begin(), fourel.end(), 0.25),1);
        QCOMPARE_EQ(jkqtpstatQuantile(fourelnan.begin(), fourelnan.end(), 0.25),1);
    }

};


QTEST_APPLESS_MAIN(JKQTPStatisticsToolsTest)

#include "jkqtpstatisticstools_test.moc"
