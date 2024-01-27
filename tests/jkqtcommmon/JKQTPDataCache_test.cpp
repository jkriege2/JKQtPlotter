#include <QObject>
#include <QtTest>
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


class JKQTPDataCacheTest : public QObject
{
    Q_OBJECT

public:
    inline JKQTPDataCacheTest() {
    }
    
    inline ~JKQTPDataCacheTest() {
    }

private slots:


    inline void test_JKQTPDataCache_ThreadSafe() {
        JKQTPDataCache<QString, int, JKQTPDataCacheThreadSafe> cache([](int key) { return QString::number(key);}, 100,0.8);

        QString sum;
        for (int i=0; i<100; i++) {
            sum+=cache.get(i);
        }
        qDebug()<<"sum.size()="<<sum.size();
        QCOMPARE_EQ(cache.size(), 100);
        for (int i=0; i<100; i++) {
            QVERIFY(cache.contains(i));
        }
        sum+=cache.get(5000);
        QCOMPARE_EQ(cache.size(), 81);
        for (int i=1000; i<1005; i++) {
            sum+=cache.get(i);
        }
        QCOMPARE_EQ(cache.size(), 86);

        qDebug()<<"sum.size()="<<sum.size();

    }

    inline void test_JKQTPDataCache_NotThreadSafe() {
        JKQTPDataCache<QString, int, JKQTPDataCacheNotThreadSafe> cache([](int key) { return QString::number(key);}, 100,0.8);

        QString sum;
        for (int i=0; i<100; i++) {
            sum+=cache.get(i);
        }
        qDebug()<<"sum.size()="<<sum.size();
        QCOMPARE_EQ(cache.size(), 100);
        for (int i=0; i<100; i++) {
            QVERIFY(cache.contains(i));
        }
        sum+=cache.get(5000);
        QCOMPARE_EQ(cache.size(), 81);
        for (int i=1000; i<1005; i++) {
            sum+=cache.get(i);
        }
        QCOMPARE_EQ(cache.size(), 86);

        qDebug()<<"sum.size()="<<sum.size();

    }

};


QTEST_APPLESS_MAIN(JKQTPDataCacheTest)

#include "JKQTPDataCache_test.moc"
