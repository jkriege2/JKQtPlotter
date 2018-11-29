/*
    Copyright (c) 2008-2018 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center

    

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


/**
 * \defgroup jkqtptools Tool Functions and Definitions
 * \ingroup jkqtplotter
 */

/** \file jkqtptools.h
  * \ingroup jkqtptools
  */


#ifndef JKQTPTOOLS_H_INCLUDED
#define JKQTPTOOLS_H_INCLUDED
#include "jkqtplottertools/jkqtp_imexport.h"
#include <QString>
#include <QElapsedTimer>
#include <QToolBar>
#include <QPainter>
#include <cmath>
#include <cfloat>
#include <QComboBox>
#include <QPrinter>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <time.h>
#include <math.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>


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
#else
  /** \brief a separator between two directories in a path between \c " quotes */
  #define JKQTPPATHSEPARATOR_STRING "/"
  /** \brief a separator between two directories in a path between \c ' quotes */
  #define JKQTPPATHSEPARATOR_CHAR '/'
#endif



/**
 * \defgroup tools_getset get_var and set_var macros
 * \ingroup tools
 *
 * The macros in this group have the purpose to make writing \c get_<varname>() and \c set_<varname>(<varname>)
 * methods for classes easier. They can be used by giving the type and the name of a private variable for which
 * \c get_ and \c set_ methods should be created:
 * \code
 *   class a {
 *     private:
 *       int var1;
 *       std::strign var2;
 *     public:
 *       GetSetMacro(int, var1);
 *       JKQTPGetMacro(std::string, var2);
 *   }
 * \endcode
 * This code will create a \c set_var1, a \c set_var2 and a \c get_var1 method with the apropriate types.
 * All functions will be declared \c virtual and can thus be easily overloaded in inheriting classes. They are also
 * declared \c inline so the compiler may optimize them by really inlining them.
 *
 * The GetSetMacro creates get and set methods while the GetMacro and the SetMacro only create one of the
 * both.
 */
/*@{*/

/**
 * \brief create get_varname() and set_varname(type __value) methods/functions
 * inside a class, where \c type is the type of \c varname and \c varname is a
 * previously declared private variable that should be accessed by these
 * methodes
*/
#define JKQTPGET_SET_MACRO(type,varname) \
    typedef type typedef_set_##varname ;\
  /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  inline virtual void set_##varname (const typedef_set_##varname & __value)  \
  { \
    this->varname = __value; \
  } \
  /** \brief returns the property varname. \see varname for more information */ \
  inline virtual type get_##varname () const  \
  {\
    return this->varname; \
  }
#define JKQTPGET_SET_VMACRO(type,varname) \
    typedef type typedef_set_##varname ;\
  /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  inline virtual void set_##varname (const typedef_set_##varname & __value)  \
  { \
    this->varname = __value; \
  } \
  /** \brief returns the property varname. \see varname for more information */ \
  inline virtual type get_##varname () const  \
  {\
    return this->varname; \
  }
#define JKQTPGetSetMacro(type,varname) JKQTPGET_SET_MACRO(type,varname)

/**
 * \brief create get_varname() and set_varname(type __value) methods/functions
 * inside a class, where \c type is the type of \c varname and \c varname is a
 * previously declared private variable that should be accessed by these
 * methodes. In addition this will set the property paramsChanged to true, which
 * you will have to declare in your class.
*/
#define JKQTPGET_SET_MACRO_P(type,varname) \
    typedef type typedef_set_##varname ;\
  inline virtual void set_##varname (const typedef_set_##varname & __value) /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  { \
    this->varname = __value; \
    this-> paramsChanged=true; \
  } \
  inline virtual type get_##varname () const /** \brief returns the property varname. \see varname for more information */ \
  {\
    return this->varname; \
  }
#define JKQTPGetSetMacroP(type,varname) JKQTPGET_SET_MACRO_P(type,varname)

/**
 * \brief like GetSetMacroP(), but adds the instruction \a inst to the set method. This may be used
 *        to update a component after a value has been set.
*/
#define JKQTPGET_SET_MACRO_IP(type,varname,inst) \
    typedef type typedef_set_##varname ;\
  /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  inline virtual void set_##varname (const typedef_set_##varname & __value)   \
  { \
    if (this->varname != __value) { \
        this->varname = __value; \
        this-> paramsChanged=true; \
        inst; \
    } \
  } \
  /** \brief returns the property varname. \see varname for more information */ \
  inline virtual type get_##varname () const  \
  { \
    return this->varname; \
  }
#define JKQTPGetSetMacroIP(type,varname,inst) JKQTPGET_SET_MACRO_IP(type,varname,inst)

/**
 * \brief like GetSetMacro(), but adds the instruction \a inst to the set method. This may be used
 *        to update a component after a value has been set.
*/
#define JKQTPGET_SET_MACRO_I(type,varname,inst) \
    typedef type typedef_set_##varname ;\
    /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
    inline virtual void set_##varname (const typedef_set_##varname & __value)  \
    {\
        if (this->varname != __value) { \
            this->varname = __value; \
            inst; \
        } \
    } \
    /** \brief returns the property varname. \see varname for more information */ \
    inline virtual type get_##varname () const  \
    {\
        return this->varname; \
    }
#define JKQTPGetSetMacroI(type,varname,inst) JKQTPGET_SET_MACRO_I(type,varname,inst)

/**
 * \brief sets two given properties at the same time, i.e. with one setter function with the name <code>set_<name>(type __value, type2 __value2)</code>. The getter methods are still separate. This may be used
 *        to update a component after a value has been set.
*/
#define JKQTPGET_SET_MACRO_TWO(name,type,varname,type2,varname2) \
    /** \brief sets the properties varname and varname2 to the specified \a __value and \a __value2. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \details Description of the parameter varname2 is: <CENTER>\copybrief varname2.</CENTER> \see varname and varname2 for more information */ \
    inline virtual void set_##name (type __value, type2 __value2)  \
    {\
        bool set=false; \
        if (this->varname != __value) { \
            this->varname = __value; \
        } \
        if (this->varname2 != __value2) { \
            this->varname2 = __value2; \
        } \
    } \
    /** \brief returns the property varname. \see varname for more information */ \
    inline virtual type get_##varname () const  \
    {\
        return this->varname; \
    } \
    /** \brief returns the property varname2. \see varname2 for more information */\
    inline virtual type2 get_##varname2 () const  \
    {\
        return this->varname2; \
    }
#define JKQTPGetSetMacroTwo(name,type,varname,type2,varname2) JKQTPGET_SET_MACRO_TWO(name,type,varname,type2,varname2)

/**
 * \brief like GetSetMacroTwo(), but adds the instruction \a inst to the set method. This may be used
 *        to update a component after a value has been set.
*/
#define JKQTPGET_SET_MACRO_TWO_I(name,type,varname,type2,varname2,inst) \
    /** \brief sets the properties varname and varname2 to the specified \a __value and \a __value2. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \details Description of the parameter varname2 is: <CENTER>\copybrief varname2.</CENTER> \see varname and varname2 for more information */ \
    inline virtual void set_##name (type __value, type2 __value2)  \
    {\
        bool set=false; \
        if (this->varname != __value) { \
            this->varname = __value; \
            set=true; \
        } \
        if (this->varname2 != __value2) { \
            this->varname2 = __value2; \
            set=true; \
        } \
        if (set) { \
            inst; \
        } \
    } \
    /** \brief returns the property varname. \see varname for more information */ \
    inline virtual type get_##varname () const  \
    {\
        return this->varname; \
    } \
    /** \brief returns the property varname2. \see varname2 for more information */ \
    inline virtual type2 get_##varname2 () const  \
    {\
        return this->varname2; \
    }
#define JKQTPGetSetMacroTwoI(name,type,varname,type2,varname2,inst) JKQTPGET_SET_MACRO_TWO_I(name,type,varname,type2,varname2,inst)


/**
 * \brief like GetSetMacro(), but adds the instruction \a inst to the set method. This may be used
 *        to update a component after a value has been set. Alwys updates (no comparison of current
 *        and former value
*/
#define JKQTPGET_SET_MACRO_INC(type,varname,inst) \
    typedef type typedef_set_##varname ;\
    /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is: <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
    inline virtual void set_##varname (typedef_set_##varnam __value)  \
    {\
        this->varname = __value; \
        inst; \
    } \
    /** \brief returns the property varname. \see varname for more information */ \
    inline virtual type get_##varname () const  \
    {\
        return this->varname; \
    }
#define JKQTPGetSetMacroINC(type,varname,inst) JKQTPGET_SET_MACRO_INC(type,varname,inst)

/** \brief create get_varname() and set_varname(type __value) methods/functions
 * inside a class, where \c type is the type of \c varname and \c varname is a
 * previously declared private variable that should be accessed by these
 * methodes
*/
#define JKQTPGET_MACRO(type,varname) \
    /** \brief returns the property varname. \details Description of the parameter varname is:  <CENTER>\copybrief varname.</CENTER>. \see varname for more information */ \
    inline virtual type get_##varname() const  \
    {   return this->varname;   }

#define JKQTPGetMacro(type,varname) JKQTPGET_MACRO(type,varname)

/**
 * \brief create set_varname(type __value) methods/functions
 * inside a class, where \c type is the type of \c varname and \c varname is a
 * previously declared private variable that should be accessed by these
 * methodes
*/
#define JKQTPSET_MACRO(type,varname) \
    /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is:  <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
    inline virtual void set_##varname (const type & __value)  \
    { \
        this->varname = __value; \
    }
#define JKQTPSetMacro(type,varname) JKQTPSET_MACRO(type,varname)

/**
 * \brief create set_varname(type __value) methods/functions
 * inside a class, where \c type is the type of \c varname and \c varname is a
 * previously declared private variable that should be accessed by these
 * methodes
*/
#define JKQTPSET_CAST_MACRO(typefunction,typeinternal,varname) \
    /** \brief sets the property varname to the specified \a __value, where __value is static_cast'ed from typefunction to typeinternal. \details Description of the parameter varname is:  <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
    inline virtual void set_##varname (const typefunction & __value)  \
    { \
        this->varname = static_cast<typeinternal>(__value); \
    }

/**
 * \brief create set_varname(type __value) methods/functions
 * inside a class, where \c type is the type of \c varname and \c varname is a
 * previously declared private variable that should be accessed by these
 * methodes. In addition this will set the property paramsChanged to true, which
 * you will have to declare in your class.
*/
#define JKQTPSET_MACRO_P(type,varname) \
    typedef type typedef_set_##varname ;\
    /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is:  <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  inline virtual void set_##varname (const typedef_set_##varname & __value)  \
  { \
    if (this->varname != __value) { \
        this->varname = __value; \
        this->paramsChanged=true; \
    } \
  }
#define JKQTPSetMacroP(type,varname) JKQTPSET_MACRO_P(type,varname)

/**
 * \brief like SetMacro(), but adds the instruction \a inst to the set method. This may be used
 *        to update a component after a value has been set
 */
#define JKQTPSET_MACRO_I(type,varname,inst) \
    typedef type typedef_set_##varname ;\
  /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is:  <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  inline virtual void set_##varname(const typedef_set_##varname & __value)  \
  { \
    if (this->varname != __value) { \
        this->varname = __value; \
        inst; \
    } \
  }
#define JKQTPSetMacroI(type,varname,inst) JKQTPSET_MACRO_I(type,varname,inst)

/**
 * \brief like SetMacro(), but adds the instruction \a inst at the start of the set method. This may be used
 *        e.g. to lock a mutex in the set operation, e.g. using QMutexLocker locker(mutex); from Qt
 */
#define JKQTPSET_MACRO_I_BEFORE(type,varname,inst) \
    typedef type typedef_set_##varname ;\
  /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is:  <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  inline virtual void set_##varname(const typedef_set_##varname & __value)  \
  { \
    inst; \
    if (this->varname != __value) { \
        this->varname = __value; \
    } \
  }
#define JKQTPSetMacroIBefore(type,varname,inst) JKQTPSET_MACRO_I_BEFORE(type,varname,inst)
/**
 * \brief like SetMacroP(), but adds the instruction \a inst to the set method. This may be used
 *        to update a component after a value has been set
 */
#define JKQTPSET_MACRO_IP(type,varname,inst) \
    typedef type typedef_set_##varname ;\
  /** \brief sets the property varname to the specified \a __value. \details Description of the parameter varname is:  <CENTER>\copybrief varname.</CENTER> \see varname for more information */ \
  inline virtual void set_##varname (const typedef_set_##varname & __value)  \
  { \
    if (this->varname != __value) { \
        this->varname = __value; \
        this->paramsChanged=true; \
        inst; \
    } \
  }
#define JKQTPSetMacroIP(type,varname,inst) JKQTPSET_MACRO_IP(type,varname,inst)
/*@}*/

/** \defgroup tools_files filesystem and file I/O
 *  \ingroup tools
 */

/** \defgroup tools_streams C++ stream tools
 *  \ingroup tools_files
 */
/*@{*/

class JKQTPEnhancedPainter; // forward

#define JKQTPLOTTER_ABS_MIN_LINEWIDTH 0.02

/** \brief check whether the dlotaing point number is OK (i.e. non-inf, non-NAN)
 * \ingroup jkqtptools
 */
#define JKQTPIsOKFloat(v) (std::isfinite(v)&&(!std::isinf(v))&&(!std::isnan(v)))


/** \brief converts a QColor into a string using the jkqtp_rgbtostring() method.
 * \ingroup jkqtptools
 *
 * This returns a QString which contains the name of named colors and the RGBA values in a QT readable form othertwise.
 */
#define JKQTP_QColor2String(color) QString(jkqtp_rgbtostring(static_cast<unsigned char>((color).red()), static_cast<unsigned char>((color).green()), static_cast<unsigned char>((color).blue()), static_cast<unsigned char>((color).alpha())).c_str())

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


class JKQtBasePlotter; // forward declaration


/** \brief display mode for an axis
 * \ingroup jkqtptools */
enum JKQTPCAdrawMode {
    JKQTPCADMcomplete=0, /*!< \brief draw axis with ticks, ticklabels and axis label */
    JKQTPCADMticksAndLabels, /*!< \brief draw axis with ticks and tick labels */
    JKQTPCADMticks, /*!< \brief draw axis with ticks */
    JKQTPCADMline, /*!< \brief draw axis as thick line */
    JKQTPCADMnone /*!< \brief draw no axis */
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
    JKQTPCALTexponentCharacter, /*!< \brief print the numbers and show a unit character, i.e. \c 5µ for \f$ 5\cdot10^{-6} \f$ , \cd 3k for \f$ 3\cdot10^3 \f$ ... */
    JKQTPCALTexponent, /*!< \brief show numbers in exponential for, e.g. \f$ 3\cdot10^5 \f$ ... */
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


/** \brief a modified a href="http://doc.trolltech.com/4.5/qtoolbar.html">QToolBar</a> which vanishes when the mouse leaves the toolbar.
 * \ingroup jkqtptools
 */
class LIB_EXPORT JKVanishQToolBar: public QToolBar {
        Q_OBJECT
    public:
        /** \brief class constructor */
        JKVanishQToolBar(const QString& title, QWidget* parent=nullptr): QToolBar(title, parent) {
            toolbarVanishes=true;
        }
        /** \brief class constructor */
        JKVanishQToolBar(QWidget* parent=nullptr): QToolBar(parent){
            toolbarVanishes=true;
        }
        JKQTPGET_SET_MACRO(bool, toolbarVanishes)
    protected:
        bool toolbarVanishes;
        /** \brief this event triggers the vanishing of the toolbar */
        void leaveEvent ( QEvent * /*event*/ ) {
            if (toolbarVanishes) hide();
        }
};


/**
 * \brief create a property variable and a default variable for it. Also creates a doxygen comment for the default variable
 * \ingroup jkqtptools
 */
#define JKQTPPROPERTY(type,varname) \
  type varname; \
  /** \brief default value for property property varname. \see varname for more information */ \
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
 * \ingroup jkqtplotter
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



/** \brief converts a JKQTPerrorPlotstyle variable into a human-readable string
 * \ingroup jkqtptools
 */
LIB_EXPORT QString JKQTPerrorPlotstyle2String(JKQTPerrorPlotstyle pos);

/** \brief converts a String into a JKQTPerrorPlotstyle
 * \ingroup jkqtptools
 */
LIB_EXPORT JKQTPerrorPlotstyle String2JKQTPerrorPlotstyle(QString pos);

/** \brief plot styles for a graph
 * \ingroup jkqtplotter
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
 * \ingroup jkqtplotter
 */
enum JKQTPgraphSymbols {
    JKQTPnoSymbol=0,             /*!< \brief plots no symbol at all (usefull together with error bars) */
    JKQTPdot=1,                 /*!< \brief a small dot */
    JKQTPcross=2,               /*!< \brief a X cross */
    JKQTPplus=3,                /*!< \brief a + cross */
    JKQTPcircle=4,              /*!< \brief an unfilled circle */
    JKQTPfilledCircle=5,        /*!< \brief a filled circle */
    JKQTPrect=6,                /*!< \brief an unfilled rectangle */
    JKQTPfilledRect=7,          /*!< \brief a filled rectangle */
    JKQTPtriangle=8,            /*!< \brief an unfilled triangle (tip at top) */
    JKQTPfilledTriangle=9,      /*!< \brief a filled triangle (tip at top) */
    JKQTPdownTriangle=10,        /*!< \brief an unfilled triangle (tip at bottom) */
    JKQTPfilledDownTriangle=11,  /*!< \brief a filled triangle (tip at bottom) */
    JKQTPdiamond=12,                /*!< \brief an unfilled diamond */
    JKQTPfilledDiamond=13,          /*!< \brief a filled diamond */
    JKQTPstar=14,                /*!< \brief an unfilled diamond */
    JKQTPfilledStar=15,          /*!< \brief a filled diamond */
    JKQTPpentagon=16,                /*!< \brief an unfilled pentagon */
    JKQTPfilledPentagon=17,          /*!< \brief a filled pentagon */
    JKQTPasterisc=18,          /*!< \brief an asterisc star '*' */

    JKQTPtarget=19,              /*!< \brief a target symbol (circle with cross) */
    JKQTPrectCross=20,              /*!< \brief a square symbol with a cross inside */
    JKQTPrectPlus=21,              /*!< \brief a square symbol with a plus inside */

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

/*! \brief plot the specified symbol at pixel position x,y
   \ingroup jkqtptools

    \param painter the QPainter to draw to
    \param x x-coordinate of the symbol center
    \param y y-coordinate of the symbol center
    \param symbol type of the symbol to plot, see JKQTPgraphSymbols
    \param size size (width/height) of the symbol around (\a x , \a y)
    \param symbolLineWidth width of the lines used to draw the symbol
    \param color color of the symbol lines
    \param fillColor color of the symbol filling
 */
LIB_EXPORT void JKQTPplotSymbol(JKQTPEnhancedPainter& painter, double x, double y, JKQTPgraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);

/*! \brief plot the specified symbol at pixel position x,y
   \ingroup jkqtptools

    \param paintDevice the paint device to draw on
    \param x x-coordinate of the symbol center
    \param y y-coordinate of the symbol center
    \param symbol type of the symbol to plot, see JKQTPgraphSymbols
    \param size size (width/height) of the symbol around (\a x , \a y)
    \param symbolLineWidth width of the lines used to draw the symbol
    \param color color of the symbol lines
    \param fillColor color of the symbol filling
 */
LIB_EXPORT void JKQTPplotSymbol(QPaintDevice& paintDevice, double x, double y, JKQTPgraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);


/*! \brief plot an arrow between positions (x1,y1) and (x2,y2)
   \ingroup jkqtptools

    \param painter the QPainter to draw to
    \param x1 first x-coordinate of the arrow
    \param y1 first y-coordinate of the arrow
    \param x2 second x-coordinate of the arrow
    \param y2 second y-coordinate of the arrow
    \param symbol type of the symbol to plot, see JKQTPgraphSymbols
    \param size size (width/height) of the symbol around (\a x , \a y)
    \param symbolLineWidth width of the lines used to draw the symbol
    \param color color of the symbol lines
    \param fillColor color of the symbol filling
 */
//LIB_EXPORT void JKQTPplotArrow(JKQTPEnhancedPainter& painter, int x, int y, JKQTPgraphSymbols symbol, double size, double symbolLineWidth, QColor color, QColor fillColor);


/*! \brief draw an ellipse without setting pen or brush, or saving the painter!
    \ingroup jkqtptools

    \return a QVector<QPointF> with points that may be used for drawing
    \param x center of ellipse (x-coordinate)
    \param y center of ellipse (y-coordinate)
    \param a half axis in x-direction
    \param b half axis in y-direction
    \param angle_start starting angle of ellipse section
    \param angle_end ending angle of ellipse section
    \param alpha rotation angle of ellipse
    \param controlPoints the number of points to use for drawing
    \param[out] x_start first point of ellipse
    \param[out] x_end last point of ellipse

    \note all angles are given in degrees [0..360]
*/
LIB_EXPORT QVector<QPointF> JKQTPdrawEllipse(double x, double y, double a, double b, double angle_start=0, double angle_end=360, double alpha=0, int controlPoints=180, QPointF* x_start=nullptr, QPointF* x_end=nullptr);


#include <QDoubleSpinBox>
/*! \brief enhanced QDoubleSpinBox
    \ingroup jkqtptools

*/
class LIB_EXPORT JKQTPEnhancedDoubleSpinBox : public QDoubleSpinBox {
        Q_OBJECT
    public:
        JKQTPEnhancedDoubleSpinBox(QWidget* parent=nullptr);
        ~JKQTPEnhancedDoubleSpinBox();
    signals:
        void editingFinished(double value);
    protected slots:
        void intEditingFinished();
};



#include <QSpinBox>
/*! \brief enhanced QDoubleSpinBox
    \ingroup jkqtptools

*/
class LIB_EXPORT JKQTPEnhancedSpinBox : public QSpinBox {
        Q_OBJECT
    public:
        JKQTPEnhancedSpinBox(QWidget* parent=nullptr);
        ~JKQTPEnhancedSpinBox();
    signals:
        void editingFinished(int value);
    protected slots:
        void intEditingFinished();
};

inline QString JKQTPCDoubleToQString(double value) {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    return loc.toString(value, 'g', 18);
}


inline QString JKQTPDoubleToQString(double value, int prec = 10, char f = 'g', QChar decimalSeparator='.') {
    QLocale loc=QLocale::c();
    loc.setNumberOptions(QLocale::OmitGroupSeparator);
    QString res=loc.toString(value, f, prec);
    if (loc.decimalPoint()!=decimalSeparator) {
        res=res.replace(loc.decimalPoint(), decimalSeparator);
    }
    return res;
}

#include <QTableView>

/*! \brief this class extends the QTableView
    \ingroup jkqtptools

    This enhanced table view adds some functionality to the Qt class:
      - return HTML code that describes the table
      - the selected cells may be copied to Excel using the clipboard (Excel will recognize number !)
    .
*/
class LIB_EXPORT JKQTPEnhancedTableView : public QTableView {
        Q_OBJECT
    public:
        JKQTPEnhancedTableView(QWidget* parent=nullptr);
        virtual ~JKQTPEnhancedTableView();

        /** \brief return the contents of the table view as HTML fragment */
        QString toHtml(int borderWidth=1, bool non_breaking=false, int fontSizePt=-1) const;
        void print(QPrinter* printer, bool onePageWide=false, bool onePageHigh=false);

        void paint(QPainter& painter, QRect pageRec=QRect());
        QSizeF getTotalSize() const;
        QAction* getPrintAction() const { return printAction; }

    signals:
        void keyPressed(int key, Qt::KeyboardModifiers modifiers, QString text);
    public slots:
        void copySelectionToExcel(int copyrole=Qt::EditRole, bool storeHead=true);
        void copySelectionToExcelNoHead(int copyrole=Qt::EditRole);
        void copySelectionToCSV(int copyrole=Qt::EditRole, bool storeHead=true, const QString& separator=", ", const QChar& decimalpoint='.');
        void copySelectionToCSVNoHead(int copyrole=Qt::EditRole, const QString& separator=", ", const QChar& decimalpoint='.');
        void print();

    protected:
        virtual void keyPressEvent(QKeyEvent* event);
        void paint(QPainter &painter, double scale, int page, double hhh, double vhw, const QList<int>& pageCols, const QList<int>& pageRows, QPrinter* p=nullptr);

        QPrinter* getPrinter(QPrinter* printerIn=nullptr, bool *localPrinter=nullptr);
        QAction* printAction;
    private:
};




#include <QPainter>


/*! \brief this class extends the QPainter
    \ingroup jkqtptools


*/
class LIB_EXPORT JKQTPEnhancedPainter : public QPainter {
    public:
        JKQTPEnhancedPainter(QPaintDevice* device);
        JKQTPEnhancedPainter();
        virtual ~JKQTPEnhancedPainter();


        //void drawPath(const QPainterPath &path);
        //void drawPoints(const QPoint *points, int pointCount);
        //void drawPoints(const QPointF *points, int pointCount);
//        void drawLines(const QLineF *lines, int lineCount);
//        void drawLines(const QPointF *pointPairs, int lineCount);
//        void drawLines(const QLine *lines, int lineCount);
//        void drawLines(const QPoint *pointPairs, int lineCount);


//        inline void drawPoint(const QPointF &p) {
//            drawPoints(&p, 1);
//        }

//        inline void drawPoint(const QPoint &p) {
//             drawPoints(&p, 1);
//        }

//        inline void drawPoint(int x, int y) {
//            drawPoint(QPointF(x,y));
//        }

//        inline void drawPoint(double x, double y) {
//            drawPoint(QPointF(x,y));
//        }

//        inline void drawPoints(const QPolygonF &points){
//             drawPoints(points.constData(), points.size());
//        }

//        inline void drawPoints(const QPolygon &points){
//            drawPoints(points.constData(), points.size());
//        }

//        inline void drawLine(const QLineF &line) {
//            drawLines(&l, 1);
//        }

//        inline void drawLine(const QLine &line) {
//            drawLines(&l, 1);
//        }

//        inline void drawLine(int x1, int y1, int x2, int y2) {
//            drawLines(QLineF(x1,y1,x2,y2), 1);
//        }

//        inline void drawLine(double x1, double y1, double x2, double y2) {
//            drawLines(QLineF(x1,y1,x2,y2), 1);
//        }

//        inline void drawLine(const QPoint &p1, const QPoint &p2) {
//            drawLine(QLineF(QLine(p1, p2)));
//        }

//        inline void drawLine(const QPointF &p1, const QPointF &p2) {
//            drawLine(QLineF(p1, p2));
//        }

//        inline void drawLines(const QVector<QLineF> &lines) {
//            drawLines(lines.constData(), lines.size());
//        }

//        inline void drawLines(const QVector<QPointF> &pointPairs) {
//            drawLines(pointPairs.constData(), pointPairs.size() / 2);
//        }

//        inline void drawLines(const QVector<QLine> &lines) {
//            drawLines(lines.constData(), lines.size());
//        }

//        inline void drawLines(const QVector<QPoint> &pointPairs) {
//            drawLines(pointPairs.constData(), pointPairs.size() / 2);
//        }

//        inline void drawRect(const QRectF &rect) {
//            drawRects(&rect, 1);
//        }

//        inline void drawRect(int x1, int y1, int w, int h) {
//            QRectF r(x, y, w, h);
//            drawRects(&r, 1);
//        }

//        inline void drawRect(double x1, double y1, double w, double h) {
//            QRectF r(x, y, w, h);
//            drawRects(&r, 1);
//        }

//        inline void drawRect(const QRect &rect) {
//           drawRects(&r, 1);
//        }

    protected:
        virtual void initQEnhacedPainter();
    private:

};




LIB_EXPORT QPolygonF jkqtpRotateRect(QRectF r, double angle);




/*! \brief swap two elements \a l and \a r in an array \a a
    \ingroup jkqtptools

*/
template <class T>
inline void jkqtpSwap(T* a, long long l, long long r){
    const T tmp=a[l];
    a[l]=a[r];
    a[r]=tmp;
}




/*! \brief QuickSort (recursive implementation)
    \ingroup jkqtptools

    implementation from http://www.linux-related.de/index.html?/coding/sort/sort_quick.htm
*/
template <class T>
inline void jkqtpQuicksort(T* a, long long l, long long r){
    if(r>l){
        long long i=l-1;
        long long j=r;

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
inline void jkqtpQuicksort(T* a, T2* a2, long long l, long long r){
    if(r>l){
        long long i=l-1;
        long long j=r;

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
inline void jkqtpSort(T* input, T2* input2, long long N, T* output=nullptr, T2* output2=nullptr) {
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

LIB_EXPORT std::string jkqtp_tolower(const std::string& s);

LIB_EXPORT bool jkqtp_strtobool(std::string data);
LIB_EXPORT std::string jkqtp_toupper(const std::string& s);

LIB_EXPORT std::string jkqtp_format(const std::string& templ, ...);

LIB_EXPORT long jkqtp_get_filesize(char *FileName);
LIB_EXPORT std::string jkqtp_bytestostr(double bytes);

LIB_EXPORT std::string jkqtp_inttostr(long data);

LIB_EXPORT std::string jkqtp_inttohex(long data);

LIB_EXPORT std::string jkqtp_uinttostr(unsigned long data);

LIB_EXPORT std::string jkqtp_floattostr(double data, int past_comma=-1, bool remove_trail0=false, double belowIsZero=1e-16);

LIB_EXPORT std::string jkqtp_floattounitstr(double dataa, std::string unitname);
LIB_EXPORT std::string jkqtp_booltostr(bool data);
LIB_EXPORT std::string jkqtp_rgbtostring(unsigned char r, unsigned char g, unsigned char b, unsigned char a=255);
LIB_EXPORT std::string jkqtp_to_valid_variable_name(std::string input);

LIB_EXPORT std::string jkqtp_floattounitstr(double data, int past_comma=5, bool remove_trail0=false);
LIB_EXPORT std::string jkqtp_floattolatexstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
LIB_EXPORT std::string jkqtp_floattohtmlstr(double data, int past_comma=5, bool remove_trail0=false, double belowIsZero=1e-16, double minNoExponent=1e-3, double maxNoExponent=1e4);
LIB_EXPORT std::string jkqtp_chartostr(char data);

/** \brief wandelt einen Datentyp in einen double um, wird von JKQTPDatastore zur Wandlung benutzt
 *
 * Diese Funktion nutzt per default static_cast<double>(), kann aber für spezielle Datentypen überschrieben werden, etwa für bool
*/
template<typename T>
inline constexpr double jkqtp_todouble(const T& d) {
    return static_cast<double>(d);
}


/** \brief wandelt einen boolean in einen double um, wird von JKQTPDatastore zur Wandlung benutzt,
 *         Spezialisierung für bool (true -> 1.0, false -> 0.0) */
template<>
inline constexpr double jkqtp_todouble(const bool& d) {
    return static_cast<double>((d)?1.0:0.0);
}
#endif // JKQTPTOOLS_H_INCLUDED
