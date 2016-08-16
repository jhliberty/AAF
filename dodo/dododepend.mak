###############################################################################
#
# $Id: dododepend.mak,v 1.31 2009/06/01 11:46:51 stuart_hc Exp $ $Name: V116 $
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

all : depend.mk


BLD_CFG_DIR = ../ref-impl/bld_cfg

include $(BLD_CFG_DIR)/common.mk


# Include the file that lists all of the dodo target names.
include aafobjects.mk


depend.mk : aafobjects.mk GenDepend.sh
	@ $(ECHO) Creating depend.tmp ...
	@ $(RM) -f depend.tmp
	$(ECHO) Doing first depend step...
	$(CP)  aafobjects.mk tmp.sh
	$(CHMOD) a+w tmp.sh
	$(CAT) GenDepend.sh >> tmp.sh
	$(SH) tmp.sh > depend.tmp
	$(ECHO) Doing second depend step...
	$(CP)  aafobjects.mk tmp2.sh
	$(CHMOD) a+w tmp2.sh
	$(CAT) GenDepend2.sh >> tmp2.sh
	$(SH) tmp2.sh
	$(RM) tmp.sh tmp2.sh
	@ $(MV) depend.tmp depend.mk
	@ $(ECHO) "Done with depend.mk."


clean :
	$(RM) -rf depend.mk



