#include "testform.h"
#include "ui_testform.h"
#include <QDebug>


TestForm::TestForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi(this);
    ui->cmbTestset->addItem("simple relations", "$a{\\leq}b$, $a{\\geq}b$, $a{\\equiv}b$, $a=b$, $a{\\neq}b$, $a<b$, $a>b$");
    ui->cmbTestset->addItem("simple relations in different modes", "math: $a{\\leq}b$, math/no braces: $a\\leq b$, no math: a{\\leq}b, no math/no braces: a\\leq b");
    ui->cmbTestset->addItem("named symbols 1", "ll: $\\ll$\\ gg: $\\gg$\\ leq: $\\leq$\\ geq: $\\geq$\\ pm: $\\pm$\\ mp: $\\mp$\\ ");
    ui->cmbTestset->addItem("named symbols 2", "nexists: $\\nexists$\\ ni: $\\ni$\\ notni: $\\notni$\\ circ: $\\circ$\\ sim: $\\sim$\\ emptyset: $\\emptyset$\\ odot: $\\odot$\\ ominus: $\\ominus$\\ subsetnot: $\\subsetnot$\\ bot: $\\bot$");
    ui->cmbTestset->addItem("named symbols 3", "leftharpoonup: $\\leftharpoonup$\\ rightharpoonup: $\\rightharpoonup$\\ upharpoonleft: $\\upharpoonleft$\\ downharpoonleft: $\\downharpoonleft$\\ leftrightharpoon: $\\leftrightharpoon$\\ rightleftharpoon: $\\rightleftharpoon$");
    ui->cmbTestset->addItem("named symbols 4", "coprod: $\\coprod$\\ leftharpoondown: $\\leftharpoondown$\\ rightharpoondown: $\\rightharpoondown$\\ upharpoonright: $\\upharpoonright$\\ downharpoonright: $\\downharpoonright$\\ nwarrow: $\\nwarrow$\\ nearrow: $\\nearrow$\\ ");
    ui->cmbTestset->addItem("named symbols 5", "searrow: $\\searrow$\\ swarrow: $\\swarrow$\\ mapsto: $\\mapsto$\\ div: $\\div$\\ multimap: $\\multimap$\\ maporiginal: $\\maporiginal$\\ mapimage: $\\mapimage$\\ ");
    ui->cmbTestset->addItem("named symbols 6", "times: $\\times$\\ propto: $\\propto$\\ bullet: $\\bullet$\\ neq: $\\neq$\\ ne: $\\ne$\\ equiv: $\\equiv$\\ approx: $\\approx$\\ otimes: $\\otimes$\\ oplus: $\\oplus$");
    ui->cmbTestset->addItem("named symbols 7", "oslash: $\\oslash$\\ cap: $\\cap$\\ land: $\\land$\\ cup: $\\cup$\\ lor: $\\lor$\\ supset: $\\supset$\\ supseteq: $\\supseteq$\\ supsetnot: $\\supsetnot$\\ subset: $\\subset$");
    ui->cmbTestset->addItem("named symbols 8", "subseteq: $\\subseteq$\\ in: $\\in$\\ notin: $\\notin$\\ cdot: $\\cdot$\\ wedge: $\\wedge$\\ vee: $\\vee$\\ cong: $\\cong$\\ bot: $\\bot$");
    ui->cmbTestset->addItem("symbols", "$\\ll\\gg\\leq\\geq\\leftrightarrow\\leftarrow\\rightarrow\\to\\uparrow\\downarrow\\updownarrow\\Leftrightarrow\\iff\\Leftarrow\\Rightarrow\\Uparrow\\Downarrow\\Updownarrow\\pm\\mp\\nexists\\ni\\notni\\circ\\sim\\emptyset\\odot\\ominus\\subsetnot\\bot\\leftharpoonup\\rightharpoonup\\upharpoonleft\\downharpoonleft\\leftrightharpoon\\rightleftharpoon\\coprod\\leftharpoondown\\rightharpoondown\\upharpoonright\\downharpoonright\\nwarrow\\nearrow\\searrow\\swarrow\\mapsto\\div\\multimap\\maporiginal\\mapimage\\times\\propto\\bullet\\neq\\ne\\equiv\\approx\\otimes\\oplus\\oslash\\cap\\land\\cup\\lor\\supset\\supseteq\\supsetnot\\subset\\subseteq\\in\\notin\\cdot\\wedge\\vee\\cong\\bot$");
    ui->cmbTestset->addItem("std dev", "$\\sigma_x=\\sqrt{\\langle (x-\\langle x\\rangle)^2\\rangle}=\\sqrt{\\frac{1}{N-1}\\cdot\\left( \\sum_{i=1}^N{x_i}^2-\\frac{1}{N}\\cdot\\left(\\sum_{i=1}^Nx_i\\right)^2\\right)}$");
    ui->cmbTestset->addItem("std dev 2", "$\\sigma_x=\\sqrt{\\langle (x-\\langle x\\rangle)^2\\rangle}=\\sqrt{\\frac{1}{N-1}\\cdot\\left( \\sum_{i=1}^Nx_i^2-\\frac{1}{N}\\cdot\\left(\\sum_{i=1}^Nx_i\\right)^2\\right)}$");
    ui->cmbTestset->addItem("rotation matrix", "$\\mathrm{\\mathbf{M}}(\\alpha) = \\left(\\begin{matrix}\\cos(\\alpha)+n_x^2\\cdot (1-\\cos(\\alpha))  &  n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))-n_z\\cdot \\sin(\\alpha) &  n_x\\cdot n_z\\cdot (1-\\cos(\\alpha))+n_y\\cdot \\sin(\\alpha)\\\\n_x\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_z\\cdot \\sin(\\alpha) & \\cos(\\alpha)+n_y^2\\cdot (1-\\cos(\\alpha))  &   n_y\\cdot n_z\\cdot (1-\\cos(\\alpha))-n_x\\cdot \\sin(\\alpha)\\\\n_z\\cdot n_x\\cdot (1-\\cos(\\alpha))-n_y\\cdot \\sin(\\alpha) & n_z\\cdot n_y\\cdot (1-\\cos(\\alpha))+n_x\\cdot \\sin(\\alpha)  & \\cos(\\alpha)+n_z^2\\cdot (1-\\cos(\\alpha))\\end{matrix}\\right)$");
    ui->cmbTestset->addItem("like in label at bottom (no MM)", "\\left(\\left[\\sqrt{2\\pi\\cdot\\int_{-\\infty}^\\infty f(x)\\;\\mathrm{d}x}\\right]\\right)");
    ui->cmbTestset->addItem("like in label at bottom (MM)", "$\\left(\\left[\\sqrt{2\\pi\\cdot\\int_{-\\infty}^\\infty f(x)\\;\\mathrm{d}x}\\right]\\right)$");
    ui->cmbTestset->addItem("text 0", "text");
    ui->cmbTestset->addItem("text 1", "text \\mathbf{bold}");
    ui->cmbTestset->addItem("text 2", "text \\mathbf{bold}\\textcolor{red}{RED}");
    ui->cmbTestset->addItem("decoration", "$\\vec{x}\\vec{X}\\vec{\\psi} -- \\dot{x}\\dot{X}\\dot{\\psi} -- \\ddot{x}\\ddot{X}\\ddot{\\psi} -- \\overline{x}\\overline{X}\\overline{\\psi} -- \\underline{x}\\underline{X}\\underline{\\psi} -- \\hat{x}\\hat{X}\\hat{\\psi} -- \\tilde{x}\\tilde{X}\\tilde{\\psi} -- \\uul{x}\\uul{X}\\uul{\\psi} -- \\ool{x}\\ool{X}\\ool{\\psi} -- \\bar{x}\\bar{X}\\bar{\\psi} -- \\arrow{x}\\arrow{X}\\arrow{\\psi}$");
    ui->cmbTestset->addItem("mathtest", "This is normal text: $this is math:\\langle r^2(\\tau)\\rangle=\\left\\langle (\\vec{r}(t)-\\vec{r}(t+\\tau) )^2\\right\\rangle\\ \\ \\ g(\\tau)=\\frac{1}{N}\\cdot\\left(1+\\frac{2}{3}\\frac{\\langle r^2(\\tau)\\rangle}{w_{xy}^2}\\right)^{-1}  \\lfloor\\rfloor\\lceil\\rceil\\langle\\rangle\\left\\{\\va\\left|\\|\\va\\|_2\\geq2\\right.\\right\\} \\vr\\vR\\frac{\\sqrt{\\sqrt{\\sqrt{\\sum_{i=0}^\\infty \\hat{i}^2}+y^\\alpha}+1}}{\\dot{v}\\equiv\\ddot{r}}\\argmin_{\\vec{k}}\\sum_{\\sqrt{i}=0}^{N}\\int_{x_0}^{x_1}\\left(\\left(\\left(x\\right)\\right)\\right)\\underbrace{\\left[\\left\\{\\frac{\\partial f}{\\partial x}\\right\\}\\cdot\\frac{1}{2}\\right]}{\\text{underbraced text \\hbar}}\\cdots\\frac{\\sqrt{\\sum_{i=0}^2 \\hat{i}^2}+y^\\alpha}{\\dot{v}\\equiv\\ddot{r}}, \\hat{t}\\hat{T} \\overbrace{\\left|\\sqrt{x\\cdot Y}\\right|}{\\propto\\bbN\\circ\\bbZ}   \\left<\\arrow{x(\\tau)}\\cdot\\vec{R}(t+\\bar{\\tau})\\right>   \\alpha\\beta\\gamma\\delta\\epsilon\\Gamma\\Delta\\Theta\\Omega  \\left\\_\\left~\\cbrt{\\hbar\\omega}\\right~\\right\\_$");
    ui->cmbTestset->addItem("upper/lower parantheses test:", "$\\text{bblabla} \\frac{1}{2}\\cdot\\left(\\frac{1}{\\mathrm{e}^x+\\mathrm{e}^{-x}}\\right)\\cdot\\left(\\frac{1}{\\frac{1+2}{5+x}}\\right)\\cdot\\left(\\frac{1}{\\exp\\left[-\\frac{y^2}{\\sqrt{x}}\\right]\\cdot\\exp\\left[-\\frac{1}{\\frac{1}{2}}\\right]}\\right) $");
    ui->cmbTestset->addItem("ACF test", "$g_{rg}^{ab}(\\tau)=\\frac{1}{N}\\cdot\\left(1+\\frac{2}{3}\\frac{\\langle r^2(\\tau)\\rangle}{w_{xy}^2}\\right)^{-1}\\cdot\\left(1+\\frac{2}{3}\\frac{\\langle r^2(\\tau)\\rangle}{w_{xy}^2}\\right)^{-\\frac{1}{2}}$");
    ui->cmbTestset->addItem("MSD test", "$\\mathrm{MSD}(\\tau)\\equiv\\langle r^2(\\tau)\\rangle=\\left\\langle (\\vec{r}(t)-\\vec{r}(t+\\tau) )^2\\right\\rangle=2n\\cdot\\frac{K_\\alpha}{\\Gamma(1+\\alpha)}\\cdot\\tau^\\alpha$");
    ui->cmbTestset->addItem("f() test", "$f(\\vec{x})=\\frac{1}{\\sqrt{2\\pi\\cdot\\sigma^2}}\\exp\\left(-\\frac{(\\vec{x}-\\vec{x}_0)^2}{\\sigma^2}\\right)$");
    ui->cmbTestset->addItem("chi^2 test", "$\\vec{p}^\\ast=\\argmax\\limits_{\\vec{p}}\\chi^2=\\argmax\\limits_{\\vec{p}}\\sum\\limits_{i=1}^N\\left|\\frac{\\hat{f}_i-f(x_i;\\vec{p})}{\\sigma_i}\\right|^2$");
    ui->cmbTestset->addItem("symbol test", "\\vdots\\cdots\\ddots\\iddots\\lfloor\\rfloor\\lceil\\rceil\\langle\\rangle\\sum\\int \\iint \\oint \\prod \\leftrightarrow \\leftarrow\\Leftarrow\\rightarrow\\Rightarrow\\pm\\mp\\leq\\geq\\ll\\gg\\hbar\\euro\\bbC\\bbH\\bbN\\bbP\\bbQ\\bbZ\\bbR\\Angstrom\\Alef\\Bet\\Gimel\\Dalet\\nexists\\ni\\notni\\circ\\tilde\\oiint\\oiiint\\emptyset\\odot\\ominus\\subsetnot\\DC\\bot\\cdots\\perthousand\\leftharpoonup\\rightharpoonup \\upharpoonleft \\downharpoonleft \\leftrightharpoon \\rightleftharpoon \\coprod \\leftharpoondown \\rightharpoondown \\nwarrow \\nearrow \\mapsto \\cent \\pound \\yen \\div \\multimap \\maporiginal \\mapimage \\bigcap \\bigcup \\benzene \\times \\cdot \\propto \\equiv \\Im \\Re \\ ");
    ui->cmbTestset->addItem("arrowtest 1", "$\\leftarrow \\longleftarrow \\Leftarrow \\Longleftarrow \\rightarrow \\longrightarrow \\Rightarrow \\Longrightarrow \\uparrow \\Uparrow \\downarrow \\Downarrow \\leftrightarrow \\Leftrightarrow \\longleftrightarrow \\Longleftrightarrow$");
    ui->cmbTestset->addItem("arrowtest 2", "$\\nwarrow \\nearrow \\searrow \\swarrow \\mapsto \\leftharpoonup \\rightharpoonup \\upharpoonleft \\downharpoonleft \\leftrightharpoon \\rightleftharpoon \\leftharpoondown \\rightharpoondown \\upharpoonright \\downharpoonright $");
    ui->cmbTestset->addItem("math: blackboard", "$\\mathbb{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math: bf", "$\\mathbf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math: it", "$\\mathit{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math: sf", "$\\mathsf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math: rm", "$\\mathrm{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math: cal", "$\\mathcal{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("math: tt", "$\\mathtt{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}$");
    ui->cmbTestset->addItem("bf", "\\textbf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("it", "\\textit{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("sf", "\\textsf{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("rm", "\\textrm{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("cal", "\\textcal{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("tt", "\\texttt{ABCDEFGHIJKLMNOPQRSTUVWXYZ120}");
    ui->cmbTestset->addItem("subscript test", "$r_{123}\\ \\ r_{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("subscript0 test", "$r_{123}$");
    ui->cmbTestset->addItem("subscript1 test", "$r_{123}\\ $");
    ui->cmbTestset->addItem("subscript2 test", "$r_{123}\\ \\ $");
    ui->cmbTestset->addItem("subscript3 test", "$r_{123}r_{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("superscript test", "$r^{123}\\ \\ r^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("superscript0 test", "$r^{123}$");
    ui->cmbTestset->addItem("superscript1 test", "$r^{123}\\ $");
    ui->cmbTestset->addItem("superscript2 test", "$r^{123}\\ \\ $");
    ui->cmbTestset->addItem("superscript3 test", "$r^{123}r^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("asuperscript test", "$a^{123}\\ \\ a^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("asuperscript0 test", "$a^{123}$");
    ui->cmbTestset->addItem("gsuperscript1 test", "$g^{123}\\ $");
    ui->cmbTestset->addItem("gsuperscript2 test", "$g^{123}\\ \\ $");
    ui->cmbTestset->addItem("gsuperscript3 test", "$g^{123}g^{\\frac{1}{2}}$");
    ui->cmbTestset->addItem("subsuperscript test", "$r_{123}^2$\\ \\ $r^2_{123}$\\ \\ $\\left(\\stackrel{a}{b}\\right)_{123}^2$\\ \\ $\\left(\\stackrel{a}{b}\\right)^2_{123}$\\ \\ $r_{\\frac{1}{2}}^2$\\ \\ $r^2_{\\frac{1}{2}}$\\ \\ $r^{\\frac{1}{2}}_2$\\ \\ $r_2^{\\frac{1}{2}}$\\ \\ $\\left(\\stackrel{a}{b}\\right)_2^{\\frac{1}{2}}$\\ \\ $\\left(\\stackrel{a}{b}\\right)^{\\frac{1}{2}}_2$");
    ui->cmbTestset->addItem("frac test", "$\\frac{a}{b}+\\frac{g}{a}-\\frac{a^2}{b^2}\\cdot\\frac{a^2}{b^{\\frac{1}{2}}}$");
    ui->cmbTestset->addItem("tfrac test", "$\\tfrac{a}{b}+\\tfrac{g}{a}-\\tfrac{a^2}{b^2}\\cdot\\tfrac{a^2}{b^{\\tfrac{1}{2}}}$");
    ui->cmbTestset->addItem("dfrac test", "$\\dfrac{a}{b}+\\dfrac{g}{a}-\\dfrac{a^2}{b^2}\\cdot\\dfrac{a^2}{b^{\\dfrac{1}{2}}}$");
    ui->cmbTestset->addItem("sfrac test", "$\\sfrac{a}{b}+\\sfrac{g}{a}-\\sfrac{a^2}{b^2}\\cdot\\sfrac{a^2}{b^{\\sfrac{1}{2}}}$");
    ui->cmbTestset->addItem("stfrac test", "$\\stfrac{a}{b}+\\stfrac{g}{a}-\\stfrac{a^2}{b^2}\\cdot\\stfrac{a^2}{b^{\\stfrac{1}{2}}}$");
    ui->cmbTestset->addItem("number fractions test", "frac: \\frac{1}{2}, \\frac{3}{4},\\ \\ \\ tfrac: \\tfrac{1}{2}, \\tfrac{3}{4},\\ \\ \\ sfrac: \\sfrac{1}{2}, \\sfrac{3}{4},\\ \\ \\ stfrac: \\stfrac{1}{2}, \\stfrac{3}{4},\\ \\ \\ ");
    ui->cmbTestset->addItem("stackrel test", "$\\stackrel{a}{b}+\\stackrel{g}{a}-\\stackrel{a^2}{b^2}\\cdot\\stackrel{a^2}{b^{\\stackrel{1}{2}}}$");
    ui->cmbTestset->addItem("brace0 test", "\\langle\\langle -- $\\langle\\langle$");
    ui->cmbTestset->addItem("brace1 test", "\\langle\\langle r^{123} -- $\\langle\\langle r^{123}$");
    ui->cmbTestset->addItem("brace2 test", "\\langle\\langle r^{123}\\rangle\\rangle -- $\\langle\\langle r^{123}\\rangle\\rangle$");
    ui->cmbTestset->addItem("brace3 test", "\\left\\langle r^{123}\\right\\rangle -- $\\left\\langle r^{123}\\right\\rangle$");
    ui->cmbTestset->addItem("brace4 test", "\\left\\langle\\left\\langle\\left\\langle r^{123}\\right\\rangle\\right\\rangle\\right\\rangle -- $\\left\\langle\\left\\langle\\left\\langle r^{123}\\right\\rangle\\right\\rangle\\right\\rangle$");
    ui->cmbTestset->addItem("brace5 test:  ( )", "\\left(\\left(\\left( r^{123}\\right)\\right)\\right) -- $\\left(\\left(\\left( r^{123}\\right)\\right)\\right)$");
    ui->cmbTestset->addItem("brace6 test:  [ ]", "\\left[\\left[\\left[ r^{123}\\right]\\right]\\right] -- $\\left[\\left[\\left[ r^{123}\\right]\\right]\\right]$");
    ui->cmbTestset->addItem("brace7 test:  { }", "\\left\\{\\left\\{\\left\\{ r^{123}\\right\\}\\right\\}\\right\\} -- $\\left\\{\\left\\{\\left\\{ r^{123}\\right\\}\\right\\}\\right\\}$");
    ui->cmbTestset->addItem("brace8 test: || ||", "\\left\\|\\left\\|\\left\\| r^{123}\\right\\|\\right\\|\\right\\| -- $\\left\\|\\left\\|\\left\\| r^{123}\\right\\|\\right\\|\\right\\|$");
    ui->cmbTestset->addItem("brace9 test:  | |", "\\left|\\left|\\left| r^{123}\\right|\\right|\\right| -- $\\left|\\left|\\left| r^{123}\\right|\\right|\\right|$");
    ui->cmbTestset->addItem("brace10 test", "\\left\\{\\left[\\left( r^{123}\\right)\\right]\\right\\} -- $\\left\\{\\left[\\left( r^{123}\\right)\\right]\\right\\}$");
    ui->cmbTestset->addItem("brace11 test:  floor", "\\left\\lfloor\\left\\lfloor\\left\\lfloor r^{123}\\right\\rfloor\\right\\rfloor\\right\\rfloor -- $\\left\\lfloor\\left\\lfloor\\left\\lfloor r^{123}\\right\\rfloor\\right\\rfloor\\right\\rfloor$");
    ui->cmbTestset->addItem("brace12 test:  ceil", "\\left\\lceil\\left\\lceil\\left\\lceil r^{123}\\right\\rceil\\right\\rceil\\right\\rceil -- $\\left\\lceil\\left\\lceil\\left\\lceil r^{123}\\right\\rceil\\right\\rceil\\right\\rceil$");
    ui->cmbTestset->addItem("sub-, superscript test", "r^{1234}_{321} r_{321}^{1234} -- $r^{1234}_{321} r_{321}^{1234} -- \\kappa^2 -- \\kappa_2 -- \\kappa_2^2$");
    ui->cmbTestset->addItem("super-, subscript test", "r^{123}_{4321} r_{4321}^{123} -- $r^{123}_{4321} r_{4321}^{123} -- \\kappa^2 -- \\kappa_2 -- \\kappa_2^2$");
    //ui->cmbTestset->addItem("", "");
    ui->cmbTestset->addItem("math 1", "$f(x)=\\int_{-\\infty}^xe^{-t^2}\\;\\mathrm{d}t$");
    ui->cmbTestset->addItem("math 2", "$\\sum_{i=1}^\\infty\\frac{-e^{i\\pi}}{2^n}$");
    ui->cmbTestset->addItem("math 3", "$\\mbox{det} \\begin{pmatrix} 1 & x_1 & \\ldots & x_1^{n-1} \\\\ 1 & x_2 & \\ldots & x_2^{n-1} \\\\ \\vdots & \\vdots & \\ddots & \\vdots \\\\ 1 & x_n & \\ldots & x_n^{n-1} \\end{pmatrix} = \\prod_{1 \\leq i < j \\leq n} (x_j - x_i) $");
    ui->cmbTestset->addItem("math 4", "$\\sqrt{1+\\sqrt{1+\\sqrt{1+\\sqrt{1+\\sqrt{1+\\sqrt{1+x}}}}}}$");
    ui->cmbTestset->addItem("math 5", "$\\left(\\stackrel{p}{2}\\right)=x^2y^{p-2}-\\frac{1}{1-x}\\frac{1}{1-x^2}$");
    ui->cmbTestset->addItem("math 6", "$a_0+\\frac{1}{a_1+\\frac{1}{a_2+\\frac{1}{a_3+\\frac{1}{a_4}}}}$");
    ui->cmbTestset->addItem("math 7", "$\\left(\\frac{\\partial^2}{\\partial x^2}+\\frac{\\partial^2}{\\partial y^2}\\right)\\left|\\varphi(x+\\mathrm{i}y)\\right|^2=0$");
    ui->cmbTestset->addItem("math 8", "$2^{2^{2^{x}}}$");
    ui->cmbTestset->addItem("math 9", "$\\iint_Df(x,y)\\;\\mathrm{d}x\\;\\mathrm{d}y$");
    ui->cmbTestset->addItem("math 10 (overbrace)", "$\\overbrace{x+x+...+x}{k\\ \\mathrm{times}}$");
    ui->cmbTestset->addItem("math 11 (underbrace)", "$\\underbrace{x+x+...+x}{k\\ \\mathrm{times}}$");
    ui->cmbTestset->addItem("math 12 (under/overbrace)", "$\\underbrace{\\overbrace{x+x+...+x}{k\\ \\mathrm{times}} \\overbrace{x+x+...+x}{k\\ \\mathrm{times}}}{2k\\ \\mathrm{times}}$");
    ui->cmbTestset->addItem("math 13", "$y_1''\\ \\ \\ y_2'''$");
    ui->cmbTestset->addItem("math 14", "$f(x)=\\begin{cases} 1/3 & \\mathrm{if}\\ 0\\leq x\\leq1 \\\\ 2/3 & \\mathrm{if}\\ 3\\leq x\\leq4 \\\\0 & \\mathrm{elsewhere} \\end{cases}$");
    ui->cmbTestset->addItem("math 15", "$\\Re{z} =\\frac{n\\pi \\dfrac{\\theta +\\psi}{2}}{\\left(\\dfrac{\\theta +\\psi}{2}\\right)^2 + \\left( \\dfrac{1}{2}\\log \\left\\lvert\\dfrac{B}{A}\\right\\rvert\\right)^2}.$");
    ui->cmbTestset->addItem("math 16", "$\\sum_{m=1}^\\infty\\sum_{n=1}^\\infty\\frac{m^2\\,n}{3^m\\left(m\\,3^n+n\\,3^m\\right)}$");
    ui->cmbTestset->addItem("math 17", "$\\phi_n(\\kappa) =\\frac{1}{4\\pi^2\\kappa^2} \\int_0^\\infty\\frac{\\sin(\\kappa R)}{\\kappa R}\\frac{\\partial}{\\partial R}\\left[R^2\\frac{\\partial D_n(R)}{\\partial R}\\right]\\,dR$");
    ui->cmbTestset->addItem("math 18", "${}_pF_q(a_1,\\dots,a_p;c_1,\\dots,c_q;z)= \\sum_{n=0}^\\infty\\frac{(a_1)_n\\cdots(a_p)_n}{(c_1)_n\\cdots(c_q)_n}\\frac{z^n}{n!}$");
    ui->cmbTestset->addItem("math 19 (overset)", "$X \\overset{=}{def} Y$\\ \\ \\ \\ \\ $X \\overset{=}{!} Y$\\ \\ \\ \\ \\ $X \\overset{\\rightarrow}{f} Y$\\ \\ \\ \\ \\ $\\frac{f(x+\\Delta x)-f(x)}{\\Delta x}\\overset{\\longrightarrow}{\\Delta x\\to 0}f'(x)$");
    ui->cmbTestset->addItem("math 20 (underset)", "$X \\underset{=}{\\text{def (5)}} Y$\\ \\ \\ \\ \\ $X \\underset{\\rightarrow}{f} Y$\\ \\ \\ \\ \\ $\\frac{f(x+\\Delta x)-f(x)}{\\Delta x}\\underset{\\longrightarrow}{\\Delta x\\to 0}f'(x)$");
    ui->cmbTestset->addItem("Jacobi Matrix and VarCov matrix", "$J_{ij}= \\left.\\frac{\\partial f(x,\\vec{p})}{\\partial p_i}\\right|_{\\vec{p},x=x_j}\\ \\ \\ \\ \\ \\mat{C}=\\left(\\mat{J}^\\mathrm{T}\\ J\\right)^{-1}\\ \\ \\ \\ \\ test: \\left|\\frac{\\partial f(x,\\vec{p})}{\\partial p_i}\\right|_{\\vec{p},x=x_j}^2$");
    ui->cmbTestset->addItem("operator test (textmode)", "x=0\\ \\ y>0\\ \\ x+y\\ \\ -1\\ \\ x-2\\ \\ x\\cdot y\\ \\ x\\geq 4\\ \\ x~4");
    ui->cmbTestset->addItem("operator test (mathmode)", "$x=0\\ \\ y>0\\ \\ x+y\\ \\ -1\\ \\ x-2\\ \\ x\\cdot y\\ \\ x\\geq 4\\ \\ x~4$");
    ui->cmbTestset->addItem("color test", "\\textcolor{red}{RED}\\textcolor{blue}{BLUE}");
    ui->cmbTestset->addItem("boxed test", "test: \\boxed{boxed text} in the middle");
    ui->cmbTestset->addItem("mathboxed test", "\\fbox{2^{2^{\\colorbox{red}{2^{x}}}}}");
    ui->cmbTestset->addItem("axiom of power test", "$\\forall A \\, \\exists P \\, \\forall B \\, [B \\in P \\iff \\forall C \\, (C \\in B \\Rightarrow C \\in A)]$");
    ui->cmbTestset->addItem("De Morgan's law", "$\\neg(P\\land Q)\\iff(\\neg P)\\lor(\\neg Q)$ or $\\overline{\\bigcap_{i \\in I} A_{i}}\\equiv\\bigcup_{i \\in I} \\overline{A_{i}}$ or $\\overline{A \\cup B}\\equiv\\overline{A} \\cap \\overline{B}$");
    ui->cmbTestset->addItem("quadratic formula", "$x=\\frac{-b \\pm \\sqrt{b^2-4ac}}{2a}$");
    ui->cmbTestset->addItem("combination", "$\\binom{n}{k} = \\frac{n(n-1)...(n-k+1)}{k(k-1)\\dots1}=\\frac{n!}{k!(n-k)!}$");
    ui->cmbTestset->addItem("Sophomore's dream 1", "$\\int_0^1 x^{-x}\\,dx = \\sum_{n=1}^\\infty n^{-n}(\\scriptstyle{= 1.29128599706266354040728259059560054149861936827\\dots)}$");
    ui->cmbTestset->addItem("Sophomore's dream 2", "$\\int_0^1 x^x   \\,dx = \\sum_{n=1}^\\infty (-1)^{n+1}n^{-n} = - \\sum_{n=1}^\\infty (-n)^{-n} (\\scriptstyle{= 0.78343051071213440705926438652697546940768199014\\dots})$");
    ui->cmbTestset->addItem("divergence 1", "$\\operatorname{div}\\vec{F} = \\nabla\\cdot\\vec{F}=\\frac{\\partial U}{\\partial x}+\\frac{\\partial V}{\\partial y}+\\frac{\\partial W}{\\partial z}$");
    ui->cmbTestset->addItem("divergence 2", "$\\overrightarrow{\\operatorname{div}}\\,(\\mathbf{\\underline{\\underline{\\epsilon}}}) = "
                            "\\begin{bmatrix}"
                            "\\frac{\\partial \\epsilon_{xx}}{\\partial x} +\\frac{\\partial \\epsilon_{yx}}{\\partial y} +\\frac{\\partial \\epsilon_{zx}}{\\partial z} \\\\"
                            "\\frac{\\partial \\epsilon_{xy}}{\\partial x} +\\frac{\\partial \\epsilon_{yy}}{\\partial y} +\\frac{\\partial \\epsilon_{zy}}{\\partial z} \\\\"
                            "\\frac{\\partial \\epsilon_{xz}}{\\partial x} +\\frac{\\partial \\epsilon_{yz}}{\\partial y} +\\frac{\\partial \\epsilon_{zz}}{\\partial z}"
                            "\\end{bmatrix}$");
    ui->cmbTestset->addItem("lim, sum ...", "$\\lim_{x\\to\\infty} f(x) = \\binom{k}{r} + \\frac{a}{b} \\sum_{n=1}^\\infty a_n + \\displaystyle{ \\left\\{ \\frac{1}{13} \\sum_{n=1}^\\infty b_n \\right\\} }.$");
    ui->cmbTestset->addItem("array test", "$f(x) := \\left\\{\\begin{array} x^2 \\sin \\frac{1}{x} & \\textrm{if } x \\ne 0, \\\\ 0 & \\textrm{if } x = 0 . \\end{array}\\right.$");
    ui->cmbTestset->addItem("Schwinger-Dyson", "$\\left\\langle\\psi\\left|\\mathcal{T}\\{F \\phi^j\\}\\right|\\psi\\right\\rangle=\\left\\langle\\psi\\left|\\mathcal{T}\\{iF_{,i}D^{ij}-FS_{int,i}D^{ij}\\}\\right|\\psi\\right\\rangle.$");
    ui->cmbTestset->addItem(QLatin1String("Schrödinger's equation"), "$\\left[-\\frac{\\hbar^2}{2m}\\frac{\\partial^2}{\\partial x^2}+V\\right]\\Psi(x)=\\mathrm{i}\\hbar\\frac{\\partial}{\\partial t}\\Psi(x)$");
    ui->cmbTestset->addItem("Cauchy-Schwarz inequality", "$\\left( \\sum_{k=1}^n a_k b_k \\right)^2 \\leq \\left( \\sum_{k=1}^n a_k^2 \\right) \\left( \\sum_{k=1}^n b_k^2 \\right)$");
    ui->cmbTestset->addItem("Maxwell's equations", "$\\begin{aligned}\\nabla \\times \\vec{\\mathbf{B}} -\\, \\frac{1}{c}\\, \\frac{\\partial\\vec{\\mathbf{E}}}{\\partial t} & = \\frac{4\\pi}{c}\\vec{\\mathbf{j}} \\\\   \\nabla \\cdot \\vec{\\mathbf{E}} & = 4 \\pi \\rho \\\\\\nabla \\times \\vec{\\mathbf{E}}\\, +\\, \\frac{1}{c}\\, \\frac{\\partial\\vec{\\mathbf{B}}}{\\partial t} & = \\vec{\\mathbf{0}} \\\\\\nabla \\cdot \\vec{\\mathbf{B}} & = 0 \\end{aligned}$");
    ui->cmbTestset->addItem("Langevin Equation", "$m \\dot{v}(t) = -\\gamma v(t) + F(x,t)+ f(t)$");
    ui->cmbTestset->addItem("Fokker-Planck Equation", "$\\frac{\\partial}{\\partial t}P(y,t)=-\\frac{\\partial}{\\partial y}\\left[ A(y,t)P(y,t)\\right] +\\frac{\\Gamma}{2}\\frac{\\partial^2}{\\partial y^2}\\left[ P(y,t)\\right]$");
    ui->cmbTestset->addItem("Hamilton Equations of motion", "$\\mathcal{H}(\\mathbf{q},\\mathbf{p})=\\frac{\\mathbf{p}^2}{2\\,m}+V(\\mathbf{q})\\ \\ \\ \\text{and}\\ \\ \\ \\dot{q}_k =\\frac{p_k}{m}\\ ,\\ \\dot{p}_k = - \\frac{\\partial V}{\\partial q_k}$");
    ui->cmbTestset->addItem("Gaussian Distrubution", "$f(x | \\mu,\\sigma^2)=\\frac{1}{\\sqrt{2\\pi\\sigma^2}}\\operatorname{exp}\\left(-\\frac{(x-\\mu)^2}{2\\sigma^2}\\right)=\\frac{1}{\\sqrt{2\\pi\\sigma^2}} e^{-\\frac{(x-\\mu)^2}{2\\sigma^2}}\\quad -\\infty<x<\\infty$");
    ui->cmbTestset->addItem("User-Editable Text");
    //
    //ui->cmbTestset->addItem("", "$$");
    //ui->cmbTestset->addItem("", "$$");
    //ui->cmbTestset->addItem("", "");
    ui->cmbTestset->setCurrentIndex(0);

    ui->labMath->setMath("\\left(\\left[\\sqrt{2\\pi\\cdot\\int_{-\\infty}^\\infty f(x)\\;\\mathrm{d}x}\\right]\\right)");
    ui->cmbFont->setCurrentIndex(1);
}

TestForm::~TestForm()
{
    delete ui;
}

#define W 2000
#define H 2000
#define X1 15
#define Y1 50



double TestForm::draw(QPainter& painter, double X, double YY, JKQTMathText& mt, QString name, double& durationSizingMS, double&durationTimingMS) {


    double Y=YY;
    painter.save();
    ht.start();
    QSizeF s=mt.getSize(painter);
    Y=Y+mt.getAscent(painter);
    durationSizingMS=ht.getTime()/1000.0;
    qDebug()<<"    sizing in "<<durationSizingMS<<" ms\n";
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
    mt.draw(painter, X, Y, ui->chkBoxes->isChecked());
    durationTimingMS=ht.getTime()/1000.0;
    qDebug()<<"   drawing in "<<durationTimingMS<<" ms";
    p.setColor("blue");
    painter.setPen(p);

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

QTreeWidgetItem *TestForm::createTree(JKQTMathText::MTnode *node, QTreeWidgetItem* parent)
{
    QString name;
    JKQTMathText::MTtextNode* txtN=dynamic_cast<JKQTMathText::MTtextNode*>(node);
    JKQTMathText::MTwhitespaceNode* spN=dynamic_cast<JKQTMathText::MTwhitespaceNode*>(node);
    JKQTMathText::MTsymbolNode* symN=dynamic_cast<JKQTMathText::MTsymbolNode*>(node);
    JKQTMathText::MTlistNode* lstN=dynamic_cast<JKQTMathText::MTlistNode*>(node);
    JKQTMathText::MTinstruction1Node* inst1N=dynamic_cast<JKQTMathText::MTinstruction1Node*>(node);
    JKQTMathText::MTsubscriptNode* subN=dynamic_cast<JKQTMathText::MTsubscriptNode*>(node);
    JKQTMathText::MTsuperscriptNode* superN=dynamic_cast<JKQTMathText::MTsuperscriptNode*>(node);
    JKQTMathText::MTbraceNode* braceN=dynamic_cast<JKQTMathText::MTbraceNode*>(node);
    JKQTMathText::MTsqrtNode* sqrtN=dynamic_cast<JKQTMathText::MTsqrtNode*>(node);
    JKQTMathText::MTfracNode* fracN=dynamic_cast<JKQTMathText::MTfracNode*>(node);
    JKQTMathText::MTmatrixNode* matrixN=dynamic_cast<JKQTMathText::MTmatrixNode*>(node);
    JKQTMathText::MTdecoratedNode* decoN=dynamic_cast<JKQTMathText::MTdecoratedNode*>(node);

    QTreeWidgetItem* ti=nullptr;
    if (parent) ti=new QTreeWidgetItem(parent);
    else ti=new QTreeWidgetItem(ui->tree);

    if (decoN) {
        name=QString("MTdecoratedNode: mode='%1'").arg(JKQTMathText::decorationToString(decoN->getDecoration()));
        if (decoN->getChild()) ti->addChild(createTree(decoN->getChild(), ti));
    } else if (matrixN)  {
        int l=matrixN->getLines();
        int c=matrixN->getColumns();
        name=QString("MTmatrixNode: l*c=%1*%2").arg(l).arg(c);
        QVector<QVector<JKQTMathText::MTnode*> > children=matrixN->getChildren();
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
        name=QString("MTfracNode: mode='%1'").arg(JKQTMathText::fracModeToString(fracN->getMode()));
        if (fracN->getChild1()) ti->addChild(createTree(fracN->getChild1(), ti));
        if (fracN->getChild2()) ti->addChild(createTree(fracN->getChild2(), ti));
    } else if (sqrtN)  {
        name=QString("MTsqrtNode: deg=%1").arg(sqrtN->getDegree());
        if (sqrtN->getChild()) ti->addChild(createTree(sqrtN->getChild(), ti));
    } else if (braceN)  {
        name=QString("MTbraceNode: l='%1', r='%2', showR=%3").arg(braceN->getOpenbrace()).arg(braceN->getClosebrace()).arg(braceN->getShowRightBrace());
        if (braceN->getChild()) ti->addChild(createTree(braceN->getChild(), ti));
    } else if (superN)  {
        name=QString("MTsuperscriptNode");
        if (superN->getChild()) ti->addChild(createTree(superN->getChild(), ti));
    } else if (subN)  {
        name=QString("MTsubscriptNode");
        if (subN->getChild()) ti->addChild(createTree(subN->getChild(), ti));
    } else if (inst1N)  {
        name=QString("MTinstruction1Node: \'%1\'").arg(inst1N->getName());
        if (inst1N->getChild()) ti->addChild(createTree(inst1N->getChild(), ti));
    } else if (lstN)  {
        name=QString("MTlistNode");
        QList<JKQTMathText::MTnode*> list=lstN->getNodes();
        for (int i=0; i<list.size(); i++) {
            ti->addChild(createTree(list[i], ti));
        }
    } else if (symN)  {
        name=QString("MTsymbolNode: \'%1\' (%2)").arg(symN->getSymbolName()).arg(symN->getSymbolFontName());
    } else if (spN)  {
        name=QString("MTwhitespaceNode :\'%1\'").arg(txtN->getText());
    } else if (txtN)  {
        name=QString("MTtextNode: \'%1\'").arg(txtN->getText());

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
    //QString mathTest="$\\left\\|\\stackrel{1}{2}\\right\\|\\cdot\\left|\\begin{Bmatrix}1123&2^2&\\frac{3}{4}\\\\4&5_5&6\\\\72_8&8_{\\text{längerer Index}}&9&10&11\\end{Bmatrix}\\right|\\cdots f(x)$";
    //QString mathTest="0";
    //QString text="x_0 \\sqrt{a^2+b^2}\\underbrace{underbrace}{x_0} (\\bf{Test})\\_\\it{Text} ... ! Äquator\\alpha\\beta $f(x)=\\sin(x{\\cdot}y)$ \\ul{Text\\ol{\\gamma}} \\script{Script}, \\tt{Typewriter}, \\sf{sans serif}";
    //QString text="$\\sum_{i=0_{j=-1_{k=-45}}}^{n^{m^{123456789}}}\\left(\\left(\\left(\\int_0^\\inf\\left[r+i^{\\left\\{2^2\\right\\}}\\right]{\\dd}r\\right)\\right)\\right)$ \\bf{Test}\\_\\it{Text} ... ! \\sc{&Äquator \\&}\\alpha\\beta $f(x)=\\sin(x\\cdot\\cos(a+{\\ii}b))$ \\ul{Text\\gamma} \\script{Script-Text}\\tt{ and Typewriter} \\sf{also sans serif}";
    //QString text="$\\sum_{i=0_{j=-1_{k=-45}}}^2\\left(\\left(\\left(\\int_0^\\inf\\left[r+i^{\\left\\{2^2\\right\\}}\\right]{\\dd}r\\right)\\right)\\right)$ \\bf{Test}\\_\\it{Text} ... ! \\sc{&Äquator \\&}\\alpha\\beta $f(x)=\\sin(x\\cdot\\cos(a+{\\ii}b))$ \\ul{Text\\gamma} \\script{Script-Text}\\tt{ and Typewriter} \\sf{also sans serif}";

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

    QPixmap pix(W, H);
    pix.fill();
    QPainter painter;
    JKQTMathText mt(this);


    double Y=Y1;

    painter.begin(&pix);
    if (ui->chkAntiAlias->isChecked()) painter.setRenderHint(QPainter::Antialiasing);
    if (ui->chkAntiAliasHQ->isChecked()) painter.setRenderHint(QPainter::HighQualityAntialiasing);
    if (ui->chkAntiAliasText->isChecked()) painter.setRenderHint(QPainter::TextAntialiasing);
    if (ui->chkSmoothTransform->isChecked()) painter.setRenderHint(QPainter::QPainter::SmoothPixmapTransform);
    ht.start();



    mt.setFontRoman(ui->cmbUnicodeSerif->currentFont().family());
    mt.setFontSans(ui->cmbUnicodeSans->currentFont().family());
    mt.setFontTypewriter(ui->cmbUnicodeFixed->currentFont().family());
    mt.setFontSymbol(ui->cmbUnicodeSymbol->currentFont().family());
    mt.setFontCaligraphic(ui->cmbCaligraphic->currentFont().family());

    switch (ui->cmbFont->currentIndex()) {
        case 1: mt.useXITS(); break;
        case 2: mt.useSTIX(); break;
        case 3: mt.useAnyUnicode(ui->cmbUnicodeSerif->currentFont().family(),ui->cmbUnicodeSans->currentFont().family(), false); break;
        case 4: mt.useAnyUnicode(ui->cmbUnicodeSerif->currentFont().family(),ui->cmbUnicodeSans->currentFont().family(), true); break;
        case 5: mt.useLatexFonts(ui->edtLatexPrefix->text(), ui->edtLatexPostfix->text()); break;
        case 6: mt.useASANA(); break;

    }

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

    for (int i=0; i<sl.size(); i++) {
        bool ok=true;
        int size=sl[i].trimmed().toUInt(&ok);
        if (!ok) size=10+i*5;
        mt.setFontSize(size);
        double durationSizingMS=0, durationTimingMS=0;
        Y+=draw(painter, X1, Y, mt, QString("%1, %2, %3pt").arg(ui->cmbTestset->currentText()).arg(ui->cmbFont->currentText()).arg(size), durationSizingMS, durationTimingMS);

        if (i==0) {
            if (mt.getErrorList().size()>0) {
                ui->labError->setText("<span color=\"red\">"+mt.getErrorList().join("<br>")+"</span>");
            } else {
                ui->labError->setText("<span color=\"green\">OK</span>");
            }
        }

        ui->labRenderTimes->setText(ui->labRenderTimes->text()+QString("     %1pt: %2ms/%3ms").arg(size).arg(durationSizingMS, 0, 'F', 1).arg(durationTimingMS, 0, 'F', 1));
        ui->textBrowser->textCursor().insertHtml("<hr>"+mt.toHtml(&okh)+"<hr><br><br>");
        qDebug()<<"HTML: ---------------------------------------------\n"<<mt.toHtml(&okh)<<"\nHTML: --------------------------------------------- ok="<<okh;
        if (mt.getErrorList().size()>0) {
            qDebug()<<mt.getErrorList().join("\n")<<"\n";
        }
    }

    painter.end();


    ui->label->setPixmap(pix);
    QApplication::restoreOverrideCursor();
}
