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



/** \file jkqtpcomboboxes.cpp
  * \ingroup jkqtplotter
  */

#include "jkqtplottergui/jkqtpcomboboxes.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include <QImage>




JKQTPMathImageColorPaletteComboBox::JKQTPMathImageColorPaletteComboBox(QWidget *parent):
    QComboBox(parent)
{
    setIconSize(QSize(JKQTP_PALETTE_ICON_WIDTH,16));
    QStringList pal=JKQTPimagePlot_getPredefinedPalettes();
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
    for (int i=0; i<=JKQTPmaxSymbolID; i++) {
        addSymbol(static_cast<JKQTPgraphSymbols>(i), JKQTPgraphSymbols2NameString(static_cast<JKQTPgraphSymbols>(i)));
    }
    setCurrentIndex(0);
}

JKQTPgraphSymbols JKQTPSymbolComboBox::getSymbol() const
{
    return String2JKQTPgraphSymbols(itemData(currentIndex()).toString());
}

void JKQTPSymbolComboBox::setSymbol(JKQTPgraphSymbols symbol)
{
    int i=findData(JKQTPgraphSymbols2String(symbol));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPSymbolComboBox::setCurrentSymbol(JKQTPgraphSymbols symbol)
{
    setSymbol(symbol);
}

void JKQTPSymbolComboBox::addSymbol(JKQTPgraphSymbols symbol, const QString &name)
{
    QPixmap pix(12,12);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPplotSymbol(p, 6,6,symbol,10,1,QColor("blue"), QColor("blue").lighter());
    p.end();
    addItem(QIcon(pix), name, JKQTPgraphSymbols2String(symbol));
}






JKQTPLinePlotStyleComboBox::JKQTPLinePlotStyleComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    defaultSymbol=JKQTPcross;
    refill();
    setCurrentIndex(0);
}

void JKQTPLinePlotStyleComboBox::setDefaultSymbol(JKQTPgraphSymbols symbol)
{
    defaultSymbol=symbol;
    refill();
}

void JKQTPLinePlotStyleComboBox::addUsedSymbol(JKQTPgraphSymbols symbol)
{
    if (!symbols.contains(symbol)) symbols.append(symbol);
    refill();
}

JKQTPgraphSymbols JKQTPLinePlotStyleComboBox::getSymbol() const
{
    bool ok=true;
    int idx=itemData(currentIndex()).toInt(&ok);
    if (idx==-2) return JKQTPnoSymbol;
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
    addSymbol(JKQTPnoSymbol, true, tr("line"), -2);
    addSymbol(defaultSymbol, false, JKQTPgraphSymbols2NameString(defaultSymbol), -1);
    addSymbol(defaultSymbol, true, JKQTPgraphSymbols2NameString(defaultSymbol)+tr("+line"), -1);
    for (int i=0; i<symbols.size(); i++) {
        if (symbols[i]!=defaultSymbol) {
            addSymbol(symbols[i], false, JKQTPgraphSymbols2NameString(symbols[i]), i);
            addSymbol(symbols[i], true, JKQTPgraphSymbols2NameString(symbols[i])+tr("+line"), i);
        }
    }
    addSymbol(JKQTPnoSymbol, false, "none", -2);
    int idx=findText(txt);
    if (idx>=0) setCurrentIndex(idx);
    else setCurrentIndex(0);
    setUpdatesEnabled(true);
}

void JKQTPLinePlotStyleComboBox::addSymbol(JKQTPgraphSymbols symbol, bool line, const QString &name, const QVariant &data)
{
    QPixmap pix(12,12);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPplotSymbol(p, 6,6,symbol,7,1,QColor("blue"), QColor("blue").lighter());
    p.setPen(QColor("blue"));
    if (line) p.drawLine(0,6,12,6);
    p.end();
    addItem(QIcon(pix), name, data);
}



JKQTPerrorPlotstyleComboBox::JKQTPerrorPlotstyleComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addSymbol(JKQTPnoError, tr("none"), QIcon(":/JKQTPlotter/jkqtp_enone.png"));
    addSymbol(JKQTPerrorLines, tr("error lines"), QIcon(":/JKQTPlotter/jkqtp_elines.png"));
    addSymbol(JKQTPerrorBars, tr("error bars"), QIcon(":/JKQTPlotter/jkqtp_ebars.png"));
    addSymbol(JKQTPerrorSimpleBars, tr("simple error bars"), QIcon(":/JKQTPlotter/jkqtp_esbars.png"));
    addSymbol(JKQTPerrorPolygons, tr("error polygons"), QIcon(":/JKQTPlotter/jkqtp_epoly.png"));
    addSymbol(JKQTPerrorBarsLines, tr("bars + lines"), QIcon(":/JKQTPlotter/jkqtp_elinesbars.png"));
    addSymbol(JKQTPerrorBarsPolygons, tr("bars + polygons"), QIcon(":/JKQTPlotter/jkqtp_epolybars.png"));
    addSymbol(JKQTPerrorSimpleBarsLines, tr("simple bars + lines"), QIcon(":/JKQTPlotter/jkqtp_elinessbars.png"));
    addSymbol(JKQTPerrorSimpleBarsPolygons, tr("simple bars + polygons"), QIcon(":/JKQTPlotter/jkqtp_epolysbars.png"));
    addSymbol(JKQTPerrorBoxes, tr("boxes"), QIcon(":/JKQTPlotter/jkqtp_eboxes.png"));
    addSymbol(JKQTPerrorEllipses, tr("ellipses"), QIcon(":/JKQTPlotter/jkqtp_eellipses.png"));
    setCurrentIndex(2);
}

JKQTPerrorPlotstyle JKQTPerrorPlotstyleComboBox::getErrorStyle() const
{
    return String2JKQTPerrorPlotstyle(itemData(currentIndex()).toString());
}

void JKQTPerrorPlotstyleComboBox::setSymbol(JKQTPerrorPlotstyle symbol)
{
    int i=findData(JKQTPerrorPlotstyle2String(symbol));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPerrorPlotstyleComboBox::setCurrentErrorStyle(JKQTPerrorPlotstyle symbol)
{
    int i=findData(JKQTPerrorPlotstyle2String(symbol));
    if (i>=0) setCurrentIndex(i);

}

void JKQTPerrorPlotstyleComboBox::addSymbol(JKQTPerrorPlotstyle symbol, const QString &name, const QIcon& icon)
{
    addItem(icon, name, JKQTPerrorPlotstyle2String(symbol));
}





JKQTPkeyPositionComboBox::JKQTPkeyPositionComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addPosition(JKQTPkeyInsideTopRight, tr("inside, top-right"), QIcon(":/JKQTPlotter/jkqtp_key_itr.png"));
    addPosition(JKQTPkeyInsideTopLeft, tr("inside, top-left"), QIcon(":/JKQTPlotter/jkqtp_key_itl.png"));
    addPosition(JKQTPkeyInsideBottomLeft, tr("inside, bottom-left"), QIcon(":/JKQTPlotter/jkqtp_key_ibl.png"));
    addPosition(JKQTPkeyInsideBottomRight, tr("inside, bottom-right"), QIcon(":/JKQTPlotter/jkqtp_key_ibr.png"));
    addPosition(JKQTPkeyOutsideTopRight, tr("outside, top-right"), QIcon(":/JKQTPlotter/jkqtp_key_otr.png"));
    addPosition(JKQTPkeyOutsideTopLeft, tr("outside, top-left"), QIcon(":/JKQTPlotter/jkqtp_key_otl.png"));
    addPosition(JKQTPkeyOutsideLeftTop, tr("outside, left-top"), QIcon(":/JKQTPlotter/jkqtp_key_olt.png"));
    addPosition(JKQTPkeyOutsideLeftBottom, tr("outside, left-bottom"), QIcon(":/JKQTPlotter/jkqtp_key_olb.png"));
    addPosition(JKQTPkeyOutsideBottomRight, tr("outside, bottom-right"), QIcon(":/JKQTPlotter/jkqtp_key_obr.png"));
    addPosition(JKQTPkeyOutsideBottomLeft, tr("outside, bottom-left"), QIcon(":/JKQTPlotter/jkqtp_key_obl.png"));
    addPosition(JKQTPkeyOutsideRightTop, tr("outside, right-top"), QIcon(":/JKQTPlotter/jkqtp_key_ort.png"));
    addPosition(JKQTPkeyOutsideRightBottom, tr("outside, right-bottom"), QIcon(":/JKQTPlotter/jkqtp_key_orb.png"));
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(posChanged(int)));
}

JKQTPkeyPosition JKQTPkeyPositionComboBox::getPosition() const
{
    return String2JKQTPkeyPosition(itemData(currentIndex()).toString());
}

void JKQTPkeyPositionComboBox::setPosition(JKQTPkeyPosition position)
{
    int i=findData(JKQTPkeyPosition2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPkeyPositionComboBox::addPosition(JKQTPkeyPosition position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPkeyPosition2String(position));
}

void JKQTPkeyPositionComboBox::posChanged(int index)
{
    emit currentPositionChanged(String2JKQTPkeyPosition(itemData(index).toString()));
}

JKQTPkeyLayoutComboBox::JKQTPkeyLayoutComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addKeyLayout(JKQTPkeyLayoutOneColumn, tr("one column"));
    addKeyLayout(JKQTPkeyLayoutOneRow, tr("one row"));
    addKeyLayout(JKQTPkeyLayoutMultiColumn, tr("multiple columns"));
    setCurrentIndex(0);
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedP(int)));
}

JKQTPkeyLayout JKQTPkeyLayoutComboBox::getKeyLayout() const
{
    return String2JKQTPkeyLayout(itemData(currentIndex()).toString());
}

void JKQTPkeyLayoutComboBox::setKeyLayout(JKQTPkeyLayout position)
{
    int i=findData(JKQTPkeyLayout2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPkeyLayoutComboBox::addKeyLayout(JKQTPkeyLayout position, const QString &name)
{
    addItem(name, JKQTPkeyLayout2String(position));
}

void JKQTPkeyLayoutComboBox::currentIndexChangedP(int index)
{
    emit currentLayoutChanged(String2JKQTPkeyLayout(itemData(index).toString()));
}

JKQTPlabelPositionComboBox::JKQTPlabelPositionComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addPosition(JKQTPlabelCenter, tr("center"));
    addPosition(JKQTPlabelMin, tr("left"));
    addPosition(JKQTPlabelMax, tr("right"));
    setCurrentIndex(0);
}

JKQTPlabelPosition JKQTPlabelPositionComboBox::getPosition() const
{
    return String2JKQTPlabelPosition(itemData(currentIndex()).toString());
}

void JKQTPlabelPositionComboBox::setPosition(JKQTPlabelPosition position)
{
    int i=findData(JKQTPlabelPosition2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPlabelPositionComboBox::addPosition(JKQTPlabelPosition position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPlabelPosition2String(position));
}

JKQTPCAlabelTypeComboBox::JKQTPCAlabelTypeComboBox(QWidget *parent):
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

JKQTPCAlabelType JKQTPCAlabelTypeComboBox::getLabelType() const
{
    return String2JKQTPCAlabelType(itemData(currentIndex()).toString());
}

void JKQTPCAlabelTypeComboBox::setLabelType(JKQTPCAlabelType position)
{
    int i=findData(JKQTPCAlabelType2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPCAlabelTypeComboBox::addLabelType(JKQTPCAlabelType position, const QString &name, const QIcon& icon)
{
    addItem(icon, name, JKQTPCAlabelType2String(position));
}

JKQTPCAdrawModeComboBox::JKQTPCAdrawModeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    addDrawMode(JKQTPCADMcomplete, tr("complete"), QIcon(":/JKQTPlotter/jkqtp_axis_complete.png"));
    addDrawMode(JKQTPCADMticksAndLabels, tr("tick+lab."), QIcon(":/JKQTPlotter/jkqtp_axis_ticksandlabels.png"));
    addDrawMode(JKQTPCADMticks, tr("tick"), QIcon(":/JKQTPlotter/jkqtp_axis_ticks.png"));
    addDrawMode(JKQTPCADMline, tr("line"), QIcon(":/JKQTPlotter/jkqtp_axis_line.png"));
    addDrawMode(JKQTPCADMnone, tr("none"), QIcon(":/JKQTPlotter/jkqtp_axis_none.png"));
    setCurrentIndex(0);
}

JKQTPCAdrawMode JKQTPCAdrawModeComboBox::getDrawMode() const
{
    return String2JKQTPCAdrawMode(itemData(currentIndex()).toString());
}

void JKQTPCAdrawModeComboBox::setDrawMode(JKQTPCAdrawMode position)
{
    int i=findData(JKQTPCAdrawMode2String(position));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPCAdrawModeComboBox::addDrawMode(JKQTPCAdrawMode position, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPCAdrawMode2String(position));
}




JKQTPLinePlotStyleWithSymbolSizeComboBox::JKQTPLinePlotStyleWithSymbolSizeComboBox(QWidget *parent):
    QComboBox(parent)
{
    setEditable(false);
    defaultSymbol=JKQTPcross;
    defaultSize=10;
    refill();
    setCurrentIndex(0);
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::setDefaultSymbol(JKQTPgraphSymbols symbol, double size)
{
    defaultSymbol=symbol;
    defaultSize=size;
    refill();
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::addUsedSymbol(JKQTPgraphSymbols symbol, double symbolSize, bool line)
{
    styleData d;
    d.symbol=symbol;
    d.symbolSize=symbolSize;
    d.line=line;
    if (!symbols.contains(d)) symbols.append(d);
    refill();
}

JKQTPgraphSymbols JKQTPLinePlotStyleWithSymbolSizeComboBox::getSymbol() const
{
    QVariant idxV=itemData(currentIndex());
    bool ok=false;
    int idx=idxV.toInt(&ok);
    if (idxV.isValid() && ok) {
        if (idx>=0 && idx<symbols.size()) return symbols[idx].symbol;
        if (idx==-4) return JKQTPnoSymbol;
        if (idx==-3) return JKQTPnoSymbol;
        return defaultSymbol;
    }
    return JKQTPnoSymbol;
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
    addSymbol(JKQTPnoSymbol, true, defaultSize, tr("line"), -3);
    addSymbol(defaultSymbol, false, defaultSize, JKQTPgraphSymbols2NameString(defaultSymbol), -2);
    addSymbol(defaultSymbol, true, defaultSize, JKQTPgraphSymbols2NameString(defaultSymbol)+tr("+line"), -1);
    for (int i=0; i<symbols.size(); i++) {
        if (symbols[i].symbol!=defaultSymbol||symbols[i].symbolSize!=defaultSize) {
            addSymbol(symbols[i].symbol, symbols[i].line, symbols[i].symbolSize, JKQTPgraphSymbols2NameString(symbols[i].symbol), i);
        }
    }
    addSymbol(JKQTPnoSymbol, false, defaultSize, "none", -4);
    int idx=findText(txt);
    if (idx>=0) setCurrentIndex(idx);
    else setCurrentIndex(0);
    setUpdatesEnabled(true);
}

void JKQTPLinePlotStyleWithSymbolSizeComboBox::addSymbol(JKQTPgraphSymbols symbol, bool line, double symbolSize, const QString &name, const QVariant &data)
{
    int pixSize=qMax(16.0, 1.2*symbolSize);
    QPixmap pix(pixSize, pixSize);
    pix.fill(Qt::transparent);
    JKQTPEnhancedPainter p;
    p.begin(&pix);
    p.setRenderHint(JKQTPEnhancedPainter::Antialiasing);
    p.setRenderHint(JKQTPEnhancedPainter::TextAntialiasing);
    JKQTPplotSymbol(p, double(pixSize)/2.0,double(pixSize)/2.0,symbol,symbolSize,1,QColor("blue"), QColor("blue").lighter());
    p.setPen(QColor("blue"));
    if (line) p.drawLine(QLineF(0,double(pixSize)/2.0,pixSize,double(pixSize)/2.0));
    p.end();
    addItem(QIcon(pix), name, data);
}


bool JKQTPLinePlotStyleWithSymbolSizeComboBox::styleData::operator==(const JKQTPLinePlotStyleWithSymbolSizeComboBox::styleData &other) const
{
    return (symbol==other.symbol && symbolSize==other.symbolSize && line==other.line);
}


JKQTPstepTypeComboBox::JKQTPstepTypeComboBox(QWidget *parent):
    QComboBox(parent)
{
    clear();
    setEditable(false);
    addStep(JKQTPstepType::JKQTPstepLeft, JKQTPstepType2String(JKQTPstepType::JKQTPstepLeft));
    addStep(JKQTPstepType::JKQTPstepCenter, JKQTPstepType2String(JKQTPstepType::JKQTPstepCenter));
    addStep(JKQTPstepType::JKQTPstepRight, JKQTPstepType2String(JKQTPstepType::JKQTPstepRight));
    setCurrentIndex(0);
}

JKQTPstepType JKQTPstepTypeComboBox::getStepType() const
{
    return String2JKQTPstepType(itemData(currentIndex()).toString());
}

void JKQTPstepTypeComboBox::setStepType(JKQTPstepType step)
{
    int i=findData(JKQTPstepType2String(step));
    if (i>=0) setCurrentIndex(i);
}

void JKQTPstepTypeComboBox::setCurrentStepType(JKQTPstepType step)
{
    setStepType(step);
}

void JKQTPstepTypeComboBox::addStep(JKQTPstepType step, const QString &name, const QIcon &icon)
{
    addItem(icon, name, JKQTPstepType2String(step));
}


JKQTPImageModifierModeComboBox::JKQTPImageModifierModeComboBox(QWidget * /*parent*/)
{
    addMode(JKQTPMathImageBase::ModifyNone, tr("none"));
    addMode(JKQTPMathImageBase::ModifyValue, tr("value"));
    addMode(JKQTPMathImageBase::ModifySaturation, tr("saturation"));
    addMode(JKQTPMathImageBase::ModifyAlpha, tr("alpha"));
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
    setCurrentIndex(findData((int)mode));
}

void JKQTPImageModifierModeComboBox::addMode(JKQTPMathImageBase::ModifierMode mode, const QString &name, const QIcon &icon)
{
    addItem(icon, name, (int)mode);
}
