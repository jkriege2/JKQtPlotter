/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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



/**
 * \defgroup jkqtpdatastorage Data Storage Classes/System
 * \ingroup jkqtplotter
 *
 * The classes in this group implement a data storage system for use with the main plotter class.
 * Basically a table of data is generated as a set of logical columns that may be bound to different
 * data sources (internal or external memory arrays. Later on it is simply possible to plot graphs
 * using the column number and the not a link to the actual data array, as the link is stored in these
 * classes.
 */

/** \file jkqtpdatastorage.h
  * \ingroup jkqtpdatastorage
  */

#include "jkqtp_imexport.h"
#include "jkqtptools.h"
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


class JKQTPcolumn; // forward declaration
class JKQTPdatastoreItem; // foward declaration
class JKQTPdatastoreModel; // forward declaration


/** \brief the types of data in one JKQTdatastoreItem
 * \ingroup jkqtpdatastorage
 *
 * \c JKQTPsingleColumn:
 * \verbatim
+-----+-----+-----+....................+-----+
|  0  |  1  |  2  |                    + N-1 |
+-----+-----+-----+....................+-----+
\endverbatim
 *
 *
 * \c JKQTPmatrixColumn:
 * \verbatim
 ================= COLUMN 1 =================  ================= COLUMN 2 =================
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
|  0  |  1  |  2  |                    + N-1 ||  0  |  1  |  2  |                    + N-1 |
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
 =R1== =R2== =R3==                      =RN==  =R1== =R2== =R3==                      =RN==
\endverbatim
 *
 *
 * \c JKQTPmatrixRow (C standard representation of matrices):
 * \verbatim
 ================== ROW  1 ==================  ================== ROW  2 ==================
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
|  0  |  1  |  2  |                    + N-1 ||  0  |  1  |  2  |                    + N-1 |
+-----+-----+-----+....................+-----++-----+-----+-----+....................+-----+ .....
 =C1== =C2== =C3==                      =CN==  =C1== =C2== =C3==                      =CN==
\endverbatim
 */
enum JKQTPdatastoreItemFormat {
	JKQTPsingleColumn,                /*!< \brief a 1D vector of doubles. (default option) */
	JKQTPmatrixColumn,                /*!< \brief a 1D vector of double that represents a number of columns. The data is store column after column. */
	JKQTPmatrixRow                    /*!< \brief a 1D vector of double that represents a number of rows (C standard representation of matrices). The data is stored row after row.*/
};

/** \brief This class manages chunks of memory that are used for column data in JKQtPlotterBase descendents
 * \ingroup jkqtpdatastorage
 *
 * This class manages a list if JKQTPdatastoreItem onjects that may each contain a chunk of memory, containig
 * one or more columns of data. Each item can be accessed with get() by a specific ID which is returned by add().
 * JKQTPcolumn. You may only clear all chunks of memory/items. If you no longer need some of the data, but still want
 * to access the rest you will simply have to destroy all JKQTPcolumn that point to the item with their
 * JKQTPcolumns:datastoreItem property.
 *
 * \verbatim

+- JKQTPdatastore ---------------------+               std::vector<JKQTPcolumn>:
|                                      |                   +- JKQTPcolumn ----------------+
|    +- JKQTPdatastoreItem --------+   |                 0 |   datastore                  |
|  0 |     JKQTPsingleColumn       |<--|-------------------|---datastoreItem = 0          |
|    |   0 # # # # # # # # # # #<--|---|-------------------|---datastoreOffset = 0        |
|    |                             |   |                   |                              |
|    +- JKQTPdatastoreItem --------+   |                   +- JKQTPcolumn ----------------+
|  1 |     JKQTPsingleColumn       |<--|---\             1 |   datastore                  |
|    |   0 # # # # # # # # # # #<--|---|-\  \--------------|---datastoreItem = 1          |
|    |                             |   |  \----------------|---datastoreOffset = 0        |
|    +- JKQTPdatastoreItem --------+   |                   |                              |
|    :                             :   |                   +------------------------------+
|    :                             :   |                   :                              :
|    :                             :   |                   :                              :
|    +- JKQTPdatastoreItem --------+   |                   :                              :
|N-1 |     JKQTPmatrixRow          |<--|---\               :                              :
|    |   0 # # # # # # # # # # #   |   |    \              +- JKQTPcolumn ----------------+
|    |   1 # # # # # # # # # # #   |   |     \         M-1 |   datastore                  |
|    |   2 # # # # # # # # # # #<--|---|--\   \------------|---datastoreItem = N-1        |
|    |                             |   |   \---------------|---datastoreOffset = 2        |
|    +-----------------------------+   |                   |                              |
|                                      |                   +------------------------------+
+--------------------------------------+


\endverbatim
 *
 * In addition the JKQTPdatastore manages a std::vector<JKQTPcolumn> which may be used to access the data chunks in the logical
 * notion of data columns. This class provides a set of interface methods for this list:
 *
 */
class LIB_EXPORT JKQTPdatastore{
    private:
        /** \brief a std::vector that contains all items managed by this datastore */
        QMap<size_t, JKQTPdatastoreItem*> items;
        /** \brief a std::vector of all columns that may be used to access the managed chunks of memory. */
        QMap<size_t, JKQTPcolumn> columns;

        size_t maxItemID;
        size_t maxColumnsID;
    protected:

        /** \brief add a new column to the datastore and return its ID */
        size_t addColumn(JKQTPcolumn col);


    public:
        /** \brief class constructor, generates an empty datastore */
        JKQTPdatastore();
        /** \brief class destructor, destroys all subordered JKQTPdatastoreItem objects */
        ~JKQTPdatastore() {
            clear();
        }

        /** \brief deletes all items from the datastore and possibly frees the memory they manage */
        void clear();

        /** \brief returns the JKQTPdatastoreItem object for the \a i -th item in the store */
        inline JKQTPdatastoreItem* getItem(size_t i) const {
            return items.value(i, NULL);
        }

        /** \brief add a new item to the datastore and return its ID */
        size_t addItem(JKQTPdatastoreItem* item);


        /** \brief add a new columns/item with \a rows rows to the datastore and return its ID. The item uses internal memory management. */
        size_t addItem(unsigned long long rows);

        /** \brief delete the given column, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteColumn(size_t column, bool removeItems=true);

        /** \brief delete all columns with the given name, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteAllColumns(QString name, bool removeItems=true);

        /** \brief delete all columns where the name starts with a given prefix, if no other columns points to the datastore item of the column and \a removeItems is \c true, the item will be removed */
        void deleteAllPrefixedColumns(QString prefix, bool removeItems=true);

        /** \brief add a new item with \a rows rows and \a columns columns to the datastore and return its ID. The item uses internal memory management. */
        size_t addItem(size_t columns, unsigned long long rows);

        /** \brief add one external column to the datastore. It contains \a rows rows.*/
        size_t addItem(double* data, unsigned long long rows);
        /** \brief add one internal column to the datastore. It contains \a rows rows.*/
        size_t addInternalItem(double* data, unsigned long long rows);

        /** \brief add an external memory block to the datastore. It contains \a rows rows and \a columns columns. \a dataformat determined the memory layout*/
        size_t addItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columns, unsigned long long rows);

        /** \brief add one external column to the datastore. It contains \a rows rows. The data is copied and the copy managed internally */
        size_t addCopiedItem(const double *data, unsigned long long rows);

        /** \brief add one external data block to the datastore. It contains \a rows rows and \a columns columns. The data is copied and the copy managed internally */
        size_t addCopiedItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columns, unsigned long long rows);



        /** \brief returns the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!)  */
        double get(size_t column, unsigned long long row);

        /** \brief sets the value at position (\c column, \c row). \c column is the logical column and will be mapped to the according memory block internally!) */
        void set(size_t column, unsigned long long row, double value);



        /** \brief add a new columns which references a specified item and a specified column therein. */
        size_t addColumnForItem(size_t itemID, size_t columnInItem, QString name=QString(""));

        /** \brief add a new columns with \a rows rows to the datastore and return its column ID. The new item uses internal memory management. */
        size_t addColumn(unsigned long long rows, QString name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. This returns its logical column ID.*/
        size_t addColumn(double* data, unsigned long long rows, QString name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedColumn(const double* data, unsigned long long rows, QString name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedColumn(const double* data, unsigned long long rows, unsigned long long stride, QString name=QString(""));


        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedColumn(const QVector<double>& data, QString name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedColumn(const QVector<double>& data, QString name, unsigned long long stride);


        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedColumn(const QVector<float>& data, QString name=QString(""));



        /** \brief copies the given \A old_column into a new one, reading the data with the given start column and stride */
        size_t copyColumn(size_t old_column, unsigned long long start, unsigned long long stride, QString name=QString(""));

        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const bool* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const uint8_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const uint16_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const uint32_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const int8_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const int16_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const int32_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const int64_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const uint64_t* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumn(const float* data, unsigned long long rows, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumnMasked(const float* data, const bool* mask, unsigned long long rows, QString name=QString(""), bool useIfMaskEquals=false);

        /** \brief add one external column to the datastore. It contains \a rows rows. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.
         *
         *  \note This function converts the input array \a data into an array of double!
         */
        size_t addCopiedColumnMasked(const double* data, const bool* mask, unsigned long long rows, QString name=QString(""), bool useIfMaskEquals=false);




        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a row-major image and is copied as such. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedImageAsColumn(double* data, unsigned long long width, unsigned long long height, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a row-major image and is copied as such. The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedImageAsColumn(bool* data, unsigned long long width, unsigned long long height, QString name=QString(""));
        /** \brief add one external column to the datastore. It contains \a width * \a height rows. The external data is assumed to be organized as a column-major image and is copied as row-major (i.e. is transposed). The external data is copied to an internal array, so
         *         afterwards you can delete the external arrayThis returns its logical column ID.*/
        size_t addCopiedImageAsColumnTranspose(double* data, unsigned long long width, unsigned long long height, QString name=QString(""));




        /** \brief add a column to the datastore that contains \a rows rows with increasing value starting at \a start and ending at \a end.
         *         the values are equidistant between \a start end \a end */
        size_t addLinearColumn(unsigned long long rows, double start, double end, QString name=QString(""));

        /** \brief returns the number of (logical) columns currently managed by the datastore */
        inline size_t getColumnCount() { return columns.size(); }

        inline QList<size_t> getColumnIDs() { return columns.keys(); }

        /** \brief return the num of the first column  with the given name, or -1 if none was found */
        int getColumnNum(QString name);

        /** \brief return the num of the first column  with the given name, if none was found this creates a new column with no rows and returns its num */
        size_t ensureColumnNum(QString name);

        /** \brief returns the JKQTPcolumn object for the \a i -th column in the store */
        JKQTPcolumn getColumn(size_t i);

        /** \brief returns the maximum number of rows in all columns */
        unsigned long long getMaxRows();

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
        void saveCSV(QString filename, QSet<int> userColumns=QSet<int>(), QString separator=QString(", "), QString decimal_separator=QString("."), QString comment=QString("#"), QString aroundStrings=QString(""), char floatformat='g');
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
        void saveCSV(QTextStream& txt, QSet<int> userColumns=QSet<int>(), QString separator=QString(", "), QString decimal_separator=QString("."), QString comment=QString("#"), QString aroundStrings=QString(""), char floatformat='g');

        /** \brief save contents of datastore as <a href="http://en.wikipedia.org/wiki/SYmbolic_LinK_(SYLK)">SYLK file (SYmbolic LinK)</a>
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         */
        void saveSYLK(QString filename, QSet<int> userColumns=QSet<int>(), QString floatformat=QString("%10.10lf"));

        /** \brief return contents of datastore as QList<QVector<double> >, i.e. a list of column-vectors
         *
         * \param columnNames if \c !=NULL this will afterwards conatin the column titles
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        QList<QVector<double> > getData(QStringList* columnNames=NULL, QSet<int> userColumns=QSet<int>());

        /** \brief save contents of datastore as <a href="http://www.fileformat.info/format/dif/egff.htm">DIF file (data interchange format)</a>
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         * \param floatformat a \c printf format string that is used to print floating point numbers to the file
         */
        void saveDIF(QString filename, QSet<int> userColumns=QSet<int>(), QString floatformat=QString("%10.10lf"));

        /** \brief save contents of datastore as a Matlab script
         *
         * \param filename the file to create
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        void saveMatlab(QString filename, QSet<int> userColumns=QSet<int>());
        /** \brief save contents of datastore as a Matlab script
         *
         * \param txt the QTextStream to write to
         * \param userColumns a list of all columns to export, an empty list means: export all, the indexes in the list refer to getColumnsNames()
         */
        void saveMatlab(QTextStream& txt, QSet<int> userColumns=QSet<int>());

        /** \brief return a list with all columns available in the datastore */
        QStringList getColumnNames() const;
        size_t addInternalColumn(double *data, unsigned long long rows, QString name);
};


/** \brief stores information about one data column. See JKQTPdatastore for more information.
 * \ingroup jkqtpdatastorage
 *
 * \see JKQTPdatastore
 */
class LIB_EXPORT JKQTPcolumn {
  private:
    /** \brief index of the item in the datastore that contains the data for this column */
    size_t datastoreItem;
    /** \brief offset, if the datastore item contains more than one column */
    unsigned long long datastoreOffset;
    /** \brief a name describing the column */
    QString name;
    /** \brief pointer to the datastore object used to manage the data of the plot */
    JKQTPdatastore* datastore;

    bool valid;

  protected:
    JKQTPGET_MACRO(JKQTPdatastore*, datastore)
  public:
    JKQTPcolumn() {
        datastore=NULL;
        name="";
        datastoreItem=0;
        datastoreOffset=0;
        valid=false;
    };
    /** \brief class constructor that binds the column to a specific datastore object.
     *
     * The use of this constructor is mandatory. The default constructor (no arguments) is hidden. Also note
     * that you cannot change the binding of a column to a datastore object after creation of the column.
     */
    JKQTPcolumn(JKQTPdatastore* datastore, QString name=QString(""), size_t datastoreItem=0, unsigned long long datastoreOffset=0) {
        this->datastore=datastore;
        this->datastoreItem=datastoreItem;
        this->datastoreOffset=datastoreOffset;
        this->name=name;
        valid=true;
    }

    inline bool isValid() const { return valid; }

    /** \brief class destructor */
    virtual ~JKQTPcolumn() {};

    JKQTPGET_SET_MACRO(QString, name)

    /** \brief returns the number of rows in this column (accesses the datastore) */
    unsigned long long getRows() const;

    void copyData(QVector<double>& copyTo) const;
    QVector<double> copyData();

    /** \brief reads the \a n'th value from the column
     *
     * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
     * column.
     */
    double getValue(unsigned long long n) const;
    /** \brief gets a pointer to the n-th value in the column
     */
    double* getPointer(unsigned long long n=0) const;

    /** \brief sets the \a n'th value from the column
     *
     * This method accesses the datastore and returns the double value stored in the \a n'th row of the according
     * column.
     */
    void setValue(unsigned long long n, double val);

    /** \brief returns a pointer to the datastore item representing this column */
    inline JKQTPdatastoreItem* getDatastoreItem() const { return datastore->getItem(datastoreItem); };

    /** \brief copy data from the given array into the column
     *
     * This copies \a N elements from \a data into the column where the first overwritten column
     * line is \a offset, so you can shift the location where the copy process starts.
     */
    void copy(double* data, unsigned long long N, unsigned long long offset=0);

    /** \brief exchange everz occurence of a given \a value by a \a replace value */
    void exchange(double value, double replace);

    /** \brief subtracts a given value from all members of the column */
    void subtract(double value);
    /** \brief scales all members of the column with the given factor */
    void scale(double factor);

    JKQTPGET_MACRO(size_t, datastoreItem)
    JKQTPGET_MACRO(unsigned long long, datastoreOffset)
};




/** \brief this represents one chunk of memory which can represent one or more columns of data for JKQTBasePlotter.
 *         See JKQTPdatastore for more information.
 * \ingroup jkqtpdatastorage
 *
 * Each chunk of memory is pointed at by a simple double* pointer \c data. the memory layout of the memory layout of
 * the RAM segment pointed at by \c data is determined by the parameter \c dataformat:
 *   \copydoc JKQTPdatastoreItemFormat
 *
 * The properties \c columns and \c rows determine how many columns and rows are represented by this item (access via
 * get_columns() and get_rows() ). This class may manage chunks of "internal" and "external" memory (which is indicated by
 * the boolean property \c internal. Internal memory will be allocated (in the constructor) and freed (in the destructor) by this
 * object. External memory may be accessed via this class, but will neither by allocated nor freed. These tasks are up to the
 * user. Use this option, if you don't want to generate copies of huge datasets, or you want to change the data while the program
 * is running and don't want to do the change at more than one spot.
 *
 * you can use get() and set() to access the memory chunc.
 *
 * \see JKQTPdatastore
 */
class LIB_EXPORT JKQTPdatastoreItem {
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
    unsigned long long rows;
    /** \brief memory format of the data in this item */
    JKQTPdatastoreItemFormat dataformat;
  protected:
    /** \brief hidden default constructor */
    JKQTPdatastoreItem();
  public:
    /** \brief class constructor: initializes the object for internal data storage */
    JKQTPdatastoreItem(size_t columns, unsigned long long rows);
    /** \brief class constructor: initializes the object for external data storage */
    JKQTPdatastoreItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columns, unsigned long long rows);
    /** \brief class constructor: initializes the object for external data storage */
    JKQTPdatastoreItem(JKQTPdatastoreItemFormat dataformat, double* data, size_t columns, unsigned long long rows, bool internal);
    /** \brief class destructor: frees unfreed internal memory */
    virtual ~JKQTPdatastoreItem();

    /** \brief change the size of all columns to the givne number of rows. The data will be lost */
    void resizeColumns(unsigned long long rows);

    JKQTPGET_MACRO(unsigned long long, rows)
    JKQTPGET_MACRO(size_t, columns)


    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline double get(size_t column, unsigned long long row) {
        if (data!=NULL) switch(dataformat) {
            case JKQTPsingleColumn:
              return data[row];
            case JKQTPmatrixColumn:
              return data[column*rows+row];
            case JKQTPmatrixRow:
              return data[row*columns+column];
        }
        return 0;
    };


    /** \brief returns the data at the position (\a column, \a row ). The column index specifies the column inside THIS item, not the global column number. */
    inline double* getPointer(size_t column, unsigned long long row) {
        if (data!=NULL) switch(dataformat) {
            case JKQTPsingleColumn:
              return &(data[row]);
            case JKQTPmatrixColumn:
              return &(data[column*rows+row]);
            case JKQTPmatrixRow:
              return &(data[row*columns+column]);
        }
        return NULL;
    };
    /** \brief set the data at the position (\a column, \a row ) to \a value. The column index specifies the column inside THIS item, not the global column number. */
    inline void set(size_t column, unsigned long long row, double value) {
        if (data!=NULL) switch(dataformat) {
            case JKQTPsingleColumn:
              data[row]=value;
              return;
            case JKQTPmatrixColumn:
              data[column*rows+row]=value;
              return;
            case JKQTPmatrixRow:
              data[row*columns+column]=value;
              return;
        }
    };
};


/** \brief 	QAbstractTableModel descendent that allows to view data in a JKQTPdatastore
 * \ingroup jkqtpdatastorage
 *
 * \see JKQTPdatastore
 */
class LIB_EXPORT JKQTPdatastoreModel: public QAbstractTableModel {
        Q_OBJECT
    public:
        JKQTPdatastoreModel(JKQTPdatastore* datastore, QObject* parent=NULL);
        ~JKQTPdatastoreModel();


        virtual QVariant data(const QModelIndex &index, int role) const;
        virtual Qt::ItemFlags flags(const QModelIndex &index) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const ;

    public slots:
        void reloadModel();

    protected:
        JKQTPdatastore* datastore;

};


#endif // JKQTPDATASTORAGE_H
