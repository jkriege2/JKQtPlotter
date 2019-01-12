/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>)
    with contributions from: Razi Alavizadeh

    

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


/*
  Name: jkqtmathtext.h
  Copyright: (c) 2010-2015
  Author: Jan krieger <jan@jkrieger.de>, http://www.jkrieger.de/
*/


/** \file jkqtmathtext.h
  * \ingroup jkqtmathtext
  */


#ifndef JKQTMATHTEXT_H
#define JKQTMATHTEXT_H

#include <QObject>
#include <QSettings>
#include <QPainter>
#include <QString>
#include <QSet>
#include <QFile>
#include "jkqtplottertools/jkqtptools.h"
#include "jkqtplottertools/jkqtp_imexport.h"
#include <QWidget>
#include <QLabel>
#include <QHash>

#define JKQTMATHTEXT_ABS_MIN_LINEWIDTH 0.02



/** \brief initialized Qt-ressources necessary for JKQtMathText
 *  \ingroup jkqtmathtext
 */
LIB_EXPORT void initJKQtMathTextResources();

/*! \brief this class manages an enhanced text message (containing a subset of LaTeX markups) and allows to draw a representation
    \ingroup jkqtmathtext

    This class first parses the enhanced text string and then allows to question the size of the representation
    and to draw a representation to a given painter object.

    The supported LaTeX subset is listes below. Please note that some commands are used differently than in actual LaTeX.
    For example \c \\bf is just a renamed form of \c \\textbf and used as \c \\bf{...} and NOT as \c {\\bf...} .

      - \c $ : enter/leave math mode
      - \c \\bf{...} \c \\textbf{...} \c \\mathbf{...}:  draw the contained text in bold face
      - \c \\it{...} \c \\textit{...} \c \\mathit{...} :  draw the contained text in italic face
      - \c \\rm{...} \c \\textrm{...} \c \\mathrm{...} \c \\mbox{...} :  draw the contained text in normal upright roman font face
      - \c \\sf{...} \c \\textsf{...} \c \\mathsf{...} :  draw the contained text in normal upright sans-serif font face
      - \c \\script{...} \c \\textscript{...} \c \\mathscript{...} :  draw the contained text in a script font face
      - \c \\sc{...} : draw the text in small caps
      - \c \\ul{...} \c \\underline{...} \c \\underlined{...} : draw the text with underlining
      - \c \\tt{...} \c \\texttt{...} \c \\mathtt{...} : draw text in typewriter font
      - \c \\alpha ... : display the according greek letter
      - \c ^{...} : display the contents of braces in superscript
      - \c _{...} : display the contents of braces in subscript
      - \c \\{ / \\} : display opening/closing brace
      - \c \\_ : display underscore
      - \c \\sum \c \\prod \c \\int ... : plot special symbol. Note that depending on the fontEncoding the available
                                          symbols may differ (there are not all symbols defined in the MS Windows Symbol
                                          font!). Best coverage should be given by Unicode font encoding with a good
                                          unicode font installed!
    .

    Braces in math mode are adjusted in size, so they are a small bit (factor \c brace_factor ) larger than the contents.
    To enable this you have to write braces with \c \\left and \c \\right. These types of braces are defined (slight
    differences to LaTeX standard):
      - \c \\left( \c \\right) : default meaning ()
      - \c \\left[ \c \\right] : default meaning []
      - \c \\left\\{ \c \\right\\} : default meaning {}
      - \c \\left< \c \\right> : "averaging" braces <>
      - \c \\left\\_ \c \\right\\_ : floor braces |_  _|
      - \c \\left~ \c \\right~ : ceil braces |~  ~|
    .

    Several fonts are defined as properties to the class:
      - A "roman" font used as the standard font
      - A "sans-serif" font which may be activated with \c \\sf ...
      - A "typewriter" font which may be activated with \c \\tt ...
      - A "script" font which may be activated with \c \\script ...
      - A greek font which is used to display greek letters \c \\alpha ...
      - A symbol font used to display special (math) symbols.
      - A "roman" font used as the standard font in math mode
      - A "sans-serif" used as sans serif font in math mode
      - A "blackboard" font used to display double stroked characters
      - A "caligraphic" font used to display caligraphic characters
    .

    The method toHtml() may be used to get a HTML representation of the LaTeX string, if possible (only for simple LaTeX equations!). Whether
    the transformation was possible is returned as a call by value argument!

    This small peace of C++ code may serve as an example of the usage and capabilities of the class:
    \include test/jkqtmathtext_test/jkqtmathtext_test.cpp

    Here is a qmake project file for this:
    \include test/jkqtmathtext_test/jkqtmathtext_test.pro

 */
class LIB_EXPORT JKQTmathText : public QObject {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTmathText(QObject * parent = nullptr);
        /** \brief class destructor */
        ~JKQTmathText();
        /** \brief load the object settings from the given QSettings object with the given name prefix */
        void loadSettings(QSettings& settings, QString group=QString("mathtext"));
        /** \brief store the object settings to the given QSettings object with the given name prefix */
        void saveSettings(QSettings& settings, QString group=QString("mathtext"));
        /** \brief parse the given enhanced string. Returns \c true on success. */
        bool parse(QString text);
        /** \brief get the size of the drawn representation. returns an invalid size if no text has been parsed. */
        QSizeF getSize(QPainter& painter);
        /** \brief return the descent, i.e. the distance from the baseline to the lowest part of the representation */
        double getDescent(QPainter& painter);
        /** \brief return the ascentt, i.e. the distance from the baseline to the highest part of the representation */
        double getAscent(QPainter& painter);
        /** \brief return the detailes sizes of the text */
        void getSizeDetail(QPainter& painter, double& width, double& ascent, double& descent, double& strikeoutPos);
        /** \brief draw a representation to the QPainter object at the specified position */
        void draw(QPainter& painter, double x, double y, bool drawBoxes=false);
        /** \brief overloaded version of draw(QPainter& painter, double x, double y).
         *
         *  This version draws the text inside the given rectangle according to the specified flags.
         */
        void draw(QPainter& painter, int flags, QRectF rect, bool drawBoxes=false);

        /** \brief configures the class to use the STIX fonts in mathmode */
        void useSTIX();

        /** \brief configures the class to use the XITS fonts in mathmode */
        void useXITS();

        /** \brief configures the class to use the ASANA fonts in mathmode */
        void useASANA();

        /** \brief configures the class to use a unicode font for symbols in mathmode */
        void useAnyUnicode(QString timesFont=QString(""), QString sansFont=QString(""));

        void useLatexFonts(QString prefix=QString(""), QString postfix=QString(""));

        /** \brief convert LaTeX to HTML. returns \c ok=true on success and \c ok=false else. */
        QString toHtml(bool* ok=nullptr, double fontPointSize=10);

        /*! \brief used to specify the font encoding used for drawing

               - \c MTFEwinSymbol: This assumes that symbols shal be taken from a MS Windows style Symbol font
               - \c MTFElatex:     This assumes that symbols shal be taken from the CM (computer modern) fonts, used by LaTeX
               - \c MTFEunicode:   This assumes that symbols shall be taken from a Unicode font
                                   (e.g. the STIX fonts from <a href="http://www.stixfonts.org/">http://www.stixfonts.org/</a>)
            .
        */
        enum MTfontEncoding {
            MTFEwinSymbol,
            MTFEunicode,
            MTFElatex
        };

        /*! \brief sets the property fontColor to the specified \a __value. 
            \details Description of the parameter fontColor is: <BLOCKQUOTE>\copybrief fontColor </BLOCKQUOTE> 
            \see fontColor for more information */ 
        inline virtual void set_fontColor(const QColor & __value)  
        {
            this->fontColor = __value;
        } 
        /*! \brief returns the property fontColor. 
            \details Description of the parameter fontColor is: <BLOCKQUOTE>\copybrief fontColor </BLOCKQUOTE> 
            \see fontColor for more information */ 
        inline virtual QColor get_fontColor() const  
        {
            return this->fontColor; 
        }
        /*! \brief sets the property fontSize to the specified \a __value. 
            \details Description of the parameter fontSize is: <BLOCKQUOTE>\copybrief fontSize </BLOCKQUOTE> 
            \see fontSize for more information */ 
        inline virtual void set_fontSize(double __value)
        {
            this->fontSize = __value;
        } 
        /*! \brief returns the property fontSize. 
            \details Description of the parameter fontSize is: <BLOCKQUOTE>\copybrief fontSize </BLOCKQUOTE> 
            \see fontSize for more information */ 
        inline virtual double get_fontSize() const  
        {
            return this->fontSize; 
        }
        /*! \brief sets the property fontRoman to the specified \a __value. 
            \details Description of the parameter fontRoman is: <BLOCKQUOTE>\copybrief fontRoman </BLOCKQUOTE> 
            \see fontRoman for more information */ 
        inline virtual void set_fontRoman(const QString & __value)  
        {
            this->fontRoman = __value;
        } 
        /*! \brief returns the property fontRoman. 
            \details Description of the parameter fontRoman is: <BLOCKQUOTE>\copybrief fontRoman </BLOCKQUOTE> 
            \see fontRoman for more information */ 
        inline virtual QString get_fontRoman() const  
        {
            return this->fontRoman; 
        }
        /*! \brief sets the property fontSans to the specified \a __value. 
            \details Description of the parameter fontSans is: <BLOCKQUOTE>\copybrief fontSans </BLOCKQUOTE> 
            \see fontSans for more information */ 
        inline virtual void set_fontSans(const QString & __value)  
        {
            this->fontSans = __value;
        } 
        /*! \brief returns the property fontSans. 
            \details Description of the parameter fontSans is: <BLOCKQUOTE>\copybrief fontSans </BLOCKQUOTE> 
            \see fontSans for more information */ 
        inline virtual QString get_fontSans() const  
        {
            return this->fontSans; 
        }
        /*! \brief sets the property fontTypewriter to the specified \a __value. 
            \details Description of the parameter fontTypewriter is: <BLOCKQUOTE>\copybrief fontTypewriter </BLOCKQUOTE> 
            \see fontTypewriter for more information */ 
        inline virtual void set_fontTypewriter(const QString & __value)  
        {
            this->fontTypewriter = __value;
        } 
        /*! \brief returns the property fontTypewriter. 
            \details Description of the parameter fontTypewriter is: <BLOCKQUOTE>\copybrief fontTypewriter </BLOCKQUOTE> 
            \see fontTypewriter for more information */ 
        inline virtual QString get_fontTypewriter() const  
        {
            return this->fontTypewriter; 
        }
        /*! \brief sets the property fontScript to the specified \a __value. 
            \details Description of the parameter fontScript is: <BLOCKQUOTE>\copybrief fontScript </BLOCKQUOTE> 
            \see fontScript for more information */ 
        inline virtual void set_fontScript(const QString & __value)  
        {
            this->fontScript = __value;
        } 
        /*! \brief returns the property fontScript. 
            \details Description of the parameter fontScript is: <BLOCKQUOTE>\copybrief fontScript </BLOCKQUOTE> 
            \see fontScript for more information */ 
        inline virtual QString get_fontScript() const  
        {
            return this->fontScript; 
        }
        /*! \brief sets the property fontGreek to the specified \a __value. 
            \details Description of the parameter fontGreek is: <BLOCKQUOTE>\copybrief fontGreek </BLOCKQUOTE> 
            \see fontGreek for more information */ 
        inline virtual void set_fontGreek(const QString & __value)  
        {
            this->fontGreek = __value;
        } 
        /*! \brief returns the property fontGreek. 
            \details Description of the parameter fontGreek is: <BLOCKQUOTE>\copybrief fontGreek </BLOCKQUOTE> 
            \see fontGreek for more information */ 
        inline virtual QString get_fontGreek() const  
        {
            return this->fontGreek; 
        }
        /*! \brief sets the property fontSymbol to the specified \a __value. 
            \details Description of the parameter fontSymbol is: <BLOCKQUOTE>\copybrief fontSymbol </BLOCKQUOTE> 
            \see fontSymbol for more information */ 
        inline virtual void set_fontSymbol(const QString & __value)  
        {
            this->fontSymbol = __value;
        } 
        /*! \brief returns the property fontSymbol. 
            \details Description of the parameter fontSymbol is: <BLOCKQUOTE>\copybrief fontSymbol </BLOCKQUOTE> 
            \see fontSymbol for more information */ 
        inline virtual QString get_fontSymbol() const  
        {
            return this->fontSymbol; 
        }
        /*! \brief sets the property fontBraces to the specified \a __value. 
            \details Description of the parameter fontBraces is: <BLOCKQUOTE>\copybrief fontBraces </BLOCKQUOTE> 
            \see fontBraces for more information */ 
        inline virtual void set_fontBraces(const QString & __value)  
        {
            this->fontBraces = __value;
        } 
        /*! \brief returns the property fontBraces. 
            \details Description of the parameter fontBraces is: <BLOCKQUOTE>\copybrief fontBraces </BLOCKQUOTE> 
            \see fontBraces for more information */ 
        inline virtual QString get_fontBraces() const  
        {
            return this->fontBraces; 
        }
        /*! \brief sets the property fontIntegrals to the specified \a __value. 
            \details Description of the parameter fontIntegrals is: <BLOCKQUOTE>\copybrief fontIntegrals </BLOCKQUOTE> 
            \see fontIntegrals for more information */ 
        inline virtual void set_fontIntegrals(const QString & __value)  
        {
            this->fontIntegrals = __value;
        } 
        /*! \brief returns the property fontIntegrals. 
            \details Description of the parameter fontIntegrals is: <BLOCKQUOTE>\copybrief fontIntegrals </BLOCKQUOTE> 
            \see fontIntegrals for more information */ 
        inline virtual QString get_fontIntegrals() const  
        {
            return this->fontIntegrals; 
        }
        /*! \brief sets the property fontCaligraphic to the specified \a __value. 
            \details Description of the parameter fontCaligraphic is: <BLOCKQUOTE>\copybrief fontCaligraphic </BLOCKQUOTE> 
            \see fontCaligraphic for more information */ 
        inline virtual void set_fontCaligraphic(const QString & __value)  
        {
            this->fontCaligraphic = __value;
        } 
        /*! \brief returns the property fontCaligraphic. 
            \details Description of the parameter fontCaligraphic is: <BLOCKQUOTE>\copybrief fontCaligraphic </BLOCKQUOTE> 
            \see fontCaligraphic for more information */ 
        inline virtual QString get_fontCaligraphic() const  
        {
            return this->fontCaligraphic; 
        }
        /*! \brief sets the property fontBlackboard to the specified \a __value. 
            \details Description of the parameter fontBlackboard is: <BLOCKQUOTE>\copybrief fontBlackboard </BLOCKQUOTE> 
            \see fontBlackboard for more information */ 
        inline virtual void set_fontBlackboard(const QString & __value)  
        {
            this->fontBlackboard = __value;
        } 
        /*! \brief returns the property fontBlackboard. 
            \details Description of the parameter fontBlackboard is: <BLOCKQUOTE>\copybrief fontBlackboard </BLOCKQUOTE> 
            \see fontBlackboard for more information */ 
        inline virtual QString get_fontBlackboard() const  
        {
            return this->fontBlackboard; 
        }
        /*! \brief sets the property fontMathRoman to the specified \a __value. 
            \details Description of the parameter fontMathRoman is: <BLOCKQUOTE>\copybrief fontMathRoman </BLOCKQUOTE> 
            \see fontMathRoman for more information */ 
        inline virtual void set_fontMathRoman(const QString & __value)  
        {
            this->fontMathRoman = __value;
        } 
        /*! \brief returns the property fontMathRoman. 
            \details Description of the parameter fontMathRoman is: <BLOCKQUOTE>\copybrief fontMathRoman </BLOCKQUOTE> 
            \see fontMathRoman for more information */ 
        inline virtual QString get_fontMathRoman() const  
        {
            return this->fontMathRoman; 
        }
        /*! \brief sets the property fontMathSans to the specified \a __value. 
            \details Description of the parameter fontMathSans is: <BLOCKQUOTE>\copybrief fontMathSans </BLOCKQUOTE> 
            \see fontMathSans for more information */ 
        inline virtual void set_fontMathSans(const QString & __value)  
        {
            this->fontMathSans = __value;
        } 
        /*! \brief returns the property fontMathSans. 
            \details Description of the parameter fontMathSans is: <BLOCKQUOTE>\copybrief fontMathSans </BLOCKQUOTE> 
            \see fontMathSans for more information */ 
        inline virtual QString get_fontMathSans() const  
        {
            return this->fontMathSans; 
        }
        /*! \brief sets the property fontLatexPrefix to the specified \a __value. 
            \details Description of the parameter fontLatexPrefix is: <BLOCKQUOTE>\copybrief fontLatexPrefix </BLOCKQUOTE> 
            \see fontLatexPrefix for more information */ 
        inline virtual void set_fontLatexPrefix(const QString & __value)  
        {
            this->fontLatexPrefix = __value;
        } 
        /*! \brief returns the property fontLatexPrefix. 
            \details Description of the parameter fontLatexPrefix is: <BLOCKQUOTE>\copybrief fontLatexPrefix </BLOCKQUOTE> 
            \see fontLatexPrefix for more information */ 
        inline virtual QString get_fontLatexPrefix() const  
        {
            return this->fontLatexPrefix; 
        }
        /*! \brief sets the property fontLatexPostfix to the specified \a __value. 
            \details Description of the parameter fontLatexPostfix is: <BLOCKQUOTE>\copybrief fontLatexPostfix </BLOCKQUOTE> 
            \see fontLatexPostfix for more information */ 
        inline virtual void set_fontLatexPostfix(const QString & __value)  
        {
            this->fontLatexPostfix = __value;
        } 
        /*! \brief returns the property fontLatexPostfix. 
            \details Description of the parameter fontLatexPostfix is: <BLOCKQUOTE>\copybrief fontLatexPostfix </BLOCKQUOTE> 
            \see fontLatexPostfix for more information */ 
        inline virtual QString get_fontLatexPostfix() const  
        {
            return this->fontLatexPostfix; 
        }
        /*! \brief sets the property fontEncoding to the specified \a __value. 
            \details Description of the parameter fontEncoding is: <BLOCKQUOTE>\copybrief fontEncoding </BLOCKQUOTE> 
            \see fontEncoding for more information */ 
        inline virtual void set_fontEncoding(const MTfontEncoding & __value)  
        {
            this->fontEncoding = __value;
        } 
        /*! \brief returns the property fontEncoding. 
            \details Description of the parameter fontEncoding is: <BLOCKQUOTE>\copybrief fontEncoding </BLOCKQUOTE> 
            \see fontEncoding for more information */ 
        inline virtual MTfontEncoding get_fontEncoding() const  
        {
            return this->fontEncoding; 
        }
        /*! \brief returns the property useSTIXfonts. 
            \details Description of the parameter useSTIXfonts is:  <BLOCKQUOTE>\copybrief useSTIXfonts </BLOCKQUOTE>. 
            \see useSTIXfonts for more information */ 
        inline bool get_useSTIXfonts() const { 
            return this->useSTIXfonts; 
        }
        /*! \brief returns the property useXITSfonts. 
            \details Description of the parameter useXITSfonts is:  <BLOCKQUOTE>\copybrief useXITSfonts </BLOCKQUOTE>. 
            \see useXITSfonts for more information */ 
        inline bool get_useXITSfonts() const { 
            return this->useXITSfonts; 
        }
        /*! \brief sets the property brace_factor to the specified \a __value. 
            \details Description of the parameter brace_factor is: <BLOCKQUOTE>\copybrief brace_factor </BLOCKQUOTE> 
            \see brace_factor for more information */ 
        inline virtual void set_brace_factor(double __value)
        {
            this->brace_factor = __value;
        } 
        /*! \brief returns the property brace_factor. 
            \details Description of the parameter brace_factor is: <BLOCKQUOTE>\copybrief brace_factor </BLOCKQUOTE> 
            \see brace_factor for more information */ 
        inline virtual double get_brace_factor() const  
        {
            return this->brace_factor; 
        }
        /*! \brief sets the property subsuper_size_factor to the specified \a __value. 
            \details Description of the parameter subsuper_size_factor is: <BLOCKQUOTE>\copybrief subsuper_size_factor </BLOCKQUOTE> 
            \see subsuper_size_factor for more information */ 
        inline virtual void set_subsuper_size_factor(double __value)
        {
            this->subsuper_size_factor = __value;
        } 
        /*! \brief returns the property subsuper_size_factor. 
            \details Description of the parameter subsuper_size_factor is: <BLOCKQUOTE>\copybrief subsuper_size_factor </BLOCKQUOTE> 
            \see subsuper_size_factor for more information */ 
        inline virtual double get_subsuper_size_factor() const  
        {
            return this->subsuper_size_factor; 
        }
        /*! \brief sets the property italic_correction_factor to the specified \a __value. 
            \details Description of the parameter italic_correction_factor is: <BLOCKQUOTE>\copybrief italic_correction_factor </BLOCKQUOTE> 
            \see italic_correction_factor for more information */ 
        inline virtual void set_italic_correction_factor(double __value)
        {
            this->italic_correction_factor = __value;
        } 
        /*! \brief returns the property italic_correction_factor. 
            \details Description of the parameter italic_correction_factor is: <BLOCKQUOTE>\copybrief italic_correction_factor </BLOCKQUOTE> 
            \see italic_correction_factor for more information */ 
        inline virtual double get_italic_correction_factor() const  
        {
            return this->italic_correction_factor; 
        }
        /*! \brief sets the property operatorsubsuper_size_factor to the specified \a __value. 
            \details Description of the parameter operatorsubsuper_size_factor is: <BLOCKQUOTE>\copybrief operatorsubsuper_size_factor </BLOCKQUOTE> 
            \see operatorsubsuper_size_factor for more information */ 
        inline virtual void set_operatorsubsuper_size_factor(double __value)
        {
            this->operatorsubsuper_size_factor = __value;
        } 
        /*! \brief returns the property operatorsubsuper_size_factor. 
            \details Description of the parameter operatorsubsuper_size_factor is: <BLOCKQUOTE>\copybrief operatorsubsuper_size_factor </BLOCKQUOTE> 
            \see operatorsubsuper_size_factor for more information */ 
        inline virtual double get_operatorsubsuper_size_factor() const  
        {
            return this->operatorsubsuper_size_factor; 
        }
        /*! \brief sets the property mathoperator_width_factor to the specified \a __value. 
            \details Description of the parameter mathoperator_width_factor is: <BLOCKQUOTE>\copybrief mathoperator_width_factor </BLOCKQUOTE> 
            \see mathoperator_width_factor for more information */ 
        inline virtual void set_mathoperator_width_factor(double __value)
        {
            this->mathoperator_width_factor = __value;
        } 
        /*! \brief returns the property mathoperator_width_factor. 
            \details Description of the parameter mathoperator_width_factor is: <BLOCKQUOTE>\copybrief mathoperator_width_factor </BLOCKQUOTE> 
            \see mathoperator_width_factor for more information */ 
        inline virtual double get_mathoperator_width_factor() const  
        {
            return this->mathoperator_width_factor; 
        }
        /*! \brief sets the property super_shift_factor to the specified \a __value. 
            \details Description of the parameter super_shift_factor is: <BLOCKQUOTE>\copybrief super_shift_factor </BLOCKQUOTE> 
            \see super_shift_factor for more information */ 
        inline virtual void set_super_shift_factor(double __value)
        {
            this->super_shift_factor = __value;
        } 
        /*! \brief returns the property super_shift_factor. 
            \details Description of the parameter super_shift_factor is: <BLOCKQUOTE>\copybrief super_shift_factor </BLOCKQUOTE> 
            \see super_shift_factor for more information */ 
        inline virtual double get_super_shift_factor() const  
        {
            return this->super_shift_factor; 
        }
        /*! \brief sets the property sub_shift_factor to the specified \a __value. 
            \details Description of the parameter sub_shift_factor is: <BLOCKQUOTE>\copybrief sub_shift_factor </BLOCKQUOTE> 
            \see sub_shift_factor for more information */ 
        inline virtual void set_sub_shift_factor(double __value)
        {
            this->sub_shift_factor = __value;
        } 
        /*! \brief returns the property sub_shift_factor. 
            \details Description of the parameter sub_shift_factor is: <BLOCKQUOTE>\copybrief sub_shift_factor </BLOCKQUOTE> 
            \see sub_shift_factor for more information */ 
        inline virtual double get_sub_shift_factor() const  
        {
            return this->sub_shift_factor; 
        }
        /*! \brief sets the property brace_shrink_factor to the specified \a __value. 
            \details Description of the parameter brace_shrink_factor is: <BLOCKQUOTE>\copybrief brace_shrink_factor </BLOCKQUOTE> 
            \see brace_shrink_factor for more information */ 
        inline virtual void set_brace_shrink_factor(double __value)
        {
            this->brace_shrink_factor = __value;
        } 
        /*! \brief returns the property brace_shrink_factor. 
            \details Description of the parameter brace_shrink_factor is: <BLOCKQUOTE>\copybrief brace_shrink_factor </BLOCKQUOTE> 
            \see brace_shrink_factor for more information */ 
        inline virtual double get_brace_shrink_factor() const  
        {
            return this->brace_shrink_factor; 
        }
        /*! \brief sets the property underbrace_factor to the specified \a __value. 
            \details Description of the parameter underbrace_factor is: <BLOCKQUOTE>\copybrief underbrace_factor </BLOCKQUOTE> 
            \see underbrace_factor for more information */ 
        inline virtual void set_underbrace_factor(double __value)
        {
            this->underbrace_factor = __value;
        } 
        /*! \brief returns the property underbrace_factor. 
            \details Description of the parameter underbrace_factor is: <BLOCKQUOTE>\copybrief underbrace_factor </BLOCKQUOTE> 
            \see underbrace_factor for more information */ 
        inline virtual double get_underbrace_factor() const  
        {
            return this->underbrace_factor; 
        }
        /*! \brief sets the property underset_factor to the specified \a __value. 
            \details Description of the parameter underset_factor is: <BLOCKQUOTE>\copybrief underset_factor </BLOCKQUOTE> 
            \see underset_factor for more information */ 
        inline virtual void set_underset_factor(double __value)
        {
            this->underset_factor = __value;
        } 
        /*! \brief returns the property underset_factor. 
            \details Description of the parameter underset_factor is: <BLOCKQUOTE>\copybrief underset_factor </BLOCKQUOTE> 
            \see underset_factor for more information */ 
        inline virtual double get_underset_factor() const  
        {
            return this->underset_factor; 
        }
        /*! \brief sets the property frac_factor to the specified \a __value. 
            \details Description of the parameter frac_factor is: <BLOCKQUOTE>\copybrief frac_factor </BLOCKQUOTE> 
            \see frac_factor for more information */ 
        inline virtual void set_frac_factor(double __value)
        {
            this->frac_factor = __value;
        } 
        /*! \brief returns the property frac_factor. 
            \details Description of the parameter frac_factor is: <BLOCKQUOTE>\copybrief frac_factor </BLOCKQUOTE> 
            \see frac_factor for more information */ 
        inline virtual double get_frac_factor() const  
        {
            return this->frac_factor; 
        }
        /*! \brief sets the property frac_shift_factor to the specified \a __value. 
            \details Description of the parameter frac_shift_factor is: <BLOCKQUOTE>\copybrief frac_shift_factor </BLOCKQUOTE> 
            \see frac_shift_factor for more information */ 
        inline virtual void set_frac_shift_factor(double __value)
        {
            this->frac_shift_factor = __value;
        } 
        /*! \brief returns the property frac_shift_factor. 
            \details Description of the parameter frac_shift_factor is: <BLOCKQUOTE>\copybrief frac_shift_factor </BLOCKQUOTE> 
            \see frac_shift_factor for more information */ 
        inline virtual double get_frac_shift_factor() const  
        {
            return this->frac_shift_factor; 
        }
        /*! \brief sets the property brace_y_shift_factor to the specified \a __value. 
            \details Description of the parameter brace_y_shift_factor is: <BLOCKQUOTE>\copybrief brace_y_shift_factor </BLOCKQUOTE> 
            \see brace_y_shift_factor for more information */ 
        inline virtual void set_brace_y_shift_factor(double __value)
        {
            this->brace_y_shift_factor = __value;
        } 
        /*! \brief returns the property brace_y_shift_factor. 
            \details Description of the parameter brace_y_shift_factor is: <BLOCKQUOTE>\copybrief brace_y_shift_factor </BLOCKQUOTE> 
            \see brace_y_shift_factor for more information */ 
        inline virtual double get_brace_y_shift_factor() const  
        {
            return this->brace_y_shift_factor; 
        }
        /*! \brief sets the property decoration_height_factor to the specified \a __value. 
            \details Description of the parameter decoration_height_factor is: <BLOCKQUOTE>\copybrief decoration_height_factor </BLOCKQUOTE> 
            \see decoration_height_factor for more information */ 
        inline virtual void set_decoration_height_factor(double __value)
        {
            this->decoration_height_factor = __value;
        } 
        /*! \brief returns the property decoration_height_factor. 
            \details Description of the parameter decoration_height_factor is: <BLOCKQUOTE>\copybrief decoration_height_factor </BLOCKQUOTE> 
            \see decoration_height_factor for more information */ 
        inline virtual double get_decoration_height_factor() const  
        {
            return this->decoration_height_factor; 
        }
        /*! \brief sets the property expensiveRendering to the specified \a __value. 
            \details Description of the parameter expensiveRendering is: <BLOCKQUOTE>\copybrief expensiveRendering </BLOCKQUOTE> 
            \see expensiveRendering for more information */ 
        inline virtual void set_expensiveRendering(bool __value)
        {
            this->expensiveRendering = __value;
        } 
        /*! \brief returns the property expensiveRendering. 
            \details Description of the parameter expensiveRendering is: <BLOCKQUOTE>\copybrief expensiveRendering </BLOCKQUOTE> 
            \see expensiveRendering for more information */ 
        inline virtual bool get_expensiveRendering() const  
        {
            return this->expensiveRendering; 
        }
        /*! \brief sets the property useUnparsed to the specified \a __value. 
            \details Description of the parameter useUnparsed is: <BLOCKQUOTE>\copybrief useUnparsed </BLOCKQUOTE> 
            \see useUnparsed for more information */ 
        inline virtual void set_useUnparsed(bool __value)
        {
            this->useUnparsed = __value;
        } 
        /*! \brief returns the property useUnparsed. 
            \details Description of the parameter useUnparsed is: <BLOCKQUOTE>\copybrief useUnparsed </BLOCKQUOTE> 
            \see useUnparsed for more information */ 
        inline virtual bool get_useUnparsed() const  
        {
            return this->useUnparsed; 
        }
        /*! \brief returns the property error_list. 
            \details Description of the parameter error_list is:  <BLOCKQUOTE>\copybrief error_list </BLOCKQUOTE>. 
            \see error_list for more information */ 
        inline QStringList get_error_list() const { 
            return this->error_list; 
        }

    protected:

        /** \brief the available fonts */
        enum MTenvironmentFont { MTEroman, MTEsans, MTEtypewriter, MTEscript, MTEblackboard, MTEcaligraphic };

        /** \brief describes the current drawing environment (base fontname ...) */
        struct MTenvironment {
            MTenvironment();
            QColor color;
            MTenvironmentFont font;
            double fontSize;
            bool bold;
            bool italic;
            bool smallCaps;
            bool underlined;
            bool overline;
            bool strike;
            bool insideMath;


            QFont getFont(JKQTmathText* parent) const;
            QString toHtmlStart(MTenvironment defaultEv) const;
            QString toHtmlAfter(MTenvironment defaultEv) const;
        };


    public:
        /** \brief subclass representing one node in the syntax tree */
        class MTnode {
            public:
                MTnode(JKQTmathText* parent);
                virtual ~MTnode();
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos)=0;
                void getSize(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                /** \brief draw the contents at the designated position. returns the x position which to use for the next part of the text */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv)=0;
                /** \brief returns true if node is subscript or superscript node */
                virtual bool isSubOrSuper() { return false; }
                /** \brief convert node to HTML and returns \c true on success */
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);

                /*! \brief returns the property drawBoxes. 
                    \details Description of the parameter drawBoxes is:  <BLOCKQUOTE>\copybrief drawBoxes </BLOCKQUOTE>. 
                    \see drawBoxes for more information */ 
                inline bool get_drawBoxes() const { 
                    return this->drawBoxes; 
                }
                virtual void set_drawBoxes(bool draw);
                virtual QString getTypeName() const;
            protected:
                JKQTmathText* parent;
                bool drawBoxes;
                void doDrawBoxes(QPainter& painter, double x, double y, JKQTmathText::MTenvironment currentEv);
        };

        /** \brief subclass representing one text node in the syntax tree */
        class MTtextNode: public MTnode {
            public:
                MTtextNode(JKQTmathText* parent, QString text, bool addWhitespace, bool stripInnerWhitepace=false);
                virtual ~MTtextNode();
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                /*! \brief returns the property text. 
                    \details Description of the parameter text is:  <BLOCKQUOTE>\copybrief text </BLOCKQUOTE>. 
                    \see text for more information */ 
                inline QString get_text() const { 
                    return this->text; 
                }
                virtual QString getTypeName() const override ;
            protected:
                QString text;
                virtual QString textTransform(const QString& text, JKQTmathText::MTenvironment currentEv, bool forSize=false);
        };

        /** \brief subclass representing one text node in the syntax tree */
        class MTplainTextNode: public MTtextNode {
            public:
                MTplainTextNode(JKQTmathText* parent, QString text, bool addWhitespace, bool stripInnerWhitepace=false);
                virtual QString getTypeName() const;
            protected:
                virtual QString textTransform(const QString& text, JKQTmathText::MTenvironment currentEv, bool forSize=false);
        };
        /** \brief subclass representing one whitepsace node in the syntax tree */
        class MTwhitespaceNode: public MTtextNode {
            public:
                MTwhitespaceNode(JKQTmathText* parent);
                virtual ~MTwhitespaceNode();
                virtual QString getTypeName() const;
                /** \brief convert node to HTML and returns \c true on success */
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
        };

        /** \brief subclass representing one symbol (e.g. \c \\alpha , \c \\cdot ...) node in the syntax tree */
        class MTsymbolNode: public MTnode {
            public:
                MTsymbolNode(JKQTmathText* parent, QString name, bool addWhitespace);
                virtual ~MTsymbolNode();
                virtual QString getTypeName() const;
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                /*! \brief returns the property symbolName. 
                    \details Description of the parameter symbolName is:  <BLOCKQUOTE>\copybrief symbolName </BLOCKQUOTE>. 
                    \see symbolName for more information */ 
                inline QString get_symbolName() const { 
                    return this->symbolName; 
                }
            protected:
                /** \brief this string will be sent to the drawText method with properly set fonts */
                QString symbol;
                /** \brief the symbol name supplied to the constructor */
                QString symbolName;
                /** \brief these fonts may be used for symbols */
                enum symbolFont { MTSFdefault, MTSFsymbol, MTSFgreek, MTSFbraces, MTSFintegrals, MTSFcaligraphic, MTSFblackboard };
                /** \brief changes the font name according to a given symbolFont value */
                QFont getFontName(symbolFont f, QFont& fi);
                /** \brief magnification factor for the font size */
                symbolFont font;
                /** \brief magnification factor for the font size */
                double fontFactor;
                /** \brief 0: leave italic setting as is, >0: set italic, <0 set italic to false */
                char italic;
                /** \brief 0: leave bold setting as is, >0: set bold, <0 set bold to false */
                char bold;
                /** \brief this corrects the y position of a symbol: draws at y <- y+ height*yfactor) */
                double yfactor;
                /** \brief indicates whether to draw a bar (like for \c \\hbar ) */
                bool drawBar;
                bool heightIsAscent;
                bool exactAscent;
                bool extendWidthInMathmode;
        };

        /** \brief subclass representing a list of nodes in the syntax tree */
        class MTlistNode: public MTnode {
            public:
                MTlistNode(JKQTmathText* parent);
                virtual ~MTlistNode();
                virtual QString getTypeName() const;
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                void addNode(MTnode* n) { nodes.append(n); }
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
                /*! \brief returns the property nodes. 
                    \details Description of the parameter nodes is:  <BLOCKQUOTE>\copybrief nodes </BLOCKQUOTE>. 
                    \see nodes for more information */ 
                inline QList<MTnode*> get_nodes() const { 
                    return this->nodes; 
                }
            protected:
                QList<MTnode*> nodes;
                QSet<QString> subsupOperations;
        };

        /** \brief subclass representing an instruction node with exactly one argument in the syntax tree */
        class MTinstruction1Node: public MTnode {
            public:
                MTinstruction1Node(JKQTmathText* parent, QString name, MTnode* child, const QStringList& parameters=QStringList());
                virtual ~MTinstruction1Node();
                virtual QString getTypeName() const;
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                /** \brief convert node to HTML and returns \c true on success */
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
                /*! \brief returns the property child. 
                    \details Description of the parameter child is:  <BLOCKQUOTE>\copybrief child </BLOCKQUOTE>. 
                    \see child for more information */ 
                inline MTnode* get_child() const { 
                    return this->child; 
                }
                /*! \brief returns the property name. 
                    \details Description of the parameter name is:  <BLOCKQUOTE>\copybrief name </BLOCKQUOTE>. 
                    \see name for more information */ 
                inline QString get_name() const { 
                    return this->name; 
                }
                /*! \brief returns the property parameters. 
                    \details Description of the parameter parameters is:  <BLOCKQUOTE>\copybrief parameters </BLOCKQUOTE>. 
                    \see parameters for more information */ 
                inline QStringList get_parameters() const { 
                    return this->parameters; 
                }
            protected:
                bool setupMTenvironment(JKQTmathText::MTenvironment &ev);

                MTnode* child;
                QString name;
                QStringList parameters;
        };



        /** \brief subclass representing an subscript node with exactly one argument in the syntax tree */
        class MTsubscriptNode: public MTnode {
            public:
                MTsubscriptNode(JKQTmathText* parent, MTnode* child);
                virtual ~MTsubscriptNode();
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                /** \brief returns true if node is subscript or superscript node */
                virtual bool isSubOrSuper() ;
                virtual QString getTypeName() const;
                /*! \brief returns the property child. 
                    \details Description of the parameter child is:  <BLOCKQUOTE>\copybrief child </BLOCKQUOTE>. 
                    \see child for more information */ 
                inline MTnode* get_child() const { 
                    return this->child; 
                }
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
            protected:
                MTnode* child;
        };

        /** \brief subclass representing an superscript node with exactly one argument in the syntax tree */
        class MTsuperscriptNode: public MTnode {
            public:
                MTsuperscriptNode(JKQTmathText* parent, MTnode* child);
                virtual ~MTsuperscriptNode();
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                /** \brief returns true if node is subscript or superscript node */
                virtual bool isSubOrSuper();
                /*! \brief returns the property child. 
                    \details Description of the parameter child is:  <BLOCKQUOTE>\copybrief child </BLOCKQUOTE>. 
                    \see child for more information */ 
                inline MTnode* get_child() const { 
                    return this->child; 
                }
                virtual QString getTypeName() const;
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
            protected:
                MTnode* child;
        };

        /** \brief subclass representing a brace node  */
        class MTbraceNode: public MTnode {
            public:
                MTbraceNode(JKQTmathText* parent, QString openbrace, QString closebrace, MTnode* child, bool showRightBrace=true);
                virtual ~MTbraceNode();
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
                virtual QString getTypeName() const;
                /*! \brief returns the property child. 
                    \details Description of the parameter child is:  <BLOCKQUOTE>\copybrief child </BLOCKQUOTE>. 
                    \see child for more information */ 
                inline MTnode* get_child() const { 
                    return this->child; 
                }
                /*! \brief returns the property openbrace. 
                    \details Description of the parameter openbrace is:  <BLOCKQUOTE>\copybrief openbrace </BLOCKQUOTE>. 
                    \see openbrace for more information */ 
                inline QString get_openbrace() const { 
                    return this->openbrace; 
                }
                /*! \brief returns the property closebrace. 
                    \details Description of the parameter closebrace is:  <BLOCKQUOTE>\copybrief closebrace </BLOCKQUOTE>. 
                    \see closebrace for more information */ 
                inline QString get_closebrace() const { 
                    return this->closebrace; 
                }
                /*! \brief returns the property showRightBrace. 
                    \details Description of the parameter showRightBrace is:  <BLOCKQUOTE>\copybrief showRightBrace </BLOCKQUOTE>. 
                    \see showRightBrace for more information */ 
                inline bool get_showRightBrace() const { 
                    return this->showRightBrace; 
                }
            protected:
                MTnode* child;
                QString openbrace;
                QString closebrace;
                bool showRightBrace;

                void getBraceWidth(QPainter& painter, MTenvironment currentEv, double baselineHeight, double overallHeight, double& bracewidth, double& braceheight);
        };


        /** \brief subclass representing a sqrt node  */
        class MTsqrtNode: public MTnode {
            public:
                MTsqrtNode(JKQTmathText* parent, MTnode* child, int degree=2);
                virtual ~MTsqrtNode();
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
                virtual QString getTypeName() const ;
                /*! \brief returns the property child. 
                    \details Description of the parameter child is:  <BLOCKQUOTE>\copybrief child </BLOCKQUOTE>. 
                    \see child for more information */ 
                inline MTnode* get_child() const { 
                    return this->child; 
                }
                /*! \brief returns the property degree. 
                    \details Description of the parameter degree is:  <BLOCKQUOTE>\copybrief degree </BLOCKQUOTE>. 
                    \see degree for more information */ 
                inline int get_degree() const { 
                    return this->degree; 
                }
            protected:
                MTnode* child;
                int degree;
        };

        enum MTfracMode {
            MTFMfrac,
            MTFMdfrac,
            MTFMtfrac,
            MTFMunderbrace,
            MTFMoverbrace,
            MTFMstackrel,
            MTFMunderset,
            MTFMoverset
        };

        static QString fracModeToString(MTfracMode mode);

        /** \brief subclass representing a \\frac node  */
        class MTfracNode: public MTnode {
            public:
                MTfracNode(JKQTmathText* parent, MTnode* child_top, MTnode* child_bottom, MTfracMode mode);
                virtual ~MTfracNode();
                virtual QString getTypeName() const ;
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
                /*! \brief returns the property child1. 
                    \details Description of the parameter child1 is:  <BLOCKQUOTE>\copybrief child1 </BLOCKQUOTE>. 
                    \see child1 for more information */ 
                inline MTnode* get_child1() const { 
                    return this->child1; 
                }
                /*! \brief returns the property child2. 
                    \details Description of the parameter child2 is:  <BLOCKQUOTE>\copybrief child2 </BLOCKQUOTE>. 
                    \see child2 for more information */ 
                inline MTnode* get_child2() const { 
                    return this->child2; 
                }
                /*! \brief returns the property mode. 
                    \details Description of the parameter mode is:  <BLOCKQUOTE>\copybrief mode </BLOCKQUOTE>. 
                    \see mode for more information */ 
                inline MTfracMode get_mode() const { 
                    return this->mode; 
                }
            protected:
                MTnode* child1;
                MTnode* child2;
                MTfracMode mode;
        };

        /** \brief subclass representing a \\begin{matrix} node  */
        class MTmatrixNode: public MTnode {
            public:
                MTmatrixNode(JKQTmathText* parent, QVector<QVector<MTnode*> > children);
                virtual ~MTmatrixNode();
                virtual QString getTypeName() const;
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                /*! \brief returns the property children. 
                    \details Description of the parameter children is:  <BLOCKQUOTE>\copybrief children </BLOCKQUOTE>. 
                    \see children for more information */ 
                inline QVector<QVector<MTnode*> > get_children() const { 
                    return this->children; 
                }
                /*! \brief returns the property columns. 
                    \details Description of the parameter columns is:  <BLOCKQUOTE>\copybrief columns </BLOCKQUOTE>. 
                    \see columns for more information */ 
                inline int get_columns() const { 
                    return this->columns; 
                }
                /*! \brief returns the property lines. 
                    \details Description of the parameter lines is:  <BLOCKQUOTE>\copybrief lines </BLOCKQUOTE>. 
                    \see lines for more information */ 
                inline int get_lines() const { 
                    return this->lines; 
                }
            protected:
                virtual void set_drawBoxes(bool draw);
                QVector<QVector<MTnode*> > children;
                int columns;
                int lines;
        };


        enum MTdecoration {
            MTDvec,
            MTDhat,
            MTDdot,
            MTDddot,
            MTDbar,
            MTDarrow,
            MTDoverline,
            MTDdoubleoverline,
            MTDunderline,
            MTDdoubleunderline,
            MTDtilde
        };
        static QString decorationToString(MTdecoration mode);

        /** \brief subclass representing a decorated text m (e.g. \c \\vec \c \\hat ...) node  */
        class MTdecoratedNode: public MTnode {
            public:
                MTdecoratedNode(JKQTmathText* parent, MTdecoration decoration, MTnode* child);
                virtual ~MTdecoratedNode();
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv);
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);
                virtual void set_drawBoxes(bool draw);
                virtual QString getTypeName() const ;
                /*! \brief returns the property child. 
                    \details Description of the parameter child is:  <BLOCKQUOTE>\copybrief child </BLOCKQUOTE>. 
                    \see child for more information */ 
                inline MTnode* get_child() const { 
                    return this->child; 
                }
                /*! \brief returns the property decoration. 
                    \details Description of the parameter decoration is:  <BLOCKQUOTE>\copybrief decoration </BLOCKQUOTE>. 
                    \see decoration for more information */ 
                inline MTdecoration get_decoration() const { 
                    return this->decoration; 
                }
            protected:
                MTnode* child;
                MTdecoration decoration;
        };

    protected:


        /** \brief font color */
        QColor fontColor;
        /*! \brief default value for property fontColor.
            \see fontColor for more information */
        QColor def_fontColor;
        /** \brief base font size in points */
        double fontSize;
        /*! \brief default value for property fontSize.
            \see fontSize for more information */
        double def_fontSize;
        /** \brief roman font */
        QString fontRoman;
        /*! \brief default value for property fontRoman.
            \see fontRoman for more information */
        QString def_fontRoman;
        /** \brief sans-serif font */
        QString fontSans;
        /*! \brief default value for property fontSans.
            \see fontSans for more information */
        QString def_fontSans;
        /** \brief typewriter font */
        QString fontTypewriter;
        /*! \brief default value for property fontTypewriter.
            \see fontTypewriter for more information */
        QString def_fontTypewriter;
        /** \brief script font */
        QString fontScript;
        /*! \brief default value for property fontScript.
            \see fontScript for more information */
        QString def_fontScript;
        /** \brief greek font */
        QString fontGreek;
        /*! \brief default value for property fontGreek.
            \see fontGreek for more information */
        QString def_fontGreek;
        /** \brief symbol font, used for math symbols. */
        QString fontSymbol;
        /*! \brief default value for property fontSymbol.
            \see fontSymbol for more information */
        QString def_fontSymbol;
        /** \brief symbol font, used for braces in math mode. */
        QString fontBraces;
        /*! \brief default value for property fontBraces.
            \see fontBraces for more information */
        QString def_fontBraces;
        /** \brief symbol font, used for integrals in math mode. */
        QString fontIntegrals;
        /*! \brief default value for property fontIntegrals.
            \see fontIntegrals for more information */
        QString def_fontIntegrals;
        /** \brief font used for caligraphic symbols (escepcially in math environments) */
        QString fontCaligraphic;
        /*! \brief default value for property fontCaligraphic.
            \see fontCaligraphic for more information */
        QString def_fontCaligraphic;
        /** \brief font used for blackboard (double-stroke) symbols (escepcially in math environments) */
        QString fontBlackboard;
        /*! \brief default value for property fontBlackboard.
            \see fontBlackboard for more information */
        QString def_fontBlackboard;
        /** \brief roman font for math environment */
        QString fontMathRoman;
        /*! \brief default value for property fontMathRoman.
            \see fontMathRoman for more information */
        QString def_fontMathRoman;
        /** \brief sans font for math environment */
        QString fontMathSans;
        /*! \brief default value for property fontMathSans.
            \see fontMathSans for more information */
        QString def_fontMathSans;
        /** \brief prefix for LaTeX fonts */
        QString fontLatexPrefix;
        /*! \brief default value for property fontLatexPrefix.
            \see fontLatexPrefix for more information */
        QString def_fontLatexPrefix;
        /** \brief postfix for LaTeX fonts */
        QString fontLatexPostfix;
        /*! \brief default value for property fontLatexPostfix.
            \see fontLatexPostfix for more information */
        QString def_fontLatexPostfix;
        /** \brief specifies the encoding of special character fonts (default is \c MTFEwinSymbol ) */
        MTfontEncoding fontEncoding;
        /*! \brief default value for property fontEncoding.
            \see fontEncoding for more information */
        MTfontEncoding def_fontEncoding;
        /** \brief resizing factor for braces in math mode */
        double brace_factor;
        /*! \brief default value for property brace_factor.
            \see brace_factor for more information */
        double def_brace_factor;
        /** \brief shrinking the width of braces in math mode 0: reduce to 0 pixel width, 1: leave unchanged*/
        double brace_shrink_factor;
        /*! \brief default value for property brace_shrink_factor.
            \see brace_shrink_factor for more information */
        double def_brace_shrink_factor;
        /** \brief resizing factor for font size in sub-/superscript */
        double subsuper_size_factor;
        /*! \brief default value for property subsuper_size_factor.
            \see subsuper_size_factor for more information */
        double def_subsuper_size_factor;
        /** \brief fraction of a whitespace by which to shift a sub-/superscript left/right when the previous text is italic */
        double italic_correction_factor;
        /*! \brief default value for property italic_correction_factor.
            \see italic_correction_factor for more information */
        double def_italic_correction_factor;
        /** \brief like subsuper_size_factor, but for operators (\\sum, \\int) where the text is placed above/below the symbol */
        double operatorsubsuper_size_factor;
        /*! \brief default value for property operatorsubsuper_size_factor.
            \see operatorsubsuper_size_factor for more information */
        double def_operatorsubsuper_size_factor;
        /** \brief factor, used to extend the size of an operator in math mode */
        double mathoperator_width_factor;
        /*! \brief default value for property mathoperator_width_factor.
            \see mathoperator_width_factor for more information */
        double def_mathoperator_width_factor;
        /** \brief relative shift of text in superscript to normal text:
         *         0= baseline kept, 1: baseline shifted to top of normal text */
        double super_shift_factor;
        /*! \brief default value for property super_shift_factor.
            \see super_shift_factor for more information */
        double def_super_shift_factor;
        /** \brief relative shift of text in subscript to normal text:
         *         0= baseline kept, 1: baseline shifted to bottom of normal text */
        double sub_shift_factor;
        /*! \brief default value for property sub_shift_factor.
            \see sub_shift_factor for more information */
        double def_sub_shift_factor;
        /** \brief indicates whether to use STIX fonts or not */
        bool useSTIXfonts;
        /*! \brief default value for property useSTIXfonts.
            \see useSTIXfonts for more information */
        bool def_useSTIXfonts;
        /** \brief indicates whether to use XITS fonts or not */
        bool useXITSfonts;
        /*! \brief default value for property useXITSfonts.
            \see useXITSfonts for more information */
        bool def_useXITSfonts;
        /** \brief indicates whether to use XITS fonts or not */
        bool useASANAfonts;
        /*! \brief default value for property useASANAfonts.
            \see useASANAfonts for more information */
        bool def_useASANAfonts;
        /** \brief scaling factor for font of nominator and denominator of a fraction */
        double frac_factor;
        /*! \brief default value for property frac_factor.
            \see frac_factor for more information */
        double def_frac_factor;
        /** \brief shift of denominator/nummerator away from central line of a frac */
        double frac_shift_factor;
        /*! \brief default value for property frac_shift_factor.
            \see frac_shift_factor for more information */
        double def_frac_shift_factor;
        /** \brief scaling factor for font of underbrace/overbrace text */
        double underbrace_factor;
        /*! \brief default value for property underbrace_factor.
            \see underbrace_factor for more information */
        double def_underbrace_factor;
        /** \brief scaling factor for font of underset/overset text */
        double underset_factor;
        /*! \brief default value for property underset_factor.
            \see underset_factor for more information */
        double def_underset_factor;
        /** \brief fraction of the brace ascent that the brace is shifted downwards, when scaled */
        double brace_y_shift_factor;
        /*! \brief default value for property brace_y_shift_factor.
            \see brace_y_shift_factor for more information */
        double def_brace_y_shift_factor;
        /** \brief size of the decorations (dot, tilde, ...), as fractio of the baselineheight */
        double decoration_height_factor;
        /*! \brief default value for property decoration_height_factor.
            \see decoration_height_factor for more information */
        double def_decoration_height_factor;
        /** \brief switches on some options that will grant better rendering at the expense of higher time consumption */
        bool expensiveRendering;
        /*! \brief default value for property expensiveRendering.
            \see expensiveRendering for more information */
        bool def_expensiveRendering;
        /** \brief a list that will be filled with error messages while parsing, if any error occur */
        QStringList error_list;
        /** \brief used by the parser. This is used to implement brace pairs with \\right. */
        bool showRightBrace;

        /** \brief the result of parsing the last string supplied to the object via parse() */
        MTnode* parsedNode;
        /** \brief a tree containing the unparsed text as a single node */
        MTnode* unparsedNode;
        /** \brief if true, the unparsedNode is drawn */
        bool useUnparsed;

        inline MTnode* getTree() const {
            if (useUnparsed) return unparsedNode;
            return parsedNode;
        }

        /** \brief the token types that may arrise in the string */
        enum tokenType {
            MTTnone,
            MTTtext,
            MTTinstruction,
            MTTunderscore,
            MTThat,
            MTTdollar,
            MTTopenbrace,
            MTTclosebrace,
            MTTwhitespace,
            MTTampersand
        };

        /** \brief tokenizer for the LaTeX parser */
        tokenType getToken();
        /** \brief parse a LaTeX string */
        MTnode* parseLatexString(bool get, QString quitOnClosingBrace=QString(""), QString quitOnEnvironmentEnd=QString(""));
        /** \brief parse a LaTeX math environment */
        MTnode* parseMath(bool get);

        /** \brief used by the tokenizer. type of the current token */
        tokenType currentToken;
        /** \brief used by the tokenizer. Name of the current token, id applicable */
        QString currentTokenName;
        /** \brief used by the tokenizer. Points to the currently read character in parseString */
        int currentTokenID;
        /** \brief used by the tokenizer. The string to be parsed */
        QString parseString;
        /** \brief used by the parser. indicates whether we are in a math environment */
        bool parsingMathEnvironment;


    public:
        /*! \brief returns the property parsedNode. 
            \details Description of the parameter parsedNode is:  <BLOCKQUOTE>\copybrief parsedNode </BLOCKQUOTE>. 
            \see parsedNode for more information */ 
        inline MTnode* get_parsedNode() const { 
            return this->parsedNode; 
        }

        struct tbrData {
            explicit tbrData(const QFont& f, const QString& text, QPaintDevice *pd);
            QFontMetricsF fm;
            QString text;
            QRectF tbr;
            QFont f;
            int ldpiX, ldpiY, pdpiX, pdpiY;
            //QPaintDevice *pd;

            bool operator==(const tbrData& other) const;
        };
        struct tbrDataH {
            explicit tbrDataH(const QFont& f, const QString& text, QPaintDevice *pd);
            QString text;
            QFont f;
            int ldpiX, ldpiY, pdpiX, pdpiY;

            bool operator==(const tbrDataH& other) const;
        };
        static QList<JKQTmathText::tbrData> tbrs;
        static QHash<JKQTmathText::tbrDataH, QRectF> tbrh;
        static QRectF getTBR(const QFont &fm, const QString& text,  QPaintDevice *pd);
};



inline uint qHash(const JKQTmathText::tbrDataH& data) {
    return qHash(data.f.family())+qHash(data.text);
}



class LIB_EXPORT JKQTMathTextLabel: public QLabel {
        Q_OBJECT
    public:
        explicit JKQTMathTextLabel(QWidget* parent=nullptr);
        virtual ~JKQTMathTextLabel();

        JKQTmathText* getMathText() const;
        void setMath(const QString& text, bool doRepaint=true);
    protected:
        JKQTmathText* m_mathText;
        QString lastText;
        QPixmap buffer;
        bool repaintDo;
        void internalPaint();

        void paintEvent(QPaintEvent * event);
};


#endif // JKQTMATHTEXT_H









