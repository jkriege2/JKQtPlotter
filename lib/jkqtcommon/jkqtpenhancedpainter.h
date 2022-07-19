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
        JKQTPEnhancedPainter(QPaintDevice* device);
        JKQTPEnhancedPainter();


    protected:
        void initQEnhacedPainter();
    private:

};





#endif // JKQTPENHANCED_PAINTERS_H
