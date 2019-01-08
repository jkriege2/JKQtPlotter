/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>)
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

/**
 * \defgroup jkqtmathtext JKQtMathText LaTeX-Renderer for Qt
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

/**
 * \brief create a property variable and a default variable for it. Also creates a doxygen comment for the default variable
 * \ingroup jkqtmathtext
 */
#define JKQTMTPROPERTY(type,varname) \
  type varname; \
  /** \brief default value for property property varname. \see varname for more information */ \
  type def_##varname;

/**
 * \brief saves the given property (for which also a def_property exists) into the given settings object
 * \ingroup jkqtmathtext
 */
#define JKQTMTPROPERTYsave(settings, group, var, varname) \
    if (var!=def_##var) settings.setValue(group+varname, var);
/**
 * \brief loads the given property from the given settings object
 * \ingroup jkqtmathtext
 */
#define JKQTMTPROPERTYload(settings, group, var, varname, varconvert) \
    var=settings.value(group+varname, var).varconvert;

/** \brief initialized Qt-ressources necessary for JKQtMathText */
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

        JKQTPGET_SET_MACRO(QColor, fontColor)
        JKQTPGET_SET_MACRO(double, fontSize)
        JKQTPGET_SET_MACRO(QString, fontRoman)
        JKQTPGET_SET_MACRO(QString, fontSans)
        JKQTPGET_SET_MACRO(QString, fontTypewriter)
        JKQTPGET_SET_MACRO(QString, fontScript)
        JKQTPGET_SET_MACRO(QString, fontGreek)
        JKQTPGET_SET_MACRO(QString, fontSymbol)
        JKQTPGET_SET_MACRO(QString, fontBraces)
        JKQTPGET_SET_MACRO(QString, fontIntegrals)
        JKQTPGET_SET_MACRO(QString, fontCaligraphic)
        JKQTPGET_SET_MACRO(QString, fontBlackboard)
        JKQTPGET_SET_MACRO(QString, fontMathRoman)
        JKQTPGET_SET_MACRO(QString, fontMathSans)
        JKQTPGET_SET_MACRO(QString, fontLatexPrefix)
        JKQTPGET_SET_MACRO(QString, fontLatexPostfix)
        JKQTPGET_SET_MACRO(MTfontEncoding, fontEncoding)
        JKQTPGET_MACRO(bool, useSTIXfonts)
        JKQTPGET_MACRO(bool, useXITSfonts)
        JKQTPGET_SET_MACRO(double, brace_factor)
        JKQTPGET_SET_MACRO(double, subsuper_size_factor)
        JKQTPGET_SET_MACRO(double, italic_correction_factor)
        JKQTPGET_SET_MACRO(double, operatorsubsuper_size_factor)
        JKQTPGET_SET_MACRO(double, mathoperator_width_factor)
        JKQTPGET_SET_MACRO(double, super_shift_factor)
        JKQTPGET_SET_MACRO(double, sub_shift_factor)
        JKQTPGET_SET_MACRO(double, brace_shrink_factor)
        JKQTPGET_SET_MACRO(double, underbrace_factor)
        JKQTPGET_SET_MACRO(double, underset_factor)
        JKQTPGET_SET_MACRO(double, frac_factor)
        JKQTPGET_SET_MACRO(double, frac_shift_factor)
        JKQTPGET_SET_MACRO(double, brace_y_shift_factor)
        JKQTPGET_SET_MACRO(double, decoration_height_factor)
        JKQTPGET_SET_MACRO(bool, expensiveRendering)
        JKQTPGET_SET_MACRO(bool, useUnparsed)
        JKQTPGET_MACRO(QStringList, error_list)

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
                virtual ~MTnode() { };
                virtual void getSizeInternal(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos)=0;
                void getSize(QPainter& painter, MTenvironment currentEv, double& width, double& baselineHeight, double& overallHeight, double& strikeoutPos);
                /** \brief draw the contents at the designated position. returns the x position which to use for the next part of the text */
                virtual double draw(QPainter& painter, double x, double y, MTenvironment currentEv)=0;
                /** \brief returns true if node is subscript or superscript node */
                virtual bool isSubOrSuper() { return false; }
                /** \brief convert node to HTML and returns \c true on success */
                virtual bool toHtml(QString& html, JKQTmathText::MTenvironment currentEv, JKQTmathText::MTenvironment defaultEv);

                JKQTPGET_MACRO(bool, drawBoxes)
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
                JKQTPGET_MACRO(QString, text)
                virtual QString getTypeName() const ;
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
                JKQTPGET_MACRO(QString, symbolName)
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
                JKQTPGET_MACRO(QList<MTnode*>, nodes)
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
                JKQTPGET_MACRO(MTnode*, child)
                JKQTPGET_MACRO(QString, name)
                JKQTPGET_MACRO(QStringList, parameters)
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
                JKQTPGET_MACRO(MTnode*, child)
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
                JKQTPGET_MACRO(MTnode*, child)
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
                JKQTPGET_MACRO(MTnode*, child)
                JKQTPGET_MACRO(QString, openbrace)
                JKQTPGET_MACRO(QString, closebrace)
                JKQTPGET_MACRO(bool, showRightBrace)
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
                JKQTPGET_MACRO(MTnode*, child)
                JKQTPGET_MACRO(int, degree)
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
                JKQTPGET_MACRO(MTnode*, child1)
                JKQTPGET_MACRO(MTnode*, child2)
                JKQTPGET_MACRO(MTfracMode, mode)
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
                JKQTPGET_MACRO(QVector<QVector<MTnode*> >, children)
                JKQTPGET_MACRO(int, columns)
                JKQTPGET_MACRO(int, lines)
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
                JKQTPGET_MACRO(MTnode*, child)
                JKQTPGET_MACRO(MTdecoration, decoration)
            protected:
                MTnode* child;
                MTdecoration decoration;
        };

    protected:


        /** \brief font color */
        JKQTMTPROPERTY(QColor, fontColor)
        /** \brief base font size in points */
        JKQTMTPROPERTY(double, fontSize)
        /** \brief roman font */
        JKQTMTPROPERTY(QString, fontRoman)
        /** \brief sans-serif font */
        JKQTMTPROPERTY(QString, fontSans)
        /** \brief typewriter font */
        JKQTMTPROPERTY(QString, fontTypewriter)
        /** \brief script font */
        JKQTMTPROPERTY(QString, fontScript)
        /** \brief greek font */
        JKQTMTPROPERTY(QString, fontGreek)
        /** \brief symbol font, used for math symbols. */
        JKQTMTPROPERTY(QString, fontSymbol)
        /** \brief symbol font, used for braces in math mode. */
        JKQTMTPROPERTY(QString, fontBraces)
        /** \brief symbol font, used for integrals in math mode. */
        JKQTMTPROPERTY(QString, fontIntegrals)
        /** \brief font used for caligraphic symbols (escepcially in math environments) */
        JKQTMTPROPERTY(QString, fontCaligraphic)
        /** \brief font used for blackboard (double-stroke) symbols (escepcially in math environments) */
        JKQTMTPROPERTY(QString, fontBlackboard)
        /** \brief roman font for math environment */
        JKQTMTPROPERTY(QString, fontMathRoman)
        /** \brief sans font for math environment */
        JKQTMTPROPERTY(QString, fontMathSans)
        /** \brief prefix for LaTeX fonts */
        JKQTMTPROPERTY(QString, fontLatexPrefix)
        /** \brief postfix for LaTeX fonts */
        JKQTMTPROPERTY(QString, fontLatexPostfix)
        /** \brief specifies the encoding of special character fonts (default is \c MTFEwinSymbol ) */
        JKQTMTPROPERTY(MTfontEncoding, fontEncoding)
        /** \brief resizing factor for braces in math mode */
        JKQTMTPROPERTY(double, brace_factor)
        /** \brief shrinking the width of braces in math mode 0: reduce to 0 pixel width, 1: leave unchanged*/
        JKQTMTPROPERTY(double, brace_shrink_factor)
        /** \brief resizing factor for font size in sub-/superscript */
        JKQTMTPROPERTY(double, subsuper_size_factor)
        /** \brief fraction of a whitespace by which to shift a sub-/superscript left/right when the previous text is italic */
        JKQTMTPROPERTY(double, italic_correction_factor)
        /** \brief like subsuper_size_factor, but for operators (\\sum, \\int) where the text is placed above/below the symbol */
        JKQTMTPROPERTY(double, operatorsubsuper_size_factor)
        /** \brief factor, used to extend the size of an operator in math mode */
        JKQTMTPROPERTY(double, mathoperator_width_factor)
        /** \brief relative shift of text in superscript to normal text:
         *         0= baseline kept, 1: baseline shifted to top of normal text */
        JKQTMTPROPERTY(double, super_shift_factor)
        /** \brief relative shift of text in subscript to normal text:
         *         0= baseline kept, 1: baseline shifted to bottom of normal text */
        JKQTMTPROPERTY(double, sub_shift_factor)
        /** \brief indicates whether to use STIX fonts or not */
        JKQTMTPROPERTY(bool, useSTIXfonts)
        /** \brief indicates whether to use XITS fonts or not */
        JKQTMTPROPERTY(bool, useXITSfonts)
        /** \brief indicates whether to use XITS fonts or not */
        JKQTMTPROPERTY(bool, useASANAfonts)
        /** \brief scaling factor for font of nominator and denominator of a fraction */
        JKQTMTPROPERTY(double, frac_factor)
        /** \brief shift of denominator/nummerator away from central line of a frac */
        JKQTMTPROPERTY(double, frac_shift_factor)
        /** \brief scaling factor for font of underbrace/overbrace text */
        JKQTMTPROPERTY(double, underbrace_factor)
        /** \brief scaling factor for font of underset/overset text */
        JKQTMTPROPERTY(double, underset_factor)
        /** \brief fraction of the brace ascent that the brace is shifted downwards, when scaled */
        JKQTMTPROPERTY(double, brace_y_shift_factor)
        /** \brief size of the decorations (dot, tilde, ...), as fractio of the baselineheight */
        JKQTMTPROPERTY(double, decoration_height_factor)
        /** \brief switches on some options that will grant better rendering at the expense of higher time consumption */
        JKQTMTPROPERTY(bool, expensiveRendering)
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
        JKQTPGET_MACRO(MTnode*, parsedNode)

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









