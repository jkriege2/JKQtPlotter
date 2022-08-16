/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)
    with contributions from: Razi Alavizadeh

    

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




#ifndef JKQTMATHTEXTMATRIXNODE_H
#define JKQTMATHTEXTMATRIXNODE_H
#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtexttools.h"
#include "jkqtmathtext/nodes/jkqtmathtextnode.h"
#include <QPainter>

class JKQTMathText; // forward
// JKQTMATHTEXT_LIB_EXPORT


/** \brief subclass representing a \c \\begin{matrix} , \c \\begin{tabular} , \c \\begin{array} , ... node
 *  \ingroup jkqtmathtext_items
 *
 *  Definition of the matrix geometry
 *  \image html jkqtmathtext/jkqtmathtext_matrix_geometry.png
 *
 *
 *  Two examples:
 *  \image html jkqtmathtext/jkqtmathtext_tabular.png
 *
 *  \image html jkqtmathtext/jkqtmathtext_array.png
  */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextMatrixNode: public JKQTMathTextMultiChildNode {
    public:
        /** \brief types of lines */
        enum LineType {
            LTnone, /*!< \brief noline */
            LTline, /*!< \brief single line */
            LTdoubleline, /*!< \brief double line */
            LTheavyline, /*!< \brief heavier/thick line */
            LTdashed, /*!< \brief single dashed line */
            LTdoubleDashed, /*!< \brief double dashed line */
            LTdefault=LTnone
        };
        JKQTMathTextMatrixNode(JKQTMathText* parent, const QVector<QVector<JKQTMathTextNode*> >& children, const QString& columnSpec=QString());
        JKQTMathTextMatrixNode(JKQTMathText* parent, const QString& columnSpec=QString());
        virtual ~JKQTMathTextMatrixNode() override;
        /** \brief sets the child nodes  */
        void setChildren(const QVector<QVector<JKQTMathTextNode*> >& children);
        /** \copydoc horizontalLineBottomRow */
        void setRowBottomLine(int col, LineType line);
        /** \copydoc horizontalLineTop */
        void setTopLine(LineType line);
        /** \copydoc JKQTMathTextNode::getTypeName() */
        virtual QString getTypeName() const override;
        /** \copydoc JKQTMathTextNode::draw() */
        virtual double draw(QPainter& painter, double x, double y, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::toHtml() */
        virtual bool toHtml(QString& html, JKQTMathTextEnvironment currentEv, JKQTMathTextEnvironment defaultEv) const override;
        /** \brief returns the child nodes  */
        QVector<QVector<JKQTMathTextNode*> > getChildrenMatrix() const;
        /** \copydoc columns */ 
        int getColumns() const;
        /** \copydoc lines */ 
        int getLines() const;
        /** \copydoc JKQTMathTextMultiChildNode::getChildren() */
        virtual QList<JKQTMathTextNode*> getChildren() override;
        /** \copydoc JKQTMathTextMultiChildNode::childCount() */
        virtual int childCount() const override;
        /** \copydoc JKQTMathTextMultiChildNode::clearChildren() */
        virtual void clearChildren(bool deleteChildren=true) override;
        /** \copydoc JKQTMathTextMultiChildNode::deleteChild() */
        virtual void deleteChild(int i) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* getChild(int i) override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual const JKQTMathTextNode* getChild(int i) const override;
        /** \copydoc JKQTMathTextMultiChildNode::getChild() */
        virtual JKQTMathTextNode* replaceChild(int i, JKQTMathTextNode* newChild) override;
    protected:
        /** \brief describes the layout of the whole node */
        struct LayoutInfo: public JKQTMathTextNodeSize {
            LayoutInfo();
            /** \brief widths of the columns */
            QVector<double> colwidth;
            /** \brief widths of the cells */
            QVector<QVector<double> > cellwidth;
            inline double& getCellwidth(int line, int col) {
                return cellwidth[line].operator[](col);
            }
            inline const double& getCellwidth(int line, int col) const {
                return cellwidth[line].operator[](col);
            }
            /** \brief heights of the rows */
            QVector<double> rowheight;
            /** \brief ascents of the rows */
            QVector<double> rowascent;
            double leftPadding;
            double rightPadding;
            double topPadding;
            double bottomPadding;

        };
        /** \brief calclates the layout of the whole block/node
         *
         *  \note This function does NOT call transformEnvironment();
         *        it has to be called before calling this!
         */
        LayoutInfo calcLayout(QPainter& painter, const JKQTMathTextEnvironment& currentEv) const;
        /** \copydoc JKQTMathTextNode::getSizeInternal() */
        virtual JKQTMathTextNodeSize getSizeInternal(QPainter& painter, JKQTMathTextEnvironment currentEv) const override;
        /** \copydoc JKQTMathTextNode::setDrawBoxes() */
        virtual void setDrawBoxes(bool draw) override;
        /** \brief child nodes making up the matrix, vector of rows */
        QVector<QVector<JKQTMathTextNode*> > children;
        /** \brief alignment of the columns */
        QVector<JKQTMathTextHorizontalAlignment> columnAlignment;
        /** \brief lines to the right of each column */
        QMap<int,LineType> verticalLineRHSColumn;
        /** \brief line at the left of the table */
        LineType verticalLineLeft;
        /** \brief lines to the bottom of each row */
        QMap<int,LineType> horizontalLineBottomRow;
        /** \brief line at the top of the table */
        LineType horizontalLineTop;

        /** \brief number of columns in the matrix */
        int columns;
        /** \brief number of rows in the matrix */
        int lines;

        /** \brief draw a vertical line starting at pixel position (\a x, \a y ) with length \a height, using linetype \a lt, width \a linewidth and \a color */
        static void drawVLine(QPainter& painter, double x, double y, double height, LineType lt, double linewidth, double linewidthHeavy, QColor color, double doublelineseparation);
        /** \brief draw a horizontal line starting at pixel position (\a x, \a y ) with length \a width, using linetype \a lt, width \a linewidth and \a color */
        static void drawHLine(QPainter& painter, double x, double y, double width, LineType lt, double linewidth, double linewidthHeavy, QColor color, double doublelineseparation);
        /** \brief parses a column specifier */
        void parseColumnSpec(const QString& columnspec);
        /** \copydoc JKQTMathTextMultiChildNode::clearChildren() */
        void clearChildrenImpl(bool deleteChildren);
};

#endif // JKQTMATHTEXTMATRIXNODE_H









