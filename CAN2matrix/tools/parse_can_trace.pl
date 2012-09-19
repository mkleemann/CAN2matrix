#!/bin/perl -w

#use strict ;
use File::Spec;
use File::Basename;
use File::Find;

##############################################################################
#
#   filename     : parse_can_trace.pl
#   description  : generates parsed output from trace files by given IDs
#
#   prog lang    : Perl
#   platform     : ActiveState ActivePerl 5.8 (Win32)
#
#   author       : M. Kleemann
#   date         : 13.09.2012
#
##############################################################################

## FILEFORMAT TO PARSE #######################################################

# ;$FILEVERSION=1.1
# ;$STARTTIME=41165.376038044
# ;
# ;   <path>\<file>.trc
# ;
# ;   Start time: 13.09.2012 09:01:29.687.x
# ;
# ;   Message Number
# ;   |         Time Offset (ms)
# ;   |         |        Type
# ;   |         |        |        ID (hex)
# ;   |         |        |        |     Data Length Code
# ;   |         |        |        |     |   Data Bytes (hex) ...
# ;   |         |        |        |     |   |
# ;---+--   ----+----  --+--  ----+---  +  -+ -- -- -- -- -- -- --
#      1)    755889.9  Rx         0591  3  00 00 89
#      2)    755896.9  Rx         0151  4  00 00 A0 A0
#      3)    755898.4  Rx         062F  4  01 FF 70 90
#      4)    755907.5  Rx         0470  5  20 00 00 FF 00

##############################################################################

### PROGRAM SETTINGS #########################################################

# get current working directory
my $rootDir = File::Spec->rel2abs(basename($0, ''), dirname($0, ''));
   $rootDir = dirname($rootDir);    # remove program name

my $trace2parse = "";
my $outfile = ".parsed";
my $initial = 0;
my @id_storage;
my %id_lookup;

# get arguments
while ($_ = shift @ARGV) {
   if ( /^-help/) {           # need help
      usagePrint();
      exit;
   }
   if (0 == $initial) {
      $trace2parse = $_;
      $initial = 1;
      next;
   }
   # get ids
   push(@id_storage, uc $_);
}

# fill hash from argument array to fast lookup availability
@id_lookup{@id_storage} = ();

foreach $element (@id_storage) {
   $outfile = join($element, "_", $outfile);
}

open(TRACE2PARSE, "<$trace2parse") or die "Could not open $trace2parse.";
open(OUT, ">$outfile") or die "Could not create $outfile.";

printf(OUT "ID\tLength\tData\tTimestamp\n");

while (<TRACE2PARSE>) {
   # example of line to parse
   #      1)    755889.9  Rx         0591  3  00 00 89
   if (/[0-9]*\)[\t ]*([0-9.]*)[\t ]*(T|R)x[\t ]*([0-9A-F]{4})[\t ]*([0-8]{1})[\t ]*(.*)/) {
      if (exists $id_lookup{$3}) {
         # $1 timestamp
         # $2 Rx/Tx
         # $3 CAN ID
         # $4 message length
         # $5 payload (data bytes)
         my @bytes = split(/[ \t]/, $5);
         my $datalen = @bytes;

         # print data from logfile
         printf(OUT "%s\t%s\t%s", $3, $4, $5);

         # fill with spaces for alignment (formatting reasons)
         for ($i = $datalen; $i < 8; $i++) {
            printf(OUT "   ");
         }

         # add ASCII representation for readability
         printf(OUT " - ");
         foreach $character (@bytes) {
            my $value = hex($character);
            if ($value > 0x1F) {
               printf(OUT "%s", chr($value));
            } else {
               printf(OUT ".");
            }
         }

         # fill with spaces for timestamp alignment
         for ($i = $datalen; $i < 8; $i++) {
            printf(OUT " ");
         }

         # print timestamp as last entry
         printf(OUT "\t%s\n", $1);
      }
   }
}

close(TRACE2PARSE);
close(OUT);

exit 0;

sub usagePrint {
   system "pod2text $rootDir\\parse_can_trace.pl";
#   system "pod2html $rootDir\\parse_can_trace.pl >parse_can_trace.html";
}


######### POD ##############################################################

=pod

parse_can_trace.pl

=head1 DESCRIPTION

This script is parsing logfiles written by PCAN toolchain. It filters by
IDs given by program arguments.

The output is written to a file. The name is compiled by IDs given in
arguments and other information.

=head2 File Associations on a Win32 System

If you want to use the tool by simply typing the name and any needed
arguments, you should check if the file extension 'B<.pl>' is connected
to 'B<perl.exe>'. If not you have to type something like

   perl parse_can_trace.pl ...

or if Perl isn't included in your search path

   <Drive>:\<Perl Install Path>\bin\perl build.pl ...

=head1 USAGE

   parse_can_trace.pl [option(s)] tracefile ID1 [ID2 [ID3 ... IDn]]

=head2 [option(s)]

   -help                - show this help

=head1 EXAMPLES

   parse_can_trace.pl test.trc 05E4 0271
      parse test.trc for any entry with ID 05E4 or 0271 and writes it to
      _05E4_0271.csv.

   parse_can_trace.pl -help
      show this help

=head1 AUTHOR

Matthias Kleemann I<dev@layer128.net>

=head1 COPYRIGHT

"THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware license):
<dev@layer128.net> wrote this file. As long as you retain this notice you
can do whatever you want with this stuff. If we meet some day, and you think
this stuff is worth it, you can buy me a be(ve)er(age) in return. (I don't
like beer much.)

=head1 VERSION

Version 0.1

=cut


