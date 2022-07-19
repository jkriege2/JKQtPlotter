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




#include "jkqtplotter/gui/jkqtpenhancedtableview.h"
#include <QAction>
#include <QApplication>
#include <QKeyEvent>
#include <QClipboard>
#include <QDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QHeaderView>
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
#  include <QPrintDialog>
#  include <QPrinter>
#endif
#include <QLabel>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"



JKQTPEnhancedTableView::JKQTPEnhancedTableView(QWidget *parent):
    QTableView(parent)
{
    setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* act;

    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16.png"), tr("Copy Selection to Clipboard (for Excel ...)"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToExcel()));
    addAction(act);
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16_nohead.png"), tr("Copy Selection to clipboard (for Excel ...) without header row/column"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToExcelNoHead()));
    addAction(act);
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16.png"), tr("Copy Selection to Clipboard (as CSV)"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToCSV()));
    addAction(act);
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_copy16_nohead.png"), tr("Copy Selection to clipboard (as CSV ...) without header row/column"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(copySelectionToCSVNoHead()));
    addAction(act);
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    act=new QAction(QIcon(":/JKQTPlotter/jkqtp_printtable.png"), tr("Print Table"), this);
    connect(act, SIGNAL(triggered()), this, SLOT(print()));
    addAction(act);
    printAction=act;
#endif
}

JKQTPEnhancedTableView::~JKQTPEnhancedTableView()
= default;


QString JKQTPEnhancedTableView::toHtml(int borderWidth, bool /*non_breaking*/, int fontSizePt) const
{
    if (!model()) return "";
    QString fsstyle="";
    if (fontSizePt>0) {
        fsstyle=QString(" font-size: %1pt;").arg(fontSizePt);
    }
    QString html=QString("<table border=\"%1\" cellspacing=\"0\" cellpadding=\"0\">").arg(borderWidth);
    for (int row=-1; row<model()->rowCount(); row++) {
        html+="<tr>";
        if (row==-1) {
            for (int col=-1; col<model()->columnCount(); col++) {
                html+=QString("<th style=\"white-space: nowrap;%1\"><nobr>").arg(fsstyle);
                if (col>=0) {
                    html+=model()->headerData(col, Qt::Horizontal).toString();
                }
                html+="</nobr></th>";
            }
        } else {
            for (int col=-1; col<model()->columnCount(); col++) {
                if (col==-1) {
                    html+=QString("<th style=\"white-space: nowrap;%1\"><nobr>").arg(fsstyle);
                    html+=model()->headerData(row, Qt::Vertical).toString();
                    html+="</nobr></th>";
                } else {
                    QModelIndex index=model()->index(row, col);
                    QVariant check=index.data(Qt::CheckStateRole);
                    QBrush back=index.data(Qt::BackgroundRole).value<QBrush>();
                    QString style=fsstyle+"white-space: nowrap; ";
                    //qDebug()<<"r="<<row<<"\tc="<<col<<"\tcolor="<<back.color().name();
                    if (back.color()!=QColor(0,0,0) && index.data(Qt::BackgroundRole).isValid()) style=QString("background: %1;").arg(back.color().name());
                    if (style.isEmpty()) html+=QString("<td style=\"%1\"><nobr>").arg(fsstyle);
                    else html+=QString("<td style=\"%1\"><nobr>").arg(style);
                    if (check.isValid()) {
                        if (check.toInt()!=0) {
                            html+= QString("|&times;|&nbsp;&nbsp;");
                        } else {
                            html+= QString("|&nbsp;|&nbsp;&nbsp;");
                        }
                    }
                    html+=index.data().toString();
                    html+="</nobr></td>";
                }
            }
        }
        html+="</tr>";

    }
    html+="</table>";
    return html;
}

void JKQTPEnhancedTableView::copySelectionToExcel(int copyrole, bool storeHead)
{
    if (!model()) return;
    if (!selectionModel()) return;
    QModelIndexList sel=selectionModel()->selectedIndexes();
    QLocale loc=QLocale::system();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    if (sel.size()==1) {
        QVariant vdata=sel[0].data(copyrole);
        QString txt="";
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
        switch (vdata.typeId()) {
            case QMetaType::Int:
            case QMetaType::LongLong:
            case QMetaType::UInt:
            case QMetaType::ULongLong:
            case QMetaType::Bool:
                txt=vdata.toString();
                break;
            case QMetaType::Double:
                txt=loc.toString(vdata.toDouble());
                break;
            case QMetaType::QPointF:
                txt=loc.toString(vdata.toPointF().x());
                break;
            default:
                txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                break;
        }
#else
        switch (vdata.type()) {
            case QVariant::Int:
            case QVariant::LongLong:
            case QVariant::UInt:
            case QVariant::ULongLong:
            case QVariant::Bool:
                txt=vdata.toString();
                break;
            case QVariant::Double:
                txt=loc.toString(vdata.toDouble());
                break;
            case QVariant::PointF:
                txt=loc.toString(vdata.toPointF().x());
                break;
            default:
                txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                break;
        }
#endif
         QApplication::clipboard()->setText(txt);
    } else {
        QSet<int> rows, cols;
        int colmin=0;
        int rowmin=0;
        for (int i=0; i<sel.size(); i++) {
            int r=sel[i].row();
            int c=sel[i].column();
            rows.insert(r);
            cols.insert(c);
            if (i==0) {
                colmin=c;
                rowmin=r;
            } else {
                if (c<colmin) colmin=c;
                if (r<rowmin) rowmin=r;
            }
        }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QList<int> rowlist(rows.begin(), rows.end());
#else
        QList<int> rowlist=QList<int>::fromSet(rows);
#endif
        std::sort(rowlist.begin(), rowlist.end());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QList<int> collist(cols.begin(), cols.end());
#else
        QList<int> collist=QList<int>::fromSet(cols);
#endif
        std::sort(collist.begin(), collist.end());
        int rowcnt=rowlist.size();
        int colcnt=collist.size();
        QList<QStringList> data;

        // header row:
        //
        //  <EMPTY> | <HOR_HEDER1> | <HOR_HEADER2> | ...
        QStringList hrow;
        if (storeHead) {
            hrow.append(""); // empty header for first column (vertical headers!)
            for (int c=0; c<colcnt; c++) {
                hrow.append(QString("\"%1\"").arg(model()->headerData(collist[c], Qt::Horizontal).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " ")));
            }
            data.append(hrow);
        }

        // now add dta rows:
        //
        //               <~~~~~~~~~ colcnt times ~~~~~~~~~~>
        //  <VER_HEADER> | <EMPTY> | <EMPTY> | ... | <EMPTY>
        for (int r=0; r<rowcnt; r++) {
            QStringList row;
            if (storeHead) row.append(QString("\"%1\"").arg(model()->headerData(rowlist[r], Qt::Vertical).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "))); // vertical header
            for (int c=0; c<colcnt; c++) {
                row.append(""); // empty columns for data
            }
            data.append(row);
        }
        for (int i=0; i<sel.size(); i++) {
            int r=rowlist.indexOf(sel[i].row());
            int c=collist.indexOf(sel[i].column());
            QVariant vdata=sel[i].data(copyrole);
            QString txt="";
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
            switch (vdata.typeId()) {
                case QMetaType::Int:
                case QMetaType::LongLong:
                case QMetaType::UInt:
                case QMetaType::ULongLong:
                case QMetaType::Bool:
                    txt=vdata.toString();
                    break;
                case QMetaType::Double:
                    txt=loc.toString(vdata.toDouble());
                    break;
                case QMetaType::QPointF:
                    txt=loc.toString(vdata.toPointF().x());
                    break;
                default:
                    txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                    break;
            }
#else
            switch (vdata.type()) {
                case QVariant::Int:
                case QVariant::LongLong:
                case QVariant::UInt:
                case QVariant::ULongLong:
                case QVariant::Bool:
                    txt=vdata.toString();
                    break;
                case QVariant::Double:
                    txt=loc.toString(vdata.toDouble());
                    break;
                case QVariant::PointF:
                    txt=loc.toString(vdata.toPointF().x());
                    break;
                default:
                    txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                    break;
            }
#endif
            int shift=0;
            if (storeHead) shift=1;
            if ((r>=0) && (c>=0) && (r<=data.size()) && (c<=colcnt))data[r+shift][c+shift]=txt;
        }

        QString result="";
        for (int r=0; r<data.size(); r++) {
            result+=data[r].join("\t")+"\n";
        }
        QApplication::clipboard()->setText(result);
    }
}

void JKQTPEnhancedTableView::copySelectionToExcelNoHead(int copyrole)
{
    copySelectionToExcel(copyrole, false);
}

void JKQTPEnhancedTableView::copySelectionToCSV(int copyrole, bool storeHead, const QString &separator, const QChar &decimalpoint)
{
    if (!model()) return;
    if (!selectionModel()) return;
    QModelIndexList sel=selectionModel()->selectedIndexes();
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    if (sel.size()==1) {
        QVariant vdata=sel[0].data(copyrole);
        QString txt="";
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
        switch (vdata.typeId()) {
            case QMetaType::Int:
            case QMetaType::LongLong:
            case QMetaType::UInt:
            case QMetaType::ULongLong:
            case QMetaType::Bool:
                txt=vdata.toString();
                break;
            case QMetaType::Double:
                txt=JKQTPDoubleToQString(vdata.toDouble(), 15, 'g', decimalpoint);
                break;
            case QMetaType::QPointF:
                txt=JKQTPDoubleToQString(vdata.toPointF().x(), 15, 'g', decimalpoint);
                break;
            default:
                txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                break;
        }
#else
        switch (vdata.type()) {
            case QVariant::Int:
            case QVariant::LongLong:
            case QVariant::UInt:
            case QVariant::ULongLong:
            case QVariant::Bool:
                txt=vdata.toString();
                break;
            case QVariant::Double:
                txt=JKQTPDoubleToQString(vdata.toDouble(), 15, 'g', decimalpoint);
                break;
            case QVariant::PointF:
                txt=JKQTPDoubleToQString(vdata.toPointF().x(), 15, 'g', decimalpoint);
                break;
            default:
                txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                break;
        }
#endif
         QApplication::clipboard()->setText(txt);
    } else {
        QSet<int> rows, cols;
        int colmin=0;
        int rowmin=0;
        for (int i=0; i<sel.size(); i++) {
            int r=sel[i].row();
            int c=sel[i].column();
            rows.insert(r);
            cols.insert(c);
            if (i==0) {
                colmin=c;
                rowmin=r;
            } else {
                if (c<colmin) colmin=c;
                if (r<rowmin) rowmin=r;
            }
        }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QList<int> rowlist(rows.begin(), rows.end());
#else
        QList<int> rowlist=QList<int>::fromSet(rows);
#endif
        std::sort(rowlist.begin(), rowlist.end());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
        QList<int> collist(cols.begin(), cols.end());
#else
        QList<int> collist=QList<int>::fromSet(cols);
#endif
        std::sort(collist.begin(), collist.end());
        int rowcnt=rowlist.size();
        int colcnt=collist.size();
        QList<QStringList> data;

        // header row:
        //
        //  <EMPTY> | <HOR_HEDER1> | <HOR_HEADER2> | ...
        QStringList hrow;
        if (storeHead) {
            hrow.append(""); // empty header for first column (vertical headers!)
            for (int c=0; c<colcnt; c++) {
                hrow.append(QString("\"%1\"").arg(model()->headerData(collist[c], Qt::Horizontal).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " ")));
            }
            data.append(hrow);
        }

        // now add dta rows:
        //
        //               <~~~~~~~~~ colcnt times ~~~~~~~~~~>
        //  <VER_HEADER> | <EMPTY> | <EMPTY> | ... | <EMPTY>
        for (int r=0; r<rowcnt; r++) {
            QStringList row;
            if (storeHead) row.append(QString("\"%1\"").arg(model()->headerData(rowlist[r], Qt::Vertical).toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "))); // vertical header
            for (int c=0; c<colcnt; c++) {
                row.append(""); // empty columns for data
            }
            data.append(row);
        }
        for (int i=0; i<sel.size(); i++) {
            int r=rowlist.indexOf(sel[i].row());
            int c=collist.indexOf(sel[i].column());
            QVariant vdata=sel[i].data(copyrole);
            QString txt="";
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
                switch (vdata.typeId()) {
                    case QMetaType::Int:
                    case QMetaType::LongLong:
                    case QMetaType::UInt:
                    case QMetaType::ULongLong:
                    case QMetaType::Bool:
                        txt=vdata.toString();
                        break;
                    case QMetaType::Double:
                        txt=JKQTPDoubleToQString(vdata.toDouble(), 15, 'g', decimalpoint);
                        break;
                    case QMetaType::QPointF:
                        txt=JKQTPDoubleToQString(vdata.toPointF().x(), 15, 'g', decimalpoint);
                        break;
                    default:
                        txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                        break;
                }
#else
            switch (vdata.type()) {
                case QVariant::Int:
                case QVariant::LongLong:
                case QVariant::UInt:
                case QVariant::ULongLong:
                case QVariant::Bool:
                    txt=vdata.toString();
                    break;
                case QVariant::Double:
                    txt=JKQTPDoubleToQString(vdata.toDouble(), 15, 'g', decimalpoint);
                    break;
                case QVariant::PointF:
                    txt=JKQTPDoubleToQString(vdata.toPointF().x(), 15, 'g', decimalpoint);
                    break;
                default:
                    txt=QString("\"%1\"").arg(vdata.toString().replace('"', "''").replace('\n', "\\n ").replace('\r', "\\r ").replace('\t', " "));
                    break;
            }
#endif
            int shift=0;
            if (storeHead) shift=1;
            if ((r>=0) && (c>=0) && (r<=data.size()) && (c<=colcnt))data[r+shift][c+shift]=txt;
        }

        QString result="";
        for (int r=0; r<data.size(); r++) {
            result+=data[r].join(separator)+"\n";
        }
        QApplication::clipboard()->setText(result);
    }
}

void JKQTPEnhancedTableView::copySelectionToCSVNoHead(int copyrole, const QString &separator, const QChar &decimalpoint)
{
     copySelectionToCSV(copyrole, false, separator, decimalpoint);
}

void JKQTPEnhancedTableView::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy)) {
        copySelectionToExcel(Qt::EditRole, false);
        event->accept();
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
    } else if (event->matches(QKeySequence::Print)) {
        print();
        event->accept();
#endif
    } else QTableView::keyPressEvent(event);
    emit keyPressed(event->key(), event->modifiers(), event->text());
}

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
void JKQTPEnhancedTableView::print()
{
    QPrinter* tablePrinter=getPrinter(nullptr);

    if (tablePrinter) {
        QDialog* dlg=new QDialog(this);
        dlg->setWindowTitle(tr("Table print options ..."));
        QGridLayout* lay=new QGridLayout();
        dlg->setLayout(lay);
        lay->addWidget(new QLabel(tr("<b>scaling:</b>")), 0,0);
        QCheckBox* chk1Wide=new QCheckBox(tr("one page wide"), dlg);
        QCheckBox* chk1High=new QCheckBox(tr("one page high"), dlg);
        lay->addWidget(chk1Wide, 0, 1);
        lay->addWidget(chk1High, 1, 1);

        QDialogButtonBox* btns=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,dlg);
        connect(btns, SIGNAL(accepted()), dlg, SLOT(accept()));
        connect(btns, SIGNAL(rejected()), dlg, SLOT(reject()));
        lay->addWidget(btns, 2, 0,1,2);

        if (dlg->exec()) {
            print(tablePrinter, chk1Wide->isChecked(), chk1High->isChecked());
        }
        delete dlg;
        delete tablePrinter;
    }
}
#endif

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
void JKQTPEnhancedTableView::print(QPrinter *printer, bool onePageWide, bool onePageHigh)
{
    QPrinter* p=printer;


     //p->setPageMargins(10,10,10,10,QPageLayout::Millimeter);

     /*if (width()>height()) {
         p->setPageOrientation(QPageLayout::Landscape);
     } else {
         p->setPageOrientation(QPageLayout::Portrait);
     }*/

     clearSelection();


     /// PRINT HERE //////////////////////////////////////////////////////////////////////////////////
     // calculate the total width/height table would need without scaling
     const int rows = model()->rowCount();
     const int cols = model()->columnCount();
     double vhw=verticalHeader()->width()+8;
     double totalWidth = vhw;
     double minWidth=1e33;
     double maxWidth=0;
     for (int c = -1; c < cols; ++c)
     {
         double w=columnWidth(c);
         totalWidth += w;
         if (w<minWidth) minWidth=w;
         if (w>maxWidth) maxWidth=w;
     }
     double hhh=horizontalHeader()->height()+8;
     double totalHeight = hhh;
     double minHeight=1e33;
     double maxHeight=0;
     for (int r = 0; r < rows; ++r)
     {
        double h=rowHeight(r);
        totalHeight += h;
        if (h<minHeight) minHeight=h;
        if (h>maxHeight) maxHeight=h;
     }
     double scale=1.0;
     // adjust scale, so the widest/highest column fits on one page
     /*if (maxWidth*scale>p->pageLayout().paintRectPixels(p->resolution()).width()) scale=p->pageLayout().paintRectPixels(p->resolution()).width()/maxWidth;
     if (maxHeight*scale>p->pageLayout().paintRectPixels(p->resolution()).height()) scale=p->pageLayout().paintRectPixels(p->resolution()).height()/maxHeight;*/
     if (onePageWide) {

         if (totalWidth>p->pageLayout().paintRectPixels(p->resolution()).width()) scale=p->pageLayout().paintRectPixels(p->resolution()).width()/totalWidth;
     }
     if (onePageHigh) {
         if (totalHeight>p->pageLayout().paintRectPixels(p->resolution()).height()) scale=qMin(scale, p->pageLayout().paintRectPixels(p->resolution()).height()/totalHeight);
     }

     //qDebug()<<scale;

     // print scaled pixmap

     int pagesWide=1;
     int pagesHigh=1;
     QList<int> pageCols, pageRows;
     pageCols<<0;
     pageRows<<0;
     { // find number of pages needed
         double x=vhw, x0=vhw;
         if (!onePageWide) {
             for (int c=0; c<cols; c++) {
                 double cw=columnWidth(c);
                 if (x+cw>p->pageLayout().paintRectPixels(p->resolution()).width()/scale) {
                     pagesWide++;
                     x=0;
                     pageCols<<c;
                 } else {
                     x=x+cw;
                     x0=x0+cw;
                 }
             }
         }
         if (pageCols.size()>0 && cols>pageCols.last()) pageCols<<cols;
         if (pageCols.size()==1) pageCols<<cols;
         double y=hhh, y0=hhh;
         if (!onePageHigh) {
             for (int r=0; r<rows; r++) {
                 double rh=rowHeight(r);
                 if (y+rh>p->pageLayout().paintRectPixels(p->resolution()).height()/scale) {
                     pagesHigh++;
                     pageRows<<r;
                     y=hhh;
                 } else {
                     y=y+rh;
                     y0=y0+rh;
                 }
             }
         }
         if (pageRows.size()>0 && rows>pageRows.last()) pageRows<<rows;
         if (pageRows.size()==1) pageRows<<rows;
     }

     JKQTPEnhancedPainter painter(p);
     paint(painter, scale, -1, hhh, vhw, pageCols, pageRows, p);
     painter.end();




     /// PRINT DONE //////////////////////////////////////////////////////////////////////////////////
}
#endif

void JKQTPEnhancedTableView::paint(QPainter &painter, QRect pageRect)
{
    QRect pageRec=pageRect;
    if (pageRec.width()==0 || pageRec.height()==0) pageRec=QRect(QPoint(0,0), getTotalSize().toSize());
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    painter.translate(pageRec.topLeft());
    QSizeF size=getTotalSize();
    double vhw=verticalHeader()->width()+8;
    double hhh=horizontalHeader()->height()+8;
    double scaleX=size.width()/double(pageRec.width());
    double scaleY=size.height()/double(pageRec.height());
    double scale=qMin(scaleX, scaleY);
    QList<int>pageCols, pageRows;
    pageCols<<0<<model()->columnCount();
    pageRows<<0<<model()->rowCount();
    paint(painter, scale, -1, hhh, vhw, pageCols, pageRows);
    
}

QSizeF JKQTPEnhancedTableView::getTotalSize() const
{
    const int rows = model()->rowCount();
    const int cols = model()->columnCount();
    double vhw=verticalHeader()->width()+8;
    double totalWidth = vhw;
    for (int c = -1; c < cols; ++c)
    {
        double w=columnWidth(c);
        totalWidth += w;
    }
    double hhh=horizontalHeader()->height()+8;
    double totalHeight = hhh;
    for (int r = 0; r < rows; ++r)
    {
       double h=rowHeight(r);
       totalHeight += h;
    }

    return QSizeF((totalWidth), (totalHeight));
}

#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
void JKQTPEnhancedTableView::paint(QPainter &painter, double scale, int page, double hhh, double vhw, const QList<int>& pageCols, const QList<int>& pageRows, QPrinter* p)
#else
void JKQTPEnhancedTableView::paint(QPainter &painter, double scale, int page, double hhh, double vhw, const QList<int>& pageCols, const QList<int>& pageRows)
#endif
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QStyleOptionViewItem option;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    initViewItemOption(&option);
#else
    option = viewOptions();
#endif
    painter.scale(scale, scale);
    QPen headerPen("black");
    headerPen.setWidth(2);
    QPen cellPen("gray");
    cellPen.setWidth(1);
    QFont headerFont=horizontalHeader()->font();
    headerFont.setBold(true);
    int pagesWide=pageCols.size()-1;
    int pagesHigh=pageRows.size()-1;
    //painter.translate(p->pageLayout().paintRectPixels(p->resolution()).topLeft());
    int pageCnt=0;
    for (int ph=0; ph<pageRows.size()-1; ph++) {
        for (int pw=0; pw<pageCols.size()-1; pw++) {
            if (page<0 || page==pageCnt) {
                //qDebug()<<"print page "<<ph<<"/"<<pageRows.size()<<pagesHigh<<"    "<<pw<<"/"<<pageCols.size()<<pagesWide;
                //painter.drawPicture(p->pageLayout().paintRectPixels(p->resolution()).topLeft(), pic);
                double y=0;
                if (ph==0) {
                    y=hhh;
                }
                //qDebug()<<"   rows = "<<pageRows[ph]<<"..."<<pageRows[ph+1]-1;
                //qDebug()<<"   cols = "<<pageCols[pw]<<"..."<<pageCols[pw+1]-1;
                for (int r=pageRows[ph]; r<pageRows[ph+1]; r++) {
                    double x=0;
                    int rh=rowHeight(r);
                    if (pw%pagesWide==0) {
                        x=0;
                        option.rect = QRect(static_cast<int>(x), static_cast<int>(y), static_cast<int>(vhw), rh);
                        //verticalHeader()->itemDelegate()->paint(&painter, option, model()->index(r, c, QModelIndex()));
                        x=vhw;
                    }
                    for (int c=pageCols[pw]; c<pageCols[pw+1]; c++) {
                        double cw=columnWidth(c);
                        option.rect = QRect(static_cast<int>(x), static_cast<int>(y), static_cast<int>(cw), rh);
                        itemDelegate()->paint(&painter, option, model()->index(r, c, QModelIndex()));
                        painter.setPen(cellPen);
                        painter.drawRect(option.rect);
                        x=x+cw;

                    }
                    y=y+rh;
                }

                if (ph==0) {
                    y=0;
                    int x=0;
                    if (pw%pagesWide==0) x=static_cast<int>(vhw);
                    painter.setPen(headerPen);
                    for (int c=pageCols[pw]; c<pageCols[pw+1]; c++) {
                        QRect rec=QRect(x, static_cast<int>(y), columnWidth(c), static_cast<int>(hhh));
                        painter.fillRect(rec, QColor("lightgrey"));
                        painter.setFont(headerFont);
                        painter.setPen(headerPen);
                        painter.drawText(QRect(rec.x()+4, rec.y()+4, rec.width()-8, rec.height()-8), model()->headerData(c, Qt::Horizontal).toString());
                        painter.drawRect(rec);
                        //if (x==vhw &&) painter.drawLine(rec.topLeft(), QPoint(rec.left(), p->pageLayout().paintRectPixels(p->resolution()).height()));
                        x=x+columnWidth(c);
                    }

                }
                if (pw%pagesWide==0) {
                    y=0;
                    int x=0;
                    if (ph==0) y=hhh;
                    for (int r=pageRows[ph]; r<pageRows[ph+1]; r++) {
                        QRect rec=QRect(x, static_cast<int>(y), static_cast<int>(vhw), rowHeight(r));
                        painter.fillRect(rec, QColor("lightgrey"));
                        painter.setPen(headerPen);
                        painter.setFont(headerFont);
                        painter.drawText(QRect(rec.x()+4, rec.y()+4, rec.width()-8, rec.height()-8), model()->headerData(r, Qt::Vertical).toString());
                        painter.drawRect(rec);
                        //if (x==vhw &&) painter.drawLine(rec.topLeft(), QPoint(rec.left(), p->pageLayout().paintRectPixels(p->resolution()).height()));
                        y=y+rowHeight(r);
                    }

                }


#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
                if (p && pw<pagesWide-1) p->newPage();
#endif
            }
            pageCnt++;
        }
#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
        if (p && ph<pagesHigh-1) p->newPage();
#endif
    }
    
}



#ifndef JKQTPLOTTER_COMPILE_WITHOUT_PRINTSUPPORT
QPrinter *JKQTPEnhancedTableView::getPrinter(QPrinter *printerIn, bool *localPrinter)
{
    QPrinter* p=printerIn;
    if (p==nullptr) {
        p=new QPrinter();
        if (localPrinter) *localPrinter=true;
    }

    QPrintDialog *dialog = new QPrintDialog(p, nullptr);
    dialog->setWindowTitle(tr("Print Table"));
    if (dialog->exec() != QDialog::Accepted) {
        if (localPrinter && *localPrinter) delete p;
        delete dialog;
        return printerIn;
    }
    p=dialog->printer();
    delete dialog;

    return p;
}
#endif
