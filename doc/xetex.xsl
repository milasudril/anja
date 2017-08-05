<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text" indent="no"/>
<xsl:strip-space elements="*[not(self::code)] "/>

<!--xsl:template match="verbatiminput" >\verbatiminput{<xsl:value-of select="@src"/>}</xsl:template-->
<xsl:template match="verbatiminput" />

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

<xsl:template match="row"><xsl:apply-templates select="col[1]" mode="normal"/> &amp; <xsl:apply-templates select="col[2]" mode="normal"/>\\
</xsl:template>

<xsl:template match="tabular">
\begin{tabular}{lp{0.5\textwidth}}
\hline
\hline
<xsl:apply-templates select="node()"/>\hline
\hline
\end{tabular}
</xsl:template>

<xsl:template match="chapter">

\chapter{<xsl:value-of select="node()"/>}
\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="section">

\section{<xsl:apply-templates select="node()"/>}
%\label{<xsl:value-of select="@id"/>}
</xsl:template>

<xsl:template match="strong">\textbf{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="cite">\cite{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="prgname|libname|dirname">\texttt{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="ref">\cref{<xsl:apply-templates select="node()"/>}</xsl:template>

<xsl:template match="code">\begin{verbatim}
<xsl:apply-templates select="node()"/>
\end{verbatim}

</xsl:template>



<xsl:template match="/">\documentclass[a4paper,twoside,12pt]{scrbook}
\usepackage[T1]{fontenc}
\usepackage[english]{babel}
\usepackage{lmodern}
\usepackage{verbatim}
\usepackage{graphicx}

\usepackage{varioref}
\usepackage{hyperref}
\usepackage{cleveref}

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
