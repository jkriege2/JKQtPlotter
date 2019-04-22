/*
    Copyright (c) 2008-2019 Jan W. Krieger & Sebastian Isbaner (contour plot)

    

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


#include "jkqtplotter/jkqtpgraphscontour.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtpimagetools.h"
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtpenhancedpainter.h"
#include "jkqtplotter/jkqtplotter.h"
#include <QDebug>
#include <QImageWriter>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QClipboard>
# include <QVector3D>

JKQTPContour::JKQTPContour(JKQTBasePlotter *parent) :
    JKQTPMathImage(parent)
{
    colorBarRightVisible=false;
    ignoreOnPlane=false;
    numberOfLevels=1;
    colorFromPalette=true;
    datatype=JKQTPMathImageBase::DoubleArray;
    relativeLevels=false;

    initLineStyle(parent, parentPlotStyle);

}

JKQTPContour::JKQTPContour(double x, double y, double width, double height, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, DataType datatype, JKQTBasePlotter* parent) :
    JKQTPMathImage( x, y, width, height, datatype, data, Nx, Ny, palette, parent)
{
    colorBarRightVisible=false;
    ignoreOnPlane=false;
    numberOfLevels=1;
    colorFromPalette=true;
    relativeLevels=false;

    initLineStyle(parent, parentPlotStyle);
}


JKQTPContour::JKQTPContour(JKQTPlotter *parent) :
    JKQTPContour(parent->getPlotter())
{

}

JKQTPContour::JKQTPContour(double x, double y, double width, double height, void* data, int Nx, int Ny, JKQTPMathImageColorPalette palette, DataType datatype, JKQTPlotter* parent) :
    JKQTPContour( x, y, width, height, data, Nx, Ny, palette, datatype, parent->getPlotter())
{
}


void JKQTPContour::draw(JKQTPEnhancedPainter &painter)
{
    //qDebug()<<"JKQTPContourPlot::draw";

    if(contourLevels.isEmpty()) createContourLevels(numberOfLevels);
    else {
        numberOfLevels=contourLevels.size();
//        qSort(contourLevels);
    }

    if(contourLines.isEmpty()) { // contour lines are only calculated once
        for(int i =0; i<contourLevels.size();++i) {
            contourLines.append(QVector<QLineF> (0));
        }
        this->calcContourLines(contourLines);
    }



    // draw lines
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});
    QPen p=getLinePen(painter, parent);

    painter.setPen(p);
    QImage colorLevels = getPaletteImage(palette,numberOfLevels);
    QVector<QLineF> contourLinesTransformedSingleLevel;
    QLineF lineTranformed;
    for(int i =0; i<numberOfLevels;++i) {
        if(colorFromPalette) {
            p.setColor(QColor(colorLevels.pixel(i,0)));
            painter.setPen(p);
        }
        // transform into plot coordinates
        for(QVector<QLineF >::const_iterator line =contourLines.at(i).begin(); line!=contourLines.at(i).end();++line ) {
            lineTranformed.setP1(transform(x+line->p1().x()/double(Nx-1)*width, y+line->p1().y()/double(Ny-1)*height));
            lineTranformed.setP2(transform(x+line->p2().x()/double(Nx-1)*width, y+line->p2().y()/double(Ny-1)*height));
            contourLinesTransformedSingleLevel.append(lineTranformed);
        }
        painter.drawLines(contourLinesTransformedSingleLevel);
        contourLinesTransformedSingleLevel.clear();
    }


}

void JKQTPContour::createContourLevels(int nLevels)
{
    if (!data) return;
    if (nLevels<1) return;
    double min,max;
    getDataMinMax(min,max);
    double delta=(max-min)/static_cast<double>(nLevels+1);

    for(int i=1; i<=nLevels; ++i) {
        contourLevels.append(min + i*delta);
    }
    relativeLevels=false;

}

void JKQTPContour::createContourLevelsLog(int nLevels, int m)
{
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
}

void JKQTPContour::setIgnoreOnPlane(bool __value)
{
    this->ignoreOnPlane = __value;
}

bool JKQTPContour::getIgnoreOnPlane() const
{
    return this->ignoreOnPlane;
}

void JKQTPContour::setNumberOfLevels(int __value)
{
    this->numberOfLevels = __value;
}

int JKQTPContour::getNumberOfLevels() const
{
    return this->numberOfLevels;
}

void JKQTPContour::setColorFromPalette(bool __value)
{
    this->colorFromPalette = __value;
}

bool JKQTPContour::getColorFromPalette() const
{
    return this->colorFromPalette;
}

void JKQTPContour::setContourLevels(const QList<double> &__value)
{
    this->contourLevels = __value;
}

QList<double> JKQTPContour::getContourLevels() const
{
    return this->contourLevels;
}

void JKQTPContour::setRelativeLevels(bool __value)
{
    this->relativeLevels = __value;
}

bool JKQTPContour::getRelativeLevels() const
{
    return this->relativeLevels;
}

void JKQTPContour::setImageColumn(size_t columnID)
{
    datatype=JKQTPMathImageBase::DoubleArray;
    data=parent->getDatastore()->getColumn(columnID).getPointer(0);
}


void JKQTPContour::ensureImageData()
{
}

double JKQTPContour::value(int xIdx, int yIdx)
{
    // row-major in datastore
    ensureImageData();
    if (!data) return 0;
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return (static_cast<double*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::FloatArray:
            return (static_cast<float*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt8Array:
            return (static_cast<uint8_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt16Array:
            return (static_cast<uint16_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt32Array:
            return (static_cast<uint32_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::UInt64Array:
            return (static_cast<uint64_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int8Array:
            return (static_cast<int8_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int16Array:
            return (static_cast<int16_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int32Array:
            return (static_cast<int32_t*>(data))[yIdx*getNx()+xIdx];
        case JKQTPMathImageBase::Int64Array:
            return (static_cast<int64_t*>(data))[yIdx*getNx()+xIdx];
    default:
        return 0;
    }
}

bool JKQTPContour::intersect(QLineF &line, const QVector3D &vertex1,const QVector3D &vertex2,const QVector3D &vertex3,double level)
{
    bool found = true;

    // Are the vertices below (-1), on (0) or above (1) the plane ?
    const int eq1 = compare2level(vertex1,level);
    const int eq2 = compare2level(vertex2,level);
    const int eq3 = compare2level(vertex3,level);

    /*
        (a) All the vertices lie below the contour level.
        (b) Two vertices lie below and one on the contour level.
        (c) Two vertices lie below and one above the contour level.
        (d) One vertex lies below and two on the contour level.
        (e) One vertex lies below, one on and one above the contour level.
        (f) One vertex lies below and two above the contour level.
        (g) Three vertices lie on the contour level.
        (h) Two vertices lie on and one above the contour level.
        (i) One vertex lies on and two above the contour level.
        (j) All the vertices lie above the contour level.
     */

    static const int caseLUT[3][3][3] =
    {
        // jump table to avoid nested case statements
        { { 0, 0, 8 }, { 0, 2, 5 }, { 7, 6, 9 } },
        { { 0, 3, 4 }, { 1, 10, 1 }, { 4, 3, 0 } },
        { { 9, 6, 7 }, { 5, 2, 0 }, { 8, 0, 0 } }
    };

    const int caseType = caseLUT[eq1+1][eq2+1][eq3+1];
    switch (caseType)
    {
        case 1:
            // d(0,0,-1), h(0,0,1)
            line.setP1(vertex1.toPointF());
            line.setP2(vertex2.toPointF());
            break;
        case 2:
            // d(-1,0,0), h(1,0,0)
            line.setP1(vertex2.toPointF());
            line.setP2(vertex3.toPointF());
            break;
        case 3:
            // d(0,-1,0), h(0,1,0)
            line.setP1(vertex3.toPointF());
            line.setP2(vertex1.toPointF());
            break;
        case 4:
            // e(0,-1,1), e(0,1,-1)
            line.setP1(vertex1.toPointF());
            line.setP2(interpolatePoint(vertex2, vertex3, level));
            break;
        case 5:
            // e(-1,0,1), e(1,0,-1)
            line.setP1(vertex2.toPointF());
            line.setP2(interpolatePoint(vertex3, vertex1, level));
            break;
        case 6:
            // e(-1,1,0), e(1,0,-1)
            line.setP1(vertex3.toPointF());
            line.setP2(interpolatePoint(vertex1, vertex2, level));
            break;
        case 7:
            // c(-1,1,-1), f(1,1,-1)
            line.setP1(interpolatePoint(vertex1, vertex2, level));
            line.setP2(interpolatePoint(vertex2, vertex3, level));
            break;
        case 8:
            // c(-1,-1,1), f(1,1,-1)
            line.setP1(interpolatePoint(vertex2, vertex3, level));
            line.setP2(interpolatePoint(vertex3, vertex1, level));
            break;
        case 9:
            // f(-1,1,1), c(1,-1,-1)
            line.setP1(interpolatePoint(vertex3, vertex1, level));
            line.setP2(interpolatePoint(vertex1, vertex2, level));
            break;
        case 10:
            // g(0,0,0)
            // The CONREC algorithm has no satisfying solution for
            // what to do, when all vertices are on the plane.

            if ( ignoreOnPlane )
                found = false;
            else
            {
                line.setP1(vertex3.toPointF());
                line.setP2(vertex1.toPointF());
            }
            break;
        default:
            found = false;
    }
//    qDebug()<<caseType;
//    qDebug()<<line;
    return found;
}


int JKQTPContour::compare2level(const QVector3D &vertex, double level)
{
    if (vertex.z() > level)
        return 1;

    if (vertex.z() < level)
        return -1;

    return 0;
}

void JKQTPContour::calcContourLines(QList<QVector<QLineF> > &ContourLines)
{

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

        for ( int xp = 0; xp < (int64_t)getNy() - 1; ++xp ) {

            if ( xp == 0 )
            {
                vertices[TopRight].setX(xp); // will be used for TopLeft later
                vertices[TopRight].setY(yp);
                vertices[TopRight].setZ(
                            value( vertices[TopRight].x(), vertices[TopRight].y())*scale
                );

                vertices[BottomRight].setX(xp);
                vertices[BottomRight].setY(yp+1);
                vertices[BottomRight].setZ(
                            value(vertices[BottomRight].x(), vertices[BottomRight].y())*scale
                );
            }

            vertices[TopLeft] = vertices[TopRight]; // use right vertices of the last box as new left vertices
            vertices[BottomLeft] = vertices[BottomRight];

            vertices[TopRight].setX(xp + 1);
            vertices[TopRight].setY(yp); // <----
            vertices[TopRight].setZ(
                value(vertices[TopRight].x(), vertices[TopRight].y())*scale
            );

            vertices[BottomRight].setX(xp + 1);
            vertices[BottomRight].setY(yp + 1);
            vertices[BottomRight].setZ(
                value(vertices[BottomRight].x(), vertices[BottomRight].y())*scale
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

QPointF JKQTPContour::interpolatePoint(const QVector3D &point1, const QVector3D &point2,double level)
{  
    const double h1 = point1.z() - level; // height above contour level
    const double h2 = point2.z() - level;

//    // check if h1 or h2 is zero
//    Division by zero is not possible (the intersect function is not called if h2-h1 is zero, !)
//    if(h2-h1==0||h1==0||h2==0) {
//        qDebug()<<h1<<h2;
//        qDebug()<<"interpolate p1="<<point1<<", p2="<<point2<<" level="<<level;
//    }

    const double x = (h2 * point1.x() - h1 * point2.x()) / (h2 - h1); // linear interpolation in x.direction (independent of y)
    const double y = (h2 * point1.y() - h1 * point2.y()) / (h2 - h1);

//    const double alpha=(level-point1.z())/(point2.z()-point1.z());
//    const double x=point1.x()+alpha*(point2.x()-point1.x());
//    const double y=point1.y()+alpha*(point2.y()-point1.y());

    return QPointF(x, y);
}

