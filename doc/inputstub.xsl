<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="@*|node()">
        <xsl:copy>
            <xsl:apply-templates select="@*|node()"/>
        </xsl:copy>
    </xsl:template>

	<xsl:template match="input">
		<!--this is a hack (only works if we are one level from project root)-->
		<xsl:apply-templates select="document(concat('../',@src))/content/*"/>
	</xsl:template>

</xsl:stylesheet>
