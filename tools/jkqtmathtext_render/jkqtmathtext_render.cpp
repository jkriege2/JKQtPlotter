/** \example jkqtmathtext_render.cpp
 * A very basic example for the usage of JKQTMathText
 *
 * \ref JKQTMathTextRenderCmdLineTool

 */
 
#include <QApplication>
#include <QPixmap>
#include <QPainter>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QElapsedTimer>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <algorithm>
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#else
#include<QRegExp>
#endif
#include "jkqtmathtext/jkqtmathtextlabel.h"
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtmathtext/nodes/jkqtmathtextsymbolnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextnodetools.h"
#include <iostream>

void processFont(const QString font, QStringList& fonts, QString& mathFont)
{
    fonts.clear();
    mathFont="";
    QString current="";
    bool isExtra=false;
    for (int i=0; i<font.size(); i++) {
        if (font[i]==',') {
            fonts.push_back(current);
            current="";
        } else if (font[i]=='+') {
            fonts.push_back(current);
            isExtra=true;
            current="";
        } else {
            if (isExtra) {
                mathFont+=font[i];
            } else {
                current+=font[i];
            }
        }
    }
    if (current.size()>0) {
        fonts.append(current);
    }
}

int main(int argc, char* argv[])
{
    // 1. create Qt Appcilation object and a QCommandLineParser to go with it
    QApplication app(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("JKQTMathText command line tool that renders LaTeX markup into an image");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("latex", "LaTeX markup to be rendered");
    parser.addPositionalArgument("output", "output image file (extensions determines file type)");
    QCommandLineOption inputfileOption("inputfile", "read from a file instead of from command line.", "inputfile", "");
    parser.addOption(inputfileOption);
    QCommandLineOption outputDirectoryOption("outputdir", "write results into this directory.", "outputdir", app.applicationDirPath());
    parser.addOption(outputDirectoryOption);
    QCommandLineOption listsymbolsOption("listsymbols", "list all symbols in the given output file and generate images.", "listsymbols", "");
    parser.addOption(listsymbolsOption);
    QCommandLineOption listsymbolsfontsOption("listsymbolsfonts", "fonts for listsymbols.", "listsymbolsfonts", "XITS");
    parser.addOption(listsymbolsfontsOption);
    QCommandLineOption drawBoxesOption(QStringList()<<"drawboxes"<<"showboxes", "draw boxes.");
    parser.addOption(drawBoxesOption);
    QCommandLineOption verboseOption("verbose", "verbose output.");
    parser.addOption(verboseOption);
    QCommandLineOption guifontsOption("guifonts", "use GUI fonts.");
    parser.addOption(guifontsOption);
    QCommandLineOption fontOption("font", "font( size)s) to use.", "font", "XITS");
    parser.addOption(fontOption);
    QCommandLineOption fontsizeOption("fontsize", "font size.", "fontsize", "12");
    parser.addOption(fontsizeOption);
    QCommandLineOption fontRomanOption("fontroman", "set the text-mode roman font to use.", "fontroman", "");
    parser.addOption(fontRomanOption);
    QCommandLineOption fontSansOption("fontsans", "set the text-mode sans font to use.", "fontsans", "");
    parser.addOption(fontSansOption);
    QCommandLineOption fontMathRomanOption("fontmathroman", "set the math-mode roman font to use.", "fontmathroman", "");
    parser.addOption(fontMathRomanOption);
    QCommandLineOption fontMathSansOption("fontmathsans", "set the math-mode sans font to use.", "fontmathsans", "");
    parser.addOption(fontMathSansOption);
    QCommandLineOption fontFallbackSymbolOption("fontfallbacksymbol", "set the fallback symbol font to use.", "fontfallbacksymbol", "");
    parser.addOption(fontFallbackSymbolOption);
    QCommandLineOption fontFallbackSymbol_symbolencodingOption("fontfallbacksymbol_symbolencoding", "set the fallback symbol font to use and use the encoding of the MS Symbol font for it.", "fontfallbacksymbol_symbolencoding", "");
    parser.addOption(fontFallbackSymbol_symbolencodingOption);
    QCommandLineOption fontTypewriterOption("fonttypewriter", "set the typewriter font to use.", "fonttypewriter", "");
    parser.addOption(fontTypewriterOption);
    QCommandLineOption fontScriptOption("fontscript", "set the Script font to use.", "fontscript", "");
    parser.addOption(fontScriptOption);
    QCommandLineOption fontFrakturOption("fontfraktur", "set the Fraktur font to use.", "fontfraktur", "");
    parser.addOption(fontFrakturOption);
    QCommandLineOption fontcaligraphicOption("fontcaligraphic", "set the caligraphic font to use.", "fontcaligraphic", "");
    parser.addOption(fontcaligraphicOption);
    QCommandLineOption fontblackboardOption("fontblackboard", "set the blackboard font to use.", "fontblackboard", "");
    parser.addOption(fontblackboardOption);
    QCommandLineOption fontblackboardmodeOption("fontblackboardmode", "set the usage mode for the blackboard font.", "fontblackboardmode", "default");
    parser.addOption(fontblackboardmodeOption);
    QCommandLineOption textcolorOption("textcolor", "set the color of the text.", "textcolor", "black");
    parser.addOption(textcolorOption);
    QCommandLineOption sizeincreaseOption("sizeincrease", "additional pixels around output.", "sizeincrease", "2");
    parser.addOption(sizeincreaseOption);
    QCommandLineOption resolutionOption("resolution", "set output image resolution in DPI.", "resolution", "96");
    parser.addOption(resolutionOption);
    QCommandLineOption backgroundOption("background", "background color.", "background", "white");
    parser.addOption(backgroundOption);
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    const QString latex_cmdline=args.value(0, "$x_{1/2}=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a}$");
    const QDir outputDir(parser.value(outputDirectoryOption));
    const QString outputFilename_cmdline=outputDir.absoluteFilePath(args.value(1, "output.png"));
    const QString inputfile=parser.value(inputfileOption);
    const QString listsymbols=parser.value(listsymbolsOption);
    const QStringList listsymbolsfonts=parser.value(listsymbolsfontsOption).split(',');
    const bool verbose = parser.isSet(verboseOption);

    QStringList latex, outputFilename;
    QList<QMap<QString,QString>> cmdoptions;
    if (listsymbols.size()>0) {
        //std::cout<<"LISTSYMBOLS: "<<outputDir.absoluteFilePath(listsymbols).toStdString()<<"\n";
        QFile fileListF(outputDir.absoluteFilePath(listsymbols));
        if (fileListF.open(QFile::WriteOnly|QFile::Text)) {
            QTextStream fileList(&fileListF);
            QStringList symbolsAll=JKQTMathTextSymbolNode::getSymbols();
            QSet<QString> symbolsRemaining;
            for (const QString& s: symbolsAll) symbolsRemaining.insert(s);
            std::sort(symbolsAll.begin(), symbolsAll.end(), [](const QString& a, const QString& b) { const QString al=a.toLower(); const QString bl=b.toLower(); if (al==bl) { return a<b; } else return al<bl; });
            static QStringList greekLetterNames=QStringList()<<"alpha"<<"beta"<<"gamma"<<"delta"<<"epsilon"<<"zeta"<<"eta"<<"theta"<<"iota"<<"kappa"<<"lambda"<<"mu"<<"nu"<<"xi"<<"pi"<<"rho"<<"sigma"<<"tau"<<"upsilon"<<"phi"<<"chi"<<"psi"<<"omega";
            static QStringList arrowNames=QStringList()<<"arrow"<<"harpoon"<<"mapsto";
            int i=1;
            fileList<<"/*!\n"
                      "   \\defgroup jkqtmathtext_supportedlatexsymbols Supported LaTeX-Symbols\n"
                      "   \\ingroup jkqtmathtext_general_latex\n\n\\tableofcontents\n\n";
            fileList<<"   Symbols are shown in these fonts:\n"
                      "   <ul>\n";
            for (const QString& fnt: listsymbolsfonts) {
                fileList<<"     <li>"<<fnt<<"</li>\n";
            }
            fileList<<"   </ul>\n\n\n";
            fileList<<"   \\section jkqtmathtext_supportedlatexsymbols_greek Greek Letters\n";
            fileList<<"   The following table lists all greek letters and their variants available in JKQTMathParser. They are defined in the node-class JKQTMathTextSymbolNode:\n";
            fileList<<"   <table>\n";
            fileList<<"     <tr>\n";
            for (const QString& gletter: greekLetterNames) {
                QString code="";
                for (const QString& symbol: symbolsAll) {
                    if (symbol.size()>0 && (symbol.toLower()==gletter || symbol.toLower()=="var"+gletter) && symbol[0].isLetter()) {
                        code+="\\"+symbol+"\\;";
                        symbolsRemaining.remove(symbol);
                    }
                }
                for (const QString& symbol: symbolsAll) {
                    if (symbol.size()>0 && (symbol.toLower()=="up"+gletter || symbol.toLower()=="upvar"+gletter) && symbol[0].isLetter()) {
                        code+="\\"+symbol+"\\;";
                        symbolsRemaining.remove(symbol);
                    }
                }
                for (const QString& symbol: symbolsAll) {
                    if (symbol.size()>0 && (symbol.toLower()=="text"+gletter || symbol.toLower()=="textvar"+gletter) && symbol[0].isLetter()) {
                        code+="\\"+symbol+"\\;";
                        symbolsRemaining.remove(symbol);
                    }
                }
                QString symbol_lower;
                for (const QChar& ch: gletter) {
                    if (ch.isUpper()) symbol_lower+=QString(2, ch).toLower();
                    else symbol_lower+=ch;
                }
                for (const QString& fnt: listsymbolsfonts) {
                    latex.append(code+"\\vphantom{Iq}");
                    outputFilename.append("jkqtmathtext_greek_"+symbol_lower+"_"+fnt+".png");
                    QMap<QString,QString> localoptions;
                    localoptions["fontmathroman"]=fnt;
                    cmdoptions.append(localoptions);
                }
                //std::cout<<"  - "<<latex.last().toStdString()<<": "<<outputFilename.last().toStdString()<<"\n";
                code=code.replace("\\;", " ");
                code=code.replace("\\", "\\\\");
                fileList<<"       <td><code>"<<code<<"</code>:\n       <td> ";
                for (int i=0; i< listsymbolsfonts.size(); i++) {
                    if (i>0) fileList<<"            ";

                    fileList<<"\\image html jkqtmathtext/symbols/"<<outputFilename[outputFilename.size()-listsymbolsfonts.size()+i]<<"\n";
                }
                //if (i%2==0) {
                    fileList<<"     </tr>\n     <tr>\n";
                //}
                i++;
            }
            fileList<<"     </tr>";
            fileList<<"   </table>\n\n\n";


            fileList<<"   \\section jkqtmathtext_supportedlatexsymbols_arrows Arrows\n";
            fileList<<"   The following table lists all arrow-typed symbols available in JKQTMathParser. They are defined in the node-class JKQTMathTextSymbolNode:\n";
            fileList<<"   <table>\n";
            fileList<<"     <tr>\n";
            i=1;

            auto myIsLower=[](const QString& s) { for (int i=0; i<s.size(); i++) if (!s[i].isLower()) return false; return true; };
            auto myIsUpper=[](const QString& s) { for (int i=0; i<s.size(); i++) if (!s[i].isUpper()) return false; return true; };
#if (QT_VERSION>=QT_VERSION_CHECK(6, 0, 0))
            std::sort
#else
            qSort
#endif
            (symbolsAll.begin(), symbolsAll.end(), [myIsLower,myIsUpper](const QString& a, const QString& b) {  if (a.contains("harpoon") && !b.contains("harpoon")) return false;
                                                                                                else if (myIsLower(a) && myIsUpper(b)) return true;
                                                                                                else if (myIsUpper(a) && myIsLower(b)) return false;
                                                                                                else return a<b;
                                                                                             });
            for (const QString& arrow: arrowNames) {
                QString code="";
                for (const QString& symbol: symbolsAll) {
                    if (symbol.size()>0 && symbol.toLower().contains(arrow) && symbol[0].isLetter()) {
                        symbolsRemaining.remove(symbol);
                        QString symbol_lower;
                        for (const QChar& ch: symbol) {
                            if (ch.isUpper()) symbol_lower+=QString(2, ch).toLower();
                            else symbol_lower+=ch;
                        }
                        for (const QString& fnt: listsymbolsfonts) {
                            latex.append("\\"+symbol+"\\vphantom{Iq}");
                            outputFilename.append("jkqtmathtext_greek_"+symbol_lower+"_"+fnt+".png");
                            QMap<QString,QString> localoptions;
                            localoptions["fontmathroman"]=fnt;
                            cmdoptions.append(localoptions);
                        }
                        fileList<<"       <td><code>\\\\"<<symbol<<"</code>:\n";
                        fileList<<"       <td>";
                        for (int i=0; i< listsymbolsfonts.size(); i++) {
                            if (i>0) fileList<<"            ";

                            fileList<<"\\image html jkqtmathtext/symbols/"<<outputFilename[outputFilename.size()-listsymbolsfonts.size()+i]<<"\n";
                        }
                        if (i%3==0) {
                            fileList<<"     </tr>\n     <tr>\n";
                        }
                        i++;
                    }
                }
            }
            fileList<<"     </tr>";
            fileList<<"   </table>\n\n\n";

            std::sort(symbolsAll.begin(), symbolsAll.end(), [](const QString& a, const QString& b) { const QString al=a.toLower(); const QString bl=b.toLower(); if (al==bl) { return a<b; } else return al<bl; });
            fileList<<"   \\section jkqtmathtext_supportedlatexsymbols_bigmathop Big Math Operators\n";
            fileList<<"   The following table lists all math-operator symbols like \\c \\\\sum,\\\\int,\\\\lim... that allow to over/underset sub-/superscript using \\c \\\\limits (and \\c \\\\nolimits ) available in JKQTMathParser. They are defined in the node-class JKQTMathTextSymbolNode:\n";
            fileList<<"   <table>\n";
            fileList<<"     <tr>\n";
            i=1;
            {
                for (const QString& symbol: symbolsAll) {
                    if (symbol.size()>0 && JKQTMathTextSymbolNode::isSubSuperscriptBelowAboveSymbol(symbol) && symbol[0].isLetter()) {
                        symbolsRemaining.remove(symbol);
                        QString symbol_lower;
                        for (const QChar& ch: symbol) {
                            if (ch.isUpper()) symbol_lower+=QString(2, ch).toLower();
                            else symbol_lower+=ch;
                        }
                        static QSet<QString> specialSymbols=QSet<QString>()<<"lim"<<"liminf"<<"limsup"<<"arg"<<"argmin"<<"argmax";
                        QString code="";
                        if (JKQTMathTextSymbolNode::getSymbolLength(symbol)>1 && !specialSymbols.contains(symbol)) {
                            code="$\\"+symbol+"(\\cdot)$";
                        } else {
                            if (JKQTMathTextSymbolNode::getSymbolLength(symbol)>1) {
                                code="$\\"+symbol+"\\limits_{x\\rightarrow\\infty}f(x)$";
                            } else {
                                code="$\\"+symbol+"\\limits_{x=0}^\\infty f(x)$";
                            }
                        }
                        for (const QString& fnt: listsymbolsfonts) {
                            latex.append(code+"\\vphantom{Iq}");
                            outputFilename.append("jkqtmathtext_greek_"+symbol_lower+"_"+fnt+".png");
                            QMap<QString,QString> localoptions;
                            localoptions["fontmathroman"]=fnt;
                            cmdoptions.append(localoptions);
                        }
                        fileList<<"       <td><code>\\\\"<<symbol<<"</code>:\n";
                        fileList<<"       <td>";
                        for (int i=0; i< listsymbolsfonts.size(); i++) {
                            if (i>0) fileList<<"            ";

                            fileList<<"\\image html jkqtmathtext/symbols/"<<outputFilename[outputFilename.size()-listsymbolsfonts.size()+i]<<"\n";
                        }
                        if (i%3==0) {
                            fileList<<"     </tr>\n     <tr>\n";
                        }
                        i++;
                    }
                }
            }
            fileList<<"     </tr>";
            fileList<<"   </table>\n\n\n";


            fileList<<"   \\section jkqtmathtext_supportedlatexsymbols_mathspecialfunctions Math Special Functions\n";
            fileList<<"   The following table lists all remaining math-operator symbols like available in JKQTMathParser. They are defined in the node-class JKQTMathTextSymbolNode:\n";
            fileList<<"   <table>\n";
            fileList<<"     <tr>\n";
            i=1;
            {
                for (const QString& symbol: symbolsAll) {
                    if (symbol.size()>0 && symbolsRemaining.contains(symbol) && JKQTMathTextSymbolNode::isExtendedWidthSymbol(symbol) && JKQTMathTextSymbolNode::getSymbolLength(symbol)>1 && symbol[0].isLetter()) {
                        symbolsRemaining.remove(symbol);
                        QString symbol_lower;
                        for (const QChar& ch: symbol) {
                            if (ch.isUpper()) symbol_lower+=QString(2, ch).toLower();
                            else symbol_lower+=ch;
                        }
                        for (const QString& fnt: listsymbolsfonts) {
                            latex.append("$\\"+symbol+"(\\cdot)$\\vphantom{Iq}");
                            outputFilename.append("jkqtmathtext_greek_"+symbol_lower+"_"+fnt+".png");
                            QMap<QString,QString> localoptions;
                            localoptions["fontmathroman"]=fnt;
                            cmdoptions.append(localoptions);
                        }
                        fileList<<"       <td><code>\\\\"<<symbol<<"</code>:\n";
                        fileList<<"       <td>";
                        for (int i=0; i< listsymbolsfonts.size(); i++) {
                            if (i>0) fileList<<"            ";

                            fileList<<"\\image html jkqtmathtext/symbols/"<<outputFilename[outputFilename.size()-listsymbolsfonts.size()+i]<<"\n";
                        }
                        if (i%3==0) {
                            fileList<<"     </tr>\n     <tr>\n";
                        }
                        i++;
                    }
                }
            }
            fileList<<"     </tr>";
            fileList<<"   </table>\n\n\n";


            fileList<<"   \\section jkqtmathtext_supportedlatexsymbols_other Other Symbols\n";
            fileList<<"   The following table lists all other symbols that are available in JKQTMathParser. They are defined in the node-class JKQTMathTextSymbolNode:\n";
            fileList<<"   <table>\n";
            fileList<<"     <tr>\n";
            i=1;
            QStringList symbolsRemainingL;
            for (const QString& s: symbolsRemaining) symbolsRemainingL.append(s);

            std::sort(symbolsRemainingL.begin(), symbolsRemainingL.end(), [](const QString& a, const QString& b) { const QString al=a.toLower(); const QString bl=b.toLower(); if (al==bl) { return a<b; } else return al<bl; });
            for (const QString& symbol: symbolsRemainingL) {
                if (symbol.size()>0 && symbol[0].isLetter()) {
                    QString symbol_lower;
                    for (const QChar& ch: symbol) {
                        if (ch.isUpper()) symbol_lower+=QString(2, ch).toLower();
                        else symbol_lower+=ch;
                    }
                    const QString code="\\"+symbol+"\\vphantom{Iq}";
                    for (const QString& fnt: listsymbolsfonts) {
                        latex.append(code+"\\vphantom{Iq}");
                        outputFilename.append("jkqtmathtext_greek_"+symbol_lower+"_"+fnt+".png");
                        QMap<QString,QString> localoptions;
                        localoptions["fontmathroman"]=fnt;
                        cmdoptions.append(localoptions);
                    }
                    fileList<<"       <td><code>\\\\"<<symbol<<"</code>:\n";
                    fileList<<"       <td>";
                    for (int i=0; i< listsymbolsfonts.size(); i++) {
                        if (i>0) fileList<<"            ";

                        fileList<<"\\image html jkqtmathtext/symbols/"<<outputFilename[outputFilename.size()-listsymbolsfonts.size()+i]<<"\n";
                    }
                    if (i%3==0) {
                        fileList<<"     </tr>\n     <tr>\n";
                    }
                    i++;
                }
            }
            fileList<<"     </tr>";
            fileList<<"   </table>\n";            fileList<<"*/\n";
        }
    } else if (inputfile.size()>0){
        QFile f(inputfile);
        if (f.open(QFile::ReadOnly|QFile::Text)) {
            QString currentOutFile="";
            QString currentLatex="";
            QMap<QString,QString> currentOptions;
            bool isFile=true;
            bool beforeLatex=true;
            while (!f.atEnd()) {
                QString line=f.readLine();
                while (line.endsWith('\n') || line.endsWith('\r') || line.endsWith(QChar(0x0))) line=line.left(line.size()-1);
                const QString line_trimmed=line.trimmed();
                const QString line_simple=line_trimmed.simplified();
                if (line_simple=="---" || line_simple=="###") {
                    if (currentOutFile.size()>0) {
                        outputFilename.append(currentOutFile);
                        latex.append(currentLatex);
                        cmdoptions.append(currentOptions);
                        isFile=true;
                        beforeLatex=true;
                        currentLatex="";
                        currentOutFile="";
                        currentOptions.clear();
                    }
                } else if (isFile) {
                    currentOutFile=line_trimmed;
                    isFile=false;
                } else if (beforeLatex) {
                    if (line_trimmed.startsWith("--")) {
                        QStringList commands=line_trimmed.right(line_trimmed.size()-2).split("--");
                        for (QString cmd: commands) {
                            cmd=cmd.trimmed();
                            QString cmdn="", param="";
                            bool iscmd=true;
                            for (const QChar& ch: cmd) {
                                if (iscmd) {
                                    if (ch=='=') {
                                        iscmd=false;
                                    } else {
                                        cmdn+=ch;
                                    }
                                } else {
                                    param+=ch;
                                }
                            }
                            currentOptions[cmdn.trimmed()]=param.trimmed();
                        }
                    } else {
                        if (currentLatex.size()>0) currentLatex+="\n";
                        currentLatex+=line;
                    }
                    beforeLatex=false;
                } else {
                    if (currentLatex.size()>0) currentLatex+="\n";
                    currentLatex+=line;
                }
            }
            f.close();
            if (currentOutFile.size()>0) {
                outputFilename.append(currentOutFile);
                latex.append(currentLatex);
                cmdoptions.append(currentOptions);
            }
        }

    } else {
        latex.append(latex_cmdline);
        outputFilename.append(outputFilename_cmdline);
        cmdoptions.append(QMap<QString,QString>());
    }

    if (verbose) {
        std::cout
            <<"===========================================================\n"
            <<"= jkqtmathtext_render: ";
        if (inputfile.size()>0) std::cout<<"FILE-MODE (read from "<<inputfile.toStdString()<<")\n";
        else if (listsymbols.size()>0)  std::cout<<"LIST-SYMBOLS-MODE (store to "<<listsymbols.toStdString()<<")\n";
        else std::cout<<"COMMAND-LINE-MODE\n";
        std::cout<<"\n"
            <<"===========================================================\n";
    }


    for (int i=0; i<latex.size(); i++) {
        if (inputfile.size()>0 && verbose) {
            std::cout<<"\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
                    <<"+ processing entry "<<(i+1)<<"/"<<latex.size()<<"\n";
            if (cmdoptions[i].size()>0) {
                std::cout<<"+ additional command-line options: ";
                for (const QString& key: cmdoptions[i].keys()) {
                    std::cout<<"  --"<<key.toStdString()<<"="<<cmdoptions[i].value(key).toStdString();
                }
                std::cout<<"\n";
            }

        }
        QStringList fonts;
        QString mathFont="";
        processFont(parser.value(fontOption), fonts, mathFont);
        bool drawBoxes = parser.isSet(drawBoxesOption);
        double fontsize = parser.value(fontsizeOption).toDouble();
        int sizeincrease = parser.value(sizeincreaseOption).toInt();
        int resolution_dpi = parser.value(resolutionOption).toInt();
        QColor backgroundColor = jkqtp_String2QColor(parser.value(backgroundOption));
        QColor textColor = jkqtp_String2QColor(parser.value(textcolorOption));
        QString fontBlackboard=parser.value(fontblackboardOption);
        QString fontRoman=parser.value(fontRomanOption);
        QString fontSans=parser.value(fontSansOption);
        QString fontMathRoman=parser.value(fontMathRomanOption);
        QString fontMathSans=parser.value(fontMathSansOption);
        QString fontFallbackSymbol=parser.value(fontFallbackSymbolOption);
        QString fontFallbackSymbol_symbolencoding=parser.value(fontFallbackSymbol_symbolencodingOption);
        QString fontTypewriter=parser.value(fontTypewriterOption);
        QString fontScript=parser.value(fontScriptOption);
        QString fontFraktur=parser.value(fontFrakturOption);
        QString fontCaligraphic=parser.value(fontcaligraphicOption);
        JKQTMathTextBlackboradDrawingMode fontBlackboardMode=String2JKQTMathTextBlackboradDrawingMode(parser.value(fontblackboardmodeOption));
        bool guiFonts=parser.isSet(guifontsOption);

        if (cmdoptions[i].size()>0) {
            for (const QString& key: cmdoptions[i].keys()) {
                if (key=="drawboxes" || key=="showboxes") drawBoxes=true;
                else if (key=="nodrawboxes" || key=="noshowboxes") drawBoxes=false;
                else if (key=="guifonts") guiFonts=true;
                else if (key=="noguifonts") guiFonts=false;
                else if (key=="fontsize") fontsize=cmdoptions[i].value(key).toDouble();
                else if (key=="sizeincrease") sizeincrease=cmdoptions[i].value(key).toInt();
                else if (key=="background") backgroundColor=jkqtp_String2QColor(cmdoptions[i].value(key));
                else if (key=="textcolor") textColor=jkqtp_String2QColor(cmdoptions[i].value(key));
                else if (key=="fontblackboard") fontBlackboard=cmdoptions[i].value(key);
                else if (key=="font") processFont(cmdoptions[i].value(key), fonts, mathFont);
                else if (key=="fontroman") fontRoman=cmdoptions[i].value(key);
                else if (key=="fontsans") fontSans=cmdoptions[i].value(key);
                else if (key=="fontmathroman") fontMathRoman=cmdoptions[i].value(key);
                else if (key=="fontmathsans") fontMathSans=cmdoptions[i].value(key);
                else if (key=="fonttypewriter") fontTypewriter=cmdoptions[i].value(key);
                else if (key=="fontfallbacksymbol") fontFallbackSymbol=cmdoptions[i].value(key);
                else if (key=="fontfallbacksymbol_symbolencoding") fontFallbackSymbol_symbolencoding=cmdoptions[i].value(key);
                else if (key=="fontscript") fontScript=cmdoptions[i].value(key);
                else if (key=="fontcaligraphic") fontCaligraphic=cmdoptions[i].value(key);
                else if (key=="fontfraktur") fontFraktur=cmdoptions[i].value(key);
                else if (key=="fontblackboardmode") fontBlackboardMode=String2JKQTMathTextBlackboradDrawingMode(cmdoptions[i].value(key));
                else {
                    std::cerr<<"unknown command-line option --"<<key.toStdString()<<" in inputfile\n";
                }
            }
        }

        // 2. now we create a JKQTMathText object and configure it
        JKQTMathText mathText;
        if (fonts.size()==1) {
            mathText.useAnyUnicode(fonts[0], mathText.getFontSans());
        } else if (fonts.size()==2) {
            mathText.useAnyUnicode(fonts[0], fonts[1]);
        } else if (fonts.size()==4) {
            mathText.useAnyUnicodeForTextOnly(fonts[0], fonts[1]);
            mathText.useAnyUnicodeForMathOnly(fonts[2], fonts[3]);
        }
        if (mathFont.toUpper() == "XITS") mathText.useXITS(true);
        else if (mathFont.toUpper() == "XITS_MATHANDTEXT") mathText.useXITS(false);
        if (mathFont.toUpper() == "STIX") mathText.useSTIX(true);
        else if (mathFont.toUpper() == "STIX_MATHANDTEXT") mathText.useSTIX(false);
        if (mathFont.toUpper() == "ASANA") mathText.useASANA(true);
        else if (mathFont.toUpper() == "ASANA_MATHANDTEXT") mathText.useASANA(false);
        if (mathFont.toUpper() == "FIRA") mathText.useFiraMath(true);
        else if (mathFont.toUpper() == "FIRA_MATHANDTEXT") mathText.useFiraMath(false);
        if (fontRoman.size()>0) mathText.setFontRoman(fontRoman, MTFEUnicode);
        if (fontSans.size()>0) mathText.setFontSans(fontSans, MTFEUnicode);
        if (fontMathRoman.size()>0) {
            if (fontMathRoman.toUpper()=="XITS") mathText.useXITS(true);
            else if (fontMathRoman.toUpper()=="STIX") mathText.useSTIX(true);
            else if (fontMathRoman.toUpper()=="ASANA") mathText.useASANA(true);
            else if (fontMathRoman.toUpper()=="FIRA") mathText.useFiraMath(true);
            else if (fontMathRoman.toUpper()=="XITS_MATHANDTEXT") mathText.useXITS(false);
            else if (fontMathRoman.toUpper()=="STIX_MATHANDTEXT") mathText.useSTIX(false);
            else if (fontMathRoman.toUpper()=="ASANA_MATHANDTEXT") mathText.useASANA(false);
            else if (fontMathRoman.toUpper()=="FIRA_MATHANDTEXT") mathText.useFiraMath(false);
            else mathText.setFontMathRoman(fontMathRoman, MTFEUnicode);
        }
        if (fontMathSans.size()>0) mathText.setFontMathSans(fontMathSans, MTFEUnicode);
        if (fontTypewriter.size()>0) mathText.setFontTypewriter(fontTypewriter, MTFEUnicode);
        if (fontScript.size()>0) mathText.setFontScript(fontScript, MTFEUnicode);
        if (fontCaligraphic.size()>0) mathText.setFontCaligraphic(fontCaligraphic, MTFEUnicode);
        if (fontFraktur.size()>0) mathText.setFontFraktur(fontFraktur, MTFEUnicode);
        if (fontFallbackSymbol.size()>0) mathText.setFallbackFontSymbols(fontFallbackSymbol, MTFEUnicode);
        if (fontFallbackSymbol_symbolencoding.size()>0) mathText.setFallbackFontSymbols(fontFallbackSymbol_symbolencoding, MTFEWinSymbol);

        if (fontBlackboard.size()>0) {
            mathText.setFontBlackboard(fontBlackboard, MTFEUnicode);
        }
        mathText.setFontBlackboradMode(fontBlackboardMode);
        mathText.setFontSize(fontsize);
        mathText.setFontColor(textColor);
        if (guiFonts) {
            mathText.useGuiFonts();
        }

        // 3. now we parse some LaTeX code
        QElapsedTimer timer;
        timer.start();
        mathText.parse(latex[i]);
        const double durParseMS=static_cast<double>(timer.nsecsElapsed())/1.0e6;
        if (mathText.hadErrors()) {
            std::cerr<<"ERRORS while parsing LaTeX:\n"
                    <<"-----------------------------------------------------------\n"
                    <<latex[i].toStdString()<<"\n"
                    <<"-----------------------------------------------------------\n"
                    <<mathText.getErrorList().join("\n").toStdString()<<"\n"
                   <<"-----------------------------------------------------------\n"
                   <<"RENDERTREE:\n"<<JKQTMathTextNodeTree2String(mathText.getNodeTree()).toStdString()
                   <<"-----------------------------------------------------------\n"
                    ;
        } else if (verbose) {
            std::cout<<"parsing LaTeX: OK\n"
                    <<"parsing duration: "<<durParseMS<<"ms\n"
                    <<"-----------------------------------------------------------\n"
                    <<latex[i].toStdString()<<"\n"
                    <<"-----------------------------------------------------------\n"
                    <<"RENDERTREE:\n"<<JKQTMathTextNodeTree2String(mathText.getNodeTree()).toStdString()
                    <<"-----------------------------------------------------------\n";
        }

        // 4. now we draw the result into a QPixmap
        timer.start();
        const QImage pix=mathText.drawIntoImage(drawBoxes, backgroundColor, sizeincrease, 1.0, resolution_dpi);
        const double durRenderMS=static_cast<double>(timer.nsecsElapsed())/1.0e6;
        if (verbose) std::cout<<"rendering into QImage "<<pix.width()<<"x"<<pix.height()<<"pixels\n"
                              <<"rendering duration: "<<durRenderMS<<"ms\n"
                              <<"rendering resolution: "<<pix.dotsPerMeterX()/(1000.0/25.4)<<"x"<<pix.dotsPerMeterY()/(1000.0/25.4)<<"dpi\n"
                              <<"devicepixelratio: "<<pix.devicePixelRatioF()<<"\n"
                               ;

        // 5. before ending the application, we store the result into a file
        const QString outname=outputDir.absoluteFilePath(outputFilename[i]);
        if (QFileInfo::exists(outname)) QFile::remove(outname);
        if (!pix.save(outname)) {
            std::cerr<<"ERROR storing to "<<outname.toStdString()<<", fileExists="<<std::boolalpha<<QFileInfo::exists(outname)<<" permissions=0x"<<std::hex<<QFileInfo(outname).permissions()<<"\n";
        } else {
            if (verbose) std::cout<<"stored to "<<outname.toStdString()<<"\n"
                                  <<"     size "<<static_cast<double>(QFileInfo(outname).size())/1024.0<<"kBytes\n";
        }
    }
    return EXIT_SUCCESS;
}
