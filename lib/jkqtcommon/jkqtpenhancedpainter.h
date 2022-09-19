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


#ifndef JKQTPENHANCED_PAINTERS_H
#define JKQTPENHANCED_PAINTERS_H
#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QPainter>



/*! \brief this class extends the <a href="http://doc.qt.io/qt-5/qpainter.html">QPainter</a>
    \ingroup jkqtptools_drawing


*/
class JKQTCOMMON_LIB_EXPORT JKQTPEnhancedPainter : public QPainter {
    public:
        /** \brief flags that are used to configure a JKQTPEnhancedPainter */
        enum PainterFlag { DefaultPaintMode = 0x00, /*!< \brief the default  mode, the JKQTPEnhancedPainter draws on a pixel-device */
                           VectorPainting   = 0x01, /*!< \brief if set, the JKQTPEnhancedPainter draws onto a vector-device, like a printer, PDF or SVG-output */
                         };
        Q_ENUM(PainterFlag)
        /** \brief specifies the plot styles for the error information, e.g. error bars, boxes, lines ...
         * \ingroup jkqtplotter_basegraphserrors
         *
         * \qFlagsNote{JKQTPErrorPlotstyle,JKQTPErrorPlotstyleElements}
         *
         * \see JKQTPErrorPlotstyleElements, JKQTPXGraphErrorData, JKQTPYGraphErrorData
         */
        Q_DECLARE_FLAGS(PainterFlags, PainterFlag)
        Q_FLAG(PainterFlags)

        JKQTPEnhancedPainter(QPaintDevice* device);
        JKQTPEnhancedPainter();
        /** \copydoc m_flags */
        PainterFlags painterFlags() const ;
        /** \copydoc m_flags */
        void setPainterFlag(PainterFlag flag, bool enabled=true);
        /** \copydoc m_flags */
        void setPainterFlag(PainterFlags flags);

        /** \brief faster variant of QPainter::drawPolyline(),it turns out that drawing single lines is way faster than drawing with drawPolyLine()
         *
         *  At least for thin (1 Pixel) lines on non-vector-devices, this does not make much difference in appearance
         */
        void drawPolylineFast(const QPointF *points, int pointCount);
        inline void drawPolylineFast(const QPolygonF &polyline) {
            drawPolylineFast(polyline.constData(), int(polyline.size()));
        }
        void drawPolylineFast(const QPoint *points, int pointCount);
        inline void drawPolylineFast(const QPolygon &polyline) {
            drawPolylineFast(polyline.constData(), int(polyline.size()));
        }

    protected:
        void initQEnhacedPainter();
    private:
        /** \brief flags, specifying how the JKQTPEnhancedPainter works
         *
         *  \see PainterFlags */
        PainterFlags m_flags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(JKQTPEnhancedPainter::PainterFlags)



#endif // JKQTPENHANCED_PAINTERS_H
