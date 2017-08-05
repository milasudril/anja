<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="@*|node()">
        <xsl:copy>
            <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
    </xsl:template>

	<xsl:template match="h1">
		<chapter>
			<xsl:attribute name="id">cmdline</xsl:attribute>
            <xsl:apply-templates select="@*|node()"/>
        </chapter>
	</xsl:template>

	<xsl:template match="h2">
		<section>
			<xsl:attribute name="id">cmdline<xsl:value-of select="position()"/></xsl:attribute>
            <xsl:apply-templates select="@*|node()"/>
        </section>
	</xsl:template>

	<xsl:template match="code">
		<cmdoption><xsl:apply-templates select="@*|node()"/></cmdoption>
	</xsl:template>

	<xsl:template match="li">
		<xsl:copy>
			<xsl:attribute name="class">cmdoption</xsl:attribute>
            <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
	</xsl:template>

</xsl:stylesheet>
