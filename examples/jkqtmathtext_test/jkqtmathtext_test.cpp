#include <QApplication>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QScrollArea>
#include <QDebug>
#include <QHBoxLayout>
#include <QTextBrowser>
#include "jkqtmathtext/jkqtmathtext.h"
#include "jkqtplottertools/jkqtphighrestimer.h"
#include "testform.h"


#define W 2000
#define H 3000
#define X1 15
#define Y1 50

/*
HighResTimer ht;


double draw(QPainter& painter, double X, double YY, JKQTMathText& mt, QString name) {
    double Y=YY;
    painter.save();
    ht.start();
    QSizeF s=mt.getSize(painter);
    Y=Y+mt.getAscent(painter);
    std::cout<<"    sizing in "<<ht.get_time()/1000.0<<" ms\n";
    QPen p=painter.pen();
    p.setColor("lightcoral");
    p.setStyle(Qt::DashLine);
    p.setWidth(2);
    painter.setPen(p);
    QRectF r(X, Y-mt.getAscent(painter),s.width(), s.height());
    painter.drawRect(r);
    p.setColor("lightblue");
    painter.setPen(p);
    painter.drawLine(X, Y, X+s.width(), Y);
    ht.start();
    p.setStyle(Qt::SolidLine);
    p.setWidth(1);
    p.setColor("black");
    painter.setPen(p);
    mt.draw(painter, X, Y);
    std::cout<<"   drawing in "<<ht.get_time()/1000.0<<" ms\n";
    p.setColor("blue");
    painter.setPen(p);

    QFont f;
    f.setFamily("sans serif");
    f.setUnderline(true);
    f.setPointSize(10);
    painter.setFont(f);
    painter.drawText(X, Y-mt.getAscent(painter)-6, name+":");
    painter.restore();
    std::cout<<name.toStdString()<<":  width="<<s.width()<<"  height="<<s.height()<<"  ascent="<<mt.getAscent(painter)<<"  descent="<<mt.getDescent(painter)<<std::endl;
    return mt.getDescent(painter)+mt.getAscent(painter)+40;
}

void timingTest(QPainter& painter, QString text, QString name, double fontSize) {
    double sum_parse=0;
    double sum_size=0;
    double sum_draw=0;
    double sqrsum_parse=0, sqrsum_size=0, sqrsum_draw=0;
    int N=20;
    std::cout<<"testing '"<<name.toStdString()<<"':\nparse [ms],\tsize [ms],\tdraw [ms]\n---------------------------------------------\n";
    for (int i=0; i<N; i++) {
        double t;
        JKQTMathText mt;
        mt.set_fontSize(fontSize);
        ht.start();    mt.parse(text);         t=ht.get_time()/1000.0;
        sum_parse+=t;
        sqrsum_parse+=t*t;
        //std::cout<<t<<",\t";
        ht.start();    mt.getSize(painter);    t=ht.get_time()/1000.0;
        sum_size+=t;
        sqrsum_size+=t*t;
        //std::cout<<t<<",\t";
        ht.start();    mt.draw(painter, 0,0);  t=ht.get_time()/1000.0;
        sum_draw+=t;
        sqrsum_draw+=t*t;
        //std::cout<<t<<"\n";
    }
    //std::cout<<"---------------------------------------------\n";
    std::cout<<sum_parse/(double)N<<",\t"<<sum_size/(double)N<<",\t"<<sum_draw/(double)N<<"\t: average\n";
    std::cout<<sqrt(sqrsum_parse-sum_parse*sum_parse/(double)N)/(double)N<<",\t"<<sqrt(sqrsum_size-sum_size*sum_size/(double)N)/(double)N<<",\t"<<sqrt(sqrsum_draw-sum_draw*sum_draw/(double)N)/(double)N<<"\t: std dev\n";
    std::cout<<"\n\n";
}

int main(int argc, char* argv[])
{
    std::cout<<sizeof(unsigned char)<<"  "<<sizeof(unsigned short)<<"  "<<sizeof(unsigned int)<<"  "<<sizeof(unsigned long)<<"  "<<sizeof(unsigned long long)<<"\n\n";


    QString mathTest="This is normal text: $this is math:\\langle r^2(\\tau)\\rangle=\\left\\langle (\\vec{r}(t)-\\vec{r}(t+\\tau) )^2\\right\\rangle\\ \\ \\ g(\\tau)=\\frac{1}{N}\\cdot\\left(1+\\frac{2}{3}\\frac{\\langle r^2(\\tau)\\rangle}{w_{xy}^2}\\right)^{-1}  \\lfloor\\rfloor\\lceil\\rceil\\langle\\rangle\\left\\{\\va\\left|\\|\\va\\|_2\\geq2\\right.\\right\\} \\vr\\vR\\frac{\\sqrt{\\sqrt{\\sqrt{\\sum_{i=0}^\\infty \\hat{i}^2}+y^\\alpha}+1}}{\\dot{v}\\equiv\\ddot{r}}\\argmin_{\\vec{k}}\\sum_{\\sqrt{i}=0}^{N}\\int_{x_0}^{x_1}\\left(\\left(\\left(x\\right)\\right)\\right)\\underbrace{\\left[\\left\\{\\frac{\\partial f}{\\partial x}\\right\\}\\cdot\\frac{1}{2}\\right]}{\\text{underbraced text \\hbar}}\\cdots\\frac{\\sqrt{\\sum_{i=0}^2 \\hat{i}^2}+y^\\alpha}{\\dot{v}\\equiv\\ddot{r}}, \\hat{t}\\hat{T} \\overbrace{\\left|\\sqrt{x\\cdot Y}\\right|}{\\propto\\bbN\\circ\\bbZ}   \\left<\\arrow{x(\\tau)}\\cdot\\vec{R}(t+\\bar{\\tau})\\right>   \\alpha\\beta\\gamma\\delta\\epsilon\\Gamma\\Delta\\Theta\\Omega  \\left\\_\\left~\\cbrt{\\hbar\\omega}\\right~\\right\\_$";
    QString symbolTest="\\lfloor\\rfloor\\lceil\\rceil\\langle\\rangle\\sum\\int \\iint \\oint \\prod \\leftrightarrow \\leftarrow\\Leftarrow\\rightarrow\\Rightarrow\\pm\\mp\\leq\\geq\\ll\\gg\\hbar\\euro\\bbC\\bbH\\bbN\\bbP\\bbQ\\bbZ\\bbR\\Angstrom\\Alef\\Bet\\Gimel\\Dalet\\nexists\\ni\\notni\\circ\\tilde\\oiint\\oiiint\\emptyset\\odot\\ominus\\subsetnot\\DC\\bot\\cdots\\perthousand\\leftharpoonup\\rightharpoonup \\upharpoonleft \\downharpoonleft \\leftrightharpoon \\rightleftharpoon \\coprod \\leftharpoondown \\rightharpoondown \\nwarrow \\nearrow \\mapsto \\cent \\pound \\yen \\div \\multimap \\maporiginal \\mapimage \\bigcap \\bigcup \\benzene \\times \\cdot \\propto \\equiv \\Im \\Re \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ ";
    //QString mathTest="$\\left\\|\\stackrel{1}{2}\\right\\|\\cdot\\left|\\begin{Bmatrix}1123&2^2&\\frac{3}{4}\\\\4&5_5&6\\\\72_8&8_{\\text{längerer Index}}&9&10&11\\end{Bmatrix}\\right|\\cdots f(x)$";
    //QString mathTest="0";
    //QString text="x_0 \\sqrt{a^2+b^2}\\underbrace{underbrace}{x_0} (\\bf{Test})\\_\\it{Text} ... ! Äquator\\alpha\\beta $f(x)=\\sin(x{\\cdot}y)$ \\ul{Text\\ol{\\gamma}} \\script{Script}, \\tt{Typewriter}, \\sf{sans serif}";
    //QString text="$\\sum_{i=0_{j=-1_{k=-45}}}^{n^{m^{123456789}}}\\left(\\left(\\left(\\int_0^\\inf\\left[r+i^{\\left\\{2^2\\right\\}}\\right]{\\dd}r\\right)\\right)\\right)$ \\bf{Test}\\_\\it{Text} ... ! \\sc{&Äquator \\&}\\alpha\\beta $f(x)=\\sin(x\\cdot\\cos(a+{\\ii}b))$ \\ul{Text\\gamma} \\script{Script-Text}\\tt{ and Typewriter} \\sf{also sans serif}";
    //QString text="$\\sum_{i=0_{j=-1_{k=-45}}}^2\\left(\\left(\\left(\\int_0^\\inf\\left[r+i^{\\left\\{2^2\\right\\}}\\right]{\\dd}r\\right)\\right)\\right)$ \\bf{Test}\\_\\it{Text} ... ! \\sc{&Äquator \\&}\\alpha\\beta $f(x)=\\sin(x\\cdot\\cos(a+{\\ii}b))$ \\ul{Text\\gamma} \\script{Script-Text}\\tt{ and Typewriter} \\sf{also sans serif}";

    QString text="x_0My Text$\\sqrt{a^2+b^2 } \\underbrace{underbrace}{x_0}$";

    QApplication app(argc, argv);

    QWidget w;
    QHBoxLayout lay(&w);
    w.setLayout(&lay);
    QScrollArea area;
    QTextBrowser browser;
    lay.addWidget(&area);
    lay.addWidget(&browser);
    QLabel win;
    area.setBackgroundRole(QPalette::Dark);
    area.setWidget(&win);
    QPixmap pix(W, H), pix2(W,H);
    pix.fill();
    QPainter painter;
    JKQTMathText mt(&win);

    double Y=Y1;

    painter.begin(&pix);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    ht.start();

    mt.set_fontRoman("Times New Roman");
    mt.set_fontMathRoman("Times New Roman");
    mt.set_fontSans("Arial");
    mt.set_fontMathSans("Arial");
    mt.set_fontTypewriter("Courier");
    mt.parse(mathTest);
    bool okh=true;
    browser.textCursor().insertHtml("<hr>"+mt.toHtml(&okh)+"<hr><br><br>");
    qDebug()<<"HTML1: ---------------------------------------------\n"<<mt.toHtml(&okh)<<"\nHTML1: --------------------------------------------- ok="<<okh;
    if (mt.get_error_list().size()>0) {
        std::cout<<mt.get_error_list().join("\n").toStdString()<<std::endl<<std::endl;
    }
    std::cout<<"parse mathTest in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "math, symbol, 10pt");
    mt.set_fontSize(20);
    Y+=draw(painter, X1, Y, mt, "math, symbol, 20pt");
    ht.start();
    mt.parse(symbolTest);
    std::cout<<"parse symbolTest in "<<ht.get_time()/1000.0<<" ms\n";
    browser.textCursor().insertHtml("<hr>"+mt.toHtml(&okh)+"<hr><br><br>");
    qDebug()<<"HTML2: ---------------------------------------------\n"<<mt.toHtml(&okh)<<"\nHTML2: --------------------------------------------- ok="<<okh;
    mt.set_fontSize(12);
    Y+=draw(painter, X1, Y, mt, "math, XITS, 12pt");
    ht.start();
    mt.parse(text);
    std::cout<<"parse text in "<<ht.get_time()/1000.0<<" ms\n";
    browser.textCursor().insertHtml("<hr>"+mt.toHtml(&okh)+"<hr><br><br>");
    qDebug()<<"HTML2: ---------------------------------------------\n"<<mt.toHtml(&okh)<<"\nHTML2: --------------------------------------------- ok="<<okh;
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "text, symbol, 10pt");
    mt.set_fontSize(16);
    Y+=draw(painter, X1, Y, mt, "text, symbol, 16pt");

    Y+=30;
    mt.useXITS();
    ht.start();
    mt.parse(mathTest);
    std::cout<<"parse mathTest in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "math, XITS, 10pt");
    mt.set_fontSize(20);
    Y+=draw(painter, X1, Y, mt, "math, XITS, 20pt");
    ht.start();
    mt.parse(symbolTest);
    std::cout<<"parse symbolTest in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(12);
    Y+=draw(painter, X1, Y, mt, "math, XITS, 12pt");
    ht.start();
    mt.parse(text);
    std::cout<<"parse text in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "text, XITS, 10pt");
    mt.set_fontSize(16);
    Y+=draw(painter, X1, Y, mt, "text, XITS, 16pt");



    Y+=30;
    mt.useAnyUnicode("Arial", "Times New Roman");
    ht.start();
    mt.parse(mathTest);
    std::cout<<"parse mathTest in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "math, Arial Unicode, 10pt");
    mt.set_fontSize(20);
    Y+=draw(painter, X1, Y, mt, "math, Arial Unicode, 20pt");
    ht.start();
    mt.parse(symbolTest);
    std::cout<<"parse symbolTest in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(12);
    Y+=draw(painter, X1, Y, mt, "math, Arial Unicode, 12pt");
    ht.start();
    mt.parse(text);
    std::cout<<"parse text in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "text, Arial Unicode, 10pt");
    mt.set_fontSize(16);
    Y+=draw(painter, X1, Y, mt, "text, Arial Unicode, 16pt");

    Y+=30;
    mt.useSTIX();
    ht.start();
    mt.parse(mathTest);
    std::cout<<"parse mathTest in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "math, STIX, 10pt");
    mt.set_fontSize(20);
    Y+=draw(painter, X1, Y, mt, "math, STIX, 20pt");
    ht.start();
    mt.parse(symbolTest);
    std::cout<<"parse symbolTest in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(12);
    Y+=draw(painter, X1, Y, mt, "math, STIX, 12pt");
    ht.start();
    mt.parse(text);
    std::cout<<"parse text in "<<ht.get_time()/1000.0<<" ms\n";
    mt.set_fontSize(10);
    Y+=draw(painter, X1, Y, mt, "text, STIX, 10pt");
    mt.set_fontSize(16);
    Y+=draw(painter, X1, Y, mt, "text, STIX, 16pt");





    painter.end();


    // timing test
    painter.begin(&pix2);
    std::cout<<"timing test, font size 10pt:\n";
    timingTest(painter, mathTest, "mathTest", 10);
    timingTest(painter, text, "text", 10);
    std::cout<<"timing test, font size 20pt:\n";
    timingTest(painter, mathTest, "mathTest", 20);
    timingTest(painter, text, "text", 20);
    std::cout<<"timing test, font size 30pt:\n";
    timingTest(painter, mathTest, "mathTest", 30);
    timingTest(painter, text, "text", 30);


    win.setPixmap(pix);

    win.resize(W, H);
    w.show();
    w.showMaximized();

    return app.exec();
}
*/



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    TestForm w;
    w.show();
    w.updateMath();
    w.showMaximized();

    return app.exec();
}
