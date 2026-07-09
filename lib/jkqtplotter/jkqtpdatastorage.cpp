/*
    Copyright (c) 2008-2026 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "jkqtplotter/jkqtpdatastorage.h"
#include <QDebug>
#include <QtGlobal>
#include <limits>
#include <cmath>
#include <cstring>

/**************************************************************************************************************************
 * JKQTPColumn
 **************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore_):
    data(nullptr),
    rows(0),
    datavec(),
    storageType(StorageType::Vector),
    imageColumns(1),
    datastore(datastore_),
    name(""),
    valid(false)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore_, const QString &name_, DataTransferMode transferMode,  double *data_, size_t datasize_, size_t imageColumns_):
    data(data_),
    rows(datasize_),
    datavec(),
    storageType(StorageType::External),
    imageColumns(imageColumns_),
    datastore(datastore_),
    name(name_),
    valid(true)
{
    if (!data) {
        rows=0;
        data=nullptr;
        imageColumns=1;
    } else {
        switch(transferMode) {
        case DataTransferMode::TransferOwnership:
            storageType=StorageType::Internal;
            rows=datasize_;
            data=data_;
            break;
        case DataTransferMode::CopyData:
            storageType=StorageType::Vector;
            data=nullptr;
            rows=0;
            datavec.reserve(datasize_*110/100);
            datavec.resize(datasize_);
            std::memcpy(datavec.data(), data, datasize_*sizeof(double));
            break;
        case DataTransferMode::ExternallyOwned:
            storageType=StorageType::External;
            rows=datasize_;
            data=data_;
            break;
        }
        valid=true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore_, const QString &name_, const double *data_, size_t datasize_, size_t imageColumns_):
    data(nullptr),
    rows(0),
    storageType(StorageType::Vector),
    imageColumns(imageColumns_),
    datastore(datastore_),
    name(name_),
    valid(true)
{
    storageType=StorageType::Vector;
    data=nullptr;
    rows=0;
    datavec.reserve(datasize_*110/100);
    datavec.resize(datasize_);
    std::memcpy(datavec.data(), data, datasize_*sizeof(double));
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore_, const QString &name_, const QVector<double> &data_, size_t imageColumns_):
    data(nullptr),
    rows(0),
    datavec(data_),
    storageType(StorageType::Vector),
    imageColumns(imageColumns_),
    datastore(datastore_),
    name(name_),
    valid(true)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore_, const QString &name_, QVector<double> &&data_, size_t imageColumns_):
    data(nullptr),
    rows(0),
    datavec(std::forward<QVector<double>>(data_)),
    storageType(StorageType::Vector),
    imageColumns(imageColumns_),
    datastore(datastore_),
    name(name_),
    valid(true)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore_, const QString &name_, size_t rows, double initialVal, size_t imageColumns_):
    JKQTPColumn(datastore_, name_, QVector<double>(rows, initialVal), imageColumns_)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore_, const QString &name_, size_t imageColumns_):
    JKQTPColumn(datastore_, name_, QVector<double>(), imageColumns_)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::~JKQTPColumn()
{
    clear();
    valid=false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPColumn && other):
    data(other.data),
    rows(other.rows),
    datavec(std::move(other.datavec)),
    storageType(other.storageType),
    imageColumns(other.imageColumns),
    datastore(other.datastore),
    name(other.name),
    valid(other.valid)
{
    other.data=nullptr;
    other.rows=0;
    other.valid=false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn& JKQTPColumn::operator=(JKQTPColumn && other)
{
    clear();
    data=other.data;
    rows=other.rows;
    datavec=std::move(other.datavec);
    storageType=other.storageType;
    imageColumns=other.imageColumns;
    datastore=other.datastore;
    name=other.name;
    valid=other.valid;
    other.data=nullptr;
    other.rows=0;
    other.valid=false;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPColumn::operator==(const JKQTPColumn &other) const {
    if (!valid && !other.valid) return true;
    if (storageType==other.storageType) {
        switch(storageType) {
        case StorageType::Vector:
            return datavec==other.datavec;
        case StorageType::Internal:
        case StorageType::External:
            return data==other.data && rows==other.rows;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::setName(const QString &__value)
{
    name = __value;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::setImageColumns(size_t imageWidth)
{
    imageColumns=imageWidth;
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::copyData(QVector<double> &copyTo) const
{
    const auto* p=getPointer();
    const auto N=getRows();
    if (N==0 || p==nullptr) {
        copyTo.clear();
        return;
    }
    copyTo.resize(N);
    std::memcpy(copyTo.data(), p, N*sizeof(double));
}

////////////////////////////////////////////////////////////////////////////////////////////////
QVector<double> JKQTPColumn::copyData()
{
    QVector<double> d;
    copyData(d);
    return d;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::clear()
{
    if (storageType==StorageType::Internal) {
        if (data && rows>0) std::free(data);
    }
    data=nullptr;
    rows=0;
    datavec.clear();
    imageColumns=1;
    storageType=StorageType::Vector;
    valid=true;
}




////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPColumn::copy(const double *data, size_t N, size_t offset) {
    auto* p=getPointer(offset);
    if (!p) return 0;
    const auto rows = getRows();
    // rows=10;
    // offset=2
    // N=10
    //        0  1  2  3  4  5  6  7  8  9
    // p     |----------------------------|
    // offset       |
    // offset+N     |---------------------|------|
    // NtoCopy = 8.........................
    const size_t Ntocopy=(N+offset<=rows) ? N: (rows-offset);
    std::memcpy(p, data, Ntocopy*sizeof(double));
    return Ntocopy;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::exchange(double value, double replace)
{
    const size_t rows=getRows();
    auto* p=getPointer();
    if (!p) return;
    for (size_t i=0; i<rows; i++) {
        const double v=p[i];
        if (v==value) p[i]=replace;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::subtract(double value)
{
    const size_t rows=getRows();
    auto* p=getPointer();
    if (!p) return;
    for (size_t i=0; i<rows; i++) {
        p[i] = p[i] - value;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::scale(double factor)
{
    const size_t rows=getRows();
    auto* p=getPointer();
    if (!p) return;
    for (size_t i=0; i<rows; i++) {
        p[i] = p[i] * factor;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::setAll(double value)
{
    const size_t rows=getRows();
    auto* p=getPointer();
    if (!p) return;
    for (size_t i=0; i<rows; i++) {
        p[i] = value;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPColumn::convertToVectorColumn()
{
    if (isVectorColumn() || !isValid()) return false;
    QVector<double> old;
    copyData(old);
    clear();
    datavec=std::move(old);
    storageType=StorageType::Vector;
    valid=true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::convertToVectorColumnFromRanges(size_t start1, size_t end1, size_t start2, size_t end2)
{
    if (!isValid()) clear();
    QVector<double> old_data;
    const auto NRowsOld=getRows();
    const auto* p=getPointer();
    const auto N=getRows();

    const bool copyRange1=start1<=end1 && start1<NRowsOld && end1<NRowsOld;
    const size_t NRowsRange1=end1-start1+1;
    const bool copyRange2=start2<=end2 && start2<NRowsOld && end2<NRowsOld;
    const size_t NRowsRange2=end2-start2+1;
    const bool copyAll=(start1>=NRowsOld && end1>=NRowsOld && start2>=NRowsOld && end2>=NRowsOld);
    const size_t NToReserve=copyAll?NRowsOld:(copyRange1?NRowsRange1:0)+(copyRange2?NRowsRange2:0)+10;

    // copy all data into old_data, as specified by start1,end1,start2,end2
    old_data.reserve(NToReserve);
    if (copyAll) {
        copyData(old_data);
    } else {
        if (copyRange1) {
            for (size_t it=start1; it<=std::min(NRowsOld-1,end1); ++it) old_data.push_back(getValue(it));
        }
        if (copyRange2) {
            for (size_t it=start2; it<=std::min(NRowsOld-1,end2); ++it) old_data.push_back(getValue(it));
        }
    }
    clear();
    datavec=std::move(old_data);
    storageType=StorageType::Vector;
    valid=true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::convertToVectorColumnFromRange(size_t start1, size_t end1)
{
    convertToVectorColumnFromRanges(start1,end1, getRows(), getRows());
}








/**************************************************************************************************************************
 * JKQTPDatastore
 **************************************************************************************************************************/


////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(JKQTPColumn &&col) {
    columns.insert(maxColumnsID, std::make_shared<JKQTPColumn>(std::forward<JKQTPColumn>(col)));
    maxColumnsID++;
    return maxColumnsID-1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastore::JKQTPDatastore():
    m_invalidColumn(new JKQTPColumn),
    maxColumnsID(0)
{
    clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastore::~JKQTPDatastore() {
    clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPDatastore::hasColumn(size_t i) const
{
    return columns.find(i)!=columns.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::begin(int i)
{
    JKQTPASSERT(i>=0);
    return begin(static_cast<size_t>(i));
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::end(int i)
{
    JKQTPASSERT(i>=0);
    return end(static_cast<size_t>(i));
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::begin(int i) const
{
    JKQTPASSERT(i>=0);
    return cbegin(static_cast<size_t>(i));

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::end(int i) const
{
    JKQTPASSERT(i>=0);
    return cend(static_cast<size_t>(i));

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::cbegin(int i) const
{
    JKQTPASSERT(i>=0);
    return cbegin(static_cast<size_t>(i));

}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::cend(int i) const
{
    JKQTPASSERT(i>=0);
    return cend(static_cast<size_t>(i));
}



////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::begin(size_t i)
{
    auto it=columns.find(i);
    if (it==columns.end()) throw std::runtime_error("column "+std::to_string(i)+" does not exist in JKQTPDatastore");
    else return it.value()->begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::end(size_t i)
{
    auto it=columns.find(i);
    if (it==columns.end()) throw std::runtime_error("column "+std::to_string(i)+" does not exist in JKQTPDatastore");
    else return it.value()->end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::begin(size_t i) const
{
    return cbegin(i);
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::end(size_t i) const
{
    return cend(i);
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::cbegin(size_t i) const
{
    auto it=columns.find(i);
    if (it==columns.end()) throw std::runtime_error("column "+std::to_string(i)+" does not exist in JKQTPDatastore");
    else return it.value()->begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::cend(size_t i) const
{
    auto it=columns.find(i);
    if (it==columns.end()) throw std::runtime_error("column "+std::to_string(i)+" does not exist in JKQTPDatastore");
    else return it.value()->end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnBackInserter JKQTPDatastore::backInserter(int i)
{
    if (i<0) return JKQTPColumnBackInserter(this, std::numeric_limits<size_t>::max());
    auto it=columns.find(static_cast<size_t>(i));
    if (it==columns.end()) return JKQTPColumnBackInserter(this, std::numeric_limits<size_t>::max());
    else return JKQTPColumnBackInserter(this, static_cast<size_t>(i));
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnBackInserter JKQTPDatastore::backInserter(size_t i)
{
    auto it=columns.find(i);
    if (it==columns.end()) return JKQTPColumnBackInserter(this, std::numeric_limits<size_t>::max());
    else return JKQTPColumnBackInserter(this, static_cast<size_t>(i));
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::clear(){
    maxColumnsID=0;
    columns.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::deleteAllColumns(const QString& name) {
    QList<size_t> ids;
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value()->getName()==name) {
            ids.append(it.key());
        }
    }
    for (int i=0; i<ids.size(); i++) {
        deleteColumn(ids[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::deleteAllPrefixedColumns(QString prefix) {
    QList<size_t> ids;
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value()->getName().startsWith(prefix)) {
            ids.append(it.key());
        }
    }
    for (int i=0; i<ids.size(); i++) {
        deleteColumn(ids[i]);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////
quint16 JKQTPDatastore::getColumnChecksum(int column) const
{
    return columns[column]->calculateChecksum();
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::deleteColumn(size_t column) {
    columns.remove(column);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getColumnNum(const QString& name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value()->getName()==name) return static_cast<int>(it.key());
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
QString JKQTPDatastore::getColumnName(size_t column)
{
    auto it=columns.find(column);

    if (it==columns.end()) return "";
    else return it.value()->getName();
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::ensureColumnNum(const QString& name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value()->getName()==name) return static_cast<int>(it.key());
    }
    return static_cast<int>(addColumn(0, name));
}


////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(size_t rows, const QString& name, double initVal) {
    return addColumn(JKQTPColumn(this, name, rows, initVal));
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(const QString& name) {
    return addColumn(JKQTPColumn(this, name));
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(double* data, size_t rows, const QString& name) {
    return addColumn(JKQTPColumn(this, name, JKQTPColumn::ExternallyOwned, data, rows));
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addInternalColumn(double* data, size_t rows, const QString& name) {
    return addColumn(JKQTPColumn(this, name, JKQTPColumn::TransferOwnership, data, rows));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addInternalColumn(QVector<double> &&data, const QString &name)
{
    return addColumn(JKQTPColumn(this, name, std::forward<QVector<double>>(data)));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addImageColumn(size_t width, size_t height, const QString &name)
{
    return addColumn(JKQTPColumn(this, name, width*height, 0.0, width));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addImageColumn(double *data, size_t width, size_t height, const QString &name)
{
    return addColumn(JKQTPColumn(this, name, JKQTPColumn::ExternallyOwned, data, width*height, width));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addInternalImageColumn(double *data, size_t width, size_t height, const QString &name)
{
    return addColumn(JKQTPColumn(this, name, JKQTPColumn::TransferOwnership, data, width*height, width));
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnImageWidth(size_t column, size_t imageWidth)
{
    columns[column]->setImageColumns(imageWidth);
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPDatastore::isColumnDataExternal(size_t column) const
{
    return columns[column]->getStorageType()==JKQTPColumn::StorageType::External;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPDatastore::isColumnDataExternal(int column) const
{
    return columns[column]->getStorageType()==JKQTPColumn::StorageType::External;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPDatastore::isVectorColumn(int column) const
{
    return columns[column]->isVectorColumn();
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPDatastore::isVectorColumn(size_t column) const
{
    return columns[column]->isVectorColumn();
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::convertToVectorColumn(size_t column)
{
    columns[column]->convertToVectorColumn();
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::convertToVectorColumn(int column)
{
    convertToVectorColumn(static_cast<size_t>(column));
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnImageHeight(size_t column, size_t imageHeight)
{
    columns[column]->setImageColumns(columns[column]->getRows()/imageHeight);
}








////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::copyColumn(size_t old_column, size_t start, size_t stride, const QString& name)
{
    const auto old=columns[old_column];
    size_t N=old->getRows();
    QVector<double> d; d.reserve(N);
    for (size_t i=start; i<N; i+=stride) {
        d.push_back(old->getValue(i));
    }
    size_t n=addInternalColumn(std::move(d), name);
    return n;
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::copyColumn(size_t old_column, const QString& name)
{
    return copyColumn(old_column, 0, 1, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::copyColumnData(size_t toColumn, size_t fromColumn)
{
    resizeColumn(toColumn, getRows(fromColumn));
    std::copy(begin(fromColumn), end(fromColumn), begin(toColumn));
    setColumnImageWidth(toColumn, getColumnImageWidth(static_cast<int>(fromColumn)));
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const std::list<QPoint> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const std::list<QPointF> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const std::vector<QPoint> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const std::vector<QPointF> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const QList<QPoint> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const QList<QPointF> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}

#if QT_VERSION<QT_VERSION_CHECK(6,0,0)
////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const QVector<QPoint> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addCopiedPoints(const QVector<QPointF> &points, const QString &namex, const QString &namey)
{
    const auto cx=addColumn(points.size(),namex,0.0);
    const auto cy=addColumn(points.size(),namey,0.0);
    int i=0;
    for (const auto& p: points) {
        set(cx, i, p.x());
        set(cy, i, p.y());
        i++;
    }
    return {cx,cy};
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addLinearColumn(size_t rows, double start, double end, const QString& name) {
    const double delta=(end-start)/static_cast<double>(rows-1);
    QVector<double> d(rows);
    for (size_t i=0; i<rows; i++) {
        d[i]= start+static_cast<double>(i) * delta;
         //std::cout<<"copy@"<<i<<" = "<<data[i]<<std::endl;
    }
    return addInternalColumn(std::move(d),name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addLogColumn(size_t rows, double start, double end, const QString &name)
{
    const double x0=log(start)/JKQTPSTATISTICS_LN10;
    const double x1=log(end)/JKQTPSTATISTICS_LN10;
    QVector<double> d(rows);
    for (size_t i=0; i<rows; i++) {
        d[i]= pow(10.0, x0+static_cast<double>(i)/static_cast<double>(rows-1)*(x1-x0));
    }
    return addInternalColumn(std::move(d),name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addDecadeLogColumn(size_t rows, double startDecade, double endDecade, const QString &name)
{
    const double x0=startDecade;
    const double x1=endDecade;
    QVector<double> d(rows);
    for (size_t i=0; i<rows; i++) {
        d[i]= pow(10.0, x0+static_cast<double>(i)/static_cast<double>(rows-1)*(x1-x0));
    }
    return addInternalColumn(std::move(d),name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
std::pair<size_t, size_t> JKQTPDatastore::addLinearGridColumns(size_t width, double startX, double endX, size_t height, double startY, double endY, const QString &nameX, const QString &nameY)
{
    const double decX=(endX-startX)/static_cast<double>(width-1);
    const double decY=(endY-startY)/static_cast<double>(height-1);
    double y=startY;
    size_t cx=addColumn(width*height, nameX);
    size_t cy=addColumn(width*height, nameY);
    size_t i=0;
    for (size_t iy=0; iy<height; iy++) {
        double x=startX;
        for (size_t ix=0; ix<width; ix++) {
            set(cx, i, x);
            set(cy, i, y);
            x+=decX;
            i++;
        }
        y+=decY;
    }
    return std::make_pair(cx,cy);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCalculatedColumn(size_t rows, const std::function<double (size_t, JKQTPDatastore *)> &f, const QString &name)
{
    JKQTPColumn col(this,name, rows, 0.0);
    for (size_t i=0; i<rows; i++) {
        col[i]=f(i, this);
    }
    return addColumn(std::move(col));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCalculatedColumn(size_t rows, const std::function<double (size_t)> &f, const QString &name)
{
    JKQTPColumn col(this,name, rows, 0.0);
    for (size_t i=0; i<rows; i++) {
        col[i]=f(i);
    }
    return addColumn(std::move(col));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCalculatedImageColumn(size_t cols, size_t rows, const std::function<double (size_t, size_t)> &f, const QString &name)
{
    size_t col=addImageColumn(cols, rows, name);
    auto colref = columns[col];
    for (size_t y=0; y<rows; y++) {
        for (size_t x=0; x<cols; x++) {
            colref->setPixelValue(x, y, f(x,y));
        }
    }
    return col;

}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCalculatedColumnFromColumn(size_t otherColumn, const std::function<double (double)> &f, const QString &name)
{
    const auto oc=columns[otherColumn];
    const auto N=oc->getRows();
    JKQTPColumn col(this,name, N, 0.0);
    for (size_t i=0; i<N; i++) {
        col[i]=f(oc->operator[](i));
    }
    return addColumn(std::move(col));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCalculatedColumnFromColumn(size_t otherColumnX, size_t otherColumnY, const std::function<double (double, double)> &f, const QString &name)
{
    const auto ocx=columns[otherColumnX];
    const auto ocy=columns[otherColumnY];
    const size_t N= qMin(ocx->getRows(), ocy->getRows());
    JKQTPColumn col(this,name, N, 0.0);
    for (size_t i=0; i<N; i++) {
        col[i]=f(ocx->operator[](i), ocy->operator[](i));
    }
    return addColumn(std::move(col));
}

////////////////////////////////////////////////////////////////////////////////////////////////
QVector<int> JKQTPDatastore::getColumnIDsIntVec() const {
    QVector<int> ks;
    const auto keys=columns.keys();
    for (auto it=keys.begin(); it!=keys.end(); ++it) {
        ks<<static_cast<int>(*it);
    }
    return ks;
}


////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextLowerIndex(size_t column, size_t row, int start, int end) const
{
    const auto col=columns[column];
    if (start<0 && end>=0) return getNextLowerIndex(column, row, 0, end);
    else if (start>=0 && end<0)  return getNextLowerIndex(column, row, start, static_cast<int>(col->getRows())-1);
    else if (start<0 && end<0)  return getNextLowerIndex(column, row, 0, static_cast<int>(col->getRows())-1);
    else {
        double d=0;
        const double v=col->getValue(row);
        int res=-1;
        for ( int i=start; i<=end; i++) {
            if (i!=static_cast<int>(row)) {
                const double v1=col->getValue(i);
                const double dd=v1-v;
                if ((dd<0) && ((fabs(dd)<d)||(d==0.0))) {
                    res=i;
                    d=fabs(dd);
                    //std::cout<<"   getNextLowerIndex("<<column<<", "<<row<<", "<<start<<", "<<end<<"): i="<<i<<":  OK   res="<<res<<" d="<<d<<"\n";
                } /*else {
                    std::cout<<"   getNextLowerIndex("<<column<<", "<<row<<", "<<start<<", "<<end<<"): i="<<i<<":  FAIL res="<<res<<" dd="<<dd<<" v1="<<v1<<" v="<<v<<"\n";
                }*/
            }
        }
        return res;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextLowerIndex(size_t column, size_t row) const
{
    return getNextLowerIndex(column, row, 0, static_cast<int>(columns[column]->getRows())-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextHigherIndex(size_t column, size_t row, int start, int end) const
{
    const auto col=columns[column];
    if (start<0 && end>=0) return getNextHigherIndex(column, row, 0, end);
    else if (start>=0 && end<0)  return getNextHigherIndex(column, row, start, static_cast<int>(col->getRows())-1);
    else if (start<0 && end<0)  return getNextHigherIndex(column, row, 0, static_cast<int>(col->getRows())-1);
    else {
        double d=0;
        const double v=col->getValue(row);
        int res=-1;
        for ( int i=start; i<=end; i++) {
            if (i!=static_cast<int>(row)) {
                const double v1=col->getValue(i);
                const double dd=v1-v;
                if ((dd>0) && ((fabs(dd)<d)||(d==0.0))) {
                    res=i;
                    d=fabs(dd);
                }
            }
        }
        return res;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextHigherIndex(size_t column, size_t row) const
{
    return getNextHigherIndex(column, row, 0, static_cast<int>(columns[column]->getRows())-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextLowerIndex(int column, size_t row, int start, int end) const {
    return getNextLowerIndex(static_cast<size_t>(column), row, start, end);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextLowerIndex(int column, size_t row) const {
    return getNextLowerIndex(static_cast<size_t>(column), row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextHigherIndex(int column, size_t row, int start, int end) const {
    return getNextHigherIndex(static_cast<size_t>(column), row, start, end);
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::eraseFromColumn(JKQTPColumnConstIterator pos) {
    eraseFromColumn(pos, pos);
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::eraseFromColumn(JKQTPColumnConstIterator pos, JKQTPColumnConstIterator posEnd) {
    // both iterators must reference the same column object
    if (pos.getColumn() != posEnd.getColumn()) {
        JKQTPASSERT_M(false, "in eraseFromColumn(pos,posEnd) both iterators have to point to the same column");
        return;
    }

    const JKQTPColumn* pcol = pos.getColumn();
    if (!pcol) return;

    // find the key (index) of this column in the QMap by address comparison
    size_t itc = std::numeric_limits<size_t>::max();
    for (auto it = columns.begin(); it != columns.end(); ++it) {
        if (it.value().get() == pcol) { itc = it.key(); break; }
    }
    if (itc == std::numeric_limits<size_t>::max()) {
        // column not found in map: nothing to do
        return;
    }

    auto column = columns[itc];
    const size_t NRows = column->getRows();
    if (!pos.isValid()) {
        // nothing to erase if start iterator is invalid
        return;
    }

    // prepare ranges to copy: [start1..end1] and [start2..end2] (inclusive)
    size_t start1 = 0;
    size_t end1 = (NRows > 0 ? NRows - 1 : 0);
    size_t start2 = NRows; // disabled by default (start2 > end2)
    size_t end2 = NRows;

    // compute the two ranges that should be kept (i.e. copy those ranges into a new vector item)
    if (pos == column->begin()) {
        // deleting from begin
        if ( (posEnd == column->end()) ||
            (posEnd.isValid() && static_cast<size_t>(posEnd.getPosition()) == (NRows > 0 ? (NRows - 1) : 0)) ) {
            column->clear();
            return;
        } else if (posEnd.isValid()) {
            // keep rows after posEnd
            start1 = static_cast<size_t>((posEnd + 1).getPosition());
            end1 = (NRows > 0 ? NRows - 1 : 0);
            start2 = NRows; // disable second range
            end2 = NRows;
        } else {
            // posEnd invalid and not equal to end -> nothing sensible to do
            return;
        }
    } else {
        // deleting beginning somewhere after 0
        if (!posEnd.isValid() || posEnd == column->end()) {
            // keep 0..pos-1 only
            start1 = 0;
            end1 = static_cast<size_t>((pos - 1).getPosition());
            start2 = NRows; // disable second range
            end2 = NRows;
        } else {
            // keep 0..pos-1 and posEnd+1..NRows-1
            start1 = 0;
            end1 = static_cast<size_t>((pos - 1).getPosition());
            start2 = static_cast<size_t>((posEnd + 1).getPosition());
            end2 = (NRows > 0 ? NRows - 1 : 0);
        }
    }

    // try to convert to a vector item from the computed ranges; convertToVectorItemFromRanges returns true if a new vector item was created
    column->convertToVectorColumnFromRanges(start1, end1, start2, end2);
}


////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::getMaxRows() const {
    size_t res=0;
    /*for (size_t i=0; i<columns.size(); i++) {
        size_t r=columns[i].getRows();
        if (r>res) res=r;
    }*/
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    while (it.hasNext()) {
        it.next();
        size_t r=it.value()->getRows();
        if (r>res) res=r;
    }

    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::saveCSV(const QString& filename, const QSet<int>& userColumns, const QString& separator, const QString& decimal_separator, const QString& comment, const QString& aroundStrings, char floatformat) const {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    //QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    saveCSV(txt, userColumns, separator, decimal_separator, comment, aroundStrings, floatformat);
    f.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::saveMatlab(const QString& filename, const QSet<int>& userColumns) const {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    //QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    saveMatlab(txt, userColumns);
    f.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////
QStringList JKQTPDatastore::getColumnNames() const {
    QStringList names;
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    int col=0;
    while (it.hasNext()) {
        it.next();
        names.append(it.value()->getName());
        col++;
    }
    return names;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::saveMatlab(QTextStream &txt, const QSet<int>& userColumns) const {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QSet<QString> varnames;


    txt.setLocale(loc);
/*    if (comment.size()>0) {
        txt<<comment<<" ";
        bool first=true;
        QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
        while (it.hasNext()) {
            it.next();
            if (!first) txt<<separator;
            txt<<aroundStrings<<it.value().getName()<<aroundStrings;
            first=false;
        }
        txt<<"\n";
    }*/
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    int col=0;
    int i=0;
    while (it.hasNext()) {
        it.next();

        if (userColumns.isEmpty() || userColumns.contains(i)) {

            // convert the column name into a variable name (name cleanup!) If the column name is empty, we  use a default name ("column")
            QString newvarbase=jkqtp_to_valid_variable_name(it.value()->getName().toStdString()).c_str();
            if (newvarbase.isEmpty()) newvarbase="column";
            int cnt=1;
            QString newvar=newvarbase;
            // now we make sure that the name is not already used, i.e. we compare it to all variable names used so far and if it has already been used,
            // an increasing number is added to the name.
            while (varnames.contains(newvar)) {
                newvar=newvarbase+QString::number(cnt);
                cnt++;
            }
            varnames.insert(newvar);
            txt<<QString("% data from columne %1 ('%2')\n").arg(col+1).arg(it.value()->getName());
            txt<<QString("%1 = [ ").arg(newvar);
            for (size_t rr=0; rr<it.value()->getRows(); rr++) {
                txt<<loc.toString(get(it.key(), rr))<<" ";

            }
            txt<<"];\n\n";
            col++;
        }
        i++;
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::saveCSV(QTextStream& txt, const QSet<int>& userColumns, const QString& separator, const QString& decimal_separator, const QString& comment, const QString& aroundStrings, char floatformat) const {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    const auto dsep=loc.decimalPoint();


    txt.setLocale(loc);
    if (comment.size()>0) {
        txt<<comment<<" ";
        bool first=true;
        QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
        int i=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(i)) {
                if (!first) txt<<separator;
                txt<<aroundStrings<<it.value()->getName()<<aroundStrings;
                first=false;
            }
            i++;
        }
        txt<<"\n";
    }
    for (size_t i=0; i<getMaxRows(); i++) {
        bool first=true;
        QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
        int j=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(j)) {
                if (!first) txt<<separator;
                first=false;
                if (it.value()->getRows()>i) {
                    QString num=QString("%1").arg(get(it.key(), i), 0, floatformat);
                    num=num.replace(dsep, decimal_separator);
                    txt<<num;
                }
                //std::cout<<"("<<i<<", "<<c<<")\n";
            }
            j++;
        }
        txt<<"\n";
    }
    txt<<"\n";

}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::saveSYLK(const QString& filename, const QSet<int>& userColumns, const QString& floatformat) const {
    Q_UNUSED(floatformat)
    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    //QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    txt.setLocale(loc);
    // write SYLK header
    txt<<"ID;P\n";

    // write column headers
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            txt<<"C;Y1;X"<<c<<";K\""<<it.value()->getName()<<"\"\n";
            txt<<"F;Y1;X"<<c<<";SDB\n";
            c++;
        }
        i++;
    }



    for (size_t rr=0; rr<getMaxRows(); rr++) {
        QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
        size_t c=1;
        int i=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(static_cast<int>(i))) {
                if (it.value()->getRows()>rr) {
                    txt<<QString("C;X%1;Y%2;N;K%3\n").arg(c).arg(rr+2).arg(get(it.key(), rr));
                }
                c++;
            }
            i++;
        }
    }
    txt<<"E\n\n";
    f.close();



}

////////////////////////////////////////////////////////////////////////////////////////////////
QList<QVector<double> > JKQTPDatastore::getData(QStringList *columnNames, const QSet<int>& userColumns) const
{
    QStringList cl;
    QList<QVector<double> > res;

    // write column headers
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            QVector<double> dat;
            for (size_t i=0; i<it.value()->getRows(); i++) {
                dat<<it.value()->getValue(i);
            }

            cl<<it.value()->getName();
            res<<dat;
            c++;
        }
        i++;
    }

    if (columnNames) *columnNames=cl;

    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////
QVector<double> JKQTPDatastore::getData(size_t column, QString *columnName) const
{
    QVector<double> res;
    if (hasColumn(column)) {
        if (columnName) *columnName=columns[column]->getName();
        columns[column]->copyData(res);
    }
    return res;
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::saveDIF(const QString& filename, const QSet<int>& userColumns, const QString& floatformat) const {
    Q_UNUSED(floatformat)
    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    //QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    txt.setLocale(loc);
    const size_t N=getMaxRows();

    // write DIF header
    txt<<QString("TABLE\n0,1\n\"\"\n");
    if (userColumns.isEmpty()) txt<<QString("VECTORS\n0,%1\n\"\"\n").arg(columns.size());
    else txt<<QString("VECTORS\n0,%1\n\"\"\n").arg(userColumns.size());
    txt<<QString("TUPLES\n0,%1\n\"\"\n").arg(N);

    txt<<QString("-1,0\nBOT\n");

    // write column headers
    QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            txt<<QString("1,0\n\"%1\"\n").arg(it.value()->getName());
            c++;
        }
        i++;
    }



    for (size_t i=0; i<N; i++) {
        txt<<QString("-1,0\nBOT\n");
        QMapIterator<size_t, std::shared_ptr<JKQTPColumn>> it(columns);
        c=1;
        int j=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(j)) {
                //qDebug()<<i<<"/"<<N<<",   "<<c;
                if (i<it.value()->getRows()) {
                    txt<<QString("0,%1\nV\n").arg(get(it.key(), i));
                }
                c++;
            }
            j++;
        }
    }
    txt<<"-1,0\nEOT\n";
    f.close();


}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreModel::JKQTPDatastoreModel(JKQTPDatastore *datastore, QObject *parent):
    QAbstractTableModel(parent)
{
    this->datastore=datastore;
    reloadModel();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreModel::~JKQTPDatastoreModel()
= default;

////////////////////////////////////////////////////////////////////////////////////////////////
QVariant JKQTPDatastoreModel::data(const QModelIndex &index, int role) const {
    int row=index.row();
    int column=index.column();
    if (datastore) {
        if (role==Qt::DisplayRole || role==Qt::EditRole) {
            int col=static_cast<int>(datastore->getColumnIDsIntVec().value(column, -1));
            if (col>-1 && row>=0 && row<static_cast<int>(datastore->getRows(col))) {
                return datastore->get(col, static_cast<size_t>(row));
            }
        }
    }
    return QVariant();
}

////////////////////////////////////////////////////////////////////////////////////////////////
Qt::ItemFlags JKQTPDatastoreModel::flags(const QModelIndex &/*index*/) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

////////////////////////////////////////////////////////////////////////////////////////////////
QVariant JKQTPDatastoreModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (datastore) {
        if (role==Qt::DisplayRole) {
            if (orientation==Qt::Horizontal) {
                if (section>=0 && section<static_cast<int64_t>(datastore->getColumnCount())) {
                    //qDebug()<<"columns: "<<datastore->getColumnNames().size()<<datastore->getColumnIDs().size()<<": "<<datastore->getColumnNames();
                    return datastore->getColumnNames().value(section);
                }
            } else {
                return QString::number(section+1);
            }
        }
    }
    return QVariant();
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastoreModel::rowCount(const QModelIndex &/*parent*/) const {
    if (datastore) {
        return static_cast<int>(datastore->getMaxRows());
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastoreModel::columnCount(const QModelIndex &/*parent*/) const {
    if (datastore) {
        return static_cast<int>(datastore->getColumnCount());
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastoreModel::reloadModel()
{
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    beginResetModel();
    endResetModel();
    #else
    reset();
    #endif
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextHigherIndex(int column, size_t row) const {
    return getNextHigherIndex(static_cast<size_t>(column), row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnData(size_t toColumn, const QVector<double> &data)
{
    setColumnCopiedData(toColumn, data.data(), data.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnCopiedData(size_t toColumn, const double *data, size_t N)
{
    resizeColumn(toColumn, N);
    columns[toColumn]->copy(data, N);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnCopiedImageData(size_t toColumn, const double *data, size_t width, size_t height)
{
    setColumnCopiedData(toColumn, data, width*height);
    columns[toColumn]->setImageColumns(width);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumn(size_t column, double value)
{
    // quick guard: column must exist
    auto it=columns.find(column);
    if (it == columns.end()) {
        qDebug()<<"column "<<column<<" does not exist, so we cannnot add a value";
        return;
    }
    it.value()->append(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::resizeColumn(size_t column, size_t new_rows)
{
    if (columns[column]->getRows()==new_rows) return;
    columns[column]->resize(new_rows);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::resizeImageColumn(size_t column, size_t new_image_width, size_t new_image_height)
{
    resizeColumn(column, new_image_width*new_image_height);
    columns[column]->setImageColumns(new_image_width);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumns(size_t column1, size_t column2, double value1, double value2)
{
    appendToColumn(column1,value1);
    appendToColumn(column2,value2);
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumns(size_t columnX, size_t columnY, const QPointF& value)
{
    appendToColumn(columnX,value.x());
    appendToColumn(columnY,value.y());
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumns(size_t columnX, size_t columnY, const QPoint& value)
{
    appendToColumn(columnX,value.x());
    appendToColumn(columnY,value.y());
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumns(size_t column1, size_t column2, size_t column3, double value1, double value2, double value3)
{
    appendToColumn(column1,value1);
    appendToColumn(column2,value2);
    appendToColumn(column3,value3);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumns(size_t column1, size_t column2, size_t column3, size_t column4, double value1, double value2, double value3, double value4)
{
    appendToColumn(column1,value1);
    appendToColumn(column2,value2);
    appendToColumn(column3,value3);
    appendToColumn(column4,value4);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumns(size_t column1, size_t column2, size_t column3, size_t column4, size_t column5, double value1, double value2, double value3, double value4, double value5)
{
    appendToColumn(column1,value1);
    appendToColumn(column2,value2);
    appendToColumn(column3,value3);
    appendToColumn(column4,value4);
    appendToColumn(column5,value5);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::getColumnImageWidth(int column) const
{
    if (column<0) return 0;
    return columns[static_cast<size_t>(column)]->getImageColumns();
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::getColumnImageHeight(int column) const
{
    if (column<0) return 0;
    return columns[static_cast<size_t>(column)]->getRows()/columns[static_cast<size_t>(column)]->getImageColumns();
}
