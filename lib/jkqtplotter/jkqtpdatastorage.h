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

#ifndef JKQTPDATASTORAGE_H
#define JKQTPDATASTORAGE_H


#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtpdebuggingtools.h"
#include "jkqtcommon/jkqttools.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <memory>
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
#include <stdexcept>

class JKQTPColumn; // forward declaration
class JKQTPColumnIterator; // forward declaration
class JKQTPColumnConstIterator; // forward declaration
class JKQTPDatastoreItem; // foward declaration
class JKQTPDatastoreModel; // forward declaration
class JKQTPColumnBackInserter; // forward declaration


/** \brief the types of data in one JKQTdatastoreItem
 * \ingroup jkqtpdatastorage_classes
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

/** \brief This class manages data columns (with entries of type \c double ), used by JKQTPlotter/JKQTBasePlotter to represent data for plots
  *  \ingroup jkqtpdatastorage_classes
  *
  *  \see \ref JKQTPlotterBasicJKQTPDatastore for a detailed description of how to use this class for data management!
  *
  *
  *  \section jkqtpdatastore_column_management Column Management
  *
  *  \subsection jkqtpdatastore_column_management_generation Column Generation/Deletion
  *
  *  A JKQTPDatastore manages a set of data columns. Each column is a continuous vector of numbers. Optionally it can be interpreted
  *  as a 2D image. In the latter case, the data is assumed to be ordered in row-major ordering (i.e. row for row).
  *
  *  You can generate new columns e.g. by:
  *  <ul>
  *    <li> addColumn() which either generates internally managed (and extensible) columns, or accepts a simple \c double* pointer
  *      to external. In the latter case owner-ship may be transferred to the datastore, but can also retain externally.
  *    <li> addImageColumn() adds a column that should be interpreted as an image with given width and height (i.e. a row-major
  *      matrix with \c width columns and \c height rows.
  *    <li> copyColumn() duplicates an existing column
  *    <li> addCopiedColumn() copies an external dataset into the datastore. e.g. with code like:
  *        \code{.cpp}
  *           QVector<double> X, Y;
  *           const int Ndata=100;
  *           for (int i=0; i<Ndata; i++) {
  *              const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
  *              X<<x;
  *              Y<<sin(x);
  *          }
  *          plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
  *          // by calling JKQTPDatastore::addCopiedColumn() the data is COPIED from the vector into the datastore
  *          linegraph->setXColumn(datastore->addCopiedColumn(X, "x"));
  *          linegraph->setYColumn(datastore->addCopiedColumn(Y, "y"));
  *        \endcode
  *   <li> addLinearColumn() adds a column with linearly increasing numbers (in a given range)
  *   <li> addLogColumn() and addDecadeLogColumn() add columns with logarithmically spaced values
  *   <li> addLinearGridColumns() adds two columns which represent x- and y- coordinates of points
  *     on a rectangular grid (useful for calculating image data)
  *   <li> addCalculatedColumn() calculates a column, based on row numbers and a C++ functor
  *   <li> addColumnCalculatedFromColumn() calculates a column, based on another column data
  *   <li> addCopiedMap() copies data from a std::map/QMap into two columns
  *   <li> ... several more functions for specific cases exist.
  *   <li> Also note that there are even library extensions that allow to import data directly from OpenCV matrices: JKQTPCopyCvMatToColumn()
  * </ul>
  *
  * Of course columns can also be deleted by calling:
  *   - deleteColumn()
  *   - deleteAllColumns()
  *   - deleteAllPrefixedColumns()
  *   - clear()
  * .
  *
  * \subsection jkqtpdatastore_column_management_props Column Properties
  *
  * The properties of columns may be accessed using:
  *   - getRows() returns the number of rows in a specific column
  *   - getColumnPointer() returns a pointer to the data in the column
  *   - getColumnChecksum() calculated a checksum over the data in the column
  *   - getColumnNames() / getColumnName()
  * .
  *
  * \subsection jkqtpdatastore_column_management_modify Modify Column Contents
  *
  * You can modify all data in a column by functions like:
  *   - setAll()
  *   - scaleColumnValues()
  * .
  *
  * Also you can access a single entry from a column, using:
  *   - set() / get() set/read a column entry
  *   - inc() / dec() increment/decrement a column entry
  *   - appendToColumn() (adds a single row/new value to a column, if the column was not internally managed before, it will be copied into a new internal memory segment by the first call to this function!)
  *   - appendToColumns() (adds several values to several columns simultaneously, i.e. a shortcut to prevents writing several consecutive appendToColumn() for each column separately)
  *   - appendFromContainerToColumn() (adds several rows from a container to a column, if the column was not internally managed before, it will be copied into a new internal memory segment by the first call to this function!)
  * .
  *
  * \subsection jkqtpdatastore_column_management_iterators Iterator Interface
  *
  * In addition to teh fucntions above, JKQTPDatastore also provides C++-style iteratos to read the data from a column:
  *   - begin()
  *   - end()
  * .
  *
  * ... and also a \c std::back_inserter -style interface to append data to columns:
  *   - backInserter()
  * .
  *
  * These allow to use the C++ standard algorithms to work with columns and also enabled the
  * library \ref jkqtptools_math_statistics in this software package. You can use the functions
  * above e.g. for code like:
  *
  * \code{.cpp}
  *     auto inserter it=datastore->backInserter(datastore->addColumn("new column"));
  *     for (auto it=datastore->begin(col1); it!=datastore->begin(col1) ++it) {
  *         *++inserter=sqrt(*it);
  *     }
  * \endcode
  *
  * or simply
  *
  * \code{.cpp}
  *     // mean of a column in a JKQTPDatastore:
  *     double mean=jkqtpstatAverage(datastore1->begin(col1), datastore1->end(col1));
  * \endcode
  *
  * Also there are functions to add data from iterator-defined ranges, e.g.:
  *   - addCopiedColumn()
  *   - addCopiedMap()
  *   - appendToColumn()
  * .
  *
  * \see \ref JKQTPlotterBasicJKQTPDatastoreIterators
  *
  *
  *
  * \subsection jkqtpdatastore_column_management_images Image Column
  *
  * JKQTPDatastore stores the width and height of the represented image as metadata with any image column.
  * This metadata is used to provide convenience access to the image pixels with:
  *   - setPixel()
  *   - getPixel()
  * .
  *
  * The image width/height are read using:
  *   - getColumnImageWidth()
  *   - getColumnImageHeight()
  * .
  *
  * This allows to write code like:
  * \code{.cpp}
  *     for (int iy=0; iy<10; iy++) {
  *         for (int ix=0; ix<10; ix++) {
  *             datastore->setPixel(imgColumn, ix, iy, sin(ix*iy/30.0));
  *         }
  *     }
  * \endcode
  *
  * \section jkqtpdatastore_dataio Data Output
  *
  * JKQTPDatastore provides several functions that allow to store its contents into files:
  *   - saveCSV()
  *   - saveSYLK()
  *   - saveMatlab()
  *   - saveDIF()
  * .
  *
  * ... and function to read data into different data structures:
  *   - getData()
  * .
  *
  *
  *
  *
  * \section jkqtpdatastore_internals Internal Working
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
class JKQTPLOTTER_LIB_EXPORT JKQTPDatastore{
    private:
        /** \brief a std::vector that contains all items managed by this datastore */
        QMap<size_t, JKQTPDatastoreItem*> items;
        /** \brief a std::vector of all columns that may be used to access the managed chunks of memory. */
        QMap<size_t, JKQTPColumn> columns;

        /** \brief an internal invalid column object, used e.g. to return invalid column iterators
         *  \internal
         */
        std::unique_ptr<JKQTPColumn> m_invalidColumn;

        /** \brief internal variable to keep track about the highest item ID (in items) used so far
         *  \internal
         *  \see items
         */
        size_t maxItemID;
        /** \brief internal variable to keep track about the highest column ID (in columns) used so far
         *  \internal
         *  \see columns
         */
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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addColumnForItem(size_t itemID, size_t columnInItem, const QString& name=QString(""));


        /** \brief mutable iterator for columns in the JKQTPDatastore (\c ColumnIterator::first: column number, \c ColumnIterator::second: column data) */
        typedef QMap<size_t, JKQTPColumn>::iterator ColumnIterator;
        /** \brief constant iterator for columns in the JKQTPDatastore (\c ColumnIterator::first: column number, \c ColumnIterator::second: column data) */
        typedef QMap<size_t, JKQTPColumn>::const_iterator ConstColumnIterator;

        /** \brief returns an iterator to the first column in the JKQTPDatastore \see ColumnIterator */
        ColumnIterator begin();
        /** \brief returns an iterator behind the last column in the JKQTPDatastore \see ColumnIterator */
        ColumnIterator end();
        /** \brief returns a const iterator to the first column in the JKQTPDatastore \see ConstColumnIterator */
        ConstColumnIterator begin() const;
        /** \brief returns a const iterator behind the last column in the JKQTPDatastore \see ConstColumnIterator */
        ConstColumnIterator end() const;
    public:

        /** \brief class constructor, generates an empty datastore */
        JKQTPDatastore();
        /** \brief class destructor, destroys all subordered JKQTPDatastoreItem objects */
        ~JKQTPDatastore();

        /** \brief determines whether a column  with the given ID exists */
        bool hasColumn(size_t i) const;

        /** \brief returns an iterator to the first data entry in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::iterator */
        JKQTPColumnIterator begin(int i);
        /** \brief returns an iterator behind the last data entry data in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::iterator */
        JKQTPColumnIterator end(int i);
        /** \brief returns a const iterator to the first data entry in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::const_iterator */
        JKQTPColumnConstIterator begin(int i) const;
        /** \brief returns a const iterator behind the last data entry data in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::const_iterator */
        JKQTPColumnConstIterator end(int i) const;

        /** \brief returns an iterator to the first data entry in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::iterator */
        JKQTPColumnIterator begin(size_t i);
        /** \brief returns an iterator behind the last data entry data in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::iterator */
        JKQTPColumnIterator end(size_t i);
        /** \brief returns a const iterator to the first data entry in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::const_iterator */
        JKQTPColumnConstIterator begin(size_t i) const;
        /** \brief returns a const iterator behind the last data entry data in the \a i -th column in the JKQTPDatastore \see JKQTPColumn::const_iterator */
        JKQTPColumnConstIterator end(size_t i) const;

        /** \brief removes the entry \a pos
         *
         *  \warning If the memory was externally managed before, it will be internally managed afterwards
         *
         *  \warning the iterator \a pos is rendered invalid by this column, as the in some cases the internal column is redefined!
         */
        inline void eraseFromColumn(const JKQTPColumnIterator &pos);
        /** \brief removes the entry \a pos
         *
         *  \warning If the memory was externally managed before, it will be internally managed afterwards
         *
         *  \warning the iterator \a pos is rendered invalid by this column, as the in some cases the internal column is redefined!
         */
        inline void eraseFromColumn(const JKQTPColumnConstIterator &pos);
        /** \brief removes the entries \a pos to \a posEnd
         *
         *  \warning If the memory was externally managed before, it will be internally managed afterwards
         *
         *  \warning the iterator \a pos is rendered invalid by this column, as the in some cases the internal column is redefined!
         */
        inline void eraseFromColumn(const JKQTPColumnConstIterator &pos, const JKQTPColumnConstIterator &posEnd);
        /** \brief removes the entries \a pos to \a posEnd
         *
         *  \warning If the memory was externally managed before, it will be internally managed afterwards
         *
         *  \warning the iterator \a pos is rendered invalid by this column, as the in some cases the internal column is redefined!
         */
        inline void eraseFromColumn(const JKQTPColumnIterator &pos, const JKQTPColumnIterator &posEnd);


        /** \brief returns a back-inserter iterator (JKQTPColumnBackInserter) to the \a i -th column in the JKQTPDatastore \see JKQTPColumnBackInserter */
        JKQTPColumnBackInserter backInserter(int i);
        /** \brief returns a back-inserter iterator (JKQTPColumnBackInserter) to the \a i -th column in the JKQTPDatastore \see JKQTPColumnBackInserter */
        JKQTPColumnBackInserter backInserter(size_t i);

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
        size_t getColumnImageWidth(int column) const;
        /** \brief returns the height of the image, represented by \a column (in row-major ordering) */
        size_t getColumnImageHeight(int column) const;
        /** \brief sets the height of the image, represented by \a column (in row-major ordering) to \a imageHeight */
        void setColumnImageHeight(size_t column, size_t imageHeight);
        /** \brief sets the width of the image, represented by \a column (in row-major ordering) to \a imageWidth */
        void setColumnImageWidth(size_t column, size_t imageWidth);

        /** \brief returns the data checksum of the given column */
        quint16 getColumnChecksum(int column) const;

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
        /** \brief copies the data from \a data into an existing column \a toColumn
         *
         *  \param toColumn target of the copy operation
         *  \param data data to copy
         *
         *  \warning If the memory in \a toColumn was externally managed before, it will be
         *           internally managed afterwards!
        */
        void setColumnData(size_t toColumn, const QVector<double>& data);
        /** \brief copies the data from \a data into an existing column \a toColumn
         *
         *  \param toColumn target of the copy operation
         *  \param data data to copy
         *  \param N entries in \a data
         *
         *  \warning If the memory in \a toColumn was externally managed before, it will be
         *           internally managed afterwards!
        */
        void setColumnCopiedData(size_t toColumn, const double* data, size_t N);
        /** \brief copies the image data from \a data into an existing column \a toColumn
         *
         *  \param toColumn target of the copy operation
         *  \param data data to copy, size is \a width * \a height
         *  \param width number of columns in \a data
         *  \param height number of rows in \a data
         *
         *  \warning If the memory in \a toColumn was externally managed before, it will be
         *           internally managed afterwards!
        */
        void setColumnCopiedImageData(size_t toColumn, const double* data, size_t width, size_t height);
        /** \brief adds a value \a value to the column \a column. This changes the column length (number of rows).
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see appendToColumns()
         */
        void appendToColumn(size_t column, double value);
        /** \brief resizes the column \a column to have \a new_rows rows
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see resizeImageColumn(), appendToColumn()
         */
        void resizeColumn(size_t column, size_t new_rows);
        /** \brief resizes the column \a column to have enough rows to be interpreted as an image of size \a new_image_width * \a new_image_height pixels, also sets the internally store image size in the column!
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see resizeColumn(), appendToColumn()
         */
        void resizeImageColumn(size_t column, size_t new_image_width, size_t new_image_height);
        /** \brief adds a value \a value1 to the column \a column1 and a value \a value2 to \a column2.
         *
         * This is equivalent to
         * \code
         *   appendToColumn(column1, value1);
         *   appendToColumn(column2, value2);
         * \endcode
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see appendToColumn()
         */
        void appendToColumns(size_t column1, size_t column2, double value1, double value2);

        /** \brief adds a the x-coordinate of \a value to the column \a columnX and the y-coordinate to \a columnY.
         *
         * This is equivalent to
         * \code
         *   appendToColumn(columnX, value.x());
         *   appendToColumn(columnY, value.y());
         * \endcode
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see appendToColumn()
         */
        void appendToColumns(size_t columnX, size_t columnY, const QPointF& value);

        /** \brief adds a the x-coordinate of \a value to the column \a columnX and the y-coordinate to \a columnY.
         *
         * This is equivalent to
         * \code
         *   appendToColumn(columnX, value.x());
         *   appendToColumn(columnY, value.y());
         * \endcode
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see appendToColumn()
         */
        void appendToColumns(size_t columnX, size_t columnY, const QPoint& value);

        /** \brief adds a value \a value1 to the column \a column1, a value \a value2 to \a column2, etc.
         *
         * This is equivalent to
         * \code
         *   appendToColumn(column1, value1);
         *   appendToColumn(column2, value2);
         *   appendToColumn(column3, value3);
         * \endcode
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see appendToColumn()
         */
        void appendToColumns(size_t column1, size_t column2, size_t column3, double value1, double value2, double value3);

        /** \brief adds a value \a value1 to the column \a column1, a value \a value2 to \a column2, etc.
         *
         * This is equivalent to
         * \code
         *   appendToColumn(column1, value1);
         *   appendToColumn(column2, value2);
         *   appendToColumn(column3, value3);
         *   appendToColumn(column4, value4);
         * \endcode
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see appendToColumn()
         */
        void appendToColumns(size_t column1, size_t column2, size_t column3, size_t column4, double value1, double value2, double value3, double value4);

        /** \brief adds a value \a value1 to the column \a column1, a value \a value2 to \a column2, etc.
         *
         * This is equivalent to
         * \code
         *   appendToColumn(column1, value1);
         *   appendToColumn(column2, value2);
         *   appendToColumn(column3, value3);
         *   appendToColumn(column4, value4);
         *   appendToColumn(column5, value5);
         * \endcode
         *
         * \warning This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards .
         *
         * \see appendToColumn()
         */
        void appendToColumns(size_t column1, size_t column2, size_t column3, size_t column4, size_t column5, double value1, double value2, double value3, double value4, double value5);




        /** \brief adds a values in container \a values to the column \a column. This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards
         *
         *  \tparam TContainer a container with standard iterators
         *  \param column the column to extend
         *  \param values vector with data to append to column \a column
         */
        template<class TContainer>
        inline void appendFromContainerToColumn(size_t column, const TContainer& values);
        /** \brief adds a values from a range \a first ... \a last (defined by C++ iterators) to the column \a column. This changes the column length (number of rows). If the memory was externally managed before, it will be internally managed afterwards
         *
         *  \tparam TIterator a standard C++ iterator
         *  \param column the column to extend
         *  \param first points to the first value in the range of values to add
         *  \param last points behind the last value in the range of values to add
         */
        template<class TIterator>
        inline void appendToColumn(size_t column, TIterator first, TIterator last);

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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addColumn(size_t rows, const QString& name=QString(""));

        /** \brief add a new and empty column to the datastore and return its column ID. The new item uses internal memory management.
         *   \param name name for the column
         *   \return the ID of the newly created column
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addColumn(const QString& name=QString(""));

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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addInternalColumn(double *data, size_t rows, const QString& name);


        /** \brief add a new columns with \a width * \a height rows to the datastore and return its column ID. The new item uses internal memory management.
         *         The column is meant to represent an image in row-major order with x-dimention \a width and y-dimension \a height .
         *
         *   \param width width of the image represented by the data array
         *   \param height height of the image represented by the data array
         *   \param name name for the column
         *   \return the ID of the newly created column
         *  \see addImageColumn(), addInternalImageColumn(), \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see addColumn(), addImageColumn(), addInternalImageColumn(), \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see addInternalColumn(), addImageColumn(), addInternalImageColumn(), \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t copyColumn(size_t old_column, const QString& name=QString(""));
        /** \brief copies the data from \a fromColumn into an existing column \a toColumn
         *
         *  \param toColumn target of the copy operation
         *  \param fromColumn source of the copy operation
         *
         *  \warning If the memory in \a toColumn was externally managed before, it will be
         *           internally managed afterwards!
        */
        void copyColumnData(size_t toColumn, size_t fromColumn);



        /** \brief add one column to the datastore. It will be filled with the values from \a first ... \a last
         *
         *  \tparam TIterator a standard C++ iterator
         *  \param first points to the first value in the range of values to add
         *  \param last points behind the last value in the range of values to add
         *  \param name name for the column
         *  \return the ID of the newly created column
         *
         * \code
         *    QVector<double> X, Y;
         *    const int Ndata=100;
         *    for (int i=0; i<Ndata; i++) {
         *        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
         *        X<<x;
         *        Y<<sin(x);
         *    }
         *    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
         *    // by calling JKQTPDatastore::addCopiedColumn() the data is COPIED from the vector into the datastore
         *    linegraph->setXColumn(datastore->addCopiedColumn(X.begin(), X.end(), "x"));
         *    linegraph->setYColumn(datastore->addCopiedColumn(Y.begin(), Y.end(), "y"));
         * \endcode
         *
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        template <typename TIterator>
        size_t addCopiedColumn(TIterator first, TIterator last, const QString& name=QString("")) {
            const size_t N=static_cast<size_t>(std::abs(std::distance(first,last)));
            double* d=static_cast<double*>(malloc(static_cast<size_t>(N)*sizeof(double)));
            if (N>0) {
                size_t r=0;
                for (auto it=first; it!=last; ++it) {
                    d[r]=jkqtp_todouble(*it);
                    r++;
                }
            }
            size_t itemid=addInternalItem(d, N);
            return addColumnForItem(itemid, 0, name);
        }

        /** \brief add one external column to the datastore. It will be filled with the contents of vector \a data.
         *
         *   \tparam TContainer datatype of the container, which need to support standard C++ iterators. The contents needs to be convertible to double.
         *   \param data data vector to be copied
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         * \code
         *    QVector<double> X, Y;
         *    const int Ndata=100;
         *    for (int i=0; i<Ndata; i++) {
         *        const double x=double(i)/double(Ndata)*8.0*JKQTPSTATISTICS_PI;
         *        X<<x;
         *        Y<<sin(x);
         *    }
         *    plot.addGraph(linegraph=new JKQTPXYLineGraph(&plot));
         *    // by calling JKQTPDatastore::addCopiedColumn() the data is COPIED from the vector into the datastore
         *    linegraph->setXColumn(datastore->addCopiedColumn(X, "x"));
         *    linegraph->setYColumn(datastore->addCopiedColumn(Y, "y"));
         * \endcode
         *
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        template <typename TContainer>
        size_t addCopiedColumn(const TContainer& data, const QString& name=QString("")) {
            return addCopiedColumn(data.begin(), data.end(), name);
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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
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
         *   \param rows items in data
         *   \param name name for the column
         *   \return the ID of the newly created column
         *
         *  \note This function converts the input array \a data into an array of double!
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        template<typename T>
        size_t addCopiedColumn(const T* data, size_t rows, const QString& name=QString("")){
            double* d=static_cast<double*>(malloc(static_cast<size_t>(rows)*sizeof(double)));
            if (d) {
                if (data) {
                    for (size_t r=0; r<rows; r++) {
                        d[r]=jkqtp_todouble(data[r]);
                    }
                }
                const size_t itemid=addInternalItem(d, rows);
                return addColumnForItem(itemid, 0, name);
            } else {
                throw std::runtime_error("could not allocate memory in JKQTPDataStore::addCopiedColumn()");
            }
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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
         */
        template<typename T>
        size_t addCopiedColumn(const T* data, size_t rows, size_t stride, int start, const QString& name) {
            double* d=static_cast<double*>(malloc(static_cast<size_t>(rows)*sizeof(double)));
            if (d) {
                if (data) {
                    for (size_t r=0; r<rows; r++) {
                        d[r]=jkqtp_todouble(data[static_cast<size_t>(start+static_cast<int64_t>(r*stride))]);
                    }
                }
                size_t itemid=addInternalItem(d, rows);
                return addColumnForItem(itemid, 0, name);
            } else {
                throw std::runtime_error("could not allocate memory in JKQTPDataStore::addCopiedColumn()");
            }
        }

        /** \brief copy an external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *   \tparam T datatype of the element in the vector, this has to be convertible to double!
         *   \param data pointer to the data to be copied
         *   \param rows items in data
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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
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
         *   \tparam T datatype of the element in the vector, this has to be convertible to double!
         *   \param data pointer to the data to be copied
         *   \param mask boolean array with \a rows entries, used to mask data when copying from \a data
         *   \param rows items in data
         *   \param name name for the column
         *   \param useIfMaskEquals data from \a data is copied if and only if the corresponding entry of \a mask equals this value
         *  \note This function converts the input array \a data into an array of double!
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
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
         *   \param useIfMaskEquals data from \a data is copied if and only if the corresponding entry of \a mask equals this value
         *   \return the ID of the newly created column
         *  \see \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see \ref JKQTPlotterBasicJKQTPDatastore, jkqtp_todouble()
         *
         *  \tparam TIterator a standard C++ iterator for a map, dereferencing with \a it->first and \a it->second
         *  \param first points to the first value in the range of values to add
         *  \param last points behind the last value in the range of values to add
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
         *    linegraph->setXYColumns(datastore->addCopiedMap(datamap.begin(), datamap.end(), "map_x", "map_y"));
         *    linegraph->setTitle(QObject::tr("copied map"));
         * \endcode
         */
       template <typename TIterator>
        std::pair<size_t, size_t> addCopiedMap(TIterator first, TIterator last, const QString& nameKey=QString("map_key"), const QString& nameValue=QString("map_value")) {
            const size_t N=static_cast<size_t>(std::abs(std::distance(first,last)));
            double* xvals=static_cast<double*>(malloc(N*sizeof(double)));
            double* yvals=static_cast<double*>(malloc(N*sizeof(double)));
            size_t i=0;
            for (auto it=first; it!=last; ++it) {
                xvals[i]=(jkqtp_todouble(it->first));
                yvals[i]=(jkqtp_todouble(it->second));
                i++;
            }
            const size_t cx=addInternalColumn(xvals, N, nameKey);
            const size_t cy=addInternalColumn(yvals, N, nameValue);
            return std::pair<size_t, size_t>(cx,cy);
        }

        /** \brief copies the contents of the map-like container \a c into two columns of the datastore,
         *         returns the two IDs of the items as a std::pair
         *  \see \ref JKQTPlotterBasicJKQTPDatastore, jkqtp_todouble()
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
            return addCopiedMap(c.begin(), c.end(), nameKey, nameValue);
        }




        /** \brief add a column to the datastore that contains \a rows rows with monotonely increasing value starting at \a start and ending at \a end.
         *         the values are equidistant between \a start end \a end
         *  \see addLogColumn(), addDecadeLogColumn(), \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addLinearColumn(size_t rows, double start, double end, const QString& name=QString(""));
        /** \brief add a column to the datastore that contains \a rows rows with monotonely increasing value starting at \a start and ending at \a end.
         *         the values are logarithmically spaced between \a start end \a end
         *  \see addLinearColumn(), addDecadeLogColumn(), \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addLogColumn(size_t rows, double start, double end, const QString& name=QString(""));
        /** \brief add a column to the datastore that contains \a rows rows with monotonely increasing value starting at 10^start and ending at 10^end.
         *         the values are logarithmically spaced between 10^start end 10^end
         *  \see addLinearColumn(), addLogColumn(), \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see addLogGridColumns(), addDecadeLogGridColumns(), addColumnCalculatedFromColumn(), JKQTPXYParametrizedScatterGraph, \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see addColumnCalculatedFromColumn(), \ref JKQTPlotterBasicJKQTPDatastore
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
         *  \see addColumnCalculatedFromColumn(), \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addCalculatedColumn(size_t rows, const std::function<double(size_t)>& f, const QString& name=QString(""));
        /** \brief add an image column with width \a cols and height \a rows (i.e. \a rows * \a cols entries), that is calculated by calling \a f for each entry
         *
         * Pseudocode:
         * \code
         *   for (y=0; y<rows; y++) {
         *     for (x=0; x<cols; x++) {
         *       newColumn.push_back(f(x,y));
         *     }
         *   }
         *   return newColumn;
         * \endcode
         *
         *  \see addColumnCalculatedFromColumn(), \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addCalculatedImageColumn(size_t cols, size_t rows, const std::function<double(size_t,size_t)>& f, const QString& name=QString(""));
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
         *  \see addCalculatedColumn(), \ref JKQTPlotterBasicJKQTPDatastore
         */
        size_t addColumnCalculatedFromColumn(size_t otherColumn, const std::function<double(double)>& f, const QString& name=QString(""));
        /** \brief add a column with the same number of entries, as in the other column \a otherColumn , that are calculated by calling \a f for each pair of entries in \a otherColumnX and  \a otherColumnY
         *
         * Pseudocode:
         * \code
         *   for (i=0; i<std::min(rows(otherColumnX), rows(otherColumnY)); i++) {
         *     newColumn.push_back(f(getValue(otherColumnX, i), getValue(otherColumnY, i));
         *   }
         *   return newColumn;
         * \endcode
         *
         *  \see addCalculatedColumn(), \ref JKQTPlotterBasicJKQTPDatastore
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

        /** \brief return the title of the first column  with the given name, or -1 if none was found */
        QString getColumnName(size_t column);

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

        /** \brief return contents of a given column as QVector<double>
         *
         * \param column column to copy
         * \param columnName if \c !=nullptr this will afterwards conatin the column title
         */
        QVector<double> getData(size_t column, QString* columnName=nullptr) const;

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





/** \brief internally stores information about one data column. See JKQTPDatastore for more information.
 * \ingroup jkqtpdatastorage_classes
 * \internal
 *
 * \see JKQTPDatastore
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPColumn {
  private:
    /** \brief index of the item in the datastore that contains the data for this column */
    size_t datastoreItem;
    /** \brief offset, if the datastore item contains more than one column */
    size_t datastoreOffset;
    /** \brief number of columns, if interpreted as a row-major image */
    size_t imageColumns;
    /** \brief pointer to the datastore object used to manage the data of the plot */
    JKQTPDatastore* datastore;
    /** \brief a name describing the column */
    QString name;
    /** \brief is this item valid?/usable? */
    bool valid;
  public:
    typedef JKQTPColumnIterator iterator;
    typedef JKQTPColumnConstIterator const_iterator;


    JKQTPColumn();
    /** \brief class constructor that binds the column to a specific datastore object.
     *
     * The use of this constructor is mandatory. The default constructor (no arguments) is hidden. Also note
     * that you cannot change the binding of a column to a datastore object after creation of the column.
     */
    JKQTPColumn(JKQTPDatastore* datastore, const QString& name=QString(""), size_t datastoreItem=0, size_t datastoreOffset=0, size_t imageColumns=1);

    inline bool isValid() const { return valid; }

    /** \brief two columns are equal, if the same memory in the same datastore is referenced */
    inline bool operator==(const JKQTPColumn& other) const {
        return (datastoreItem==other.datastoreItem)
                && (datastoreOffset==other.datastoreOffset)
                && (datastore==other.datastore)
                && (valid==other.valid);
    }

    /** \copydoc name */
    void setName (const QString& __value);
    /** \copydoc name */
    QString getName () const;

    /** \copydoc imageColumns */
    void setImageColumns (size_t imageWidth);
    /** \copydoc imageColumns */
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
    /** \brief returns a reference to the \a n -th row in this column (possibly throwing an exception if it does not exist!)
     *
     * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
     * column.
     *
     * \see iterator
     */
    inline double& at(int n);
    /** \brief returns a reference to the \a n -th row in this column (possibly throwing an exception if it does not exist!)
     *
     * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
     * column.
     *
     * \see const_iterator
     */
    inline const double& at(int n) const;
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
    void copy(const double* data, size_t N, size_t offset=0);

    /** \brief exchange every occurence of a given \a value by a \a replace value */
    void exchange(double value, double replace);

    /** \brief subtracts a given value from all members of the column */
    void subtract(double value);
    /** \brief scales all members of the column with the given factor */
    void scale(double factor);

    /** \brief set all values in the column to a specific \a value */
    void setAll(double value);

    /** \brief calculates a checksum over the contents of the column (using <a href="https://doc.qt.io/qt-5/qbytearray.html#qChecksum">qChecksum()</a>) */
    inline quint16 calculateChecksum() const;

    /** \copydoc datastoreItem */ \
    inline size_t getDatastoreItemNum() const  \
    {   return this->datastoreItem;   }
    /** \copydoc datastoreOffset */ \
    inline size_t getDatastoreOffset() const  \
    {   return this->datastoreOffset;   }

    /** \brief returns an iterator to the internal data
     * \see JKQTPColumnIterator */
    inline iterator begin();
    /** \brief returns an iterator to the internal data
     * \see JKQTPColumnIterator */
    inline iterator end();

    /** \brief returns an iterator to the internal data
     * \see JKQTPColumnIterator */
    inline const_iterator begin() const;
    /** \brief returns an iterator to the internal data
     * \see JKQTPColumnIterator */
    inline const_iterator end() const;


    friend class JKQTPDatastore;
    friend class JKQTPColumnIterator;
    friend class JKQTPColumnConstIterator;

    protected:
      inline JKQTPDatastore* getDatastore() { return datastore; }
      inline const JKQTPDatastore* getDatastore() const { return datastore; }
      inline void replaceMemory(size_t datastoreItem_=0, size_t datastoreOffset_=0) {
          datastoreItem=datastoreItem_;
          datastoreOffset=datastoreOffset_;
      }
      /** \brief removes the entry \a row */
      inline void erase(size_t row);
      /** \brief removes the entries \a row to \a rowEnd */
      inline void erase(size_t row, size_t rowEnd);

};

#pragma pack(push,1)
/** \brief iterator over the data in the column of a JKQTPDatastore
 * \ingroup jkqtpdatastorage_classes
 *
 * \see JKQTPColumn, JKQTPDatastore::begin(), JKQTPDatastore::end(), JKQTPDatastore, JKQTPConstColumnIterator
 */
class JKQTPColumnIterator {
    private:
        /** \brief references the column this iterator iterates over */
        JKQTPColumn* col_;
        /** \brief current row in col_ this iterator points to */
        int pos_;
    protected:
        /** \brief constructs an iterator for the data represented by \a col, starting with row \a startpos */
        inline JKQTPColumnIterator(JKQTPColumn* col, int startpos=0) : col_(col), pos_(startpos) {  }
    public:
        typedef JKQTPColumnIterator self_type;
        typedef double value_type;
        typedef double& reference;
        typedef const double& const_reference;
        typedef double* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        /** \brief constructs an invalid iterator */
        inline JKQTPColumnIterator() : col_(nullptr), pos_(-1) {  }
        inline JKQTPColumnIterator(const JKQTPColumnIterator&)=default;
        inline JKQTPColumnIterator(JKQTPColumnIterator&&)=default;
        inline JKQTPColumnIterator& operator=(const JKQTPColumnIterator&)=default;
        inline JKQTPColumnIterator& operator=(JKQTPColumnIterator&&)=default;
        inline self_type operator++(int /*junk*/) {
            self_type i = *this;
            if (!isValid()) pos_++;
            return i;
        }
        inline self_type operator++() {
            if (!isValid()) return self_type(col_);
            pos_++; return *this;
        }
        inline self_type operator--(int /*junk*/) {
            self_type i = *this;
            if (isValid()) {
                pos_--;
            } else {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())-1;
            }
            return i;
        }
        inline self_type operator--() {
            if (isValid()) {
                pos_--;
            } else {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())-1;
            }
            return *this;
        }
        inline self_type operator+=(int inc) {
            if (isValid()) {
                pos_+=inc;
            } else if (inc<0) {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())+inc;
            }
            return *this;
        }
        inline self_type operator-=(int dec) {
            if (isValid()) {
                pos_-=dec;
            } else {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())-dec;
            }

            return *this;
        }

        friend self_type operator+(difference_type off, const self_type& right) {
            if (right.isValid()) {
                return self_type(right.col_, off + right.pos_);
            } else {
              if (off<0) return self_type(right.col_, off + static_cast<int>(right.col_->getRows()));
              else return self_type(right.col_);
            }
        }
        friend self_type operator-(difference_type off, const self_type& right) {
            if (right.isValid()) {
                return self_type(right.col_, off - right.pos_);
            } else {
              return self_type(right.col_);
            }
        }
        inline self_type operator+(difference_type rhs) const {
            if (isValid()) {
                return self_type(col_, pos_+rhs);
            } else if (rhs<0){
                JKQTPASSERT(col_);
                return self_type(col_, static_cast<int>(col_->getRows())+rhs);
            } else {
                return self_type(col_);
            }
        }
        inline self_type operator-(difference_type rhs) const {
            if (isValid()) {
                return self_type(col_, pos_-rhs);
            } else {
                JKQTPASSERT(col_);
                return self_type(col_, static_cast<int>(col_->getRows())-rhs);
            }
        }
        inline difference_type operator-(self_type rhs) const {
            if (!isValid() && !rhs.isValid()) return 0;
            if (!isValid() && rhs.isValid() && col_==rhs.col_) return static_cast<difference_type>(col_->getRows())-rhs.pos_;
            if (isValid() && !rhs.isValid() && col_==rhs.col_) return pos_-static_cast<difference_type>(col_->getRows());
            JKQTPASSERT(isValid() && rhs.isValid() && col_==rhs.col_);
            return pos_-rhs.pos_;
        }

        /** \brief dereferences the iterator, throws an exception if the iterator is invalid (see isValid() ) or the value does not exist in the column */
        inline reference operator*() const {
            JKQTPASSERT((col_!=nullptr) && (pos_>=0) && (pos_<static_cast<int>(col_->getRows())));
            return col_->at(pos_);
        }
        inline reference operator[](difference_type off) const
        {
            if (!isValid() && off<0) {
                return col_->at(static_cast<int>(col_->getRows())+off);
            }
            JKQTPASSERT((col_!=nullptr) && (pos_+off>=0) && (pos_+off<static_cast<int>(col_->getRows())));
            return col_->at(pos_+off);
        }

        /** \brief comparison operator (equals)
         *
         * two iterators are equal, if:
         *   - they are both invalid (see isValid() )
         *   - they point to the same column and are both invalid (isValid()) or equal to end()
         *   - they point to the same column and the same row therein
         * .
         *
         * \see operator!=()
         * */
        inline bool operator==(const self_type& rhs) const {
            if (!isValid() && !rhs.isValid()) return true;
            if (col_ == rhs.col_) {
                if ((pos_<0 || pos_>=static_cast<int>(rhs.col_->getRows())) && (pos_<0 || rhs.pos_>=static_cast<int>(rhs.col_->getRows()))) return true;
                return pos_==rhs.pos_;
            }
            return false;
        }
        /** \brief comparison operator (unequals), inverse result of operator==()
         *
         * \see operator==()
         * */
        inline bool operator!=(const self_type& rhs) const { return !this->operator==(rhs); }
        /** \brief checks the iterator for validity (i.e. points to an existing column and position is in a valid range) */
        inline bool isValid() const {
            return col_ && pos_>=0 && pos_<static_cast<int>(col_->getRows());
        }
        /** \copydoc isValid() */
        inline operator bool() const { return isValid(); }
        /** \brief returns the referenced position/row, or -1 for an invalid iterator */
        inline int getPosition() const {
            if (!isValid()) return -1;
            return pos_;
        }
        /** \brief returns the referenced position/row interpreted as an image pixel, returns (-1,-1) for an invalid operator */
        inline QPoint getImagePosition() const {
            if (!isValid()) return QPoint(-1,-1);
            return QPoint(pos_ % static_cast<int>(col_->getImageColumns()), pos_ / static_cast<int>(col_->getImageColumns()));
        }
        /** \brief returns the referenced position/row interpreted as an image pixel, x-component, returns -1 for an invalid operator */
        inline int getImagePositionX() const {
            if (!isValid()) return -1;
            return pos_ % static_cast<int>(col_->getImageColumns());
        }
        /** \brief returns the referenced position/row interpreted as an image pixel, y-component, returns -1 for an invalid operator */
        inline int getImagePositionY() const {
            if (!isValid()) return -1;
            return pos_ / static_cast<int>(col_->getImageColumns());
        }
        /*! \brief if the data in the column is interpreted as an image, this is the number of columns (x-dimension) of the image
            \see JKQTPColumn::imageColumns */
        inline size_t getImageColumns () const {
            if (!isValid()) return 0;
            return col_->getImageColumns();
        }
        /*! \brief if the data in the column is interpreted as an image, this is the number of rows (y-dimension) of the image
            \see JKQTPColumn::imageColumns */
        inline size_t getImageRows () const {
            if (!isValid()) return 0;
            return col_->getRows() / col_->getImageColumns();
        }
        friend class JKQTPColumnConstIterator;
        friend class JKQTPColumn;
        friend class JKQTPDatastore;
    protected:
        /** \brief returns the referenced column */
        inline JKQTPColumn* getColumn() { return col_; }
        /** \brief returns the referenced column */
        inline const JKQTPColumn* getColumn() const { return col_; }
};






/** \brief iterator, which allows to insert into a column of a JKQTPDatastore
 * \ingroup jkqtpdatastorage_classes
 *
 * \see JKQTPDatastore::backInserter(), JKQTPDatastore, http://www.cplusplus.com/reference/iterator/insert_iterator/
 */
class JKQTPColumnBackInserter {
    private:
        /** \brief references the datastore to access */
        JKQTPDatastore* ds_;
        /** \brief references the column to access */
        size_t col_;
    protected:
        /** \brief constructs an iterator for the data represented by \a col, starting with row \a startpos */
        inline JKQTPColumnBackInserter(JKQTPDatastore* ds, size_t col) : ds_(ds), col_(col) {  }
    public:
        typedef JKQTPColumnBackInserter self_type;
        typedef double value_type;
        typedef double& reference;
        typedef const double& const_reference;
        typedef double* pointer;
        typedef std::output_iterator_tag iterator_category;
        typedef int difference_type;
        /** \brief constructs an invalid iterator */
        inline JKQTPColumnBackInserter() : ds_(nullptr), col_(0) {  }
        inline JKQTPColumnBackInserter(const JKQTPColumnBackInserter&)=default;
        inline JKQTPColumnBackInserter(JKQTPColumnBackInserter&&)=default;
        inline JKQTPColumnBackInserter& operator=(const JKQTPColumnBackInserter&)=default;
        inline JKQTPColumnBackInserter& operator=(JKQTPColumnBackInserter&&)=default;
        inline JKQTPColumnBackInserter& operator=(const double& val) {
            JKQTPASSERT(ds_);
            ds_->appendToColumn(col_, val);
            return *this;
        }
        inline JKQTPColumnBackInserter& operator=(double&& val) {
            JKQTPASSERT(ds_);
            ds_->appendToColumn(col_, std::move(val));
            return *this;
        }
        inline self_type operator++(int /*junk*/) { return *this; }
        inline self_type operator++() { return *this; }
        inline self_type operator*() {
            return *this;
        }
        friend class JKQTPDatastore;
};

/** \brief iterator over the data in the column of a JKQTPDatastore
 * \ingroup jkqtpdatastorage_classes
 *
 * \see JKQTPColumn, JKQTPDatastore::begin(), JKQTPDatastore::end(), JKQTPDatastore, JKQTPColumnIterator
 */
class JKQTPColumnConstIterator {
    private:
        /** \brief references the column this iterator iterates over */
        const JKQTPColumn* col_;
        /** \brief current row in col_ this iterator points to */
        int pos_;
    protected:
        /** \brief constructs an iterator for the data represented by \a col, starting with row \a startpos */
        inline JKQTPColumnConstIterator(const JKQTPColumn* col, int startpos=0) : col_(col), pos_(startpos) {  }
    public:
        typedef JKQTPColumnConstIterator self_type;
        typedef double value_type;
        typedef const double& reference;
        typedef reference const_reference;
        typedef const double* pointer;
        typedef std::forward_iterator_tag iterator_category;
        typedef int difference_type;
        /** \brief constructs an invalid iterator */
        inline JKQTPColumnConstIterator() : col_(nullptr), pos_(-1) {  }
        inline JKQTPColumnConstIterator(const JKQTPColumnConstIterator&)=default;
        inline JKQTPColumnConstIterator(JKQTPColumnConstIterator&&)=default;
        inline JKQTPColumnConstIterator(const JKQTPColumnIterator& rhs): col_(rhs.col_), pos_(rhs.pos_) {}
        inline JKQTPColumnConstIterator(JKQTPColumnIterator&& rhs):
            col_(std::move(rhs.col_)), pos_(std::move(rhs.pos_))
        {
            rhs.col_=nullptr;
            rhs.pos_=-1;
        }
        inline JKQTPColumnConstIterator& operator=(const JKQTPColumnConstIterator&)=default;
        inline JKQTPColumnConstIterator& operator=(JKQTPColumnConstIterator&&)=default;
        inline JKQTPColumnConstIterator& operator=(const JKQTPColumnIterator&rhs){
            col_=rhs.col_;
            pos_=rhs.pos_;
            return *this;
        }
        inline JKQTPColumnConstIterator& operator=(JKQTPColumnIterator&&rhs){
            col_=rhs.col_;
            pos_=rhs.pos_;
            rhs.col_=nullptr;
            rhs.pos_=-1;
            return *this;
        }
        inline self_type operator++(int /*junk*/) {
            self_type i = *this;
            if (!isValid()) pos_++;
            return i;
        }
        inline self_type operator++() {
            if (!isValid()) return self_type(col_);
            pos_++; return *this;
        }
        inline self_type operator--(int /*junk*/) {
            self_type i = *this;
            if (isValid()) {
                pos_--;
            } else {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())-1;
            }
            return i;
        }
        inline self_type operator--() {
            if (isValid()) {
                pos_--;
            } else {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())-1;
            }
            return *this;
        }
        inline self_type operator+=(int inc) {
            if (isValid()) {
                pos_+=inc;
            } else if (inc<0) {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())+inc;
            }
            return *this;
        }
        inline self_type operator-=(int dec) {
            if (isValid()) {
                pos_-=dec;
            } else {
                JKQTPASSERT(col_);
                pos_=static_cast<int>(col_->getRows())-dec;
            }

            return *this;
        }

        friend self_type operator+(difference_type off, const self_type& right) {
            if (right.isValid()) {
                return self_type(right.col_, off + right.pos_);
            } else {
              if (off<0) return self_type(right.col_, off + static_cast<int>(right.col_->getRows()));
              else return self_type(right.col_);
            }
        }
        friend self_type operator-(difference_type off, const self_type& right) {
            if (right.isValid()) {
                return self_type(right.col_, off - right.pos_);
            } else {
              return self_type(right.col_);
            }
        }
        inline self_type operator+(difference_type rhs) const {
            if (isValid()) {
                return self_type(col_, pos_+rhs);
            } else if (rhs<0){
                JKQTPASSERT(col_);
                return self_type(col_, static_cast<int>(col_->getRows())+rhs);
            } else {
                return self_type(col_);
            }
        }
        inline self_type operator-(difference_type rhs) const {
            if (isValid()) {
                return self_type(col_, pos_-rhs);
            } else {
                JKQTPASSERT(col_);
                return self_type(col_, static_cast<int>(col_->getRows())-rhs);
            }
        }
        inline difference_type operator-(self_type rhs) const {
            if (!isValid() && !rhs.isValid()) return 0;
            if (!isValid() && rhs.isValid() && col_==rhs.col_) return static_cast<difference_type>(col_->getRows())-rhs.pos_;
            if (isValid() && !rhs.isValid() && col_==rhs.col_) return pos_-static_cast<difference_type>(col_->getRows());
            JKQTPASSERT(isValid() && rhs.isValid() && col_==rhs.col_);
            return pos_-rhs.pos_;
        }

        /** \brief dereferences the iterator, throws an exception if the iterator is invalid (see isValid() ) or the value does not exist in the column */
        inline reference operator*() {
            JKQTPASSERT(col_!=nullptr && pos_>=0 && pos_<static_cast<int>(col_->getRows()));
            return col_->at(pos_);
        }
        inline reference operator[](difference_type off) const
        {
            if (!isValid() && off<0) {
                JKQTPASSERT(col_!=nullptr);
                return col_->at(static_cast<int>(col_->getRows())+off);
            }
            JKQTPASSERT(col_!=nullptr && pos_+off>=0 && pos_+off<static_cast<int>(col_->getRows()));
            return col_->at(pos_+off);
        }
        /** \brief dereferences the iterator, throws an exception if the iterator is invalid (see isValid() ) or the value does not exist in the column */
        inline const_reference operator*() const {
            JKQTPASSERT(col_!=nullptr && pos_>=0 && pos_<static_cast<int>(col_->getRows()));
            return col_->at(pos_);
        }

        /** \brief comparison operator (equals)
         *
         * two iterators are equal, if:
         *   - they are both invalid (see isValid() )
         *   - they point to the same column and are both invalid (isValid()) or equal to end()
         *   - they point to the same column and the same row therein
         * .
         *
         * \see operator!=()
         * */
        inline bool operator==(const self_type& rhs) const {
            if (!isValid() && !rhs.isValid()) return true;
            if (col_ == rhs.col_) {
                if ((pos_<0 || pos_>=static_cast<int>(rhs.col_->getRows())) && (pos_<0 || rhs.pos_>=static_cast<int>(rhs.col_->getRows()))) return true;
                return pos_==rhs.pos_;
            }
            return false;
        }
        /** \brief comparison operator (unequals), inverse result of operator==()
         *
         * \see operator==()
         * */
        inline bool operator!=(const self_type& rhs) const { return !this->operator==(rhs); }

        /** \brief checks the iterator for validity (i.e. points to an existing column and position is in a valid range) */
        inline bool isValid() const {
            return col_ && pos_>=0 && pos_<static_cast<int>(col_->getRows());
        }
        /** \copydoc isValid() */
        inline operator bool() const { return isValid(); }
        /** \brief returns the referenced position/row, or -1 for an invalid iterator */
        inline int getPosition() const {
            if (!isValid()) return -1;
            return pos_;
        }
        /** \brief returns the referenced position/row interpreted as an image pixel, returns (-1,-1) for an invalid operator */
        inline QPoint getImagePosition() const {
            if (!isValid()) return QPoint(-1,-1);
            return QPoint(pos_ % static_cast<int>(col_->getImageColumns()), pos_ / static_cast<int>(col_->getImageColumns()));
        }
        /** \brief returns the referenced position/row interpreted as an image pixel, x-component, returns -1 for an invalid operator */
        inline int getImagePositionX() const {
            if (!isValid()) return -1;
            return pos_ % static_cast<int>(col_->getImageColumns());
        }
        /** \brief returns the referenced position/row interpreted as an image pixel, y-component, returns -1 for an invalid operator */
        inline int getImagePositionY() const {
            if (!isValid()) return -1;
            return pos_ / static_cast<int>(col_->getImageColumns());
        }
        /*! \brief if the data in the column is interpreted as an image, this is the number of columns (x-dimension) of the image
            \see JKQTPColumn::imageColumns */
        inline size_t getImageColumns () const {
            if (!isValid()) return 0;
            return col_->getImageColumns();
        }
        /*! \brief if the data in the column is interpreted as an image, this is the number of rows (y-dimension) of the image
            \see JKQTPColumn::imageColumns */
        inline size_t getImageRows () const {
            if (!isValid()) return 0;
            return col_->getRows() / col_->getImageColumns();
        }

        friend class JKQTPColumn;
        friend class JKQTPDatastore;
    protected:
        /** \brief returns the referenced column */
        inline const JKQTPColumn* getColumn() const { return col_; }
};


/** \brief this represents one chunk of memory which can represent one or more columns of data for JKQTBasePlotter.
 *         See JKQTPDatastore for more information.
 * \ingroup jkqtpdatastorage_classes
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
class JKQTPLOTTER_LIB_EXPORT JKQTPDatastoreItem {
  private:
    /** \brief how data is represented in this JKQTPDatastoreItem */
    enum class StorageType {
        Internal, /*!< \brief data is stored in an internally managed (=owned, i.e. freed in the destructor) C-array */
        External, /*!< \brief data is stored in an externally managed (=not owned) C-array */
        Vector    /*!< \brief data is stored in the internal \a QVector<double> datavec */
    };
    /** \brief a pointer to the actual data */
    double* data;
    /** \brief as data may also point to a matrix, this specifies the number of columns in this element (default: 1) */
    size_t columns;
    /** \brief number of rows in this item */
    size_t rows;
    /** \brief iif \a storageType is \c StorageType::Vector, the data is actually save here and data contains a pointer to the data in datavec */
    QVector<double> datavec;
    /** \brief memory format of the data in this item */
    JKQTPDatastoreItemFormat dataformat;
    /** \brief specifies whether the datastore manages the memory (\c true ) or whether the user application does this (\c false ) .*/
    StorageType storageType;
    /** \brief Specifies whether memory for the data has been allocated. This is only used, when \c internal==true. */
    bool allocated;
  protected:
    /** \brief hidden default constructor */
    JKQTPDatastoreItem();

  public:
    /** \brief class constructor: initializes the object for internal data storage */
    JKQTPDatastoreItem(size_t columns, size_t rows);
    /** \brief class constructor: initializes the object for internal data storage with the given data */
    JKQTPDatastoreItem(const QVector<double> &data);
    /** \brief class constructor: initializes the object for external data storage */
    JKQTPDatastoreItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows);
    /** \brief class constructor: initializes the object for external data storage */
    JKQTPDatastoreItem(JKQTPDatastoreItemFormat dataformat, double* data, size_t columns, size_t rows, bool storageType);
    /** \brief class destructor: frees unfreed internal memory */
    ~JKQTPDatastoreItem();

    /** \brief change the size of all columns to the givne number of rows. Returns \c true if the old data could be retained/saved and \c false if the old data was lost (which happens in most of the cases!) */
    bool resizeColumns(size_t rows);

    /** \copydoc JKQTPDatastoreItem::rows */
    inline size_t getRows() const
    {   return rows;   }
    /** \copydoc JKQTPDatastoreItem::columns */
    inline size_t getColumns() const
    {   return columns;   }

    /** \brief checks whether dataformat==JKQTPDatastoreItemFormat::SingleColumn and storageType==StorageType::Vector */
    inline bool isVector() const {
        return dataformat==JKQTPDatastoreItemFormat::SingleColumn && storageType==StorageType::Vector;
    }

    /** \brief if \c isValid() : resize the row to have \a rows_new rows */
    inline void resize(size_t rows_new) {
        JKQTPASSERT(isVector());
        datavec.resize(static_cast<int>(rows_new));
        rows=static_cast<size_t>(datavec.size());
        data=datavec.data();
    }

    /** \brief if \c isValid() : erase the row \a row */
    inline void erase(size_t row) {
        JKQTPASSERT(isVector());
        datavec.erase(datavec.begin()+row, datavec.end());
        rows=static_cast<size_t>(datavec.size());
        data=datavec.data();
    }
    /** \brief if \c isValid() : erase all rows (and including) from  \a row to \a rowEnd */
    inline void erase(size_t row, size_t rowEnd) {
        if (row>rowEnd) erase(rowEnd, row);
        else if (row==rowEnd) erase(row);
        else {
            JKQTPASSERT(isVector());
            if (rowEnd>=static_cast<size_t>(datavec.size())) datavec.erase(datavec.begin()+row, datavec.end());
            else datavec.erase(datavec.begin()+row, datavec.begin()+rowEnd);
            rows=static_cast<size_t>(datavec.size());
            data=datavec.data();
        }
    }
    /** \brief returns the data at the position (\a column, \a row ).
     *
     * \note The column index specifies the column inside THIS item, not the global column number. */
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


    /** \brief returns a reference to the data at the position (\a column, \a row ). Throws an exception when the entry does not exist!
     *
     * \note The column index specifies the column inside THIS item, not the global column number. */
    inline double& at(size_t column, size_t row) {
        if (data!=nullptr) {
            switch(dataformat) {
                case JKQTPDatastoreItemFormat::SingleColumn:
                  return data[row];
                case JKQTPDatastoreItemFormat::MatrixColumn:
                  return data[column*rows+row];
                case JKQTPDatastoreItemFormat::MatrixRow:
                  return data[row*columns+column];
            }
        }
        throw std::out_of_range("index does not exist in JKQTPDatastoreItem");
    }

    /** \brief returns a const reference to the data at the position (\a column, \a row ). Throws an exception when the entry does not exist!
     *
     * \note The column index specifies the column inside THIS item, not the global column number. */
    inline const double& at(size_t column, size_t row) const {
        if (data!=nullptr) {
            switch(dataformat) {
                case JKQTPDatastoreItemFormat::SingleColumn:
                  return data[row];
                case JKQTPDatastoreItemFormat::MatrixColumn:
                  return data[column*rows+row];
                case JKQTPDatastoreItemFormat::MatrixRow:
                  return data[row*columns+column];
            }
        }
        throw std::out_of_range("index does not exist in JKQTPDatastoreItem");
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
#pragma pack(pop)


/** \brief     QAbstractTableModel descendent that allows to view data in a JKQTPDatastore
 * \ingroup jkqtpdatastorage_classes
 *
 * \see JKQTPDatastore
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPDatastoreModel: public QAbstractTableModel {
        Q_OBJECT
    public:
        JKQTPDatastoreModel(JKQTPDatastore* datastore, QObject* parent=nullptr);
        virtual ~JKQTPDatastoreModel() override;


        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const  override;

    public Q_SLOTS:
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
quint16 JKQTPColumn::calculateChecksum() const
{
    if (!datastore) return 0;
    if (!datastore->getItem(datastoreItem)) return 0;
    return jkqtp_checksum(reinterpret_cast<const char*>(getPointer(0)), static_cast<uint>(getRows()*sizeof(double)));
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::iterator JKQTPColumn::begin() {
    return JKQTPColumn::iterator(this, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::iterator JKQTPColumn::end() {
    return JKQTPColumn::iterator(this, -1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::const_iterator JKQTPColumn::begin() const {
    return JKQTPColumn::const_iterator(this, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////
JKQTPColumn::const_iterator JKQTPColumn::end() const {
    return JKQTPColumn::const_iterator(this, -1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::erase(size_t row) {
    datastore->getItem(datastoreItem)->erase(row);
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPColumn::erase(size_t row, size_t rowEnd) {
    datastore->getItem(datastoreItem)->erase(row, rowEnd);
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
inline const double& JKQTPColumn::at(int n) const {
    JKQTPASSERT(datastore && datastore->getItem(datastoreItem));
    JKQTPASSERT(n>=0);
    return datastore->getItem(datastoreItem)->at(datastoreOffset, static_cast<size_t>(n));
}

////////////////////////////////////////////////////////////////////////////////////////////////
inline double& JKQTPColumn::at(int n) {
    JKQTPASSERT(datastore && datastore->getItem(datastoreItem));
    JKQTPASSERT(n>=0);
    return datastore->getItem(datastoreItem)->at(datastoreOffset, static_cast<size_t>(n));
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
template<class TContainer>
void JKQTPDatastore::appendFromContainerToColumn(size_t column, const TContainer &values)
{
    appendToColumn(column, values.begin(), values.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////
template<class TIterator>
inline void JKQTPDatastore::appendToColumn(size_t column, TIterator first, TIterator last)
{
    for(auto it=first; it!=last; it++) {
        appendToColumn(column, *it);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::eraseFromColumn(const JKQTPColumnIterator &pos) {
    if (pos.isValid()) {
        bool ok=pos.getColumn()->getDatastoreItem()->isVector();
        auto itc=columns.key(*pos.getColumn());
        if (!ok) {
            QVector<double> old_data;
            for (auto it=pos.getColumn()->begin(); it!=pos; ++it) old_data.push_back(*it);


            size_t itemID=addItem(new JKQTPDatastoreItem(old_data));
            columns[itc].replaceMemory(itemID, 0);
        } else {
            columns[itc].erase(static_cast<size_t>(pos.getPosition()));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::eraseFromColumn(const JKQTPColumnConstIterator &pos) {
    if (pos.isValid()) {
        bool ok=pos.getColumn()->getDatastoreItem()->isVector();
        auto itc=columns.key(*pos.getColumn());
        if (!ok) {
            QVector<double> old_data;
            for (auto it=pos.getColumn()->begin(); it!=pos; ++it) old_data.push_back(*it);


            size_t itemID=addItem(new JKQTPDatastoreItem(old_data));
            columns[itc].replaceMemory(itemID, 0);
        } else {
            columns[itc].erase(static_cast<size_t>(pos.getPosition()));
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::eraseFromColumn(const JKQTPColumnIterator &pos, const JKQTPColumnIterator &posEnd) {
    if (pos.isValid()) {
        bool ok=pos.getColumn()->getDatastoreItem()->isVector();
        auto itc=columns.key(*pos.getColumn());
        if (!ok) {
            QVector<double> old_data;
            for (auto it=pos.getColumn()->begin(); it!=pos.getColumn()->end(); ++it) old_data.push_back(*it);
            size_t itemID=addItem(new JKQTPDatastoreItem(old_data));
            columns[itc].replaceMemory(itemID, 0);
        }
        if (posEnd.isValid()) columns[itc].erase(static_cast<size_t>(pos.getPosition()), static_cast<size_t>(posEnd.getPosition()));
        else columns[itc].erase(static_cast<size_t>(pos.getPosition()), static_cast<size_t>(pos.getPosition())+columns[itc].getRows()+1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
void JKQTPDatastore::eraseFromColumn(const JKQTPColumnConstIterator &pos, const JKQTPColumnConstIterator &posEnd) {
    if (pos.isValid()) {
        bool ok=pos.getColumn()->getDatastoreItem()->isVector();
        auto itc=columns.key(*pos.getColumn());
        if (!ok) {
            QVector<double> old_data;
            for (auto it=pos.getColumn()->begin(); it!=pos.getColumn()->end(); ++it) old_data.push_back(*it);
            size_t itemID=addItem(new JKQTPDatastoreItem(old_data));
            columns[itc].replaceMemory(itemID, 0);
        }
        if (posEnd.isValid()) columns[itc].erase(static_cast<size_t>(pos.getPosition()), static_cast<size_t>(posEnd.getPosition()));
        else columns[itc].erase(static_cast<size_t>(pos.getPosition()), static_cast<size_t>(pos.getPosition())+columns[itc].getRows()+1);
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
