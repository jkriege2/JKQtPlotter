#include "testform.h"
#include "ui_testform.h"
#include <QDebug>
#include <sstream>
#include "jkqtcommon/jkqtpstringtools.h"
#include "jkqtmathtext/nodes/jkqtmathtexttextnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextbracenode.h"
#include "jkqtmathtext/nodes/jkqtmathtextdecoratednode.h"
#include "jkqtmathtext/nodes/jkqtmathtextfracnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextinstructionnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextlistnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextmatrixnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsqrtnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsubsupernode.h"
#include "jkqtmathtext/nodes/jkqtmathtextsymbolnode.h"
#include "jkqtmathtext/nodes/jkqtmathtextwhitespacenode.h"


TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
    const QString testText="ABCabc00, 123-45+6.0\\%\\S";
    const QString umla="\\\"A\\\"a\\`A\\`a\\'A\\'a\\^A\\^a\\~A\\~a\\r{A}\\r{a}\\u{A}\\u{a}\\=A\\=a\\AA\\aa{\\AE}{\\ae}\\v{C}\\v{c}\\\"E\\\"e\\.e\\L\\l\\\"O\\\"o\\`O\\`o\\'O\\'o\\^O\\^o\\~O\\~o\\O\\o{\\OE}{\\ae}\\v{S}\\v{s}\\ss\\\"U\\\"u\\`U\\`u\\'U\\'u\\^U\\^u\\~U\\~u";
    const QString testTextUmla="\\\"Aq{\\\"u}\\\"{a}t{\\oe}r00, 123-45+6.0\\%\\S";
    const QString umla_math="\\ddot{A}\\ddot{a}\\grave{A}\\grave{a}\\acute{A}\\acute{a}\\hat{A}\\hat{a}\\tilde{A}\\tilde{a}\\r{A}\\r{a}\\u{A}\\u{a}\\bar{A}\\bar{a}\\AA\\aa{\\AE}{\\ae}\\check{C}\\check{c}\\ddot{E}\\ddot{e}\\dot{e}\\L\\l\\ddot{O}\\ddot{o}\\grave{O}\\grave{o}\\acute{O}\\acute{o}\\hat{O}\\hat{o}\\tilde{O}\\tilde{o}\\O\\o{\\OE}{\\ae}\\check{S}\\check{s}\\ss\\ddot{U}\\ddot{u}\\grave{U}\\grave{u}\\acute{U}\\acute{u}\\hat{U}\\hat{u}\\tilde{U}\\tilde{u}";
    const QString testTextUmla_math="\\ddot{A}q{\\ddot{u}}\\ddot{{}a}t{\\oe}r00, 123-45+6.0\\%\\S";
    ui->cmbTestset->addItem("text: fonts", "rm: \\textrm{"+testText+"}, sf: \\textsf{"+testText+"}, tt: \\texttt{"+testText+"}, cal: \\textcal{"+testText+"}, scr: \\textscr{"+testText+"}, bb: \\textbb{"+testText+"}, frak: \\textfrak{"+testText+"}, ");
    ui->cmbTestset->addItem("text/math: font comparison",
                            "\\begin{matrix}\n"
                            "  text: & abc123+d/e\\\\\n"
                            "  textit: & \\textit{abc123+d/e}\\\\\n"
                            "  textbf: & \\textbf{abc123+d/e}\\\\\n"
                            "  math: & $abc123+d/e$\\\\\n"
                            "  mathrm: & $\\mathrm{abc123+d/e}$\\\\\n"
                            "  mathit: & $\\mathit{abc123+d/e}$\\\\\n"
                            "  mathbf: & $\\mathbf{abc123+d/e}$\\\\\n"
                            "  mathfrak: & $\\mathfrak{abc123+d/e}$\n"
                            "\\end{matrix}");
    ui->cmbTestset->addItem("text: umlaute", umla);
    ui->cmbTestset->addItem("text: umlaute and fonts", "rm: \\textrm{"+testTextUmla+"}, sf: \\textsf{"+testTextUmla+"}, tt: \\texttt{"+testTextUmla+"}, cal: \\textcal{"+testTextUmla+"}, scr: \\textscr{"+testTextUmla+"}, bb: \\textbb{"+testTextUmla+"}, frak: \\textfrak{"+testTextUmla+"}, ");
    ui->cmbTestset->addItem("text: dashes", "hyphen: - endash: -- emdash: --- \\ \\ \\ endash--within text\\ \\ \\ emdash---within text\\ \\ \\ enemdash-----within text\\ \\ \\ ememdash------within text");
    ui->cmbTestset->addItem("math: fonts", "base: $"+testText+"$, rm: $\\mathrm{"+testText+"}$, sf: $\\mathsf{"+testText+"}$, tt: $\\mathtt{"+testText+"}$, cal: $\\mathcal{"+testText+"}$, scr: $\\mathscr{"+testText+"}$, bb: $\\mathbb{"+testText+"}$, frak: $\\mathfrak{"+testText+"}$, ");
    ui->cmbTestset->addItem("math: umlaute", "$"+umla_math+"$");
    ui->cmbTestset->addItem("math: umlaute and \\text{...}", "\\${\\backslash}text\\{...\\}\\$: $\\text{"+umla+"}\\ \\ \\ \\$...\\$: "+umla+"$");
    ui->cmbTestset->addItem("math: umlaute and fonts", "base: $"+testTextUmla_math+"$, rm: $\\mathrm{"+testTextUmla_math+"}$, sf: $\\mathsf{"+testTextUmla_math+"}$, tt: $\\mathtt{"+testTextUmla_math+"}$, cal: $\\mathcal{"+testTextUmla_math+"}$, scr: $\\mathscr{"+testTextUmla_math+"}$, bb: $\\mathbb{"+testTextUmla_math+"}$, frak: $\\mathfrak{"+testTextUmla_math+"}$, ");
    ui->cmbTestset->addItem("math: simple relations", "$a{\\leq}b$, $a{\\geq}b$, $a{\\equiv}b$, $a=b$, $a{\\neq}b$, $a<b$, $a>b$");
    const auto wsExample=[](const QStringList& spaces, const QString before, const QString& after)->QString {
        QString s;
        for (int i=0; i<spaces.size(); i++) {
            s+="text: {\\backslash}"+spaces[i]+": "+before+"{\\"+spaces[i]+"}"+after;
            s+="\\ \\ \\ \\ ";
        }
        return s;
    };
    QStringList whitespaces=QStringList()<<" "<<"enspace"<<"quad"<<"qquad"<<","<<":"<<";"<<"!"<<"negmedspace"<<"negthickspace";
    ui->cmbTestset->addItem("text: whitespaces arrows", "text: "+wsExample(whitespaces, "\\rightarrow", "\\leftarrow"));
    ui->cmbTestset->addItem("text: whitespaces fg", "text: "+wsExample(whitespaces, "f", "g"));
    ui->cmbTestset->addItem("math: whitespaces arrows", "math: "+wsExample(whitespaces, "$\\rightarrow", "\\leftarrow$"));
    ui->cmbTestset->addItem("math: whitespaces fg", "math: "+wsExample(whitespaces, "$f", "g$"));
    ui->cmbTestset->addItem("text/math: simple relations in different modes", "math: $a{\\leq}b$, math/no braces: $a\\leq b$, no math: a{\\leq}b, no math/no braces: a\\leq b");
    ui->cmbTestset->addItem("math: named symbols 1", "ll: $\\ll$\\ gg: $\\gg$\\ leq: $\\leq$\\ geq: $\\geq$\\ pm: $\\pm$\\ mp: $\\mp$\\ ");
    ui->cmbTestset->addItem("math: named symbols 2", "nexists: $\\nexists$\\ ni: $\\ni$\\ notni: $\\notni$\\ circ: $\\circ$\\ sim: $\\sim$\\ emptyset: $\\emptyset$\\ odot: $\\odot$\\ ominus: $\\ominus$\\ subsetnot: $\\subsetnot$\\ bot: $\\bot$");
    ui->cmbTestset->addItem("math: named symbols 3", "leftharpoonup: $\\leftharpoonup$\\ rightharpoonup: $\\rightharpoonup$\\ upharpoonleft: $\\upharpoonleft$\\ downharpoonleft: $\\downharpoonleft$\\ leftrightharpoon: $\\leftrightharpoon$\\ rightleftharpoon: $\\rightleftharpoon$");
    ui->cmbTestset->addItem("math: named symbols 4", "coprod: $\\coprod$\\ leftharpoondown: $\\leftharpoondown$\\ rightharpoondown: $\\rightharpoondown$\\ upharpoonright: $\\upharpoonright$\\ downharpoonright: $\\downharpoonright$\\ nwarrow: $\\nwarrow$\\ nearrow: $\\nearrow$\\ ");
    ui->cmbTestset->addItem("math: named symbols 5", "searrow: $\\searrow$\\ swarrow: $\\swarrow$\\ mapsto: $\\mapsto$\\ div: $\\div$\\ multimap: $\\multimap$\\ maporiginal: $\\maporiginal$\\ mapimage: $\\mapimage$\\ ");
    ui->cmbTestset->addItem("math: named symbols 6", "times: $\\times$\\ ast: $\\ast$\\ star: $\\star$\\ propto: $\\propto$\\ bullet: $\\bullet$\\ neq: $\\neq$\\ ne: $\\ne$\\ equiv: $\\equiv$\\ approx: $\\approx$\\ otimes: $\\otimes$\\ oplus: $\\oplus$");
    ui->cmbTestset->addItem("math: named symbols 7", "oslash: $\\oslash$\\ cap: $\\cap$\\ land: $\\land$\\ cup: $\\cup$\\ lor: $\\lor$\\ supset: $\\supset$\\ supseteq: $\\supseteq$\\ supsetnot: $\\supsetnot$\\ subset: $\\subset$");
    ui->cmbTestset->addItem("math: named symbols 8", "subseteq: $\\subseteq$\\ in: $\\in$\\ notin: $\\notin$\\ cdot: $\\cdot$\\ wedge: $\\wedge$\\ vee: $\\vee$\\ cong: $\\cong$\\ bot: $\\bot$");
    ui->cmbTestset->addItem("math: named symbols with special fallback-drawing", "\\infty\\ \\prod\\ \\coprod\\ \\nexists\\ \\sum\\ \\varnothing\\ \\exists\\ \\forall\\ \\neq\\ \\ni\\ \\alpha\\beta\\Omega");
    ui->cmbTestset->addItem("text: named symbols from custom fonts", "\\HandRight\\HandLeft");
    ui->cmbTestset->addItem("math: symbols", "$\\ll\\gg\\leq\\geq\\leftrightarrow\\leftarrow\\rightarrow\\to\\uparrow\\downarrow\\updownarrow\\Leftrightarrow\\iff\\Leftarrow\\Rightarrow\\Uparrow\\Downarrow\\Updownarrow\\pm\\mp\\nexists\\ni\\notni\\circ\\sim\\emptyset\\odot\\ominus\\subsetnot\\bot\\leftharpoonup\\rightharpoonup\\upharpoonleft\\downharpoonleft\\leftrightharpoon\\rightleftharpoon\\coprod\\leftharpoondown\\rightharpoondown\\upharpoonright\\downharpoonright\\nwarrow\\nearrow\\searrow\\swarrow\\mapsto\\div\\multimap\\maporiginal\\mapimage\\times\\propto\\bullet\\neq\\ne\\equiv\\approx\\otimes\\oplus\\oslash\\cap\\land\\cup\\lor\\supset\\supseteq\\supsetnot\\subset\\subseteq\\in\\notin\\cdot\\wedge\\vee\\cong\\bot$");
    ui->cmbTestset->addItem("math: std dev", "$\\sigma_x=\\sqrt{\\langle (x-\\langle x\\rangle)^2\\rangle}=\\sqrt{\\frac{1}{N-1}\\cdot\\left( \\sum_{i=1}^N{x_i}^2-\\frac{1}{N}\\cdot\\left(\\sum_{i=1}^Nx_i\\right)^2\\right)}$");
    ui->cmbTestset->addItem("math: std dev 2", "$\\sigma_x=\\sqrt{\\langle (x-\\langle x\\rangle)^2\\rangle}=\\sqrt{\\frac{1}{N-1}\\cdot\\left( \\sum_{i=1}^Nx_i^2-\\frac{1}{N}\\cdot\\left(\\sum_{i=1}^Nx_i\\right)^2\\right)}$");
    ui->cmbTestset->addItem("math: rotation matrix", "$\\mathrm{\\mathbf{M}}(\\alpha) = \\left(\\begin{matrix}\\cos(\\alpha)+n_x^2\\cdot (1-\\cos(\\alpha))  &  n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))-n_z\\cdot \\sin(\\alpha) &  n_x\\cdot n_z\\cdot (1-\\cos(\\alpha))+n_y\\cdot \\sin(\\alpha)\\\\n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_z\\cdot \\sin(\\alpha) & \\cos(\\alpha)+n_y^2\\cdot (1-\\cos(\\alpha))  &   n_y\\cdot n_z\\cdot (1-\\cos(\\alpha))-n_x\\cdot \\sin(\\alpha)\\\\n_z\\cdot n_x\\cdot (1-\\cos(\\alpha))-n_y\\cdot \\sin(\\alpha) & n_z\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_x\\cdot \\sin(\\alpha)  & \\cos(\\alpha)+n_z^2\\cdot (1-\\cos(\\alpha))\\end{matrix}\\right)$");
    ui->cmbTestset->addItem("math: like in label at bottom", "$\\left(\\left[\\sqrt{2\\pi\\cdot\\int_{-\\infty}^\\infty f(x)\\;\\mathrm{d}x}\\right]\\right)$");
    ui->cmbTestset->addItem("text: like in label at bottom)", "\\left(\\left[\\sqrt{2\\pi\\cdot\\int_{-\\infty}^\\infty f(x)\\;\\mathrm{d}x}\\right]\\right)");
    ui->cmbTestset->addItem("text 0", "text");
    ui->cmbTestset->addItem("text (bold)", "text \\mathbf{bold}");
    ui->cmbTestset->addItem("textcolor", "text \\mathbf{bold}\\textcolor{red}{RED}");
    ui->cmbTestset->addItem("userfont", "text, \\userfont{Arial}{Arial}, \\userfont{Comic Sans MS}{Comic Sans MS}");
    ui->cmbTestset->addItem("text: \\char", "A: \\char65, circonflex: \\char\"109 accent: \\char\'351");
    ui->cmbTestset->addItem("math: \\char", "A: $\\char65$, circonflex: $\\char\"109$ accent: $\\char\'351");
    ui->cmbTestset->addItem("unicode", "star: \\unicode{2605}, circonflex: \\unicode{109} emoticons: \\usym{1F440} \\usym{1F929}");
    ui->cmbTestset->addItem("UTF8", "star: \\utfeight{e29885} emoticons \\utfeight{F09F9881} \\utfeight{f09f98bb}");
    const auto mathDecoExample=[](const QString& deco)->QString { return "\\"+deco+"{x}\\"+deco+"{i}\\"+deco+"{X}\\"+deco+"{\\psi}\\"+deco+"{abc}"; };
    ui->cmbTestset->addItem("decoration: math", "$"+mathDecoExample("vec")+" -- "+mathDecoExample("grave")+" -- "+mathDecoExample("acute")+" -- "+mathDecoExample("dot")+" -- "+mathDecoExample("ddot")+" -- "+mathDecoExample("ocirc")+" -- "+mathDecoExample("overline")+" -- "+mathDecoExample("underline")+" -- "+mathDecoExample("hat")+" -- "+mathDecoExample("widehat")+" -- "+mathDecoExample("check")+" -- "+mathDecoExample("widecheck")+" -- "+mathDecoExample("breve")+" -- "+mathDecoExample("tilde")+" -- "+mathDecoExample("widetilde")+" -- "+mathDecoExample("uul")+" -- "+mathDecoExample("ool")+" -- "+mathDecoExample("bar")+" -- "+mathDecoExample("arrow")+" -- "+mathDecoExample("cancel")+" -- "+mathDecoExample("bcancel")+" -- "+mathDecoExample("xcancel")+" -- "+mathDecoExample("sout")+"$");
    ui->cmbTestset->addItem("decoration: text", "Text \\ul{underlined Text Equator} -- \\ol{overlined Text Equator} -- \\sout{striked out Text Equator} -- \\cancel{canceled out Text Equator} -- \\bcancel{b-canceled out Text Equator} -- \\xcancel{x-canceled out Text Equator}");
    ui->cmbTestset->addItem("text: \\verb", "\\verb!\\LaTeX{} is not pares inside \\verb~..~! outside {\\backslash}verb");
    ui->cmbTestset->addItem("text: \\begin{verbatim}", "outside\\begin{verbatim}\ninside \\LaTeX verbatim\n  2nd verbaimline\n\t3rd line\n\\end{verbatim}");
    ui->cmbTestset->addItem("text: \\begin{verbatim*}", "outside\\begin{verbatim*}\ninside \\LaTeX verbatim\n  2nd verbaimline\n\t3rd line\n\\end{verbatim*}");
    ui->cmbTestset->addItem("text: \\begin{lstlistings}", "outside\\begin{lstlisting}\nint main() {\n  printf(\"Hello World\\n\");\n}\n\\end{lstlisting}");
    ui->cmbTestset->addItem("text: flushleft", "\\begin{flushleft}text\\\\\\textbf{2^{nd} line of text}\\\\last \\textit{line!} $\\frac{1}{2}$\\end{flushleft}");
    ui->cmbTestset->addItem("text: flushright", "\\begin{flushright}text\\\\\\textbf{2^{nd} line of text}\\\\last \\textit{line!} $\\frac{1}{2}$\\end{flushright}");
    ui->cmbTestset->addItem("text: center", "\\begin{center}text\\\\\\textbf{2^{nd} line of text}\\\\last \\textit{line!} $\\frac{1}{2}$\\end{center}");
    ui->cmbTestset->addItem("text: framed", "\\begin{framed}text\\\\\\textbf{2^{nd} line of text}\\\\last \\textit{line!} $\\frac{1}{2}$\\end{framed}");
    ui->cmbTestset->addItem("text: shaded", "\\begin{shaded}text\\\\\\textbf{2^{nd} line of text}\\\\last \\textit{line!} $\\frac{1}{2}$\\end{shaded}");
    ui->cmbTestset->addItem("text: snugshade", "\\begin{snugshade}text\\\\\\textbf{2^{nd} line of text}\\\\last \\textit{line!} $\\frac{1}{2}$\\end{snugshade}");
    ui->cmbTestset->addItem("mathtest", "This is normal text: $this is math:\\langle r^2(\\tau)\\rangle=\\left\\langle (\\vec{r}(t)-\\vec{r}(t+\\tau) )^2\\right\\rangle\\ \\ \\ g(\\tau)=\\frac{1}{N}\\cdot\\left(1+\\frac{2}{3}\\frac{\\langle r^2(\\tau)\\rangle}{w_{xy}^2}\\right)^{-1}  \\lfloor\\rfloor\\lceil\\rceil\\langle\\rangle\\left\\{\\vec{a}\\left|\\|\\vec{a}\\|_2\\geq2\\right.\\right\\} \\vec{r}\\vec{R}\\frac{\\sqrt{\\sqrt{\\sqrt{\\sum_{i=0}^\\infty \\hat{i}^2}+y^\\alpha}+1}}{\\dot{v}\\equiv\\ddot{r}}\\argmin_{\\vec{k}}\\sum_{\\sqrt{i}=0}^{N}\\int_{x_0}^{x_1}\\left(\\left(\\left(x\\right)\\right)\\right)\\underbrace{\\left[\\left\\{\\frac{\\partial f}{\\partial x}\\right\\}\\cdot\\frac{1}{2}\\right]}{\\text{underbraced text \\hbar}}\\cdots\\frac{\\sqrt{\\sum_{i=0}^2 \\hat{i}^2}+y^\\alpha}{\\dot{v}\\equiv\\ddot{r}}, \\hat{t}\\hat{T} \\overbrace{\\left|\\sqrt{x\\cdot Y}\\right|}{\\propto\\bbN\\circ\\bbZ}   \\left<\\arrow{x(\\tau)}\\cdot\\vec{R}(t+\\bar{\\tau})\\right>   \\alpha\\beta\\gamma\\delta\\epsilon\\Gamma\\Delta\\Theta\\Omega  \\left\\_\\left~\\cbrt{\\hbar\\omega}\\right~\\right\\_$");
    ui->cmbTestset->addItem("math: upper/lower parantheses test:", "$\\text{bblabla} \\frac{1}{2}\\cdot\\left(\\frac{1}{\\mathrm{e}^x+\\mathrm{e}^{-x}}\\right)\\cdot\\left(\\frac{1}{\\frac{1+2}{5+x}}\\right)\\cdot\\left(\\frac{1}{\\exp\\left[-\\frac{y^2}{\\sqrt{x}}\\right]\\cdot\\exp\\left[-\\frac{1}{\\frac{1}{2}}\\right]}\\right) $");
    ui->cmbTestset->addItem("math: ACF test", "$g_{rg}^{ab}(\\tau)=\\frac{1}{N}\\cdot\\left(1+\\frac{2}{3}\\frac{\\langle r^2(\\tau)\\rangle}{w_{xy}^2}\\right)^{-1}\\cdot\\left(1+\\frac{2}{3}\\frac{\\langle r^2(\\tau)\\rangle}{w_{xy}^2}\\right)^{-\\frac{1}{2}}$");
    ui->cmbTestset->addItem("math: MSD test", "$\\mathrm{MSD}(\\tau)\\equiv\\langle r^2(\\tau)\\rangle=\\left\\langle (\\vec{r}(t)-\\vec{r}(t+\\tau) )^2\\right\\rangle=2n\\cdot\\frac{K_\\alpha}{\\Gamma(1+\\alpha)}\\cdot\\tau^\\alpha$");
    ui->cmbTestset->addItem("math: f() test", "$f(\\vec{x})=\\frac{1}{\\sqrt{2\\pi\\cdot\\sigma^2}}\\exp\\left(-\\frac{(\\vec{x}-\\vec{x}_0)^2}{\\sigma^2}\\right)$");
    ui->cmbTestset->addItem("math: chi^2 test", "$\\vec{p}^\\ast=\\argmax\\limits_{\\vec{p}}\\chi^2=\\argmax\\limits_{\\vec{p}}\\sum\\limits_{i=1}^N\\left|\\frac{\\hat{f}_i-f(x_i;\\vec{p})}{\\sigma_i}\\right|^2$");
    ui->cmbTestset->addItem("text: symbol test", "\\vdots\\cdots\\ddots\\iddots\\lfloor\\rfloor\\lceil\\rceil\\langle\\rangle\\sum\\int \\iint \\oint \\prod \\leftrightarrow \\leftarrow\\Leftarrow\\rightarrow\\Rightarrow\\pm\\mp\\leq\\geq\\ll\\gg\\hbar\\euro\\bbC\\bbH\\bbN\\bbP\\bbQ\\bbZ\\bbR\\Angstrom\\Alef\\Bet\\Gimel\\Dalet\\nexists\\ni\\notni\\circ\\tilde\\oiint\\oiiint\\emptyset\\odot\\ominus\\subsetnot\\DC\\bot\\cdots\\perthousand\\leftharpoonup\\rightharpoonup \\upharpoonleft \\downharpoonleft \\leftrightharpoon \\rightleftharpoon \\coprod \\leftharpoondown \\rightharpoondown \\nwarrow \\nearrow \\mapsto \\cent \\pound \\yen \\div \\multimap \\maporiginal \\mapimage \\bigcap \\bigcup \\benzene \\times \\cdot \\propto \\equiv \\Im \\Re \\ ");
    ui->cmbTestset->addItem("math: arrowtest 1", "$\\leftarrow \\longleftarrow \\Leftarrow \\Longleftarrow \\rightarrow \\longrightarrow \\Rightarrow \\Longrightarrow \\uparrow \\Uparrow \\downarrow \\Downarrow \\leftrightarrow \\Leftrightarrow \\longleftrightarrow \\Longleftrightarrow$");
    ui->cmbTestset->addItem("math: arrowtest 2", "$\\nwarrow \\nearrow \\searrow \\swarrow \\mapsto \\leftharpoonup \\rightharpoonup \\upharpoonleft \\downharpoonleft \\leftrightharpoon \\rightleftharpoon \\leftharpoondown \\rightharpoondown \\upharpoonright \\downharpoonright $");
    ui->cmbTestset->addItem("math-font: greek letters", "$\\alpha\\beta\\gamma\\delta\\epsilon\\varepsilon\\zeta\\eta\\theta\\vartheta\\iota\\kappa\\lambda\\mu\\nu\\xi\\pi\\varpi\\rho\\varrho\\sigma\\varsigma\\tau\\upsilon\\phi\\varphi\\chi\\psi\\omega\\Omega\\Gamma\\Delta\\Theta\\Lambda\\Xi\\Pi\\Sigma\\Upsilon\\Phi\\Psi$");
    ui->cmbTestset->addItem("text-font: greek letters", "\\alpha\\beta\\gamma\\delta\\epsilon\\varepsilon\\zeta\\eta\\theta\\vartheta\\iota\\kappa\\lambda\\mu\\nu\\xi\\pi\\varpi\\rho\\varrho\\sigma\\varsigma\\tau\\upsilon\\phi\\varphi\\chi\\psi\\omega\\Omega\\Gamma\\Delta\\Theta\\Lambda\\Xi\\Pi\\Sigma\\Upsilon\\Phi\\Psi");
    ui->cmbTestset->addItem("math-font: upgreek letters", "$\\upalpha\\upbeta\\upgamma\\updelta\\upepsilon\\upvarepsilon\\upzeta\\upeta\\uptheta\\upvartheta\\upiota\\upkappa\\uplambda\\upmu\\upnu\\upxi\\uppi\\upvarpi\\uprho\\upvarrho\\upsigma\\upvarsigma\\uptau\\upupsilon\\upphi\\upvarphi\\upchi\\uppsi\\upomega\\upOmega\\upGamma\\upDelta\\upTheta\\upLambda\\upXi\\upPi\\upSigma\\upUpsilon\\upPhi\\upPsi$");
    ui->cmbTestset->addItem("text-font: upgreek letters", "\\upalpha\\upbeta\\upgamma\\updelta\\upepsilon\\upvarepsilon\\upzeta\\upeta\\uptheta\\upvartheta\\upiota\\upkappa\\uplambda\\upmu\\upnu\\upxi\\uppi\\upvarpi\\uprho\\upvarrho\\upsigma\\upvarsigma\\uptau\\upupsilon\\upphi\\upvarphi\\upchi\\uppsi\\upomega\\upOmega\\upGamma\\upDelta\\upTheta\\upLambda\\upXi\\upPi\\upSigma\\upUpsilon\\upPhi\\upPsi");
    ui->cmbTestset->addItem("math-font: blackboard", "$\\mathbb{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: bf", "$\\mathbf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: it", "$\\mathit{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: frak", "$\\mathfrak{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: sf", "$\\mathsf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: rm", "$\\mathrm{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: cal", "$\\mathcal{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: script", "$\\mathscr{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math-font: tt", "$\\mathtt{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("text: bf", "\\textbf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("text: it", "\\textit{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("text: frak", "\\textfrak{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("text: sf", "\\textsf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("text: rm", "\\textrm{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("text: cal", "\\textcal{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("text: script", "\\textscr{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("text: tt", "\\texttt{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("math: subscript test", "$r_{123}\\ \\ r_{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("math: subscript0 test", "$r_{123}$");
    ui->cmbTestset->addItem("math: subscript1 test", "$r_{123}\\ $");
    ui->cmbTestset->addItem("math: subscript2 test", "$r_{123}\\ \\ $");
    ui->cmbTestset->addItem("math: subscript3 test", "$r_{123}r_{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("math: superscript test", "$r^{123}\\ \\ r^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("math: superscript0 test", "$r^{123}$");
    ui->cmbTestset->addItem("math: superscript1 test", "$r^{123}\\ $");
    ui->cmbTestset->addItem("math: superscript2 test", "$r^{123}\\ \\ $");
    ui->cmbTestset->addItem("math: superscript3 test", "$r^{123}r^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("math: asuperscript test", "$a^{123}\\ \\ a^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("math: asuperscript0 test", "$a^{123}$");
    ui->cmbTestset->addItem("math: gsuperscript1 test", "$g^{123}\\ $");
    ui->cmbTestset->addItem("math: gsuperscript2 test", "$g^{123}\\ \\ $");
    ui->cmbTestset->addItem("math: gsuperscript3 test", "$g^{123}g^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("math: subsuperscript test", "$r_{123}^2$\\ \\ $r^2_{123}$\\ \\ $\\left(\\stackrel{a}{b}\\right)_{123}^2$\\ \\ $\\left(\\stackrel{a}{b}\\right)^2_{123}$\\ \\ $r_{\\frac{1}{2}}^2$\\ \\ $r^2_{\\frac{1}{2}}$\\ \\ $r^{\\frac{1}{2}}_2$\\ \\ $r_2^{\\frac{1}{2}}$\\ \\ $\\left(\\stackrel{a}{b}\\right)_2^{\\frac{1}{2}}$\\ \\ $\\left(\\stackrel{a}{b}\\right)^{\\frac{1}{2}}_2$");
    ui->cmbTestset->addItem("math: radicals", "$\\sqrt{a}\\sqrt{5}\\sqrt{-1}\\sqrt{h}\\sqrt{jA}\\sqrt{\\vec{A}}\\sqrt{\\frac{1}{a}}\\frac{\\sqrt{a}}{\\sqrt{a}}\\sqrt{\\frac{1}{1+\\frac{1}{a}}}\\frac{1}{\\sqrt{1+\\frac{1}{a}}}\\sqrt{a+\\sqrt{a+b}}$");
    ui->cmbTestset->addItem("text: radicals", "\\sqrt{a}\\sqrt{5}\\sqrt{-1}\\sqrt{h}\\sqrt{jA}\\sqrt{\\vec{A}}\\sqrt{\\frac{1}{a}}\\frac{\\sqrt{a}}{\\sqrt{a}}\\sqrt{\\frac{1}{1+\\frac{1}{a}}}\\frac{1}{\\sqrt{1+\\frac{1}{a}}}\\sqrt{a+\\sqrt{a+b}}");
    ui->cmbTestset->addItem("math: non-2 radicals", "$\\sqrt[3]{a}\\sqrt[3]{5}\\sqrt[3]{-1}\\sqrt[3]{h}\\sqrt[3]{\\vec{A}}\\sqrt[3]{\\frac{1}{a}}\\frac{\\sqrt[3]{a}}{\\sqrt[3]{a}}\\sqrt[3]{\\frac{1}{1+\\frac{1}{a}}}\\frac{1}{\\sqrt[3]{1+\\frac{1}{a}}}\\sqrt[3]{a+\\sqrt[3]{a+b}}$");
    ui->cmbTestset->addItem("text: non-2 radicals", "\\sqrt[3]{a}\\sqrt[3]{5}\\sqrt[3]{-1}\\sqrt[3]{h}\\sqrt[3]{\\vec{A}}\\sqrt[3]{\\frac{1}{a}}\\frac{\\sqrt[3]{a}}{\\sqrt[3]{a}}\\sqrt[3]{\\frac{1}{1+\\frac{1}{a}}}\\frac{1}{\\sqrt[3]{1+\\frac{1}{a}}}\\sqrt[3]{a+\\sqrt[3]{a+b}}");
    ui->cmbTestset->addItem("math: long non-2 radicals", "$\\sqrt[3.14156]{a}\\sqrt[3.14156]{5}$");
    ui->cmbTestset->addItem("text: long non-2 radicals", "\\sqrt[3.14156]{a}\\sqrt[3.14156]{5}");
    ui->cmbTestset->addItem("math: sum, prod, ... bare/+\\nolimits", "no-limits: $\\prod\\nolimits_{i=1}^n \\sum\\nolimits_{j=1}^c (i + j)\\cdot\\frac{1}{2}$\\ \\ \\ --\\ \\ \\ bare: $\\prod_{i=1}^n \\sum_{j=1}^c (i + j)\\cdot\\frac{1}{2}$\\ \\ \\ --\\ \\ \\ long-below, bare: $\\sum_{n=\\{a,b,c,d,e,f,g\\}} f(x)$\\ \\ \\ --\\ \\ \\ long-above, bare: $\\sum^{n=\\{a,b,c,d,e,f,g\\}} f(x)$");
    ui->cmbTestset->addItem("text: sum, prod, ... bare/+\\limits", "bare: \\prod_{i=1}^n \\sum_{j=1}^c (i + j)\\cdot\\frac{1}{2}\\ \\ \\ --\\ \\ \\ limits: \\prod\\limits_{i=1}^n \\sum\\limits_{j=1}^c (i + j)\\cdot\\frac{1}{2}\\ \\ \\ --\\ \\ \\ long-below, bare: \\sum_{n=\\{a,b,c,d,e,f,g\\}} f(x)\\ \\ \\ --\\ \\ \\ long-above, bare: \\sum^{n=\\{a,b,c,d,e,f,g\\}} f(x)");
    ui->cmbTestset->addItem("math: more sum-symbols ", "$\\sum\\limits_{i=0}^N\\prod\\limits_{i=0}^N\\coprod\\limits_{i=0}^N\\bigcup\\limits_{i=0}^N\\bigcap\\limits_{i=0}^N\\bigsqcup\\limits_{i=0}^N\\bigvee\\limits_{i=0}^N\\bigwedge\\limits_{i=0}^N\\bigoplus\\limits_{i=0}^N\\bigotimes\\limits_{i=0}^N\\bigodot\\limits_{i=0}^N\\biguplus\\limits_{i=0}^N$");
    ui->cmbTestset->addItem("math: integrals+sums, bare/+\\nolimits", "bare: $\\int_{0}^1 f(x)\\;\\mathrm{d}x\\ \\sum_{x=0}^1f(x)\\ \\iint_{0}^1 f(x)\\;\\mathrm{d}x\\ \\iiint_{0}^1 f(x)\\;\\mathrm{d}x\\ \\oint_{0}^1 f(x)\\;\\mathrm{d}x\\ \\int_{x} f(x)\\;\\mathrm{d}x$\\ \\ \\ --\\ \\ \\ nolimits: $\\int\\nolimits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\sum\\nolimits_{x=0}^1f(x)\\ \\iint\\nolimits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\iiint\\nolimits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\oint\\nolimits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\int\\nolimits_{x} f(x)\\;\\mathrm{d}x$");
    ui->cmbTestset->addItem("text: integrals+sums, bare/+\\limits", "bare: \\int_{0}^1 f(x)\\;\\mathrm{d}x\\ \\sum_{x=0}^1f(x)\\ \\iint_{0}^1 f(x)\\;\\mathrm{d}x\\ \\iiint_{0}^1 f(x)\\;\\mathrm{d}x\\ \\oint_{0}^1 f(x)\\;\\mathrm{d}x\\ \\int_{x} f(x)\\;\\mathrm{d}x$\\ \\ \\ --\\ \\ \\ limits: $\\int\\limits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\sum\\limits_{x=0}^1f(x)\\ \\iint\\limits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\iiint\\limits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\oint\\limits_{0}^1 f(x)\\;\\mathrm{d}x\\ \\int\\limits_{x} f(x)\\;\\mathrm{d}x");
    ui->cmbTestset->addItem("math: integral+sub, bare", "$\\iint_{\\partial S} f(s)\\;\\mathrm{d}s$");
    ui->cmbTestset->addItem("math: integral+sup, bare", "$\\iint^{\\partial S} f(s)\\;\\mathrm{d}s$");
    ui->cmbTestset->addItem("math: integral+sub, no-limits", "$\\iint\\nolimits_{\\partial S} f(s)\\;\\mathrm{d}s$");
    ui->cmbTestset->addItem("math: integral+sup, no-limits", "$\\iint\\nolimits^{\\partial S} f(s)\\;\\mathrm{d}s$");
    ui->cmbTestset->addItem("math: integral+sub+sup, bare", "$\\iint_0^1f(s)\\;\\mathrm{d}s$");
    ui->cmbTestset->addItem("math: integral+sub+sup, no-limits", "$\\iint\\nolimits_0^1f(s)\\;\\mathrm{d}s$");
    ui->cmbTestset->addItem("math: frac test", "$\\frac{a}{b}+\\frac{g}{a}-\\frac{a^2}{b^2}\\cdot\\frac{a^2}{b^{\\frac{1}{2}}}$");
    ui->cmbTestset->addItem("math/text: stacked frac", "math: $\\frac{1+\\frac{a}{b}}{1+\\frac{1}{1+\\frac{1}{a}}}$\\ \\ text: \\frac{1+\\frac{a}{b}}{1+\\frac{1}{1+\\frac{1}{a}}}\\ \\ \\ \\ \\ --\\ \\ \\ \\ \\ math: $a_0+\\cfrac{1}{a_1+\\cfrac{1}{a_2+\\cfrac{1}{a_3+\\cdots}}}$\\ \\ text: a_0+\\cfrac{1}{a_1+\\cfrac{1}{a_2+\\cfrac{1}{a_3+\\cdots}}}");
    ui->cmbTestset->addItem("math: tfrac test", "$\\tfrac{a}{b}+\\tfrac{g}{a}-\\tfrac{a^2}{b^2}\\cdot\\tfrac{a^2}{b^{\\tfrac{1}{2}}}$");
    ui->cmbTestset->addItem("math: dfrac test", "$\\dfrac{a}{b}+\\dfrac{g}{a}-\\dfrac{a^2}{b^2}\\cdot\\dfrac{a^2}{b^{\\dfrac{1}{2}}}$");
    ui->cmbTestset->addItem("math: sfrac test", "$\\sfrac{a}{b}+\\sfrac{g}{a}-\\sfrac{a^2}{b^2}\\cdot\\sfrac{a^2}{b^{\\sfrac{1}{2}}}$");
    ui->cmbTestset->addItem("math: stfrac test", "$\\stfrac{a}{b}+\\stfrac{g}{a}-\\stfrac{a^2}{b^2}\\cdot\\stfrac{a^2}{b^{\\stfrac{1}{2}}}$");
    ui->cmbTestset->addItem("text: number fractions test", "frac: \\frac{1}{2}, \\frac{3}{4},\\ \\ \\ tfrac: \\tfrac{1}{2}, \\tfrac{3}{4},\\ \\ \\ sfrac: \\sfrac{1}{2}, \\sfrac{3}{4},\\ \\ \\ stfrac: \\stfrac{1}{2}, \\stfrac{3}{4},\\ \\ \\ ");
    ui->cmbTestset->addItem("math: stackrel test", "$\\stackrel{a}{b}+\\stackrel{g}{a}-\\stackrel{a^2}{b^2}\\cdot\\stackrel{a^2}{b^{\\stackrel{1}{2}}}$");
    ui->cmbTestset->addItem("text/math: brace test langle", "text: \\langle\\langle -- math: $\\langle\\langle$");
    ui->cmbTestset->addItem("text/math: brace test lange+super 1", "text: \\langle\\langle r^{123} -- math: $\\langle\\langle r^{123}$");
    ui->cmbTestset->addItem("text/math: brace test lange+super 2", "text: \\langle\\langle r^{123}\\rangle\\rangle -- math: $\\langle\\langle r^{123}\\rangle\\rangle$");
    ui->cmbTestset->addItem("text/math: brace test left-langle 1", "text: \\left\\langle r^{123}\\right\\rangle -- math: $\\left\\langle r^{123}\\right\\rangle$");
    ui->cmbTestset->addItem("text/math: brace test left-langle 2", "text: \\left\\langle\\left\\langle\\left\\langle r^{123}\\right\\rangle\\right\\rangle\\right\\rangle -- math: $\\left\\langle\\left\\langle\\left\\langle r^{123}\\right\\rangle\\right\\rangle\\right\\rangle$");
    auto fAddBraceTest=[](const QString& open, const QString& close) {
        QString res= "\\left"+open+"\\left"+open+"\\left"+open+" r^{123}\\right"+close+"\\right"+close+"\\right"+close;
        res += "\\left"+open+"\\left"+open+"\\left"+open+" r_{123}\\right"+close+"\\right"+close+"\\right"+close;
        res += "\\left"+open+"\\left"+open+"\\left"+open+" r_{123}^{123}\\right"+close+"\\right"+close+"\\right"+close;
        return res;
    };
    ui->cmbTestset->addItem("text/math: brace test:  left/right ( )", "text: "+fAddBraceTest("(",")")+" -- math: $"+fAddBraceTest("(",")")+"$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right [ ]", "text: "+fAddBraceTest("[","]")+" -- math: $"+fAddBraceTest("[","]")+"$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right { }", "text: "+fAddBraceTest("\\{","\\}")+" -- math: $"+fAddBraceTest("\\{","\\}")+"$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right || ||", "text: "+fAddBraceTest("\\|","\\|")+" -- math: $"+fAddBraceTest("\\|","\\|")+"$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right | |", "text: "+fAddBraceTest("|","|")+" -- math: $"+fAddBraceTest("|","|")+"$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right angles", "text: \\left\\langle\\left\\langle\\left\\langle r^{123}\\right\\rangle\\right\\rangle\\right\\rangle -- math: $\\left\\langle\\left\\langle\\left\\langle r^{123}\\right\\rangle\\right\\rangle\\right\\rangle$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right floor", "text: \\left\\lfloor\\left\\lfloor\\left\\lfloor r^{123}\\right\\rfloor\\right\\rfloor\\right\\rfloor -- math: $\\left\\lfloor\\left\\lfloor\\left\\lfloor r^{123}\\right\\rfloor\\right\\rfloor\\right\\rfloor$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right ceil", "text: \\left\\lceil\\left\\lceil\\left\\lceil r^{123}\\right\\rceil\\right\\rceil\\right\\rceil -- math: $\\left\\lceil\\left\\lceil\\left\\lceil r^{123}\\right\\rceil\\right\\rceil\\right\\rceil$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right top-corner", "text: "+fAddBraceTest("\\ulcorner","\\urcorner")+" -- math: $"+fAddBraceTest("\\ulcorner","\\urcorner")+"$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right bottom-corner", "text: "+fAddBraceTest("\\llcorner","\\lrcorner")+" -- math: $"+fAddBraceTest("\\llcorner","\\lrcorner")+"$");
    ui->cmbTestset->addItem("text/math: brace test:  left/right mixed", "text: \\left\\{\\left[\\left( r^{123}\\right)\\right]\\right\\} -- math: $\\left\\{\\left[\\left( r^{123}\\right)\\right]\\right\\}$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right ( )", "text: (((r^{123}))) -- math: $(((r^{123})))$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right [ ]", "text: [[[r^{123}]]] -- math: $[[[r^{123}]]]$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right { }", "text: \\{\\{\\{r^{123}\\}\\}\\} -- math: $\\{\\{\\{ r^{123}\\}\\}\\}$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right | |", "text: |||r^{123}||| -- math: $|||r^{123}|||$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right angle", "text: \\langle\\langle\\langle r^{123}\\rangle\\rangle\\rangle -- math: $\\langle\\langle\\langle r^{123}\\rangle\\rangle\\rangle$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right floor", "text: \\lfloor\\lfloor\\lfloor r^{123}\\rfloor\\rfloor\\rfloor -- math: $\\lfloor\\lfloor\\lfloor r^{123}\\rfloor\\rfloor\\rfloor$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right ceil", "text: \\lceil\\lceil\\lceil r^{123}\\rceil\\rceil\\rceil -- math: $\\lceil\\lceil\\lceil r^{123}\\rceil\\rceil\\rceil$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right top-corner", "text: \\ulcorner\\ulcorner\\ulcorner r^{123}\\urcorner\\urcorner\\urcorner -- math: $\\ulcorner\\ulcorner\\ulcorner r^{123}\\urcorner\\urcorner\\urcorner$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right bottom-corner", "text: \\llcorner\\llcorner\\llcorner r^{123}\\lrcorner\\lrcorner\\lrcorner -- math: $\\llcorner\\llcorner\\llcorner r^{123}\\lrcorner\\lrcorner\\lrcorner$");
    ui->cmbTestset->addItem("text/math: brace test:  non-left/right { | }", "text: \\{r^{123}|r\\equiv 5\\} -- math: $\\{r^{123}|r\\equiv 5\\}$");
    ui->cmbTestset->addItem("math: brace test:  bigl/Bigl/... mixed", "math: $\\Biggl\\langle\\biggl\\{\\Bigl[\\bigl(( r^{123})\\bigr)\\Bigr]\\biggr\\}\\Biggr\\rangle$");
    ui->cmbTestset->addItem("text/math: sub-, superscript test", "text: r^{1234}_{321} r_{321}^{1234} -- math: $r^{1234}_{321} r_{321}^{1234} -- \\kappa^2 -- \\kappa_2 -- \\kappa_2^2$");
    ui->cmbTestset->addItem("text/math: super-, subscript test", "text: r^{123}_{4321} r_{4321}^{123} -- math: $r^{123}_{4321} r_{4321}^{123} -- \\kappa^2 -- \\kappa_2 -- \\kappa_2^2$");
    //ui->cmbTestset->addItem("", "");
    ui->cmbTestset->addItem("math: nested sqrt", "$\\sqrt{1+\\sqrt{1+\\sqrt{1+\\sqrt{1+\\sqrt{1+\\sqrt{1+x}}}}}}$");
    ui->cmbTestset->addItem("math: nested cbrt", "$\\sqrt[3]{1+\\sqrt[3]{1+\\sqrt[3]{1+\\sqrt[3]{1+\\sqrt[3]{1+\\sqrt[3]{1+x}}}}}}$");
    ui->cmbTestset->addItem("math: nested sqrt, high contents", "$\\sqrt{\\frac{1}{2}+\\sqrt{\\frac{1}{2}+\\sqrt{\\frac{1}{2}+\\sqrt{\\frac{1}{2}+\\sqrt{\\frac{1}{2}+\\sqrt{\\frac{1}{2}+x}}}}}}$");
    ui->cmbTestset->addItem("math: frac+stackrel", "$\\left(\\stackrel{p}{2}\\right)=x^2y^{p-2}-\\frac{1}{1-x}\\frac{1}{1-x^2}$");
    ui->cmbTestset->addItem("math: nested frac", "$a_0+\\frac{1}{a_1+\\frac{1}{a_2+\\frac{1}{a_3+\\frac{1}{a_4}}}}$");
    ui->cmbTestset->addItem("math: partial derivatives", "$\\left(\\frac{\\partial^2}{\\partial x^2}+\\frac{\\partial^2}{\\partial y^2}\\right)\\left|\\varphi(x+\\mathrm{i}y)\\right|^2=0$");
    ui->cmbTestset->addItem("math: powers of powers", "$2^{2^{2^{x}}}$");
    ui->cmbTestset->addItem("math: double-integral", "$\\iint_Df(x,y)\\;\\mathrm{d}x\\;\\mathrm{d}y$");
    ui->cmbTestset->addItem("math: overbrace+overbracket", "$\\overbrace{x+x+...+x}{k\\ \\mathrm{times}}\\ \\ \\ \\overbracket{x+x+...+x}{k\\ \\mathrm{times}}$");
    ui->cmbTestset->addItem("math: underbrace+underbracket", "$\\underbrace{x+x+...+x}{k\\ \\mathrm{times}}\\ \\ \\ \\underbracket{x+x+...+x}{k\\ \\mathrm{times}}$");
    ui->cmbTestset->addItem("math: under/overbrace+under/overbracket", "$\\underbrace{\\overbrace{x+x+...+x}{k\\ \\mathrm{times}} \\overbrace{x+x+...+x}{k\\ \\mathrm{times}}}{2k\\ \\mathrm{times}}\\ \\ \\ \\underbracket{\\overbracket{x+x+...+x}{k\\ \\mathrm{times}} \\overbracket{x+x+...+x}{k\\ \\mathrm{times}}}{2k\\ \\mathrm{times}}$");
    ui->cmbTestset->addItem("math: apostrophy/prime derivatives", "$y_1'\\ y_2''\\ y_3'''\\ \\ \\ --\\ \\ \\ y_1^\\prime\\ y_2^\\dprime\\ y_3^\\trprime\\ \\ \\ --\\ \\ \\ y_1^\\prime\\ y_2^{\\prime\\prime}\\ y_3^{\\prime\\prime\\prime}$");
    ui->cmbTestset->addItem("math: cases", "$f(x)=\\begin{cases} 1/3 & \\mathrm{if}\\ 0\\leq x\\leq1 \\\\ 2/3 & \\mathrm{if}\\ 3\\leq x\\leq4 \\\\0 & \\mathrm{elsewhere} \\end{cases}$");
    ui->cmbTestset->addItem("math: overset", "$X \\overset{=}{def} Y$\\ \\ \\ \\ \\ $X \\overset{=}{!} Y$\\ \\ \\ \\ \\ $X \\overset{\\rightarrow}{f} Y$\\ \\ \\ \\ \\ $\\frac{f(x+\\Delta x)-f(x)}{\\Delta x}\\overset{\\longrightarrow}{\\Delta x\\to 0}f'(x)$");
    ui->cmbTestset->addItem("math: underset", "$X \\underset{=}{\\text{def (5)}} Y$\\ \\ \\ \\ \\ $X \\underset{\\rightarrow}{f} Y$\\ \\ \\ \\ \\ $\\frac{f(x+\\Delta x)-f(x)}{\\Delta x}\\underset{\\longrightarrow}{\\Delta x\\to 0}f'(x)$");
    ui->cmbTestset->addItem("math: substack", "substack: $\\substack{i=0\\\\\\sin(x)\\equiv\\pi\\\\a=a\\\\a=b}$\\ \\ \\ substack[l]: $\\substack[l]{i=0\\\\\\sin(x)\\equiv\\pi\\\\a=a\\\\a=b}$\\ \\ \\ lsubstack: $\\lsubstack{i=0\\\\\\sin(x)\\equiv\\pi\\\\a=a\\\\a=b}$\\ \\ \\ rsubstack: $\\rsubstack{i=0\\\\\\sin(x)\\equiv\\pi\\\\a=a\\\\a=b}$\\ \\ \\ comma: $\\sum\\limits_{0\\leq i\\leq 10, -5\\leq j\\leq100}f_{i,j}$\\ \\ \\ substack: $\\sum\\limits_\\substack{0\\leq i\\leq 10\\\\ -5\\leq j\\leq100}f_{i,j}$\\ \\ \\ rsubstack: $\\sum\\limits_\\rsubstack{0\\leq i\\leq 10\\\\ -5\\leq j\\leq100}f_{i,j}$");
    ui->cmbTestset->addItem("math: Jacobi Matrix and VarCov matrix", "$J_{ij}= \\left.\\frac{\\partial f(x,\\vec{p})}{\\partial p_i}\\right|_{\\vec{p},x=x_j}\\ \\ \\ \\ \\ \\mat{C}=\\left(\\mat{J}^\\mathrm{T}\\ J\\right)^{-1}\\ \\ \\ \\ \\ test: \\left|\\frac{\\partial f(x,\\vec{p})}{\\partial p_i}\\right|_{\\vec{p},x=x_j}^2$");
    ui->cmbTestset->addItem("math: operator test (textmode)", "x=0\\ \\ y>0\\ \\ x+y\\ \\ -1\\ \\ x-2\\ \\ x\\cdot y\\ \\ x\\geq 4\\ \\ x~4");
    ui->cmbTestset->addItem("math: operator test (mathmode)", "$x=0\\ \\ y>0\\ \\ x+y\\ \\ -1\\ \\ x-2\\ \\ x\\cdot y\\ \\ x\\geq 4\\ \\ x~4$");
    ui->cmbTestset->addItem("text: color test", "\\textcolor{red}{RED}\\textcolor{blue}{BLUE}");
    ui->cmbTestset->addItem("text: boxed test", "test: {\\backslash}boxed: \\boxed{boxed text} {\\backslash}doublebox: \\doublebox{boxed text} {\\backslash}ovalbox: \\ovalbox{boxed text} {\\backslash}Ovalbox: \\Ovalbox{boxed text} {\\backslash}ovaldoublebox: \\ovaldoublebox{boxed text}  {\\backslash}dashbox\\{...\\}: \\dashbox{boxed text}  {\\backslash}dottedbox\\{...\\}: \\dottedbox{boxed text} {\\backslash}colorbox\\{red\\}: \\colorbox{red}{boxed text} {\\backslash}shaded\\{AliceBlue\\}: \\shaded{AliceBlue}{boxed text} {\\backslash}fcolorbox\\{red\\}\\{AliceBlue\\}: \\fcolorbox{red}{AliceBlue}{boxed text} in the middle");
    ui->cmbTestset->addItem("mathboxed test", "$\\fbox{2^{2^{\\colorbox{red}{2^{x}}}}}$");
    ui->cmbTestset->addItem("axiom of power test", "$\\forall A \\, \\exists P \\, \\forall B \\, [B \\in P \\iff \\forall C \\, (C \\in B \\Rightarrow C \\in A)]$");
    ui->cmbTestset->addItem("math: De Morgan's law", "$\\neg(P\\land Q)\\iff(\\neg P)\\lor(\\neg Q)$ or $\\overline{\\bigcap_{i \\in I} A_{i}}\\equiv\\bigcup_{i \\in I} \\overline{A_{i}}$ or $\\overline{A \\cup B}\\equiv\\overline{A} \\cap \\overline{B}$");
    ui->cmbTestset->addItem("math: combination", "$\\binom{n}{k} = \\frac{n(n-1)...(n-k+1)}{k(k-1)\\dots1}=\\frac{n!}{k!(n-k)!}$");
    ui->cmbTestset->addItem("math: Sophomore's dream 1", "$\\int_0^1 x^{-x}\\,dx = \\sum_{n=1}^\\infty n^{-n}(\\scriptstyle{= 1.29128599706266354040728259059560054149861936827\\dots)}$");
    ui->cmbTestset->addItem("math: Sophomore's dream 2", "$\\int_0^1 x^x   \\,dx = \\sum_{n=1}^\\infty (-1)^{n+1}n^{-n} = - \\sum_{n=1}^\\infty (-n)^{-n} (\\scriptstyle{= 0.78343051071213440705926438652697546940768199014\\dots})$");
    ui->cmbTestset->addItem("math: divergence 1", "$\\operatorname{div}\\vec{F} = \\nabla\\cdot\\vec{F}=\\frac{\\partial U}{\\partial x}+\\frac{\\partial V}{\\partial y}+\\frac{\\partial W}{\\partial z}$");
    ui->cmbTestset->addItem("math: divergence 2", "$\\overrightarrow{\\operatorname{div}}\\,(\\mathbf{\\underline{\\underline{\\epsilon}}}) = "
                            "\\begin{bmatrix}"
                            "\\frac{\\partial \\epsilon_{xx}}{\\partial x} +\\frac{\\partial \\epsilon_{yx}}{\\partial y} +\\frac{\\partial \\epsilon_{zx}}{\\partial z} \\\\"
                            "\\frac{\\partial \\epsilon_{xy}}{\\partial x} +\\frac{\\partial \\epsilon_{yy}}{\\partial y} +\\frac{\\partial \\epsilon_{zy}}{\\partial z} \\\\"
                            "\\frac{\\partial \\epsilon_{xz}}{\\partial x} +\\frac{\\partial \\epsilon_{yz}}{\\partial y} +\\frac{\\partial \\epsilon_{zz}}{\\partial z}"
                            "\\end{bmatrix}$");
    ui->cmbTestset->addItem("math: lim, sum ...", "$\\lim_{x\\to\\infty} f(x) = \\binom{k}{r} + \\frac{a}{b} \\sum_{n=1}^\\infty a_n + \\displaystyle{ \\left\\{ \\frac{1}{13} \\sum_{n=1}^\\infty b_n \\right\\} }.$");
    ui->cmbTestset->addItem("math: array test", "$f(x) := \\left\\{\\begin{array} x^2 \\sin \\frac{1}{x} & \\textrm{if } x \\ne 0, \\\\ 0 & \\textrm{if } x = 0 . \\end{array}\\right.$");
    ui->cmbTestset->addItem("math: Schwinger-Dyson", "$\\left\\langle\\psi\\left|\\mathcal{T}\\{F \\phi^j\\}\\right|\\psi\\right\\rangle=\\left\\langle\\psi\\left|\\mathcal{T}\\{iF_{,i}D^{ij}-FS_{int,i}D^{ij}\\}\\right|\\psi\\right\\rangle.$");
    ui->cmbTestset->addItem(QString(QString("math: Schr")+QChar(0xF6)+"dinger's equation"), QString("Schr")+QChar(0xF6)+"dinger's equation: $\\left[-\\frac{\\hbar^2}{2m}\\frac{\\partial^2}{\\partial x^2}+V\\right]\\Psi(x)=\\mathrm{i}\\hbar\\frac{\\partial}{\\partial t}\\Psi(x)$");
    ui->cmbTestset->addItem("math: Cauchy-Schwarz inequality", "$\\left( \\sum_{k=1}^n a_k b_k \\right)^2 \\leq \\left( \\sum_{k=1}^n a_k^2 \\right) \\left( \\sum_{k=1}^n b_k^2 \\right)$");
    ui->cmbTestset->addItem("math: Maxwell's equations", "$\\begin{aligned}\\nabla \\times \\vec{\\mathbf{B}} -\\, \\frac{1}{c}\\, \\frac{\\partial\\vec{\\mathbf{E}}}{\\partial t} & = \\frac{4\\pi}{c}\\vec{\\mathbf{j}} \\\\   \\nabla \\cdot \\vec{\\mathbf{E}} & = 4 \\pi \\rho \\\\\\nabla \\times \\vec{\\mathbf{E}}\\, +\\, \\frac{1}{c}\\, \\frac{\\partial\\vec{\\mathbf{B}}}{\\partial t} & = \\vec{\\mathbf{0}} \\\\\\nabla \\cdot \\vec{\\mathbf{B}} & = 0 \\end{aligned}$");
    ui->cmbTestset->addItem("math: Langevin Equation", "$m \\dot{v}(t) = -\\gamma v(t) + F(x,t)+ f(t)$");
    ui->cmbTestset->addItem("math: Fokker-Planck Equation", "$\\frac{\\partial}{\\partial t}P(y,t)=-\\frac{\\partial}{\\partial y}\\left[ A(y,t)P(y,t)\\right] +\\frac{\\Gamma}{2}\\frac{\\partial^2}{\\partial y^2}\\left[ P(y,t)\\right]$");
    ui->cmbTestset->addItem("math: Hamilton Equations of motion", "$\\mathcal{H}(\\mathbf{q},\\mathbf{p})=\\frac{\\mathbf{p}^2}{2\\,m}+V(\\mathbf{q})\\ \\ \\ \\text{and}\\ \\ \\ \\dot{q}_k =\\frac{p_k}{m}\\ ,\\ \\dot{p}_k = - \\frac{\\partial V}{\\partial q_k}$");
    ui->cmbTestset->addItem("math: Gaussian Distrubution", "$f(x | \\mu,\\sigma^2)=\\frac{1}{\\sqrt{2\\pi\\sigma^2}}\\operatorname{exp}\\left(-\\frac{(x-\\mu)^2}{2\\sigma^2}\\right)=\\frac{1}{\\sqrt{2\\pi\\sigma^2}} e^{-\\frac{(x-\\mu)^2}{2\\sigma^2}}\\quad -\\infty<x<\\infty$");
    ui->cmbTestset->addItem("math: quadratic equation", "$x_{1/2} = \\frac{-b\\pm\\sqrt{b^2-4ac}}{2a}$");
    ui->cmbTestset->addItem("math 1", "$f(x)=\\int_{-\\infty}^xe^{-t^2}\\;\\mathrm{d}t$");
    ui->cmbTestset->addItem("math 2", "$\\sum_{i=1}^\\infty\\frac{-e^{i\\pi}}{2^n}$");
    ui->cmbTestset->addItem("math 3", "$\\mbox{det} \\begin{pmatrix} 1 & x_1 & \\ldots & x_1^{n-1} \\\\ 1 & x_2 & \\ldots & x_2^{n-1} \\\\ \\vdots & \\vdots & \\ddots & \\vdots \\\\ 1 & x_n & \\ldots & x_n^{n-1} \\end{pmatrix} = \\prod_{1 \\leq i < j \\leq n} (x_j - x_i) $");
    ui->cmbTestset->addItem("math 4", "$\\Re(z) =\\frac{n\\pi \\dfrac{\\theta +\\psi}{2}}{\\left(\\dfrac{\\theta +\\psi}{2}\\right)^2 + \\left( \\dfrac{1}{2}\\log \\left\\lvert\\dfrac{B}{A}\\right\\rvert\\right)^2}.$");
    ui->cmbTestset->addItem("math 5", "$\\sum_{m=1}^\\infty\\sum_{n=1}^\\infty\\frac{m^2\\,n}{3^m\\left(m\\,3^n+n\\,3^m\\right)}$");
    ui->cmbTestset->addItem("math 6", "$\\phi_n(\\kappa) =\\frac{1}{4\\pi^2\\kappa^2} \\int_0^\\infty\\frac{\\sin(\\kappa R)}{\\kappa R}\\frac{\\partial}{\\partial R}\\left[R^2\\frac{\\partial D_n(R)}{\\partial R}\\right]\\,dR$");
    ui->cmbTestset->addItem("math 7", "${}_pF_q(a_1,\\dots,a_p;c_1,\\dots,c_q;z)= \\sum_{n=0}^\\infty\\frac{(a_1)_n\\cdots(a_p)_n}{(c_1)_n\\cdots(c_q)_n}\\frac{z^n}{n!}$");
    ui->cmbTestset->addItem("User-Editable Text");
    //
    //ui->cmbTestset->addItem("", "$$");
    //ui->cmbTestset->addItem("", "$$");
    //ui->cmbTestset->addItem("", "");

    JKQTMathText mt(this);
    ui->cmbUnicodeSerif->setCurrentFont(QFont(mt.getFontRoman()));
    ui->cmbEncodingSerif->setCurrentIndex(static_cast<int>(mt.getFontEncodingRoman()));
    ui->cmbUnicodeSans->setCurrentFont(QFont(mt.getFontSans()));
    ui->cmbEncodingSans->setCurrentIndex(static_cast<int>(mt.getFontEncodingSans()));
    ui->cmbUnicodeSerifMath->setCurrentFont(QFont(mt.getFontMathRoman()));
    ui->cmbEncodingSerifMath->setCurrentIndex(static_cast<int>(mt.getFontEncodingMathRoman()));
    ui->cmbUnicodeSansMath->setCurrentFont(QFont(mt.getFontMathSans()));
    ui->cmbEncodingSansMath->setCurrentIndex(static_cast<int>(mt.getFontEncodingMathSans()));
    ui->cmbUnicodeTypewriter->setCurrentFont(QFont(mt.getFontTypewriter()));
    ui->cmbEncodingTypewriter->setCurrentIndex(static_cast<int>(mt.getFontEncodingTypewriter()));
    ui->cmbCaligraphic->setCurrentFont(QFont(mt.getFontCaligraphic()));
    ui->cmbEncodingCaligraphic->setCurrentIndex(static_cast<int>(mt.getFontEncodingCaligraphic()));
    ui->cmbScript->setCurrentFont(QFont(mt.getFontScript()));
    ui->cmbEncodingScript->setCurrentIndex(static_cast<int>(mt.getFontEncodingScript()));
    ui->cmbUnicodeFraktur->setCurrentFont(QFont(mt.getFontFraktur()));
    ui->cmbEncodingFraktur->setCurrentIndex(static_cast<int>(mt.getFontEncodingFraktur()));
    ui->cmbUnicodeBlackboard->setCurrentFont(QFont(mt.getFontBlackboard()));
    ui->cmbEncodingBlackboard->setCurrentIndex(static_cast<int>(mt.getFontEncodingBlackboard()));
    ui->cmbUnicodeSymbol->setCurrentFont(QFont(mt.getFallbackFontSymbols()));
    ui->cmbEncodingSymbol->setCurrentIndex(static_cast<int>(mt.getFontEncodingFallbackFontSymbols()));
    ui->cmdBlackboradMode->setCurrentText(JKQTMathTextBlackboradDrawingMode2String(mt.getFontBlackboradMode()));




    ui->cmbTestset->setCurrentIndex(0);

    ui->labMath->setMath("$\\left(\\left[\\sqrt{2\\pi\\cdot\\int_{-\\infty}^\\infty f(x)\\;\\mathrm{d}x}\\right]\\right)$");
    ui->cmbFont->setCurrentIndex(1);

    connect(ui->chkBoxes, SIGNAL(toggled(bool)), this, SLOT(updateMath()));
    connect(ui->chkBigBox, SIGNAL(toggled(bool)), this, SLOT(updateMath()));
    connect(ui->chkAntiAlias, SIGNAL(toggled(bool)), this, SLOT(updateMath()));
    connect(ui->chkAntiAliasText, SIGNAL(toggled(bool)), this, SLOT(updateMath()));
    connect(ui->cmdBlackboradMode, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbLastAlign, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbFont, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbScript, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbSizeUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbTestset, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbCaligraphic, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeSans, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeSansMath, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingSans, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingSansMath, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeTypewriter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeSerif, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeSerifMath, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingSerif, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingSerifMath, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeSymbol, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingScript, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingSymbol, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeFraktur, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingFraktur, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbUnicodeBlackboard, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingBlackboard, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingTypewriter, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->cmbEncodingCaligraphic, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMath()));
    connect(ui->edtSizes, SIGNAL(textChanged(QString)), this, SLOT(updateMath()));


    updateMath();
}

TestForm::~TestForm()
{
    delete ui;
}

#define W 4000
#define H 2000
#define X1 15
#define Y1 50



double TestForm::draw(QPainter& painter, double X, double YY, JKQTMathText& mt, QString name, double& durationSizingMS, double&durationTimingMS, QStringList* lstErrors) {


    double Y=YY;
    painter.save();
    ht.start();
    QSizeF s=mt.getSize(painter);
    Y=Y+mt.getAscent(painter);
    durationSizingMS=ht.getTime()/1000.0;
    qDebug()<<"    sizing in "<<durationSizingMS<<" ms\n";
    QPen p=painter.pen();
    if (ui->chkBigBox->isChecked()) {
        p.setColor("lightcoral");
        p.setStyle(Qt::DashLine);
        p.setWidth(2);
        painter.setPen(p);
        QRectF r(X, Y-mt.getAscent(painter),s.width(), s.height());
        painter.drawRect(r);
        p.setColor("lightblue");
        painter.setPen(p);
        painter.drawLine(X, Y, X+s.width(), Y);
    }
    qDebug()<<getFonts(mt);
    ht.start();
    p.setStyle(Qt::SolidLine);
    p.setWidth(1);
    p.setColor("black");
    painter.setPen(p);
    mt.draw(painter, X, Y, ui->chkBoxes->isChecked());
    durationTimingMS=ht.getTime()/1000.0;
    qDebug()<<"   drawing in "<<durationTimingMS<<" ms";
    p.setColor("blue");
    painter.setPen(p);
    if (lstErrors) *lstErrors=mt.getErrorList();

    QFont f;
    f.setFamily("sans serif");
    f.setUnderline(true);
    f.setPointSize(10);
    painter.setFont(f);
    painter.drawText(X, Y-mt.getAscent(painter)-6, name+":");
    painter.restore();
    qDebug()<<name<<":  width="<<s.width()<<"  height="<<s.height()<<"  ascent="<<mt.getAscent(painter)<<"  descent="<<mt.getDescent(painter);
    return mt.getDescent(painter)+mt.getAscent(painter)+40;
}

double TestForm::drawAligned(QPainter& painter, double X, double YY, JKQTMathText& mt, QString name) {


    double Y=YY;
    painter.save();
    ht.start();
    const QSizeF s=mt.getSize(painter);
    const QRectF rect(X, Y, s.width()+32, s.height()+32);
    int flags=Qt::AlignLeft|Qt::AlignTop;
    switch(ui->cmbLastAlign->currentIndex()) {
    case 0: flags=Qt::AlignLeft|Qt::AlignTop; break;
    case 1: flags=Qt::AlignLeft|Qt::AlignVCenter; break;
    case 2: flags=Qt::AlignLeft|Qt::AlignBottom; break;
    case 3: flags=Qt::AlignHCenter|Qt::AlignTop; break;
    case 4: flags=Qt::AlignHCenter|Qt::AlignVCenter; break;
    case 5: flags=Qt::AlignHCenter|Qt::AlignBottom; break;
    case 6: flags=Qt::AlignRight|Qt::AlignTop; break;
    case 7: flags=Qt::AlignRight|Qt::AlignVCenter; break;
    case 8: flags=Qt::AlignRight|Qt::AlignBottom; break;
    }

    qDebug()<<getFonts(mt);
    QPen p=painter.pen();
    p.setColor("darkred");
    p.setStyle(Qt::SolidLine);
    p.setWidth(2);
    painter.setPen(p);
    painter.drawRect(rect);
    p.setStyle(Qt::SolidLine);
    p.setWidth(1);
    p.setColor("black");
    painter.setPen(p);
    mt.draw(painter, flags, rect, ui->chkBoxes->isChecked());
    p.setColor("blue");
    painter.setPen(p);

    QFont f;
    f.setFamily("sans serif");
    f.setUnderline(true);
    f.setPointSize(10);
    painter.setFont(f);
    painter.drawText(X, Y-6, name+":");
    painter.restore();
    qDebug()<<name<<":  width="<<s.width()<<"  height="<<s.height()<<"  ascent="<<mt.getAscent(painter)<<"  descent="<<mt.getDescent(painter);
    qDebug()<<getFonts(mt);
    return rect.bottom()+40;
}

QString TestForm::getFonts(const JKQTMathText& mt) const
{
    std::ostringstream str;
    str<<"Roman: "<<mt.getFontRoman().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingRoman()).toStdString()<<"\n";
    str<<"Math-Roman: "<<mt.getFontMathRoman().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingMathRoman()).toStdString()<<"\n";
    str<<"Sans: "<<mt.getFontSans().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingSans()).toStdString()<<"\n";
    str<<"Math-Sans: "<<mt.getFontMathSans().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingMathSans()).toStdString()<<"\n";
    str<<"FallbackSymbols: "<<mt.getFallbackFontSymbols().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingFallbackFontSymbols()).toStdString()<<"\n";
    str<<"Fraktur: "<<mt.getFontFraktur().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingFraktur()).toStdString()<<"\n";
    str<<"Script: "<<mt.getFontScript().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingScript()).toStdString()<<"\n";
    str<<"Typewriter: "<<mt.getFontTypewriter().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingTypewriter()).toStdString()<<"\n";
    str<<"Caligraphic: "<<mt.getFontCaligraphic().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingCaligraphic()).toStdString()<<"\n";
    str<<"Blackboard: "<<mt.getFontBlackboard().toStdString()<<", "<<JKQTMathTextFontEncoding2String(mt.getFontEncodingBlackboard()).toStdString()<<", mode="<<JKQTMathTextBlackboradDrawingMode2String(mt.getFontBlackboradMode()).toStdString()<<"\n";
    return str.str().c_str();
}

QTreeWidgetItem *TestForm::createTree(JKQTMathTextNode *node, QTreeWidgetItem* parent)
{
    QString name;
    JKQTMathTextTextNode* txtN=dynamic_cast<JKQTMathTextTextNode*>(node);
    JKQTMathTextWhitespaceNode* spN=dynamic_cast<JKQTMathTextWhitespaceNode*>(node);
    JKQTMathTextSymbolNode* symN=dynamic_cast<JKQTMathTextSymbolNode*>(node);
    JKQTMathTextHorizontalListNode* lstN=dynamic_cast<JKQTMathTextHorizontalListNode*>(node);
    JKQTMathTextVerticalListNode* lstNV=dynamic_cast<JKQTMathTextVerticalListNode*>(node);
    JKQTMathTextModifiedTextPropsInstructionNode* inst1N=dynamic_cast<JKQTMathTextModifiedTextPropsInstructionNode*>(node);
    JKQTMathTextBoxInstructionNode* inst1B=dynamic_cast<JKQTMathTextBoxInstructionNode*>(node);
    JKQTMathTextSimpleInstructionNode* instS=dynamic_cast<JKQTMathTextSimpleInstructionNode*>(node);
    JKQTMathTextSubscriptNode* subN=dynamic_cast<JKQTMathTextSubscriptNode*>(node);
    JKQTMathTextSuperscriptNode* superN=dynamic_cast<JKQTMathTextSuperscriptNode*>(node);
    JKQTMathTextBraceNode* braceN=dynamic_cast<JKQTMathTextBraceNode*>(node);
    JKQTMathTextSqrtNode* sqrtN=dynamic_cast<JKQTMathTextSqrtNode*>(node);
    JKQTMathTextFracNode* fracN=dynamic_cast<JKQTMathTextFracNode*>(node);
    JKQTMathTextMatrixNode* matrixN=dynamic_cast<JKQTMathTextMatrixNode*>(node);
    JKQTMathTextDecoratedNode* decoN=dynamic_cast<JKQTMathTextDecoratedNode*>(node);
    JKQTMathTextEmptyBoxNode* emptyN=dynamic_cast<JKQTMathTextEmptyBoxNode*>(node);
    JKQTMathTextVerbatimNode* verbN=dynamic_cast<JKQTMathTextVerbatimNode*>(node);
    JKQTMathTextPhantomNode* phanN=dynamic_cast<JKQTMathTextPhantomNode*>(node);

    QTreeWidgetItem* ti=nullptr;
    if (parent) ti=new QTreeWidgetItem(parent);
    else ti=new QTreeWidgetItem(ui->tree);

    if (decoN) {
        name=QString("DecoratedNode: mode='%1'").arg(JKQTMathTextDecoratedNode::DecorationType2String(decoN->getDecoration()));
        if (decoN->getChild()) ti->addChild(createTree(decoN->getChild(), ti));
    } else if (matrixN)  {
        int l=matrixN->getLines();
        int c=matrixN->getColumns();
        name=QString("MatrixNode: l*c=%1*%2").arg(l).arg(c);
        QVector<QVector<JKQTMathTextNode*> > children=matrixN->getChildren();
        for (int y=0; y<l; y++) {
            for (int x=0; x<c; x++) {
                if (children[y].at(x)!=nullptr) {
                    QTreeWidgetItem* it=createTree(children[y].at(x), ti);
                    it->setText(0, QString("(%1,%2):").arg(y).arg(x)+it->text(0));
                    ti->addChild(it);
                }
            }
        }
    } else if (fracN)  {
        name=QString("FracNode: mode='%1'").arg(JKQTMathTextFracNode::FracType2String(fracN->getMode()));
        if (fracN->getChild1()) ti->addChild(createTree(fracN->getChild1(), ti));
        if (fracN->getChild2()) ti->addChild(createTree(fracN->getChild2(), ti));
    } else if (sqrtN)  {
        name=QString("SqrtNode");
        if (sqrtN->getChildDegree()) ti->addChild(createTree(sqrtN->getChildDegree(), ti));
        if (sqrtN->getChild()) ti->addChild(createTree(sqrtN->getChild(), ti));
    } else if (phanN)  {
        name=QString("PhantomNode: mode=%1").arg(phanN->getInstructionName());
        if (sqrtN->getChild()) ti->addChild(createTree(sqrtN->getChild(), ti));
    } else if (braceN)  {
        name=QString("BraceNode: l='%1', r='%2'").arg(JKQTMathTextBraceType2String(braceN->getOpenbrace())).arg(JKQTMathTextBraceType2String(braceN->getClosebrace()));
        if (braceN->getChild()) ti->addChild(createTree(braceN->getChild(), ti));
    } else if (superN)  {
        name=QString("SuperscriptNode");
        if (superN->getChild()) ti->addChild(createTree(superN->getChild(), ti));
    } else if (subN)  {
        name=QString("SubscriptNode");
        if (subN->getChild()) ti->addChild(createTree(subN->getChild(), ti));
    } else if (instS)  {
        name=QString("SimpleInstructionNode: \'%1\' (subsuper=%2, params=%3)").arg(instS->getInstructionName()).arg(instS->isSubSuperscriptAboveBelowNode()).arg(instS->getParameters().join("/"));
    } else if (inst1N)  {
        name=QString("ModifiedTextPropsInstructionNode: \'%1\' (subsuper=%2, params=%3)").arg(inst1N->getInstructionName()).arg(inst1N->isSubSuperscriptAboveBelowNode()).arg(inst1N->getParameters().join("/"));
        if (inst1N->getChild()) ti->addChild(createTree(inst1N->getChild(), ti));
    } else if (inst1B)  {
        name=QString("BoxInstructionNode: \'%1\' (subsuper=%2, params=%3)").arg(inst1B->getInstructionName()).arg(inst1B->isSubSuperscriptAboveBelowNode()).arg(inst1B->getParameters().join("/"));
        if (inst1B->getChild()) ti->addChild(createTree(inst1B->getChild(), ti));
    } else if (lstN)  {
        name=QString("HorizontalListNode");
        QList<JKQTMathTextNode*> list=lstN->getChildren();
        for (int i=0; i<list.size(); i++) {
            ti->addChild(createTree(list[i], ti));
        }
    } else if (lstNV)  {
        name=QString("VerticalListNode (align=%1, spacingFactor=%2x, spacingMode=%3, verticalOrientation=%4)").arg(JKQTMathTextHorizontalAlignment2String(lstNV->getAlignment())).arg(lstNV->getLineSpacingFactor()).arg(lstNV->SpacingMode2String(lstNV->getSpacingMode())).arg(JKQTMathTextVerticalOrientation2String(lstNV->getVerticalOrientation()));
        QList<JKQTMathTextNode*> list=lstNV->getChildren();
        for (int i=0; i<list.size(); i++) {
            ti->addChild(createTree(list[i], ti));
        }
    } else if (verbN)  {
        name=QString("VerbatimTextNode (align=%1, spacingFactor=%2x, verticalOrientation=%3, text='%4')").arg(JKQTMathTextHorizontalAlignment2String(verbN->getAlignment())).arg(verbN->getLineSpacingFactor()).arg(JKQTMathTextVerticalOrientation2String(verbN->getVerticalOrientation())).arg(jkqtp_backslashEscape(verbN->getText()));
    } else if (symN)  {
        name=QString("SymbolNode: \'%1\' (subsuper=%3)").arg(symN->getSymbolName()).arg(symN->isSubSuperscriptAboveBelowNode());
    } else if (spN)  {
        name=QString("WhitespaceNode :type=%1, count=%2").arg(spN->Type2String(spN->getWhitespaceType())).arg(spN->getWhitespaceCount());
    } else if (txtN)  {
        name=QString("TextNode: \'%1\'").arg(txtN->getText());
    } else if (emptyN)  {
        name=QString("EmptyBoxNode %1%2 x %3%4").arg(emptyN->getWidth()).arg(JKQTMathTextEmptyBoxNode::Units2String(emptyN->getWidthUnit())).arg(emptyN->getHeight()).arg(JKQTMathTextEmptyBoxNode::Units2String(emptyN->getHeightUnit()));
    } else {
        name=QString("unknown");
    }

    ti->setText(0,name);

    /*
    QString space="";
    QTreeWidgetItem* p=ti->parent();
    while (p) {
        space+="  ";
        p=p->parent();
    }

    qDebug()<<space<<"createTree()";
    */

    return ti;
}

void TestForm::updateMath()
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString mathTest="";
    //QString symbolTest="\\lfloor\\rfloor\\lceil\\rceil\\langle\\rangle\\sum\\int \\iint \\oint \\prod \\leftrightarrow \\leftarrow\\Leftarrow\\rightarrow\\Rightarrow\\pm\\mp\\leq\\geq\\ll\\gg\\hbar\\euro\\bbC\\bbH\\bbN\\bbP\\bbQ\\bbZ\\bbR\\Angstrom\\Alef\\Bet\\Gimel\\Dalet\\nexists\\ni\\notni\\circ\\tilde\\oiint\\oiiint\\emptyset\\odot\\ominus\\subsetnot\\DC\\bot\\cdots\\perthousand\\leftharpoonup\\rightharpoonup \\upharpoonleft \\downharpoonleft \\leftrightharpoon \\rightleftharpoon \\coprod \\leftharpoondown \\rightharpoondown \\nwarrow \\nearrow \\mapsto \\cent \\pound \\yen \\div \\multimap \\maporiginal \\mapimage \\bigcap \\bigcup \\benzene \\times \\cdot \\propto \\equiv \\Im \\Re \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ ";
    //QString mathTest="$\\left\\|\\stackrel{1}{2}\\right\\|\\cdot\\left|\\begin{Bmatrix}1123&2^2&\\frac{3}{4}\\\\4&5_5&6\\\\72_8&8_{\\text{l\"{a}erer Index}}&9&10&11\\end{Bmatrix}\\right|\\cdots f(x)$";
    //QString mathTest="0";
    //QString text="x_0 \\sqrt{a^2+b^2}\\underbrace{underbrace}{x_0} (\\bf{Test})\\_\\it{Text} ... ! Equator\\alpha\\beta $f(x)=\\sin(x{\\cdot}y)$ \\ul{Text\\ol{\\gamma}} \\script{Script}, \\tt{Typewriter}, \\sf{sans serif}";
    //QString text="$\\sum_{i=0_{j=-1_{k=-45}}}^{n^{m^{123456789}}}\\left(\\left(\\left(\\int_0^\\inf\\left[r+i^{\\left\\{2^2\\right\\}}\\right]{\\dd}r\\right)\\right)\\right)$ \\bf{Test}\\_\\it{Text} ... ! \\sc{&Equator \\&}\\alpha\\beta $f(x)=\\sin(x\\cdot\\cos(a+{\\ii}b))$ \\ul{Text\\gamma} \\script{Script-Text}\\tt{ and Typewriter} \\sf{also sans serif}";
    //QString text="$\\sum_{i=0_{j=-1_{k=-45}}}^2\\left(\\left(\\left(\\int_0^\\inf\\left[r+i^{\\left\\{2^2\\right\\}}\\right]{\\dd}r\\right)\\right)\\right)$ \\bf{Test}\\_\\it{Text} ... ! \\sc{&Equator \\&}\\alpha\\beta $f(x)=\\sin(x\\cdot\\cos(a+{\\ii}b))$ \\ul{Text\\gamma} \\script{Script-Text}\\tt{ and Typewriter} \\sf{also sans serif}";

    //QString text="x_0My Text$\\sqrt{a^2+b^2 } \\underbrace{underbrace}{x_0}$";

    if (ui->cmbTestset->currentIndex()==ui->cmbTestset->count()-1) {
        mathTest=ui->textBrowserSource->toPlainText();
        ui->textBrowserSource->setReadOnly(false);
        ui->btnRender->setEnabled(true);
    } else {
        mathTest=ui->cmbTestset->itemData(ui->cmbTestset->currentIndex()).toString();
        ui->textBrowserSource->setPlainText(mathTest);
        ui->textBrowserSource->setReadOnly(true);
        ui->btnRender->setEnabled(false);
    }

    ui->scrollArea->setBackgroundRole(QPalette::Dark);

    const qreal dpr = devicePixelRatioF();
    QPixmap pix(W*dpr, H*dpr);
    pix.fill();
    pix.setDevicePixelRatio(dpr);
    QPainter painter;
    JKQTMathText mt(this);


    double Y=Y1;

    painter.begin(&pix);
    if (ui->chkAntiAlias->isChecked()) painter.setRenderHint(QPainter::Antialiasing);
    if (ui->chkAntiAliasText->isChecked()) painter.setRenderHint(QPainter::TextAntialiasing);
    ht.start();


    mt.setFallbackFontSymbols(ui->cmbUnicodeSymbol->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingSymbol->currentIndex()));
    if (ui->cmbFont->currentIndex()<=3) {
        mt.setFontRoman(ui->cmbUnicodeSerif->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingSerif->currentIndex()));
        mt.setFontSans(ui->cmbUnicodeSans->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingSans->currentIndex()));
        mt.setFontMathRoman(ui->cmbUnicodeSerifMath->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingSerifMath->currentIndex()));
        mt.setFontMathSans(ui->cmbUnicodeSansMath->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingSansMath->currentIndex()));
        mt.setFontTypewriter(ui->cmbUnicodeTypewriter->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingTypewriter->currentIndex()));
        mt.setFontCaligraphic(ui->cmbCaligraphic->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingCaligraphic->currentIndex()));
        mt.setFontScript(ui->cmbScript->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingScript->currentIndex()));
        mt.setFontFraktur(ui->cmbUnicodeFraktur->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingFraktur->currentIndex()));
        mt.setFontBlackboard(ui->cmbUnicodeBlackboard->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingBlackboard->currentIndex()));
        mt.setFallbackFontSymbols(ui->cmbUnicodeSymbol->currentFont().family(), static_cast<JKQTMathTextFontEncoding>(ui->cmbEncodingSymbol->currentIndex()));
    } else if (ui->cmbFont->currentIndex()==5 || ui->cmbFont->currentIndex()==6) {
        mt.setFontRoman(QGuiApplication::font().family());
        mt.setFontMathRoman(QGuiApplication::font().family());
    } else if (ui->cmbFont->currentIndex()==7) {
        mt.useXITS();
        mt.useAnyUnicode("Times New Roman", "Times New Roman");
    } else if (ui->cmbFont->currentIndex()==8) {
        mt.useXITS();
        mt.useAnyUnicode("Arial", "Arial");
    } else if (ui->cmbFont->currentIndex()==9) {
        mt.useXITS();
        mt.useAnyUnicode("Courier New", "Courier New");
    } else if (ui->cmbFont->currentIndex()==10) {
        mt.useXITS();
        mt.useAnyUnicode("Comic Sans MS", "Comic Sans MS");
    } else if (ui->cmbFont->currentIndex()==11) {
        mt.useAnyUnicodeForTextOnly("Times New Roman", "Times New Roman");
        mt.useXITS();
    } else if (ui->cmbFont->currentIndex()==12) {
        mt.useAnyUnicodeForTextOnly("Arial", "Arial");
        mt.useXITS();
    } else if (ui->cmbFont->currentIndex()==13) {
        mt.useAnyUnicodeForTextOnly("Courier New", "Courier New");
        mt.useXITS();
    } else if (ui->cmbFont->currentIndex()==14) {
        mt.useAnyUnicodeForTextOnly("Comic Sans MS", "Comic Sans MS");
        mt.useXITS();
    } else if (ui->cmbFont->currentIndex()==15) {
        mt.useXITS(false);
    }

    mt.setFontBlackboradMode(String2JKQTMathTextBlackboradDrawingMode(ui->cmdBlackboradMode->currentText()));

    if (ui->cmbFont->currentIndex()==1) qDebug()<<"useXITS: "<<mt.useXITS();
    else if (ui->cmbFont->currentIndex()==2) qDebug()<<"useSTIX: "<<mt.useSTIX();
    else if (ui->cmbFont->currentIndex()==3) qDebug()<<"useASANA: "<<mt.useASANA();
    else if (ui->cmbFont->currentIndex()==6) qDebug()<<"useXITS: "<<mt.useXITS();
    ui->tree->clear();
    ht.start();
    double durationParse=0;
    if (mt.parse(mathTest)) {
        durationParse=ht.getTime()/1000.0;
        ui->tree->addTopLevelItem(createTree(mt.getParsedNode()));
    } else {
        durationParse=ht.getTime()/1000.0;
    }
    ui->labParsingTimes->setText(QString("   %1ms").arg(durationParse, 0, 'f', 3));
    ui->tree->expandAll();
    bool okh=true;
    ui->textBrowser->clear();
    qDebug()<<"parse mathTest in "<<ht.getTime()/1000.0<<" ms\n";

    QStringList sl=ui->edtSizes->text().split(",");
    ui->labRenderTimes->setText("");

    for (int i=0; i<sl.size()-1; i++) {
        bool ok=true;
        int size=sl[i].trimmed().toUInt(&ok);
        if (!ok) size=10+i*5;
        QString unit="";
        if (ui->cmbSizeUnit->currentIndex()==0) { unit="pt"; mt.setFontSize(size); }
        else {unit="px"; mt.setFontSizePixels(size); }
        double durationSizingMS=0, durationTimingMS=0;
        Y+=draw(painter, X1, Y, mt, QString("%1, %2, %3"+unit).arg(ui->cmbTestset->currentText()).arg(ui->cmbFont->currentText()).arg(size), durationSizingMS, durationTimingMS);

        if (i==0) {
            ui->labError->clear();
            if (mt.getErrorList().size()>0) {
                ui->labError->setHtml(QString("<span color=\"red\">"+mt.getErrorList().join("<br>")+"</span><br><br><b>Fonts:</b><br><i>%1</i>").arg(getFonts(mt).replace("\n", "<br>")));
            } else {
                ui->labError->setHtml(QString("<span color=\"green\">OK</span><br><br><b>Fonts:</b><br><i>%1</i>").arg(getFonts(mt).replace("\n", "<br>")));
            }
        }

        ui->labRenderTimes->setText(ui->labRenderTimes->text()+QString("     %1pt: %2ms/%3ms").arg(size).arg(durationSizingMS, 0, 'F', 1).arg(durationTimingMS, 0, 'F', 1));
        if (i==0) {
            ui->textBrowser->textCursor().insertHtml("<hr>"+mt.toHtml(&okh)+"<hr><br><br>");
            qDebug()<<"HTML: ---------------------------------------------\n"<<mt.toHtml(&okh)<<"\nHTML: --------------------------------------------- ok="<<okh;
            if (mt.getErrorList().size()>0) {
                qDebug()<<mt.getErrorList().join("\n")<<"\n";
            }
        }
    }

    if (sl.size()>0) {
        bool ok=true;
        int size=sl.last().trimmed().toUInt(&ok);
        if (!ok) size=font().pointSizeF();
        QString unit;
        if (ui->cmbSizeUnit->currentIndex()==0) { unit="pt"; mt.setFontSize(size); }
        else { unit="px"; mt.setFontSizePixels(size); }
        Y+=drawAligned(painter, X1, Y, mt, QString("%1, %2"+unit+", align: %3").arg(ui->cmbTestset->currentText()).arg(size).arg(ui->cmbLastAlign->currentText()));
    }

    painter.end();


    ui->label->setPixmap(pix);
    QApplication::restoreOverrideCursor();
}
