/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)

    

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



/**
 * \defgroup jkqtpcomboboxes diverse comboboxes for properties of JKQtPlotter
 * \ingroup jkqtpbaseplotter
 *
 *
 */

/** \file jkqtpcomboboxes.h
  * \ingroup jkqtpbaseplotter
  */

#ifndef JKQTPCOMBOBOXES_H
#define JKQTPCOMBOBOXES_H

#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include "jkqtplotter/jkqtpgraphsimage.h"
#include <QComboBox>
#include "jkqtplottertools/jkqtpimagetools.h"
#include "jkqtplottertools/jkqtptools.h"



/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtplotter_imagelots

 */
class LIB_EXPORT JKQTPMathImageColorPaletteComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPMathImageColorPaletteComboBox(QWidget* parent=nullptr);
        JKQTPMathImageColorPalette colorPalette() const;
        JKQTPMathImageColorPalette currentColorPalette() const;
        JKQTPMathImageColorPalette getColorPalette() const;
        void setColorPalette(JKQTPMathImageColorPalette palette);
        void setCurrentColorPalette(JKQTPMathImageColorPalette palette);
    signals:
        void currentPaletteChanged(JKQTPMathImageColorPalette palette);
    protected slots:
        void intIndexChanged(int i);

};



/*! \brief a QComboBox which shows  JKQTPCAdrawMode
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPCAdrawModeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPCAdrawModeComboBox(QWidget* parent=nullptr);

        JKQTPCAdrawMode getDrawMode() const;
        void setDrawMode(JKQTPCAdrawMode position);
    protected:
        void addDrawMode(JKQTPCAdrawMode position, const QString& name, const QIcon& icon=QIcon());
};



/*! \brief a QComboBox which shows  JKQTPCAlabelType
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPCAlabelTypeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPCAlabelTypeComboBox(QWidget* parent=nullptr);

        JKQTPCAlabelType getLabelType() const;
        void setLabelType(JKQTPCAlabelType position);
    protected:
        void addLabelType(JKQTPCAlabelType position, const QString& name, const QIcon& icon=QIcon());
};


/*! \brief a QComboBox which shows  JKQTPlabelPosition
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPlabelPositionComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPlabelPositionComboBox(QWidget* parent=nullptr);

        JKQTPlabelPosition getPosition() const;
        void setPosition(JKQTPlabelPosition position);
    protected:
        void addPosition(JKQTPlabelPosition position, const QString& name, const QIcon& icon=QIcon());
};



/*! \brief a QComboBox which shows JKQTPkeyPosition
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPkeyPositionComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPkeyPositionComboBox(QWidget* parent=nullptr);

        JKQTPkeyPosition getPosition() const;
        void setPosition(JKQTPkeyPosition position);
    signals:
        void currentPositionChanged(JKQTPkeyPosition pos);
    protected:
        void addPosition(JKQTPkeyPosition position, const QString& name, const QIcon& icon=QIcon());
    protected slots:
        void posChanged(int index);
};



/*! \brief a QComboBox which shows JKQTPkeyPosition
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPkeyLayoutComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPkeyLayoutComboBox(QWidget* parent=nullptr);

        JKQTPkeyLayout getKeyLayout() const;
        void setKeyLayout(JKQTPkeyLayout layout);
    signals:
        void currentLayoutChanged(JKQTPkeyLayout layout);
    protected:
        void addKeyLayout(JKQTPkeyLayout layout, const QString& name);
    protected slots:
        void currentIndexChangedP(int index);
};


/*! \brief a QComboBox which shows JKQTPerrorPlotstyle
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPerrorPlotstyleComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPerrorPlotstyleComboBox(QWidget* parent=nullptr);

        JKQTPerrorPlotstyle getErrorStyle() const;
        void setSymbol(JKQTPerrorPlotstyle symbol);
        void setCurrentErrorStyle(JKQTPerrorPlotstyle symbol);
    protected:
        void addSymbol(JKQTPerrorPlotstyle symbol, const QString& name, const QIcon &icon=QIcon());
};



/*! \brief a QComboBox which shows JKQTPgraphSymbols
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPSymbolComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPSymbolComboBox(QWidget* parent=nullptr);

        JKQTPgraphSymbols getSymbol() const;
        void setSymbol(JKQTPgraphSymbols symbol);
        void setCurrentSymbol(JKQTPgraphSymbols symbol);
    protected:
        void addSymbol(JKQTPgraphSymbols symbol, const QString& name);
};


/*! \brief a QComboBox to select whether a line, symbols or both should be displayed
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPLinePlotStyleComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPLinePlotStyleComboBox(QWidget* parent=nullptr);

        void setDefaultSymbol(JKQTPgraphSymbols symbol);
        void addUsedSymbol(JKQTPgraphSymbols symbol);

        JKQTPgraphSymbols getSymbol() const;
        bool getDrawLine() const;
    protected:
        void refill();
        void addSymbol(JKQTPgraphSymbols symbol, bool line, const QString& name=QString(""), const QVariant& data=QVariant());
        QList<JKQTPgraphSymbols> symbols;
        JKQTPgraphSymbols defaultSymbol;
};


/*! \brief a QComboBox to select whether a line, symbols or both should be displayed, in addition to JKQTPLinePlotStyleComboBox this may also have different symbol sizes!
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPLinePlotStyleWithSymbolSizeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPLinePlotStyleWithSymbolSizeComboBox(QWidget* parent=nullptr);

        void setDefaultSymbol(JKQTPgraphSymbols symbol, double size);
        void addUsedSymbol(JKQTPgraphSymbols symbol, double symbolSize, bool line);

        JKQTPgraphSymbols getSymbol() const;
        bool getDrawLine() const;
        double getSymbolSize() const;
    protected:
        void refill();
        void addSymbol(JKQTPgraphSymbols symbol, bool line, double symbolSize, const QString& name=QString(""), const QVariant& data=QVariant());
        struct styleData {
            JKQTPgraphSymbols symbol;
            bool line;
            double symbolSize;
            bool operator==(const styleData& other)const;
        };

        QList<styleData> symbols;
        JKQTPgraphSymbols defaultSymbol;
        double defaultSize;
};


/*! \brief a QComboBox which shows JKQTPstepType
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPstepTypeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPstepTypeComboBox(QWidget* parent=nullptr);

        JKQTPstepType getStepType() const;
        void setStepType(JKQTPstepType step);
        void setCurrentStepType(JKQTPstepType step);
    protected:
        void addStep(JKQTPstepType step, const QString& name, const QIcon &icon=QIcon());
};


/*! \brief a QComboBox which shows  JKQTPMathImageBase::ModifierMode
    \ingroup jkqtptools
 */
class LIB_EXPORT JKQTPImageModifierModeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPImageModifierModeComboBox(QWidget* parent=nullptr);

        JKQTPMathImageBase::ModifierMode getModifierMode() const;
        void setModifierMode(JKQTPMathImageBase::ModifierMode mode);
    protected:
        void addMode(JKQTPMathImageBase::ModifierMode mode, const QString& name, const QIcon& icon=QIcon());
};

#endif // JKQTPCOMBOBOXES_H
