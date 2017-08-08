<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:fn="http://www.w3.org/2005/xpath-functions" version="1.0">
  <xsl:output method="text" indent="no"/>
<xsl:strip-space elements="*"/>
<xsl:preserve-space elements="code"/>

<xsl:template match="text()" mode="verbatim">
<xsl:value-of select="." />
</xsl:template>

<xsl:template match="text()"><xsl:text>&#x7f;</xsl:text><xsl:value-of select="."/><xsl:text>&#x7f;</xsl:text></xsl:template>

<xsl:template match="titlepic">
\includegraphics[width=0.8\textwidth]{<xsl:value-of select="includegraphics[last()]/@src"/>}
</xsl:template>

<xsl:template match="title">\title{<xsl:value-of select="node()"/>}
</xsl:template>
<xsl:template match="subtitle">\subtitle{<xsl:apply-templates select="node()"/>}
</xsl:template>
<xsl:template match="author">\author{<xsl:value-of select="node()"/>}
</xsl:template>

<xsl:template match="abstract">
\chapter*{Abstract}
\thispagestyle{empty}
<xsl:apply-templates select="@*|node()"/>
</xsl:template>

<xsl:template match="chapter-star">
<xsl:choose>
<xsl:when test="text()='Acknowledgements'">
\chapter*{<xsl:apply-templates select="node()"/>}
\thispagestyle{empty}
\pagenumbering{roman}
\setcounter{page}{0}
</xsl:when>
<xsl:otherwise>
\makeatletter\@openrightfalse\makeatother
\chapter*{<xsl:apply-templates select="node()"/>}
\addcontentsline{toc}{chapter}{<xsl:apply-templates select="node()"/>}
</xsl:otherwise>
</xsl:choose>
</xsl:template>

<xsl:template match="ul">\begin{itemize}\setlength{\itemsep}{1ex}
    \setlength{\parskip}{0pt}
    \setlength{\parsep}{0pt}
<xsl:apply-templates select="node()"/>\end{itemize}
</xsl:template>

<xsl:template match="ul[@class='deps']">\begin{multicols}{3}
\begin{itemize}\setlength{\itemsep}{1ex}
    \setlength{\parskip}{0pt}
    \setlength{\parsep}{0pt}
<xsl:apply-templates select="node()"/>\end{itemize}
\end{multicols}
</xsl:template>

<xsl:template match="p"><xsl:apply-templates select="node()"/><xsl:text>

</xsl:text></xsl:template>

<xsl:template match="li">  \item{<xsl:apply-templates select="node()"/>}
</xsl:template>

<xsl:template match="tableofcontents">\tableofcontents</xsl:template>

<xsl:template match="col" mode="header">\textbf{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="col" mode="normal"><xsl:apply-templates select="node()"/></xsl:template>


<xsl:template match="colheaders"><xsl:for-each select="col"><xsl:if test="position()>1"> &amp; </xsl:if><xsl:apply-templates select="." mode="header"/>
</xsl:for-each> \\
\hline
\endhead
</xsl:template>

<xsl:template match="hline">\hline</xsl:template>

<xsl:template match="row"><xsl:for-each select="col"><xsl:if test="position()>1"> &amp; </xsl:if><xsl:apply-templates select="." mode="normal"/>
</xsl:for-each>\\

</xsl:template>

<xsl:template match="tabular">\begin{center}
\begin{tabularx}{\textwidth}{<xsl:for-each select="./colheaders/col"><xsl:choose><xsl:when test="position()=last()">X</xsl:when><xsl:otherwise>l</xsl:otherwise></xsl:choose></xsl:for-each>}
\hline
\hline
<xsl:apply-templates select="node()"/>\hline
\hline
\end{tabularx}
\end{center}
</xsl:template>

<xsl:template match="chapter">
\chapter{<xsl:value-of select="node()"/>}
\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="//chapter[1]">
\clearpage
\mbox{}
\thispagestyle{empty}
\chapter{<xsl:value-of select="node()"/>}
\makeatletter\@openrighttrue\makeatother
\label{<xsl:value-of select="@id"/>}
\setcounter{page}{1}
\pagenumbering{arabic}
</xsl:template>

<xsl:template match="section">

\section{<xsl:apply-templates select="node()"/>}
\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="subsection">

\subsection{<xsl:apply-templates select="node()"/>}
\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="subsubsection">

\subsubsection{<xsl:apply-templates select="node()"/>}
\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="strong">\textbf{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="cite">\cite{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="prgname|libname|dirname">\texttt{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="ref">\cref{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="code">\begin{verbatim}
<xsl:apply-templates select="node()" mode="verbatim"/>\end{verbatim}

</xsl:template>

<xsl:template match="kbd">\kbd{<xsl:apply-templates select="node()" />}</xsl:template>
<xsl:template match="ui-button">\uibutton{<xsl:apply-templates select="node()" />}</xsl:template>
<xsl:template match="ui-formfield">\uiformfield{<xsl:apply-templates select="node()" />}</xsl:template>
<xsl:template match="dfn">\emph{<xsl:apply-templates select="node()" />}</xsl:template>
<xsl:template match="char">\charwithcodepoint{<xsl:apply-templates select="node()" />}</xsl:template>

<xsl:template match="includegraphics">\includegraphics[height=1.5ex]{<xsl:value-of select="@src"/><xsl:if test="substring-after(@src,'.')='svg'">.pdf</xsl:if>}</xsl:template>

<xsl:template match="col/includegraphics">\multicolumn{1}{c}{\includegraphics[height=2ex]{<xsl:value-of select="@src"/><xsl:if test="substring-after(@src,'.')='svg'">.pdf</xsl:if>}}</xsl:template>

<xsl:template match="quantity">\SI{<xsl:value-of select="node()"/>}{<xsl:value-of select="@unit"/>}</xsl:template>

<xsl:template match="infobox">
\begin{centering}
\begin{tcolorbox}[width=0.8\textwidth,colback=<xsl:value-of select="@type"/>back,colframe=<xsl:value-of select="@type"/>border,sidebyside,sidebyside align=top seam,lefthand width=8ex,lower separated=false,sidebyside gap=0.0cm,left=0cm,right=0cm,top=0cm,bottom=0cm,boxsep=0.2cm]
\includegraphics[height=6ex]{message_<xsl:value-of select="@type"/>.svg.pdf}
\tcblower
<xsl:apply-templates select="node()" />
\end{tcolorbox}
\end{centering}
</xsl:template>

<xsl:template match="figure">
\begin{figure}
\centering
\includegraphics[max height=0.3\textheight,max width=\textwidth]{<xsl:value-of select="includegraphics/@src"/><xsl:if test="substring-after(includegraphics/@src,'.')='svg'">.pdf</xsl:if>}
\caption{\label{<xsl:value-of select="@id"/>}<xsl:apply-templates select="caption" />}
\end{figure}
</xsl:template>

<xsl:template match="url">\url{<xsl:value-of select="node()"/>}</xsl:template>

<xsl:template match="bibliography/item">\bibitem{<xsl:value-of select="@id"/>}<xsl:apply-templates select="node()"/><xsl:text>

</xsl:text>
</xsl:template>

<xsl:template match="bibliography">
\makeatletter\@openrightfalse\makeatother
\begin{thebibliography}{9}
<xsl:apply-templates select="node()"/>\end{thebibliography}
</xsl:template>

<xsl:template match="appendix">
\appendix</xsl:template>

<xsl:template match="herefigure">
\begin{center}
\centering
\includegraphics[max height=0.3\textheight,max width=\textwidth]{<xsl:value-of select="includegraphics/@src"/>}
\end{center}
</xsl:template>

<xsl:template match="cmdoption">\texttt{<xsl:apply-templates select="node()" />}</xsl:template>

<xsl:template match="em">\emph{<xsl:apply-templates select="node()" />}</xsl:template>

<xsl:template match="sub">\textsubscript{<xsl:apply-templates select="node()"/>}</xsl:template>
<xsl:template match="var">\textit{<xsl:apply-templates select="node()"/>}</xsl:template>
<xsl:template match="env">\texttt{<xsl:apply-templates select="node()"/>}</xsl:template>
<xsl:template match="function">\texttt{<xsl:apply-templates select="node()"/>}</xsl:template>
<xsl:template match="class">\texttt{<xsl:apply-templates select="node()"/>}</xsl:template>


<xsl:template match="/">\documentclass[a4paper,twoside,12pt,BCOR=10mm]{scrbook}
\usepackage[english]{babel}
\usepackage{fontspec}
\setmainfont{Latin Modern Roman}
\newfontfamily{\defaultfont}{Latin Modern Roman}
\newfontfamily{\mathfont}{Latin Modern Math}
\newfontfamily\arrowfont{DejaVu Sans}
\newfontfamily{\uifont}{Latin Modern Sans}

\usepackage[Latin,MathematicalOperators,Arrows,Dingbats]{ucharclasses}
\setTransitionFrom{MathematicalOperators}{\defaultfont}
\setTransitionTo{MathematicalOperators}{\mathfont}
\setTransitionTo{Arrows}{\arrowfont}
\setTransitionTo{Dingbats}{\arrowfont}
\DeclareTextFontCommand{\textuiimpl}{\uifont}
\newcommand{\textui}[1]{\textuiimpl{#1}}

\usepackage{verbatim}
\usepackage[export]{adjustbox} % loads also graphicx
\usepackage{color}
\usepackage{xcolor}
\usepackage[multidot]{grffile}



\usepackage{varioref}
\usepackage[hidelinks,
	pdfauthor={<xsl:value-of select="node()/author"/>},
    pdftitle={<xsl:value-of select="node()/title"/>},
    pdfkeywords={<xsl:value-of select="node()/keywords"/>}]{hyperref}

\usepackage{cleveref}
\usepackage{siunitx}
\usepackage{tcolorbox}
\usepackage{multicol}
\usepackage{tabularx}
\usepackage{booktabs}
\usepackage{ltablex}

\definecolor{lightgray}{gray}{0.9}
\definecolor{warningborder}{HTML}{FFC000}
\definecolor{infoborder}{HTML}{0000FF}
\definecolor{warningback}{HTML}{FFF0A0}
\definecolor{infoback}{HTML}{D0D0FF}

\newcommand{\kbd}[1]{\colorbox{black}{\textcolor{white}{{\texttt{#1}}}} }
\newcommand{\uibutton}[1]{ \colorbox{lightgray}{ \textui{#1} } }
\newcommand{\uiformfield}[1]{ \colorbox{lightgray}{ \textui{ {\textit{#1} } }  } }

\makeatletter
\def\charwithcodepointb#1{%
\texttt{#1}
(%
U+%
\expandafter\UTFviii@hexnumber\expandafter{%
                     \the\numexpr\expandafter\decode@UTFviii\detokenize{#1}\relax})%
}


\gdef\decode@UTFviii#1\relax{%
  \expandafter\UTFviii@cleanup
    \the\numexpr\dec@de@UTFviii#1\relax)))))\@empty}
\gdef\UTFviii@cleanup#1)#2\@empty{#1}
\def\d@c@de@UTFviii#1{%
\dec@de@UTFviii{#1}}
\gdef\dec@de@UTFviii#1{%
\ifx\relax#1%
\else
  \ifnum`#1&gt;"EF
    ((((`#1-"F0)%
  \else
    \ifnum`#1&gt;"DF
      (((`#1-"E0)%
    \else
      \ifnum`#1&gt;"BF
        ((`#1-"C0)%
      \else
        \ifnum`#1&gt;"7F
        )*64+(`#1-"80)%
        \else
        +`#1 %
        \fi
      \fi
    \fi
  \fi
  \expandafter\dec@de@UTFviii
\fi}
\gdef\UTFviii@hexnumber#1{%
\ifnum#1&lt;"10 0\fi
\ifnum#1&lt;"100 0\fi
\ifnum#1&lt;"1000 0\fi
\UTFviii@hexnumb@r{#1}}
\gdef\UTFviii@hexnumb@r#1{%
\ifnum#1&gt;15 %
\expandafter\UTFviii@hexnumb@r\expandafter{\the\numexpr(#1-8)/16\relax}%
\fi
\UTFviii@hexdigit{\numexpr#1\ifnum#1&gt;0-((#1-8)/16)*16\fi\relax}%
}
\gdef\UTFviii@hexdigit#1{\ifcase\numexpr#1\relax
  0\or1\or2\or3\or4\or5\or6\or7\or8\or9\or
  A\or B\or C\or D\or E\or F\fi}



\def\charwithcodepoint#1{%
\ifcat\relax\noexpand#1%
\expandafter\expandafter\expandafter\charwithcodepointb\expandafter\@gobble\string#1
\else
\charwithcodepointb{#1}%
\fi}
\makeatother

\addto\captionsenglish{\renewcommand{\figurename}{\sffamily{\textbf{Figure}}}}
\renewcommand*{\figureformat}{%
  \figurename~\thefigure%
%  \autodot% DELETED
}


<xsl:apply-templates select="node()/title"/>
<xsl:apply-templates select="node()/subtitle"/>
<xsl:apply-templates select="node()/author"/>
\titlehead{\centering<xsl:apply-templates select="node()/titlepic"/>}

\begin{document}
\defaultfont
\maketitle
{\noindent This document is also available at \url{https://milasudril.github.io/anja}}
<xsl:apply-templates select="node()/abstract"/>
<xsl:apply-templates select="node()/chapter-star[@id='ack']"/>
<xsl:apply-templates select="node()/chapter-star[@id='ack']/following-sibling::node()"/>

\end{document}
</xsl:template>

</xsl:stylesheet>
