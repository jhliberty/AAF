<?xml version="1.0" encoding="UTF-8"?>
<!--
  $Id: propdef.xsl,v 1.2 2009/06/01 11:47:10 stuart_hc Exp $ $Name: V116 $

  The contents of this file are subject to the AAF SDK Public Source
  License Agreement Version 2.0 (the "License"); You may not use this
  file except in compliance with the License.  The License is available
  in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
  Advanced Media Workflow Association, Inc., or its successor.

  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
  the License for the specific language governing rights and limitations
  under the License.  Refer to Section 3.3 of the License for proper use
  of this Exhibit.

  WARNING:  Please contact the Advanced Media Workflow Association,
  Inc., for more information about any additional licenses to
  intellectual property covering the AAF Standard that may be required
  to create and distribute AAF compliant products.
  (http://www.amwa.tv/policies).

  Copyright Notices:
  The Original Code of this file is Copyright 1998-2009, licensor of the
  Advanced Media Workflow Association.  All rights reserved.

  The Initial Developer of the Original Code of this file and the
  licensor of the Advanced Media Workflow Association is
  British Broadcasting Corporation.
  All rights reserved.
-->
<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:xs="http://www.w3.org/2001/XMLSchema" 
    xmlns:aafm="http://www.aafassociation.org/aafx/v1.1/20050628" 
    exclude-result-prefixes="aafm">

    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>


    <xsl:template match="aafm:PropertyDefinition">

        <xsl:choose>
            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':ByteOrder')">
                <xs:element name="{aafm:Symbol}" type="{$baselinePrefix}:_T_ByteOrder"/>
            </xsl:when>
            <xsl:otherwise>
                <xs:element name="{aafm:Symbol}" type="{aafm:Type}">
                    <xsl:call-template name="constraints_for_sets">
                        <xsl:with-param name="property" select="."/>
                        <xsl:with-param name="isRoot" select="'false'"/>
                    </xsl:call-template>
                </xs:element>
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>

    
    <xsl:template name="prop_defs_in_class">
        <xsl:param name="class" select="/.."/>

        <xsl:variable name="isRootClass">
            <xsl:call-template name="is_root_class">
                <xsl:with-param name="class" select="$class"/>
            </xsl:call-template>
        </xsl:variable>

        <xsl:if test="$isRootClass = 'false'">
            <xsl:call-template name="prop_defs_in_class">
                <xsl:with-param name="class" select="key('classDefQSymbolKey', $class/aafm:ParentClass)"/>
            </xsl:call-template>
        </xsl:if>

        <xsl:apply-templates select="key('propertyDefMemberOfQSymbolKey', $class/aafm:QSymbol)" mode="reference"/>

    </xsl:template>


    <xsl:template match="aafm:PropertyDefinition" mode="reference">

        <xs:element ref="{aafm:QSymbol}">
            <xsl:if test="aafm:IsOptional = 'true'">
                <xsl:attribute name="minOccurs">
                    <xsl:value-of select="'0'"/>
                </xsl:attribute>
            </xsl:if>
        </xs:element>

    </xsl:template>


</xsl:stylesheet>

