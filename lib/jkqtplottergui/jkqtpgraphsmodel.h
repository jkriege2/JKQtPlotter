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



#ifndef jkqtpgraphsmodel_H
#define jkqtpgraphsmodel_H

#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtcommon/jkqtp_imexport.h"

#include <QAbstractTableModel>

class JKQTBasePlotter; // forward
/** \brief data model with all plots ina JKQTBasePlotter (+ allows to switch them visible/invisible)
  * \ingroup jkqtpgraphsmodel
  *
  * This class is derived from <a href="http://doc.qt.io/qt-5/qabstracttablemodel.html">QAbstractTableModel</a> and implements a model where
  * some properties are editable. It is designed to be used together with e.g. <a href="http://doc.qt.io/qt-5/qlistview.html">QListView</a>.
  *
  * \image html JKQTPGraphsModel.png
  *
  */
class JKQTPGraphsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    JKQTPGraphsModel(JKQTBasePlotter *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override ;
public slots:
    void plotUpdated();
private:
    JKQTBasePlotter* m_plotter;
};


#endif // jkqtpgraphsmodel_H
