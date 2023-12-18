/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)



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

#include "jkqtplotter/jkqtpkey.h"
#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtpgraphsbase.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include <QDebug>
#include <QDateTime>
#include <cfloat>
#include "jkqtplotter/jkqtptools.h"
#include "jkqtmathtext/jkqtmathtext.h"



JKQTPBaseKey::JKQTPBaseKey(JKQTBasePlotter* _parent):
    QObject(_parent),
    parent(_parent),
    localKeyStyle()
{

}

JKQTPBaseKey::~JKQTPBaseKey() {

}

void JKQTPBaseKey::setParent(JKQTBasePlotter* parent) {
    this->parent=parent;
    QObject::setParent(parent);
}

void JKQTPBaseKey::redrawPlot() {
    if (parent)  {
        parent->updateSecondaryAxes();
        parent->redrawPlot();
    }
}

void JKQTPBaseKey::loadSettings(const QSettings& settings, const QString& group) {
    keyStyle().loadSettings(settings, group);
}

void JKQTPBaseKey::saveSettings(QSettings& settings, const QString& group) const {
    keyStyle().saveSettings(settings, group);
}

void JKQTPBaseKey::drawKey(JKQTPEnhancedPainter &painter, const QRectF &rect, const KeySizeDescription& layout)
{
    if (!keyStyle().visible) return;

    QFont kf(JKQTMathTextFontSpecifier::fromFontSpec(keyStyle().fontName).fontName(), keyStyle().fontSize);
    kf.setPointSizeF(keyStyle().fontSize*getParent()->getFontSizeMultiplier());
    const QFontMetricsF kfm(kf, painter.device());
    const qreal Xwid=kfm.boundingRect('X').width();
    const qreal FHeight=kfm.height();

    // determine layouting info and size
    QPointF internalOffset(0,0);
    const QSizeF keySize=extendLayoutSize(QSizeF(layout.d->calcOverallWidth(keyStyle().sampleLineLength*Xwid, keyStyle().xSeparation*Xwid, keyStyle().columnSeparation*Xwid),
                                                 layout.d->calcOverallHeight(keyStyle().ySeparation*FHeight, keyStyle().sampleHeight*FHeight)), painter,  &internalOffset);
    if (keySize.isEmpty() || keySize.isNull() || !keySize.isValid()) return;

    const double frameWidth=qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, getParent()->pt2px(painter, keyStyle().frameWidth*getParent()->getLineWidthMultiplier()));

    const bool drawDebugRects=getParent()->isDebugShowRegionBoxesEnabled();

    // calculate start position for drawing
    QPointF x0(0,0);
    if (keyStyle().position.testFlag(JKQTPKeyLeft))  {
        x0.setX(rect.left()+keyStyle().xOffset*Xwid);
    } else if (keyStyle().position.testFlag(JKQTPKeyHCenter))  {
        x0.setX(rect.left()+(rect.width()-keySize.width())/2.0+keyStyle().xOffset*Xwid);
    } else if (keyStyle().position.testFlag(JKQTPKeyRight))  {
        x0.setX(rect.right()-keySize.width()-keyStyle().xOffset*Xwid);
    }
    if (keyStyle().position.testFlag(JKQTPKeyTop))  {
        x0.setY(rect.top()+keyStyle().yOffset*Xwid);
    } else if (keyStyle().position.testFlag(JKQTPKeyVCenter))  {
        x0.setY(rect.top()+(rect.height()-keySize.height())/2.0+keyStyle().yOffset*Xwid);
    } else if (keyStyle().position.testFlag(JKQTPKeyBottom))  {
        x0.setY(rect.bottom()-keySize.height()-keyStyle().yOffset*Xwid);
    }


    const QRectF rectKey(x0, keySize);

    // construct necessary pens ...
    QPen pf;
    if (keyStyle().frameVisible) {
        pf.setColor(keyStyle().frameColor);
        pf.setWidthF(frameWidth);
        pf.setStyle(keyStyle().frameLineStyle);
    } else {
        pf=Qt::NoPen;
    }

    // start drawing
    {
        painter.save(); auto __finalpaint=JKQTPFinally([&painter]() {painter.restore();});

        // draw background rectangle
        {
            painter.save(); auto __finalbackpaint=JKQTPFinally([&painter]() {painter.restore();});
            painter.setBrush(keyStyle().backgroundBrush);
            painter.setPen(pf);
            if (keyStyle().frameRounding<=0) {
                painter.drawRect(rectKey);
            } else {
                painter.drawRoundedRect(rectKey, getParent()->pt2px(painter, keyStyle().frameRounding), getParent()->pt2px(painter, keyStyle().frameRounding));
            }
        }

        // draw key table/contents
        x0=x0+internalOffset;
        QPointF xi=x0;
        for (const auto& c: layout.d->columns) {
            xi.setY(x0.y());
            int ir=0;
            for (const auto& r: c.rows) {
                const QRectF sampleRect(xi, QSizeF(keyStyle().sampleLineLength*Xwid, keyStyle().sampleHeight*FHeight));
                const double rowHeight=layout.d->calcRowHeight(ir, sampleRect.height());
                const QRectF textRect(xi+QPointF((keyStyle().sampleLineLength+keyStyle().xSeparation)*Xwid, 0), QSize(r.size.width(), rowHeight));
                drawEntrySample(r.id, painter, sampleRect),
                getParentMathText()->setFontColor(keyStyle().textColor);
                getParentMathText()->setFontPointSize(keyStyle().fontSize*getParent()->getFontSizeMultiplier());
                getParentMathText()->setFontSpecial(keyStyle().fontName);
                getParentMathText()->parse(r.text);
                getParentMathText()->draw(painter, Qt::AlignLeft|Qt::AlignVCenter, textRect, getParent()->isDebugShowTextBoxesEnabled());

                if (drawDebugRects) {
                    painter.save(); auto __finalpaintinner=JKQTPFinally([&painter]() {painter.restore();});
                    QPen p("orange");
                    QColor col=p.color(); col.setAlphaF(0.8f); p.setColor(col);
                    p.setWidthF(getParent()->getCurrentPlotterStyle().debugRegionLineWidth/2.0);
                    p.setStyle(Qt::DashLine);
                    painter.setPen(p);
                    painter.setBrush(QBrush(QColor(Qt::transparent)));
                    painter.drawRect(textRect);
                    painter.drawRect(sampleRect);
                }

                xi.setY(xi.y()+rowHeight+keyStyle().ySeparation*FHeight);
                ir++;
            }
            xi.setX(xi.x()+c.calcColumnWidth(keyStyle().sampleLineLength*Xwid, keyStyle().xSeparation*Xwid)+keyStyle().columnSeparation*Xwid);
        }

    }
}

JKQTPBaseKey::KeySizeDescription JKQTPBaseKey::getSize(JKQTPEnhancedPainter &painter)
{
    KeySizeDescription size;
    if (!keyStyle().visible) return size;

    QFont kf(JKQTMathTextFontSpecifier::fromFontSpec(keyStyle().fontName).fontName(), keyStyle().fontSize);
    kf.setPointSizeF(keyStyle().fontSize*getParent()->getFontSizeMultiplier());
    const QFontMetricsF kfm(kf, painter.device());
    const qreal Xwid=kfm.boundingRect('X').width();

    // calculate layout of the "table" of samples and labels
    const KeyLayoutDescription layout=getKeyLayout(painter);
    size.d->operator=(layout);

    // calculate size of full key
    calcLayoutSize(painter, size);

    return size;
}


void JKQTPBaseKey::calcLayoutSize(JKQTPEnhancedPainter &painter, KeySizeDescription &layout) const
{
    QFont kf(JKQTMathTextFontSpecifier::fromFontSpec(keyStyle().fontName).fontName(), keyStyle().fontSize);
    kf.setPointSizeF(keyStyle().fontSize*getParent()->getFontSizeMultiplier());
    const QFontMetricsF kfm(kf, painter.device());
    const qreal Xwid=kfm.boundingRect('X').width();
    const qreal FHeight=kfm.height();

    layout.requiredSize=extendLayoutSize(QSizeF(layout.d->calcOverallWidth(keyStyle().sampleLineLength*Xwid, keyStyle().xSeparation*Xwid, keyStyle().columnSeparation*Xwid),
                                                layout.d->calcOverallHeight(keyStyle().ySeparation*FHeight, keyStyle().sampleHeight*FHeight)), painter);

    // determine location of key from keySTyle().positon
    if (keyStyle().position.testFlag(JKQTPKeyInside)) layout.keyLocation=KeySizeDescription::keyInside;
    else if (keyStyle().position.testFlag(JKQTPKeyOutsideTop)) { layout.keyLocation=KeySizeDescription::keyOutsideTop; layout.requiredSize+=QSizeF(0,keyStyle().yOffset*Xwid); }
    else if (keyStyle().position.testFlag(JKQTPKeyOutsideBottom)) { layout.keyLocation=KeySizeDescription::keyOutsideBottom; layout.requiredSize+=QSizeF(0,keyStyle().yOffset*Xwid); }
    else if (keyStyle().position.testFlag(JKQTPKeyOutsideLeft)) { layout.keyLocation=KeySizeDescription::keyOutsideLeft; layout.requiredSize+=QSizeF(0,keyStyle().xOffset*Xwid); }
    else if (keyStyle().position.testFlag(JKQTPKeyOutsideRight)) { layout.keyLocation=KeySizeDescription::keyOutsideRight; layout.requiredSize+=QSizeF(0,keyStyle().xOffset*Xwid); }
}

void JKQTPBaseKey::modifySize(JKQTPEnhancedPainter &painter, KeySizeDescription &currentKeyLayout, QSizeF preliminaryPlotSize)
{
    const auto lay=getLayout();
    if (lay==JKQTPKeyLayoutMultiColumn || lay==JKQTPKeyLayoutMultiRow) {
        std::function<bool(QSizeF, QSizeF)> fCompare=[](const QSizeF& requiredSize, const QSizeF& preliminaryPlotSize) {
            return true;
        };

        if (currentKeyLayout.keyLocation==KeySizeDescription::keyInside) {
            fCompare=[](const QSizeF& requiredSize, const QSizeF& preliminaryPlotSize) {
                            return (requiredSize.width()>preliminaryPlotSize.width() || requiredSize.height()>preliminaryPlotSize.height());
                        };
        } else if (currentKeyLayout.keyLocation==KeySizeDescription::keyOutsideTop || currentKeyLayout.keyLocation==KeySizeDescription::keyOutsideBottom) {
            fCompare=[](const QSizeF& requiredSize, const QSizeF& preliminaryPlotSize) {
                            return (requiredSize.height()>preliminaryPlotSize.height());
                        };
        } else if (currentKeyLayout.keyLocation==KeySizeDescription::keyOutsideLeft || currentKeyLayout.keyLocation==KeySizeDescription::keyOutsideRight) {
            fCompare=[](const QSizeF& requiredSize, const QSizeF& preliminaryPlotSize) {
                            return (requiredSize.width()>preliminaryPlotSize.width());
                        };
        }

        const int itemCnt=currentKeyLayout.d->countItems();
        int newCount=1;
        while ((newCount<=itemCnt) && (currentKeyLayout.requiredSize.width()>preliminaryPlotSize.width() || currentKeyLayout.requiredSize.height()>preliminaryPlotSize.height())) {
            newCount++;
            if (lay==JKQTPKeyLayoutMultiColumn) {
                currentKeyLayout.d->redistributeOverColumns(newCount);
            } else if (lay==JKQTPKeyLayoutMultiRow) {
                currentKeyLayout.d->redistributeOverRows(newCount);
            }
            calcLayoutSize(painter, currentKeyLayout);
        }
    }
}

JKQTMathText* JKQTPBaseKey::getParentMathText() {
    if (!parent) return nullptr;
    return parent->getMathText();
}

const JKQTMathText* JKQTPBaseKey::getParentMathText() const {
    if (!parent) return nullptr;
    return parent->getMathText();
}

const JKQTPKeyStyle &JKQTPBaseKey::getCurrentKeyStyle() const
{
    return keyStyle();
}

void JKQTPBaseKey::setCurrentKeyStyle(const JKQTPKeyStyle &style)
{
    keyStyle()=style;
    redrawPlot();
}

JKQTPBaseKey::KeyLayoutDescription JKQTPBaseKey::getKeyLayout(JKQTPEnhancedPainter &painter)
{
    QFont kf(JKQTMathTextFontSpecifier::fromFontSpec(keyStyle().fontName).fontName(), keyStyle().fontSize);
    kf.setPointSizeF(keyStyle().fontSize*getParent()->getFontSizeMultiplier());
    QFontMetricsF kfm(kf, painter.device());
    //const qreal Xwid=kfm.boundingRect('X').width();
    const qreal Fheight=kfm.height();
    //const double frameWidth=qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, getParent()->pt2px(painter, keyStyle().frameWidth*getParent()->getLineWidthMultiplier()));


    JKQTPBaseKey::KeyLayoutDescription layout;

    KeyColumnDescription allItems;

    // first collect all items into one column
    const int NItems=getEntryCount();
    for (int i=0; i<NItems; i++) {
        KeyItemData item;
        item.id=i;
        item.text=getEntryText(i);
        item.size=getParent()->getTextSizeSize(keyStyle().fontName, keyStyle().fontSize, item.text, painter);
        item.size.setHeight(qMax(item.size.height(), keyStyle().sampleHeight*Fheight));
        allItems.rows.push_back(item);
    }

    // now redistrbute accordint to keyStyle().layout
    if (keyStyle().layout==JKQTPKeyLayoutOneColumn || keyStyle().layout==JKQTPKeyLayoutMultiColumn) {
        layout.columns.append(allItems);
    } else if (keyStyle().layout==JKQTPKeyLayoutOneRow || keyStyle().layout==JKQTPKeyLayoutMultiRow) {
        for (const auto& r: allItems.rows) {
            layout.columns.emplaceBack(r);
        }
    }


    return layout;
}


QSizeF JKQTPBaseKey::extendLayoutSize(QSizeF rawLayoutSize, JKQTPEnhancedPainter& painter, QPointF *offset) const
{
    QFont kf(JKQTMathTextFontSpecifier::fromFontSpec(keyStyle().fontName).fontName(), keyStyle().fontSize);
    kf.setPointSizeF(keyStyle().fontSize*getParent()->getFontSizeMultiplier());
    QFontMetricsF kfm(kf, painter.device());
    const qreal Xwid=kfm.boundingRect('X').width();
    const double frameWidth=qMax(JKQTPlotterDrawingTools::ABS_MIN_LINEWIDTH, getParent()->pt2px(painter, keyStyle().frameWidth*getParent()->getLineWidthMultiplier()));

    if (rawLayoutSize.width()>0) rawLayoutSize.setWidth(rawLayoutSize.width()+2.0*keyStyle().xMargin*Xwid+2.0*frameWidth);
    if (rawLayoutSize.height()>0) rawLayoutSize.setHeight(rawLayoutSize.height()+2.0*keyStyle().yMargin*Xwid+2.0*frameWidth);
    if (offset) *offset=QPointF(keyStyle().xMargin*Xwid+frameWidth, keyStyle().yMargin*Xwid+frameWidth);
    return rawLayoutSize;
}

const JKQTPKeyStyle &JKQTPBaseKey::keyStyle() const
{
    return localKeyStyle;
}

JKQTPKeyStyle &JKQTPBaseKey::keyStyle()
{
    return localKeyStyle;
}

JKQTPBaseKey::KeyItemData::KeyItemData(int _id, const QString &_text, const QSizeF _size):
    id(_id),
    text(_text),
    size(_size)
{

}


JKQTPBaseKey::KeySizeDescription::KeySizeDescription(QSize _requiredSize, KeyLocation _keyLocation):
    requiredSize(_requiredSize),
    keyLocation(_keyLocation),
    d(new JKQTPBaseKey::KeyLayoutDescription)
{

}

JKQTPBaseKey::KeySizeDescription::KeySizeDescription(const KeySizeDescription &other):
    requiredSize(),
    keyLocation(KeyLocation::keyInside),
    d(new JKQTPBaseKey::KeyLayoutDescription)
{
    operator=(other);
}

JKQTPBaseKey::KeySizeDescription &JKQTPBaseKey::KeySizeDescription::operator=(const KeySizeDescription &other)
{
    requiredSize=other.requiredSize;
    keyLocation=other.keyLocation;
    d->operator=(*(other.d));
    return *this;
}

JKQTPBaseKey::KeyColumnDescription::KeyColumnDescription():
    rows()
{

}

JKQTPBaseKey::KeyColumnDescription::KeyColumnDescription(const KeyItemData &item1):
    KeyColumnDescription()
{
    rows.append(item1);
}

double JKQTPBaseKey::KeyColumnDescription::calcMaxLabelWidth() const
{
    double w=0;
    for (const auto& r: rows) {
        w=qMax(w, r.size.width());
    }
    return w;
}

double JKQTPBaseKey::KeyColumnDescription::calcColumnWidth(double sampleLineLength, double xSeparation) const
{
    return calcMaxLabelWidth()+sampleLineLength+xSeparation;
}

double JKQTPBaseKey::KeyLayoutDescription::calcOverallWidth(double sampleLineLength, double xSeparation, double columnSeparation) const
{
    double w=0;
    for (const auto& c: columns) {
        if (w>0) w+=columnSeparation;
        w+=sampleLineLength;
        w+=xSeparation;
        w+=c.calcMaxLabelWidth();
    }
    return w;
}

double JKQTPBaseKey::KeyLayoutDescription::calcOverallHeight(double ySeparation, double sampleHeight) const
{
    double h=0;
    const int N=calcRowCount();
    for (int i=0; i<N; i++) {
        h+=calcRowHeight(i, sampleHeight);
        if (i>0) h+=ySeparation;
    }
    return h;
}

double JKQTPBaseKey::KeyLayoutDescription::calcRowHeight(int i, double sampleHeight) const
{
    double h=0;
    for (const auto& c: columns) {
        if (i>=0 && i<c.rows.size()) h=qMax(h, c.rows[i].size.height());
    }
    if (h>0) h=qMax(sampleHeight, h);
    return h;

}

int JKQTPBaseKey::KeyLayoutDescription::calcRowCount() const
{
    int n=0;
    for (const auto& c: columns) {
        n=qMax(n, c.rows.size());
    }
    return n;
}

int JKQTPBaseKey::KeyLayoutDescription::countItems() const
{
    int n=0;
    for (const auto& c: columns) {
        n+=c.rows.size();
    }
    return n;
}

void JKQTPBaseKey::KeyLayoutDescription::redistributeIntoOneColumn()
{
    if (countItems()>1) {
        for (int i=1; i<columns.size(); i++) {
            columns[0].rows.append(columns[i].rows);
        }
        for (int i=columns.size()-1; i>=1; i--) {
            columns.remove(i);
        }
    }
}

void JKQTPBaseKey::KeyLayoutDescription::redistributeOverRows(int rowCnt)
{
    const int itemCnt=countItems();
    if (itemCnt>1) {
        const int colCnt=static_cast<int>(ceil(static_cast<float>(itemCnt)/static_cast<float>(rowCnt)));
        if (colCnt>1) {
            redistributeIntoOneColumn();
            const auto items=columns[0].rows;
            columns.clear();

            int i=0;
            for (int c=0; c<colCnt; c++) {
                columns.emplace_back();
            }
            for (int r=0; r<rowCnt; r++) {
                for (int c=0; c<colCnt; c++) {
                    if (i<itemCnt) columns[c].rows.append(items[i]);
                    i++;
                }
            }
        }
    }
}

void JKQTPBaseKey::KeyLayoutDescription::redistributeOverColumns(int colCnt)
{
    const int itemCnt=countItems();
    if (itemCnt>1) {
        const int rowCnt=static_cast<int>(ceil(static_cast<float>(itemCnt)/static_cast<float>(colCnt)));
        if (colCnt>1) {
            redistributeIntoOneColumn();
            const auto items=columns[0].rows;
            columns.clear();
            int i=0;
            for (int c=0; c<colCnt; c++) {
                columns.emplace_back();
                for (int r=0; r<rowCnt; r++) {
                    if (i<itemCnt) columns[c].rows.append(items[i]);
                    i++;
                }
            }
        }
    }
}




JKQTPMainKey::JKQTPMainKey(JKQTBasePlotter *parent):
    JKQTPBaseKey(parent)
{

}

JKQTPMainKey::~JKQTPMainKey()
{

}

int JKQTPMainKey::getEntryCount() const
{
    return getPlotElements().size();
}


QString JKQTPMainKey::getEntryText(int item) const
{
    QString s;
    const auto g=getPlotElement(item);
    if (g) s=g->getTitle();
    return s;
}

QColor JKQTPMainKey::getEntryColor(int item) const
{
    QColor s=keyStyle().textColor;
    const auto g=getPlotElement(item);
    if (g) s=g->getKeyLabelColor();
    return s;
}

void JKQTPMainKey::drawEntrySample(int item, JKQTPEnhancedPainter &painter, const QRectF &rect)
{
    auto g=getPlotElement(item);
    if (g) g->drawKeyMarker(painter, rect);
}

QList<const JKQTPPlotElement *> JKQTPMainKey::getPlotElements() const
{
    const auto p=getParent();
    QList<const JKQTPPlotElement *> l;
    if (!p) return l;
    l.reserve(p->getGraphCount());
    for (size_t i=0; i<p->getGraphCount(); i++) {
        const auto g=p->getGraph(i);
        if (g && g->isVisible() && !g->getTitle().isEmpty()) {
            l<<g;
        }
    }
    return l;
}

const JKQTPKeyStyle &JKQTPMainKey::keyStyle() const
{
    return getParent()->getMainKeyStyle();
}

JKQTPKeyStyle &JKQTPMainKey::keyStyle()
{
    return getParent()->getMainKeyStyle();
}

const JKQTPPlotElement* JKQTPMainKey::getPlotElement(int item) const
{
    const auto p=getParent();
    if (!p) return nullptr;
    int curitem=0;
    for (size_t i=0; i<p->getGraphCount(); i++) {
        const auto g=p->getGraph(i);
        if (g && g->isVisible() && !g->getTitle().isEmpty()) {
            if (curitem==item) return g;
            curitem++;
        }
    }
    return nullptr;
}


JKQTPPlotElement* JKQTPMainKey::getPlotElement(int item)
{
    auto p=getParent();
    if (!p) return nullptr;
    int curitem=0;
    for (size_t i=0; i<p->getGraphCount(); i++) {
        auto g=p->getGraph(i);
        if (g && g->isVisible() && !g->getTitle().isEmpty()) {
            if (curitem==item) return g;
            curitem++;
        }
    }
    return nullptr;
}

