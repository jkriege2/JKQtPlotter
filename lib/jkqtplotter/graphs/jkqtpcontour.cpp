/*
    Copyright (c) 2008-2020 Jan W. Krieger & Sebastian Isbaner (contour plot)

    

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


#include "jkqtplotter/graphs/jkqtpcontour.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtcommon/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <QDebug>
#include <QImageWriter>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QClipboard>
# include <QVector3D>

JKQTPContourPlot::JKQTPContourPlot(JKQTBasePlotter *parent) :
    JKQTPMathImage(parent)
{
    ignoreOnPlane=false;
    contourColoringMode=ColorContoursFromPaletteByValue;
    relativeLevels=false;

    initLineStyle(parent, parentPlotStyle);
}


JKQTPContourPlot::JKQTPContourPlot(JKQTPlotter *parent) :
    JKQTPContourPlot(parent->getPlotter())
{

}



void JKQTPContourPlot::draw(JKQTPEnhancedPainter &painter)
{
    //qDebug()<<"JKQTPContourPlot::draw";
    ensureImageData();

    int numberOfLevels=contourLevels.size();

    if (numberOfLevels<=0) return;

    int64_t colChecksum=-1;
    if (data && Nx*Ny>0) {
        colChecksum=static_cast<int64_t>(qChecksum(reinterpret_cast<char*>(data), Nx*Ny* getSampleSize()));
    }
    /*if (parent && parent->getDatastore() && imageColumn>=0) {
        colChecksum=static_cast<int64_t>(parent->getDatastore()->getColumnChecksum(imageColumn));
    }*/

    if(contourLinesCache.isEmpty() || (contourLinesCachedForChecksum!=colChecksum) || (contourLinesCachedForChecksum<0)) { // contour lines are only calculated once
        QList<QVector<QLineF> > lines;
        lines.reserve(contourLevels.size());
        for(int i =0; i<contourLevels.size();++i) {
            lines.append(QVector<QLineF> (0));
        }
        this->calcContourLines(lines);
        contourLinesCache.clear();
        contourLinesCachedForChecksum=colChecksum;
        for (const QVector<QLineF>& l: lines) {
            contourLinesCache.push_back(JKQTPUnifyLinesToPolygons(l, qMin(getWidth()/static_cast<double>(getNx()),getHeight()/static_cast<double>(getNy()))/4.0));
        }
    }



    // draw lines
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);

    painter.setPen(p);
    // calculate an image with one pixel per contour level and fill it with the appropriate colors
    QImage colorLevels = getPaletteImage(palette,numberOfLevels); // (contourColoringMode==ContourColoringMode::ColorContoursFromPalette)
    if (contourColoringMode==ContourColoringMode::SingleColorContours) {
        for (int i=0; i<numberOfLevels; i++) colorLevels.setPixel(i, 0, getLineColor().rgba());
    } else if (contourColoringMode==ContourColoringMode::ColorContoursFromPaletteByValue) {
        QImage colorDataLevels = getPaletteImage(palette,2000);
        for (int i=0; i<numberOfLevels; i++) {
            colorLevels.setPixel(i, 0, colorDataLevels.pixel(qBound<int>(0, (internalDataMax-contourLevels.value(i, 0))*static_cast<double>(colorDataLevels.width())/(internalDataMax-internalDataMin), colorDataLevels.width()-1),0));
        }
    }
    // set override colors
    for (int i=0; i<numberOfLevels; i++) {
        if (contourOverrideColor.contains(contourLevels[i])) {
            colorLevels.setPixel(i, 0, contourOverrideColor[contourLevels[i]].rgba());
        }
    }
    getDataMinMax(internalDataMin, internalDataMax);
    {
#ifdef JKQTBP_AUTOTIMER
        JKQTPAutoOutputTimer jkaat(QString("JKQTPContourPlot::draw(): draw lines (incl. unify)"));
#endif
        for(int i =0; i<numberOfLevels;++i) {
            //qDebug()<<"============================================================\n== LEVEL "<<i<<"\n============================================================";
            QVector<QPolygonF> contourLinesTransformedSingleLevel;
            p.setColor(QColor(colorLevels.pixel(i,0)));
            painter.setPen(p);
            // transform into plot coordinates
            for(auto polygon =contourLinesCache.at(i).begin(); polygon!=contourLinesCache.at(i).end();++polygon ) {
                contourLinesTransformedSingleLevel.push_back(QPolygonF());
                for (auto& p: *polygon) {
                    contourLinesTransformedSingleLevel.last().append(transform(x+p.x()/double(Nx-1)*width, y+p.y()/double(Ny-1)*height));
                }
                //qDebug()<<lineTranformed;
            }
            for (const QPolygonF& p: contourLinesTransformedSingleLevel) {
                painter.drawPolyline(p);
            }
        }
    }


}

void JKQTPContourPlot::createContourLevels(int nLevels)
{
    ensureImageData();
    clearContourLevel();
    if (!data) return;
    if (nLevels<1) return;
    double min,max;
    getDataMinMax(min,max);
    double delta=(max-min)/static_cast<double>(nLevels+1);

    for(int i=1; i<=nLevels; ++i) {
        contourLevels.append(min + i*delta);
    }
    relativeLevels=false;

    clearCachedContours();

}

void JKQTPContourPlot::createContourLevelsLog(int nLevels, int m)
{
    ensureImageData();
    clearContourLevel();
    if (!data) return;
    if (nLevels<1) return;
    double min,max;
    getDataMinMax(min,max);
    if(min<=0) min=1; // FIXME get smallest number greater zero

    int S=floor((log10(max)-log10(min))/log10(m));
    if(S<2) S=1;
    int P = floor(static_cast<double>(nLevels)/static_cast<double>(S));
    if(P<1) P=1;

    double delta=min;
    contourLevels.append(2*delta);
    for (long s=0; s<S; s++) {
        for (long p=0; p<P; p++) {
            {
                contourLevels.append(contourLevels.last()+delta);
            }
        }
        delta=delta*m;
    }

    if(nLevels!=contourLevels.size()) {
        //qDebug()<<"nLevels="<<nLevels<<"contourLevels.size()="<<contourLevels.size();
        //qDebug()<<"adapt m";
    }

    relativeLevels=false;

    clearCachedContours();
}

void JKQTPContourPlot::setIgnoreOnPlane(bool __value)
{
    this->ignoreOnPlane = __value;
	clearCachedContours();
}

bool JKQTPContourPlot::getIgnoreOnPlane() const
{
    return this->ignoreOnPlane;
}

int JKQTPContourPlot::getNumberOfLevels() const
{
    return this->contourLevels.size();
}

void JKQTPContourPlot::setContourColoringMode(ContourColoringMode __value)
{
    this->contourColoringMode = __value;
}

JKQTPContourPlot::ContourColoringMode JKQTPContourPlot::getContourColoringMode() const
{
    return this->contourColoringMode;
}

QVector<double> JKQTPContourPlot::getContourLevels() const
{
    return this->contourLevels;
}

void JKQTPContourPlot::setRelativeLevels(bool __value)
{
    this->relativeLevels = __value;
}

bool JKQTPContourPlot::getRelativeLevels() const
{
    return this->relativeLevels;
}

void JKQTPContourPlot::addContourLevel(double level)
{
    contourLevels.append(level);
    std::sort(contourLevels.begin(), contourLevels.end());
    clearCachedContours();
}

void JKQTPContourPlot::addContourLevel(double level, QColor overrideColor)
{
    addContourLevel(level);
    setOverrideColor(level, overrideColor);
}

void JKQTPContourPlot::setOverrideColor(double level, QColor overrideColor)
{
    contourOverrideColor[level]=overrideColor;
}

QColor JKQTPContourPlot::getOverrideColor(int level) const
{
    if (level>=0 && level<contourLevels.size()) {
        if (contourOverrideColor.contains(contourLevels.at(level))) {
            return contourOverrideColor.value(contourLevels.at(level));
        }
    }
    return getLineColor();
}

bool JKQTPContourPlot::hasOverrideColor(int level) const
{
    if (level>=0 && level<contourLevels.size()) {
        if (contourOverrideColor.contains(contourLevels.at(level))) {
            return true;
        }
    }
    return false;
}

void JKQTPContourPlot::removeOverrideColor(int level)
{
    if (level>=0 && level<contourLevels.size()) {
        if (contourOverrideColor.contains(contourLevels.at(level))) {
            contourOverrideColor.remove(contourLevels.at(level));
        }
    }
}

void JKQTPContourPlot::clearContourLevel()
{
    contourLevels.clear();
    contourOverrideColor.clear();
    clearCachedContours();
}

void JKQTPContourPlot::clearCachedContours()
{
    contourLinesCache.clear();
    contourLinesCachedForChecksum=-1;
}





void JKQTPContourPlot::calcContourLines(QList<QVector<QLineF> > &ContourLines)
{
#ifdef JKQTBP_AUTOTIMER
    JKQTPAutoOutputTimer jkaat(QString("JKQTPContourPlot::calcContourLines()"));
#else
    //qDebug()<<"JKQTPContourPlot::calcContourLines()";
#endif

    double scale=1; ///< scale of the contour levels;

    if(relativeLevels) {
        double min;
        double max;
        getDataMinMax(min,max);
        scale=1/(max-min);
    }

    enum Position
    {
        // the positions of points of one box
        //      vertex 1 +-------------------+ vertex 2
        //               | \               / |
        //               |   \    m=3    /   |
        //               |     \       /     |
        //               |       \   /       |
        //               |  m=2    X   m=2   |       the center is vertex 0
        //               |       /   \       |
        //               |     /       \     |
        //               |   /    m=1    \   |
        //               | /               \ |
        //      vertex 4 +-------------------+ vertex 3
        Center=0,

        TopLeft=1,
        TopRight=2,
        BottomRight=3,
        BottomLeft=4,

        NumPositions=5
    };


    for ( int yp = 0; yp < (int64_t)getNy() - 1; ++yp ) { // go through image (pixel coordinates) in row major order
        QVector<QVector3D> vertices(NumPositions);

        for ( int xp = 0; xp < (int64_t)getNx() - 1; ++xp ) {

            if ( xp == 0 )
            {
                vertices[TopRight].setX(xp); // will be used for TopLeft later
                vertices[TopRight].setY(yp);
                vertices[TopRight].setZ(
                            getPixelValue( vertices[TopRight].x(), vertices[TopRight].y())*scale
                );

                vertices[BottomRight].setX(xp);
                vertices[BottomRight].setY(yp+1);
                vertices[BottomRight].setZ(
                            getPixelValue(vertices[BottomRight].x(), vertices[BottomRight].y())*scale
                );
            }

            vertices[TopLeft] = vertices[TopRight]; // use right vertices of the last box as new left vertices
            vertices[BottomLeft] = vertices[BottomRight];

            vertices[TopRight].setX(xp + 1);
            vertices[TopRight].setY(yp); // <----
            vertices[TopRight].setZ(
                getPixelValue(vertices[TopRight].x(), vertices[TopRight].y())*scale
            );

            vertices[BottomRight].setX(xp + 1);
            vertices[BottomRight].setY(yp + 1);
            vertices[BottomRight].setZ(
                getPixelValue(vertices[BottomRight].x(), vertices[BottomRight].y())*scale
                        );

            double zMin = vertices[TopLeft].z();
            double zMax = zMin;
            double zSum = zMin;

            for ( int i = TopRight; i <= BottomLeft; ++i ) {
                const double z = vertices[i].z();

                zSum += z;
                if ( z < zMin )
                    zMin = z;
                if ( z > zMax )
                    zMax = z;
            }


            if ( zMax >= contourLevels.first() && zMin <= contourLevels.last() ) {

                vertices[Center].setX(xp + 0.5); // pseudo pixel coordinates
                vertices[Center].setY(yp + 0.5);
                vertices[Center].setZ(0.25 * zSum);
                for (int levelIdx=0; levelIdx<contourLevels.size(); ++levelIdx) {
                    if ( contourLevels.at(levelIdx) >= zMin && contourLevels.at(levelIdx) <= zMax ) {

                        QLineF line;
                        QVector<QVector3D> triangle(3);

                       /*                     triangle[1]
                                                 X
                                               /   \
                                             /       \
                                           /     m     \
                                         /               \
                           triangle[2] +-------------------+ triangle[0]
                        */

                        for (int m = TopLeft; m < NumPositions; m++) { // construct triangles
                            triangle[0] = vertices[m];
                            triangle[1] = vertices[Center];
                            triangle[2] = vertices[(m!=BottomLeft)?(m + 1):TopLeft];

                            const bool intersects =intersect(line, triangle.at(0),triangle.at(1),triangle.at(2),
                                                             contourLevels.at(levelIdx));
                            if ( intersects ) {
                                ContourLines[levelIdx]<<line;
                            }
                        }
                    }
                }
            }
        }
    }
}




JKQTPColumnContourPlot::JKQTPColumnContourPlot(JKQTBasePlotter *parent):
    JKQTPContourPlot(parent)
{
    this->datatype=JKQTPMathImageDataType::DoubleArray;
}

JKQTPColumnContourPlot::JKQTPColumnContourPlot(JKQTPlotter *parent):
    JKQTPColumnContourPlot(parent->getPlotter())
{
}

void JKQTPColumnContourPlot::setImageColumn(int __value)
{
	this->imageColumn = __value;
	if (parent && __value >= 0 && parent->getDatastore()) {
		setNx(parent->getDatastore()->getColumnImageWidth(__value));
		setNy(parent->getDatastore()->getColumnImageHeight(__value));
	}
}

void JKQTPColumnContourPlot::setImageColumn(size_t __value)
{
	setImageColumn(static_cast<int>(__value));
}

int JKQTPColumnContourPlot::getImageColumn() const
{
    return this->imageColumn;
}

bool JKQTPColumnContourPlot::usesColumn(int c) const
{
    return (c==imageColumn);
}

void JKQTPColumnContourPlot::ensureImageData()
{
    if (this->Nx==0 || imageColumn<0 || !parent->getDatastore()->getColumnPointer(imageColumn,0)) {
        this->Ny=0;
        this->data=nullptr;
        this->datatype=JKQTPMathImageDataType::DoubleArray;
    } else {
        this->datatype=JKQTPMathImageDataType::DoubleArray;
        this->data=parent->getDatastore()->getColumnPointer(imageColumn,0);
        this->Ny=static_cast<int>(parent->getDatastore()->getRows(imageColumn)/this->Nx);
    }
}
