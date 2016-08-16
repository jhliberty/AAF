###############################################################################
#
# $Id: GenEnumValidation.pl,v 1.9 2009/06/01 11:46:51 stuart_hc Exp $ $Name: V116 $
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

###############################################################################
#
# File: GenEnumValidation.pl
#
# Created: 16-JAN-2000 by Tom Ransdell.
# 
# Purpose:
#   Automatically generate runtime validation code for each enum recognized
# from dod the files passed on the command line. NOTE: If there are any 
# dependencies between files then files must be passed in the correct
# dependency order or the C/C++ compiler will fail to compile the generated
# validate files, CAAFEnumValidation.h and CAAFEnumValidation.cpp.
#
# Typical usage:
# perl GenEnumValidation.pl AAFTypes.dod AAFPluginTypes.dod > enumValidationLog.txt
# will generate three files:
# CAAFEnumValidation.cpp,
# CAAFEnumValidation.h and
# enumValidationLog.txt
#
# perl GenEnumValidation.pl --source AAFTypes.dod AAFPluginTypes.dod > enumValidationLog.txt
# will generate two files:
# CAAFEnumValidation.cpp and
# enumValidationLog.txt
#
#
# perl GenEnumValidation.pl --include AAFTypes.dod AAFPluginTypes.dod > enumValidationLog.txt
# will generate two files:
# CAAFEnumValidation.h and
# enumValidationLog.txt
#
###############################################################################

require 5.002;

use Getopt::Long;
use Class::Struct;


#
# Define a structure to represent an enum.
#
struct(EnumStruct => [tag => '$', name => '$', num => '$', member => '@']);

#
# Define a structure to represent an enum member.
#
struct(EnumMemberStruct => [name => '$', value => '$']);



printf "Running GenEnumValidation.pl...\n";


# Command line options that control the type of the output.
my $include = 0; # 'CAAFEnumValidation.h';
my $source = 0; #'CAAFEnumValidation.cpp';

# Setup option tags and local variable assignment.
my %optctl = (
  "debug" => \$debug,
  "include" => \$include,
  "source" => \$source
);

# Get the options
my $result = GetOptions (\%optctl, "debug", "include", "source");
exit 1 if (!$result);

# Default to creating both the source and the include files.
if (!$include && !$source) {
 $include = 1;
 $source = 1;
}

# Variable to hold the list of processed files.
my @fileList = ();

# Variable to hold a copy of the current line that we are processing.
my $line = '';

# The number of enum declaration lines found during processing.
my $enumCount = 0;

# The current enum tag or name.
my $enumName = '';

# The string that contains the enum member lines after filtering
# out all unnecessary characters.
my $enumString = '';

# The hash that contains all of the EnumStruct objects allocated
# during processing.
my %enums = ();

# State variable that holds a reference to the current line
# matching subroutine.
my $processLine;



#
# process all of the input files...
#
foreach $file (@ARGV) {
  printf "Processing: %s\n", $file;

  open (DODOFILE , "$file") or die "Could not open \"$file\".\n";

  # Save the file name.
  push @fileList, $file;

  # Initially we want to start out by searching for the first line 
  # that contains an enum declaration.
  $processLine = \&find_start_of_enum;

LINE: while (<DODOFILE>) {

    # Skip any lines that contain a dodo macro.
    next LINE if (/AD_/ || /\#c/);
	
    # special extra enum that we don't need to validate (only exists
    # to keep Microsoft's MIDL compiler happy.
    next LINE if (/aafMAX_NUM_RGBA_COMPS/);

    s/\s*\/\/.*$//; # remove c++ comments.
    s/\/\*.*\*\///; # remove one line c comments. 

    $line = $_;

    # Parse the current line.
    &$processLine;
  }
  close (DODOFILE);
}


# Write out a function prototype to validate each enum.
&make_validateHeader if ($include);
&make_validateSource if ($source);



# Exit this script. All subroutines are defined below
# the exit statement (this may not be necessary).
exit;


# Subroutine to process characters until the we match the
# starting line of an enum.
sub find_start_of_enum 
{
  # Find the starting line that defines the enum.
  if ($line =~ /^\s*(typedef)?\s*enum\s+(\w*)\s*(\})?/) {
    $enumCount++;

    printf "%d: %s: %s", $enumCount, $2, $line;

    # Create an new EnumStruct object
    $enum = new EnumStruct;

    # same the name of the enum.
    $enum->tag($2);
    $enum->name($2); # default the name to be the same as the tag.
    $enum->num(0);


    # Handle the case where the element of the enum may be on 
    # the same line as the enum tag
    if ($3 eq '}') {
      $_ = $line;
      s/^.*\{/\{/;
      $line = $_;
    }
     
    # Start looking for the end of the enum.
    $processLine = \&find_end_of_enum;
  }
}


# Subroutine to process characters until the we match the
# starting line of an enum.
sub find_end_of_enum 
{
  my $lastLine = '';
  
  # There are many substitution operations on this line.
  $_ = $line;

  # Find the last line that defines the enum.
  if (/}/) {
    $lastLine = $_;
    s/}.*$//;     # remove the '}' and any trailing characters.
                  # continue and see if there was any member on this line.
  } 

  # Filter out any unnecessary text.
  s/{//;          # remove any '{'.
  s/\,.*$//;      # remove any commas and trailing characters.
  s/^\s*//;       # remove all leading whitespace.
  s/^\n$//;       # remove any blank lines.
  $line = $_;

  if ($line ne '') { # skip any blank lines.
    # At this point the line should be either:
    # memberName or
    # memberName = value
    /^(\w+)\s*|$/ or die "Unexpected or malformed enum member.\n";
    $enumMember = new EnumMemberStruct;
    $enumMember->name($1);

    # Save this enum member inside the current enum.
    $enum->member($enum->num, $enumMember);
    $enum->num($enum->num + 1);

    # Grow the current expression string.
    $enumString = join ('', $enumString, $line);
  }

  if ($lastLine ne '') {
    # Save the enum name if one exists.
    if ($lastLine =~ /}\s*(\w+)\s*;/) {
      if ($1 ne '') {
        $enum->name($1);
        printf "%s\n", $enum->name;
      }
    }
    
    # Skip anonymous enums. These should not be used
    # for validation.
    if ($enum->name ne '') { 
      $enums{$enum->name} = $enum;
    }

    print $enumString, "\n";
    
    # cleanup
    undef ($enum);
    $enumString = '';

    # Start over looking for the start of the enum.
    $processLine = \&find_start_of_enum;
  }
}


# Subroutine to print the validation function declaration.
# This is shared by both make_validateHeader and 
# make_validateSource.
sub printFunctionDeclaration
{
  local (*THEFILE, $name) = @_;  # Filehandle, string

  # Enums that end in AAF are special. Strip off the
  # the _e so that we can use the rest of the name as 
  # base and append the '_t' to create the corresponding 
  # type name.
  $_ = $name;
  s/(_[et])$/_t/;
  $name = $_;

  print THEFILE 'bool Is_', $name, '_Valid(', $name, ' param)';
}

# Subroutine to write out the validateEnums.h header file
# with a function prototype for each enum.
sub make_validateHeader
{
  open(HEADER, ">CAAFEnumValidation.h") or die "Count not create enum validation header file.\n";

  print HEADER "\n";
  
  # Include order is important. For now we assume that the files
  # are being processing in dependency order. For example:
  # the file list had better have AAFTypes.dod before AAFPluginsTypes.dod.
  foreach $file (@fileList) {
    $_ = $file;
    s/(\.dod)$//;
    
    print HEADER '#include "', $_;
    print HEADER '.h' if ($1 ne ''); # only add if one was removed.
    print HEADER '"', "\n";
  }

#  print HEADER '#include "AAFTypes.h"', "\n";
#  print HEADER '#include "AAFPluginTypes.h"', "\n\n";

  # Write out a function prototype to validate each enum.
  print HEADER "\n\n// Function prototypes for all AAF enums.\n\n";
  print HEADER '#ifdef __cplusplus', "\n";
  print HEADER 'extern "C" {', "\n";
  print HEADER '#endif', "\n\n";

  foreach $enumName (sort keys %enums) {
    $enum = $enums{$enumName};
    printFunctionDeclaration(*HEADER, $enumName);
    print HEADER ";\n";
   }

  print HEADER "\n", '#ifdef __cplusplus', "\n";
  print HEADER '}', "\n";
  print HEADER '#endif', "\n";

  close(HEADER);
}


# Subroutine to write out the validateEnums.h header file
# with a function prototype for each enum.
sub make_validateSource
{
  open(SOURCE, ">CAAFEnumValidation.cpp") or die "Count not create enum validation source file.\n";

  print SOURCE "\n";
  print SOURCE '#include "CAAFEnumValidation.h"', "\n\n";

  foreach $enumName (sort keys %enums) {
    $enum = $enums{$enumName};
    printFunctionDeclaration(*SOURCE, $enumName);
    printf SOURCE "\n{\n";
    printf SOURCE "  switch (param) {\n";
    my $i;
    for ($i = 0; $i < $enum->num; $i++) {
      $enumMember = $enum->member($i);
      printf SOURCE "    case %s:\n", $enumMember->name;
    }
    printf SOURCE "      return true;\n";
    printf SOURCE "    default:\n";
    printf SOURCE "      return false;\n";
    printf SOURCE "  }\n";
    printf SOURCE "}\n\n";
  }

  close(SOURCE);
}
