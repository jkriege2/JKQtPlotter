/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    last modification: $LastChangedDate$  (revision $Rev$)

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/




/** \file jkqtpdatastorage.cpp
  * \ingroup jkqtpdatastorage
  */

#include "jkqtpdatastorage.h"
#include <QDebug>
#include <QtGlobal>


/**************************************************************************************************************************
 * JKQTPcolumn
 **************************************************************************************************************************/
unsigned long long JKQTPcolumn::getRows() const {
    if (!valid || !datastore) return 0;
    JKQTPdatastoreItem* i=datastore->getItem(datastoreItem);
    if (i) return i->get_rows();
    else return 0;
}

void JKQTPcolumn::copyData(QVector<double> &copyTo) const
{
    double* d=getPointer(0);
    copyTo.clear();
    unsigned long long i, cnt=getRows();
    if (cnt>0) {
        copyTo.resize(cnt);
        for (i=0; i<cnt; i++) {
            copyTo[i]=d[i];
        }
    }
}

QVector<double> JKQTPcolumn::copyData()
{
    QVector<double> d;
    copyData(d);
    return d;
};

/** \brief reads the \a n'th value from the column
 *
 * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
 * column.
 */
double JKQTPcolumn::getValue(unsigned long long n) const {
    if (!datastore) return 0;
    if (!datastore->getItem(datastoreItem)) return 0;
    return datastore->getItem(datastoreItem)->get(datastoreOffset, n);
}

double *JKQTPcolumn::getPointer(unsigned long long n) const
{
    if (!datastore) return 0;
    if (!datastore->getItem(datastoreItem)) return 0;
    return datastore->getItem(datastoreItem)->getPointer(datastoreOffset, n);
};

/** \brief sets the \a n'th value from the column
 *
 * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
 * column.
 */
void JKQTPcolumn::setValue(unsigned long long n, double val) {
    if (!datastore) return ;
    if (!datastore->getItem(datastoreItem))  return;
    datastore->getItem(datastoreItem)->set(datastoreOffset, n, val);
};

void JKQTPcolumn::copy(double* data, unsigned long long N, unsigned long long offset) {
    if (!datastore) return ;
    JKQTPdatastoreItem* it=datastore->getItem(datastoreItem);
    if (!it) return;
    for (unsigned long long i=0; i<N; i++) {
        it->set(datastoreOffset, i+offset, data[i]);
    }
}

void JKQTPcolumn::exchange(double value, double replace)
{
    if (!datastore) return ;
    for (unsigned long long i=0; i<getRows(); i++) {
        double v=getValue(i);
        if (v==value) v=replace;
        setValue(i, v);
    }
}

void JKQTPcolumn::subtract(double value)
{
    if (!datastore) return ;
    double* data=getPointer();
    unsigned long long N=getRows();
    if (data){
        for (unsigned long long i=0; i<N; i++) {
            data[i]=data[i]-value;
        }
    }
}

void JKQTPcolumn::scale(double factor)
{
    if (!datastore) return ;
    double* data=getPointer();
    unsigned long long N=getRows();
    if (data){
        for (unsigned long long i=0; i<N; i++) {
            data[i]=data[i]*factor;
        }
    }
}


/**************************************************************************************************************************
 * JKQTPdatastoreItem
 **************************************************************************************************************************/
JKQTPdatastoreItem::JKQTPdatastoreItem(){
    this->dataformat=JKQTPsingleColumn;
    this->allocated=false;
    this->data=NULL;
    this->columns=0;
    this->rows=0;
    this->internal=true;
}

JKQTPdatastoreItem::JKQTPdatastoreItem(size_t columns, unsigned long long rows){
    this->internal=true;
    this->allocated=false;
    if (columns>1) {
        this->dataformat=JKQTPmatrixRow;
        this->data=(double*)calloc(columns*rows, sizeof(double));
    } else {
        this->dataformat=JKQTPsingleColumn;
        this->data=(double*)calloc(rows, sizeof(double));
    }
    this->columns=columns;
    this->rows=rows;
    this->allocated=true;
}

void JKQTPdatastoreItem::resizeColumns(unsigned long long new_rows) {
    if (internal && allocated && data!=NULL) {
        free(data);
        data=NULL;
    }
    data=(double*)calloc(columns*new_rows, sizeof(double));
    rows=new_rows;
    internal=true;
    allocated=true;
}

JKQTPdatastoreItem::JKQTPdatastoreItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columns, unsigned long long rows){
    this->dataformat=dataformat;
    this->allocated=true;
    this->data=data;
    this->columns=columns;
    this->rows=rows;
    this->internal=false;
}

JKQTPdatastoreItem::JKQTPdatastoreItem(JKQTPdatastoreItemFormat dataformat, double *data, size_t columns, unsigned long long rows, bool internal)
{
    this->dataformat=dataformat;
    this->allocated=true;
    this->data=data;
    this->columns=columns;
    this->rows=rows;
    this->internal=internal;
}

JKQTPdatastoreItem::~JKQTPdatastoreItem(){
    if (internal && allocated && data!=NULL) {
        free(data);
        data=NULL;
    }
}



/**************************************************************************************************************************
 * JKQTPdatastore
 **************************************************************************************************************************/

size_t JKQTPdatastore::addItem(JKQTPdatastoreItem* item) {
    /*items.push_back(item);
    return items.size()-1;*/
    items.insert(maxItemID, item);
    maxItemID++;
    return maxItemID-1;
}

size_t JKQTPdatastore::addColumn(JKQTPcolumn col) {
    columns.insert(maxColumnsID, col);
    maxColumnsID++;
    return maxColumnsID-1;
}

JKQTPdatastore::JKQTPdatastore()
{
    maxItemID=0;
    maxColumnsID=0;
    clear();
}

void JKQTPdatastore::clear(){
    maxItemID=0;
    maxColumnsID=0;
    /*if (items.size()>0) {
        for (size_t i=0; i<items.size(); i++) {
            delete items[i];
        }
    }*/
    QMapIterator<size_t, JKQTPdatastoreItem*> it(items);
    while (it.hasNext()) {
        it.next();
        delete it.value();
    }
    items.clear();
    columns.clear();
}

void JKQTPdatastore::deleteAllColumns(QString name, bool removeItems) {
    QList<size_t> ids;
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().get_name()==name) {
            ids.append(it.key());
        }
    }
    for (int i=0; i<ids.size(); i++) {
        deleteColumn(ids[i], removeItems);
    }
}

void JKQTPdatastore::deleteAllPrefixedColumns(QString prefix, bool removeItems) {
    QList<size_t> ids;
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().get_name().startsWith(prefix)) {
            ids.append(it.key());
        }
    }
    for (int i=0; i<ids.size(); i++) {
        deleteColumn(ids[i], removeItems);
    }

}

void JKQTPdatastore::deleteColumn(size_t column, bool removeItems) {
    if (removeItems) {
        size_t dsitem=columns[column].get_datastoreItem();
        int cnt=0;
        QMapIterator<size_t, JKQTPcolumn> it(columns);
        while (it.hasNext()) {
            it.next();
            if (it.value().get_datastoreItem()==dsitem) {
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

int JKQTPdatastore::getColumnNum(QString name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().get_name()==name) return it.key();
    }
    return -1;
}

size_t JKQTPdatastore::ensureColumnNum(QString name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().get_name()==name) return it.key();
    }
    return addColumn(0, name);
}

size_t JKQTPdatastore::addCopiedItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columnsnum, unsigned long long rows) {
    JKQTPdatastoreItem* it=NULL;
    if ((dataformat==JKQTPsingleColumn)||(columnsnum==1)) {
        return addCopiedItem(data, rows);
    } else if (dataformat==JKQTPmatrixColumn) {
        it=new JKQTPdatastoreItem(columnsnum, rows);
        for (size_t c=0; c<columnsnum; c++) {
            for (unsigned long long r=0; r<rows; r++) {
                it->set(c, r, data[c*rows+r]);
            }
        }
    } else if (dataformat==JKQTPmatrixColumn) {
        it=new JKQTPdatastoreItem(columnsnum, rows);
        for (unsigned long long r=0; r<rows; r++) {
            for (size_t c=0; c<columnsnum; c++) {
                it->set(c, r, data[r*columnsnum+c]);
            }
        }
    }
    /*items.push_back(it);
    return items.size()-1;*/
    return addItem(it);
};



/** \brief add a new columns/item with \a rows rows to the datastore and return its ID. The item uses internal memory management. */
size_t JKQTPdatastore::addItem(unsigned long long rows) {
    /*items.push_back(new JKQTPdatastoreItem(1, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(1, rows));
};

/** \brief add a new item with \a rows rows and \a columns columns to the datastore and return its ID. The item uses internal memory management. */
size_t JKQTPdatastore::addItem(size_t columnsnum, unsigned long long rows) {
    /*items.push_back(new JKQTPdatastoreItem(columnsnum, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(columnsnum, rows));
};

/** \brief add one external column to the datastore. It contains \a rows rows.*/
size_t JKQTPdatastore::addItem(double* data, unsigned long long rows) {
    /*items.push_back(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
}

size_t JKQTPdatastore::addInternalItem(double *data, unsigned long long rows)
{
    JKQTPdatastoreItem* dsi=new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows, true);
    return addItem(dsi);
};

/** \brief add an external memory block to the datastore. It contains \a rows rows and \a columns columns. \a dataformat determined the memory layout*/
size_t JKQTPdatastore::addItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columnsnum, unsigned long long rows) {
    /*items.push_back(new JKQTPdatastoreItem(dataformat, data, columnsnum, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(dataformat, data, columnsnum, rows));
};

/** \brief add one external array to the datastore. It contains \a rows rows. The data is copied and the copy managed internally */
size_t JKQTPdatastore::addCopiedItem(const double* data, unsigned long long rows) {
    JKQTPdatastoreItem* it=new JKQTPdatastoreItem(1, rows);
    if (data) {
        for (unsigned long long i=0; i<rows; i++) {
            it->set(0, i, data[i]);
             //std::cout<<"copy@"<<i<<" = "<<data[i]<<std::endl;
        }
    }
    /*items.push_back(it);
    return items.size()-1;*/
    return addItem(it);
};

/** \brief add a new columns which references a specified item and a specified column therein. */
size_t JKQTPdatastore::addColumnForItem(size_t itemID, size_t columnInItem, QString name) {
    /*JKQTPcolumn c(this, name, itemID, columnInItem);
    columns.push_back(c);
    return columns.size()-1;*/
    return addColumn(JKQTPcolumn(this, name, itemID, columnInItem));
};

/** \brief add a new columns with \a rows rows to the datastore and return its column ID. The new item uses internal memory management. */
size_t JKQTPdatastore::addColumn(unsigned long long rows, QString name) {
    //items.push_back(new JKQTPdatastoreItem(1, rows));
    //return addColumnForItem(items.size()-1, 0, name);
    size_t item= addItem(new JKQTPdatastoreItem(1, rows));
    return addColumnForItem(item, 0, name);
};

/** \brief add one external column to the datastore. It contains \a rows rows. This returns its logical column ID.*/
size_t JKQTPdatastore::addColumn(double* data, unsigned long long rows, QString name) {
    //items.push_back(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    //std::cout<<"added item\n";
    //size_t it=items.size()-1;
    //std::cout<<"adding column\n";
    size_t it=addItem(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    return addColumnForItem(it, 0, name);
};
/** \brief add one external column to the datastore. It contains \a rows rows. This returns its logical column ID.*/
size_t JKQTPdatastore::addInternalColumn(double* data, unsigned long long rows, QString name) {
    //items.push_back(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    //std::cout<<"added item\n";
    //size_t it=items.size()-1;
    //std::cout<<"adding column\n";
    size_t it=addItem(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows,true));
    return addColumnForItem(it, 0, name);
};

/** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
 *         afterwards you can delete the external arrayThis returns its logical column ID.*/
size_t JKQTPdatastore::addCopiedColumn(const double* data, unsigned long long rows, QString name) {
    size_t itemid=addCopiedItem(data, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const double *data, unsigned long long rows, unsigned long long stride, QString name)
{
    double* d=(double*)malloc(rows*sizeof(double));
    for (unsigned long long i=0; i<rows; i++) {
        d[i]=data[i*stride];
    }
    return addInternalColumn(d, rows, name);
}

size_t JKQTPdatastore::addCopiedColumn(const QVector<double> &data, QString name)
{
    return addCopiedColumn(data.data(), data.size(), name);
}

size_t JKQTPdatastore::addCopiedColumn(const QVector<double> &data, QString name, unsigned long long stride)
{
    return addCopiedColumn(data.data(), data.size(), stride, name);
}

size_t JKQTPdatastore::addCopiedColumn(const QVector<float> &data, QString name)
{
    return addCopiedColumn(data.data(), data.size(), name);
}



size_t JKQTPdatastore::copyColumn(size_t old_column, unsigned long long start, unsigned long long stride, QString name)
{
    JKQTPcolumn old=columns[old_column];
    unsigned long long rows=old.getRows();
    double* d=(double*)malloc(rows*sizeof(double));
    double* dd=old.getPointer(0);
    int j=0;
    for (unsigned long long i=start; i<rows; i+=stride) {
        d[j]=dd[i];
        //qDebug()<<old_column<<name<<": "<<j<<i<<d[j];
        j++;
    }
    size_t n=addCopiedColumn(d, j, name);
    free(d);
    return n;
}

size_t JKQTPdatastore::addCopiedColumn(const bool *data, unsigned long long rows, QString name) {
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=(data[r])?1.0:0.0;
        }
    }
    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const uint8_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }

    }
    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const uint16_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }
    }

    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const uint32_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }
    }

    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const int8_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }
    }


    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const int16_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }
    }

    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const int32_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }
    }

    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const int64_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }
    }

    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const uint64_t *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    for (unsigned long long r=0; r<rows; r++) {
        d[r]=data[r];
    }
    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumn(const float *data, unsigned long long rows, QString name)
{
    double* d=(double*)calloc(rows, sizeof(double));
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            d[r]=data[r];
        }
    }

    size_t itemid=addInternalItem(d, rows);
    return addColumnForItem(itemid, 0, name);
}

size_t JKQTPdatastore::addCopiedColumnMasked(const float *data, const bool *mask, unsigned long long rows, QString name, bool useIfMaskEquals)
{
    double* d=(double*)calloc(rows, sizeof(double));
    int rrs=0;
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            if (!mask || (mask && (mask[r]==useIfMaskEquals))) {
                d[rrs]=data[r];
                rrs++;
            }
        }
    }


    size_t col= addCopiedColumn(d, rrs, name);
    free(d);
    return col;
}

size_t JKQTPdatastore::addCopiedColumnMasked(const double *data, const bool *mask, unsigned long long rows, QString name, bool useIfMaskEquals)
{
    double* d=(double*)calloc(rows, sizeof(double));
    int rrs=0;
    if (data) {
        for (unsigned long long r=0; r<rows; r++) {
            if (!mask || (mask && (mask[r]==useIfMaskEquals))) {
                d[rrs]=data[r];
                rrs++;
            }
        }
    }


    size_t col= addCopiedColumn(d, rrs, name);
    free(d);
    return col;
}

size_t JKQTPdatastore::addCopiedImageAsColumn(double *data, unsigned long long width, unsigned long long height, QString name)
{
    return addCopiedColumn(data, width*height, name);
}

size_t JKQTPdatastore::addCopiedImageAsColumn(bool *data, unsigned long long width, unsigned long long height, QString name)
{
    return addCopiedColumn(data, width*height, name);
}

size_t JKQTPdatastore::addCopiedImageAsColumnTranspose(double *data, unsigned long long width, unsigned long long height, QString name)
{
    double* temp=(double*)malloc(width*height*sizeof(double));

    for (unsigned long long x=0; x<width; x++) {
        for (unsigned long long y=0; y<height; y++) {
            temp[x*height+y]=data[y*width+x];
        }

    }

    size_t idx=addCopiedColumn(temp, width*height, name);
    free(temp);
    return idx;
}

size_t JKQTPdatastore::addLinearColumn(unsigned long long rows, double start, double end, QString name) {
    double delta=(end-start)/(double)(rows-1);
    JKQTPdatastoreItem* it=new JKQTPdatastoreItem(1, rows);
    for (unsigned long long i=0; i<rows; i++) {
        it->set(0, i, start+(double)i * delta);
         //std::cout<<"copy@"<<i<<" = "<<data[i]<<std::endl;
    }
    /*items.push_back(it);
    size_t itemid=items.size()-1;*/
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

/** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
double JKQTPdatastore::get(size_t column, unsigned long long row) {
    return columns[column].getValue(row);
};

/** \brief sets the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!) */
void JKQTPdatastore::set(size_t column, unsigned long long row, double value) {
    columns[column].setValue(row, value);
};

JKQTPcolumn JKQTPdatastore::getColumn(size_t i) {
    //std::cout<<"datastore->getColumn("<<i<<")\n";
    return columns.value(i);
};

unsigned long long JKQTPdatastore::getMaxRows() {
    unsigned long long res=0;
    /*for (size_t i=0; i<columns.size(); i++) {
        unsigned long long r=columns[i].getRows();
        if (r>res) res=r;
    }*/
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        unsigned long long r=it.value().getRows();
        if (r>res) res=r;
    }

    return res;
}

void JKQTPdatastore::saveCSV(QString filename, QSet<int> userColumns, QString separator, QString decimal_separator, QString comment, QString aroundStrings, char floatformat) {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    saveCSV(txt, userColumns, separator, decimal_separator, comment, aroundStrings, floatformat);
    f.close();
}

void JKQTPdatastore::saveMatlab(QString filename, QSet<int> userColumns) {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    saveMatlab(txt, userColumns);
    f.close();
}

QStringList JKQTPdatastore::getColumnNames() const {
    QStringList names;
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    int col=0;
    while (it.hasNext()) {
        it.next();
        names.append(it.value().get_name());
        col++;
    }
    return names;
}

void JKQTPdatastore::saveMatlab(QTextStream &txt, QSet<int> userColumns) {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QSet<QString> varnames;


    txt.setLocale(loc);
/*    if (comment.size()>0) {
        txt<<comment<<" ";
        bool first=true;
        QMapIterator<size_t, JKQTPcolumn> it(columns);
        while (it.hasNext()) {
            it.next();
            if (!first) txt<<separator;
            txt<<aroundStrings<<it.value().get_name()<<aroundStrings;
            first=false;
        }
        txt<<"\n";
    }*/
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    int col=0;
    int i=0;
    while (it.hasNext()) {
        it.next();

        if (userColumns.isEmpty() || userColumns.contains(i)) {

            // convert the column name into a variable name (name cleanup!) If the column name is empty, we  use a default name ("column")
            QString newvarbase=jkqtp_to_valid_variable_name(it.value().get_name().toStdString()).c_str();
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
            txt<<QString("% data from columne %1 ('%2')\n").arg(col+1).arg(it.value().get_name());
            txt<<QString("%1 = [ ").arg(newvar);
            for (unsigned long long i=0; i<it.value().getRows(); i++) {
                txt<<loc.toString(get(it.key(), i))<<" ";

            }
            txt<<"];\n\n";
            col++;
        }
        i++;
    }

}

void JKQTPdatastore::saveCSV(QTextStream& txt, QSet<int> userColumns, QString separator, QString decimal_separator, QString comment, QString aroundStrings, char floatformat) {
    //std::cout<<filename<<"\n";

    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QChar dsep=loc.decimalPoint();


    txt.setLocale(loc);
    if (comment.size()>0) {
        txt<<comment<<" ";
        bool first=true;
        QMapIterator<size_t, JKQTPcolumn> it(columns);
        int i=0;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(i)) {
                if (!first) txt<<separator;
                txt<<aroundStrings<<it.value().get_name()<<aroundStrings;
                first=false;
            }
            i++;
        }
        txt<<"\n";
    }
    unsigned long long rows=getMaxRows();
    for (unsigned long long i=0; i<rows; i++) {
        bool first=true;
        QMapIterator<size_t, JKQTPcolumn> it(columns);
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

void JKQTPdatastore::saveSYLK(QString filename, QSet<int> userColumns, QString floatformat) {
    Q_UNUSED(floatformat)
    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    txt.setLocale(loc);
    // write SYLK header
    txt<<"ID;P\n";

    // write column headers
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            txt<<"C;Y1;X"<<c<<";K\""<<it.value().get_name()<<"\"\n";
            txt<<"F;Y1;X"<<c<<";SDB\n";
            c++;
        }
        i++;
    }



    unsigned long long rows=getMaxRows();
    for (unsigned long long i=0; i<rows; i++) {
        QMapIterator<size_t, JKQTPcolumn> it(columns);
        c=1;
        while (it.hasNext()) {
            it.next();
            if (userColumns.isEmpty() || userColumns.contains(i)) {
                if (it.value().getRows()>i) {
                    txt<<QString("C;X%1;Y%2;N;K%3\n").arg(c).arg(i+2).arg(get(it.key(), i));
                }
                c++;
            }
            i++;
        }
    }
    txt<<"E\n\n";
    f.close();



}

QList<QVector<double> > JKQTPdatastore::getData(QStringList *columnNames, QSet<int> userColumns)
{
    QStringList cl;
    QList<QVector<double> > res;

    // write column headers
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            QVector<double> dat;
            for (unsigned long long i=0; i<it.value().getRows(); i++) {
                dat<<it.value().getValue(i);
            }

            cl<<it.value().get_name();
            res<<dat;
            c++;
        }
        i++;
    }

    if (columnNames) *columnNames=cl;

    return res;
}


void JKQTPdatastore::saveDIF(QString filename, QSet<int> userColumns, QString floatformat) {
    Q_UNUSED(floatformat)
    // find out the decimal and the thousand separator
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    txt.setLocale(loc);
    unsigned long long rows=getMaxRows();

    // write DIF header
    txt<<QString("TABLE\n0,1\n\"\"\n");
    if (userColumns.isEmpty()) txt<<QString("VECTORS\n0,%1\n\"\"\n").arg(columns.size());
    else txt<<QString("VECTORS\n0,%1\n\"\"\n").arg(userColumns.size());
    txt<<QString("TUPLES\n0,%1\n\"\"\n").arg(rows);

    txt<<QString("-1,0\nBOT\n");

    // write column headers
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    size_t c=1;
    int i=0;
    while (it.hasNext()) {
        it.next();
        if (userColumns.isEmpty() || userColumns.contains(i)) {
            txt<<QString("1,0\n\"%1\"\n").arg(columns[c].get_name());
            c++;
        }
        i++;
    }



    for (unsigned long long i=0; i<rows; i++) {
        txt<<QString("-1,0\nBOT\n");
        QMapIterator<size_t, JKQTPcolumn> it(columns);
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

JKQTPdatastoreModel::JKQTPdatastoreModel(JKQTPdatastore *datastore, QObject *parent):
    QAbstractTableModel(parent)
{
    this->datastore=datastore;
    reloadModel();
}

JKQTPdatastoreModel::~JKQTPdatastoreModel()
{
}

QVariant JKQTPdatastoreModel::data(const QModelIndex &index, int role) const {
    int row=index.row();
    int column=index.column();
    if (datastore) {
        if (role==Qt::DisplayRole || role==Qt::EditRole) {
            int col=datastore->getColumnIDs().value(column, -1);
            if (col>-1 && row>=0 && row<(int64_t)datastore->getColumn(col).getRows()) {
                return datastore->get(col, row);
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags JKQTPdatastoreModel::flags(const QModelIndex &/*index*/) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant JKQTPdatastoreModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (datastore) {
        if (role==Qt::DisplayRole) {
            if (orientation==Qt::Horizontal) {
                if (section>=0 && section<(int64_t)datastore->getColumnCount()) {
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

int JKQTPdatastoreModel::rowCount(const QModelIndex &/*parent*/) const {
    if (datastore) {
        return datastore->getMaxRows();
    }
    return 0;
}

int JKQTPdatastoreModel::columnCount(const QModelIndex &/*parent*/) const {
    if (datastore) {
        return datastore->getColumnCount();
    }
    return 0;
}

void JKQTPdatastoreModel::reloadModel()
{
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    beginResetModel();
    endResetModel();
    #else
    reset();
    #endif
}

