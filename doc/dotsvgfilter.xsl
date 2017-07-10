<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:svg="http://www.w3.org/2000/svg">
    <xsl:template match="@*|node()">
        <xsl:copy>
            <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
    </xsl:template>

	<xsl:template match="@font-family" />
	<xsl:template match="@font-size" />
	<xsl:template match="@width" />
	<xsl:template match="@height" />
	<xsl:template match="svg:polygon" />

	<xsl:template match="svg:polygon[preceding::svg:polygon]">
        <xsl:copy>
            <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
	</xsl:template>

	<xsl:template match="svg:svg">
	<xsl:copy>
	<xsl:apply-templates select="@*"/>
	<svg:style>
text
	{
	 font-size:12px;fill:#800040;font-family:"Latin Modern Roman",Serif;
	}
ellipse
	{
	 fill:#ffffff;stroke-width:2px,stroke:rgb(0,0,0)
	}
</svg:style>
<xsl:apply-templates select="node()"/>
</xsl:copy>
	</xsl:template>

</xsl:stylesheet>
