###############################################################################
#
# $Id: GNUmakefile,v 1.13 2009/06/01 11:46:51 stuart_hc Exp $ $Name: V116 $
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################


all: ComCutsOnlyTest ComEssenceDataTest ComExtension ComPropDirectAccess \
	ComPropDirectDump ExportAudioExample ExportSimpleComposition \
	ImportAudioExample InfoDumper MetadataExample comClientTest \
	ExportDV ImportDV ExportJPEG ImportJPEG ExportPCM ImportPCM \
	ExportVC3 ImportVC3 ExportAS05Effects

.PHONY : ComCutsOnlyTest
ComCutsOnlyTest:
	cd ComCutsOnlyTest && $(MAKE)

.PHONY : ComEssenceDataTest
ComEssenceDataTest:
	cd ComEssenceDataTest && $(MAKE)

.PHONY : ComExtension
ComExtension:
	cd ComExtension && $(MAKE)

.PHONY : ComPropDirectAccess
ComPropDirectAccess:
	cd ComPropDirectAccess && $(MAKE)

.PHONY: ComPropDirectDump
ComPropDirectDump:
	cd ComPropDirectDump && $(MAKE)

.PHONY: ExportAudioExample
ExportAudioExample:
	cd ExportAudioExample && $(MAKE)

.PHONY : ExportSimpleComposition
ExportSimpleComposition:
	cd ExportSimpleComposition && $(MAKE)

.PHONY : ImportAudioExample
ImportAudioExample:
	cd ImportAudioExample && $(MAKE)

.PHONY : InfoDumper
InfoDumper:
	cd InfoDumper && $(MAKE)

.PHONY : MetadataExample
MetadataExample :
	cd MetadataExample && $(MAKE)

.PHONY : comClientTest
comClientTest :
	cd comClientTest && $(MAKE)

.PHONY : ExportDV
ExportDV :
	cd ExportDV && $(MAKE)

.PHONY : ImportDV
ImportDV :
	cd ImportDV && $(MAKE)

.PHONY : ExportJPEG
ExportJPEG :
	cd ExportJPEG && $(MAKE)

.PHONY : ImportJPEG
ImportJPEG :
	cd ImportJPEG && $(MAKE)

.PHONY : ExportPCM
ExportPCM :
	cd ExportPCM && $(MAKE)

.PHONY : ImportPCM
ImportPCM :
	cd ImportPCM && $(MAKE)

.PHONY : ExportVC3
ExportVC3 :
	cd ExportVC3 && $(MAKE)

.PHONY : ImportVC3
ImportVC3 :
	cd ImportVC3 && $(MAKE)

.PHONY : ExportAS05Effects
ExportAS05Effects :
	cd ExportAS05Effects && $(MAKE)

# Run ComPropDirectDump, comClientTest and InfoDumper checks last since all
# generated AAF files will be available to be read and tested
.PHONY : check
check : all
	cd ComCutsOnlyTest && $(MAKE) $@
	cd ComEssenceDataTest && $(MAKE) $@
	cd ComExtension && $(MAKE) $@
	cd ComPropDirectAccess && $(MAKE) $@
	cd ExportSimpleComposition && $(MAKE) $@
	cd ExportAudioExample && $(MAKE) $@
	cd ImportAudioExample && $(MAKE) $@
	cd MetadataExample && $(MAKE) $@
	cd ExportDV && $(MAKE) $@
	cd ImportDV && $(MAKE) $@
	cd ExportJPEG && $(MAKE) $@
	cd ImportJPEG && $(MAKE) $@
	cd ExportPCM && $(MAKE) $@
	cd ImportPCM && $(MAKE) $@
	cd ExportVC3 && $(MAKE) $@
	cd ImportVC3 && $(MAKE) $@
	cd ExportAS05Effects && $(MAKE) $@
	cd ComPropDirectDump && $(MAKE) $@
	cd comClientTest && $(MAKE) $@
	cd InfoDumper && $(MAKE) $@

.PHONY : clean
clean :
	cd ComCutsOnlyTest && $(MAKE) GENDEPS=0 $@
	cd ComEssenceDataTest && $(MAKE) GENDEPS=0 $@
	cd ComExtension && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectAccess && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectDump && $(MAKE) GENDEPS=0 $@
	cd ExportAudioExample && $(MAKE) GENDEPS=0 $@
	cd ExportSimpleComposition && $(MAKE) GENDEPS=0 $@
	cd ImportAudioExample && $(MAKE) GENDEPS=0 $@
	cd InfoDumper && $(MAKE) GENDEPS=0 $@
	cd MetadataExample && $(MAKE) GENDEPS=0 $@
	cd comClientTest && $(MAKE) GENDEPS=0 $@
	cd ExportDV && $(MAKE) GENDEPS=0 $@
	cd ImportDV && $(MAKE) GENDEPS=0 $@
	cd ExportJPEG && $(MAKE) GENDEPS=0 $@
	cd ImportJPEG && $(MAKE) GENDEPS=0 $@
	cd ExportPCM && $(MAKE) GENDEPS=0 $@
	cd ImportPCM && $(MAKE) GENDEPS=0 $@
	cd ExportVC3 && $(MAKE) GENDEPS=0 $@
	cd ImportVC3 && $(MAKE) GENDEPS=0 $@
	cd ExportAS05Effects && $(MAKE) GENDEPS=0 $@

.PHONY : realclean
realclean :
	cd ComCutsOnlyTest && $(MAKE) GENDEPS=0 $@
	cd ComEssenceDataTest && $(MAKE) GENDEPS=0 $@
	cd ComExtension && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectAccess && $(MAKE) GENDEPS=0 $@
	cd ComPropDirectDump && $(MAKE) GENDEPS=0 $@
	cd ExportAudioExample && $(MAKE) GENDEPS=0 $@
	cd ExportSimpleComposition && $(MAKE) GENDEPS=0 $@
	cd ImportAudioExample && $(MAKE) GENDEPS=0 $@
	cd InfoDumper && $(MAKE) GENDEPS=0 $@
	cd MetadataExample && $(MAKE) GENDEPS=0 $@
	cd comClientTest && $(MAKE) GENDEPS=0 $@
	cd ExportDV && $(MAKE) GENDEPS=0 $@
	cd ImportDV && $(MAKE) GENDEPS=0 $@
	cd ExportJPEG && $(MAKE) GENDEPS=0 $@
	cd ImportJPEG && $(MAKE) GENDEPS=0 $@
	cd ExportPCM && $(MAKE) GENDEPS=0 $@
	cd ImportPCM && $(MAKE) GENDEPS=0 $@
	cd ExportVC3 && $(MAKE) GENDEPS=0 $@
	cd ImportVC3 && $(MAKE) GENDEPS=0 $@
	cd ExportAS05Effects && $(MAKE) GENDEPS=0 $@
