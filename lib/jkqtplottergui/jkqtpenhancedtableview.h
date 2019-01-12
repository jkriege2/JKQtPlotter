/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
 * \defgroup jkqtptools Tool Functions and Definitions
 * \ingroup jkqtplotter
 */

/** \file jkqtpenhancedtableview.h
  * \ingroup jkqtptools
  */


#ifndef JKQTPEnhancedTableView_H_INCLUDED
#define JKQTPEnhancedTableView_H_INCLUDED
#include "jkqtplottertools/jkqtp_imexport.h"
#include <QTableView>
#include <QPrinter>

/*! \brief this class extends the QTableView
    \ingroup jkqtptools

    This enhanced table view adds some functionality to the Qt class:
      - return HTML code that describes the table
      - the selected cells may be copied to Excel using the clipboard (Excel will recognize number !)
    .
*/
class LIB_EXPORT JKQTPEnhancedTableView : public QTableView {
        Q_OBJECT
    public:
        JKQTPEnhancedTableView(QWidget* parent=nullptr);
        virtual ~JKQTPEnhancedTableView();

        /** \brief return the contents of the table view as HTML fragment */
        QString toHtml(int borderWidth=1, bool non_breaking=false, int fontSizePt=-1) const;
        void print(QPrinter* printer, bool onePageWide=false, bool onePageHigh=false);

        void paint(QPainter& painter, QRect pageRec=QRect());
        QSizeF getTotalSize() const;
        QAction* getPrintAction() const { return printAction; }

    signals:
        void keyPressed(int key, Qt::KeyboardModifiers modifiers, QString text);
    public slots:
        void copySelectionToExcel(int copyrole=Qt::EditRole, bool storeHead=true);
        void copySelectionToExcelNoHead(int copyrole=Qt::EditRole);
        void copySelectionToCSV(int copyrole=Qt::EditRole, bool storeHead=true, const QString& separator=", ", const QChar& decimalpoint='.');
        void copySelectionToCSVNoHead(int copyrole=Qt::EditRole, const QString& separator=", ", const QChar& decimalpoint='.');
        void print();

    protected:
        virtual void keyPressEvent(QKeyEvent* event);
        void paint(QPainter &painter, double scale, int page, double hhh, double vhw, const QList<int>& pageCols, const QList<int>& pageRows, QPrinter* p=nullptr);

        QPrinter* getPrinter(QPrinter* printerIn=nullptr, bool *localPrinter=nullptr);
        QAction* printAction;
    private:
};


#endif // JKQTPEnhancedTableView_H_INCLUDED
