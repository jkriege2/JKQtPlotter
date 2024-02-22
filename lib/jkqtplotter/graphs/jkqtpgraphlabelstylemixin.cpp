/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    

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

#include "jkqtplotter/graphs/jkqtpgraphlabelstylemixin.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include <QApplication>
#include <QDebug>
#include <QDateTime>


QString JKQTPGraphLabelPosition2String(JKQTPGraphLabelPosition pos)
{
    switch(pos) {
    case JKQTPGLabelAboveData: return "label_above_data";
    case JKQTPGLabelBelowData: return "label_below_data";
    case JKQTPGLabelLeftHandSide: return "label_left_hand_side";
    case JKQTPGLabelRightHandSide: return "label_right_hand_side";
    case JKQTPGLabelAwayFromXAxis: return "label_away_from_xaxis";
    case JKQTPGLabelAwayFromYAxis: return "label_away_from_yaxis";
    case JKQTPGLabelTowardsXAxis: return "label_towards_xaxis";
    case JKQTPGLabelTowardsYAxis: return "label_towards_xaxis";
    case JKQTPGLabelCenteredOnData: return "label_centered";
    case JKQTPGLabelHalfwaysToXAxis: return "label_halfways_to_xaxis";
    case JKQTPGLabelHalfwaysToYAxis: return "label_halfways_to_yaxis";
    }
    return "label_away_from_xaxis";
}

JKQTPGraphLabelPosition String2JKQTPGraphLabelPosition(const QString &pos)
{
    const QString m=pos.trimmed().toLower();
    if (m=="label_above_data" || m=="above_data" || m=="above") return JKQTPGLabelAboveData;
    if (m=="label_below_data" || m=="below_data" || m=="below") return JKQTPGLabelBelowData;
    if (m=="label_left_hand_side" || m=="left_hand_side" || m=="left_hand" || m=="left" || m=="lhs") return JKQTPGLabelLeftHandSide;
    if (m=="label_right_hand_side" || m=="right_hand_side" || m=="right_hand" || m=="right" || m=="rhs") return JKQTPGLabelRightHandSide;
    if (m=="label_away_from_xaxis" || m=="away_from_xaxis") return JKQTPGLabelAwayFromXAxis;
    if (m=="label_away_from_yaxis" || m=="away_from_yaxis") return JKQTPGLabelAwayFromYAxis;
    if (m=="label_towards_xaxis" || m=="towardds_xaxis") return JKQTPGLabelTowardsXAxis;
    if (m=="label_towards_yaxis" || m=="towars_yaxis") return JKQTPGLabelTowardsYAxis;
    if (m=="label_halfways_to_xaxis" || m=="halfways_to_xaxis") return JKQTPGLabelHalfwaysToXAxis;
    if (m=="label_halfways_to_yaxis" || m=="halfways_to_yaxis") return JKQTPGLabelHalfwaysToYAxis;
    if (m=="label_centered" || m=="centered" || m=="label_centered_on_data" || m=="centered_on_data") return JKQTPGLabelCenteredOnData;
    return JKQTPGraphLabelDefault;
}

QString JKQTPGraphLabelBoxType2String(JKQTPGraphLabelBoxType pos)
{
    switch(pos) {
    case JKQTPGLSimpleBox: return "simple_box";
    case JKQTPGLSimpleBoxAndLine: return "simple_box_and_line";
    }
    return "simple_box";
}

JKQTPGraphLabelBoxType String2JKQTPGraphLabelBoxType(const QString &pos)
{
    const QString m=pos.trimmed().toLower();
    if (m=="simple_box" || m=="simple" || m=="box") return JKQTPGLSimpleBox;
    if (m=="simple_box_and_line" || m=="box_and_line") return JKQTPGLSimpleBoxAndLine;
    return JKQTPGraphLabelBoxDefault;
}



JKQTPGraphValueLabelStyleMixin::JKQTPGraphValueLabelStyleMixin(JKQTBasePlotter *parent):
    JKQTPGraphTextStyleMixin(parent), JKQTPGraphLineStyleMixin(), JKQTPGraphFillStyleMixin()
{
    m_labelOffset=getTextFontSize()/4.0;
    m_labelOffsetWithConnector=getTextFontSize()*0.75;
    m_labelBoxRounding=getTextFontSize()/3.0;
    m_labelXPadding=getTextFontSize()/3.0;
    m_labelYPadding=getTextFontSize()/8.0;
    m_labelPosition=JKQTPGraphLabelDefault;
    m_labelBoxType=JKQTPGLSimpleBox;
    m_drawLabelBoxFrame=false;
    setFillStyle(Qt::NoBrush);
}

void JKQTPGraphValueLabelStyleMixin::initValueLabelStyle(JKQTBasePlotter *parent, int &parentPlotStyle, JKQTPPlotStyleType styletype)
{

    setFillStyle(Qt::NoBrush);
    if (parent) { // get style settings from parent object
        initTextStyle(parent, parentPlotStyle,styletype);
        initLineStyle(parent, parentPlotStyle, styletype);
        initFillStyle(parent, parentPlotStyle, styletype);
        setFillColor(QColorWithAlphaF(parent->getCurrentPlotterStyle().plotBackgroundBrush.color(), 0.5));
        setFillStyle(Qt::SolidPattern);
        setLineWidth(parent->getCurrentPlotterStyle().keyStyle.frameWidth);
    }
}

JKQTPGraphValueLabelStyleMixin::~JKQTPGraphValueLabelStyleMixin()
{

}

void JKQTPGraphValueLabelStyleMixin::setLabelOffset(double r)
{
    m_labelOffset=r;
}

double JKQTPGraphValueLabelStyleMixin::getLabelOffset() const
{
    return m_labelOffset;
}

double JKQTPGraphValueLabelStyleMixin::getUsedLabelOffset() const
{
    if (m_labelBoxType==JKQTPGLSimpleBox) {
        return m_labelOffset;
    }

    return m_labelOffsetWithConnector;
}

void JKQTPGraphValueLabelStyleMixin::setLabelOffsetWithConnector(double r)
{
    m_labelOffsetWithConnector=r;
}

double JKQTPGraphValueLabelStyleMixin::getLabelOffsetWithConnector() const
{
    return m_labelOffsetWithConnector;
}
void JKQTPGraphValueLabelStyleMixin::setLabelBoxRounding(double r)
{
    m_labelBoxRounding=r;
}

double JKQTPGraphValueLabelStyleMixin::getLabelBoxRounding() const
{
    return m_labelBoxRounding;
}

void JKQTPGraphValueLabelStyleMixin::setLabelXPadding(double r)
{
    m_labelXPadding=r;
}

double JKQTPGraphValueLabelStyleMixin::getLabelXPadding() const
{
    return m_labelXPadding;
}

void JKQTPGraphValueLabelStyleMixin::setLabelYPadding(double r)
{
    m_labelYPadding=r;
}

double JKQTPGraphValueLabelStyleMixin::getLabelYPadding() const
{
    return m_labelYPadding;
}

void JKQTPGraphValueLabelStyleMixin::setLabelPosition(JKQTPGraphLabelPosition r)
{
    m_labelPosition=r;
}

JKQTPGraphLabelPosition JKQTPGraphValueLabelStyleMixin::getLabelPosition() const
{
    return m_labelPosition;
}

bool JKQTPGraphValueLabelStyleMixin::isLabelPositioningGrowingInX() const
{
    switch (m_labelPosition) {
    case JKQTPGLabelAwayFromYAxis:
    case JKQTPGLabelLeftHandSide:
    case JKQTPGLabelRightHandSide:
        return true;
    case JKQTPGLabelAboveData:
    case JKQTPGLabelBelowData:
    case JKQTPGLabelAwayFromXAxis:
    case JKQTPGLabelTowardsXAxis:
    case JKQTPGLabelTowardsYAxis:
    case JKQTPGLabelCenteredOnData:
    case JKQTPGLabelHalfwaysToXAxis:
    case JKQTPGLabelHalfwaysToYAxis:
        return false;
    }
    return false;
}

bool JKQTPGraphValueLabelStyleMixin::isLabelPositioningGrowingInY() const
{
    switch (m_labelPosition) {
    case JKQTPGLabelAwayFromXAxis:
    case JKQTPGLabelAboveData:
    case JKQTPGLabelBelowData:
        return true;
    case JKQTPGLabelAwayFromYAxis:
    case JKQTPGLabelTowardsXAxis:
    case JKQTPGLabelTowardsYAxis:
    case JKQTPGLabelLeftHandSide:
    case JKQTPGLabelRightHandSide:
    case JKQTPGLabelCenteredOnData:
    case JKQTPGLabelHalfwaysToXAxis:
    case JKQTPGLabelHalfwaysToYAxis:
        return false;
    }
    return false;
}

void JKQTPGraphValueLabelStyleMixin::setDrawLabelBoxFrame(bool r)
{
    m_drawLabelBoxFrame=r;
}

bool JKQTPGraphValueLabelStyleMixin::drawsLabelBoxFrame() const
{
    return m_drawLabelBoxFrame;
}

void JKQTPGraphValueLabelStyleMixin::setLabelBoxType(JKQTPGraphLabelBoxType r)
{
    m_labelBoxType=r;
}

JKQTPGraphLabelBoxType JKQTPGraphValueLabelStyleMixin::getLabelBoxType() const
{
    return m_labelBoxType;
}

void JKQTPGraphValueLabelStyleMixin::drawLabel(JKQTPEnhancedPainter &painter, const QPointF &xDataPixel, const QPointF &xData, const QString &contents, JKQTBasePlotter *parent, double baselineX, double baselineY) const
{
    painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

    // calculate geometry
    const LabelGeometry g=calcLabelGeometry(painter, xDataPixel, xData, contents, parent, baselineX, baselineY);

    // draw Box
    if (m_drawLabelBoxFrame) painter.setPen(getLinePenForRects(painter, parent));
    else painter.setPen(Qt::NoPen);
    painter.setBrush(getFillBrush(painter, parent));
    if (m_labelBoxType==JKQTPGLSimpleBox || m_labelBoxType==JKQTPGLSimpleBoxAndLine) {
        painter.drawRoundedRectOrRect(g.boxRect, m_labelBoxRounding);
    }
    if (m_labelBoxType==JKQTPGLSimpleBoxAndLine) {
        //qDebug()<<"linestyle="<<getLineStyle()<<", color="<<getLineColor()<<", width="<<getLineWidth();
        painter.setPen(getLinePen(painter, parent));
        switch(g.boxpos) {
        case LabelGeometry::BoxLeft:
            painter.drawLine(xDataPixel, QPointF(qMax(g.boxRect.left(),g.boxRect.right()), xDataPixel.y()));
            break;
        case LabelGeometry::BoxRight:
            painter.drawLine(xDataPixel, QPointF(qMin(g.boxRect.left(),g.boxRect.right()), xDataPixel.y()));
            break;
        case LabelGeometry::BoxTop:
            painter.drawLine(xDataPixel, QPointF(xDataPixel.x(), qMax(g.boxRect.top(),g.boxRect.bottom())));
            break;
        case LabelGeometry::BoxBottom:
            painter.drawLine(xDataPixel, QPointF(xDataPixel.x(), qMin(g.boxRect.top(),g.boxRect.bottom())));
            break;
        case LabelGeometry::BoxCentered:
            break;
        }
    }


    // draw text
    parent->getMathText()->setFontSpecial(getTextFontName());
    parent->getMathText()->setFontSize(getTextFontSize()*parent->getFontSizeMultiplier());
    parent->getMathText()->setFontColor(getTextColor());
    parent->getMathText()->parse(contents);
    parent->getMathText()->draw(painter, Qt::AlignVCenter|Qt::AlignHCenter, g.textRect, parent->getCurrentPlotterStyle().debugShowTextBoxes);
}

JKQTPGraphValueLabelStyleMixin::LabelGeometry JKQTPGraphValueLabelStyleMixin::calcLabelGeometry(JKQTPEnhancedPainter &painter, const QPointF &xDataPixel, const QPointF &xData, const QString &contents, JKQTBasePlotter *parent, double baselineX, double baselineY) const
{
    LabelGeometry res;
    res.label=contents;
    res.labelOffsetPx=parent->pt2px(getUsedLabelOffset(),painter);
    res.padX=parent->pt2px(m_labelXPadding,painter);
    res.padY=parent->pt2px(m_labelYPadding,painter);
    res.lw=parent->pt2px(getLineWidth(),painter);
    res.textSize=parent->getTextSizeDetail(getTextFontName(), getTextFontSize(), contents, painter);

    if(m_labelPosition==JKQTPGLabelAboveData || (m_labelPosition==JKQTPGLabelAwayFromXAxis && xData.y()>=0) || (m_labelPosition==JKQTPGLabelTowardsXAxis && xData.y()<0)) {
        // label above
        res.textRect=QRectF(xDataPixel.x()-res.textSize.width/2.0, xDataPixel.y()-res.labelOffsetPx-res.padY-res.lw/2.0-res.textSize.overallHeight, res.textSize.width, res.textSize.overallHeight);
        res.boxpos=LabelGeometry::BoxTop;
    } else if (m_labelPosition==JKQTPGLabelBelowData || (m_labelPosition==JKQTPGLabelAwayFromXAxis && xData.y()<0) || (m_labelPosition==JKQTPGLabelTowardsXAxis && xData.y()>=0)) {
        // label below
        res.textRect=QRectF(xDataPixel.x()-res.textSize.width/2.0, xDataPixel.y()+res.labelOffsetPx+res.padY+res.lw/2.0, res.textSize.width, res.textSize.overallHeight);
        res.boxpos=LabelGeometry::BoxBottom;
    } else if(m_labelPosition==JKQTPGLabelLeftHandSide || (m_labelPosition==JKQTPGLabelAwayFromYAxis && xData.x()<0) || (m_labelPosition==JKQTPGLabelTowardsYAxis && xData.x()>=0)) {
        // lhs
        res.textRect=QRectF(xDataPixel.x()-res.labelOffsetPx-res.padX-res.lw/2.0-res.textSize.width, xDataPixel.y()-res.textSize.overallHeight/2.0, res.textSize.width, res.textSize.overallHeight);
        res.boxpos=LabelGeometry::BoxLeft;
    } else if (m_labelPosition==JKQTPGLabelRightHandSide || (m_labelPosition==JKQTPGLabelAwayFromYAxis && xData.x()>=0) || (m_labelPosition==JKQTPGLabelTowardsYAxis && xData.x()<0)) {
        // rhs
        res.textRect=QRectF(xDataPixel.x()+res.labelOffsetPx+res.padX+res.lw/2.0, xDataPixel.y()-res.textSize.overallHeight/2.0, res.textSize.width, res.textSize.overallHeight);
        res.boxpos=LabelGeometry::BoxRight;
    } else if (m_labelPosition==JKQTPGLabelCenteredOnData) {
        res.textRect=QRectF(xDataPixel.x()-res.textSize.width/2.0, xDataPixel.y()-res.textSize.overallHeight/2.0, res.textSize.width, res.textSize.overallHeight);
        res.boxpos=LabelGeometry::BoxCentered;
    } else if (m_labelPosition==JKQTPGLabelHalfwaysToXAxis) {
        res.textRect=QRectF(xDataPixel.x()-res.textSize.width/2.0, (xDataPixel.y()+parent->getYAxis()->x2p(baselineY))/2.0-res.textSize.overallHeight/2.0, res.textSize.width, res.textSize.overallHeight);
        res.boxpos=LabelGeometry::BoxCentered;
    } else if (m_labelPosition==JKQTPGLabelHalfwaysToYAxis) {
        res.textRect=QRectF((xDataPixel.x()+parent->getXAxis()->x2p(baselineX))/2.0-res.textSize.width/2.0, xDataPixel.y()-res.textSize.overallHeight/2.0, res.textSize.width, res.textSize.overallHeight);
        res.boxpos=LabelGeometry::BoxCentered;
    }

    res.boxRect=QRectF(res.textRect.x()-res.padX-res.lw/2.0, res.textRect.y()-res.padY-res.lw/2.0, res.textRect.width()+res.padX*2.0+res.lw, res.textRect.height()+res.padY*2.0+res.lw);

    return res;
}

JKQTPXYLabelsGeneratorMixin::JKQTPXYLabelsGeneratorMixin():
    m_xValueLabelFormat("$%1$"),
    m_yValueLabelFormat("$%1$"),
    m_xyValueLabelFormat("$%1 / %2$"),
    m_xDefaultConverter(JKQTPGLDefaultConverter),
    m_yDefaultConverter(JKQTPGLDefaultConverter),
    m_xDefaultPrecision(4),
    m_yDefaultPrecision(4),
    m_xBelowIsZero(std::numeric_limits<double>::min()*4),
    m_yBelowIsZero(std::numeric_limits<double>::min()*4),
    m_xMinNoExponent(1e-3),
    m_yMinNoExponent(1e-3),
    m_xMaxNoExponent(1e4),
    m_yMaxNoExponent(1e4),
    m_xDateTimeFormat(QLocale().dateTimeFormat(QLocale::NarrowFormat)),
    m_yDateTimeFormat(QLocale().dateTimeFormat(QLocale::NarrowFormat))
{

}

JKQTPXYLabelsGeneratorMixin::~JKQTPXYLabelsGeneratorMixin()
{

}

int JKQTPXYLabelsGeneratorMixin::getXDefaultPrecision() const
{
    return m_xDefaultPrecision;
}

void JKQTPXYLabelsGeneratorMixin::setXDefaultPrecision(int v)
{
    m_xDefaultPrecision=v;
}

int JKQTPXYLabelsGeneratorMixin::getYDefaultPrecision() const
{
    return m_yDefaultPrecision;
}

void JKQTPXYLabelsGeneratorMixin::setYDefaultPrecision(int v)
{
    m_yDefaultPrecision=v;
}

JKQTPGraphLabelConverterType JKQTPXYLabelsGeneratorMixin::getXDefaultConverter() const
{
    return m_xDefaultConverter;
}

void JKQTPXYLabelsGeneratorMixin::setXDefaultConverter(JKQTPGraphLabelConverterType v)
{
    m_xDefaultConverter=v;
}

JKQTPGraphLabelConverterType JKQTPXYLabelsGeneratorMixin::getYDefaultConverter() const
{
    return m_yDefaultConverter;
}

void JKQTPXYLabelsGeneratorMixin::setYDefaultConverter(JKQTPGraphLabelConverterType v)
{
    m_yDefaultConverter=v;
}

QString JKQTPXYLabelsGeneratorMixin::getXValueLabelFormat() const
{
    return m_xValueLabelFormat;
}

void JKQTPXYLabelsGeneratorMixin::setXValueLabelFormat(const QString &v)
{
    m_xValueLabelFormat=v;
}

QString JKQTPXYLabelsGeneratorMixin::getYValueLabelFormat() const
{
    return m_yValueLabelFormat;
}

void JKQTPXYLabelsGeneratorMixin::setYValueLabelFormat(const QString &v)
{
    m_yValueLabelFormat=v;
}

QString JKQTPXYLabelsGeneratorMixin::getXYValueLabelFormat() const
{
    return m_xyValueLabelFormat;
}

void JKQTPXYLabelsGeneratorMixin::setXYValueLabelFormat(const QString &v)
{
    m_xyValueLabelFormat=v;
}

double JKQTPXYLabelsGeneratorMixin::getXBelowIsZero() const
{
    return m_xBelowIsZero;
}

void JKQTPXYLabelsGeneratorMixin::setXBelowIsZero(double v)
{
    m_xBelowIsZero=v;
}

double JKQTPXYLabelsGeneratorMixin::getYBelowIsZero() const
{
    return m_yBelowIsZero;
}

void JKQTPXYLabelsGeneratorMixin::setYBelowIsZero(double v)
{
    m_yBelowIsZero=v;
}

double JKQTPXYLabelsGeneratorMixin::getXMinNoExponent() const
{
    return m_xMinNoExponent;
}

void JKQTPXYLabelsGeneratorMixin::setXMinNoExponent(double v)
{
    m_xMinNoExponent=v;
}

double JKQTPXYLabelsGeneratorMixin::getYMinNoExponent() const
{
    return m_yMinNoExponent;
}

void JKQTPXYLabelsGeneratorMixin::setYMinNoExponent(double v)
{
    m_yMinNoExponent=v;
}

double JKQTPXYLabelsGeneratorMixin::getXMaxNoExponent() const
{
    return m_xMaxNoExponent;
}

void JKQTPXYLabelsGeneratorMixin::setXMaxNoExponent(double v)
{
    m_xMaxNoExponent=v;
}

double JKQTPXYLabelsGeneratorMixin::getYMaxNoExponent() const
{
    return m_yMaxNoExponent;
}

void JKQTPXYLabelsGeneratorMixin::setYMaxNoExponent(double v)
{
    m_yMaxNoExponent=v;
}

QString JKQTPXYLabelsGeneratorMixin::getXDateTimeFormat() const
{
    return m_xDateTimeFormat;
}

void JKQTPXYLabelsGeneratorMixin::setXDateTimeFormat(const QString &v)
{
    m_xDateTimeFormat=v;
}

QString JKQTPXYLabelsGeneratorMixin::getYDateTimeFormat() const
{
    return m_yDateTimeFormat;
}

void JKQTPXYLabelsGeneratorMixin::setYDateTimeFormat(const QString &v)
{
    m_yDateTimeFormat=v;
}


QString JKQTPXYLabelsGeneratorMixin::generateDefaultXLabel(double x, double , int ) const
{
    return m_xValueLabelFormat.arg(xValToString(x));
}

QString JKQTPXYLabelsGeneratorMixin::generateDefaultYLabel(double , double y, int ) const
{
    return m_yValueLabelFormat.arg(yValToString(y));
}

QString JKQTPXYLabelsGeneratorMixin::generateDefaultXYLabel(double x, double y, int ) const
{
    return m_xyValueLabelFormat.arg(xValToString(x)).arg(yValToString(y));
}

QString JKQTPXYLabelsGeneratorMixin::valToString(double x, JKQTPGraphLabelConverterType converter, int precision, double belowIsZero, double minNoExponent, double maxNoExponent, const QString& dateTimeFormat)
{
    switch(converter) {
    case JKQTPGLCdecimal: return jkqtp_floattoqstr(x,'f',precision,true);
    case JKQTPGLCscientific: return jkqtp_floattoqstr(x,'e',precision,true);
    case JKQTPGLCexponentCharacter: return jkqtp_floattolatexunitqstr(x,precision,true, belowIsZero);
    case JKQTPGLCexponent: return jkqtp_floattolatexqstr(x,precision,true, belowIsZero,minNoExponent,maxNoExponent);
    case JKQTPGLCdatetime: {
        QDateTime dt;
        dt.setMSecsSinceEpoch(uint64_t(x));
        return dt.toString(dateTimeFormat);
    }
    }
    qWarning()<<"unknown m_xDefaultConverter="<<converter<<" in JKQTPXYLabelsGeneratorMixin::xValToString()";
    return "";
}

QString JKQTPXYLabelsGeneratorMixin::xValToString(double x) const
{
    return valToString(x, m_xDefaultConverter,m_xDefaultPrecision, m_xBelowIsZero, m_xMinNoExponent,m_xMaxNoExponent, m_xDateTimeFormat);
}

QString JKQTPXYLabelsGeneratorMixin::yValToString(double x) const
{
    return valToString(x, m_yDefaultConverter,m_yDefaultPrecision, m_yBelowIsZero, m_yMinNoExponent,m_yMaxNoExponent, m_yDateTimeFormat);
}


