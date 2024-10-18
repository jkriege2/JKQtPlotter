/*
    Copyright (c) 2023-2023 Jan W. Krieger (<jan@jkrieger.de>)

    

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




#ifndef JKQTPKEY_H
#define JKQTPKEY_H


#include <QString>
#include <QPainter>
#include "jkqtplotter/jkqtplotter_imexport.h"
#include "jkqtplotter/jkqtpkeystyle.h"

// forward declarations
class JKQTBasePlotter;
class JKQTPPlotElement;
class JKQTMathText;



/*! \brief base class for drawing a key (or legend)
    \ingroup jkqtpbaseplotter_elements

    The class JKQTPBaseKey implements the basic layouting and drawing of a key/legend.

    \section JKQTPBaseKey_Styling Key/Legend Styling

    \copydetails JKQTPKeyStyle

    \note The class JKQTPBaseKey offers setters (slots) and getters for all the properties in the used instance of JKQTPKeyStyle .

    \section JKQTPBaseKey_Usage Usage

    The functions in JKQTPBaseKey are mainly used for drawing the main key in a JKQTBasePlotter.
    The instance representing that main key is accessible via JKQTPBaseKey::getMainKey().
    It is not a direct instance of JKQTPBaseKey, which is impossible, as it is pure virtual,
    but an instance of JKQTPMainKey, which extends JKQTPBaseKey with access to the titles assigned to
    each JKQTPPlotElement (see JKQTPPlotElement::getTitle(), JKQTPPlotElement::setTitle()) and its key-marker (JKQTPPlotElement::drawKeyMarker()).

    Note however that it is also possible to draw additional keys into the graph (e.g. to show the size/color classes in a parametrized scatter graph),
    if the corresponding JKQTPPlotElement implements this.



    \section JKQTPBaseKey_ImplementationDetails Implementation Details

    It relies on these protected virtual functions to determine the contents of the legend items:
        - JKQTPBaseKey::getEntryCount()
        - JKQTPBaseKey::getEntryTextExtent()
        - JKQTPBaseKey::getEntryText()
        - JKQTPBaseKey::drawEntrySample()
    .
    So you can derive from JKQTPBaseKey and implement these to provide a new source of data.

    There are additional customization points in this class. You can also override the
    default implementations of these functions to change the way keys/legends are drawn:
        - JKQTPBaseKey::drawKey()
        - JKQTPBaseKey::getLayout()
        - JKQTPBaseKey::modifySize()
    .

    \see JKQTPKeyStyle, JKQTPMainKey

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPBaseKey: public QObject {
        Q_OBJECT
    protected:
        struct KeyLayoutDescription; // forward
    public:

        /** \brief class constructor */
        explicit JKQTPBaseKey(JKQTBasePlotter* parent);
        /** \brief class destructor */
        virtual ~JKQTPBaseKey();
        virtual void setParent(JKQTBasePlotter* parent);
        /** \brief loads the plot properties from a QSettings object */
        virtual void loadSettings(const QSettings &settings, const QString& group=QString("plots/key/"));

        /** \brief saves the plot properties into a QSettings object.
         *
         * This method only saves those properties that differ from their default value.
         */
        virtual void saveSettings(QSettings& settings, const QString& group=QString("plots/key/")) const;



        /** \brief describes a single key entry */
        struct JKQTPLOTTER_LIB_EXPORT KeyItemData {
            KeyItemData(int _id=-1, const QString& _text="", const QSizeF _size=QSizeF());
            /** \brief id of the item, used to call drawEntrySample() etc. */
            int id;
            /** \brief text/label text/markup in the enry */
            QString text;
            /** \brief size of \a text in pixels */
            QSizeF size;

        };


        /** \brief struct, describing basic layout and size properties of a key/legend, mostly used as return value of getSize(). */
        struct JKQTPLOTTER_LIB_EXPORT KeySizeDescription {

            /** \brief type describing the position of the key */
            enum KeyLocation {
                keyInside, /*!< \brief somewhere inside the plot */
                keyOutsideLeft, /*!< \brief on the left of the plot */
                keyOutsideRight, /*!< \brief on the right of the plot */
                keyOutsideTop, /*!< \brief above the plot */
                keyOutsideBottom /*!< \brief below the plot */
            };

            KeySizeDescription(QSize _requiredSize=QSize(), KeyLocation _keyLocation=keyInside);
            KeySizeDescription(const KeySizeDescription& other);
            KeySizeDescription& operator=(const KeySizeDescription& other);

            /** \brief required spacefor the key/legend [pixels] */
            QSizeF requiredSize;
            /** \brief where the size requiredSize has to be resevred for the key */
            KeyLocation keyLocation;

            /** \brief private data \internal */
            std::unique_ptr<KeyLayoutDescription> d;

        };

        /** \brief calculates the size required for the key
         *
         *  This function internally calls getLayout(), which is (together with drawKey() )
         *  a customization point for key layout.
         */
        KeySizeDescription getSize(JKQTPEnhancedPainter& painter) ;
        /** \brief econd stage of size calculation
         *
         *  This functions allows to again modify the KeySizeDescription \a currentSize in a second step,
         *  when the preliminary size of the plot is known (given as \a preliminaryPlotSize ).
         *
         *  This is used to implement the JKQTPKeyLayout::JKQTPKeyLayoutMultiColumn and JKQTPKeyLayout::JKQTPKeyLayoutMultiRow,
         *  where the distribution of entries over rows/columns is calculated here.
         *
         *  \note This function is declared virtual so it can be used as a customization point!
         */
        virtual void modifySize(JKQTPEnhancedPainter& painter, KeySizeDescription& currentSize, QSizeF preliminaryPlotSize) ;

        /** \brief draw the key
         *
         *  \param painter the painter to use for drawing
         *  \param rect the rectangle to plot the key into, depending on JKQTPKeyStyle::position this is either the plot rectangle itself, or
         *              a rectangle outside the plot that fits the key
         *  \param layout output of getSize(), possibly modified by modifySize()
         *
         *  \note It is expected that the owning JKQTBasePlotter first calls getSize() and the determmines a QREct that fits the legend. The latter is the supplied as \a rect to this function for subsequent drawing.
         *        It is also expected that the legends contents does not changed in the meantime!
         */
        virtual void drawKey(JKQTPEnhancedPainter& painter, const QRectF &rect, const KeySizeDescription &layout);


        /** \copydoc parent */
        inline const JKQTBasePlotter* getParent() const { return parent; }
        /** \copydoc parent */
        inline JKQTBasePlotter* getParent()  { return parent; }

        /** \brief current style properties (JKQTPKeyStyle) for this JKQTPKey
         *
         * \see JKQTPKeyStyle
         */
        const JKQTPKeyStyle &getCurrentKeyStyle() const;

        /** \brief replace the current style properties for this JKQTPKey
         *
         * \see JKQTPSetSystemDefaultBaseStyle(), JKQTPSetSystemDefaultBaseStyle(), setCurrentAxisStyle(), getCurrentAxisStyle(), \ref jkqtpplotter_styling
         */
        void setCurrentKeyStyle(const JKQTPKeyStyle& style);

        /** \copydoc JKQTPKeyStyle::frameVisible */
        inline bool getFrameVisible() const { return keyStyle().frameVisible; }
        /** \copydoc JKQTPKeyStyle::visible */
        inline bool getVisible() const { return keyStyle().visible; }
        /** \copydoc JKQTPKeyStyle::xMargin */
        inline double getXMargin() const { return keyStyle().xMargin; }
        /** \copydoc JKQTPKeyStyle::yMargin */
        inline double getYMargin() const { return keyStyle().yMargin; }
        /** \copydoc JKQTPKeyStyle::xOffset */
        inline double getXOffset() const { return keyStyle().xOffset; }
        /** \copydoc JKQTPKeyStyle::yOffset */
        inline double getYOffset() const { return keyStyle().yOffset; }
        /** \copydoc JKQTPKeyStyle::xSeparation */
        inline double getXSeparation() const { return keyStyle().xSeparation; }
        /** \copydoc JKQTPKeyStyle::ySeparation */
        inline double getYSeparation() const { return keyStyle().ySeparation; }
        /** \copydoc JKQTPKeyStyle::columnSeparation */
        inline double getColumnSeparation() const { return keyStyle().columnSeparation; }
        /** \copydoc JKQTPKeyStyle::position */
        inline JKQTPKeyPosition getPosition() const { return keyStyle().position; }
        /** \copydoc JKQTPKeyStyle::layout */
        inline JKQTPKeyLayout getLayout() const { return keyStyle().layout; }
        /** \copydoc JKQTPKeyStyle::frameColor */
        inline QColor getFrameColor() const { return keyStyle().frameColor; }
        /** \copydoc JKQTPKeyStyle::frameLineStyle */
        inline Qt::PenStyle getFrameLineStyle() const { return keyStyle().frameLineStyle; }
        /** \copydoc JKQTPKeyStyle::textColor */
        inline QColor getTextColor() const { return keyStyle().textColor; }
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        inline QColor getBackgroundColor() const { return keyStyle().backgroundBrush.color(); }
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        inline const QBrush& getBackgroundBrush() const { return keyStyle().backgroundBrush; }
        /** \copydoc JKQTPKeyStyle::frameWidth */
        inline double getFrameWidth() const { return keyStyle().frameWidth; }
        /** \copydoc JKQTPKeyStyle::fontSize */
        inline double getFontSize() const { return keyStyle().fontSize; }
        /** \copydoc JKQTPKeyStyle::frameRounding */
        inline double getFrameRounding() const { return keyStyle().frameRounding; }
        /** \copydoc JKQTPKeyStyle::sampleLineLength */
        inline double getSampleLineLength() const { return keyStyle().sampleLineLength; }
    public Q_SLOTS:
        /** \copydoc JKQTPKeyStyle::visible */
        inline void setVisible(bool __value) { keyStyle().visible = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::frameVisible */
        inline void setFrameVisible(bool __value) { keyStyle().frameVisible = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::frameWidth */
        inline void setFrameWidth(double __value) { keyStyle().frameWidth = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::frameRounding */
        inline void setFrameRounding(double __value) { keyStyle().frameRounding = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::fontSize */
        inline void setFontSize(double __value) { keyStyle().fontSize = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::sampleLineLength */
        inline void setSampleLineLength(double __value) { keyStyle().sampleLineLength = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::xMargin */
        inline void setXMargin(double __value) { keyStyle().xMargin = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::yMargin */
        inline void setYMargin(double __value) { keyStyle().yMargin = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::xOffset */
        inline void setXOffset(double __value) { keyStyle().xOffset = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::yOffset */
        inline void setYOffset(double __value) { keyStyle().yOffset = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::xSeparation */
        inline void setXSeparation(double __value) { keyStyle().xSeparation = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::ySeparation */
        inline void setYSeparation(double __value) { keyStyle().ySeparation = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::columnSeparation */
        inline void setColumnSeparation(double __value) { keyStyle().columnSeparation = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::position */
        inline void setPosition(JKQTPKeyPosition __value) { keyStyle().position = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::layout */
        inline void setLayout(JKQTPKeyLayout __value) { keyStyle().layout = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::frameColor */
        inline void setFrameColor(QColor __value) { keyStyle().frameColor = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::frameLineStyle */
        inline void setFrameLineStyle(Qt::PenStyle __value) { keyStyle().frameLineStyle = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::textColor */
        inline void setTextColor(QColor __value) { keyStyle().textColor = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        inline void setBackgroundColor(QColor __value) { keyStyle().backgroundBrush = __value; redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        inline void setBackgroundColor(QColor __value, Qt::BrushStyle __style) { keyStyle().backgroundBrush = QBrush(__value, __style); redrawPlot(); }
        /** \copydoc JKQTPKeyStyle::backgroundBrush */
        inline void setBackgroundBrush(const QBrush& __value) { keyStyle().backgroundBrush = __value; redrawPlot(); }

    protected:
        /** \brief returns the number of legend entries */
        virtual int getEntryCount() const =0;
        /** \brief returns the label text (my contain markup for a JKQTMathText) of legend entry \a item 's text part */
        virtual QString getEntryText(int item) const =0;
        /** \brief returns the label color of legend entry \a item 's text part */
        virtual QColor getEntryColor(int item) const =0;
        /** \brief draws the sample for legend entry \a item into the given \a rect, using the given \a painter  */
        virtual void drawEntrySample(int item, JKQTPEnhancedPainter& painter, const QRectF& rect) =0;

        /** \brief describes one column of items in the key */
        struct JKQTPLOTTER_LIB_EXPORT KeyColumnDescription {
            KeyColumnDescription();
            KeyColumnDescription(const KeyItemData& item1);
            /** \brief list of all items in this row of key items */
            QList<KeyItemData> rows;

            /** \brief calculates the max. text width of the column in pixels */
            double calcMaxLabelWidth() const;
            /** \brief calculates the width of the column in pixels */
            double calcColumnWidth(double sampleLineLength, double xSeparation) const;
        };

        /** \brief struct, describing basic layout and size properties of a key/legend, mostly used as return value of getSize(). */
        struct JKQTPLOTTER_LIB_EXPORT KeyLayoutDescription {
            /** \brief list of all columns with items in the key */
            QList<KeyColumnDescription> columns;
            /** \brief calculates the width over all columns */
            double calcOverallWidth(double sampleLineLength, double xSeparation, double columnSeparation) const;
            /** \brief calculates the width over all columns */
            double calcOverallHeight(double ySeparation, double sampleHeight) const;
            /** \brief calculate the hieght of the \a i -th column */
            double calcRowHeight(int i, double sampleHeight) const;
            /** \brief calculates the number of rows */
            int calcRowCount() const;
            /** \brief calculate the number of items */
            int countItems() const;
            /** \brief put all items into one column */
            void redistributeIntoOneColumn();
            /** \brief takes all elements in columns and redistributes them over the given number of rows, items are distributed as equally as possible (last row may have fewer items) */
            void redistributeOverRows(int rows, bool rowMajor=true);
            /** \brief takes all elements in columns and redistributes them over the given number of columns, items are distributed as equally as possible (last column may have fewer items) */
            void redistributeOverColumns(int cols, bool colMajor=true);
        };

        /** \brief calculates all layout properties of the key/legend,necessary to size and draw it
         *
         *  This is internally called by getSize() and drawKey().
         */
        virtual KeyLayoutDescription getKeyLayout(JKQTPEnhancedPainter& painter) ;
        /** \brief fill KeySizeDescription::requiredSize */
        void calcLayoutSize(JKQTPEnhancedPainter& painter, KeySizeDescription& layout) const;
        /** \brief takes the size calculated by KeyLayoutDescription::calcOverallWidth() and KeyLayoutDescription::calcOverallHeight() and extends it with margins, line widths, ... optionally returns the one-sided offset*/
        virtual QSizeF extendLayoutSize(QSizeF rawLayoutSize, JKQTPEnhancedPainter &painter, QPointF* offset=nullptr) const;
        /** \brief provides the keyStyle to use for sizing/drawing this object */
        virtual const JKQTPKeyStyle& keyStyle() const;
        /** \brief provides the keyStyle to use for sizing/drawing this object */
        virtual JKQTPKeyStyle& keyStyle();

        /** \brief signals the parent JKQTBasePlotter to redraw */
        void redrawPlot();
        /** \brief retun parents JKQTMathText* object */
        JKQTMathText* getParentMathText();
        /** \brief retun parents JKQTMathText* object */
        const JKQTMathText* getParentMathText() const;


        /** \brief parent plotter class */
        JKQTBasePlotter* parent;
    private:
        /** \brief current style properties for this key/legend
         *
         * \see JKQTPKeyStyle
         */
        JKQTPKeyStyle localKeyStyle;
};


/*! \brief concrete class for drawing the main plot key (or legend).
 *         This class reads the key entries from the graphs list of its parent JKQTBasePlotter und references the JKQTPKeyStyle object from JKQTBasePlotterStyle::keyStyle in the parent JKQTBasePlotter.
    \ingroup jkqtpbaseplotter_elements


    \see JKQTPKeyStyle

 */
class JKQTPLOTTER_LIB_EXPORT JKQTPMainKey: public JKQTPBaseKey {
        Q_OBJECT
    protected:
    public:

        /** \brief class constructor */
        explicit JKQTPMainKey(JKQTBasePlotter* parent);
        /** \brief class destructor */
        virtual ~JKQTPMainKey();
    protected:
        /** \copydoc JKQTPBaseKey::getEntryCount() */
        virtual int getEntryCount() const override;
        /** \copydoc JKQTPBaseKey::getEntryText() */
        virtual QString getEntryText(int item) const override;
        /** \copydoc JKQTPBaseKey::getEntryColor() */
        virtual QColor getEntryColor(int item) const override;
        /** \copydoc JKQTPBaseKey::drawEntrySample() */
        virtual void drawEntrySample(int item, JKQTPEnhancedPainter& painter, const QRectF& rect) override;
        /** \brief returns the graph class from parent JKQTBasePlotter for the given \a item in the key */
        const JKQTPPlotElement* getPlotElement(int item) const;
        /** \brief returns the graph class from parent JKQTBasePlotter for the given \a item in the key */
        JKQTPPlotElement* getPlotElement(int item);
        /** \brief returns all graph classes from parent JKQTBasePlotter for the  key */
        QList<const JKQTPPlotElement*> getPlotElements() const;
        /** \brief provides the keyStyle to use for sizing/drawing this object (extracted from the parent JKQTBasePlotters's  */
        virtual const JKQTPKeyStyle& keyStyle() const override;
        /** \brief provides the keyStyle to use for sizing/drawing this object */
        virtual JKQTPKeyStyle& keyStyle() override;
};
#endif // JKQTPKEY_H
