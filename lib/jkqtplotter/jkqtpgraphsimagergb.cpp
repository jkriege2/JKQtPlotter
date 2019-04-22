/*
    Copyright (c) 2008-2019 Jan W. Krieger

    

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


#include "jkqtplotter/jkqtpgraphsimagergb.h"
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



void JKQTPRGBMathImage::initObject()
{
    actSaveImage=new QAction(tr("Save JKQTPRGBMathImage ..."), this);
    connect(actSaveImage, SIGNAL(triggered()), this, SLOT(saveImagePlotAsImage()));
    actCopyImage=new QAction(tr("Copy JKQTPRGBMathImage ..."), this);
    connect(actCopyImage, SIGNAL(triggered()), this, SLOT(copyImagePlotAsImage()));

    rgbMode=JKQTPRGBMathImageModeRGBMode;
    colorBarRightAxis=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    colorBarRightAxis->setDrawMode1(JKQTPCADMLine);
    colorBarRightAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxis->setAxisLabel("");
    colorBarRightAxis->setMinTicks(5);
    colorBarRightAxis->setMinorTicks(0);
    colorBarRightAxis->setTickOutsideLength(0);
    colorBarRightAxis->setMinorTickOutsideLength(0);
    colorBarRightAxis->setShowZeroAxis(false);
    colorBarTopAxis=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    colorBarTopAxis->setDrawMode1(JKQTPCADMLine);
    colorBarTopAxis->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxis->setAxisLabel("");
    colorBarTopAxis->setMinTicks(3);
    colorBarTopAxis->setMinorTicks(0);
    colorBarTopAxis->setTickOutsideLength(0);
    colorBarTopAxis->setMinorTickOutsideLength(0);
    colorBarTopAxis->setShowZeroAxis(false);

    colorBarRightAxisG=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    colorBarRightAxisG->setDrawMode1(JKQTPCADMLine);
    colorBarRightAxisG->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxisG->setAxisLabel("");
    colorBarRightAxisG->setMinTicks(5);
    colorBarRightAxisG->setShowZeroAxis(false);
    colorBarRightAxisG->setMinorTicks(0);
    colorBarRightAxisG->setTickOutsideLength(0);
    colorBarRightAxisG->setMinorTickOutsideLength(0);
    colorBarTopAxisG=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    colorBarTopAxisG->setDrawMode1(JKQTPCADMLine);
    colorBarTopAxisG->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxisG->setAxisLabel("");
    colorBarTopAxisG->setMinTicks(3);
    colorBarTopAxisG->setShowZeroAxis(false);
    colorBarTopAxisG->setMinorTicks(0);
    colorBarTopAxisG->setTickOutsideLength(0);
    colorBarTopAxisG->setMinorTickOutsideLength(0);

    colorBarRightAxisB=new JKQTPVerticalIndependentAxis(0, 100, 0, 100, parent);
    colorBarRightAxisB->setDrawMode1(JKQTPCADMLine);
    colorBarRightAxisB->setDrawMode2(JKQTPCADMcomplete);
    colorBarRightAxisB->setAxisLabel("");
    colorBarRightAxisB->setMinTicks(5);
    colorBarRightAxisB->setShowZeroAxis(false);
    colorBarRightAxisB->setMinorTicks(0);
    colorBarRightAxisB->setTickOutsideLength(0);
    colorBarRightAxisB->setMinorTickOutsideLength(0);
    colorBarTopAxisB=new JKQTPHorizontalIndependentAxis(0, 100, 0, 100, parent);
    colorBarTopAxisB->setDrawMode1(JKQTPCADMLine);
    colorBarTopAxisB->setDrawMode2(JKQTPCADMcomplete);
    colorBarTopAxisB->setAxisLabel("");
    colorBarTopAxisB->setMinTicks(3);
    colorBarTopAxisB->setShowZeroAxis(false);
    colorBarTopAxisB->setMinorTicks(0);
    colorBarTopAxisB->setTickOutsideLength(0);
    colorBarTopAxisB->setMinorTickOutsideLength(0);


    this->colorBarTopVisible=true;
    this->colorBarRightVisible=true;
    this->imageNameFontSize=parent->getKeyFontSize();
    this->imageNameR="";
    this->showColorBar=true;
    this->colorBarWidth=14;
    this->colorBarRelativeHeight=0.75;
    this->autoImageRange=true;
    this->imageMinR=0;
    this->imageMaxR=1;
    this->imageMinG=0;
    this->imageMaxG=1;
    this->imageMinB=0;
    this->imageMaxB=1;
    this->colorBarOffset=4;
    this->colorBarTopVisible=false;
    this->colorBarRightVisible=true;
    this->colorbarsSideBySide=true;
}



JKQTPRGBMathImage::JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent)
{
    initObject();
}


JKQTPRGBMathImage::JKQTPRGBMathImage(double x, double y, double width, double height, DataType datatype, void* data, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPMathImageBase(x, y, width, height, datatype, data, Nx, Ny, parent)
{
    initObject();
}

JKQTPRGBMathImage::JKQTPRGBMathImage(JKQTBasePlotter *parent):
    JKQTPMathImageBase(0,0,0,0, DoubleArray, nullptr, 0, 0, parent)
{
    initObject();
}


JKQTPRGBMathImage::JKQTPRGBMathImage(JKQTPlotter *parent):
    JKQTPMathImageBase(0,0,0,0, DoubleArray, nullptr, 0, 0, parent)
{
    initObject();
}

void JKQTPRGBMathImage::setParent(JKQTBasePlotter* parent) {
    if (this->parent) {
        this->parent->deregisterAdditionalAction(actSaveImage);
        this->parent->deregisterAdditionalAction(actCopyImage);
    }
    JKQTPMathImageBase::setParent(parent);
    colorBarRightAxis->setParent(parent);
    colorBarTopAxis->setParent(parent);

    if (parent) {
        parent->registerAdditionalAction(tr("Save Image Plot Images ..."), actSaveImage);
        parent->registerAdditionalAction(tr("Copy Image Plot Images ..."), actCopyImage);
    }
    actSaveImage->setEnabled(parent);
    actCopyImage->setEnabled(parent);

}



void JKQTPRGBMathImage::draw(JKQTPEnhancedPainter& painter) {
    ensureImageData();
    if (!data && !dataG && !dataB) return;
    QImage img=drawImage();
    plotImage(painter, img, x, y, width, height);
}



void JKQTPRGBMathImage::getOutsideSize(JKQTPEnhancedPainter& painter, int& leftSpace, int& rightSpace, int& topSpace, int& bottomSpace) {
    ensureImageData();
    JKQTPGraph::getOutsideSize(painter, leftSpace, rightSpace, topSpace, bottomSpace);
    if (showColorBar) {
        int visibleColorBars=0;
        if (data) visibleColorBars++;
        if (dataG) visibleColorBars++;
        if (dataB) visibleColorBars++;
        double sizeFactor=0.8/double(visibleColorBars);
        if (!colorbarsSideBySide) sizeFactor=1;

        bool firstC=false;

        getDataMinMax(internalDataMin, internalDataMax);
        if (data) {
            if (colorBarRightVisible) {
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarRightAxis->setRange(internalDataMin, internalDataMax);
                colorBarRightAxis->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotHeight());
                colorBarRightAxisB->setAxisLabel(imageNameR);
                QSizeF s2=colorBarRightAxis->getSize2(painter);
                QSizeF s1=colorBarRightAxis->getSize1(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s2.width()+s1.width());
            }
            if (colorBarTopVisible) {
                //if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarTopAxis->setRange(internalDataMin, internalDataMax);
                colorBarTopAxis->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotWidth());
                colorBarTopAxisB->setAxisLabel(imageNameR);
                QSizeF s2=colorBarTopAxisB->getSize2(painter);
                QSizeF s1=colorBarTopAxisB->getSize2(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s2.height()+s1.height());
            }
            firstC=true;
        }

        getDataMinMaxG(internalDataMinG, internalDataMaxG);
        if (dataG) {
            if (colorBarRightVisible) {
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarRightAxisG->setRange(internalDataMinG, internalDataMaxG);
                colorBarRightAxisG->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotHeight());
                colorBarRightAxisB->setAxisLabel(imageNameG);
                QSizeF s2=colorBarRightAxis->getSize2(painter);
                QSizeF s1=colorBarRightAxis->getSize1(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s2.width()+s1.width());
            }
            if (colorBarTopVisible) {
                //if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarTopAxisG->setRange(internalDataMinG, internalDataMaxG);
                colorBarTopAxisG->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotWidth());
                colorBarTopAxisB->setAxisLabel(imageNameG);
                QSizeF s2=colorBarTopAxisB->getSize2(painter);
                QSizeF s1=colorBarTopAxisB->getSize1(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s2.height()+s1.height());
             }
            firstC=true;
        }

        getDataMinMaxB(internalDataMinB, internalDataMaxB);
        if (dataB) {
            if (colorBarRightVisible) {
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarRightAxisB->setRange(internalDataMinB, internalDataMaxB);
                colorBarRightAxisB->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotHeight());
                colorBarRightAxisB->setAxisLabel(imageNameB);
                QSizeF s2=colorBarRightAxis->getSize2(painter);
                QSizeF s1=colorBarRightAxis->getSize1(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) rightSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s2.width()+s1.width());
            }
            if (colorBarTopVisible) {
                //if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset);
                colorBarTopAxisB->setRange(internalDataMinB, internalDataMaxB);
                colorBarTopAxisB->setAxisWidth(sizeFactor*colorBarRelativeHeight*parent->getPlotWidth());
                colorBarTopAxisB->setAxisLabel(imageNameB);
                QSizeF s2=colorBarTopAxisB->getSize2(painter);
                QSizeF s1=colorBarTopAxisB->getSize1(painter);
                if (!colorbarsSideBySide || (colorbarsSideBySide && !firstC)) topSpace+=parent->pt2px(painter, colorBarWidth+colorBarOffset)+static_cast<double>(s2.height()+s1.height());
            }
            firstC=true;
        }
    }
}

struct RGBOutsizeData {
    double internalDataMin;
    double internalDataMax;
    void* data;
    JKQTPVerticalIndependentAxis* colorBarRightAxis;
    JKQTPHorizontalIndependentAxis* colorBarTopAxis;
    QString name;
    JKQTPMathImageColorPalette palette;
    QImage paletteImage;
};

void JKQTPRGBMathImage::drawOutside(JKQTPEnhancedPainter& painter, QRect /*leftSpace*/, QRect rightSpace, QRect topSpace, QRect /*bottomSpace*/) {
    ensureImageData();
    if (showColorBar) {
        QList<RGBOutsizeData> l;
        int visibleColorBars=0;
        const int pd_size=200;
        uint8_t pd[pd_size];
        for (int i=0; i<pd_size; i++) pd[i]=static_cast<uint8_t>(i);

        if (data) {
            RGBOutsizeData d;
            d.data=data;
            getDataMinMax(internalDataMin, internalDataMax);
            d.internalDataMin=internalDataMin;
            d.internalDataMax=internalDataMax;
            d.colorBarRightAxis=colorBarRightAxis;
            d.colorBarTopAxis=colorBarTopAxis;
            d.name=imageNameR;
            if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
                d.palette=JKQTPMathImageRED;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
                d.palette=JKQTPMathImageINVERTED_CYANWHITE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeHSVMode || rgbMode==JKQTPRGBMathImageModeHSLMode) {
                d.palette=JKQTPMathImageHSV;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            }

            l<<d;
            visibleColorBars++;
        }
        if (dataG) {
            RGBOutsizeData d;
            d.data=dataG;
            getDataMinMaxG(internalDataMinG, internalDataMaxG);
            d.internalDataMin=internalDataMinG;
            d.internalDataMax=internalDataMaxG;
            d.colorBarRightAxis=colorBarRightAxisG;
            d.colorBarTopAxis=colorBarTopAxisG;
            d.name=imageNameG;
            JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, 199);
            if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
                d.palette=JKQTPMathImageGREEN;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
                d.palette=JKQTPMathImageINVERTED_MAGENTAWHITE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsv(c.hue(), jkqtp_roundTo<int>(double(i)/double(pd_size-1)*255.0), c.value(), c.alpha());
                    line[i]=c.rgba();
                }
            } else if (rgbMode==JKQTPRGBMathImageModeHSLMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsl(c.hue(), jkqtp_roundTo<int>(double(i)/double(pd_size-1)*255.0), c.lightness(), c.alpha());
                    line[i]=c.rgba();
                }
            }

            l<<d;
            visibleColorBars++;
        }
        if (dataB) {
            RGBOutsizeData d;
            d.data=dataB;
            getDataMinMax(internalDataMinB, internalDataMaxB);
            d.internalDataMin=internalDataMinB;
            d.internalDataMax=internalDataMaxB;
            d.colorBarRightAxis=colorBarRightAxisB;
            d.colorBarTopAxis=colorBarTopAxisB;
            d.name=imageNameB;
            JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, 199);
            if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
                d.palette=JKQTPMathImageBLUE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
                d.palette=JKQTPMathImageINVERTED_YELLOWWHITE;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, d.palette, 0, pd_size-1);
            } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsv(c.hue(), c.saturation(), jkqtp_roundTo<int>(double(i)/double(pd_size-1)*255.0), c.alpha());
                    line[i]=c.rgba();
                }
            } else if (rgbMode==JKQTPRGBMathImageModeHSLMode) {
                d.palette=JKQTPMathImageGRAY;
                d.paletteImage=QImage(1, pd_size, QImage::Format_ARGB32);
                //JKQTPImagePlot_array2image<uint8_t>(pd, 1, pd_size, d.paletteImage, l[li].palette, 0, pd_size-1);
                QRgb* line=reinterpret_cast<QRgb *>(d.paletteImage.scanLine(0));
                for (int i=0; i<pd_size; i++) {
                    QColor c=QColor("red");
                    c.setHsl(c.hue(), c.saturation(), jkqtp_roundTo<int>(double(i)/double(pd_size-1)*255.0), c.alpha());
                    line[i]=c.rgba();
                }
            }
            l<<d;
            visibleColorBars++;
        }


        double sizeFactor=0.9/double(visibleColorBars);
        double shiftSizeFactor=1.0/double(visibleColorBars);
        if (!colorbarsSideBySide) {
            sizeFactor=1;
            shiftSizeFactor=1;
        }
        double icolorBarRelativeHeight=colorBarRelativeHeight*sizeFactor;

        int gbarHeight=qMax(1, static_cast<int>(round(static_cast<double>(rightSpace.height())*icolorBarRelativeHeight)));
        int gbarWidth=qMax(1, static_cast<int>(round(static_cast<double>(topSpace.width())*icolorBarRelativeHeight)));



        double rX=rightSpace.x()+parent->pt2px(painter, colorBarOffset);
        double rY=rightSpace.top()+(rightSpace.height()-gbarHeight)/2;
        double tX=topSpace.x()+(topSpace.width()-gbarWidth)/2;
        double tY=topSpace.bottom()-parent->pt2px(painter, colorBarOffset+colorBarWidth);
        if(colorbarsSideBySide) {
            rY=rightSpace.top()+(rightSpace.height()-gbarHeight*visibleColorBars)/2;
            tX=topSpace.x()+(topSpace.width()-gbarWidth*visibleColorBars)/2;
        }


        for (int li=0; li<l.size(); li++) {
            if (colorBarRightVisible) {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});


                QRectF cb(rX, rY, parent->pt2px(painter, colorBarWidth), gbarHeight);
                painter.drawImage(cb, l[li].paletteImage.mirrored(true, false));
                QPen p=painter.pen();
                p.setColor(l[li].colorBarRightAxis->getAxisColor());
                p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, l[li].colorBarRightAxis->getLineWidth()*parent->getLineWidthMultiplier())));
                painter.setPen(p);
                painter.drawRect(cb);

                l[li].colorBarRightAxis->setRange(l[li].internalDataMin, l[li].internalDataMax);
                l[li].colorBarRightAxis->setAxisWidth(cb.height());
                l[li].colorBarRightAxis->setAxisOffset(cb.top());
                l[li].colorBarRightAxis->setOtherAxisOffset(cb.left());
                l[li].colorBarRightAxis->setOtherAxisWidth(cb.width());
                l[li].colorBarRightAxis->setLabelFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                l[li].colorBarRightAxis->setAxisLabel(l[li].name);
                l[li].colorBarRightAxis->drawAxes(painter);



                if (!colorbarsSideBySide) {
                    rX=rX+static_cast<double>(rightSpace.width())/static_cast<double>(visibleColorBars);
                } else {
                    rY=rY+static_cast<double>(rightSpace.height())*shiftSizeFactor*colorBarRelativeHeight;
                }
            }
            if (colorBarTopVisible) {
                painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});


                QRectF cb(tX, tY, gbarWidth, parent->pt2px(painter, colorBarWidth));
                QMatrix mt;
                mt.rotate(90);
                painter.drawImage(cb, l[li].paletteImage.transformed(mt));
                QPen p=painter.pen();
                p.setColor(l[li].colorBarTopAxis->getAxisColor());
                p.setWidthF(qMax(JKQTPlotterDrawinTools::ABS_MIN_LINEWIDTH, parent->pt2px(painter, l[li].colorBarTopAxis->getLineWidth()*parent->getLineWidthMultiplier())));
                painter.setPen(p);
                painter.drawRect(cb);


                l[li].colorBarTopAxis->setRange(l[li].internalDataMin, l[li].internalDataMax);
                l[li].colorBarTopAxis->setAxisWidth(cb.width());
                l[li].colorBarTopAxis->setAxisOffset(cb.left());
                l[li].colorBarTopAxis->setOtherAxisOffset(cb.top());
                l[li].colorBarTopAxis->setOtherAxisWidth(cb.height());
                l[li].colorBarTopAxis->setLabelFontSize(imageNameFontSize*parent->getFontSizeMultiplier());
                l[li].colorBarTopAxis->setAxisLabel(l[li].name);
                l[li].colorBarTopAxis->drawAxes(painter);


                if (!colorbarsSideBySide) {
                    tY=tY-topSpace.height()/double(visibleColorBars);
                } else {
                    tX=tX+topSpace.width()*shiftSizeFactor*colorBarRelativeHeight;
                }
            }
        }
    }

}

void JKQTPRGBMathImage::getDataMinMax(double& imin, double& imax) {
    ensureImageData();
    if (autoImageRange) {
        JKQTPMathImageBase::getDataMinMax(imin, imax);
    } else {
        imin=imageMinR;
        imax=imageMaxR;
    }
}

void JKQTPRGBMathImage::setDataR(void *__value)
{
    setData(__value);
}

void *JKQTPRGBMathImage::getDataR() const
{
    return getData();
}

void JKQTPRGBMathImage::setDatatypeR(JKQTPMathImageBase::DataType __value)
{
    setDatatype(__value);
}

JKQTPMathImageBase::DataType JKQTPRGBMathImage::getDatatypeR() const
{
    return getDatatype();
}

void JKQTPRGBMathImage::setDataG(void *__value)
{
    this->dataG = __value;
}

void *JKQTPRGBMathImage::getDataG() const
{
    return this->dataG;
}

void JKQTPRGBMathImage::setDatatypeG(JKQTPMathImageBase::DataType __value)
{
    this->datatypeG = __value;
}

JKQTPMathImageBase::DataType JKQTPRGBMathImage::getDatatypeG() const
{
    return this->datatypeG;
}

void JKQTPRGBMathImage::setDataB(void *__value)
{
    this->dataB = __value;
}

void *JKQTPRGBMathImage::getDataB() const
{
    return this->dataB;
}

void JKQTPRGBMathImage::setDatatypeB(JKQTPMathImageBase::DataType __value)
{
    this->datatypeB = __value;
}

JKQTPMathImageBase::DataType JKQTPRGBMathImage::getDatatypeB() const
{
    return this->datatypeB;
}

void JKQTPRGBMathImage::setShowColorBar(bool __value)
{
    this->showColorBar = __value;
}

bool JKQTPRGBMathImage::getShowColorBar() const
{
    return this->showColorBar;
}

void JKQTPRGBMathImage::setColorBarWidth(int __value)
{
    this->colorBarWidth = __value;
}

int JKQTPRGBMathImage::getColorBarWidth() const
{
    return this->colorBarWidth;
}

void JKQTPRGBMathImage::setColorBarOffset(int __value)
{
    this->colorBarOffset = __value;
}

int JKQTPRGBMathImage::getColorBarOffset() const
{
    return this->colorBarOffset;
}

void JKQTPRGBMathImage::setColorBarRelativeHeight(double __value)
{
    this->colorBarRelativeHeight = __value;
}

double JKQTPRGBMathImage::getColorBarRelativeHeight() const
{
    return this->colorBarRelativeHeight;
}

void JKQTPRGBMathImage::setImageMin(double __value)
{
    this->imageMinR = __value;
}

double JKQTPRGBMathImage::getImageMin() const
{
    return this->imageMinR;
}

void JKQTPRGBMathImage::setImageMinR(double m) {
    setImageMin(m);
}

void JKQTPRGBMathImage::setImageMax(double __value)
{
    this->imageMaxR = __value;
}

double JKQTPRGBMathImage::getImageMax() const
{
    return this->imageMaxR;
}

void JKQTPRGBMathImage::setImageMaxR(double m) {
    setImageMax(m);
}

void JKQTPRGBMathImage::setImageMinG(double __value)
{
    this->imageMinG = __value;
}

double JKQTPRGBMathImage::getImageMinG() const
{
    return this->imageMinG;
}

void JKQTPRGBMathImage::setImageMaxG(double __value)
{
    this->imageMaxG = __value;
}

double JKQTPRGBMathImage::getImageMaxG() const
{
    return this->imageMaxG;
}

void JKQTPRGBMathImage::setImageMinB(double __value)
{
    this->imageMinB = __value;
}

double JKQTPRGBMathImage::getImageMinB() const
{
    return this->imageMinB;
}

void JKQTPRGBMathImage::setImageMaxB(double __value)
{
    this->imageMaxB = __value;
}

double JKQTPRGBMathImage::getImageMaxB() const {
    return imageMaxB;
}

void JKQTPRGBMathImage::setAutoImageRange(bool __value)
{
    this->autoImageRange = __value;
}

bool JKQTPRGBMathImage::getAutoImageRange() const
{
return this->autoImageRange;
}

void JKQTPRGBMathImage::setImageName(const QString &__value)
{
    this->imageNameR = __value;
}

QString JKQTPRGBMathImage::getImageName() const
{
    return this->imageNameR;
}

QString JKQTPRGBMathImage::getImageNameR() const
{
    return this->imageNameR;
}

void JKQTPRGBMathImage::setImageNameR(const QString &m) {
    setImageName(m);
}

void JKQTPRGBMathImage::setImageNameG(const QString &__value)
{
    this->imageNameG = __value;
}

QString JKQTPRGBMathImage::getImageNameG() const
{
    return this->imageNameG;
}

void JKQTPRGBMathImage::setImageNameB(const QString &__value)
{
    this->imageNameB = __value;
}

QString JKQTPRGBMathImage::getImageNameB() const
{
    return this->imageNameB;
}

void JKQTPRGBMathImage::setImageNameFontName(const QString &__value)
{
    this->imageNameFontName = __value;
}

QString JKQTPRGBMathImage::getImageNameFontName() const
{
    return this->imageNameFontName;
}

void JKQTPRGBMathImage::setImageNameFontSize(double __value)
{
    this->imageNameFontSize = __value;
}

double JKQTPRGBMathImage::getImageNameFontSize() const
{
    return this->imageNameFontSize;
}

JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxis() {
    return this->colorBarRightAxis;
}

JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxis()  {
    return this->colorBarTopAxis;
}

JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxisR()
{
    return this->colorBarRightAxis;
}

JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxisR()
{
    return this->colorBarTopAxis;
}

JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxisG()  {
    return this->colorBarRightAxisG;
}

JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxisG()  {
    return this->colorBarTopAxisG;
}

JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxisB()  {
    return this->colorBarRightAxisB;
}

JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxisB()  {
    return this->colorBarTopAxisB;
}

const JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxis() const {
    return this->colorBarRightAxis;
}

const JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxis() const {
    return this->colorBarTopAxis;
}

const JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxisR() const {
    return this->colorBarRightAxis;
}

const JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxisR() const {
    return this->colorBarTopAxis;
}

const JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxisG() const {
    return this->colorBarRightAxisG;
}

const JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxisG() const {
    return this->colorBarTopAxisG;
}

const JKQTPVerticalIndependentAxis *JKQTPRGBMathImage::getColorBarRightAxisB() const {
    return this->colorBarRightAxisB;
}

const JKQTPHorizontalIndependentAxis *JKQTPRGBMathImage::getColorBarTopAxisB() const {
    return this->colorBarTopAxisB;
}

void JKQTPRGBMathImage::setColorBarTopVisible(bool __value)
{
    this->colorBarTopVisible = __value;
}

bool JKQTPRGBMathImage::getColorBarTopVisible() const
{
    return this->colorBarTopVisible;
}

void JKQTPRGBMathImage::setColorBarRightVisible(bool __value)
{
    this->colorBarRightVisible = __value;
}

bool JKQTPRGBMathImage::getColorBarRightVisible() const
{
    return this->colorBarRightVisible;
}

void JKQTPRGBMathImage::setColorbarsSideBySide(bool __value)
{
    this->colorbarsSideBySide = __value;
}

bool JKQTPRGBMathImage::getColorbarsSideBySide() const
{
    return this->colorbarsSideBySide;
}

void JKQTPRGBMathImage::setRgbMode(JKQTPRGBMathImageRGBMode __value)
{
    this->rgbMode = __value;
}

JKQTPRGBMathImageRGBMode JKQTPRGBMathImage::getRgbMode() const
{
    return this->rgbMode;
}

void JKQTPRGBMathImage::getDataMinMaxG(double& imin, double& imax) {
    ensureImageData();
    if (autoImageRange) {
        imin=imax=0;
        if (!dataG) return;
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray:
                imin= JKQTPImagePlot_getImageMin<double>(static_cast<double*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<double>(static_cast<double*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::FloatArray:
                imin= JKQTPImagePlot_getImageMin<float>(static_cast<float*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<float>(static_cast<float*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt8Array:
                imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<uint8_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<uint8_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt16Array:
                imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<uint16_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<uint16_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt32Array:
                imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<uint32_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<uint32_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt64Array:
                imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<uint64_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<uint64_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int8Array:
                imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<int8_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<int8_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int16Array:
                imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<int16_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<int16_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int32Array:
                imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<int32_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<int32_t*>(dataG), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int64Array:
                imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<int64_t*>(dataG), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<int64_t*>(dataG), Nx, Ny);
                break;
        }
    } else {
        imin=imageMinG;
        imax=imageMaxG;
    }
}

void JKQTPRGBMathImage::getDataMinMaxB(double& imin, double& imax) {
    ensureImageData();
    if (autoImageRange) {
        imin=imax=0;
        if (!dataG) return;
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray:
                imin= JKQTPImagePlot_getImageMin<double>(static_cast<double*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<double>(static_cast<double*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::FloatArray:
                imin= JKQTPImagePlot_getImageMin<float>(static_cast<float*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<float>(static_cast<float*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt8Array:
                imin= JKQTPImagePlot_getImageMin<uint8_t>(static_cast<uint8_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint8_t>(static_cast<uint8_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt16Array:
                imin= JKQTPImagePlot_getImageMin<uint16_t>(static_cast<uint16_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint16_t>(static_cast<uint16_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt32Array:
                imin= JKQTPImagePlot_getImageMin<uint32_t>(static_cast<uint32_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint32_t>(static_cast<uint32_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::UInt64Array:
                imin= JKQTPImagePlot_getImageMin<uint64_t>(static_cast<uint64_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<uint64_t>(static_cast<uint64_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int8Array:
                imin= JKQTPImagePlot_getImageMin<int8_t>(static_cast<int8_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int8_t>(static_cast<int8_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int16Array:
                imin= JKQTPImagePlot_getImageMin<int16_t>(static_cast<int16_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int16_t>(static_cast<int16_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int32Array:
                imin= JKQTPImagePlot_getImageMin<int32_t>(static_cast<int32_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int32_t>(static_cast<int32_t*>(dataB), Nx, Ny);
                break;
            case JKQTPMathImageBase::Int64Array:
                imin= JKQTPImagePlot_getImageMin<int64_t>(static_cast<int64_t*>(dataB), Nx, Ny);
                imax= JKQTPImagePlot_getImageMax<int64_t>(static_cast<int64_t*>(dataB), Nx, Ny);
                break;
        }
    } else {
        imin=imageMinB;
        imax=imageMaxB;
    }
}

double JKQTPRGBMathImage::getValueAt(double x, double y, int channel)
{
    ensureImageData();
    void* dd=data;
    if (channel==0) dd=data;
    if (channel==1) dd=dataG;
    if (channel==2) dd=dataB;
    int xx=static_cast<int>(trunc((x-this->x)/width*Nx));
    int yy=static_cast<int>(trunc((y-this->y)/height*Ny));
    if (xx>=0 && xx<Nx && yy>=0 && yy<Ny) {
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray: return static_cast<double>((static_cast<double*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::FloatArray: return static_cast<double>((static_cast<float*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt8Array: return static_cast<double>((static_cast<uint8_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt16Array: return static_cast<double>((static_cast<uint16_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt32Array: return static_cast<double>((static_cast<uint32_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::UInt64Array: return static_cast<double>((static_cast<uint64_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int8Array: return static_cast<double>((static_cast<int8_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int16Array: return static_cast<double>((static_cast<int16_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int32Array: return static_cast<double>((static_cast<int32_t*>(dd))[yy*Nx+xx]); break;
            case JKQTPMathImageBase::Int64Array: return static_cast<double>((static_cast<int64_t*>(dd))[yy*Nx+xx]); break;
        }    }
    return 0.0;
}

void JKQTPRGBMathImage::drawKeyMarker(JKQTPEnhancedPainter &painter, QRectF &rect)
{
    painter.drawImage(rect, QPixmap(":/JKQTPlotter/jkqtp_plot_rgbimage.png").toImage());
}

void JKQTPRGBMathImage::setTitle(const QString &title)
{
    JKQTPImageBase::setTitle(title);
    QString t=title;
    if (t.isEmpty()) t="JKQTPRGBMathImage";
    actSaveImage->setText(tr("Save %1 ...").arg(t));
    actCopyImage->setText(tr("Copy %1 ...").arg(t));
}

void JKQTPRGBMathImage::saveImagePlotAsImage(const QString &filename, const QByteArray &outputFormat)
{
    if (parent) {
        parent->loadUserSettings();
        QString currentSaveDirectory=parent->getCurrentSaveDirectory();
        QString currentFileFormat=parent->getCurrentFileFormat();
        QString fn=filename;
        QStringList filt;
        QList<QByteArray> writerformats=QImageWriter::supportedImageFormats();
        for (int i=0; i<writerformats.size(); i++) {
            filt<<QString("%1 Image (*.%2)").arg(QString(writerformats[i]).toUpper()).arg(QString(writerformats[i].toLower()));
        }
        QString selFormat;
        if (fn.isEmpty()) {
            selFormat=currentFileFormat;
            fn = QFileDialog::getSaveFileName(nullptr, tr("Save Image Plot As Image ..."),
                                        currentSaveDirectory,
                                              filt.join(";;"), &selFormat);
            if (!fn.isEmpty()) currentSaveDirectory=QFileInfo(fn).absolutePath();
        }

        parent->setCurrentFileFormat(currentFileFormat);
        parent->setCurrentSaveDirectory(currentSaveDirectory);
        parent->saveUserSettings();
        if (!fn.isEmpty()) {
            int filtID=filt.indexOf(selFormat);
            QString form="NONE";
            if (filtID>=0 && filtID<writerformats.size()) {
                form=writerformats[filtID];
            }
            if (outputFormat.size()>0) {
                form =outputFormat;
            }

            QImage image=drawImage();

            if (form=="NONE") image.save(fn);
            else image.save(fn, form.toLatin1().data());
        }
    }
}

void JKQTPRGBMathImage::copyImagePlotAsImage()
{
    QClipboard* clip=QApplication::clipboard();
    if (clip) {
        clip->setPixmap(QPixmap::fromImage(drawImage()));
    }
}

QImage JKQTPRGBMathImage::drawImage() {
    ensureImageData();
    if (!data && !dataG && !dataB) return QImage();
    QImage img(Nx, Ny, QImage::Format_ARGB32);

    if (rgbMode==JKQTPRGBMathImageModeRGBMode) {
        img.fill(Qt::transparent);
    } else if (rgbMode==JKQTPRGBMathImageModeCMYMode) {
        img.fill(Qt::white);
    } else if (rgbMode==JKQTPRGBMathImageModeHSVMode) {
        QColor c("white");
        c.setHsv(0,255,255);
        img.fill(c.rgba());
    } else if (rgbMode==JKQTPRGBMathImageModeHSLMode) {
        QColor c("white");
        c.setHsv(0,255,127);
        img.fill(c.rgba());
    }
    getDataMinMax(internalDataMin, internalDataMax);
    getDataMinMaxG(internalDataMinG, internalDataMaxG);
    getDataMinMaxB(internalDataMinB, internalDataMaxB);
    int palette = 0;
    if (data) {
        switch(datatype) {
            case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
            case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(data), Nx, Ny, img, palette, internalDataMin, internalDataMax, rgbMode); break;
        }
    }
    palette = 1;
    if (dataG) {
        switch(datatypeG) {
            case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
            case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(dataG), Nx, Ny, img, palette, internalDataMinG, internalDataMaxG, rgbMode); break;
        }
    }
    palette = 2;
    if (dataB) {
        switch(datatypeB) {
            case JKQTPMathImageBase::DoubleArray: JKQTPImagePlot_array2RGBimage<double>(static_cast<double*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::FloatArray: JKQTPImagePlot_array2RGBimage<float>(static_cast<float*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt8Array: JKQTPImagePlot_array2RGBimage<uint8_t>(static_cast<uint8_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt16Array: JKQTPImagePlot_array2RGBimage<uint16_t>(static_cast<uint16_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt32Array: JKQTPImagePlot_array2RGBimage<uint32_t>(static_cast<uint32_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::UInt64Array: JKQTPImagePlot_array2RGBimage<uint64_t>(static_cast<uint64_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int8Array: JKQTPImagePlot_array2RGBimage<int8_t>(static_cast<int8_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int16Array: JKQTPImagePlot_array2RGBimage<int16_t>(static_cast<int16_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int32Array: JKQTPImagePlot_array2RGBimage<int32_t>(static_cast<int32_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
            case JKQTPMathImageBase::Int64Array: JKQTPImagePlot_array2RGBimage<int64_t>(static_cast<int64_t*>(dataB), Nx, Ny, img, palette, internalDataMinB, internalDataMaxB, rgbMode); break;
        }
    }

    modifyImage(img);

    return img;
}

void JKQTPRGBMathImage::setData(void* data, int Nx, int Ny, DataType datatype) {
    this->data=data;
    this->datatype=datatype;
    this->dataG=nullptr;
    this->dataB=nullptr;
    this->Nx=Nx;
    this->Ny=Ny;
}

void JKQTPRGBMathImage::setData(void* data, int Nx, int Ny) {
    this->data=data;
    this->Nx=Nx;
    this->Ny=Ny;
    this->dataG=nullptr;
    this->dataB=nullptr;
}


void JKQTPRGBMathImage::setData(void *data, void *dataG, void *dataB, int Nx, int Ny, JKQTPMathImageBase::DataType datatype) {
    this->data=data;
    this->datatype=datatype;
    this->datatypeG=datatype;
    this->datatypeB=datatype;
    this->dataG=dataG;
    this->dataB=dataB;
    this->Nx=Nx;
    this->Ny=Ny;
}

void JKQTPRGBMathImage::setData(void *data, void *dataG, void *dataB, int Nx, int Ny) {
    this->data=data;
    this->dataG=dataG;
    this->dataB=dataB;
    this->Nx=Nx;
    this->Ny=Ny;
}



JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(JKQTBasePlotter *parent):
    JKQTPRGBMathImage(0,0,0,0,DoubleArray,nullptr,0,0,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, int Nx, int Ny, JKQTBasePlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=imageBColumn;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(JKQTPlotter *parent):
    JKQTPRGBMathImage(0,0,0,0,DoubleArray,nullptr,0,0,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=-1;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=-1;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=-1;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}

JKQTPColumnRGBMathImage::JKQTPColumnRGBMathImage(double x, double y, double width, double height, int imageRColumn, int imageGColumn, int imageBColumn, int Nx, int Ny, JKQTPlotter *parent):
    JKQTPRGBMathImage(x,y,width,height,DoubleArray,nullptr,Nx,Ny,parent)
{
    this->modifierColumn=-1;
    this->imageRColumn=imageRColumn;
    this->imageGColumn=imageGColumn;
    this->imageBColumn=imageBColumn;
    this->datatype=JKQTPMathImageBase::DoubleArray;
}
bool JKQTPColumnRGBMathImage::usesColumn(int c) const
{
    return (c==imageRColumn)||(c==imageBColumn)||(c==imageGColumn)||(c==modifierColumn);
}


void JKQTPColumnRGBMathImage::ensureImageData()
{
    this->datatype=JKQTPMathImageBase::DoubleArray;
    this->datatypeG=JKQTPMathImageBase::DoubleArray;
    this->datatypeB=JKQTPMathImageBase::DoubleArray;
    this->data=parent->getDatastore()->getColumn(imageRColumn).getPointer(0);
    this->dataG=parent->getDatastore()->getColumn(imageGColumn).getPointer(0);
    this->dataB=parent->getDatastore()->getColumn(imageBColumn).getPointer(0);
    /*if (Nx*Ny==0 || Nx*Ny>parent->getDatastore()->getColumn(imageRColumn).getRows()) {
        if (Nx>0) {
            Ny=parent->getDatastore()->getColumn(imageRColumn).getRows()/this->Nx;
        } else {
            Nx=parent->getDatastore()->getColumn(imageRColumn).getRows();
            Ny=1;
        }
    }*/
    this->datatypeModifier=JKQTPMathImageBase::DoubleArray;
    this->dataModifier=parent->getDatastore()->getColumn(modifierColumn).getPointer(0);
}


QVector<double> JKQTPRGBMathImage::getDataGAsDoubleVector() const
{
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<double*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<float*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint8_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint16_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint32_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint64_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int8_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int16_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int32_t*>(dataG), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int64_t*>(dataG), Nx*Ny);
            break;
    }
    QVector<double> res;
    return res;
}

QVector<double> JKQTPRGBMathImage::getDataBAsDoubleVector() const
{
    switch(datatype) {
        case JKQTPMathImageBase::DoubleArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<double*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::FloatArray:
            return JKQTPImagePlot_arrayToDVector(static_cast<float*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint8_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint16_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint32_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::UInt64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<uint64_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int8Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int8_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int16Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int16_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int32Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int32_t*>(dataB), Nx*Ny);
            break;
        case JKQTPMathImageBase::Int64Array:
            return JKQTPImagePlot_arrayToDVector(static_cast<int64_t*>(dataB), Nx*Ny);
            break;
    }
    QVector<double> res;
    return res;
}




int JKQTPColumnRGBMathImage::getModifierColumn() const
{
    return this->modifierColumn;
}

void JKQTPColumnRGBMathImage::setModifierColumn(int __value)
{
    this->modifierColumn = __value;
}

int JKQTPColumnRGBMathImage::getImageBColumn() const
{
    return this->imageBColumn;
}

void JKQTPColumnRGBMathImage::setImageBColumn(int __value)
{
    this->imageBColumn = __value;
}

int JKQTPColumnRGBMathImage::getImageGColumn() const
{
    return this->imageGColumn;
}

void JKQTPColumnRGBMathImage::setImageGColumn(int __value)
{
    this->imageGColumn = __value;
}

int JKQTPColumnRGBMathImage::getImageRColumn() const
{
    return this->imageRColumn;
}

void JKQTPColumnRGBMathImage::setImageRColumn(int __value)
{
    this->imageRColumn = __value;
}
