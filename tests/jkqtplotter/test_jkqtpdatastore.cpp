// Unit tests for JKQTPDatastore - combined and organized
// Uses Qt Test framework
//
// This file consolidates the v1-v3 tests into a single test executable.
// Tests are organized so each test_* method covers one function or a tightly
// related small group of functions.

#include <QtTest/QtTest>
#include <QBuffer>
#include <QTemporaryFile>
#include <QPoint>
#include <QPointF>
#include <cmath>
#include <map>
#include <vector>
#include <list>
#include <numeric>

#include "jkqtplotter/jkqtpdatastorage.h"

static bool roughlyEqual(double a, double b, double eps=1e-9) {
    return std::fabs(a-b) <= eps;
}

class TestJKQTPDatastoreCombined : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {}
    void cleanupTestCase() {}

    // Basic add/get/set tests
    void test_addInternalAndGetSet() {
        JKQTPDatastore ds;
        const size_t N = 5;
        double* arr = static_cast<double*>(malloc(sizeof(double) * N));
        QVERIFY(arr != nullptr);
        for (size_t i = 0; i < N; ++i) arr[i] = double(i) * 2.0 + 0.5;

        size_t col = ds.addInternalColumn(arr, N, QString("internal"));
        QCOMPARE(ds.getColumnCount(), static_cast<size_t>(1));
        QCOMPARE(ds.getRows(col), N);

        for (size_t i = 0; i < N; ++i) QCOMPARE(ds.get(col, i), arr[i]);

        ds.set(col, 2, 1234.5);
        QCOMPARE(ds.get(col, 2), 1234.5);
    }

    void test_addCopiedColumnFromVector_and_iterators() {
        JKQTPDatastore ds;
        std::vector<double> v;
        for (int i = 0; i < 7; ++i) v.push_back(i * 1.5);

        size_t col = ds.addCopiedColumn(v, QString("vec"));
        QCOMPARE(ds.getRows(col), static_cast<size_t>(v.size()));
        for (size_t i = 0; i < ds.getRows(col); ++i) QCOMPARE(ds.get(col, static_cast<int>(i)), v[i]);

        // iterator-pair overload
        std::vector<float> vf;
        for (int i = 0; i < 6; ++i) vf.push_back(float(i) + 0.25f);
        size_t col2 = ds.addCopiedColumn(vf.begin(), vf.end(), QString("iter"));
        QCOMPARE(ds.getRows(col2), static_cast<size_t>(vf.size()));
        for (size_t i = 0; i < ds.getRows(col2); ++i) QCOMPARE(ds.get(col2, static_cast<int>(i)), jkqtp_todouble(vf[i]));
    }

    void test_addImageColumn_and_pixel_accessors() {
        JKQTPDatastore ds;
        const size_t width = 4, height = 3;
        size_t img = ds.addImageColumn(width, height, QString("img"));
        QCOMPARE(ds.getColumnImageWidth(static_cast<int>(img)), width);
        QCOMPARE(ds.getColumnImageHeight(static_cast<int>(img)), height);

        ds.setPixel(img, 0, 0, 1.0);
        ds.setPixel(img, 3, 2, 9.75);
        QCOMPARE(ds.getPixel(img, 0, 0), 1.0);
        QCOMPARE(ds.getPixel(img, 3, 2), 9.75);

        ds.set(img, 5, 5.5);
        QCOMPARE(ds.get(img, 5), 5.5);
    }

    // Back inserter and append tests
    void test_backInserter_and_appendFromContainer() {
        JKQTPDatastore ds;
        size_t col = ds.addColumn(QString("out"));
        auto bi = ds.backInserter(col);
        bi = 3.14;
        bi = 2.0;
        bi = -1.5;
        QCOMPARE(ds.getRows(col), static_cast<size_t>(3));
        QCOMPARE(ds.get(col, 0), 3.14);
        QCOMPARE(ds.get(col, 1), 2.0);
        QCOMPARE(ds.get(col, 2), -1.5);

        std::vector<double> extra = { 10.0, 11.0 };
        ds.appendFromContainerToColumn(col, extra);
        QCOMPARE(ds.getRows(col), static_cast<size_t>(5));
        QCOMPARE(ds.get(col, 3), 10.0);
        QCOMPARE(ds.get(col, 4), 11.0);
    }

    // Iterator traversal and algorithms
    void test_iterators_and_accumulate() {
        JKQTPDatastore ds;
        std::vector<double> v;
        for (int i = 0; i < 10; ++i) v.push_back(i*1.0);
        size_t col = ds.addCopiedColumn(v, QString("alg"));
        double sum = 0.0;
        for (auto it = ds.begin(static_cast<int>(col)); it != ds.end(static_cast<int>(col)); ++it) sum += *it;
        QCOMPARE(sum, 45.0);
    }

    // eraseFromColumn: vector-backed vs internal-backed behavior
    void test_eraseFromColumn_vector_and_internal_backing() {
        JKQTPDatastore ds;
        // vector-backed (addCopiedColumn -> internal vector)
        std::vector<double> v = { 1, 2, 3, 4, 5 };
        size_t col = ds.addCopiedColumn(v, QString("v"));
        QCOMPARE(ds.getRows(col), static_cast<size_t>(5));
        auto it = ds.begin(static_cast<int>(col));
        it += 1;
        ds.eraseFromColumn(it);
        QCOMPARE(ds.getRows(col), static_cast<size_t>(4));
        QCOMPARE(ds.get(col, 0), 1.0);
        QCOMPARE(ds.get(col, 1), 3.0);
        QCOMPARE(ds.get(col, 3), 5.0);

        // internal-backed (addInternalColumn) -> eraseFromColumn should convert to vector-backed and erase
        const size_t N = 4;
        double* data = static_cast<double*>(malloc(sizeof(double) * N));
        QVERIFY(data != nullptr);
        for (size_t i=0;i<N;i++) data[i] = 10.0 + double(i);
        size_t icol = ds.addInternalColumn(data, N, QString("internal_backed"));
        QCOMPARE(ds.getRows(icol), N);
        auto it2 = ds.begin(static_cast<int>(icol));
        it2 += 2;
        ds.eraseFromColumn(it2);
        QCOMPARE(ds.getRows(icol), N - 1);
        QCOMPARE(ds.get(icol, 0), 10.0);
    }

    // erase ranges
    void test_eraseFromColumn_range() {
        JKQTPDatastore ds;
        std::vector<double> v = {0,1,2,3,4,5,6};
        size_t col = ds.addCopiedColumn(v, QString("r"));
        auto itstart = ds.begin(static_cast<int>(col));
        itstart += 2;
        auto itend = ds.begin(static_cast<int>(col));
        itend += 5;
        ds.eraseFromColumn(itstart, itend);
        // removed elements at positions 2,3,4 => remaining 0,1,5,6
        QCOMPARE(ds.getRows(col), static_cast<size_t>(4));
        QCOMPARE(ds.get(col, 2), 5.0);
        QCOMPARE(ds.get(col, 3), 6.0);
    }

    // getNextLowerIndex / getNextHigherIndex
    void test_getNextLowerIndex_behavior() {
        JKQTPDatastore ds;
        std::vector<double> v = {1.0, 3.0, 3.0, 7.0, 9.0};
        size_t col = ds.addCopiedColumn(v, QString("seq"));
        int lower = ds.getNextLowerIndex(col, 3);
        QVERIFY(lower >= 0);
        QVERIFY(ds.get(col, static_cast<size_t>(lower)) < ds.get(col, 3));
    }
    void test_getNextHigherIndex_behavior() {
        JKQTPDatastore ds;
        std::vector<double> v = {1.0, 3.0, 3.0, 7.0, 9.0};
        size_t col = ds.addCopiedColumn(v, QString("seq2"));
        int idx = ds.getNextHigherIndex(col, 1);
        QVERIFY(idx >= 0);
        QCOMPARE(ds.get(col, static_cast<size_t>(idx)), 7.0);
    }

    // setColumnData / setColumnCopiedData / setColumnCopiedImageData
    void test_setColumnData_and_copies() {
        JKQTPDatastore ds;
        size_t col = ds.addColumn(5, QString("setter"));
        QVector<double> qv; qv << 10.0 << 11.0 << 12.0 << 13.0 << 14.0;
        ds.setColumnData(col, qv);
        QCOMPARE(ds.getRows(col), static_cast<size_t>(5));
        for (size_t i=0;i<5;i++) QCOMPARE(ds.get(col, i), qv.at(static_cast<int>(i)));

        double arr[3] = {100.1, 200.2, 300.3};
        ds.setColumnCopiedData(col, arr, 3);
        QCOMPARE(ds.getRows(col), static_cast<size_t>(3));
        QCOMPARE(ds.get(col, 2), 300.3);

        size_t imgCol = ds.addImageColumn(2,2, QString("imgset"));
        double imgdata[4] = {1,2,3,4};
        ds.setColumnCopiedImageData(imgCol, imgdata, 2, 2);
        QCOMPARE(ds.getPixel(imgCol, 1, 1), 4.0);
    }

    // appendToColumn / resizeColumn / appendToColumns
    void test_append_resize_appendToColumns() {
        JKQTPDatastore ds;
        size_t a = ds.addColumn(QString("a"));
        size_t b = ds.addColumn(QString("b"));
        ds.appendToColumn(a, 1.0);
        ds.appendToColumn(a, 2.0);
        QCOMPARE(ds.getRows(a), static_cast<size_t>(2));
        ds.resizeColumn(a, 5);
        QCOMPARE(ds.getRows(a), static_cast<size_t>(5));
        ds.appendToColumns(a, b, 9.5, 8.5);
        QCOMPARE(ds.getRows(a), static_cast<size_t>(6));
        QCOMPARE(ds.getRows(b), static_cast<size_t>(1));
        QCOMPARE(ds.get(a, 5), 9.5);
        QCOMPARE(ds.get(b, 0), 8.5);

        ds.appendToColumns(a, b, 1.1, 2.2); // adds to both (b now has 2 rows)
        QCOMPARE(ds.getRows(b), static_cast<size_t>(2));
    }

    // appendFromContainerToColumn (vector/list)
    void test_appendFromContainerToColumn_variants() {
        JKQTPDatastore ds;
        size_t col = ds.addColumn(QString("vecAppend"));
        std::vector<double> more = {7.7, 8.8};
        ds.appendFromContainerToColumn(col, more);
        QCOMPARE(ds.getRows(col), static_cast<size_t>(2));
        QCOMPARE(ds.get(col, 1), 8.8);

        std::list<double> morel = {9.9, 10.1};
        ds.appendFromContainerToColumn(col, morel);
        QCOMPARE(ds.getRows(col), static_cast<size_t>(4));
        QCOMPARE(ds.get(col, 3), 10.1);
    }

    // scaleColumnValues / inc / dec
    void test_scale_inc_dec() {
        JKQTPDatastore ds;
        std::vector<double> v = {2.0, 4.0, 6.0};
        size_t c = ds.addCopiedColumn(v, QString("scale"));
        ds.scaleColumnValues(c, 2.0);
        QCOMPARE(ds.get(c, 0), 4.0);
        ds.inc(c, 1, 1.5);
        QCOMPARE(ds.get(c, 1), 9.5);
        ds.dec(c, 1, 0.5);
        QCOMPARE(ds.get(c, 1), 9.0);
    }

    // addCopiedPoints variants and addCopiedColumn with C-array stride
    void test_addCopiedPoints_and_stride_column() {
        JKQTPDatastore ds;
        std::vector<QPointF> ptsf = { QPointF(1.0,10.0), QPointF(2.0,20.0) };
        auto p = ds.addCopiedPoints(ptsf, QString("px"), QString("py"));
        QCOMPARE(ds.getRows(p.first), static_cast<size_t>(2));
        QCOMPARE(ds.get(p.second, 1), 20.0);

        QList<QPoint> qpts; qpts << QPoint(5,50) << QPoint(6,60);
        auto p2 = ds.addCopiedPoints(qpts, QString("qx"), QString("qy"));
        QCOMPARE(ds.get(p2.first, 0), 5.0);
        QCOMPARE(ds.get(p2.second, 0), 50.0);

        int arr[6] = {0,1,2,3,4,5};
        size_t scol = ds.addCopiedColumn<int>(arr, 3, 2, 0, QString("stride"));
        QCOMPARE(ds.getRows(scol), static_cast<size_t>(3));
        QCOMPARE(ds.get(scol, 2), 4.0);
    }

    // addCopiedImageAsColumn and transpose
    void test_addCopiedImageAsColumn_and_transpose() {
        JKQTPDatastore ds;
        std::vector<double> img = {1,2,3, 4,5,6};
        size_t col = ds.addCopiedImageAsColumn(img, 3, QString("imgc"));
        QCOMPARE(ds.getColumnImageWidth(static_cast<int>(col)), static_cast<size_t>(3));
        QCOMPARE(ds.getPixel(col, 2, 1), 6.0);

        double colmaj[] = {1,2,3, 10,20,30}; // column-major 2x3: col0=1,2,3 col1=10,20,30
        size_t tcol = ds.addCopiedImageAsColumnTranspose<double>(colmaj, 2, 3, QString("trans"));
        QCOMPARE(ds.getRows(tcol), static_cast<size_t>(6));
        QCOMPARE(ds.getPixel(tcol, 1, 2), 30.0);
    }

    // addCopiedColumnMasked variants
    void test_addCopiedColumnMasked() {
        JKQTPDatastore ds;
        std::vector<double> data = {1.1,2.2,3.3,4.4};
        std::vector<char> mask = {0,1,1,0};
        size_t col = ds.addCopiedColumnMasked<std::vector<double>, std::vector<char>>(data, mask, QString("masked"), true);
        QCOMPARE(ds.getRows(col), static_cast<size_t>(2));
        QCOMPARE(ds.get(col, 0), 2.2);

        double carr[] = {5.5, 6.6, 7.7};
        bool cmask[] = {true, false, true};
        size_t col2 = ds.addCopiedColumnMasked<double>(carr, cmask, 3, QString("masked2"), true);
        QCOMPARE(ds.getRows(col2), static_cast<size_t>(2));
        QCOMPARE(ds.get(col2, 1), 7.7);
    }

    // addLinearColumn / addLogColumn / addDecadeLogColumn
    void test_linear_and_log_columns() {
        JKQTPDatastore ds;
        size_t lin = ds.addLinearColumn(5, 0.0, 4.0, QString("lin"));
        QCOMPARE(ds.getRows(lin), static_cast<size_t>(5));
        QCOMPARE(ds.get(lin, 0), 0.0);
        QCOMPARE(ds.get(lin, 4), 4.0);

        size_t logc = ds.addLogColumn(3, 1.0, 100.0, QString("log"));
        QVERIFY(roughlyEqual(ds.get(logc, 0), 1.0));
        QVERIFY(roughlyEqual(ds.get(logc, 2), 100.0));

        size_t dec = ds.addDecadeLogColumn(3, 0.0, 2.0, QString("dec"));
        QVERIFY(roughlyEqual(ds.get(dec, 0), 1.0));
        QVERIFY(roughlyEqual(ds.get(dec, 2), 100.0));
    }

    // addLinearGridColumns
    void test_addLinearGridColumns() {
        JKQTPDatastore ds;
        auto grid = ds.addLinearGridColumns(2, 0.0, 1.0, 3, 10.0, 12.0, QString("gx"), QString("gy"));
        size_t gx = grid.first, gy = grid.second;
        QCOMPARE(ds.getRows(gx), static_cast<size_t>(6));
        QCOMPARE(ds.get(gx, 0), 0.0);
        QCOMPARE(ds.get(gy, 0), 10.0);
        QCOMPARE(ds.get(gx, ds.getRows(gx)-1), 1.0);
    }

    // addCalculatedColumn and addCalculatedImageColumn
    void test_addCalculatedColumn_variants() {
        JKQTPDatastore ds;
        size_t c1 = ds.addCalculatedColumn(4, [](size_t i, JKQTPDatastore*)->double { return double(i)*3.0; }, QString("calcA"));
        QCOMPARE(ds.getRows(c1), static_cast<size_t>(4));
        QCOMPARE(ds.get(c1, 2), 6.0);

        size_t c2 = ds.addCalculatedColumn(3, [](size_t i)->double { return double(i) + 0.5; }, QString("calcB"));
        QCOMPARE(ds.get(c2, 1), 1.5);

        size_t img = ds.addCalculatedImageColumn(3, 2, [](size_t x, size_t y)->double { return double(x) + double(y)*10.0; }, QString("cimg"));
        QCOMPARE(ds.getColumnImageWidth(static_cast<int>(img)), static_cast<size_t>(3));
        QCOMPARE(ds.getPixel(img, 2, 1), 2.0 + 10.0);
    }

    // addCalculatedColumnFromColumn (single and pair)
    void test_addCalculatedColumnFromColumn() {
        JKQTPDatastore ds;
        std::vector<double> base = {2.0,4.0,6.0};
        size_t bcol = ds.addCopiedColumn(base, QString("base"));
        size_t plus = ds.addCalculatedColumnFromColumn(bcol, [](double v)->double { return v + 1.0; }, QString("plus"));
        QCOMPARE(ds.getRows(plus), ds.getRows(bcol));
        QCOMPARE(ds.get(plus, 0), 3.0);

        std::vector<double> x = {1,2,3,4}, y = {10,20,30,40};
        size_t cx = ds.addCopiedColumn(x, QString("x"));
        size_t cy = ds.addCopiedColumn(y, QString("y"));
        size_t sum = ds.addCalculatedColumnFromColumn(cx, cy, [](double a, double b)->double { return a + b; }, QString("sum"));
        QCOMPARE(ds.get(sum, 2), 3.0 + 30.0);
    }

    // getColumnIDs / getColumnIDsIntVec / getColumnName / getColumnNames / getData
    void test_column_ids_names_and_getData() {
        JKQTPDatastore ds;
        size_t a = ds.addColumn(QString("alpha"));
        size_t b = ds.addColumn(QString("beta"));
        QList<size_t> ids = ds.getColumnIDs();
        QVERIFY(ids.contains(a));
        QVector<int> idsInt = ds.getColumnIDsIntVec();
        QVERIFY(idsInt.size() == ids.size());

        QStringList names = ds.getColumnNames();
        QVERIFY(names.contains(QString("alpha")));

        QString cname;
        auto dv = ds.getData(a, &cname);
        QCOMPARE(cname, QString("alpha"));

        QStringList allNames;
        auto allData = ds.getData(&allNames);
        QVERIFY(!allNames.isEmpty());
        QVERIFY(allData.size() >= 2);
    }

    // save functions (CSV/Matlab/SYLK/DIF) write some output
    void test_save_functions_basic_write() {
        JKQTPDatastore ds;
        std::vector<double> v = {1.0,2.0,3.0};
        ds.addCopiedColumn(v, QString("forSave"));

        QByteArray ba; QBuffer buf(&ba);
        QVERIFY(buf.open(QIODevice::WriteOnly | QIODevice::Text));
        QTextStream ts(&buf);
        ds.saveCSV(ts);
        buf.close();
        QVERIFY(!ba.isEmpty());

        QByteArray ba2; QBuffer buf2(&ba2);
        QVERIFY(buf2.open(QIODevice::WriteOnly | QIODevice::Text));
        QTextStream ts2(&buf2);
        ds.saveMatlab(ts2);
        buf2.close();
        QVERIFY(!ba2.isEmpty());

        QTemporaryFile f1; f1.open(); QString fn1 = f1.fileName(); f1.close();
        ds.saveSYLK(fn1);
        QFile c1(fn1);
        QVERIFY(c1.exists());
        QVERIFY(c1.size() > 0);

        QTemporaryFile f2; f2.open(); QString fn2 = f2.fileName(); f2.close();
        ds.saveDIF(fn2);
        QFile c2(fn2);
        QVERIFY(c2.exists());
        QVERIFY(c2.size() > 0);
    }
};

QTEST_MAIN(TestJKQTPDatastoreCombined)
#include "test_jkqtpdatastore.moc"