/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#include "jkqtplotter/jkqtpdatastorage.h"
#include <QDebug>
#include <QtGlobal>


/**************************************************************************************************************************
 * JKQTPcolumn
 **************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPcolumn::JKQTPcolumn()
{
    datastore=nullptr;
    name="";
    datastoreItem=0;
    datastoreOffset=0;
    valid=false;
}

JKQTPcolumn::JKQTPcolumn(JKQTPdatastore *datastore, const QString &name, size_t datastoreItem, size_t datastoreOffset)
{
    this->datastore=datastore;
    this->datastoreItem=datastoreItem;
    this->datastoreOffset=datastoreOffset;
    this->name=name;
    valid=true;

}

JKQTPcolumn::~JKQTPcolumn()
{

}

size_t JKQTPcolumn::getRows() const {
    if (!valid || !datastore) return 0;
    JKQTPdatastoreItem* i=datastore->getItem(datastoreItem);
    if (i) return i->get_rows();
    else return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPcolumn::copyData(QVector<double> &copyTo) const
{
    double* d=getPointer(0);
    copyTo.clear();
    size_t i, cnt=getRows();
    if (cnt>0) {
        copyTo.resize(cnt);
        for (i=0; i<cnt; i++) {
            copyTo[i]=d[i];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
QVector<double> JKQTPcolumn::copyData()
{
    QVector<double> d;
    copyData(d);
    return d;
}


////////////////////////////////////////////////////////////////////////////////////////////////
double *JKQTPcolumn::getPointer(size_t n) const
{
    if (!datastore) return nullptr;
    if (!datastore->getItem(datastoreItem)) return nullptr;
    return datastore->getItem(datastoreItem)->getPointer(datastoreOffset, n);
}


////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPcolumn::copy(double* data, size_t N, size_t offset) {
    if (!datastore) return ;
    JKQTPdatastoreItem* it=datastore->getItem(datastoreItem);
    if (!it) return;
    for (size_t i=0; i<N; i++) {
        it->set(datastoreOffset, i+offset, data[i]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPcolumn::exchange(double value, double replace)
{
    if (!datastore) return ;
    for (size_t i=0; i<getRows(); i++) {
        double v=getValue(i);
        if (v==value) v=replace;
        setValue(i, v);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPcolumn::subtract(double value)
{
    if (!datastore) return ;
    double* data=getPointer();
    size_t N=getRows();
    if (data){
        for (size_t i=0; i<N; i++) {
            data[i]=data[i]-value;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPcolumn::scale(double factor)
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
void JKQTPcolumn::setAll(double value)
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
 * JKQTPdatastoreItem
 **************************************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPdatastoreItem::JKQTPdatastoreItem(){
    this->dataformat=JKQTPsingleColumn;
    this->allocated=false;
    this->data=nullptr;
    this->columns=0;
    this->rows=0;
    this->internal=true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPdatastoreItem::JKQTPdatastoreItem(size_t columns, size_t rows){
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

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPdatastoreItem::resizeColumns(size_t new_rows) {
    if (internal && allocated && data!=nullptr) {
        free(data);
        data=nullptr;
    }
    data=(double*)calloc(columns*new_rows, sizeof(double));
    rows=new_rows;
    internal=true;
    allocated=true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPdatastoreItem::JKQTPdatastoreItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columns, size_t rows){
    this->dataformat=dataformat;
    this->allocated=true;
    this->data=data;
    this->columns=columns;
    this->rows=rows;
    this->internal=false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPdatastoreItem::JKQTPdatastoreItem(JKQTPdatastoreItemFormat dataformat, double *data, size_t columns, size_t rows, bool internal)
{
    this->dataformat=dataformat;
    this->allocated=true;
    this->data=data;
    this->columns=columns;
    this->rows=rows;
    this->internal=internal;
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPdatastoreItem::~JKQTPdatastoreItem(){
    if (internal && allocated && data!=nullptr) {
        free(data);
        data=nullptr;
    }
}



/**************************************************************************************************************************
 * JKQTPdatastore
 **************************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPdatastore::addItem(JKQTPdatastoreItem* item) {
    /*items.push_back(item);
    return items.size()-1;*/
    items.insert(maxItemID, item);
    maxItemID++;
    return maxItemID-1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPdatastore::addColumn(JKQTPcolumn col) {
    columns.insert(maxColumnsID, col);
    maxColumnsID++;
    return maxColumnsID-1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
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

void JKQTPdatastore::deleteAllColumns(const QString& name, bool removeItems) {
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

int JKQTPdatastore::getColumnNum(const QString& name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().get_name()==name) return it.key();
    }
    return -1;
}

size_t JKQTPdatastore::ensureColumnNum(const QString& name) {
    if (columns.size()<=0) return -1;
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        if (it.value().get_name()==name) return it.key();
    }
    return addColumn(0, name);
}

JKQTPcolumn JKQTPdatastore::getColumn(size_t i) const
{
    return columns.value(i);
}

size_t JKQTPdatastore::addCopiedItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columnsnum, size_t rows) {
    JKQTPdatastoreItem* it=nullptr;
    if ((dataformat==JKQTPsingleColumn)||(columnsnum==1)) {
        return addCopiedItem(data, rows);
    } else if (dataformat==JKQTPmatrixColumn) {
        it=new JKQTPdatastoreItem(columnsnum, rows);
        for (size_t c=0; c<columnsnum; c++) {
            for (size_t r=0; r<rows; r++) {
                it->set(c, r, data[c*rows+r]);
            }
        }
    } else if (dataformat==JKQTPmatrixColumn) {
        it=new JKQTPdatastoreItem(columnsnum, rows);
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



/** \brief add a new columns/item with \a rows rows to the datastore and return its ID. The item uses internal memory management. */
size_t JKQTPdatastore::addItem(size_t rows) {
    /*items.push_back(new JKQTPdatastoreItem(1, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(1, rows));
};

/** \brief add a new item with \a rows rows and \a columns columns to the datastore and return its ID. The item uses internal memory management. */
size_t JKQTPdatastore::addItem(size_t columnsnum, size_t rows) {
    /*items.push_back(new JKQTPdatastoreItem(columnsnum, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(columnsnum, rows));
};

/** \brief add one external column to the datastore. It contains \a rows rows.*/
size_t JKQTPdatastore::addItem(double* data, size_t rows) {
    /*items.push_back(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
}

size_t JKQTPdatastore::addInternalItem(double *data, size_t rows)
{
    JKQTPdatastoreItem* dsi=new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows, true);
    return addItem(dsi);
};

/** \brief add an external memory block to the datastore. It contains \a rows rows and \a columns columns. \a dataformat determined the memory layout*/
size_t JKQTPdatastore::addItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columnsnum, size_t rows) {
    /*items.push_back(new JKQTPdatastoreItem(dataformat, data, columnsnum, rows));
    return items.size()-1;*/
    return addItem(new JKQTPdatastoreItem(dataformat, data, columnsnum, rows));
};

/** \brief add one external array to the datastore. It contains \a rows rows. The data is copied and the copy managed internally */
size_t JKQTPdatastore::addCopiedItem(const double* data, size_t rows) {
    JKQTPdatastoreItem* it=new JKQTPdatastoreItem(1, rows);
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

/** \brief add a new columns which references a specified item and a specified column therein. */
size_t JKQTPdatastore::addColumnForItem(size_t itemID, size_t columnInItem, const QString& name) {
    /*JKQTPcolumn c(this, name, itemID, columnInItem);
    columns.push_back(c);
    return columns.size()-1;*/
    return addColumn(JKQTPcolumn(this, name, itemID, columnInItem));
};

/** \brief add a new columns with \a rows rows to the datastore and return its column ID. The new item uses internal memory management. */
size_t JKQTPdatastore::addColumn(size_t rows, const QString& name) {
    //items.push_back(new JKQTPdatastoreItem(1, rows));
    //return addColumnForItem(items.size()-1, 0, name);
    size_t item= addItem(new JKQTPdatastoreItem(1, rows));
    return addColumnForItem(item, 0, name);
};

/** \brief add one external column to the datastore. It contains \a rows rows. This returns its logical column ID.*/
size_t JKQTPdatastore::addColumn(double* data, size_t rows, const QString& name) {
    //items.push_back(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    //std::cout<<"added item\n";
    //size_t it=items.size()-1;
    //std::cout<<"adding column\n";
    size_t it=addItem(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    return addColumnForItem(it, 0, name);
};
/** \brief add one external column to the datastore. It contains \a rows rows. This returns its logical column ID.*/
size_t JKQTPdatastore::addInternalColumn(double* data, size_t rows, const QString& name) {
    //items.push_back(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows));
    //std::cout<<"added item\n";
    //size_t it=items.size()-1;
    //std::cout<<"adding column\n";
    size_t it=addItem(new JKQTPdatastoreItem(JKQTPsingleColumn, data, 1, rows,true));
    return addColumnForItem(it, 0, name);
};









size_t JKQTPdatastore::copyColumn(size_t old_column, size_t start, size_t stride, const QString& name)
{
    JKQTPcolumn old=columns[old_column];
    size_t rows=old.getRows();
    double* d=(double*)malloc(rows*sizeof(double));
    double* dd=old.getPointer(0);
    int j=0;
    for (size_t i=start; i<rows; i+=stride) {
        d[j]=dd[i];
        //qDebug()<<old_column<<name<<": "<<j<<i<<d[j];
        j++;
    }
    size_t n=addCopiedColumn(d, j, name);
    free(d);
    return n;
}

size_t JKQTPdatastore::copyColumn(size_t old_column, const QString& name)
{
    return copyColumn(old_column, 0, 1, name);
}

#ifdef JKQTPLOTTER_OPENCV_INTERFACE
namespace JKQTPdatastore_Helper {
    template <typename TPixel>
    void copyDataFromMat(double* data, const cv::Mat& mat, int channel) {
        size_t r=0;
        const int channels=mat.channels();
        for (int iy=0; iy<mat.rows; iy++ ) {
            const TPixel* row=mat.ptr<TPixel>(iy);
            for (int ix=0; ix<mat.cols; ix++ ) {
                data[r]=jkqtp_todouble(*static_cast<const TPixel*>(&(row[ix*channels+channel])));
                r++;
            }
        }
    }
}

size_t JKQTPdatastore::copyCvMatToColumn(const cv::Mat& mat, const QString &name, int channel)
{
    const size_t N=static_cast<size_t>(mat.cols*mat.rows);
    double* d=static_cast<double*>(malloc(static_cast<size_t>(N)*sizeof(double)));

    if (CV_MAT_DEPTH(mat.type())==CV_64F) JKQTPdatastore_Helper::copyDataFromMat<double>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_32F) JKQTPdatastore_Helper::copyDataFromMat<float>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_32S) JKQTPdatastore_Helper::copyDataFromMat<uint32_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_16S) JKQTPdatastore_Helper::copyDataFromMat<int16_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_16U) JKQTPdatastore_Helper::copyDataFromMat<uint16_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_8S) JKQTPdatastore_Helper::copyDataFromMat<int8_t>(d, mat, channel);
    else if (CV_MAT_DEPTH(mat.type())==CV_8U) JKQTPdatastore_Helper::copyDataFromMat<uint8_t>(d, mat, channel);
    else throw std::runtime_error("datatype of cv::Mat not supported by JKQTPdatastore::copyImageToColumn()");

    size_t itemid=addInternalItem(d, N);
    return addColumnForItem(itemid, 0, name);

}
#endif



size_t JKQTPdatastore::addLinearColumn(size_t rows, double start, double end, const QString& name) {
    double delta=(end-start)/(double)(rows-1);
    JKQTPdatastoreItem* it=new JKQTPdatastoreItem(1, rows);
    for (size_t i=0; i<rows; i++) {
        it->set(0, i, start+(double)i * delta);
         //std::cout<<"copy@"<<i<<" = "<<data[i]<<std::endl;
    }
    /*items.push_back(it);
    size_t itemid=items.size()-1;*/
    size_t itemid= addItem(it);
    return addColumnForItem(itemid, 0, name);
}

/** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
double JKQTPdatastore::get(size_t column, size_t row) const {
    return columns[column].getValue(row);
}

long long JKQTPdatastore::getNextLowerIndex(size_t column, size_t row, long long start, long long end) const
{
    const JKQTPcolumn& col=columns[column];
    if (start<0 && end>=0) return getNextLowerIndex(column, row, 0, end);
    else if (start>=0 && end<0)  return getNextLowerIndex(column, row, start, col.getRows()-1);
    else if (start<0 && end<0)  return getNextLowerIndex(column, row, 0, col.getRows()-1);
    else {
        double d=0;
        const double v=col.getValue(row);
        long long res=-1;
        for ( long long i=start; i<=end; i++) {
            if (i!=(long long)row) {
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

long long JKQTPdatastore::getNextLowerIndex(size_t column, size_t row) const
{
    return getNextLowerIndex(column, row, 0, columns[column].getRows()-1);
}

long long JKQTPdatastore::getNextHigherIndex(size_t column, size_t row, long long start, long long end) const
{
    const JKQTPcolumn& col=columns[column];
    if (start<0 && end>=0) return getNextHigherIndex(column, row, 0, end);
    else if (start>=0 && end<0)  return getNextHigherIndex(column, row, start, col.getRows()-1);
    else if (start<0 && end<0)  return getNextHigherIndex(column, row, 0, col.getRows()-1);
    else {
        double d=0;
        const double v=col.getValue(row);
        long long res=-1;
        for ( long long i=start; i<=end; i++) {
            if (i!=(long long)row) {
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

long long JKQTPdatastore::getNextHigherIndex(size_t column, size_t row) const
{
    return getNextHigherIndex(column, row, 0, columns[column].getRows()-1);
}

void JKQTPdatastore::set(size_t column, size_t row, double value)
{
    columns[column].setValue(row, value);
}



size_t JKQTPdatastore::getMaxRows() {
    size_t res=0;
    /*for (size_t i=0; i<columns.size(); i++) {
        size_t r=columns[i].getRows();
        if (r>res) res=r;
    }*/
    QMapIterator<size_t, JKQTPcolumn> it(columns);
    while (it.hasNext()) {
        it.next();
        size_t r=it.value().getRows();
        if (r>res) res=r;
    }

    return res;
}

void JKQTPdatastore::saveCSV(QString filename, QSet<int> userColumns, QString separator, QString decimal_separator, QString comment, QString aroundStrings, char floatformat) {
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

void JKQTPdatastore::saveMatlab(QString filename, QSet<int> userColumns) {
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
            for (size_t i=0; i<it.value().getRows(); i++) {
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
    size_t rows=getMaxRows();
    for (size_t i=0; i<rows; i++) {
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
    //QChar dsep=loc.decimalPoint();
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



    size_t rows=getMaxRows();
    for (size_t i=0; i<rows; i++) {
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
            for (size_t i=0; i<it.value().getRows(); i++) {
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
    //QChar dsep=loc.decimalPoint();
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QTextStream txt(&f);
    txt.setLocale(loc);
    size_t rows=getMaxRows();

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



    for (size_t i=0; i<rows; i++) {
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
