/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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
 * \defgroup jkqtpplotsmodel data model with all plots ina JKQtBasePlotter (+ allows to switch them visible/invisible)
 * \ingroup jkqtpbaseplotter
 *
 *
 */

/** \file jkqtpplotsmodel.h
  * \ingroup jkqtpbaseplotter
  */

#ifndef JKQTPPLOTSMODEL_H
#define JKQTPPLOTSMODEL_H

#include "jkqtpbaseplotter.h"
#include "jkqtp_imexport.h"

#include <QAbstractTableModel>

class JKQtBasePlotter; // forward
/** \brief data model with all plots ina JKQtBasePlotter (+ allows to switch them visible/invisible) */
class JKQTPPlotsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    JKQTPPlotsModel(JKQtBasePlotter *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override ;
public slots:
    void plotUpdated();
private:
    JKQtBasePlotter* m_plotter;
};


#endif // JKQTPPLOTSMODEL_H
