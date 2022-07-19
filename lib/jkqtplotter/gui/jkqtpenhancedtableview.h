/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef JKQTPEnhancedTableView_H_INCLUDED
#define JKQTPEnhancedTableView_H_INCLUDED
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtplotter_configmacros.h"
#include <QTableView>
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
#  include <QPrintDialog>
#  include <QPrinter>
#endif

/*! \brief this class extends the <a href="http://doc.qt.io/qt-5/qtableview.html">QTableView</a>
    \ingroup jkqtptools_qtwidgets

    This enhanced table view adds some functionality to the Qt class:
      - return HTML code that describes the table (see toHtml() )
      - contains methods to draw the table onto a <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>: paint()
      - the selected cells may be copied to Excel using the clipboard (Excel will recognize number !) see copySelectionToExcel() and copySelectionToCSV()
      - copySelectionToExcel() is called when the user presses the default copy key combination for your system (e.g. Ctrl+C)
      - print() is called when the user presses the default print key combination for your system (e.g. Ctrl+P)
      - the signal keyPressed() is emitted, when the suer pressed a key
    .

    \image html JKQTPEnhancedTableView.png
*/
class JKQTPLOTTER_LIB_EXPORT JKQTPEnhancedTableView : public QTableView {
        Q_OBJECT
    public:
        JKQTPEnhancedTableView(QWidget* parent=nullptr);
        virtual ~JKQTPEnhancedTableView();

        /** \brief return the contents of the table view as HTML fragment */
        QString toHtml(int borderWidth=1, bool non_breaking=false, int fontSizePt=-1) const;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief print the contents of the table view
         *
         * \param printer the QPrinter to use for printing the table view
         * \param onePageWide limit the width of the table to a single page, if not set the printout may span several pages
         * \param onePageHigh limit the height of the table to a single page, if not set the printout may span several pages
         *
         * The printouts look like this:
         *
         * \image html JKQTPEnhancedTableViewPrint.png
         *
         * Depending on the options set in the dialog above, the printout is split over several pages, or not:
         *
         * \image html JKQTPEnhancedTableViewPrintMultipage.png "Split over pages (onePageWide=false onePageHigh=false)"
         * \image html JKQTPEnhancedTableViewPrintSinglepage.png "Print on one page (onePageWide=true onePageHigh=true)"
         */
        void print(QPrinter* printer, bool onePageWide=false, bool onePageHigh=false);
#endif
        /** \brief draw the contents of the table-view into the given \a pageRect, using the given \a painter
         *
         * The output look like this:
         *
         * \image html JKQTPEnhancedTableViewPrint.png
         *
         * \see print()
         */
        void paint(QPainter& painter, QRect pageRec=QRect());
        /** \brief returns the totoal size of the table-view \see paint() */
        QSizeF getTotalSize() const;
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief return a QAction that prints the table using the methode print() \see print() */
        inline QAction* getActionPrint() { return printAction; }
#endif
    signals:
        /** \brief emitted when a key is pressed in the context of this widget */
        void keyPressed(int key, Qt::KeyboardModifiers modifiers, const QString& text);
    public slots:
        /** \brief copy the selected cells' contents to the clipboard, so it can be pasted into Excel */
        void copySelectionToExcel(int copyrole=Qt::EditRole, bool storeHead=true);
        /** \brief copy the selected cells' contents to the clipboard, so it can be pasted into Excel, without header */
        void copySelectionToExcelNoHead(int copyrole=Qt::EditRole);
        /** \brief copy the selected cells' contents to the clipboard as comma separated text
         *
         * \param copyrole specifies how the values for copying are extracted (see <a href="http://doc.qt.io/qt-5/qt.html#ItemDataRole-enum">Qt::ItemDataRole</a>).
         *                 During the export, this parameter is supplied to function \c data() of the underlying model.
         * \param storeHead en-/disable storage of the table header \see copySelectionToCSVNoHead()
         * \param separator separator between two values (default is a comma)
         * \param decimalpoint decimal separator for values (default is a dot)
         *
         * \see copySelectionToCSVNoHead()
         */
        void copySelectionToCSV(int copyrole=Qt::EditRole, bool storeHead=true, const QString& separator=", ", const QChar& decimalpoint='.');
        /** \brief copy the selected cells' contents to the clipboard as comma separated text, without the header
         *
         * \param copyrole specifies how the values for copying are extracted (see <a href="http://doc.qt.io/qt-5/qt.html#ItemDataRole-enum">Qt::ItemDataRole</a>).
         *                 During the export, this parameter is supplied to function \c data() of the underlying model.
         * \param separator separator between two values (default is a comma)
         * \param decimalpoint decimal separator for values (default is a dot)
         *
         * \see copySelectionToCSV()
         */
        void copySelectionToCSVNoHead(int copyrole=Qt::EditRole, const QString& separator=", ", const QChar& decimalpoint='.');
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief print the table contents
         *
         * Before printing this function opens a <a href="http://doc.qt.io/qt-5/qprintdialog.html">QPrintDialog</a> to select a printer
         * and then opens a second dialog that allows to set different options for the printout:
         *
         * \image html JKQTPEnhancedTableViewOptionsDialog.png
         *
         * The printouts look like this:
         *
         * \image html JKQTPEnhancedTableViewPrint.png
         *
         * Depending on the options set in the dialog above, the printout is split over several pages, or not:
         *
         * \image html JKQTPEnhancedTableViewPrintMultipage.png "Split over pages (onePageWide=false onePageHigh=false)"
         * \image html JKQTPEnhancedTableViewPrintSinglepage.png "Print on one page (onePageWide=true onePageHigh=true)"
         *
         */
        void print();
#endif
    protected:
        /** \brief handles key presses and reacts to some standard keys
         *  \internal
         */
        virtual void keyPressEvent(QKeyEvent* event);
        /** \brief internal paintig method
         *  \internal
         */
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        void paint(QPainter &painter, double scale, int page, double hhh, double vhw, const QList<int>& pageCols, const QList<int>& pageRows, QPrinter* p=nullptr);
#else
        void paint(QPainter &painter, double scale, int page, double hhh, double vhw, const QList<int>& pageCols, const QList<int>& pageRows);
#endif
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        /** \brief select a printer, using a print-selection dialog if necessary */
        QPrinter* getPrinter(QPrinter* printerIn=nullptr, bool *localPrinter=nullptr);
        /** \brief action that calls print() */
        QAction* printAction;
#endif
    private:
};


#endif // JKQTPEnhancedTableView_H_INCLUDED
