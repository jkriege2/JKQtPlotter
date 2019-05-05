/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

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



#include "jkqtplottergui/jkqtpcomboboxes.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include <QImage>




JKQTPMathImageColorPaletteComboBox::JKQTPMathImageColorPaletteComboBox(QWidget *parent):
    QComboBox(parent)
{
    setIconSize(QSize(JKQTPImageTools::PALETTE_ICON_WIDTH,16));
    QStringList pal=JKQTPImagePlot_getPredefinedPalettes();
    clear();
    for (int i=0; i<pal.size(); i++) {
        addItem(JKQTPMathImageGetPaletteIcon(i), pal[i]);
    }
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(intIndexChanged(int)));
}

JKQTPMathImageColorPalette JKQTPMathImageColorPaletteComboBox::colorPalette() const
{
    return JKQTPMathImageColorPalette(currentIndex());
}

JKQTPMathImageColorPalette JKQTPMathImageColorPaletteComboBox::currentColorPalette() const
{
    return colorPalette();

}

JKQTPMathImageColorPalette JKQTPMathImageColorPaletteComboBox::getColorPalette() const
{
    return colorPalette();
}

void JKQTPMathImageColorPaletteComboBox::setColorPalette(JKQTPMathImageColorPalette palette)
{
    setCurrentIndex(int(palette));
}

void JKQTPMathImageColorPaletteComboBox::setCurrentColorPalette(JKQTPMathImageColorPalette palette)
{
    setCurrentIndex(int(palette));
}

void JKQTPMathImageColorPaletteComboBox::intIndexChanged(int i)
{
    emit currentPaletteChanged(JKQTPMathImageColorPalette(i));
}




JKQTPSymbolComboBox::JKQTPSymbolComboBox(QWidget *parent):
    QComboBox(parent)
{
    clear();
    setEditable(false);
    for (int i=0; i<=JKQTPMaxSymbolID; i++) {
        addSymbol(static_cast<JKQTPGraphSymbols>(i), JKQTPGraphSymbols2NameString(static_cast<JKQTPGraphSymbols>(i)));
    }
    setCurrentIndex(0);
}

JKQTPGraphSymbols JKQTPSymbolComboBox::getSymbol() const
{
    return String2JKQTPGraphSymbols(itemData(currentIndex()).toString());
}

void JKQTPSymbolComboBox::setSymbolType(JKQTPGraphSymbols symbol)
{
    int i=findData(JKQTPGraphSymbols2String(symbol));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPSymbolComboBox::setCurrentSymbol(JKQTPGraphSymbols symbol)
{
    setSymbolType(symbol);
}

void JKQTPSymbolComboBox::addSymbol(JKQTPGraphSymbols symbol, const QString &name)
{
    QPixmap pix(12,12);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPPlotSymbol(p, 6,6,symbol,10,1,QColor("blue"), QColor("blue").lighter());
    p.end();
    addItem(QIcon(pix), name, JKQTPGraphSymbols2String(symbol));
}






JKQTPLinePlotStyleComboBox::JKQTPLinePlotStyleComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    defaultSymbol=JKQTPCross;
    refill();
    setCurrentIndex(0);
}

void JKQTPLinePlotStyleComboBox::setDefaultSymbol(JKQTPGraphSymbols symbol)
{
    defaultSymbol=symbol;
    refill();
}

void JKQTPLinePlotStyleComboBox::addUsedSymbol(JKQTPGraphSymbols symbol)
{
    if (!symbols.contains(symbol)) symbols.append(symbol);
    refill();
}

JKQTPGraphSymbols JKQTPLinePlotStyleComboBox::getSymbol() const
{
    bool ok=true;
    int idx=itemData(currentIndex()).toInt(&ok);
    if (idx==-2) return JKQTPNoSymbol;
    if (idx>=0&&ok) return symbols.value(idx, defaultSymbol);
    return defaultSymbol;
}

bool JKQTPLinePlotStyleComboBox::getDrawLine() const
{
    return currentIndex()%2==0;
}

void JKQTPLinePlotStyleComboBox::refill()
{
    QString txt=currentText();
    setUpdatesEnabled(false);
    clear();
    addSymbol(JKQTPNoSymbol, true, tr("line"), -2);
    addSymbol(defaultSymbol, false, JKQTPGraphSymbols2NameString(defaultSymbol), -1);
    addSymbol(defaultSymbol, true, JKQTPGraphSymbols2NameString(defaultSymbol)+tr("+line"), -1);
    for (int i=0; i<symbols.size(); i++) {
        if (symbols[i]!=defaultSymbol) {
            addSymbol(symbols[i], false, JKQTPGraphSymbols2NameString(symbols[i]), i);
            addSymbol(symbols[i], true, JKQTPGraphSymbols2NameString(symbols[i])+tr("+line"), i);
        }
    }
    addSymbol(JKQTPNoSymbol, false, "none", -2);
    int idx=findText(txt);
    if (idx>=0) setCurrentIndex(idx);
    else setCurrentIndex(0);
    setUpdatesEnabled(true);
}

void JKQTPLinePlotStyleComboBox::addSymbol(JKQTPGraphSymbols symbol, bool line, const QString &name, const QVariant &data)
{
    QPixmap pix(12,12);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPPlotSymbol(p, 6,6,symbol,7,1,QColor("blue"), QColor("blue").lighter());
    p.setPen(QColor("blue"));
    if (line) p.drawLine(0,6,12,6);
    p.end();
    addItem(QIcon(pix), name, data);
}



JKQTPErrorPlotstyleComboBox::JKQTPErrorPlotstyleComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addSymbol(JKQTPNoError, tr("none"), QIcon(":/JKQTPlotter/jkqtp_enone.png"));
    addSymbol(JKQTPErrorLines, tr("error lines"), QIcon(":/JKQTPlotter/jkqtp_elines.png"));
    addSymbol(JKQTPErrorBars, tr("error bars"), QIcon(":/JKQTPlotter/jkqtp_ebars.png"));
    addSymbol(JKQTPErrorSimpleBars, tr("simple error bars"), QIcon(":/JKQTPlotter/jkqtp_esbars.png"));
    addSymbol(JKQTPErrorPolygons, tr("error polygons"), QIcon(":/JKQTPlotter/jkqtp_epoly.png"));
    addSymbol(JKQTPErrorBarsLines, tr("bars + lines"), QIcon(":/JKQTPlotter/jkqtp_elinesbars.png"));
    addSymbol(JKQTPErrorBarsPolygons, tr("bars + polygons"), QIcon(":/JKQTPlotter/jkqtp_epolybars.png"));
    addSymbol(JKQTPErrorSimpleBarsLines, tr("simple bars + lines"), QIcon(":/JKQTPlotter/jkqtp_elinessbars.png"));
    addSymbol(JKQTPErrorSimpleBarsPolygons, tr("simple bars + polygons"), QIcon(":/JKQTPlotter/jkqtp_epolysbars.png"));
    addSymbol(JKQTPErrorBoxes, tr("boxes"), QIcon(":/JKQTPlotter/jkqtp_eboxes.png"));
    addSymbol(JKQTPErrorEllipses, tr("ellipses"), QIcon(":/JKQTPlotter/jkqtp_eellipses.png"));
    setCurrentIndex(2);
}

JKQTPErrorPlotstyle JKQTPErrorPlotstyleComboBox::getErrorLineStyle() const
{
    return String2JKQTPErrorPlotstyle(itemData(currentIndex()).toString());
}

void JKQTPErrorPlotstyleComboBox::setSymbolType(JKQTPErrorPlotstyle symbol)
{
    int i=findData(JKQTPErrorPlotstyle2String(symbol));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPErrorPlotstyleComboBox::setCurrentErrorStyle(JKQTPErrorPlotstyle symbol)
{
    int i=findData(JKQTPErrorPlotstyle2String(symbol));
    if (i>=0) setCurrentIndex(i);

}

void JKQTPErrorPlotstyleComboBox::addSymbol(JKQTPErrorPlotstyle symbol, const QString &name, const QIcon& icon)
{
    addItem(icon, name, JKQTPErrorPlotstyle2String(symbol));
}





JKQTPKeyPositionComboBox::JKQTPKeyPositionComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addPosition(JKQTPKeyInsideTopRight, tr("inside, top-right"), QIcon(":/JKQTPlotter/jkqtp_key_itr.png"));
    addPosition(JKQTPKeyInsideTopLeft, tr("inside, top-left"), QIcon(":/JKQTPlotter/jkqtp_key_itl.png"));
    addPosition(JKQTPKeyInsideBottomLeft, tr("inside, bottom-left"), QIcon(":/JKQTPlotter/jkqtp_key_ibl.png"));
    addPosition(JKQTPKeyInsideBottomRight, tr("inside, bottom-right"), QIcon(":/JKQTPlotter/jkqtp_key_ibr.png"));
    addPosition(JKQTPKeyOutsideTopRight, tr("outside, top-right"), QIcon(":/JKQTPlotter/jkqtp_key_otr.png"));
    addPosition(JKQTPKeyOutsideTopLeft, tr("outside, top-left"), QIcon(":/JKQTPlotter/jkqtp_key_otl.png"));
    addPosition(JKQTPKeyOutsideLeftTop, tr("outside, left-top"), QIcon(":/JKQTPlotter/jkqtp_key_olt.png"));
    addPosition(JKQTPKeyOutsideLeftBottom, tr("outside, left-bottom"), QIcon(":/JKQTPlotter/jkqtp_key_olb.png"));
    addPosition(JKQTPKeyOutsideBottomRight, tr("outside, bottom-right"), QIcon(":/JKQTPlotter/jkqtp_key_obr.png"));
    addPosition(JKQTPKeyOutsideBottomLeft, tr("outside, bottom-left"), QIcon(":/JKQTPlotter/jkqtp_key_obl.png"));
    addPosition(JKQTPKeyOutsideRightTop, tr("outside, right-top"), QIcon(":/JKQTPlotter/jkqtp_key_ort.png"));
    addPosition(JKQTPKeyOutsideRightBottom, tr("outside, right-bottom"), QIcon(":/JKQTPlotter/jkqtp_key_orb.png"));
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(posChanged(int)));
}

JKQTPKeyPosition JKQTPKeyPositionComboBox::getPosition() const
{
    return String2JKQTPKeyPosition(itemData(currentIndex()).toString());
}

void JKQTPKeyPositionComboBox::setPosition(JKQTPKeyPosition position)
{
    int i=findData(JKQTPKeyPosition2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPKeyPositionComboBox::addPosition(JKQTPKeyPosition position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPKeyPosition2String(position));
}

void JKQTPKeyPositionComboBox::posChanged(int index)
{
    emit currentPositionChanged(String2JKQTPKeyPosition(itemData(index).toString()));
}

JKQTPKeyLayoutComboBox::JKQTPKeyLayoutComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addKeyLayout(JKQTPKeyLayoutOneColumn, tr("one column"));
    addKeyLayout(JKQTPKeyLayoutOneRow, tr("one row"));
    addKeyLayout(JKQTPKeyLayoutMultiColumn, tr("multiple columns"));
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedP(int)));
}

JKQTPKeyLayout JKQTPKeyLayoutComboBox::getKeyLayout() const
{
    return String2JKQTPKeyLayout(itemData(currentIndex()).toString());
}

void JKQTPKeyLayoutComboBox::setKeyLayout(JKQTPKeyLayout position)
{
    int i=findData(JKQTPKeyLayout2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPKeyLayoutComboBox::addKeyLayout(JKQTPKeyLayout position, const QString &name)
{
    addItem(name, JKQTPKeyLayout2String(position));
}

void JKQTPKeyLayoutComboBox::currentIndexChangedP(int index)
{
    emit currentLayoutChanged(String2JKQTPKeyLayout(itemData(index).toString()));
}

JKQTPLabelPositionComboBox::JKQTPLabelPositionComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addPosition(JKQTPLabelCenter, tr("center"));
    addPosition(JKQTPLabelMin, tr("left"));
    addPosition(JKQTPLabelMax, tr("right"));
    setCurrentIndex(0);
}

JKQTPLabelPosition JKQTPLabelPositionComboBox::getPosition() const
{
    return String2JKQTPLabelPosition(itemData(currentIndex()).toString());
}

void JKQTPLabelPositionComboBox::setPosition(JKQTPLabelPosition position)
{
    int i=findData(JKQTPLabelPosition2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPLabelPositionComboBox::addPosition(JKQTPLabelPosition position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPLabelPosition2String(position));
}

JKQTPCALabelTypeComboBox::JKQTPCALabelTypeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addLabelType(JKQTPCALTexponent, tr("exponent"), QIcon(":/JKQTPlotter/jkqtp_ticks_exp.png"));
    addLabelType(JKQTPCALTdefault, tr("default"), QIcon(":/JKQTPlotter/jkqtp_ticks_default.png"));
    addLabelType(JKQTPCALTexponentCharacter, tr("character"), QIcon(":/JKQTPlotter/jkqtp_ticks_expchar.png"));
    addLabelType(JKQTPCALTtime, tr("time"), QIcon(":/JKQTPlotter/jkqtp_ticks_time.png"));
    addLabelType(JKQTPCALTdate, tr("date"), QIcon(":/JKQTPlotter/jkqtp_ticks_date.png"));
    addLabelType(JKQTPCALTdatetime, tr("datetime"), QIcon(":/JKQTPlotter/jkqtp_ticks_datetime.png"));
    setCurrentIndex(0);
}

JKQTPCALabelType JKQTPCALabelTypeComboBox::getLabelType() const
{
    return String2JKQTPCALabelType(itemData(currentIndex()).toString());
}

void JKQTPCALabelTypeComboBox::setLabelType(JKQTPCALabelType position)
{
    int i=findData(JKQTPCALabelType2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPCALabelTypeComboBox::addLabelType(JKQTPCALabelType position, const QString &name, const QIcon& icon)
{
    addItem(icon, name, JKQTPCALabelType2String(position));
}

JKQTPCADrawModeComboBox::JKQTPCADrawModeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addDrawMode(JKQTPCADMcomplete, tr("complete"), QIcon(":/JKQTPlotter/jkqtp_axis_complete.png"));
    addDrawMode(JKQTPCADMLineTicksTickLabels, tr("line+tick+labels"), QIcon(":/JKQTPlotter/jkqtp_axis_ticksandlabels.png"));
    addDrawMode(JKQTPCADMLineTicks, tr("line+tick"), QIcon(":/JKQTPlotter/jkqtp_axis_ticks.png"));
    addDrawMode(JKQTPCADMLine, tr("line"), QIcon(":/JKQTPlotter/jkqtp_axis_line.png"));
    addDrawMode(JKQTPCADMLineTicksTickLabels, tr("line+tick+labels"), QIcon(":/JKQTPlotter/jkqtp_axis_ticksandlabels.png"));
    addDrawMode(JKQTPCADMLineTicks, tr("line+tick"), QIcon(":/JKQTPlotter/jkqtp_axis_ticks.png"));
    addDrawMode(JKQTPCADMTicksTickLabelsAxisLabel, tr("ticks+labels+axis label"));
    addDrawMode(JKQTPCADMTicksTickLabels, tr("ticks+labels"));
    addDrawMode(JKQTPCADMTicks, tr("ticks"));
    addDrawMode(JKQTPCADMTickLabelsAxisLabel, tr("labels+axis label"));
    addDrawMode(JKQTPCADMTickLabels, tr("labels"));
    addDrawMode(JKQTPCADMnone, tr("none"), QIcon(":/JKQTPlotter/jkqtp_axis_none.png"));
    setCurrentIndex(0);
}


JKQTPCADrawMode JKQTPCADrawModeComboBox::getDrawMode() const
{
    return String2JKQTPCADrawMode(itemData(currentIndex()).toString());
}

void JKQTPCADrawModeComboBox::setDrawMode(JKQTPCADrawMode position)
{
    int i=findData(JKQTPCADrawMode2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPCADrawModeComboBox::addDrawMode(JKQTPCADrawMode position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPCADrawMode2String(position));
}




JKQTPLinePlotStyleWithSymbolSizeComboBox::JKQTPLinePlotStyleWithSymbolSizeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    defaultSymbol=JKQTPCross;
    defaultSize=10;
    refill();
    setCurrentIndex(0);
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::setDefaultSymbol(JKQTPGraphSymbols symbol, double size)
{
    defaultSymbol=symbol;
    defaultSize=size;
    refill();
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::addUsedSymbol(JKQTPGraphSymbols symbol, double symbolSize, bool line)
{
    styleData d;
    d.symbol=symbol;
    d.symbolSize=symbolSize;
    d.line=line;
    if (!symbols.contains(d)) symbols.append(d);
    refill();
}

JKQTPGraphSymbols JKQTPLinePlotStyleWithSymbolSizeComboBox::getSymbol() const
{
    QVariant idxV=itemData(currentIndex());
    bool ok=false;
    int idx=idxV.toInt(&ok);
    if (idxV.isValid() && ok) {
        if (idx>=0 && idx<symbols.size()) return symbols[idx].symbol;
        if (idx==-4) return JKQTPNoSymbol;
        if (idx==-3) return JKQTPNoSymbol;
        return defaultSymbol;
    }
    return JKQTPNoSymbol;
}

bool JKQTPLinePlotStyleWithSymbolSizeComboBox::getDrawLine() const
{
    QVariant idxV=itemData(currentIndex());
    bool ok=false;
    int idx=idxV.toInt(&ok);
    if (idxV.isValid() && ok) {
        if (idx==-3) return true;
        if (idx==-1) return true;
        if (idx>=0 && idx<symbols.size()) return symbols[idx].line;
        return false;
    }
    return false;
}

double JKQTPLinePlotStyleWithSymbolSizeComboBox::getSymbolSize() const
{
    QVariant idxV=itemData(currentIndex());
    bool ok=false;
    int idx=idxV.toInt(&ok);
    if (idxV.isValid() && ok) {
        if (idx>=0 && idx<symbols.size()) return symbols[idx].symbolSize;
        return defaultSize;
    }
    return defaultSize;
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::refill()
{
    QString txt=currentText();
    setUpdatesEnabled(false);
    clear();
    addSymbol(JKQTPNoSymbol, true, defaultSize, tr("line"), -3);
    addSymbol(defaultSymbol, false, defaultSize, JKQTPGraphSymbols2NameString(defaultSymbol), -2);
    addSymbol(defaultSymbol, true, defaultSize, JKQTPGraphSymbols2NameString(defaultSymbol)+tr("+line"), -1);
    for (int i=0; i<symbols.size(); i++) {
        if (symbols[i].symbol!=defaultSymbol||symbols[i].symbolSize!=defaultSize) {
            addSymbol(symbols[i].symbol, symbols[i].line, symbols[i].symbolSize, JKQTPGraphSymbols2NameString(symbols[i].symbol), i);
        }
    }
    addSymbol(JKQTPNoSymbol, false, defaultSize, "none", -4);
    int idx=findText(txt);
    if (idx>=0) setCurrentIndex(idx);
    else setCurrentIndex(0);
    setUpdatesEnabled(true);
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::addSymbol(JKQTPGraphSymbols symbol, bool line, double symbolSize, const QString &name, const QVariant &data)
{
    int pixSize=qMax(16.0, 1.2*symbolSize);
    QPixmap pix(pixSize, pixSize);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPPlotSymbol(p, double(pixSize)/2.0,double(pixSize)/2.0,symbol,symbolSize,1,QColor("blue"), QColor("blue").lighter());
    p.setPen(QColor("blue"));
    if (line) p.drawLine(QLineF(0,double(pixSize)/2.0,pixSize,double(pixSize)/2.0));
    p.end();
    addItem(QIcon(pix), name, data);
}


bool JKQTPLinePlotStyleWithSymbolSizeComboBox::styleData::operator==(const JKQTPLinePlotStyleWithSymbolSizeComboBox::styleData &other) const
{
    return (symbol==other.symbol && symbolSize==other.symbolSize && line==other.line);
}


JKQTPSpecialLineTypeComboBox::JKQTPSpecialLineTypeComboBox(QWidget *parent):
    QComboBox(parent)
{
    clear();
    setEditable(false);
    addStep(JKQTPSpecialLineType::JKQTPStepLeft, JKQTPSpecialLineType2String(JKQTPSpecialLineType::JKQTPStepLeft));
    addStep(JKQTPSpecialLineType::JKQTPStepCenter, JKQTPSpecialLineType2String(JKQTPSpecialLineType::JKQTPStepCenter));
    addStep(JKQTPSpecialLineType::JKQTPStepRight, JKQTPSpecialLineType2String(JKQTPSpecialLineType::JKQTPStepRight));
    addStep(JKQTPSpecialLineType::JKQTPStepAverage, JKQTPSpecialLineType2String(JKQTPSpecialLineType::JKQTPStepAverage));
    addStep(JKQTPSpecialLineType::JKQTPDirectLine, JKQTPSpecialLineType2String(JKQTPSpecialLineType::JKQTPDirectLine));
    setCurrentIndex(0);
}

JKQTPSpecialLineType JKQTPSpecialLineTypeComboBox::getSpecialLineType() const
{
    return String2JKQTPSpecialLineType(itemData(currentIndex()).toString());
}

void JKQTPSpecialLineTypeComboBox::setSpecialLineType(JKQTPSpecialLineType step)
{
    int i=findData(JKQTPSpecialLineType2String(step));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPSpecialLineTypeComboBox::setCurrentStepType(JKQTPSpecialLineType step)
{
    setSpecialLineType(step);
}

void JKQTPSpecialLineTypeComboBox::addStep(JKQTPSpecialLineType step, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPSpecialLineType2String(step));
}


JKQTPImageModifierModeComboBox::JKQTPImageModifierModeComboBox(QWidget * /*parent*/)
{
    addMode(JKQTPMathImageBase::ModifyNone, tr("none"));
    addMode(JKQTPMathImageBase::ModifyValue, tr("value"));
    addMode(JKQTPMathImageBase::ModifySaturation, tr("saturation"));
    addMode(JKQTPMathImageBase::ModifyAlpha, tr("alpha/transparency"));
    addMode(JKQTPMathImageBase::ModifyLuminance, tr("luminance"));
    addMode(JKQTPMathImageBase::ModifyHue, tr("hue"));
}

JKQTPMathImageBase::ModifierMode JKQTPImageModifierModeComboBox::getModifierMode() const
{
    bool ok=false;
    JKQTPMathImageBase::ModifierMode m=JKQTPMathImageBase::ModifyNone;
    if (currentIndex()>=0) m=JKQTPMathImageBase::ModifierMode(itemData(currentIndex()).toInt(&ok));
    if (!ok) m=JKQTPMathImageBase::ModifyNone;
    return m;
}

void JKQTPImageModifierModeComboBox::setModifierMode(JKQTPMathImageBase::ModifierMode mode)
{
    setCurrentIndex(findData(static_cast<int>(mode)));
}

void JKQTPImageModifierModeComboBox::addMode(JKQTPMathImageBase::ModifierMode mode, const QString &name, const QIcon &icon)
{
    addItem(icon, name, static_cast<int>(mode));
}
