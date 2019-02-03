/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplottertools/jkqtptools.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <QString>
#include <QMap>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QLocale>
#include <QSet>
#include <QStringList>
#include <QAbstractTableModel>
#include <QObject>
#ifdef JKQTPLOTTER_OPENCV_INTERFACE
#  include <opencv/cv.h>
#endif
#ifndef JKQTPDATASTORAGE_H
#define JKQTPDATASTORAGE_H


class JKQTPColumn; // forward declaration
class JKQTPDatastoreItem; // foward declaration
class JKQTPDatastoreModel; // forward declaration


/** \brief the types of data in one JKQTdatastoreItem
 * \ingroup jkqtpdatastorage
 *
 * \c JKQTPSingleColumn:
 * \verbatim
+-----+-----+-----+....................+-----+
|  0  |  1  |  2  |                    + N-1 |
+-----+-----+-----+....................+-----+
\endverbatim
 *
 *
 * \c JKQTPMatrixColumn:
 * \verbatim
 ================= COLUMN 1 =================  ================= COLUMN 2 =================
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
|  0  |  1  |  2  |                    + N-1 ||  0  |  1  |  2  |                    + N-1 |
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
 =R1== =R2== =R3==                      =RN==  =R1== =R2== =R3==                      =RN==
\endverbatim
 *
 *
 * \c JKQTPMatrixRow (C standard representation of matrices):
 * \verbatim
 ================== ROW  1 ==================  ================== ROW  2 ==================
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
|  0  |  1  |  2  |                    + N-1 ||  0  |  1  |  2  |                    + N-1 |
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
 =C1== =C2== =C3==                      =CN==  =C1== =C2== =C3==                      =CN==
\endverbatim
 */
enum JKQTPDatastoreItemFormat {
	JKQTPSingleColumn,                /*!< \brief a 1D vector of doubles. (default option) */
	JKQTPMatrixColumn,                /*!< \brief a 1D vector of double that represents a number of columns. The data is store column after column. */
	JKQTPMatrixRow                    /*!< \brief a 1D vector of double that represents a number of rows (C standard representation of matrices). The data is stored row after row.*/
};

/** \brief This class manages chunks of memory that are used for column data in JKQTBasePlotter descendents
 * \ingroup jkqtpdatastorage
 *
 * This class manages a list if JKQTPDatastoreItem onjects that may each contain a chunk of memory, containig
 * one or more columns of data. Each item can be accessed with get() by a specific ID which is returned by add().
 * JKQTPColumn. You may only clear all chunks of memory/items. If you no longer need some of the data, but still want
 * to access the rest you will simply have to destroy all JKQTPColumn that point to the item with their
 * JKQTPColumns:datastoreItem property.
 *
 * \verbatim

+- JKQTPDatastore ---------------------+               std::vector<JKQTPColumn>:
|                                      |                   +- JKQTPColumn ----------------+
|    +- JKQTPDatastoreItem --------+   |                 0 |   datastore                  |
|  0 |     JKQTPSingleColumn       |<--|-------------------|---datastoreItem = 0          |
|    |   0 # # # # # # # # # # #<--|---|-------------------|---datastoreOffset = 0        |
|    |                             |   |                   |                              |
|    +- JKQTPDatastoreItem --------+   |                   +- JKQTPColumn ----------------+
|  1 |     JKQTPSingleColumn       |<--|---\             1 |   datastore                  |
|    |   0 # # # # # # # # # # #<--|---|-\  \--------------|---datastoreItem = 1          |
|    |                             |   |  \----------------|---datastoreOffset = 0        |
|    +- JKQTPDatastoreItem --------+   |                   |                              |
|    :                             :   |                   +------------------------------+
|    :                             :   |                   :                              :
|    :                             :   |                   :                              :
|    +- JKQTPDatastoreItem --------+   |                   :                              :
|N-1 |     JKQTPMatrixRow          |<--|---\               :                              :
|    |   0 # # # # # # # # # # #   |   |    \              +- JKQTPColumn ----------------+
|    |   1 # # # # # # # # # # #   |   |     \         M-1 |   datastore                  |
|    |   2 # # # # # # # # # # #<--|---|--\   \------------|---datastoreItem = N-1        |
|    |                             |   |   \---------------|---datastoreOffset = 2        |
|    +-----------------------------+   |                   |                              |
|                                      |                   +------------------------------+
+--------------------------------------+


\endverbatim
 *
 * In addition the JKQTPDatastore manages a std::vector<JKQTPColumn> which may be used to access the data chunks in the logical
 * notion of data columns. This class provides a set of interface methods for this list:
 *
 */
class JKQTP_LIB_EXPORT JKQTPDatastore{
    private:
        /** \brief a std::vector that contains all items managed by this datastore */
        QMap<size_t, JKQTPDatastoreItem*> items;
        /** \brief a std::vector of all columns that may be used to access the managed chunks of memory. */
        QMap<size_t, JKQTPColumn> columns;

        size_t maxItemID;
        size_t maxColumnsID;
    protected:

        /** \brief add a new column to the datastore and return its ID */
        size_t addColumn(JKQTPColumn col);


    public:
        /** \brief class constructor, generates an empty datastore */
        JKQTPDatastore();
        /** \brief class destructor, destroys all subordered JKQTPDatastoreItem objects */
        ~JKQTPDatastore() {
            clear();
        }

        /** \brief deletes all items from the datastore and possibly frees the memory they manage */
        void clear();

        /** \brief returns the JKQTPDatastoreItem object for the \a i -th item in the store */
        inline JKQTPDatastoreItem* getItem(size_t i)  {
            return items.value(i, nullptr);
        }

        /** \brief returns the JKQTPDatastoreItem object for the \a i -th item in the store */
        inline const JKQTPDatastoreItem* getItem(size_t i) const {
            return items.value(i, nullptr);
        }

        /** \brief add a new item to the datastore and return its ID */
        size_t addItem(JKQTPDatastoreItem* item);


        /** \brief add a new columns/item with \a rows rows to the datastore and return its ID. The item uses internal memory management. */
        size_t addItem(size_t rows);

        /** \brief delete the given column, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteColumn(size_t column, bool removeItems=true);

        /** \brief delete all columns with the given name, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteAllColumns(const QString& name, bool removeItems=true);

        /** \brief delete all columns where the name starts with a given prefix, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteAllPrefixedColumns(QString prefix, bool removeItems=true);

        /** \brief add a new item with \a rows rows and \a columns columns to the datastore and return its ID. The item uses internal memory management. */
        size_t addItem(size_t columns, size_t rows);

        /** \brief add one external column to the datastore. It contains \a rows rows.*/
        size_t addItem(double* data, size_t rows);
        /** \brief add one internal column to the datastore. It contains \a rows rows.*/
        size_t addInternalItem(double* data, size_t rows);

        /** \brief add an external memory block to the datastore. It contains \a rows rows and \a columns columns. \a dataformat determined the memory layout*/
        size_t addItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows);

        /** \brief add one external column to the datastore. It contains \a rows rows. The data is copied and the copy managed internally */
        size_t addCopiedItem(const double *data, size_t rows);

        /** \brief add one external data block to the datastore. It contains \a rows rows and \a columns columns. The data is copied and the copy managed internally */
        size_t addCopiedItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows);



        /** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
        inline double get(size_t column, size_t row) const ;

        /** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
        inline double get(int column, size_t row) const ;
        /** \brief gets the index of the datapoint with the nearest, but lower value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column)*/
        int getNextLowerIndex(size_t column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the datapoint with the nearest, but lower value in the column */
        int getNextLowerIndex(size_t column, size_t row) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column) */
        int getNextHigherIndex(size_t column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column */
        int getNextHigherIndex(size_t column, size_t row) const;

        /** \brief sets the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!) */
        inline void set(size_t column, size_t row, double value);
        /** \brief sets the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!) */
        inline void set(int column, size_t row, double value);
        /** \brief gets the index of the datapoint with the nearest, but lower value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column)*/
        int getNextLowerIndex(int column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the datapoint with the nearest, but lower value in the column */
        int getNextLowerIndex(int column, size_t row) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column) */
        int getNextHigherIndex(int column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column */
        int getNextHigherIndex(int column, size_t row) const;



        /** \brief add a new columns which references a specified item and a specified column therein. */
        size_t addColumnForItem(size_t itemID, size_t columnInItem, const QString& name=QString(""));

        /** \brief add a new columns with \a rows rows to the datastore and return its column ID. The new item uses internal memory management. */
        size_t addColumn(size_t rows, const QString& name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. This returns its logical column ID.*/
        size_t addColumn(double* data, size_t rows, const QString& name=QString(""));





        /** \brief copies the given \a old_column into a new one, reading the data with the given start column and stride */
        size_t copyColumn(size_t old_column, size_t start, size_t stride, const QString& name=QString(""));
        /** \brief copies the given \a old_column into a new one */
        size_t copyColumn(size_t old_column, const QString& name=QString(""));


        /** \brief add one external column to the datastore. It will be filled with the contents of vector \a data.
         *
         *   \tparam TContainer datatype of the container, which need to support standard C++ iterators and the function \c size(). The contents needs to be convertible to double.
         *   \param data data vector to be copied
         *   \param name name for the column
         *   \return the ID of the newly created column
         */
        template <typename TContainer>
        size_t addCopiedColumn(const TContainer& data, const QString& name=QString("")) {
            const size_t N=static_cast<size_t>(data.size());
            double* d=static_cast<double*>(malloc(static_cast<size_t>(N)*sizeof(double)));
            if (N>0) {
                size_t r=0;
                for (auto it=data.begin(); it!=data.end(); ++it) {
                    d[r]=jkqtp_todouble(*it);
                    r++;
                }
            }
            size_t itemid=addInternalItem(d, N);
            return addColumnForItem(itemid, 0, name);
        }


        /** \brief add one external column to the datastore. It will be filled with the contents of vector \a data.
         *
         *   \tparam TContainer datatype of the container, which need to support standard C++ iterators and the function \c size(). The contents needs to be convertible to double.
         *   \param data data vector to be copied
         *   \param name name for the column
         *   \param stride strides through the container \a data with the given stride
         *   \param start starts copying from \a data with the element \a start
         *   \return the ID of the newly created column
         */
        template <typename TContainer>
        size_t addCopiedColumn(const TContainer& data, const QString& name, size_t stride, size_t start=0) {
            const size_t N=static_cast<size_t>(data.size()-start)/stride;
            double* d=static_cast<double*>(malloc(static_cast<size_t>(N)*sizeof(double)));
            if (N>0) {
                size_t r=0;
                auto it=data.begin();
                if (start>0) it+=start;
                for (; it!=data.end(); it+=stride) {
                    d[r]=jkqtp_todouble(*it);
                    r++;
                }
            }
            size_t itemid=addInternalItem(d, N);
            return addColumnForItem(itemid, 0, name);
        }



        /** \brief copy an external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *   \tparam T datatype of the element in the vector, this has to be convertible to double!
         *   \param data pointer to the data to be copied
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        template<typename T>
        size_t addCopiedColumn(const T* data, size_t rows, const QString& name=QString("")){
            double* d=static_cast<double*>(malloc(static_cast<size_t>(rows)*sizeof(double)));
            if (data) {
                for (size_t r=0; r<rows; r++) {
                    d[r]=jkqtp_todouble(data[r]);
                }
            }
            size_t itemid=addInternalItem(d, rows);
            return addColumnForItem(itemid, 0, name);
        }

        /** \brief copy an external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *   \tparam T datatype of the element in the vector, this has to be convertible to double!
         *   \param data pointer to the data to be copied
         *   \param rows number of items to copy from \a data
         *   \param stride when copying, this function steps throught the data with the given stride, so only eleemnts <code>[start, start+stride, start+2*stride, ... start+(rows-1)*stride]</code> are copied!
         *   \param start first element to copy
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        template<typename T>
        size_t addCopiedColumn(const T* data, size_t rows, size_t stride, int start, const QString& name) {
            double* d=static_cast<double*>(malloc(static_cast<size_t>(rows)*sizeof(double)));
            if (data) {
                for (size_t r=0; r<rows; r++) {
                    d[r]=jkqtp_todouble(data[static_cast<size_t>(start+static_cast<int64_t>(r*stride))]);
                }
            }
            size_t itemid=addInternalItem(d, rows);
            return addColumnForItem(itemid, 0, name);
        }

        /** \brief copy an external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *   \tparam T datatype of the element in the vector, this has to be convertible to double!
         *   \param data pointer to the data to be copied
         *   \param name name for the column
         *   \param stride when copying, this function steps throught the data with the given stride, so only eleemnts <code>[0, stride, 2*stride, ... (rows-1)*stride]</code> are copied!
         *   \return the ID of the newly created column
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        template<typename T>
        size_t addCopiedColumn(const T* data, size_t rows, size_t stride, const QString& name) {
            return addCopiedColumn<T>(data, rows, stride, 0, name);
        }


        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a row-major image and is copied as such. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        template <typename T>
        inline size_t addCopiedImageAsColumn(const T* data, size_t width, size_t height, const QString& name=QString(""), size_t stride=1, size_t start=0){
            return addCopiedColumn<T>(data, width*height, stride, start, name);
        }

        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a row-major image and is copied as such. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        template <typename TContainer>
        inline size_t addCopiedImageAsColumn(const TContainer& data, const QString& name=QString("")){
            return addCopiedColumn<TContainer>(data, name);
        }


        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a column-major image and is copied as row-major (i.e. is transposed). The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        template <typename T>
        size_t addCopiedImageAsColumnTranspose(const T* data, size_t width, size_t height, const QString& name=QString(""), size_t stride=1, size_t start=0){
            double* temp=static_cast<T*>(malloc(width*height*sizeof(T)));

            for (size_t x=0; x<width; x++) {
                for (size_t y=0; y<height; y++) {
                    temp[x*height+y]=data[start+(y*width+x)*stride];
                }

            }

            size_t idx=addCopiedColumn(temp, width*height, name);
            free(temp);
            return idx;
        }


        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a column-major image and is copied as row-major (i.e. is transposed). The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        template <typename T>
        inline size_t addCopiedImageAsColumnTranspose(const QVector<T>& data, size_t width, const QString& name=QString("")){
            return addCopiedImageAsColumnTranspose<T>(data.data(), width, static_cast<size_t>(data.size())/width, name);
        }


        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        template <typename T>
        size_t addCopiedColumnMasked(const T* data, const bool* mask, size_t rows, const QString& name=QString(""), bool useIfMaskEquals=false) {
            double* d=static_cast<double*>(calloc(rows, sizeof(double)));
            size_t rrs=0;
            if (data) {
                for (size_t r=0; r<rows; r++) {
                    if (!mask || (mask && (mask[r]==useIfMaskEquals))) {
                        d[rrs]=jkqtp_todouble(data[r]);
                        rrs++;
                    }
                }
            }


            size_t col= addCopiedColumn(d, rrs, name);
            free(d);
            return col;
        }

        /** \brief add one external column to the datastore. It will be filled with the contents of vector \a data.
         *
         *   \tparam TContainer datatype of the container, which need to support standard C++ iterators and the function \c size(). The contents needs to be convertible to double.
         *   \param data data vector to be copied
         *   \param name name for the column
         *   \param stride strides through the container \a data with the given stride
         *   \param start starts copying from \a data with the element \a start
         *   \return the ID of the newly created column
         */
        template <typename TContainer, typename TContainerMask>
        size_t addCopiedColumnMasked(const TContainer& data, const TContainerMask& mask, const QString& name=QString(""), bool useIfMaskEquals=false) {
            const size_t N=static_cast<size_t>(data.size());
            double* d=static_cast<double*>(malloc(static_cast<size_t>(N)*sizeof(double)));
            size_t rrs=0;
            if (data) {
                auto itmask=mask.begin();
                auto itdata=data.begin();
                for (size_t r=0; r<N; r++) {
                    if ((*itmask)==useIfMaskEquals) {
                        d[rrs]=jkqtp_todouble(*itdata);
                        rrs++;
                    }
                    ++itmask;
                    ++itdata;
                }
            }


            size_t col= addCopiedColumn(d, rrs, name);
            free(d);
            return col;

        }


        /** \brief copies the contents of the map-like container \a c into two columns of the datastore,
         *         returns the two IDs of the items as a std::pair */
        template <typename TContainer>
        std::pair<size_t, size_t> addCopiedMap(const TContainer& c, const QString& nameKey=QString("map_key"), const QString& nameValue=QString("map_value")) {
            std::vector<double> xvals;
            std::vector<double> yvals;
            for (auto it=c.begin(); it!=c.end(); ++it) {
                xvals.push_back(jkqtp_todouble(it->first));
                yvals.push_back(jkqtp_todouble(it->second));
            }
            return std::make_pair<size_t, size_t>(
                        addCopiedColumn(xvals, nameKey),
                        addCopiedColumn(yvals, nameValue)
                        );
        }





        /** \brief add a column to the datastore that contains \a rows rows with increasing value starting at \a start and ending at \a end.
         *         the values are equidistant between \a start end \a end */
        size_t addLinearColumn(size_t rows, double start, double end, const QString& name=QString(""));

        /** \brief returns the number of (logical) columns currently managed by the datastore */
        inline size_t getColumnCount() { return static_cast<size_t>(columns.size()); }

        inline QList<size_t> getColumnIDs() { return columns.keys(); }

        /** \brief return the num of the first column  with the given name, or -1 if none was found */
        int getColumnNum(const QString& name);

        /** \brief return the num of the first column  with the given name, if none was found this creates a new column with no rows and returns its num */
        int ensureColumnNum(const QString& name);

        /** \brief returns the JKQTPColumn object for the \a i -th column in the store */
        JKQTPColumn getColumn(size_t i) const;


        /** \brief returns the JKQTPColumn object for the \a i -th column in the store */
        JKQTPColumn getColumn(int i) const;

        /** \brief returns the maximum number of rows in all columns */
        size_t getMaxRows();

        /** \brief save contents of datastore as Comma Separated Values (CSV) file
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param separator the column separator char
         * \param decimal_separator the decimal separator ('.' by default)
         * \param comment comments are started with this string and end with a linebreak. If this parameter is empty no comments will be output
         * \param aroundStrings strings (in column headers) are surrounded by these characters!
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         *
         * Here are some default configuration:
         *  - <code>saveCSV(filename, ", ", ".", "#")</code> will generate a standard CSV file with commas between the columns and a point as decimal separator. Comments start with \c #
         *  - <code>saveCSV(filename, "; ", ",", "")</code> will generate a CSV file which may be read by a german Excel version: separated by ; and comma as decimal separator
         *  - <code>saveCSV(filename, "\t", ".", "#")</code> will generate a tab separated values file
         *  - ...
         * .
         */
        void saveCSV(const QString& filename, const QSet<int>& userColumns=QSet<int>(), const QString& separator=QString(", "), const QString& decimal_separator=QString("."), const QString& comment=QString("#"), const QString& aroundStrings=QString(""), char floatformat='g');
        /** \brief save contents of datastore as Comma Separated Values (CSV) file
         *
         * \param txt QTextStream to write to
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param separator the column separator char
         * \param decimal_separator the decimal separator ('.' by default)
         * \param comment comments are started with this string and end with a linebreak. If this parameter is empty no comments will be output
         * \param aroundStrings strings (in column headers) are surrounded by these characters!
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         *
         * Here are some default configuration:
         *  - <code>saveCSV(filename, ", ", ".", "#")</code> will generate a standard CSV file with commas between the columns and a point as decimal separator. Comments start with \c #
         *  - <code>saveCSV(filename, "; ", ",", "")</code> will generate a CSV file which may be read by a german Excel version: separated by ; and comma as decimal separator
         *  - <code>saveCSV(filename, "\t", ".", "#")</code> will generate a tab separated values file
         *  - ...
         * .
         */
        void saveCSV(QTextStream& txt, const QSet<int>& userColumns=QSet<int>(), const QString& separator=QString(", "), const QString& decimal_separator=QString("."), const QString& comment=QString("#"), const QString& aroundStrings=QString(""), char floatformat='g');

        /** \brief save contents of datastore as <a href="http://en.wikipedia.org/wiki/SYmbolic_LinK_(SYLK)">SYLK file (SYmbolic LinK)</a>
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         */
        void saveSYLK(const QString& filename, const QSet<int>& userColumns=QSet<int>(), const QString& floatformat=QString("%10.10lf"));

        /** \brief return contents of datastore as QList<QVector<double> >, i.e. a list of column-vectors
         *
         * \param columnNames if \c !=nullptr this will afterwards conatin the column titles
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        QList<QVector<double> > getData(QStringList* columnNames=nullptr, const QSet<int>& userColumns=QSet<int>());

        /** \brief save contents of datastore as <a href="http://www.fileformat.info/format/dif/egff.htm">DIF file (data interchange format)</a>
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         */
        void saveDIF(const QString& filename, const QSet<int>& userColumns=QSet<int>(), const QString& floatformat=QString("%10.10lf"));

        /** \brief save contents of datastore as a Matlab script
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        void saveMatlab(const QString& filename, const QSet<int>& userColumns=QSet<int>());
        /** \brief save contents of datastore as a Matlab script
         *
         * \param txt the QTextStream to write to
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        void saveMatlab(QTextStream& txt, const QSet<int>& userColumns=QSet<int>());

        /** \brief return a list with all columns available in the datastore */
        QStringList getColumnNames() const;
        /** \brief add a column with \a rows entries from the array \a data,
         *         ownership of the memory behind \a data is transfered to the datastore */
        size_t addInternalColumn(double *data, size_t rows, const QString& name);
};




/** \brief stores information about one data column. See JKQTPDatastore for more information.
 * \ingroup jkqtpdatastorage
 *
 * \see JKQTPDatastore
 */
class JKQTP_LIB_EXPORT JKQTPColumn {
  private:
    /** \brief index of the item in the datastore that contains the data for this column */
    size_t datastoreItem;
    /** \brief offset, if the datastore item contains more than one column */
    size_t datastoreOffset;
    /** \brief a name describing the column */
    QString name;
    /** \brief pointer to the datastore object used to manage the data of the plot */
    JKQTPDatastore* datastore;

    bool valid;

  protected:
    inline JKQTPDatastore* getDatastore() { return datastore; }
    inline const JKQTPDatastore* getDatastore() const { return datastore; }
  public:
    JKQTPColumn();
    /** \brief class constructor that binds the column to a specific datastore object.
     *
     * The use of this constructor is mandatory. The default constructor (no arguments) is hidden. Also note
     * that you cannot change the binding of a column to a datastore object after creation of the column.
     */
    JKQTPColumn(JKQTPDatastore* datastore, const QString& name=QString(""), size_t datastoreItem=0, size_t datastoreOffset=0);

    inline bool isValid() const { return valid; }

    /** \brief class destructor */
    ~JKQTPColumn() ;

    /*! \brief sets the property name ( \copybrief name ) to the specified \a __value. \details Description of the parameter name is: <BLOCKQUOTE>\copydoc JKQTPColumn::name </BLOCKQUOTE> \see JKQTPColumn::name for more information */
    inline void setName (const QString& __value)
    {
      this->name = __value;
    }
    /*! \brief returns the property name ( \copybrief name ). \see name for more information */
    inline QString getName () const
    {
      return this->name;
    }

    /** \brief returns the number of rows in this column (accesses the datastore) */
    size_t getRows() const;

    /** \brief copies the contained data into a QVector */
    void copyData(QVector<double>& copyTo) const;
    /** \brief returns a QVector with the contained data (as a copy) */
    QVector<double> copyData();

    /** \brief reads the \a n'th value from the column
     *
     * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
     * column.
     */
    inline double getValue(size_t n) const;
    /** \brief reads the \a n'th value from the column
     *
     * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
     * column.
     */
    inline double getValue(int n) const;
    /** \brief gets a pointer to the n-th value in the column */
    double* getPointer(size_t n=0) ;
    /** \brief gets a pointer to the n-th value in the column */
    const double* getPointer(size_t n=0) const;

    /** \brief sets the \a n'th value from the column
     *
     * This method accesses the datastore and sets the value stored in the \a n'th row of the according
     * column.
     */
    inline void setValue(size_t n, double val);

    /** \brief increment the \a n'th value from the column
     *
     */
    inline void incValue(size_t n, double increment=1.0);

    /** \brief decrement the \a n'th value from the column
     *
     */
    inline void decValue(size_t n, double decrement=1.0) {
        incValue(n, -1.0*decrement);
    }


    /** \brief sets the element at (x,y) in the column, where the data is interpreted as a row-major ordered Matrix of the given width
     *
     * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
     * column.
     */
    inline void setPixelValue(size_t x, size_t y, size_t width, double val) {
        setValue(y*width+x, val);
    }

    /** \brief returns a pointer to the datastore item representing this column */
    inline JKQTPDatastoreItem* getDatastoreItem() const { return datastore->getItem(datastoreItem); }

    /** \brief copy data from the given array into the column
     *
     * This copies \a N elements from \a data into the column where the first overwritten column
     * line is \a offset, so you can shift the location where the copy process starts.
     */
    void copy(double* data, size_t N, size_t offset=0);

    /** \brief exchange every occurence of a given \a value by a \a replace value */
    void exchange(double value, double replace);

    /** \brief subtracts a given value from all members of the column */
    void subtract(double value);
    /** \brief scales all members of the column with the given factor */
    void scale(double factor);

    /** \brief set all values in the column to a specific \a value */
    void setAll(double value);


    /*! \brief returns the property datastoreItem ( \copybrief datastoreItem ). \details Description of the parameter datastoreItem is:  <BLOCKQUOTE>\copydoc JKQTPColumn::datastoreItem </BLOCKQUOTE>. \see JKQTPColumn::datastoreItem for more information */ \
    inline size_t getDatastoreItemNum() const  \
    {   return this->datastoreItem;   }
    /*! \brief returns the property datastoreOffset ( \copybrief datastoreOffset ). \details Description of the parameter datastoreOffset is:  <BLOCKQUOTE>\copydoc JKQTPColumn::datastoreOffset </BLOCKQUOTE>. \see JKQTPColumn::datastoreOffset for more information */ \
    inline size_t getDatastoreOffset() const  \
    {   return this->datastoreOffset;   }
};




/** \brief this represents one chunk of memory which can represent one or more columns of data for JKQTBasePlotter.
 *         See JKQTPDatastore for more information.
 * \ingroup jkqtpdatastorage
 *
 * Each chunk of memory is pointed at by a simple double* pointer \c data. the memory layout of the memory layout of
 * the RAM segment pointed at by \c data is determined by the parameter \c dataformat:
 *   \copydoc JKQTPDatastoreItemFormat
 *
 * The properties \c columns and \c rows determine how many columns and rows are represented by this item (access via
 * getColumns() and getRows() ). This class may manage chunks of "internal" and "external" memory (which is indicated by
 * the boolean property \c internal. Internal memory will be allocated (in the constructor) and freed (in the destructor) by this
 * object. External memory may be accessed via this class, but will neither by allocated nor freed. These tasks are up to the
 * user. Use this option, if you don't want to generate copies of huge datasets, or you want to change the data while the program
 * is running and don't want to do the change at more than one spot.
 *
 * you can use get() and set() to access the memory chunc.
 *
 * \see JKQTPDatastore
 */
class JKQTP_LIB_EXPORT JKQTPDatastoreItem {
  private:
    /** \brief a pointer to the actual data */
    double* data;
    /** \brief specifies whether the datastore manages the memory (\c true ) or whether the user application does this (\c false ) .*/
    bool internal;
    /** \brief Specifies whether memory for the data has been allocated. This is only used, when \c internal==true. */
    bool allocated;
    /** \brief as data may also point to a matrix, this specifies the number of columns in this element (default: 1) */
    size_t columns;
    /** \brief number of rows in this item */
    size_t rows;
    /** \brief memory format of the data in this item */
    JKQTPDatastoreItemFormat dataformat;
  protected:
    /** \brief hidden default constructor */
    JKQTPDatastoreItem();
  public:
    /** \brief class constructor: initializes the object for internal data storage */
    JKQTPDatastoreItem(size_t columns, size_t rows);
    /** \brief class constructor: initializes the object for external data storage */
    JKQTPDatastoreItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows);
    /** \brief class constructor: initializes the object for external data storage */
    JKQTPDatastoreItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows, bool internal);
    /** \brief class destructor: frees unfreed internal memory */
    ~JKQTPDatastoreItem();

    /** \brief change the size of all columns to the givne number of rows. The data will be lost */
    void resizeColumns(size_t rows);

    /*! \brief returns the property rows ( \copybrief rows ). \details Description of the parameter rows is:  <BLOCKQUOTE>\copydoc JKQTPDatastoreItem::JKQTPDatastoreItemrows </BLOCKQUOTE>. \see JKQTPDatastoreItem::rows for more information */ \
    inline size_t getRows() const
    {   return rows;   }
    /*! \brief returns the property columns ( \copybrief columns ). \details Description of the parameter columns is:  <BLOCKQUOTE>\copydoc JKQTPDatastoreItem::columns </BLOCKQUOTE>. \see JKQTPDatastoreItem::columns for more information */ \
    inline size_t getColumns() const
    {   return columns;   }


    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline double get(size_t column, size_t row) {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPSingleColumn:
              return data[row];
            case JKQTPMatrixColumn:
              return data[column*rows+row];
            case JKQTPMatrixRow:
              return data[row*columns+column];
        }
        return 0;
    }


    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline double* getPointer(size_t column, size_t row) {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPSingleColumn:
              return &(data[row]);
            case JKQTPMatrixColumn:
              return &(data[column*rows+row]);
            case JKQTPMatrixRow:
              return &(data[row*columns+column]);
        }
        return nullptr;
    }

    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline const double* getPointer(size_t column, size_t row) const {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPSingleColumn:
              return &(data[row]);
            case JKQTPMatrixColumn:
              return &(data[column*rows+row]);
            case JKQTPMatrixRow:
              return &(data[row*columns+column]);
        }
        return nullptr;
    }
    /** \brief set the data at the position (\a column, \a row ) to \a value. The column index specifies the column inside THIS item, not the global column number. */
    inline void set(size_t column, size_t row, double value) {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPSingleColumn:
              data[row]=value;
              return;
            case JKQTPMatrixColumn:
              data[column*rows+row]=value;
              return;
            case JKQTPMatrixRow:
              data[row*columns+column]=value;
              return;
        }
    }
};


/** \brief 	QAbstractTableModel descendent that allows to view data in a JKQTPDatastore
 * \ingroup jkqtpdatastorage
 *
 * \see JKQTPDatastore
 */
class JKQTP_LIB_EXPORT JKQTPDatastoreModel: public QAbstractTableModel {
        Q_OBJECT
    public:
        JKQTPDatastoreModel(JKQTPDatastore* datastore, QObject* parent=nullptr);
        virtual ~JKQTPDatastoreModel() override;


        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const  override;

    public slots:
        void reloadModel();

    protected:
        JKQTPDatastore* datastore;

};





////////////////////////////////////////////////////////////////////////////////////////////////
inline void JKQTPColumn::setValue(size_t n, double val){
    if (!datastore) return ;
    if (!datastore->getItem(datastoreItem)) return ;
    datastore->getItem(datastoreItem)->set(datastoreOffset, n, val);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline void JKQTPColumn::incValue(size_t n, double increment){
    if (!datastore) return ;
    if (!datastore->getItem(datastoreItem)) return ;
    datastore->getItem(datastoreItem)->set(datastoreOffset, n, datastore->getItem(datastoreItem)->get(datastoreOffset, n)+increment);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPColumn::getValue(size_t n) const {
    if (!datastore) return 0;
    if (!datastore->getItem(datastoreItem)) return 0;
    return datastore->getItem(datastoreItem)->get(datastoreOffset, n);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPColumn::getValue(int n) const {
    if (!datastore) return 0;
    if (!datastore->getItem(datastoreItem)) return 0;
    if (n<0) return 0;
    return datastore->getItem(datastoreItem)->get(datastoreOffset, static_cast<size_t>(n));
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPDatastore::get(size_t column, size_t row) const {
    return columns[column].getValue(row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPDatastore::get(int column, size_t row) const {
    return get(static_cast<size_t>(column), static_cast<size_t>(row));
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline void JKQTPDatastore::set(size_t column, size_t row, double value)  {
    columns[column].setValue(row, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline void JKQTPDatastore::set(int column, size_t row, double value)  {
    set(static_cast<size_t>(column), static_cast<size_t>(row), value);
}



#endif // JKQTPDATASTORAGE_H
