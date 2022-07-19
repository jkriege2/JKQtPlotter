/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

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

/**************************************************************************************************************************
 * JKQTPColumn
 **************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn()
{
    datastore=nullptr;
    name="";
    datastoreItem=0;
    datastoreOffset=0;
    imageColumns=1;
    valid=false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::JKQTPColumn(JKQTPDatastore *datastore, const QString &name, size_t datastoreItem, size_t datastoreOffset, size_t imageColumns)
{
    this->datastore=datastore;
    this->datastoreItem=datastoreItem;
    this->datastoreOffset=datastoreOffset;
    this->imageColumns=imageColumns;
    this->name=name;
    valid=true;

}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::setName(const QString &__value)
{
    this->name = __value;
}

////////////////////////////////////////////////////////////////////////////////////////////////
QString JKQTPColumn::getName() const
{
    return this->name;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::setImageColumns(size_t imageWidth)
{
    imageColumns=imageWidth;
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPColumn::getRows() const {
    if (!valid || !datastore) return 0;
    JKQTPDatastoreItem* i=datastore->getItem(datastoreItem);
    if (i) return i->getRows();
    else return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::copyData(QVector<double> &copyTo) const
{
    const size_t cnt=getRows();
    if (cnt>0) {
        copyTo.resize(static_cast<int>(cnt));
        for (size_t i=0; i<cnt; i++) {
            copyTo[static_cast<int>(i)]=getValue(i);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
QVector<double> JKQTPColumn::copyData()
{
    QVector<double> d;
    copyData(d);
    return d;
}


////////////////////////////////////////////////////////////////////////////////////////////////
const double *JKQTPColumn::getPointer(size_t n) const
{
    if (!datastore) return nullptr;
    if (!datastore->getItem(datastoreItem)) return nullptr;
    return datastore->getItem(datastoreItem)->getPointer(datastoreOffset, n);
}

////////////////////////////////////////////////////////////////////////////////////////////////
double *JKQTPColumn::getPointer(size_t n)
{
    if (!datastore) return nullptr;
    if (!datastore->getItem(datastoreItem)) return nullptr;
    return datastore->getItem(datastoreItem)->getPointer(datastoreOffset, n);
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::copy(const double *data, size_t N, size_t offset) {
    if (!datastore) return ;
    JKQTPDatastoreItem* it=datastore->getItem(datastoreItem);
    if (!it) return;
    for (size_t i=0; i<N; i++) {
        it->set(datastoreOffset, i+offset, data[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::exchange(double value, double replace)
{
    if (!datastore) return ;
    for (size_t i=0; i<getRows(); i++) {
        double v=getValue(i);
        if (v==value) v=replace;
        setValue(i, v);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::subtract(double value)
{
    if (!datastore) return ;
    double* data=getPointer();
    const size_t N=getRows();
    if (data){
        for (size_t i=0; i<N; i++) {
            data[i]=data[i]-value;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::scale(double factor)
{
    if (!datastore) return ;
    double* data=getPointer();
    size_t N=getRows();
    if (data){
        for (size_t i=0; i<N; i++) {
            data[i]=data[i]*factor;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::setAll(double value)
{
    if (!datastore) return ;
    double* data=getPointer();
    size_t N=getRows();
    if (data){
        for (size_t i=0; i<N; i++) {
            data[i]=value;
        }
    }
}


/**************************************************************************************************************************
 * JKQTPDatastoreItem
 **************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreItem::JKQTPDatastoreItem(){
    this->dataformat=JKQTPDatastoreItemFormat::SingleColumn;
    this->allocated=false;
    this->data=nullptr;
    this->columns=0;
    this->rows=0;
    this->storageType=StorageType::Internal;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreItem::JKQTPDatastoreItem(size_t columns, size_t rows){
    this->storageType=StorageType::Internal;
    this->allocated=false;
    if (columns>1) {
        this->dataformat=JKQTPDatastoreItemFormat::MatrixRow;
        this->data=static_cast<double*>(calloc(columns*rows, sizeof(double)));
    } else {
        this->dataformat=JKQTPDatastoreItemFormat::SingleColumn;
        this->storageType=StorageType::Vector;
        datavec.resize(static_cast<int>(columns*rows));
        std::fill(datavec.begin(), datavec.end(), 0.0);
        this->data=datavec.data();
    }
    this->columns=columns;
    this->rows=rows;
    this->allocated=true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreItem::JKQTPDatastoreItem(const QVector<double>& data_)
{
    this->storageType=StorageType::Internal;
    this->allocated=false;
    this->dataformat=JKQTPDatastoreItemFormat::SingleColumn;
    this->storageType=StorageType::Vector;
    this->datavec=data_;
    this->data=datavec.data();
    this->columns=1;
    this->rows=static_cast<int>(data_.size());
    this->allocated=true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
bool JKQTPDatastoreItem::resizeColumns(size_t new_rows) {
    bool dataRetained=false;
    if (storageType==StorageType::Internal && allocated && data!=nullptr) {
        free(data);
        data=nullptr;
    }
    if (columns>1) {
        this->dataformat=JKQTPDatastoreItemFormat::MatrixRow;
        this->data=static_cast<double*>(calloc(columns*new_rows, sizeof(double)));
        storageType=StorageType::Internal;
    } else {
        this->dataformat=JKQTPDatastoreItemFormat::SingleColumn;
        this->storageType=StorageType::Vector;
        datavec.resize(static_cast<int>(columns*new_rows));
        if (new_rows>rows) {
            for( size_t i=rows; i<new_rows; i++) { datavec[static_cast<int>(i)]=0.0; }
        }
        this->data=datavec.data();
        dataRetained=true;
    }
    rows=new_rows;
    allocated=true;
    return dataRetained;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreItem::JKQTPDatastoreItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows){
    this->dataformat=dataformat;
    this->allocated=true;
    this->data=data;
    this->columns=columns;
    this->rows=rows;
    this->storageType=StorageType::External;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreItem::JKQTPDatastoreItem(JKQTPDatastoreItemFormat dataformat, double *data, size_t columns, size_t rows, bool internal)
{
    this->dataformat=dataformat;
    this->allocated=true;
    this->data=data;
    this->columns=columns;
    this->rows=rows;
    this->storageType=internal?StorageType::Internal:StorageType::External;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastoreItem::~JKQTPDatastoreItem(){
    if (storageType==StorageType::Internal && allocated && data!=nullptr) {
        free(data);
        data=nullptr;
    }
}



/**************************************************************************************************************************
 * JKQTPDatastore
 **************************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addItem(JKQTPDatastoreItem* item) {
    /*items.push_back(item);
    return items.size()-1;*/
    items.insert(maxItemID, item);
    maxItemID++;
    return maxItemID-1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(JKQTPColumn col) {
    columns.insert(maxColumnsID, col);
    maxColumnsID++;
    return maxColumnsID-1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastore::JKQTPDatastore():
    m_invalidColumn(new JKQTPColumn)
{
    maxItemID=0;
    maxColumnsID=0;
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
JKQTPDatastore::ColumnIterator JKQTPDatastore::begin()
{
    return columns.begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastore::ColumnIterator JKQTPDatastore::end()
{
    return columns.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastore::ConstColumnIterator JKQTPDatastore::begin() const
{
    return columns.begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPDatastore::ConstColumnIterator JKQTPDatastore::end() const
{
    return columns.end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::begin(int i)
{
    if (i<0) return m_invalidColumn->end();
    auto it=columns.find(static_cast<size_t>(i));
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::end(int i)
{
    if (i<0) return m_invalidColumn->end();
    auto it=columns.find(static_cast<size_t>(i));
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::begin(int i) const
{
    if (i<0) return m_invalidColumn->end();
    auto it=columns.find(static_cast<size_t>(i));
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::end(int i) const
{
    if (i<0) return m_invalidColumn->end();
    auto it=columns.find(static_cast<size_t>(i));
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::begin(size_t i)
{
    auto it=columns.find(i);
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnIterator JKQTPDatastore::end(size_t i)
{
    auto it=columns.find(i);
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->end();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::begin(size_t i) const
{
    auto it=columns.find(i);
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumnConstIterator JKQTPDatastore::end(size_t i) const
{
    auto it=columns.find(i);
    if (it==columns.end()) return m_invalidColumn->end();
    else return it->end();
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
    maxItemID=0;
    maxColumnsID=0;
    /*if (items.size()>0) {
        for (size_t i=0; i<items.size(); i++) {
            delete items[i];
        }
    }*/
    QMapIterator<size_t, JKQTPDatastoreItem*> it(items);
    while (it.hasNext()) {
        it.next();
        delete it.value();
    }
    items.clear();
    columns.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::deleteAllColumns(const QString& name, bool removeItems) {
    QList<size_t> ids;
    QMapIterator<size_t, JKQTPColumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().getName()==name) {
            ids.append(it.key());
        }
    }
    for (int i=0; i<ids.size(); i++) {
        deleteColumn(ids[i], removeItems);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::deleteAllPrefixedColumns(QString prefix, bool removeItems) {
    QList<size_t> ids;
    QMapIterator<size_t, JKQTPColumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().getName().startsWith(prefix)) {
            ids.append(it.key());
        }
    }
    for (int i=0; i<ids.size(); i++) {
        deleteColumn(ids[i], removeItems);
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////
quint16 JKQTPDatastore::getColumnChecksum(int column) const
{
    return columns.value(column, JKQTPColumn()).calculateChecksum();
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::deleteColumn(size_t column, bool removeItems) {
    if (removeItems) {
        size_t dsitem=columns[column].getDatastoreItemNum();
        int cnt=0;
        QMapIterator<size_t, JKQTPColumn> it(columns);
        while (it.hasNext()) {
            it.next();
            if (it.value().getDatastoreItemNum()==dsitem) {
                cnt++;
            }
        }
        if (cnt<=1) {
            delete items[dsitem];
            items.remove(dsitem);
        }
    }
    columns.remove(column);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getColumnNum(const QString& name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, JKQTPColumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().getName()==name) return static_cast<int>(it.key());
    }
    return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
QString JKQTPDatastore::getColumnName(size_t column)
{
    auto it=columns.find(column);

    if (it==columns.end()) return "";
    else return it->getName();
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::ensureColumnNum(const QString& name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, JKQTPColumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().getName()==name) return static_cast<int>(it.key());
    }
    return static_cast<int>(addColumn(0, name));
}


////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCopiedItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columnsnum, size_t rows) {
    JKQTPDatastoreItem* it=nullptr;
    if ((dataformat==JKQTPDatastoreItemFormat::SingleColumn)||(columnsnum==1)) {
        return addCopiedItem(data, rows);
    } else if (dataformat==JKQTPDatastoreItemFormat::MatrixColumn) {
        it=new JKQTPDatastoreItem(columnsnum, rows);
        for (size_t c=0; c<columnsnum; c++) {
            for (size_t r=0; r<rows; r++) {
                it->set(c, r, data[c*rows+r]);
            }
        }
    } else if (dataformat==JKQTPDatastoreItemFormat::MatrixRow) {
        it=new JKQTPDatastoreItem(columnsnum, rows);
        for (size_t r=0; r<rows; r++) {
            for (size_t c=0; c<columnsnum; c++) {
                it->set(c, r, data[r*columnsnum+c]);
            }
        }
    }
    /*items.push_back(it);
    return items.size()-1;*/
    return addItem(it);
};



////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addItem(size_t rows) {
    /*items.push_back(new JKQTPDatastoreItem(1, rows));
    return items.size()-1;*/
    return addItem(new JKQTPDatastoreItem(1, rows));
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addItem(size_t columnsnum, size_t rows) {
    /*items.push_back(new JKQTPDatastoreItem(columnsnum, rows));
    return items.size()-1;*/
    return addItem(new JKQTPDatastoreItem(columnsnum, rows));
};

/** \brief add one external column to the datastore. It contains \a rows rows.*/
size_t JKQTPDatastore::addItem(double* data, size_t rows) {
    /*items.push_back(new JKQTPDatastoreItem(JKQTPSingleColumn, data, 1, rows));
    return items.size()-1;*/
    return addItem(new JKQTPDatastoreItem(JKQTPDatastoreItemFormat::SingleColumn, data, 1, rows));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addInternalItem(double *data, size_t rows)
{
    JKQTPDatastoreItem* dsi=new JKQTPDatastoreItem(JKQTPDatastoreItemFormat::SingleColumn, data, 1, rows, true);
    return addItem(dsi);
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columnsnum, size_t rows) {
    /*items.push_back(new JKQTPDatastoreItem(dataformat, data, columnsnum, rows));
    return items.size()-1;*/
    return addItem(new JKQTPDatastoreItem(dataformat, data, columnsnum, rows));
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCopiedItem(const double* data, size_t rows) {
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, rows);
    if (data) {
        for (size_t i=0; i<rows; i++) {
            it->set(0, i, data[i]);
             //std::cout<<"copy@"<<i<<" = "<<data[i]<<std::endl;
        }
    }
    /*items.push_back(it);
    return items.size()-1;*/
    return addItem(it);
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumnForItem(size_t itemID, size_t columnInItem, const QString& name) {
    return addColumn(JKQTPColumn(this, name, itemID, columnInItem));
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(size_t rows, const QString& name) {
    size_t item= addItem(new JKQTPDatastoreItem(1, rows));
    return addColumnForItem(item, 0, name);
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(const QString& name) {
    return addColumn(0, name);
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumn(double* data, size_t rows, const QString& name) {
    size_t it=addItem(new JKQTPDatastoreItem(JKQTPDatastoreItemFormat::SingleColumn, data, 1, rows));
    return addColumnForItem(it, 0, name);
};

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addInternalColumn(double* data, size_t rows, const QString& name) {
    size_t it=addItem(new JKQTPDatastoreItem(JKQTPDatastoreItemFormat::SingleColumn, data, 1, rows,true));
    return addColumnForItem(it, 0, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addImageColumn(size_t width, size_t height, const QString &name)
{
    size_t col= addColumn(width*height, name);
    columns[col].setImageColumns(width);
    return col;
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addImageColumn(double *data, size_t width, size_t height, const QString &name)
{
    size_t col= addColumn(data, width*height, name);
    columns[col].setImageColumns(width);
    return col;
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addInternalImageColumn(double *data, size_t width, size_t height, const QString &name)
{
    size_t col= addInternalColumn(data, width*height, name);
    columns[col].setImageColumns(width);
    return col;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnImageWidth(size_t column, size_t imageWidth)
{
    columns[column].setImageColumns(imageWidth);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnImageHeight(size_t column, size_t imageHeight)
{
    columns[column].setImageColumns(columns[column].getRows()/imageHeight);
}








////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::copyColumn(size_t old_column, size_t start, size_t stride, const QString& name)
{
    JKQTPColumn old=columns[old_column];
    size_t rows=old.getRows();
    QVector<double> d(rows, 0.0);
    double* dd=old.getPointer(0);
    size_t j=0;
    for (size_t i=start; i<rows; i+=stride) {
        d[j]=dd[i];
        //qDebug()<<old_column<<name<<": "<<j<<i<<d[j];
        j++;
    }
    size_t n=addCopiedColumn(d.data(), j, name);
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
    setColumnImageWidth(toColumn, getColumnImageWidth(fromColumn));
}


////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addLinearColumn(size_t rows, double start, double end, const QString& name) {
    double delta=(end-start)/static_cast<double>(rows-1);
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, rows);
    for (size_t i=0; i<rows; i++) {
        it->set(0, i, start+static_cast<double>(i) * delta);
         //std::cout<<"copy@"<<i<<" = "<<data[i]<<std::endl;
    }
    /*items.push_back(it);
    size_t itemid=items.size()-1;*/
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addLogColumn(size_t rows, double start, double end, const QString &name)
{
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, rows);
    const double x0=log(start)/JKQTPSTATISTICS_LN10;
    const double x1=log(end)/JKQTPSTATISTICS_LN10;
    for (size_t i=0; i<rows; i++) {
        it->set(0, i, pow(10.0, x0+static_cast<double>(i)/static_cast<double>(rows-1)*(x1-x0)));
    }
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addDecadeLogColumn(size_t rows, double startDecade, double endDecade, const QString &name)
{
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, rows);
    const double x0=startDecade;
    const double x1=endDecade;
    for (size_t i=0; i<rows; i++) {
        it->set(0, i, pow(10.0, x0+static_cast<double>(i)/static_cast<double>(rows-1)*(x1-x0)));
    }
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
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
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, rows);
    for (size_t i=0; i<rows; i++) {
        it->set(0, i, f(i, this));
    }
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCalculatedColumn(size_t rows, const std::function<double (size_t)> &f, const QString &name)
{
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, rows);
    for (size_t i=0; i<rows; i++) {
        it->set(0, i, f(i));
    }
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addCalculatedImageColumn(size_t cols, size_t rows, const std::function<double (size_t, size_t)> &f, const QString &name)
{
    size_t col=addImageColumn(cols, rows, name);
    JKQTPColumn& colref = columns[col];
    for (size_t y=0; y<rows; y++) {
        for (size_t x=0; x<cols; x++) {
            colref.setPixelValue(x, y, f(x,y));
        }
    }
    return col;

}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumnCalculatedFromColumn(size_t otherColumn, const std::function<double (double)> &f, const QString &name)
{
    const JKQTPColumn& oc=columns.value(otherColumn);
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, oc.getRows());
    for (size_t i=0; i<oc.getRows(); i++) {
        it->set(0, i, f(oc.getValue(i)));
    }
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::addColumnCalculatedFromColumn(size_t otherColumnX, size_t otherColumnY, const std::function<double (double, double)> &f, const QString &name)
{
    const JKQTPColumn& ocx=columns.value(otherColumnX);
    const JKQTPColumn& ocy=columns.value(otherColumnY);
    const size_t N= qMin(ocx.getRows(), ocy.getRows());
    JKQTPDatastoreItem* it=new JKQTPDatastoreItem(1, N);
    for (size_t i=0; i<N; i++) {
        it->set(0, i, f(ocx.getValue(i), ocy.getValue(i)));
    }
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

////////////////////////////////////////////////////////////////////////////////////////////////
QVector<int> JKQTPDatastore::getColumnIDsIntVec() const {
    QVector<int> ks;
    for (const size_t& k: columns.keys()) {
        ks<<static_cast<int>(k);
    }
    return ks;
}


////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextLowerIndex(size_t column, size_t row, int start, int end) const
{
    const JKQTPColumn& col=columns[column];
    if (start<0 && end>=0) return getNextLowerIndex(column, row, 0, end);
    else if (start>=0 && end<0)  return getNextLowerIndex(column, row, start, static_cast<int>(col.getRows())-1);
    else if (start<0 && end<0)  return getNextLowerIndex(column, row, 0, static_cast<int>(col.getRows())-1);
    else {
        double d=0;
        const double v=col.getValue(row);
        int res=-1;
        for ( int i=start; i<=end; i++) {
            if (i!=static_cast<int>(row)) {
                const double v1=col.getValue(i);
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
    return getNextLowerIndex(column, row, 0, static_cast<int>(columns[column].getRows())-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
int JKQTPDatastore::getNextHigherIndex(size_t column, size_t row, int start, int end) const
{
    const JKQTPColumn& col=columns[column];
    if (start<0 && end>=0) return getNextHigherIndex(column, row, 0, end);
    else if (start>=0 && end<0)  return getNextHigherIndex(column, row, start, static_cast<int>(col.getRows())-1);
    else if (start<0 && end<0)  return getNextHigherIndex(column, row, 0, static_cast<int>(col.getRows())-1);
    else {
        double d=0;
        const double v=col.getValue(row);
        int res=-1;
        for ( int i=start; i<=end; i++) {
            if (i!=static_cast<int>(row)) {
                const double v1=col.getValue(i);
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
    return getNextHigherIndex(column, row, 0, static_cast<int>(columns[column].getRows())-1);
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
size_t JKQTPDatastore::getMaxRows() const {
    size_t res=0;
    /*for (size_t i=0; i<columns.size(); i++) {
        size_t r=columns[i].getRows();
        if (r>res) res=r;
    }*/
    QMapIterator<size_t, JKQTPColumn> it(columns);
    while (it.hasNext()) {
        it.next();
        size_t r=it.value().getRows();
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
    QMapIterator<size_t, JKQTPColumn> it(columns);
    int col=0;
    while (it.hasNext()) {
        it.next();
        names.append(it.value().getName());
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
        QMapIterator<size_t, JKQTPColumn> it(columns);
        while (it.hasNext()) {
            it.next();
            if (!first) txt<<separator;
            txt<<aroundStrings<<it.value().getName()<<aroundStrings;
            first=false;
        }
        txt<<"\n";
    }*/
    QMapIterator<size_t, JKQTPColumn> it(columns);
    int col=0;
    int i=0;
    while (it.hasNext()) {
        it.next();

        if (userColumns.isEmpty() || userColumns.contains(i)) {

            // convert the column name into a variable name (name cleanup!) If the column name is empty, we  use a default name ("column")
            QString newvarbase=jkqtp_to_valid_variable_name(it.value().getName().toStdString()).c_str();
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
            txt<<QString("% data from columne %1 ('%2')\n").arg(col+1).arg(it.value().getName());
            txt<<QString("%1 = [ ").arg(newvar);
            for (size_t rr=0; rr<it.value().getRows(); rr++) {
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
        QMapIterator<size_t, JKQTPColumn> it(columns);
        int i=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(i)) {
                if (!first) txt<<separator;
                txt<<aroundStrings<<it.value().getName()<<aroundStrings;
                first=false;
            }
            i++;
        }
        txt<<"\n";
    }
    for (size_t i=0; i<getMaxRows(); i++) {
        bool first=true;
        QMapIterator<size_t, JKQTPColumn> it(columns);
        int j=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(j)) {
                if (!first) txt<<separator;
                first=false;
                if (it.value().getRows()>i) {
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
    QMapIterator<size_t, JKQTPColumn> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            txt<<"C;Y1;X"<<c<<";K\""<<it.value().getName()<<"\"\n";
            txt<<"F;Y1;X"<<c<<";SDB\n";
            c++;
        }
        i++;
    }



    for (size_t rr=0; rr<getMaxRows(); rr++) {
        QMapIterator<size_t, JKQTPColumn> it(columns);
        c=1;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(static_cast<int>(rr))) {
                if (it.value().getRows()>rr) {
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
    QMapIterator<size_t, JKQTPColumn> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            QVector<double> dat;
            for (size_t i=0; i<it.value().getRows(); i++) {
                dat<<it.value().getValue(i);
            }

            cl<<it.value().getName();
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
        if (columnName) *columnName=columns.value(column).getName();
        columns.value(column).copyData(res);
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
    const size_t rows=getMaxRows();

    // write DIF header
    txt<<QString("TABLE\n0,1\n\"\"\n");
    if (userColumns.isEmpty()) txt<<QString("VECTORS\n0,%1\n\"\"\n").arg(columns.size());
    else txt<<QString("VECTORS\n0,%1\n\"\"\n").arg(userColumns.size());
    txt<<QString("TUPLES\n0,%1\n\"\"\n").arg(rows);

    txt<<QString("-1,0\nBOT\n");

    // write column headers
    QMapIterator<size_t, JKQTPColumn> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            txt<<QString("1,0\n\"%1\"\n").arg(columns[c].getName());
            c++;
        }
        i++;
    }



    for (size_t i=0; i<rows; i++) {
        txt<<QString("-1,0\nBOT\n");
        QMapIterator<size_t, JKQTPColumn> it(columns);
        c=1;
        int j=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(j)) {
                //qDebug()<<i<<"/"<<rows<<",   "<<c;
                if (i<it.value().getRows()) {
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
    columns[toColumn].copy(data, N);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setColumnCopiedImageData(size_t toColumn, const double *data, size_t width, size_t height)
{
    setColumnCopiedData(toColumn, data, width*height);
    columns[toColumn].setImageColumns(width);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumn(size_t column, double value)
{
    const bool ok=columns[column].getDatastoreItem()->append(columns[column].getDatastoreOffset(), value);
    if (!ok) {
        QVector<double> old_data=columns[column].copyData();
        size_t itemID=addItem(new JKQTPDatastoreItem(1, static_cast<size_t>(old_data.size()+1)));
        columns[column]=JKQTPColumn(this, columns[column].getName(), itemID, 0);
        for (int i=0; i<old_data.size(); i++) {
            columns[column].setValue(static_cast<size_t>(i), old_data[i]);
        }
        columns[column].setValue(static_cast<size_t>(old_data.size()), value);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::resizeColumn(size_t column, size_t new_rows)
{
    if (columns[column].getRows()==new_rows) return;
    const bool ok=columns[column].getDatastoreItem()->isVector();
    if (!ok) {
        QVector<double> old_data=columns[column].copyData();
        size_t itemID=addItem(new JKQTPDatastoreItem(1, static_cast<size_t>(old_data.size()+1)));
        columns[column]=JKQTPColumn(this, columns[column].getName(), itemID, 0);
        for (int i=0; i<old_data.size(); i++) {
            columns[column].setValue(static_cast<size_t>(i), old_data[i]);
        }
    }
    columns[column].getDatastoreItem()->resize(new_rows);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::resizeImageColumn(size_t column, size_t new_image_width, size_t new_image_height)
{
    resizeColumn(column, new_image_width*new_image_height);
    columns[column].setImageColumns(new_image_width);
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
    return columns[static_cast<size_t>(column)].getImageColumns();
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::getColumnImageHeight(int column) const
{
    if (column<0) return 0;
    return columns[static_cast<size_t>(column)].getRows()/columns[static_cast<size_t>(column)].getImageColumns();
}
