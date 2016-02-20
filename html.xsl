<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:key name="cite_id" match="/document/bibliography/item" use="@id" />

<xsl:key name="Listing" match="/document/listing" use="@id" />

<xsl:template match="*">
<xsl:copy-of select="." />
</xsl:template>

<xsl:template match="ref">
<a>
<xsl:attribute name="href">#<xsl:value-of select="."/></xsl:attribute><xsl:value-of select="@what"/> <xsl:text> </xsl:text>
<xsl:value-of select="count(key(@what,.)/preceding-sibling::chapter)"/>.<xsl:value-of select="count(key(@what,.)/preceding-sibling::listing)+1"/>
</a>
</xsl:template>

<xsl:template match="env">
<code class="env"><xsl:copy-of select="node()" /></code>
</xsl:template>

<xsl:template match="prgname">
<code class="prgname"><xsl:copy-of select="node()" /></code>
</xsl:template>

<xsl:template match="dirname">
<code class="dirname"><xsl:copy-of select="node()" /></code>
</xsl:template>

<xsl:template match="caption">
<xsl:apply-templates />
</xsl:template>

<xsl:template match="code">
<pre><code><xsl:attribute name="class"><xsl:value-of select="@language" /></xsl:attribute><xsl:copy-of select="node()" /></code></pre>
</xsl:template>

<xsl:template match="listing">
<div class="listing"><xsl:attribute name="id"><xsl:value-of select="@id" /></xsl:attribute>
<p><strong>Listing <xsl:number count="chapter" level="any"/>.<xsl:number from="chapter" level="any"/></strong>: <xsl:apply-templates select="caption" /></p>
<xsl:apply-templates select="code" />
</div>
</xsl:template>

<xsl:template match="libname">
<code><xsl:copy-of select="node()" /></code>
</xsl:template>

<xsl:template match="quantity">
<xsl:value-of select="." />&#160;<xsl:value-of select="./@unit" />
</xsl:template>

<xsl:template match="title"/>
<xsl:template match="author"/>

<xsl:template match="cite">&#160;<cite><a><xsl:attribute name="href">#<xsl:value-of select="."/></xsl:attribute>[<xsl:value-of select="count(key('cite_id',.)/preceding-sibling::*)+1" />]</a></cite>
</xsl:template>

<xsl:template match="p">
<p><xsl:apply-templates /></p>
</xsl:template>

<xsl:template match="abstract">
<div class="abstract">
<xsl:copy-of select="node()"/>
</div>
</xsl:template>

<xsl:template match="chapter" mode="toc">
<li><a><xsl:attribute name="href">#<xsl:value-of select="@id" />
</xsl:attribute><xsl:copy-of select="node()"/></a></li>
</xsl:template>

<xsl:template match="chapter">
<h2><xsl:attribute name="id"><xsl:value-of select="@id" /></xsl:attribute>
<xsl:number /><span class="fill" /><xsl:copy-of select="node()"/>
</h2>
</xsl:template>

<xsl:template match="section" mode="toc">
<li><a><xsl:attribute name="href">#<xsl:value-of select="@id" />
</xsl:attribute><xsl:copy-of select="node()"/></a></li>
</xsl:template>

<xsl:template match="section">
<h3><xsl:attribute name="id"><xsl:value-of select="@id" /></xsl:attribute>
<xsl:number count="chapter" level="any"/>.<xsl:number level="any" from="chapter"/> <span class="fill" /><xsl:copy-of select="node()"/>
</h3>
</xsl:template>

<xsl:template match="subsection" mode="toc">
<li><a><xsl:attribute name="href">#<xsl:value-of select="@id" />
</xsl:attribute><xsl:copy-of select="node()"/></a></li>
</xsl:template>

<xsl:template match="subsection">
<h4><xsl:attribute name="id"><xsl:value-of select="@id" /></xsl:attribute>
<xsl:number count="chapter" level="any"/>.<xsl:number level="any" from="chapter"/>.<xsl:number level="any" from="chapter"/> <span class="fill" /><xsl:copy-of select="node()"/>
</h4>
</xsl:template>



<xsl:template match="tableofcontents">
<nav id="toc">
<h2>Table of contents</h2>
<ol>
<xsl:apply-templates select="../chapter" mode="toc" />
</ol>
</nav>
</xsl:template>

<xsl:template match="url">
<a><xsl:attribute name="href"><xsl:value-of select="." /></xsl:attribute><xsl:value-of select="." /></a>
</xsl:template>

<xsl:template match="item">
<p class="bibitem"><xsl:attribute name="id">
<xsl:value-of select="@id"/>
</xsl:attribute>
[<xsl:number level="single"/>]<span class="fill" />
<xsl:apply-templates />
</p>
</xsl:template>

<xsl:template match="bibliography">
<h2>References</h2>
<xsl:apply-templates/>
</xsl:template>

<xsl:template match="document">
<html>
<head>
<title><xsl:value-of select="title" /></title>
<link rel="stylesheet" href="highlight/styles/default.css" type="text/css" />
<link rel="stylesheet" href="format.css" type="text/css" />
<script src="highlight/highlight.pack.js" type="text/javascript"></script>
<script type="text/javascript">hljs.initHighlightingOnLoad();</script>
</head>

<body>
<h1><xsl:value-of select="title" /></h1>
<p class="titlepage"><xsl:value-of select="author" /></p>

<xsl:apply-templates />
</body>

</html>
</xsl:template>

<xsl:template match="/">
<xsl:apply-templates />
</xsl:template>

</xsl:stylesheet>
