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



#ifndef JKQTPCOMBOBOXES_H
#define JKQTPCOMBOBOXES_H

#include "jkqtplotter/jkqtpbaseplotter.h"
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/graphs/jkqtpimage.h"
#include <QComboBox>
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtplotter/jkqtptools.h"



/*! \brief class to plot an image from an 2-dimensional array of values
    \ingroup jkqtpcomboboxes

    \image html JKQTPMathImageColorPaletteComboBox.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPMathImageColorPaletteComboBox: public QComboBox {
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



/*! \brief a QComboBox which shows  JKQTPCADrawMode
    \ingroup jkqtpcomboboxes
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPCADrawModeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPCADrawModeComboBox(QWidget* parent=nullptr);

        JKQTPCADrawMode getDrawMode() const;
        void setDrawMode(JKQTPCADrawMode position);
    protected:
        void addDrawMode(JKQTPCADrawMode position, const QString& name, const QIcon& icon=QIcon());
};



/*! \brief a QComboBox which shows  JKQTPCALabelType
    \ingroup jkqtpcomboboxes
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPCALabelTypeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPCALabelTypeComboBox(QWidget* parent=nullptr);

        JKQTPCALabelType getLabelType() const;
        void setLabelType(JKQTPCALabelType position);
    protected:
        void addLabelType(JKQTPCALabelType position, const QString& name, const QIcon& icon=QIcon());
};


/*! \brief a QComboBox which shows  JKQTPLabelPosition
    \ingroup jkqtpcomboboxes
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPLabelPositionComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPLabelPositionComboBox(QWidget* parent=nullptr);

        JKQTPLabelPosition getPosition() const;
        void setPosition(JKQTPLabelPosition position);
    protected:
        void addPosition(JKQTPLabelPosition position, const QString& name, const QIcon& icon=QIcon());
};



/*! \brief a QComboBox which shows JKQTPKeyPosition
    \ingroup jkqtpcomboboxes

    \image html JKQTPKeyPositionComboBox.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPKeyPositionComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPKeyPositionComboBox(QWidget* parent=nullptr);

        JKQTPKeyPosition getPosition() const;
        void setPosition(JKQTPKeyPosition position);
    signals:
        void currentPositionChanged(JKQTPKeyPosition pos);
    protected:
        void addPosition(JKQTPKeyPosition position, const QString& name, const QIcon& icon=QIcon());
    protected slots:
        void posChanged(int index);
};



/*! \brief a QComboBox which shows JKQTPKeyPosition
    \ingroup jkqtpcomboboxes

    \image html JKQTPKeyLayoutComboBox.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPKeyLayoutComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPKeyLayoutComboBox(QWidget* parent=nullptr);

        JKQTPKeyLayout getKeyLayout() const;
        void setKeyLayout(JKQTPKeyLayout layout);
    signals:
        void currentLayoutChanged(JKQTPKeyLayout layout);
    protected:
        void addKeyLayout(JKQTPKeyLayout layout, const QString& name);
    protected slots:
        void currentIndexChangedP(int index);
};


/*! \brief a QComboBox which shows JKQTPErrorPlotstyle
    \ingroup jkqtpcomboboxes

    \image html JKQTPErrorPlotstyleComboBox.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPErrorPlotstyleComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPErrorPlotstyleComboBox(QWidget* parent=nullptr);

        JKQTPErrorPlotstyle getErrorLineStyle() const;
        void setSymbolType(JKQTPErrorPlotstyle symbol);
        void setCurrentErrorStyle(JKQTPErrorPlotstyle symbol);
    protected:
        void addSymbol(JKQTPErrorPlotstyle symbol, const QString& name, const QIcon &icon=QIcon());
};



/*! \brief a QComboBox which shows JKQTPGraphSymbols
    \ingroup jkqtpcomboboxes

    \image html JKQTPSymbolComboBox.png
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSymbolComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPSymbolComboBox(QWidget* parent=nullptr);

        JKQTPGraphSymbols getSymbol() const;
        void setSymbolType(JKQTPGraphSymbols symbol);
        void setCurrentSymbol(JKQTPGraphSymbols symbol);
    protected:
        void addSymbol(JKQTPGraphSymbols symbol, const QString& name);
};


/*! \brief a QComboBox to select whether a line, symbols or both should be displayed
    \ingroup jkqtpcomboboxes
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPLinePlotStyleComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPLinePlotStyleComboBox(QWidget* parent=nullptr);

        void setDefaultSymbol(JKQTPGraphSymbols symbol);
        void addUsedSymbol(JKQTPGraphSymbols symbol);

        JKQTPGraphSymbols getSymbol() const;
        bool getDrawLine() const;
    protected:
        void refill();
        void addSymbol(JKQTPGraphSymbols symbol, bool line, const QString& name=QString(""), const QVariant& data=QVariant());
        QList<JKQTPGraphSymbols> symbols;
        JKQTPGraphSymbols defaultSymbol;
};


/*! \brief a QComboBox to select whether a line, symbols or both should be displayed, in addition to JKQTPLinePlotStyleComboBox this may also have different symbol sizes!
    \ingroup jkqtpcomboboxes
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPLinePlotStyleWithSymbolSizeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPLinePlotStyleWithSymbolSizeComboBox(QWidget* parent=nullptr);

        void setDefaultSymbol(JKQTPGraphSymbols symbol, double size);
        void addUsedSymbol(JKQTPGraphSymbols symbol, double symbolSize, bool line);

        JKQTPGraphSymbols getSymbol() const;
        bool getDrawLine() const;
        double getSymbolSize() const;
    protected:
        void refill();
        void addSymbol(JKQTPGraphSymbols symbol, bool line, double symbolSize, const QString& name=QString(""), const QVariant& data=QVariant());
        struct styleData {
            JKQTPGraphSymbols symbol;
            bool line;
            double symbolSize;
            bool operator==(const styleData& other)const;
        };

        QList<styleData> symbols;
        JKQTPGraphSymbols defaultSymbol;
        double defaultSize;
};


/*! \brief a QComboBox which shows JKQTPSpecialLineType
    \ingroup jkqtpcomboboxes

    \see JKQTPSpecialLineType
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPSpecialLineTypeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPSpecialLineTypeComboBox(QWidget* parent=nullptr);

        JKQTPSpecialLineType getSpecialLineType() const;
        void setSpecialLineType(JKQTPSpecialLineType step);
        void setCurrentStepType(JKQTPSpecialLineType step);
    protected:
        void addStep(JKQTPSpecialLineType step, const QString& name, const QIcon &icon=QIcon());
};


/*! \brief a QComboBox which shows  JKQTPMathImageModifierMode
    \ingroup jkqtpcomboboxes

    \see JKQTPMathImageModifierMode
 */
class JKQTPLOTTER_LIB_EXPORT JKQTPImageModifierModeComboBox: public QComboBox {
        Q_OBJECT
    public:
        JKQTPImageModifierModeComboBox(QWidget* parent=nullptr);

        JKQTPMathImageModifierMode getModifierMode() const;
        void setModifierMode(JKQTPMathImageModifierMode mode);
    protected:
        void addMode(JKQTPMathImageModifierMode mode, const QString& name, const QIcon& icon=QIcon());
};

#endif // JKQTPCOMBOBOXES_H
