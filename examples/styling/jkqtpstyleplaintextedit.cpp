#include "jkqtpstyleplaintextedit.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <QDebug>
#include <QGuiApplication>
#include <QFont>
#include <QFontDatabase>
#include "jkqtcommon/jkqtpicons.h"
#include "jkqtcommon/jkqttools.h"
#include "jkqtcommon/jkqtpdrawingtools.h"
#include "jkqtplotter/jkqtptools.h"
#include "jkqtplotter/jkqtpimagetools.h"
#include "jkqtmathtext/jkqtmathtext.h"

JKQTPStylePlainTextEdit::JKQTPStylePlainTextEdit(QWidget *parent):
    QPlainTextEdit(parent)
{

}

JKQTPStylePlainTextEdit::~JKQTPStylePlainTextEdit()
{

}

void JKQTPStylePlainTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    curContextMenu=cursorForPosition(event->pos());
    curContextMenu.select(QTextCursor::LineUnderCursor);
    const QString currentLine=curContextMenu.selectedText();
    QStringList currentLinePartsLower=currentLine.toLower().split('=');
    if (currentLinePartsLower.size()>1) {
        if (currentLinePartsLower[1].startsWith("\"") && currentLinePartsLower[1].endsWith("\"")) currentLinePartsLower[1]=currentLinePartsLower[1].mid(1, currentLinePartsLower[1].size()-2);
    }
    QStringList currentLineParts=currentLine.split('=');
    if (currentLineParts.size()>1) {
        if (currentLineParts[1].startsWith("\"") && currentLineParts[1].endsWith("\"")) currentLineParts[1]=currentLineParts[1].mid(1, currentLineParts[1].size()-2);
    }

    QMenu *menu = createStandardContextMenu();
    if (currentLinePartsLower.size()>1) {
        menu->addSeparator();
        QAction* currentValue=menu->addAction(tr("current value: "));
        currentValue->setVisible(false);
        QAction* currentValue2=menu->addAction(tr("also current value: "));
        currentValue2->setVisible(false);
        QMenu* submenu=menu->addMenu(tr("set value to ..."));
        submenu->setEnabled(false);
        if (currentLinePartsLower[0].endsWith("enabled")
                || currentLinePartsLower[0].endsWith("visible")
                || currentLinePartsLower[0].endsWith("display_mouse_position")
                || currentLinePartsLower[0].endsWith("use_palette_colors")
                || currentLinePartsLower[0].endsWith("debug_show_region_boxes")
                || currentLinePartsLower[0].endsWith("antialiase_system")
                || currentLinePartsLower[0].endsWith("antialiase_text")
                || currentLinePartsLower[0].endsWith("\\autosize")
                || currentLinePartsLower[0].endsWith("label_full_number")
                || currentLinePartsLower[0].endsWith("\\antialiase")
            ) {
            submenu->setEnabled(true);
            QAction* act;
            act=new QAction("true");
            connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
            submenu->addAction(act);
            act=new QAction("false");
            connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
            submenu->addAction(act);
        } else  if (currentLinePartsLower[0].endsWith("penstyle") || currentLinePartsLower[0].endsWith("line_style") || currentLinePartsLower[0].endsWith("line\\style") || currentLinePartsLower[0].endsWith("grid\\style")  || currentLinePartsLower[0].startsWith("graphs\\auto_styles\\line_style")) {
            submenu->setEnabled(true);
            currentValue->setVisible(true);
            currentValue->setText(currentValue->text()+currentLinePartsLower[1]);
            currentValue->setIcon(jkqtp_makeQPenStyleIcon(jkqtp_String2QPenStyle(currentLinePartsLower[1])));
           QAction* act;
           act=new QAction(jkqtp_makeQPenStyleIcon(Qt::SolidLine), jkqtp_QPenStyle2String(Qt::SolidLine));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQPenStyleIcon(Qt::DashLine), jkqtp_QPenStyle2String(Qt::DashLine));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQPenStyleIcon(Qt::DotLine), jkqtp_QPenStyle2String(Qt::DotLine));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQPenStyleIcon(Qt::DashDotLine), jkqtp_QPenStyle2String(Qt::DashDotLine));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQPenStyleIcon(Qt::DashDotDotLine), jkqtp_QPenStyle2String(Qt::DashDotDotLine));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQPenStyleIcon(Qt::NoPen), jkqtp_QPenStyle2String(Qt::NoPen));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
        } else  if (currentLinePartsLower[0].endsWith("brushstyle") ||currentLinePartsLower[0].endsWith("fill_style") || currentLinePartsLower[0].startsWith("graphs\\auto_styles\\fill_style")) {
            submenu->setEnabled(true);
            currentValue->setVisible(true);
            currentValue->setText(currentValue->text()+currentLinePartsLower[1]);
            currentValue->setIcon(jkqtp_makeQBrushStyleIcon(jkqtp_String2QBrushStyle(currentLinePartsLower[1])));
           QAction* act;
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::SolidPattern), jkqtp_QBrushStyle2String(Qt::SolidPattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::Dense1Pattern), jkqtp_QBrushStyle2String(Qt::Dense1Pattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::Dense2Pattern), jkqtp_QBrushStyle2String(Qt::Dense2Pattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::Dense3Pattern), jkqtp_QBrushStyle2String(Qt::Dense3Pattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::Dense4Pattern), jkqtp_QBrushStyle2String(Qt::Dense4Pattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::Dense5Pattern), jkqtp_QBrushStyle2String(Qt::Dense5Pattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::Dense6Pattern), jkqtp_QBrushStyle2String(Qt::Dense6Pattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::Dense7Pattern), jkqtp_QBrushStyle2String(Qt::Dense7Pattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::HorPattern), jkqtp_QBrushStyle2String(Qt::HorPattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::VerPattern), jkqtp_QBrushStyle2String(Qt::VerPattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::CrossPattern), jkqtp_QBrushStyle2String(Qt::CrossPattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::BDiagPattern), jkqtp_QBrushStyle2String(Qt::BDiagPattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::FDiagPattern), jkqtp_QBrushStyle2String(Qt::FDiagPattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::DiagCrossPattern), jkqtp_QBrushStyle2String(Qt::DiagCrossPattern));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
           act=new QAction(jkqtp_makeQBrushStyleIcon(Qt::NoBrush), jkqtp_QBrushStyle2String(Qt::NoBrush));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           submenu->addAction(act);
        } else  if (currentLinePartsLower[0].endsWith("decorator_type")) {
            submenu->setEnabled(true);
            currentValue->setVisible(true);
            currentValue->setText(currentValue->text()+currentLinePartsLower[1]);
            currentValue->setIcon(JKQTPLineDecoratorStyle2Icon(String2JKQTPLineDecoratorStyle(currentLinePartsLower[1])));
           QAction* act;
           JKQTPLineDecoratorStyle st;
           for (int i=0; i<static_cast<int>(JKQTPLineDecoratorCount); i++) {
               st=static_cast<JKQTPLineDecoratorStyle>(i);
               act=new QAction(JKQTPLineDecoratorStyle2Icon(st), JKQTPLineDecoratorStyle2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].startsWith("graphs\\auto_styles\\symbol") || currentLinePartsLower[0].endsWith("\\symbol")) {
            submenu->setEnabled(true);
            currentValue->setVisible(true);
            currentValue->setText(currentValue->text()+currentLinePartsLower[1]);
            currentValue->setIcon(JKQTPGraphSymbols2Icon(String2JKQTPGraphSymbols(currentLinePartsLower[1])));
           QAction* act;
           JKQTPGraphSymbols st;
           for (int i=0; i<static_cast<int>(JKQTPSymbolCount); i++) {
               st=static_cast<JKQTPGraphSymbols>(i);
               act=new QAction(JKQTPGraphSymbols2Icon(st), JKQTPGraphSymbols2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].endsWith("palette")) {
            submenu->setEnabled(true);
           QAction* act;
           QStringList pal=JKQTPImageTools::getPredefinedPalettesMachineReadable();
           for (int i=0; i<pal.size(); i++) {
               act=new QAction(JKQTPImageTools::GetPaletteIcon(i), pal[i]);
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
               if (pal[i].toLower()==currentLinePartsLower[1].toLower()) {
                   currentValue->setVisible(true);
                   currentValue->setText(currentValue->text()+currentLinePartsLower[1]);
                   currentValue->setIcon(JKQTPImageTools::GetPaletteIcon(i));

               }
           }
        } else  if (currentLinePartsLower[0].endsWith("color") || currentLinePartsLower[0].endsWith("widget_background_color_for_export") || currentLinePartsLower[0].startsWith("graphs\\auto_styles\\color")) {
            submenu->setEnabled(true);
            QMenu* menuPredef=submenu->addMenu(tr("predefined colors ..."));
            QMenu* menuPal=submenu->addMenu(tr("GUI colors ..."));
            QMenu* menuTrans=submenu->addMenu(tr("add transparency modifier ..."));
            currentValue->setVisible(true);
            currentValue->setText(currentValue->text()+currentLinePartsLower[1]);
            currentValue->setIcon(jkqtp_makeQColorIcon(jkqtp_String2QColor(currentLinePartsLower[1])));
           QAction* act;
           QColor st;
           const QStringList& names=jkqtp_listNamedColors();
           for (auto colName: names) {
               st=jkqtp_String2QColor(colName);
               act=new QAction(jkqtp_makeQColorIcon(st), jkqtp_QColor2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               menuPredef->addAction(act);
           }

           QString colName;
           colName="windowtext";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="button";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="light";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="midlight";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="dark";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="mid";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="text";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="brightttext";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="base";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="window";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="shadow";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="highlight";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="highlightedtext";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="link";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="linkvisited";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="alternatebase";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="norole";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="tooltipbase";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
           colName="tooltiptext";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
#if (QT_VERSION>=QT_VERSION_CHECK(5, 12, 0))
           colName="placeholdertext";
           act=new QAction(jkqtp_makeQColorIcon(jkqtp_String2QColor(colName)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuPal->addAction(act);
#endif
           int trans;
           trans=100;          colName=QString::number(trans)+"%";
           act=new QAction(jkqtp_makeQColorIcon(QColor(0,0,255,(100-trans)*255/100)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue), this, act->text()));
           menuTrans->addAction(act);
           trans=90;           colName=QString::number(trans)+"%";
           act=new QAction(jkqtp_makeQColorIcon(QColor(0,0,255,(100-trans)*255/100)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue), this, act->text()));
           menuTrans->addAction(act);
           trans=75;           colName=QString::number(trans)+"%";
           act=new QAction(jkqtp_makeQColorIcon(QColor(0,0,255,(100-trans)*255/100)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue), this, act->text()));
           menuTrans->addAction(act);
           trans=50;           colName=QString::number(trans)+"%";
           act=new QAction(jkqtp_makeQColorIcon(QColor(0,0,255,(100-trans)*255/100)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue), this, act->text()));
           menuTrans->addAction(act);
           trans=25;           colName=QString::number(trans)+"%";
           act=new QAction(jkqtp_makeQColorIcon(QColor(0,0,255,(100-trans)*255/100)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue), this, act->text()));
           menuTrans->addAction(act);
           trans=10;           colName=QString::number(trans)+"%";
           act=new QAction(jkqtp_makeQColorIcon(QColor(0,0,255,(100-trans)*255/100)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue), this, act->text()));
           menuTrans->addAction(act);
           trans=0;            colName=QString::number(trans)+"%";
           act=new QAction(jkqtp_makeQColorIcon(QColor(0,0,255,(100-trans)*255/100)), colName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue), this, act->text()));
           menuTrans->addAction(act);
        } else  if (currentLinePartsLower[0].endsWith("font_name")) {
            const QStringList splitValue=currentLineParts[1].split('+');
            submenu->setEnabled(true);
            QMenu* menuDefaultFonts=submenu->addMenu(tr("default fonts ..."));
            QMenu* menuSystemFonts=submenu->addMenu(tr("system fonts ..."));
            QMenu* menuAllFOnts=submenu->addMenu(tr("all fonts ..."));
            QMenu* menuMathFont=submenu->addMenu(tr("add math-fonts specifier ..."));
            currentValue->setVisible(true);
            currentValue->setText(tr("text font: ")+splitValue[0]);
            currentValue->setFont(QFont(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(JKQTMathTextFontSpecifier::fromFontSpec(currentLineParts[1]).fontName())));
            if (!JKQTMathTextFontSpecifier::fromFontSpec(currentLineParts[1]).mathFontName().isEmpty()) {
                currentValue2->setVisible(true);
                currentValue2->setText(tr("math font: ")+splitValue[1]);
                currentValue2->setFont(QFont(JKQTMathTextFontSpecifier::transformFontNameAndDecodeSpecialFonts(JKQTMathTextFontSpecifier::fromFontSpec(currentLineParts[1]).mathFontName())));
            }
           QAction* act;
           QFontDatabase fontDB;
           const QStringList names=fontDB.families();
           for (auto& fntName: names) {
               act=new QAction(fntName);
               act->setFont(QFont(fntName));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               menuAllFOnts->addAction(act);
           }

           QString fontName;
           fontName="application";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuDefaultFonts->addAction(act);
           act=new QAction(fontName+"+XITS");
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuDefaultFonts->addAction(act);
           fontName="XITS";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::getXITSFamilies().fontName()));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuDefaultFonts->addAction(act);
           menuDefaultFonts->addSeparator();
           fontName=QGuiApplication::font().family();
           act=new QAction(fontName);
           act->setFont(QFont(fontName));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuDefaultFonts->addAction(act);
           act=new QAction(fontName+"+XITS");
           act->setFont(QFont(fontName));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuDefaultFonts->addAction(act);

           fontName="application";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           act=new QAction(fontName+"+XITS");
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
           fontName="general";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="fixed";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="title";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="smallest_readable";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
#endif
           fontName="serif";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           act=new QAction(fontName+"+XITS");
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="sans-serif";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           act=new QAction(fontName+"+XITS");
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="cursive";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="typewriter";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="monospace";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="fantasy";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);
           fontName="system";
           act=new QAction(fontName);
           act->setFont(QFont(JKQTMathTextFontSpecifier::transformFontName(fontName)));
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
           menuSystemFonts->addAction(act);


           fontName="XITS";
           act=new QAction(fontName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addMathFontSpecifier), this, act->text()));
           menuMathFont->addAction(act);
           fontName="STIX";
           act=new QAction(fontName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addMathFontSpecifier), this, act->text()));
           menuMathFont->addAction(act);
           fontName="ASANA";
           act=new QAction(fontName);
           connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::addMathFontSpecifier), this, act->text()));
           menuMathFont->addAction(act);
        } else  if (currentLinePartsLower[0].endsWith("\\draw_mode1") || currentLinePartsLower[0].endsWith("\\draw_mode2")) {
           submenu->setEnabled(true);
           QAction* act;
           JKQTPCADrawMode st;
           for (int i=0; i<=static_cast<int>(JKQTPCADMmax); i++) {
               st=static_cast<JKQTPCADrawMode>(i);
               act=new QAction(JKQTPCADrawMode2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].endsWith("axis_label\\position")) {
           submenu->setEnabled(true);
           QAction* act;
           JKQTPLabelPosition st;
           for (int i=0; i<=static_cast<int>(JKQTPLabelPositionMax); i++) {
               st=static_cast<JKQTPLabelPosition>(i);
               act=new QAction(JKQTPLabelPosition2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].endsWith("ticks\\mode")) {
           submenu->setEnabled(true);
           QAction* act;
           JKQTPLabelTickMode st;
           for (int i=0; i<=static_cast<int>(JKQTPLTMmax); i++) {
               st=static_cast<JKQTPLabelTickMode>(i);
               act=new QAction(JKQTPLabelTickMode2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].endsWith("axis_label\\type")) {
           submenu->setEnabled(true);
           QAction* act;
           JKQTPCALabelType st;
           for (int i=0; i<=static_cast<int>(JKQTPCALTmax); i++) {
               st=static_cast<JKQTPCALabelType>(i);
               act=new QAction(JKQTPCALabelType2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].endsWith("key\\position")) {
           submenu->setEnabled(true);
           QAction* act;
           JKQTPKeyPosition st;
           for (int i=0; i<=static_cast<int>(JKQTPKeyPositionMax); i++) {
               st=static_cast<JKQTPKeyPosition>(i);
               act=new QAction(JKQTPKeyPosition2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].endsWith("key\\layout")) {
           submenu->setEnabled(true);
           QAction* act;
           JKQTPKeyLayout st;
           for (int i=0; i<=static_cast<int>(JKQTPKeyLayoutMax); i++) {
               st=static_cast<JKQTPKeyLayout>(i);
               act=new QAction(JKQTPKeyLayout2String(st));
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        } else  if (currentLinePartsLower[0].endsWith("color_mode")) {
           submenu->setEnabled(true);
           QAction* act;
           JKQTPColorDerivationMode::PredefinedModes st;
           for (int i=0; i<=static_cast<int>(JKQTPColorDerivationMode::JKQTPFFCMmax); i++) {
               st=static_cast<JKQTPColorDerivationMode::PredefinedModes>(i);
               act=new QAction(JKQTPColorDerivationMode(st).toString());
               connect(act, &QAction::triggered, std::bind(std::mem_fn(&JKQTPStylePlainTextEdit::changeCurrentLineValueTo), this, act->text()));
               submenu->addAction(act);
           }
        }
    }
    menu->exec(event->globalPos());
    delete menu;
}

void JKQTPStylePlainTextEdit::changeCurrentLineValueTo(const QString& targetText)
{
    const QString currentLine=curContextMenu.selectedText();
    const int idx=currentLine.indexOf('=');
    if (idx>=0) {
        const QString newLine=currentLine.left(idx+1)+targetText;
        curContextMenu.insertText(newLine);
    }
}

void JKQTPStylePlainTextEdit::addTransparencyToCurrentLineValue(const QString& newTrans)
{
    const QString currentLine=curContextMenu.selectedText();
    //qDebug()<<"current line: '"<<currentLine<<"'";
    const int idx=currentLine.indexOf('=');
    if (idx>=0) {
        QString colName=currentLine.mid(idx+1);
        if (colName.startsWith("\"")) colName=colName.right(colName.size()-1);
        if (colName.endsWith("\"")) colName=colName.left(colName.size()-1);

        QColor col=jkqtp_String2QColor(colName);
        col.setAlpha(255);
        const QString targetText=jkqtp_QColor2String(col)+","+newTrans;
        const QString newLine=currentLine.left(idx+1)+targetText;
        curContextMenu.insertText(newLine);
    }
}

void JKQTPStylePlainTextEdit::addMathFontSpecifier(const QString& newMath)
{
    const QString currentLine=curContextMenu.selectedText();
    //qDebug()<<"current line: '"<<currentLine<<"'";
    const int idx=currentLine.indexOf('=');
    const int idxp=currentLine.indexOf('+');
    if (idx>=0) {
        QString fontName=currentLine.mid(idx+1);
        if (idxp>=0) fontName=currentLine.mid(idx+1, idxp-idx-1);
        const QString targetText=fontName+"+"+newMath;
        const QString newLine=currentLine.left(idx+1)+targetText;
        curContextMenu.insertText(newLine);
    }
}

