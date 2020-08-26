/*
    Copyright (c) 2018-2020 Jan W. Krieger (<jan@jkrieger.de>)

    

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



#include "jkqtplotter/jkqtpdatastorage.h"
#include "CImg.h"

#ifndef JKQTPINTERFACECIMG_H
#define JKQTPINTERFACECIMG_H


/** \brief add one external column to the datastore. It will be filled with the contents of the CImg image \a img.
 * \ingroup jkqtpinterfacecimg
 *
 *   \param datastore the datastore to which the <a href="https://cimg.eu/">CImg</a> matrix should be added (as column)
 *   \param img <a href="http://cimg.eu/reference/structcimg__library_1_1CImg.html">cimg_library::CImg<T>-image</a> to store here
 *   \param name name for the column
 *   \param channel channel to copy from \a img
 *   \param z z-plane to copy from \a img
 *   \return the ID of the newly created column
 *
 *   \see \ref jkqtpinterfacecimg, \ref JKQTPlotterImagePlotCImg or \ref JKQTPlotterImagePlotRGBCImg for details on how to use this function.
 */
template<class T>
inline size_t JKQTPCopyCImgToColumn(JKQTPDatastore* datastore, const cimg_library::CImg<T>& img, const QString& name=QString(""), int channel=0, int z=0)
{
    const size_t N=static_cast<size_t>(img.width()*img.height());
    double* d=static_cast<double*>(malloc(static_cast<size_t>(N)*sizeof(double)));

    size_t r=0;
    for (int iy=0; iy<img.height(); iy++ ) {
        for (int ix=0; ix<img.width(); ix++ ) {
            d[r]=jkqtp_todouble(img.atXYZC(ix,iy,z,channel));
            r++;
        }
    }


    return datastore->addInternalImageColumn(d, img.width(), img.height(), name);
}



#endif // JKQTPINTERFACECIMG_H
