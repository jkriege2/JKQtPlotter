/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

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



/** \file jkqtpgraphsmodel.cpp
  * \ingroup jkqtpgraphsmodel
  */

#include "jkqtplottergui/jkqtpgraphsmodel.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include <QImage>



JKQTPgraphsModel::JKQTPgraphsModel(JKQtBasePlotter *parent):
    QAbstractTableModel(parent), m_plotter(parent)
{

}

int JKQTPgraphsModel::rowCount(const QModelIndex &/*parent*/) const
{
    return static_cast<int>(m_plotter->getGraphCount());
}

int JKQTPgraphsModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 1;
}

QVariant JKQTPgraphsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
       if (index.row()<static_cast<int>(m_plotter->getGraphCount())) return m_plotter->getGraph(static_cast<size_t>(index.row()))->get_title();
    } else if (role == Qt::CheckStateRole) {
       if (index.row()<static_cast<int>(m_plotter->getGraphCount())) return m_plotter->getGraph(static_cast<size_t>(index.row()))->get_visible()?Qt::Checked:Qt::Unchecked;
    } else if (role == Qt::DecorationRole) {
        if (index.row()<static_cast<int>(m_plotter->getGraphCount())) {
            return m_plotter->getGraph(static_cast<size_t>(index.row()))->generateKeyMarker(QSize(16,16));
        }
    }
    return QVariant();
}

bool JKQTPgraphsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        if (index.row()<static_cast<int>(m_plotter->getGraphCount())) {
            m_plotter->setGraphVisible(index.row(), value.toBool());
            return true;
        }
    }
    return false;
}

Qt::ItemFlags JKQTPgraphsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsUserCheckable | QAbstractTableModel::flags(index);
}

void JKQTPgraphsModel::plotUpdated()
{
    beginResetModel();
    endResetModel();
}
