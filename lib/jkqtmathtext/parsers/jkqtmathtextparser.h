/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)
    with contributions from: Razi Alavizadeh

    

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





#ifndef JKQTMATHTEXTPARSER_H
#define JKQTMATHTEXTPARSER_H

#include "jkqtmathtext/jkqtmathtext_imexport.h"
#include "jkqtmathtext/jkqtmathtext.h"
#include <QFlags>
#include <QObject>
#include <QString>


class JKQTMathTextNode; // forward

/*! \brief base class for all parsers used by JKQTMathText
    \ingroup jkqtmathtext_parser

 */
class JKQTMATHTEXT_LIB_EXPORT JKQTMathTextParser : public QObject {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKQTMathTextParser(JKQTMathText * parent = nullptr);
        /** \brief class destructor */
        virtual ~JKQTMathTextParser();
        /** \brief parse the given mathematical markup string.
         *
         *  \param text the markup to be parsed
         *  \param options Options for parsing, \see ParseOptions
         *
         *  \returns the memory representation of the markup \c nullptr on failure.
         */
        virtual JKQTMathTextNode* parse(const QString &text, JKQTMathText::ParseOptions options=JKQTMathText::DefaultParseOptions)=0;

        /** \brief parentMathText */
        JKQTMathText* getParentMathText();
        /** \brief parentMathText */
        const JKQTMathText* getParentMathText() const;
    protected:
        /** \brief JKQTMathText that instanciated and uses this parser */
        JKQTMathText* parentMathText;
        /** \brief adds a new error to the JKQTMathText referenced by parentMathText
         *
         *  \see JKQTMathText::addToErrorList()
         */
        void addToErrorList(const  QString& error);
        /** \brief clears the error list in the JKQTMathText referenced by parentMathText
         *
         *  \see JKQTMathText::clearErrorList()
         */
        void clearErrorList();

};


#endif // JKQTMATHTEXTPARSER_H









