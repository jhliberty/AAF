###############################################################################
#
# $Id: flatlog.awk,v 1.10 2009/06/01 11:46:49 stuart_hc Exp $ $Name: V116 $
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

#
# Flatten the output of "cvs log".
#
# Author : Tim Bingham - Tim_Bingham@avid.com
#
# A flattened log is easier to process than the output of "cvs log".
# The idea is that the log is first flattened and then the flat log
# is used as input to other tools. The output of "cvs log" is for people
# to read, the output of this program is for other programs to read.
#
# Example :
#
# $ cvs log > Everything.log
# $ cat Everything.log | awk -f flatlog.awk  > Everything.flog
#
# The following prints the date and time of the most recent change
# preceeding application of the Build-0505 tag.
#
# $ grep Build-0505 Everything.flog | \ 
#   sort -r -k 3 -k 4 | \ 
#   head -n 1 | \ 
#   awk '{printf("%s %s %s\n", $7, $3, $4)}'
# Build-0505 2002/05/29 21:23:58
# $
#
# Functionality : Flattening the log includes -
#
#   - putting the information (revision, date, user etc.) about each
#     action (add, change or remove a file) all on one line
#   - treating the act of tagging a file as just another action, in this case
#     the date and time are the date and time of the tagged revision and not
#     the date and time the tag was applied (cvs doesn't record this)
#
# Output format :
#
#   One line for each action (the lines are in no particular order).
#   Each line consists of space separated fields as follows -
#
#  <file> <revision> <date> <time> <user> <action> ...
#
# There are a variable number of fields following <action>, these
# depend on the value of <action> as follows -
#
#   <action>                             ...
#     A (add    - cvs add; cvs commit)   log-message
#     C (change - cvs commit)            log-message
#     R (remove - cvs remove)            log-message
#     T (tag    - cvs tag or cvs rtag)   tag-name
#
# Bugs:
#
# There are bugs here for
#
#  - log messages that look like fields in the header information
#  - file names containing spaces/tabs
#

/^Working file:/ {
  file  = $3;
  revision = "";
}

/^revision/ {
  revision = $2;
}

/date:/ {
  date = $2;
  time = $3;
  who = $5;
  state = $7;
  gsub(";", "", time);
  gsub(";", "", who);
  gsub(";", "", state);
  if (state == "Exp") {
    action = "C"; # Change
  } else if (state == "dead") {
    action = "R"; # Remove
  } else {
    action = "U"; # Unknown
  }
  if (revision == "1.1") {
    action = "A"; # Add
  }
  getline;
  comment = $0;
}

/^symbolic names:/ {
  inTagList = 1;
  getline;
  for (i in tagmap) {
    delete tagmap[i];
  }
}

/keyword substitution:/ {
  inTagList = 0;
}

// {
  if (inTagList == 1) {
    tag = $1;
    tagrev = $2;
    gsub(":", "", tag);
    # Associative array - yum !
    tagmap[tagrev] = tagmap[tagrev] " " tag;
  }
}

function printLogLine()
{
  printf("%s %s %s %s %s %s", file, revision, date, time, who, action);
  if ((action == "C") || (action == "A") || (action == "R")) {
    printf(" %s", comment);
  }
  printf("\n");
  if (revision in tagmap) {
   action = "T";
   split(tagmap[revision], names, " ");
   for (n in names) {
     printf("%s %s %s %s %s %s", file, revision, date, time, who, action);
     printf(" %s", names[n]);
     printf("\n");
   }
  }
}

/^----------------------------/ {
  if (revision != "") {
    printLogLine();
  }
}

/^=============================================================================/ {
  printLogLine();
}
