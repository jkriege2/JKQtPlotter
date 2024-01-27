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


class JKQTPDataCacheBenchmark : public QObject
{
    Q_OBJECT

public:
    inline JKQTPDataCacheBenchmark() {
    }
    
    inline ~JKQTPDataCacheBenchmark() {
    }

private slots:


    inline void benchmark_JKQTPDataCache_ThreadSafe() {
        JKQTPDataCache<QString, int, JKQTPDataCacheThreadSafe> cache([](int key) { return QString::number(key);}, 100,0.8);

        int sum=cache.get(1).size();
        QBENCHMARK(sum+=cache.get(1).size());
        int i=1;
        QBENCHMARK(sum+=cache.get(++i).size());
        qDebug()<<"sum.size()="<<sum<<", i="<<i;

    }

    inline void benchmark_JKQTPDataCache_NotThreadSafe() {
        JKQTPDataCache<QString, int, JKQTPDataCacheNotThreadSafe> cache([](int key) { return QString::number(key);}, 100,0.8);

        int sum=cache.get(1).size();
        QBENCHMARK(sum+=cache.get(1).size());
        int i=1;
        QBENCHMARK(sum+=cache.get(++i).size());
        qDebug()<<"sum.size()="<<sum<<", i="<<i;

    }
};


QTEST_APPLESS_MAIN(JKQTPDataCacheBenchmark)

#include "JKQTPDataCache_benchmark.moc"
