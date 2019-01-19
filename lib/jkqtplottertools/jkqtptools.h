/*
    Copyright (c) 2008-2019 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/** \file jkqtptools.h
  * \ingroup jkqtptools
  */


#ifndef JKQTPTOOLS_H_INCLUDED
#define JKQTPTOOLS_H_INCLUDED
#include "jkqtplottertools/jkqtp_imexport.h"
#include <QString>
#include <QElapsedTimer>
#include <QLocale>
#include <QPainter>
#include <cfloat>
#include <QPrinter>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <cctype>


#ifndef __WINDOWS__
# if defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#  define __WINDOWS__
# endif
#endif

#ifndef __LINUX__
# if defined(linux)
#  define __LINUX__
# endif
#endif

#undef JKTOOLS_TIMER_USE_TIME
#if defined(__WINDOWS__)
  #include<windows.h>
#elif defined(__LINUX__)
  #include <sys/time.h>
#else
  #define JKTOOLS_TIMER_USE_TIME
#endif

#ifndef __WINDOWS__
# ifndef __LINUX__
#  warning("these methods are ment to be used under windows or linux ... no other system were tested")
# endif
#endif

/* This just distinguishes between the different path formats on Windows and Unix:
 *   - on Windows you use a backslash '\' do separate directories
 *   - in Unix you use a slash '/' to separate directories
 */
#ifdef __WINDOWS__
  /** \brief a separator between two directories in a path between \c " quotes */
  #define JKQTPPATHSEPARATOR_STRING "\\"
  /** \brief a separator between two directories in a path between \c ' quotes */
  #define JKQTPPATHSEPARATOR_CHAR '\\'
  #include<windows.h>
  #include <io.h>
#else
  /** \brief a separator between two directories in a path between \c " quotes */
  #define JKQTPPATHSEPARATOR_STRING "/"
  /** \brief a separator between two directories in a path between \c ' quotes */
  #define JKQTPPATHSEPARATOR_CHAR '/'
  #include <unistd.h>
  #include <dirent.h>
#endif




class JKQTPEnhancedPainter; // forward
class JKQtBasePlotter; // forward declaration

/** \brief smallest linewidth any line in JKQtPlotter may have
 * \ingroup jkqtptools
 */
#define JKQTPLOTTER_ABS_MIN_LINEWIDTH 0.02

/** \brief check whether the dlotaing point number is OK (i.e. non-inf, non-NAN)
 * \ingroup jkqtptools
 */
template <typename T>
inline T JKQTPIsOKFloat(T v) {
    return std::isfinite(v)&&(!std::isinf(v))&&(!std::isnan(v));
}



/** \brief converts a QT::PenStyle into a string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTP_QPenStyle2String(Qt::PenStyle style);

/** \brief converts a QString into a Qt::PenStyle
 * \ingroup jkqtptools
 */
LIB_EXPORT Qt::PenStyle JKQTP_String2QPenStyle(QString style);
/** \brief converts a QT::BrushStyle into a string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTP_QBrushStyle2String(Qt::BrushStyle style);

/** \brief converts a QString into a Qt::BrushStyle
 * \ingroup jkqtptools
 */
LIB_EXPORT Qt::BrushStyle JKQTP_String2QBrushStyle(QString style);




/** \brief display mode for an axis
 * \ingroup jkqtptools */
enum JKQTPCAdrawMode {
    JKQTPCADMcomplete=0, /*!< \brief draw axis with ticks, ticklabels and axis label */
    JKQTPCADMticksAndLabels, /*!< \brief draw axis with ticks and tick labels */
    JKQTPCADMticks, /*!< \brief draw axis with ticks */
    JKQTPCADMline, /*!< \brief draw axis as thick line */
    JKQTPCADMnone /*!< \brief draw no axis */
};

/** \brief converts a JKQTPCAdrawMode variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPCAdrawMode2String(JKQTPCAdrawMode pos);

/** \brief converts a string into a JKQTPCAdrawMode
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPCAdrawMode String2JKQTPCAdrawMode(QString pos);


/** \brief display mode for the axis labels
 * \ingroup jkqtptools */
enum JKQTPCAlabelType {
    JKQTPCALTdefault, /*!< \brief simply print the numbers */
    JKQTPCALTexponentCharacter, /*!< \brief print the numbers and show a unit character, i.e. \c 5µ for \f$ 5\cdot 10^{-6} \f$ , \cd 3k for \f$ 3\cdot 10^3 \f$ ... */
    JKQTPCALTexponent, /*!< \brief show numbers in exponential for, e.g. \f$ 3\cdot 10^5 \f$ ... */
    JKQTPCALTdate, /*!< \brief show numbers as dates */
    JKQTPCALTtime, /*!< \brief show numbers as times */
    JKQTPCALTdatetime, /*!< \brief show numbers as times */
};



/** \brief mode of the axis ticks
 * \ingroup jkqtptools */
enum JKQTPLabelTickMode {
    JKQTPLTMLinOrPower=0, /*!< \brief linear, or log, depending on whether the axis is log */
    JKQTPLTMLin, /*!< \brief always linear (even for log-axes) */
    JKQTPLTMPower, /*!< \brief powers (of the log-base) */
};

/** \brief converts a JKQTPLabelTickMode variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPLabelTickMode2String(JKQTPLabelTickMode pos);

/** \brief converts a string into a JKQTPLabelTickMode
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPLabelTickMode String2JKQTPLabelTickMode(QString pos);


/** \brief converts a JKQTPCAlabelType variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPCAlabelType2String(JKQTPCAlabelType pos);

/** \brief converts a string into a JKQTPCAlabelType
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPCAlabelType String2JKQTPCAlabelType(QString pos);

/** \brief position of the axis labels
 * \ingroup jkqtptools
 */
enum JKQTPlabelPosition {
    JKQTPlabelMin=0,            /*!< \brief the axis label is near the min value of the axis (left/bottom) */
	JKQTPlabelMax,            /*!< \brief the axis label is near the max value of the axis (right/top) */
	JKQTPlabelCenter          /*!< \brief the label is at the center of the axis */
};


/** \brief converts a JKQTPlabelPosition variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPlabelPosition2String(JKQTPlabelPosition pos);

/** \brief converts a string into a JKQTPlabelPosition
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPlabelPosition String2JKQTPlabelPosition(QString pos);

/** \brief position of the key
 * \ingroup jkqtptools
 */
enum JKQTPkeyPosition {
    JKQTPkeyOutsideTopRight=0,            /*!< \brief the key is positioned on the right side of the graph */
    JKQTPkeyOutsideTopLeft,             /*!< \brief the key is positioned on the left side of the graph */
    JKQTPkeyOutsideLeftTop,              /*!< \brief the key is positioned above the graph */
    JKQTPkeyOutsideLeftBottom,           /*!< \brief the key is positioned below the graph */
    JKQTPkeyOutsideRightBottom,            /*!< \brief the key is positioned on the right side of the graph */
    JKQTPkeyOutsideRightTop,             /*!< \brief the key is positioned on the left side of the graph */
    JKQTPkeyOutsideBottomLeft,              /*!< \brief the key is positioned above the graph */
    JKQTPkeyOutsideBottomRight,           /*!< \brief the key is positioned below the graph */
    JKQTPkeyInsideTopRight,             /*!< \brief the key is positioned inside on the right side of the graph, but inside the graph*/
    JKQTPkeyInsideTopLeft,              /*!< \brief the key is positioned inside on the left side of the graph */
    JKQTPkeyInsideBottomLeft,               /*!< \brief the key is positioned inside on the upper bound of the graph */
    JKQTPkeyInsideBottomRight             /*!< \brief the key is positioned inside on the lower bound of the graph */
};


/** \brief converts a JKQTPlabelPosition variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPkeyPosition2String(JKQTPkeyPosition pos);

/** \brief converts a string into a JKQTPlabelPosition
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPkeyPosition String2JKQTPkeyPosition(QString pos);

/** \brief layout of the key
 * \ingroup jkqtptools
 */
enum JKQTPkeyLayout {
    JKQTPkeyLayoutOneColumn,            /*!< \brief the key consists of one column */
    JKQTPkeyLayoutOneRow,            /*!< \brief the key consists of one row */
    JKQTPkeyLayoutMultiColumn,           /*!< \brief the key consists of multiple columns */
};


/** \brief converts a JKQTPkeyLayout variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPkeyLayout2String(JKQTPkeyLayout pos);

/** \brief converts a String into a JKQTPkeyLayout
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPkeyLayout String2JKQTPkeyLayout(QString pos);



/** \brief used to represent the position of other graphs in \ref jkqtplotter_base_saveprint
 * \ingroup jkqtptools
 */
typedef struct {
    size_t x;
    size_t y;
    JKQtBasePlotter* plotter;
} JKQTPgridPrintingItem;




/**
 * \brief create a property variable and a default variable for it. Also creates a doxygen comment for the default variable
 * \ingroup jkqtptools
 */
#define JKQTPPROPERTY(type,varname) \
  type varname; \
  /*! \brief default value for property property varname. \see varname for more information */ \
  type def_##varname;

/**
 * \brief saves the given property (for which also a def_property exists) into the given settings object
 * \ingroup jkqtptools
 */
#define JKQTPPROPERTYsave(settings, group, var, varname) \
    if (var!=def_##var) settings.setValue(group+varname, var);
/**
 * \brief loads the given property from the given settings object
 * \ingroup jkqtptools
 */
#define JKQTPPROPERTYload(settings, group, var, varname, varconvert) \
    var=settings.value(group+varname, var).varconvert;


/** \brief plot styles for the error information
 * \ingroup jkqtptools
 */
enum JKQTPerrorPlotstyle {
    JKQTPerrorEllipses=10,           /*!< \brief an ellipse spanned by the errors */
    JKQTPerrorBoxes=9,           /*!< \brief a box spanned by the errors */
    JKQTPerrorSimpleBarsPolygons=8,           /*!< \brief simplified error barsand polygons  for each data point */
    JKQTPerrorSimpleBarsLines=7,           /*!< \brief simplified error bars and line for each data point */
    JKQTPerrorSimpleBars=6,           /*!< \brief simplified error bars for each data point */
    JKQTPerrorLines=5,                /*!< \brief a second and third graph line above and below the actual data which indicates the error value */
	JKQTPerrorBars=4,                 /*!< \brief error bars for each data point */
	JKQTPerrorPolygons=3,              /*!< \brief line error lines, but with filled range in between */
	JKQTPerrorBarsLines=2,            /*!< \brief error bars and lines for each data point */
	JKQTPerrorBarsPolygons=1,         /*!< \brief error bars and polygons for each data point */
	JKQTPnoError=0                    /*!< \brief don't show error information */
};




/** \brief converts a JKQTPerrorPlotstyle variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPerrorPlotstyle2String(JKQTPerrorPlotstyle pos);

/** \brief converts a String into a JKQTPerrorPlotstyle
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPerrorPlotstyle String2JKQTPerrorPlotstyle(QString pos);

/** \brief plot styles for a graph
 * \ingroup jkqtptools
 */
enum JKQTPgraphPlotstyle {
	JKQTPlines,                /*!< \brief plot y=f(x), connect the datapoints by straight lines */
	JKQTPfilledCurveX,         /*!< \brief plot y=f(x), as filled curve (filled until the y=0/x-axis) */
	JKQTPfilledCurveY,         /*!< \brief plot x=f(y), as filled curve (filled until the x=0/y-axis) */
	JKQTPpoints,               /*!< \brief plot y=f(x), plot each datapoint with a symbol */
	JKQTPlinesPoints,          /*!< \brief plot y=f(x), plot each datapoint with a symbol and connect them by straight lines */
	JKQTPimpulsesHorizontal,   /*!< \brief plot y=f(x), plot each datapoint as a line from (x,0) to (x,f(x))  */
	JKQTPimpulsesVertical,     /*!< \brief plot x=f(y), plot each datapoint as a line from (0,f(x)) to (x,f(x))  */
	JKQTPstepsX,               /*!< \brief plot y=f(x), as a step curve */
	JKQTPstepsY                /*!< \brief plot x=f(y), as a step curve */
};

/** \brief symbols that can be used to plot a datapoint for a graph
 * \ingroup jkqtptools
 */
enum JKQTPgraphSymbols {
    JKQTPnoSymbol=0,             /*!< \brief plots no symbol at all (usefull together with error bars) */
    JKQTPdot=1,                 /*!< \brief a small dot \image html symbols/symbol_symbol_dot.png */
    JKQTPcross=2,               /*!< \brief a X cross \image html symbols/symbol_symbol_cross.png */
    JKQTPplus=3,                /*!< \brief a + cross \image html symbols/symbol_symbol_plus.png */
    JKQTPcircle=4,              /*!< \brief an unfilled circle \image html symbols/symbol_symbol_circle.png */
    JKQTPfilledCircle=5,        /*!< \brief a filled circle \image html symbols/symbol_symbol_filled_circle.png */
    JKQTPrect=6,                /*!< \brief an unfilled rectangle \image html symbols/symbol_symbol_rect.png */
    JKQTPfilledRect=7,          /*!< \brief a filled rectangle \image html symbols/symbol_symbol_filled_rect.png */
    JKQTPtriangle=8,            /*!< \brief an unfilled triangle (tip at top) \image html symbols/symbol_symbol_triangle.png */
    JKQTPfilledTriangle=9,      /*!< \brief a filled triangle (tip at top) \image html symbols/symbol_symbol_filled_triangle.png */
    JKQTPdownTriangle=10,        /*!< \brief an unfilled triangle (tip at bottom) \image html symbols/symbol_symbol_down_triangle.png */
    JKQTPfilledDownTriangle=11,  /*!< \brief a filled triangle (tip at bottom) \image html symbols/symbol_symbol_filled_down_triangle.png */
    JKQTPdiamond=12,                /*!< \brief an unfilled diamond \image html symbols/symbol_symbol_diamond.png */
    JKQTPfilledDiamond=13,          /*!< \brief a filled diamond \image html symbols/symbol_symbol_filled_diamond.png */
    JKQTPstar=14,                /*!< \brief an unfilled diamond \image html symbols/symbol_symbol_star.png */
    JKQTPfilledStar=15,          /*!< \brief a filled diamond \image html symbols/symbol_symbol_filled_star.png */
    JKQTPpentagon=16,                /*!< \brief an unfilled pentagon \image html symbols/symbol_symbol_pentagon.png */
    JKQTPfilledPentagon=17,          /*!< \brief a filled pentagon \image html symbols/symbol_symbol_filled_pentagon.png */
    JKQTPasterisc=18,          /*!< \brief an asterisc star '*' \image html symbols/symbol_symbol_asterisc.png */

    JKQTPtarget=19,              /*!< \brief a target symbol (circle with cross) \image html symbols/symbol_symbol_target.png */
    JKQTPrectCross=20,              /*!< \brief a square symbol with a cross inside \image html symbols/symbol_symbol_rect_cross.png */
    JKQTPrectPlus=21,              /*!< \brief a square symbol with a plus inside \image html symbols/symbol_symbol_rect_plus.png */

    JKQTPmaxSymbolID=JKQTPrectPlus
};

/** \brief converts a JKQTPgraphSymbols variable into a identifier string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPgraphSymbols2String(JKQTPgraphSymbols pos);
/** \brief converts a JKQTPgraphSymbols variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPgraphSymbols2NameString(JKQTPgraphSymbols pos);

/** \brief converts a String into a JKQTPgraphSymbols
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPgraphSymbols String2JKQTPgraphSymbols(QString pos);


/** \brief convert a double to a string, using the loacle "C"
 * \ingroup jkqtptools
 */
inline QString JKQTPCDoubleToQString(double value) {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    return loc.toString(value, 'g', 18);
}

/** \brief convert a double to a string
 * \ingroup jkqtptools
 */
inline QString JKQTPDoubleToQString(double value, int prec = 10, char f = 'g', QChar decimalSeparator='.') {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QString res=loc.toString(value, f, prec);
    if (loc.decimalPoint()!=decimalSeparator) {
        res=res.replace(loc.decimalPoint(), decimalSeparator);
    }
    return res;
}



/** \brief rotate a rectangle by  given angle (rotates all points around the center of the rectangle and returns it as a QPolygonF)
 * \ingroup jkqtptools
 */
LIB_EXPORT QPolygonF jkqtpRotateRect(QRectF r, double angle);




/*! \brief swap two elements \a l and \a r in an array \a a
    \ingroup jkqtptools

*/
template <class T>
inline void jkqtpSwap(T* a, int l, int r){
    const T tmp=a[l];
    a[l]=a[r];
    a[r]=tmp;
}




/*! \brief QuickSort (recursive implementation)
    \ingroup jkqtptools

    implementation from http://www.linux-related.de/index.html?/coding/sort/sort_quick.htm
*/
template <class T>
inline void jkqtpQuicksort(T* a, int l, int r){
    if(r>l){
        int i=l-1;
        int j=r;

        for(;;){
            while(a[++i]<a[r]);
            while(a[--j]>a[r] && j>i);
            if(i>=j) break;
            jkqtpSwap<T>(a, i, j);
        }
        jkqtpSwap<T>(a, i, r);

        jkqtpQuicksort(a, l, i-1);
        jkqtpQuicksort(a, i+1, r);
    }
}


/*! \brief QuickSort (recursive implementation), sorts \a a2 alongside \a a, using \a a as sort criterion
    \ingroup jkqtptools

    implementation from http://www.linux-related.de/index.html?/coding/sort/sort_quick.htm
*/
template <class T, class T2>
inline void jkqtpQuicksort(T* a, T2* a2, int l, int r){
    if(r>l){
        int i=l-1;
        int j=r;

        for(;;){
            while(a[++i]<a[r]);
            while(a[--j]>a[r] && j>i);
            if(i>=j) break;
            jkqtpSwap(a, i, j);
            jkqtpSwap(a2, i, j);
        }
        jkqtpSwap(a, i, r);
        jkqtpSwap(a2, i, r);

        jkqtpQuicksort(a, a2, l, i-1);
        jkqtpQuicksort(a, a2, i+1, r);
    }
}




/*! \brief sort the given arrays, using \a input as sort criterion
    \ingroup jkqtptools

    \param input array to be sorted
    \param input2 array to be sorted
    \param N size of the array input
    \param output if \c !=nullptr data is written here (the memory location pointed at by \a output has to have at least the length \a N !!!),
                  otherwise the array input is sorted inplace.
    \param output2 if \c !=nullptr data is written here (the memory location pointed at by \a output has to have at least the length \a N !!!),
                  otherwise the array input is sorted inplace.

 */
template <class T, class T2>
inline void jkqtpSort(T* input, T2* input2, int N, T* output=nullptr, T2* output2=nullptr) {
    if ((!input)) return ;
    if (N<=0) return;
    T* data=input;
    if (output!=nullptr) {
        data=output;
        memcpy(output, input, N*sizeof(T));
    }
    T2* data2=input2;
    if (output2!=nullptr && input2!=nullptr) {
        data2=output2;
        memcpy(output2, input2, N*sizeof(T2));
    }
    jkqtpQuicksort(data, data2, 0, N-1);
}

/** \brief RAII construct that times its lifetime, outputting properly indented qDebug()-message
 * \ingroup jkqtptools_debugging
 */
class LIB_EXPORT JKQTPAutoOutputTimer : public QElapsedTimer
{
    public:
        explicit JKQTPAutoOutputTimer(const QString& message);
        ~JKQTPAutoOutputTimer();
        void write(const QString& message) const;
    protected:
        QString message;
        QString indent;
        static int global_indent;

};

/** \brief convert a string to lower-case characters
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_tolower(const std::string& s);

/** \brief convert a string to a boolean
 * \ingroup jkqtptools_string
 */
LIB_EXPORT bool jkqtp_strtobool(std::string data);
/** \brief convert a string to upper-case
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_toupper(const std::string& s);

/** \brief std::string wrapper around sprintf()
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_format(const std::string& templ, ...);

/** \brief convert a number of bytes to a string, formatting e.g. 1024 as 1kB, ...
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_bytestostr(double bytes);

/** \brief convert an integer to a string
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_inttostr(long data);

/** \brief convert an integer to a hex string
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_inttohex(long data);

/** \brief convert an unsigned int to a string
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_uinttostr(unsigned long data);

/** \brief convert a double to a string
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_floattostr(double data, int past_comma=-1, bool remove_trail0=false, double belowIsZero=1e-16);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000,"g") will result in "1kg"
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_floattounitstr(double dataa, std::string unitname);
/** \brief convert a boolean to a string
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_booltostr(bool data);
/** \brief converts a RGBA color into a string
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
LIB_EXPORT std::string jkqtp_rgbtostring(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);

/** \brief converts a QColor into a string using the jkqtp_rgbtostring() method.
 * \ingroup jkqtptools_string
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
inline QString JKQTP_QColor2String(QColor color) {
    return QString(jkqtp_rgbtostring(static_cast<unsigned char>((color).red()), static_cast<unsigned char>((color).green()), static_cast<unsigned char>((color).blue()), static_cast<unsigned char>((color).alpha())).c_str());
}

/** \brief clean a string to be usable as a variable name, e.g. in an expression parser, or a C++-expression
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_to_valid_variable_name(std::string input);

/** \brief convert a double to a string, encoding powers of ten as characters, e.g. \c jkqtp_floattounitstr(1000) will result in "1k"
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_floattounitstr(double data, int past_comma=5, bool remove_trail0=false);
/** \brief convert a double to a string, encoding powers of ten as exponent in LaTeX notation (e.g. <code>-1.23\cdot 10^{-5}</code>)
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_floattolatexstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
/** \brief convert a double to a string, encoding powers of ten as exponent with HTML tags
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_floattohtmlstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
/** \brief convert a character to a string
 * \ingroup jkqtptools_string
 */
LIB_EXPORT std::string jkqtp_chartostr(char data);

/** \brief wandelt einen Datentyp in einen double um, wird von JKQTPDatastore zur Wandlung benutzt
 *  \ingroup jkqtptools
 *
 * Diese Funktion nutzt per default static_cast<double>(), kann aber für spezielle Datentypen überschrieben werden, etwa für bool
*/
template<typename T>
inline constexpr double jkqtp_todouble(const T& d) {
    return static_cast<double>(d);
}


/** \brief wandelt einen boolean in einen double um, wird von JKQTPDatastore zur Wandlung benutzt,
 *         Spezialisierung für bool (true -> 1.0, false -> 0.0)
 *  \ingroup jkqtptools */
template<>
inline constexpr double jkqtp_todouble(const bool& d) {
    return static_cast<double>((d)?1.0:0.0);
}




/** \brief plot styles for the error information
 * \ingroup jkqtptools
 */
enum JKQTPstepType {
    JKQTPstepLeft=0,           /*!< \brief datapoint is on the left edge of the hor. step line */
    JKQTPstepCenter=1,           /*!< \brief datapoint is centered on the hor. step line */
    JKQTPstepRight=2,         /*!< \brief datapoint is on the right edge of the hor. step line */
};


/** \brief converts a JKQTPstepType variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPstepType2String(JKQTPstepType pos);

/** \brief converts a String into a JKQTPstepType
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPstepType String2JKQTPstepType(QString pos);

#endif // JKQTPTOOLS_H_INCLUDED
