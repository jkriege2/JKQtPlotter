/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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
enum class JKQTPDatastoreItemFormat {
    SingleColumn,                /*!< \brief a 1D C-array of doubles. (default option) */
    MatrixColumn,                /*!< \brief a 1D C-array of doubles that represents a number of columns. The data is store column after column (=column-major). */
    MatrixRow,                   /*!< \brief a 1D C-array of doubles that represents a number of rows (C standard representation of matrices). The data is stored row after row (=row-major).*/
};

/** \brief This class manages chunks of memory that are used for column data in JKQTBasePlotter descendents
 * \ingroup jkqtpdatastorage
 *
 * \see \ref JKQTPlotterAdvancedJKQTPDatastore for a detailed description of how to use this class for data management!
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

        /** \brief add a new item to the datastore and return its ID
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addItem(JKQTPDatastoreItem* item);


        /** \brief add a new columns/item with \a rows rows to the datastore and return its ID. The item uses internal memory management.
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addItem(size_t rows);

        /** \brief add a new item with \a rows rows and \a columns columns to the datastore and return its ID. The item uses internal memory management.
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addItem(size_t columns, size_t rows);

        /** \brief add one external column to the datastore. It contains \a rows rows.
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addItem(double* data, size_t rows);
        /** \brief add one internal column to the datastore. It contains \a rows rows.
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addInternalItem(double* data, size_t rows);

        /** \brief add one external column to the datastore. It contains \a rows rows. The data is copied and the copy managed internally
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addCopiedItem(const double *data, size_t rows);

        /** \brief add an external memory block to the datastore. It contains \a rows rows and \a columns columns. \a dataformat determined the memory layout.
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows);

        /** \brief add one external data block to the datastore. It contains \a rows rows and \a columns columns. The data is copied and the copy managed internally
         *         This function retuns an item ID (which can be used with getItem() ), not a column ID! */
        size_t addCopiedItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows);

        /** \brief returns the JKQTPDatastoreItem object for the \a i -th item in the store */
        inline JKQTPDatastoreItem* getItem(size_t i)  {
            return items.value(i, nullptr);
        }

        /** \brief returns the JKQTPDatastoreItem object for the \a i -th item in the store */
        inline const JKQTPDatastoreItem* getItem(size_t i) const {
            return items.value(i, nullptr);
        }

        /** \brief add a new columns which references a specified item and a specified column therein.
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addColumnForItem(size_t itemID, size_t columnInItem, const QString& name=QString(""));

        /** \brief returns the JKQTPColumn object for the \a i -th column in the store
         *
         *  \warning This function copies the pointers/references to the internal data into a new object.
         *           Therefore you should delete it as soon as possible and not store the return value over long durations,
         *           as the data may get moved in the meantime and then the object gets invalid, but is not informed of this fact!
         */
        JKQTPColumn getColumn(size_t i) const;


        /** \brief returns the JKQTPColumn object for the \a i -th column in the store
         *
         *  \warning This function copies the pointers/references to the internal data into a new object.
         *           Therefore you should delete it as soon as possible and not store the return value over long durations,
         *           as the data may get moved in the meantime and then the object gets invalid, but is not informed of this fact!
         */
        JKQTPColumn getColumn(int i) const;
    public:
        /** \brief class constructor, generates an empty datastore */
        JKQTPDatastore();
        /** \brief class destructor, destroys all subordered JKQTPDatastoreItem objects */
        ~JKQTPDatastore();



        /** \brief deletes all items from the datastore and possibly frees the memory they manage */
        void clear();


        /** \brief delete the given column, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteColumn(size_t column, bool removeItems=true);

        /** \brief delete all columns with the given name, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteAllColumns(const QString& name, bool removeItems=true);

        /** \brief delete all columns where the name starts with a given prefix, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteAllPrefixedColumns(QString prefix, bool removeItems=true);


        /** \brief returns the number of rows in the column \a column */
        inline size_t getRows(size_t column) const;
        /** \brief returns a pointer to the data in column \a column, starting ar row \a row */
        inline const double* getColumnPointer(size_t column, size_t row=0) const;
        /** \brief returns a pointer to the data in column \a column, starting ar row \a row */
        inline double* getColumnPointer(size_t column, size_t row=0);
        /** \brief returns the number of rows in the column \a column */
        inline size_t getRows(int column) const;
        /** \brief returns a pointer to the data in column \a column, starting ar row \a row */
        inline const double* getColumnPointer(int column, size_t row=0) const;
        /** \brief returns a pointer to the data in column \a column, starting ar row \a row */
        inline double* getColumnPointer(int column, size_t row=0);
        /** \brief returns the width of the image, represented by \a column (in row-major ordering).
         *         Internally this returns the imageColumns or image width, if set in the column */
        inline size_t getColumnImageWidth(int column) const;
        /** \brief returns the height of the image, represented by \a column (in row-major ordering) */
        inline size_t getColumnImageHeight(int column) const;

        /** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
        inline double get(size_t column, size_t row) const ;

        /** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
        inline double get(int column, size_t row) const ;
        /** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
        inline double get(int column, int row) const ;
        /** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
        inline double get(size_t column, int row) const ;
         /** \brief gets the index of the datapoint with the nearest, but lower value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column)*/
        int getNextLowerIndex(size_t column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the datapoint with the nearest, but lower value in the column */
        int getNextLowerIndex(size_t column, size_t row) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column) */
        int getNextHigherIndex(size_t column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column */
        int getNextHigherIndex(size_t column, size_t row) const;
        /** \brief gets the index of the datapoint with the nearest, but lower value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column)*/
        int getNextLowerIndex(int column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the datapoint with the nearest, but lower value in the column */
        int getNextLowerIndex(int column, size_t row) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column (in a given inclusive row range [start ... end] values of -1 for the ranges are "wildcards", i.e. start/end of column) */
        int getNextHigherIndex(int column, size_t row,  int start,  int end) const;
        /** \brief gets the index of the  datapoint with the nearest, but higher value in the column */
        int getNextHigherIndex(int column, size_t row) const;

        /** \brief sets the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!) */
        inline void set(size_t column, size_t row, double value);
        /** \brief sets the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!) */
        inline void set(int column, size_t row, double value);
        /** \brief adds a value \a value to the column \a column. This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards */
        inline void appendToColumn(size_t column, double value);
        /** \brief adds a values in cintainer \a values to the column \a column. This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards
         *
         *  \tparam TContainer a container with standard iterators
         *  \param colum the column to extend
         *  \param vales vector with data to append to column \a column
         */
        template<class TContainer>
        inline void appendFromContainerToColumn(size_t column, const TContainer& values);

        /** \brief returns the value at position (\c x, \c y) in the \a column-th column, which is interpreted with the imageWidth stored in that column  */
        inline double getPixel(size_t column, size_t x, size_t y) const ;
        /** \brief returns the value at position (\c x, \c y) in the \a column-th column, which is interpreted with the imageWidth stored in that column  */
        inline void setPixel(size_t column, size_t x, size_t y, double value)  ;
        /** \brief sets all entries in column \a column to \a value
         *
         * \see \ref JKQTPlotterFilledGraphs
         */
        inline void setAll(size_t column, double value);
        /** \brief scales (multiplies) all entries in column \a column by \a factor
         *
         * \see \ref JKQTPlotterFilledGraphs
         */
        inline void scaleColumnValues(size_t column, double factor);
        /** \brief increases entry in row \a row of column \a column by \a increment
         *
         * \see \ref JKQTPlotterFilledGraphs
         */
        inline void inc(size_t column, size_t row, double increment=1);
        /** \brief decrements entry in row \a row of column \a column by \a decrement
         *
         * \see \ref JKQTPlotterFilledGraphs
         */
        inline void dec(size_t column, size_t row, double decrement=1);



        /** \brief add a new columns with \a rows rows to the datastore and return its column ID. The new item uses internal memory management.
         *   \param rows number of rows in the data array
         *   \param name name for the column
         *   \return the ID of the newly created column
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addColumn(size_t rows, const QString& name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. This returns its logical column ID.
         *         Data is not owned by the JKQTPDatastore!
         *
         *   \param data data array to be copied
         *   \param rows number of rows in the data array
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         * \code
         *     #define NDATA 5
         *    double XCA[NDATA]= { 1, 2, 3, 4, 5 };
         *    double YCA[NDATA]= { 1, 0, 1, 0, 1 };
         *    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
         *    linegraph->setXColumn(datastore->addColumn(XCA, NDATA, "xca (C-array)"));
         *    linegraph->setYColumn(datastore->addColumn(YCA, NDATA, "yca (C-array)"));
         * \endcode
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addColumn(double* data, size_t rows, const QString& name=QString(""));
        /** \brief add a column with \a rows entries from the array \a data,
         *         ownership of the memory behind \a data is transfered to the datastore
         *
         *   \param data data array to be copied
         *   \param rows number of rows in the data array
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         * \code
         *    #define NDATA 5
         *    double* XCA=(double*)malloc(NDATA, sizeof(double));
         *    double* YCA=(double*)malloc(NDATA, sizeof(double));
         *    ...
         *    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
         *    linegraph->setXColumn(datastore->addInternalColumn(XCA, NDATA, "x"));
         *    linegraph->setXColumn(datastore->addInternalColumn(YCA, NDATA, "y"));
         * \endcode
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addInternalColumn(double *data, size_t rows, const QString& name);


        /** \brief add a new columns with \a width * \a height rows to the datastore and return its column ID. The new item uses internal memory management.
         *         The column is meant to represent an image in row-major order with x-dimention \a width and y-dimension \a height .
         *
         *   \param width width of the image represented by the data array
         *   \param height height of the image represented by the data array
         *   \param name name for the column
         *   \return the ID of the newly created column
         *  \see addImageColumn(), addInternalImageColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addImageColumn(size_t width, size_t height, const QString& name=QString(""));

        /** \brief add one external column to the datastore. It contains \a width * \a height rows. This returns its logical column ID.
         *         Data is not owned by the JKQTPDatastore!
         *         The column is meant to represent an image in row-major order with x-dimention \a width and y-dimension \a height .
         *
         *
         *   \param data data array to be copied
         *   \param width width of the image represented by the data array
         *   \param height height of the image represented by the data array
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         *  \see addColumn(), addImageColumn(), addInternalImageColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addImageColumn(double* data, size_t width, size_t height, const QString& name=QString(""));
        /** \brief add a column with \a width * \a height entries from the array \a data,
         *         ownership of the memory behind \a data is transfered to the datastore
         *         The column is meant to represent an image in row-major order with x-dimention \a width and y-dimension \a height .
         *
         *
         *   \param data data array to be copied
         *   \param width width of the image represented by the data array
         *   \param height height of the image represented by the data array
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         *  \see addInternalColumn(), addImageColumn(), addInternalImageColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addInternalImageColumn(double *data, size_t width, size_t height, const QString& name);



        /** \brief copies the given \a old_column into a new one, reading the data with the given start column and stride
         *
         *  \param old_column the column to be duplicated
         *  \param start for row in column \a old_column to copy
         *  \param stride stride for iterating through \a old_column when copying
         *  \param name name for the new column
         *  \return ID of the newly created column
         *
         *  Pseuo-Code:
         *  \code
         *    newColumn=addColumn(rowcount(old_column), name)
         *    forall ( r: rows(old_column)) {
         *      set(newColumn, r, get(old_column, r))
         *    }
         *    return newColumn;
         *  \endcode
         */
        size_t copyColumn(size_t old_column, size_t start, size_t stride, const QString& name=QString(""));
        /** \brief copies the given \a old_column into a new one
         *
         *  \param old_column the column to be duplicated
         *  \param name name for the new column
         *  \return ID of the newly created column
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t copyColumn(size_t old_column, const QString& name=QString(""));


        /** \brief add one external column to the datastore. It will be filled with the contents of vector \a data.
         *
         *   \tparam TContainer datatype of the container, which need to support standard C++ iterators and the function \c size(). The contents needs to be convertible to double.
         *   \param data data vector to be copied
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         * \code
         *    QVector<double> X, Y;
         *    const int Ndata=100;
         *    for (int i=0; i<Ndata; i++) {
         *        const double x=double(i)/double(Ndata)*8.0*M_PI;
         *        X<<x;
         *        Y<<sin(x);
         *    }
         *    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
         *    // by calling JKQTPDatastore::addCopiedColumn() the data is COPIED from the vector into the datastore
         *    linegraph->setXColumn(datastore->addCopiedColumn(X, "x"));
         *    linegraph->setYColumn(datastore->addCopiedColumn(Y, "y"));
         * \endcode
         *
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
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
         *                      The iterator of TContainer needs to support \c ++ and \c +=
         *   \param data data vector to be copied
         *   \param name name for the column
         *   \param stride strides through the container \a data with the given stride
         *   \param start starts copying from \a data with the element \a start
         *   \return the ID of the newly created column
         *
         * Pseudocode:
         * \code
         *     it=data.begin();
         *     it += start; // shift by start items
         *     while (it!=data.end()) {
         *       newColumn.push_back(jkqtp_todouble(*it));
         *       it += stride;
         *     }
         * \endcode
         *
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
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
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
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
         * Pseudocode:
         * \code
         *     for (i=start; i<rows; i+=stride) {
         *       newColumn.push_back(jkqtp_todouble(data[i]));
         *     }
         * \endcode

         *  \note This function converts the input array \a data into an array of double!
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
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
         *
         * Pseudocode:
         * \code
         *     for (i=0; i<rows; i+=stride) {
         *       newColumn.push_back(jkqtp_todouble(data[i]));
         *     }
         * \endcode

         *  \note This function converts the input array \a data into an array of double!
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        template<typename T>
        size_t addCopiedColumn(const T* data, size_t rows, size_t stride, const QString& name) {
            return addCopiedColumn<T>(data, rows, stride, 0, name);
        }


        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a row-major image and is copied as such. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        template <typename T>
        inline size_t addCopiedImageAsColumn(const T* data, size_t width, size_t height, const QString& name=QString(""), size_t stride=1, size_t start=0);

        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a row-major image and is copied as such. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *   \tparam TContainer datatype of the container, which need to support standard C++ iterators and the function \c size(). The contents needs to be convertible to double.
         *   \param data data vector to be copied
         *   \param width width of the image, stored in \a data
         *   \param name name for the column
         *   \return the ID of the newly created column
         */
        template <typename TContainer>
        inline size_t addCopiedImageAsColumn(const TContainer& data, size_t width, const QString& name=QString(""));


        /** \brief add a new column to the datastore, which is filled from the transposed column-major array \a data with
         *         the given \a width and \a height.
         *
         * The external data is assumed to be organized as a column-major image and is copied as row-major (i.e. is transposed).
         * The external data is copied to an internal array, so afterwards you can delete the external arrayThis returns its logical column ID.
         *
         * \tparam T data type of the array \a data, needs to be convertible to \c double by jkqtp_todouble()
         * \param data a column major image
         * \param width width of \a data
         * \param height height of \a data
         * \param name name of the new column
         * \param stride stride to use, when reading \a data. Use this to e.g. read one channel from a packed RGB-image (\c stride=3, \c start=0/1/2 )
         * \param start first entry from \a data top copy \a data. Use this to e.g. read one channel from a packed RGB-image (\c stride=3, \c start=0/1/2 )
         * \return ID of the newly added column
         */
        template <typename T>
        size_t addCopiedImageAsColumnTranspose(const T* data, size_t width, size_t height, const QString& name=QString(""), size_t stride=1, size_t start=0);


        /** \brief add one external column to the datastore. It contains \a width * \a height rows.
         *
         * The external data is assumed to be organized as a column-major image and is copied as row-major (i.e. is transposed).
         * The external data is copied to an internal array, so afterwards you can delete the external array This returns its
         * logical column ID.
         *
         *   \tparam T data type of the array \a data, needs to be convertible to \c double by jkqtp_todouble()
         *   \param data data vector to be copied
         *   \param width width of the image, stored in \a data
         *   \param name name for the column
         *   \return the ID of the newly created column
         */
        template <typename T>
        inline size_t addCopiedImageAsColumnTranspose(const QVector<T>& data, size_t width, const QString& name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
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


            size_t col= addInternalColumn(d, rrs, name);
            return col;
        }

        /** \brief add one external column to the datastore. It will be filled with the contents of vector \a data.
         *
         *   \tparam TContainer datatype of the container \a data, which need to support standard C++ iterators and the function \c size(). The contents needs to be convertible to double.
         *   \tparam TContainerMask datatype of the container \a mask, which need to support standard C++ iterators and the function \c size(). The contents needs to be convertible to bool.
         *   \param data data vector to be copied
         *   \param mask data vector to be copied
         *   \param name name for the column
         *   \param stride strides through the container \a data with the given stride
         *   \param start starts copying from \a data with the element \a start
         *   \return the ID of the newly created column
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore
         *
         * Pseudocode:
         *  \code
         *    for (i=0; i<data.size(); i++) {
         *      if (static_cast<bool>(mask[i])==useIfMaskEquals) {
         *        newColumn.push_back(jkqtp_todouble(data[i]));
         *      }
         *    }
         *    return newColumn;
         *  \endcode
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
                    if (static_cast<bool>(*itmask)==useIfMaskEquals) {
                        d[rrs]=jkqtp_todouble(*itdata);
                        rrs++;
                    }
                    ++itmask;
                    ++itdata;
                }
            }
            size_t col= addInternalColumn(d, rrs, name);
            return col;

        }


        /** \brief copies the contents of the map-like container \a c into two columns of the datastore,
         *         returns the two IDs of the items as a std::pair
         *  \see \ref JKQTPlotterAdvancedJKQTPDatastore, jkqtp_todouble()
         *
         *   \tparam TContainer datatype of the map-typed container (e.g. \c std::map or \c QMap ) The requiremen to this container is
         *                      that it supports standard iterators with \c begin() and \c end() .
         *   \param c the map to copy to the datastore
         *   \param nameKey name for the column with the map keys
         *   \param nameValue name for the column with the map values
         *   \return a pair of IDs to the newly created columns (IDkeyColumn, IDvalueColumn)
         * Example of usage:
         * \code
         *     std::map<int, double> datamap;
         *    datamap[1]=1.1;
         *    datamap[2]=1.4;
         *    datamap[4]=1.2;
         *    datamap[5]=1.8;
         *    datamap[7]=0.9;
         *    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
         *    linegraph->setXYColumns(datastore->addCopiedMap(datamap, "map_x", "map_y"));
         *    linegraph->setTitle(QObject::tr("copied map"));
         * \endcode
         */
        template <typename TContainer>
        std::pair<size_t, size_t> addCopiedMap(const TContainer& c, const QString& nameKey=QString("map_key"), const QString& nameValue=QString("map_value")) {
            const size_t N=std::distance(c.begin(), c.end());
            double* xvals=static_cast<double*>(malloc(N*sizeof(double)));
            double* yvals=static_cast<double*>(malloc(N*sizeof(double)));
            size_t i=0;
            for (auto it=c.begin(); it!=c.end(); ++it) {
                xvals[i]=(jkqtp_todouble(it->first));
                yvals[i]=(jkqtp_todouble(it->second));
                i++;
            }
            const size_t cx=addInternalColumn(xvals, N, nameKey);
            const size_t cy=addInternalColumn(yvals, N, nameValue);
            return std::pair<size_t, size_t>(cx,cy);
        }





        /** \brief add a column to the datastore that contains \a rows rows with monotonely increasing value starting at \a start and ending at \a end.
         *         the values are equidistant between \a start end \a end
         *  \see addLogColumn(), addDecadeLogColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addLinearColumn(size_t rows, double start, double end, const QString& name=QString(""));
        /** \brief add a column to the datastore that contains \a rows rows with monotonely increasing value starting at \a start and ending at \a end.
         *         the values are logarithmically spaced between \a start end \a end
         *  \see addLinearColumn(), addDecadeLogColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addLogColumn(size_t rows, double start, double end, const QString& name=QString(""));
        /** \brief add a column to the datastore that contains \a rows rows with monotonely increasing value starting at 10^start and ending at 10^end.
         *         the values are logarithmically spaced between 10^start end 10^end
         *  \see addLinearColumn(), addLogColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addDecadeLogColumn(size_t rows, double startDecade, double endDecade, const QString& name=QString(""));


        /** \brief add two columns to the datastore that contains the x- and y- coordinates of a rectangular grid with \a width points in x- and \a height
         *         points in y-direction.
         *
         *  \param width number of columns in the mesh grid
         *  \param startX x-coordinate of the first column of the mesh grid
         *  \param endX x-coordinate of the last column of the mesh grid
         *  \param height number of rows in the mesh grid
         *  \param startY y-coordinate of the first row of the mesh grid
         *  \param endY y-coordinate of the last row of the mesh grid
         *  \param nameX name for the x-coordinate column
         *  \param nameY name for the y-coordinate column
         *  \return IDs of two column that contain the x- and y- coordinates od the mesh points (in row-major order), where the
         *          x-coordinates are linearly distributed between \a startX and \a endX and the x-coordinates are linearly
         *          distributed between \a startY and \a endY .
         *
         *  \see addLogGridColumns(), addDecadeLogGridColumns(), addColumnCalculatedFromColumn(), JKQTPXYParametrizedScatterGraph, \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        std::pair<size_t,size_t> addLinearGridColumns(size_t width, double startX, double endX, size_t height, double startY, double endY, const QString& nameX=QString(""), const QString& nameY=QString(""));


        /** \brief add a column with \a rows entries, that is calculated by calling \a f for each entry
         *
         * Pseudocode:
         * \code
         *   for (i=0; i<rows; i++) {
         *     newColumn.push_back(f(i, this));
         *   }
         *   return newColumn;
         * \endcode
         *
         *  \see addColumnCalculatedFromColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addCalculatedColumn(size_t rows, const std::function<double(size_t, JKQTPDatastore*)>& f, const QString& name=QString(""));
        /** \brief add a column with \a rows entries, that is calculated by calling \a f for each entry
         *
         * Pseudocode:
         * \code
         *   for (i=0; i<rows; i++) {
         *     newColumn.push_back(f(i));
         *   }
         *   return newColumn;
         * \endcode
         *
         *  \see addColumnCalculatedFromColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addCalculatedColumn(size_t rows, const std::function<double(size_t)>& f, const QString& name=QString(""));
        /** \brief add a column with the same number of entries, as in the other column \a otherColumn , that are calculated by calling \a f for each entry in \a otherColumn
         *
         * Pseudocode:
         * \code
         *   for (i=0; i<rows(otherColumn); i++) {
         *     newColumn.push_back(f(getValue(otherColumn, i));
         *   }
         *   return newColumn;
         * \endcode
         *
         *  \see addCalculatedColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addColumnCalculatedFromColumn(size_t otherColumn, const std::function<double(double)>& f, const QString& name=QString(""));
        /** \brief add a column with the same number of entries, as in the other column \a otherColumn , that are calculated by calling \a f for each pair of entries in \a otherColumnX and  \a otherColumnY
         *
         * Pseudocode:
         * \code
         *   for (i=0; i<min(rows(otherColumnX), rows(otherColumnY)); i++) {
         *     newColumn.push_back(f(getValue(otherColumnX, i), getValue(otherColumnY, i));
         *   }
         *   return newColumn;
         * \endcode
         *
         *  \see addCalculatedColumn(), \ref JKQTPlotterAdvancedJKQTPDatastore
         */
        size_t addColumnCalculatedFromColumn(size_t otherColumnX, size_t otherColumnY, const std::function<double(double,double)>& f, const QString& name=QString(""));

        /** \brief returns the number of (logical) columns currently managed by the datastore */
        inline size_t getColumnCount() const { return static_cast<size_t>(columns.size()); }

        /** \brief returns a list with all available column IDs */
        inline QList<size_t> getColumnIDs() const { return columns.keys(); }
        /** \brief returns a list with all available column IDs */
        QVector<int> getColumnIDsIntVec() const;

        /** \brief return the num of the first column  with the given name, or -1 if none was found */
        int getColumnNum(const QString& name);

        /** \brief return the num of the first column  with the given name, if none was found this creates a new column with no rows and returns its num */
        int ensureColumnNum(const QString& name);


        /** \brief returns the maximum number of rows in all columns */
        size_t getMaxRows() const;

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
        void saveCSV(const QString& filename, const QSet<int>& userColumns=QSet<int>(), const QString& separator=QString(", "), const QString& decimal_separator=QString("."), const QString& comment=QString("#"), const QString& aroundStrings=QString(""), char floatformat='g') const;
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
        void saveCSV(QTextStream& txt, const QSet<int>& userColumns=QSet<int>(), const QString& separator=QString(", "), const QString& decimal_separator=QString("."), const QString& comment=QString("#"), const QString& aroundStrings=QString(""), char floatformat='g') const;

        /** \brief save contents of datastore as <a href="http://en.wikipedia.org/wiki/SYmbolic_LinK_(SYLK)">SYLK file (SYmbolic LinK)</a>
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         */
        void saveSYLK(const QString& filename, const QSet<int>& userColumns=QSet<int>(), const QString& floatformat=QString("%10.10lf")) const;

        /** \brief return contents of datastore as QList<QVector<double> >, i.e. a list of column-vectors
         *
         * \param columnNames if \c !=nullptr this will afterwards conatin the column titles
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        QList<QVector<double> > getData(QStringList* columnNames=nullptr, const QSet<int>& userColumns=QSet<int>()) const;

        /** \brief save contents of datastore as <a href="http://www.fileformat.info/format/dif/egff.htm">DIF file (data interchange format)</a>
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         */
        void saveDIF(const QString& filename, const QSet<int>& userColumns=QSet<int>(), const QString& floatformat=QString("%10.10lf")) const;

        /** \brief save contents of datastore as a Matlab script
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        void saveMatlab(const QString& filename, const QSet<int>& userColumns=QSet<int>()) const;
        /** \brief save contents of datastore as a Matlab script
         *
         * \param txt the QTextStream to write to
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        void saveMatlab(QTextStream& txt, const QSet<int>& userColumns=QSet<int>()) const;

        /** \brief return a list with all columns available in the datastore */
        QStringList getColumnNames() const;

        friend class JKQTPColumn;
        friend class JKQTPDatastoreItem;
        friend class JKQTPDatastoreModel;
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
    /** \brief is this item valid?/usable? */
    bool valid;
    /** \brief number of columns, if interpreted as a row-major image */
    size_t imageColumns;

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
    JKQTPColumn(JKQTPDatastore* datastore, const QString& name=QString(""), size_t datastoreItem=0, size_t datastoreOffset=0, size_t imageColumns=1);

    inline bool isValid() const { return valid; }

    /** \brief class destructor */
    ~JKQTPColumn() =default;

    /*! \brief sets the property name ( \copybrief name ) to the specified \a __value. \details Description of the parameter name is: <BLOCKQUOTE>\copydoc JKQTPColumn::name </BLOCKQUOTE> \see JKQTPColumn::name for more information */
    void setName (const QString& __value);
    /*! \brief returns the property name ( \copybrief name ). \see name for more information */
    QString getName () const;

    /*! \copydoc imageColumns */
    void setImageColumns (size_t __value);
    /*! \copydoc imageColumns */
    inline size_t getImageColumns () const { return imageColumns; }

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
     * This method accesses the datastore and returns the double value stored in the \c (y*width+x)'th row of the according
     * column.
     */
    inline void setPixelValue(size_t x, size_t y, size_t width, double val) {
        setValue(y*width+x, val);
    }


    /** \brief sets the element at (x,y) in the column, where the data is interpreted as a row-major ordered Matrix of the width imageWidth
     *
     * This method accesses the datastore and returns the double value stored in the \c (y*imageColumns+x)'th row of the according
     * column.
     */
    inline void setPixelValue(size_t x, size_t y, double val) {
        setValue(y*imageColumns+x, val);
    }

    /** \brief returns the element at (x,y) in the column, where the data is interpreted as a row-major ordered Matrix of the width imageWidth
     *
     * This method accesses the datastore and returns the double value stored in the \c (y*imageColumns+x)'th row of the according
     * column.
     */
    inline double getPixelValue(size_t x, size_t y) const {
        return getValue(y*imageColumns+x);
    }

    /** \brief returns a pointer to the datastore item representing this column */
    inline JKQTPDatastoreItem* getDatastoreItem()  { return datastore->getItem(datastoreItem); }
    /** \brief returns a pointer to the datastore item representing this column */
    inline const JKQTPDatastoreItem* getDatastoreItem() const { return datastore->getItem(datastoreItem); }

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
    enum class StorageType {
        Internal,
        External,
        Vector
    };
    /** \brief a pointer to the actual data */
    double* data;
    /** \brief iif \a storageType is \c StorageType::Vector, the data is actually save here and data contains a pointer to the data in datavec */
    QVector<double> datavec;
    /** \brief specifies whether the datastore manages the memory (\c true ) or whether the user application does this (\c false ) .*/
    StorageType storageType;
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
    JKQTPDatastoreItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows, bool storageType);
    /** \brief class destructor: frees unfreed internal memory */
    ~JKQTPDatastoreItem();

    /** \brief change the size of all columns to the givne number of rows. Returns \c true if the old data could be retained/saved and \c false if the old data was lost (which happens in most of the cases!) */
    bool resizeColumns(size_t rows);

    /*! \brief returns the property rows ( \copybrief rows ). \details Description of the parameter rows is:  <BLOCKQUOTE>\copydoc JKQTPDatastoreItem::JKQTPDatastoreItemrows </BLOCKQUOTE>. \see JKQTPDatastoreItem::rows for more information */ \
    inline size_t getRows() const
    {   return rows;   }
    /*! \brief returns the property columns ( \copybrief columns ). \details Description of the parameter columns is:  <BLOCKQUOTE>\copydoc JKQTPDatastoreItem::columns </BLOCKQUOTE>. \see JKQTPDatastoreItem::columns for more information */ \
    inline size_t getColumns() const
    {   return columns;   }


    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline double get(size_t column, size_t row) {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPDatastoreItemFormat::SingleColumn:
              return data[row];
            case JKQTPDatastoreItemFormat::MatrixColumn:
              return data[column*rows+row];
            case JKQTPDatastoreItemFormat::MatrixRow:
              return data[row*columns+column];
        }
        return 0;
    }


    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline double* getPointer(size_t column, size_t row) {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPDatastoreItemFormat::SingleColumn:
              return &(data[row]);
            case JKQTPDatastoreItemFormat::MatrixColumn:
              return &(data[column*rows+row]);
            case JKQTPDatastoreItemFormat::MatrixRow:
              return &(data[row*columns+column]);
        }
        return nullptr;
    }

    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline const double* getPointer(size_t column, size_t row) const {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPDatastoreItemFormat::SingleColumn:
              return &(data[row]);
            case JKQTPDatastoreItemFormat::MatrixColumn:
              return &(data[column*rows+row]);
            case JKQTPDatastoreItemFormat::MatrixRow:
              return &(data[row*columns+column]);
        }
        return nullptr;
    }
    /** \brief set the data at the position (\a column, \a row ) to \a value. The column index specifies the column inside THIS item, not the global column number. */
    inline void set(size_t column, size_t row, double value) {
        if (data!=nullptr) switch(dataformat) {
            case JKQTPDatastoreItemFormat::SingleColumn:
              data[row]=value;
              return;
            case JKQTPDatastoreItemFormat::MatrixColumn:
              data[column*rows+row]=value;
              return;
            case JKQTPDatastoreItemFormat::MatrixRow:
              data[row*columns+column]=value;
              return;
        }
    }

    /** \brief adds a new row to the given column. Returns \c true on success and \c false else
     *
     * This operation is currently only possible, if \c storageType==StorageType::Vector ! */
    inline bool push_back(size_t column, double value) {
        if (storageType==StorageType::Vector && dataformat==JKQTPDatastoreItemFormat::SingleColumn && column==0) {
            datavec.push_back(value);
            rows=static_cast<size_t>(datavec.size());
            data=datavec.data();
            return true;
        }
        return false;
    }

    /** \brief adds a new row to the given column. Returns \c true on success and \c false else
     *
     * This operation is currently only possible, if \c storageType==StorageType::Vector ! */
    inline bool append(size_t column, double value) {
        return push_back(column, value);
    }
    /** \brief adds new rows to the given column. Returns \c true on success and \c false else
     *
     * This operation is currently only possible, if \c storageType==StorageType::Vector ! */
    inline bool append(size_t column, const QVector<double>& values) {
        if (storageType==StorageType::Vector && dataformat==JKQTPDatastoreItemFormat::SingleColumn && column==0) {
            datavec.reserve(datavec.size()+values.size());
            for (const double& d: values) datavec.push_back(d);
            data=datavec.data();
            rows=static_cast<size_t>(datavec.size());
            return true;
        }
        return false;
    }
    /** \brief adds new rows to the given column. Returns \c true on success and \c false else
     *
     * This operation is currently only possible, if \c storageType==StorageType::Vector ! */
    inline bool append(size_t column, const std::vector<double>& values) {
        if (storageType==StorageType::Vector && dataformat==JKQTPDatastoreItemFormat::SingleColumn && column==0) {
            datavec.reserve(static_cast<int>(datavec.size())+static_cast<int>(values.size()));
            for (const double& d: values) datavec.push_back(d);
            data=datavec.data();
            rows=static_cast<size_t>(datavec.size());
            return true;
        }
        return false;
    }
};


/** \brief     QAbstractTableModel descendent that allows to view data in a JKQTPDatastore
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
    if (!datastore) return JKQTP_NAN;
    if (!datastore->getItem(datastoreItem)) return JKQTP_NAN;
    return datastore->getItem(datastoreItem)->get(datastoreOffset, n);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPColumn::getValue(int n) const {
    if (!datastore) return JKQTP_NAN;
    if (!datastore->getItem(datastoreItem)) return JKQTP_NAN;
    if (n<0) return JKQTP_NAN;
    return datastore->getItem(datastoreItem)->get(datastoreOffset, static_cast<size_t>(n));
}

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::getRows(size_t column) const {
    return columns.value(column).getRows();
}

////////////////////////////////////////////////////////////////////////////////////////////////
const double *JKQTPDatastore::getColumnPointer(int column, size_t row) const
{
    if (column<0) return nullptr;
    return columns.value(static_cast<size_t>(column)).getPointer(row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
double *JKQTPDatastore::getColumnPointer(int column, size_t row)
{
    if (column<0) return nullptr;
    return columns[static_cast<size_t>(column)].getPointer(row);
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

////////////////////////////////////////////////////////////////////////////////////////////////
size_t JKQTPDatastore::getRows(int column) const {
    if (column<0) return 0;
    return columns.value(static_cast<size_t>(column)).getRows();
}

////////////////////////////////////////////////////////////////////////////////////////////////
const double *JKQTPDatastore::getColumnPointer(size_t column, size_t row) const
{
    return columns.value(column).getPointer(row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
double *JKQTPDatastore::getColumnPointer(size_t column, size_t row)
{
    return columns[column].getPointer(row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPDatastore::get(size_t column, size_t row) const {
    return columns[column].getValue(row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPDatastore::get(int column, size_t row) const {
    if (column<0) return JKQTP_NAN;
    return get(static_cast<size_t>(column), static_cast<size_t>(row));
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPDatastore::get(int column, int row) const {
    if (column<0) return JKQTP_NAN;
    if (row<0) return JKQTP_NAN;
    return get(static_cast<size_t>(column), static_cast<size_t>(row));
}


////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPDatastore::get(size_t column, int row) const {
    if (row<0) return JKQTP_NAN;
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

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::appendToColumn(size_t column, double value)
{
    bool ok=columns[column].getDatastoreItem()->append(columns[column].getDatastoreOffset(), value);
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
template<class TContainer>
void JKQTPDatastore::appendFromContainerToColumn(size_t column, const TContainer &values)
{
    for(auto it=values.begin(); it!=values.end(); it++) {
        appendToColumn(column, *it);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double JKQTPDatastore::getPixel(size_t column, size_t x, size_t y) const {
    return columns.value(column).getPixelValue(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline void JKQTPDatastore::setPixel(size_t column, size_t x, size_t y, double value)  {
    return columns[column].setPixelValue(x, y, value);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::setAll(size_t column, double value)
{
    columns[column].setAll(value);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::scaleColumnValues(size_t column, double factor)
{
    columns[column].scale(factor);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::inc(size_t column, size_t row, double increment)
{
    columns[column].incValue(row, increment);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::dec(size_t column, size_t row, double decrement)
{
    columns[column].decValue(row, decrement);
}


////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline size_t JKQTPDatastore::addCopiedImageAsColumn(const T* data, size_t width, size_t height, const QString& name, size_t stride, size_t start){
    size_t col=addCopiedColumn<T>(data, width*height, stride, start, name);
    columns[col].setImageColumns(width);
    return col;
}

////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TContainer>
inline size_t JKQTPDatastore::addCopiedImageAsColumn(const TContainer& data, size_t width, const QString& name){
    size_t col= addCopiedColumn<TContainer>(data, name);
    columns[col].setImageColumns(width);
    return col;
}


////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
size_t JKQTPDatastore::addCopiedImageAsColumnTranspose(const T* data, size_t width, size_t height, const QString& name, size_t stride, size_t start){
    double* temp=static_cast<double*>(malloc(width*height*sizeof(double)));

    for (size_t x=0; x<width; x++) {
        for (size_t y=0; y<height; y++) {
            temp[x*height+y]=jkqtp_todouble<T>(data[start+(y*width+x)*stride]);
        }

    }

    size_t idx=addInternalColumn(temp, width*height, name);
    columns[idx].setImageColumns(height);
    return idx;
}


////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
inline size_t JKQTPDatastore::addCopiedImageAsColumnTranspose(const QVector<T>& data, size_t width, const QString& name) {
    return addCopiedImageAsColumnTranspose<T>(data.data(), width, static_cast<size_t>(data.size())/width, name);
}

#endif // JKQTPDATASTORAGE_H
