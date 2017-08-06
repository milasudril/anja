<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:fn="http://www.w3.org/2005/xpath-functions" version="1.0">
  <xsl:output method="text" indent="no"/>
<xsl:strip-space elements="*"/>
<xsl:preserve-space elements="code"/>

<!--xsl:template match="verbatiminput" >\verbatiminput{<xsl:value-of select="@src"/>}</xsl:template-->
<xsl:template match="verbatiminput" />


<xsl:template match="text()" mode="verbatim">
<xsl:value-of select="." />
</xsl:template>

<xsl:template match="text()"><xsl:text>&#x7f;</xsl:text><xsl:value-of select="."/><xsl:text>&#x7f;</xsl:text></xsl:template>

<xsl:template match="titlepic">
\includegraphics[width=0.8\textwidth]{<xsl:value-of select="includegraphics[last()]/@src"/>}
</xsl:template>

<xsl:template match="title">\title{<xsl:value-of select="node()"/>}
</xsl:template>
<xsl:template match="subtitle">\subtitle{<xsl:value-of select="node()"/>}
</xsl:template>
<xsl:template match="author">\author{<xsl:value-of select="node()"/>}
</xsl:template>

<xsl:template match="abstract">
\chapter*{Abstract}

<xsl:apply-templates select="@*|node()"/>
</xsl:template>

<xsl:template match="chapter-star">
\chapter*{<xsl:apply-templates select="node()"/>}

</xsl:template>

<xsl:template match="ul">\begin{itemize}
<xsl:apply-templates select="node()"/>\end{itemize}

</xsl:template>

<xsl:template match="p"><xsl:apply-templates select="node()"/><xsl:text>

</xsl:text></xsl:template>

<xsl:template match="li">  \item{<xsl:apply-templates select="node()"/>}
</xsl:template>

<xsl:template match="tableofcontents">\tableofcontents</xsl:template>

<xsl:template match="col" mode="header">\textbf{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="col" mode="normal"><xsl:apply-templates select="node()"/></xsl:template>


<xsl:template match="colheaders"><xsl:apply-templates select="col[1]" mode="header" /> &amp; <xsl:apply-templates select="col[2]" mode="header"/> \\
\hline
</xsl:template>

<xsl:template match="hline">\hline</xsl:template>

<xsl:template match="row"><xsl:apply-templates select="col[1]" mode="normal"/> &amp; <xsl:apply-templates select="col[2]" mode="normal"/>\\
</xsl:template>

<xsl:template match="tabular">\begin{center}
\begin{tabular}{lp{0.6\textwidth}}
\hline
\hline
<xsl:apply-templates select="node()"/>\hline
\hline
\end{tabular}
\end{center}
</xsl:template>

<xsl:template match="chapter">

\chapter{<xsl:value-of select="node()"/>}
\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="section">

\section{<xsl:apply-templates select="node()"/>}
\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="strong">\textbf{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="cite">\cite{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="prgname|libname|dirname">\texttt{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="ref">\cref{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="code">\begin{verbatim}
<xsl:apply-templates select="node()" mode="verbatim"/>
\end{verbatim}

</xsl:template>

<xsl:template match="kbd">\kbd{<xsl:apply-templates select="node()" />}</xsl:template>
<xsl:template match="ui-button">\uibutton{<xsl:apply-templates select="node()" />}</xsl:template>

<xsl:template match="ui-formfield">\uiformfield{<xsl:apply-templates select="node()" />}</xsl:template>

<xsl:template match="char">\charwithcodepoint{<xsl:apply-templates select="node()" />}</xsl:template>

<xsl:template match="includegraphics">\includegraphics[height=2ex]{<xsl:value-of select="@src"/><xsl:if test="substring-after(@src,'.')='svg'">.pdf</xsl:if>}</xsl:template>

<xsl:template match="col/includegraphics">\multicolumn{1}{c}{\includegraphics[height=2ex]{<xsl:value-of select="@src"/><xsl:if test="substring-after(@src,'.')='svg'">.pdf</xsl:if>}}</xsl:template>

<xsl:template match="/">\documentclass[a4paper,twoside,12pt]{scrbook}
\usepackage[english]{babel}

\usepackage{fontspec}
\usepackage{verbatim}
\usepackage{graphicx}
\usepackage{color}
\usepackage{xcolor}
\usepackage[multidot]{grffile}

\usepackage{varioref}
\usepackage{hyperref}
\usepackage{cleveref}



\definecolor{lightgray}{gray}{0.9}
\newcommand{\kbd}[1]{ \colorbox{black}{\textcolor{white}{{\texttt{#1}}}} }
\newcommand{\uibutton}[1]{ \colorbox{lightgray}{{{\textsf{#1}}}} }
\newcommand{\uiformfield}[1]{ \colorbox{lightgray}{{{\textit{\textsf{#1}}}}} }

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

<xsl:apply-templates select="node()/title"/>
<xsl:apply-templates select="node()/subtitle"/>
<xsl:apply-templates select="node()/author"/>
\titlehead{\centering<xsl:apply-templates select="node()/titlepic"/>}

\begin{document}
\begin{titlepage}
\maketitle
\end{titlepage}
<xsl:apply-templates select="node()/abstract"/>
<xsl:apply-templates select="node()/chapter-star[@id='ack']"/>
<xsl:apply-templates select="node()/chapter-star[@id='ack']/following-sibling::node()"/>

\end{document}
</xsl:template>

</xsl:stylesheet>
