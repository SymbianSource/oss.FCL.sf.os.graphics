#!perl
# Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
# # Script: uibench2db.pl
# # Function: This script takes the performance log files created by 
# #           the graphics uibench apps performace test and outputs data to the
# #           SymSym database
# # call: uibench2db.pl --dir="\\loncoredev02.intra\graphics\9.2" --writedb --writecsv
# # Design: This script looks at all the files under --dir
# #         seeking first build.html files then specific test reset logs. 
# #         The test result contents are then extracted.
# #         Results for this build already in the database are deleted.
# #         The new results are added to the database.
# #         The raw results are available at
# #         http://smglinx.intra/SymSym/query/dbquery.pl?dbfilter=team+%3D+%27uibench%27&viewdata=other&dbview=table&dbschema=performance&p_reset=1&dbtable=performance_uibench_tests&Submit=%3E 
# #         The results are graphed at
# #         http://smglinx.intra/twiki/bin/view/Perfresult/TestReportUibench
# #         uibench Location of script //EPOC/DV3/task/2005/November/UI-Perf/master-mnt/graphics/ui_bench/scripts/uibench2db.pl
# #         uibench contact David Kren
# #         SMG Location of script //EPOC/development/sag/performance/scripts/uibench2db.pl
# #         The SMG contact for the above is Patrick Diamond ext 1316
# # Instructions for adding new performance testcase results
# #         1) If the result is in a new file then add its name to
# #            the global variable @testfiles_tef
# #         2) amend the function  process_build_tef_file to extract the performance data
# #         3) run this script with the parms --dir & --writecsv but without --writedb
# #            check the contents of the csv file
# #         4) To switch on debugging run on the cmd line "set DEBUG=2"
# #         If stuck after trying the above contact Patrick Diamond ext 1316
# # -- SQL Definitation
# # CREATE TABLE performance.performance_uibench_tests (
# #    team text,
# #    platform text,
# #    build text,
# #    testdate timestamp,
# #    testname text,
# #    resulttype text,
# #    result real);
# # GRANT INSERT,DELETE,SELECT ON TABLE performance.performance_uibench_tests to performance_uibench;
# # GRANT SELECT ON TABLE performance.performance_uibench_tests to public;
# # GRANT USAGE ON SCHEMA performance to performance_uibench;
# # CREATE  INDEX performance_uibench_tests_platform ON performance.performance_uibench_tests(platform);
# # CREATE  INDEX performance_uibench_tests_testname ON performance.performance_uibench_tests(testname text_pattern_ops);
# # COMMENT ON TABLE performance.performance_uibench_tests is 'Overnight performance test results for Graphics team, uibench results';
# ovgbenchmark.htm TESTEXECUTEfbstest_T_Performance.script.htm TESTEXECUTEscdvtest_t_performance.script.htm 
# 
#

use Getopt::Long;
use File::Basename;
use File::Find;
use strict;
use warnings;

my ($file,$help,$platform,$days,$writedb,$writecsv);
$days=2;

my @testfiles_tef = ('TESTEXECUTEte_uibench.Script.htm');
my @testfiles_rtest ;

GetOptions ('dir=s' => \$file,
            'platform|p=s' => \$platform,
            'days|d=s' => \$days,
            'writecsv' => \$writecsv,
            'writedb'  => \$writedb,
            'help|h|?' =>\$help) || usage();

usage(0) if $help;
usage(1,'dir')  if  not defined $file;
usage(1,'days')      if  not defined $days or $days !~ /^[0-9]+$/;
$file =~ s/\\/\//g; # swap all \ for / in file/dir name
my $DEBUG=0;
$DEBUG=$ENV{DEBUG} if exists $ENV{DEBUG};

my @files;

# if the path points to a file then use that
if ( -f $file ) {
  push @files,$file;
} else {
    # use the path as a dir path and search for build.html files
    print "Searching for build.html files\n" if $DEBUG;
    $file .= '/';          # ensure dir path ends in /
    $file =~ s/\/\/$/\//g; # ensure dir path doesn't end in //
    foreach my $f ("${file}build.html" , glob($file . '*/build.html')) {
        next if not defined $f;
        next if not -f $f;
        next if -M $f > $days;
        push @files,$f;
    }
}


# check that this machine has the perl drivers necessary to contact the database
check_db_drivers();

# DB connect
my $dbh;
$dbh = connect_to_db() if defined $writedb;

# Now extract test data from each build and call script to place data in database
foreach my $f (@files) {
  my $testdata = process_test_build($f);

  my $team='uibench';
  my $build = $testdata->{'build'};
  my $date  = $testdata->{'date'};
  my $platform = $testdata->{'platform'};
   
  $dbh->begin_work if defined $writedb;
  remove_duplicates ($dbh,$team,$build,$platform,$date) if defined $writedb;
  my $csv;
  open($csv,'>',dirname($f) . "/performance.csv") or die $! if defined $writecsv;

  # loop over each result and output it to db and csv
  foreach my $result (@{$testdata->{'results'}}) {
    next if not defined $result;
    next if not defined $result->{'value'} or $result->{'value'} eq '';
    
    process_sql($dbh,$team,$platform,$build,$date,$result->{'testname'},$result->{'type'},$result->{'value'}) if defined $writedb;
    print $csv "$team,$platform,$build,$date,\"$result->{'testname'}\",\"$result->{'type'}\",$result->{'value'}\n" if defined $writecsv;

    print "." if not $DEBUG;
  }
  
  $dbh->commit or die ("DB error : $dbh->errorstr") if defined $writedb;
  close($csv) if defined $writecsv;
  
  print "\n" if not $DEBUG;
}

#########################
# Define subroutines

# print out a usage message for this script and exit
sub usage {
  my ($r,$p) = @_;
  $r = 0 if not defined $r;

  print "uibench2db.pl \n";
  print "Error: Parameter \"--$p\" missing\n" if defined $p;

  print "    --dir=<location of the build.html file with the test results>\n";
  print "    --days=<filter on number of days since data(zip) file was modified>\n";
  print "    --help : this text\n";
  exit $r;
}

# Find and process each performance test case log
sub process_test_build {
    my ($file) = @_;
    print "Function process_test_build: $file\n" if $DEBUG;

    my $testdata = {'date' => undef,'platform' => undef, 'build' => undef, 'results' => []};
    my $dir = dirname($file);
    
    # find date,platform and build values for this set of test results    
    process_build_log($testdata,$file);

    # find and process rtest logs
    foreach my $f (@testfiles_rtest) {
        print "\tLooking for $f\n" if $DEBUG > 1;
        my ($logfile) = glob("$dir/*/$f");
        process_build_rtest_file($testdata,$logfile) if defined $logfile;
    }

    # find and process TEF logs
    foreach my $f (@testfiles_tef) {
        print "\tLooking for $f\n" if $DEBUG > 1;
        my ($logfile) = glob("$dir/*/$f");
        process_build_tef_file($testdata,$logfile) if defined $logfile;
    }

    return $testdata;
}

# Extract the platform and date from the build.html file
sub process_build_log {
    my ($test,$build_log) = @_;

    open (FH, "<", $build_log) or die "Error opening $file\n$!\n";
    my @lines = <FH>;
    close(FH);

    # extract the build date and platform 
    my ($date_line) =  grep {/Build report for build /} @lines;
    my ($year,$month,$day,$hour,$minute,$platform) = 
        ($date_line =~ /([0-9]{4})_([0-9][0-9])_([0-9][0-9])_([0-9][0-9])([0-9][0-9])_(.*?)\</);
    $test->{'date'} = "$year-$month-$day $hour:$minute";   

    # determine the platform name
    my @tags = split /_/,lc($platform);
    $test->{'platform'} = "$tags[1] $tags[0] $tags[2]";

    # extract the name of the build
    my ($build_line) =  grep {/Full OS Build Used:/} @lines;
    my ($build) = ($build_line =~ /Used:\s*(.*?)\.*\</);
    $test->{'build'} = $build;
}


# Extract the platform and date from the build.summary file
sub process_build_rtest_file {
    my ($testdata,$file) = @_;

    print "Function process_build_rtest_file : $file\n" if $DEBUG;
    my $data = read_file($file);
  	1 while $data =~ s/\<.*?\>//g; # strip all html tags
		
    my ($txt,$state) = ('','');
    foreach my $line (split /\n/,$data) {
        chomp($line);
  		$line =~ s/[[:cntrl:]]//g; # strip ctl chars
        if ($line =~ /^RTEST TITLE:\s*([a-z]+.*?)\s*$/i) {
            # start of new test case
            $state = $1;
            $state =~ s/\s*[0-9\.\(\)]+$//; # strip numeric suffix
            $txt = '';
        } elsif ($state ne '' and $line =~ /^RTEST: SUCCESS\s*:/i) {
            # end of test case
            $txt .= "$line\n";            
            process_build_rtest_text($txt, $testdata->{'results'},$file,$state);
            $txt = '';
            $state = '';
        } elsif ($state eq '' and $line =~ /^\s*RTEST:\s+Level\s+[0-9]+\s+Next\s+test\s+.\s+(.*?)\s*$/i) {
            # start of new test case where the title hasn't been used
            # e.g. RTEST: Level  001 Next test - Defect tests
            $state = $1;
            $txt = '';
        }
        $txt .= "$line\n" if $state ne '';
    }
}

# Extract test performance data frome each TEF file
sub process_build_tef_file {
    my ($testdata,$file) = @_;

    print "Function process_build_tef_file : $file\n" if $DEBUG;
    my $result = $testdata->{'results'};
    my $data = read_file($file);
  	1 while $data =~ s/\<.*?\>//g; # strip all html tags
	
		
    my ($txt,$state) = ('','');
    foreach my $line (split /\n/,$data) {
        chomp($line);
  		$line =~ s/[[:cntrl:]]/ /g; # strip ctl chars
  		$txt .= $line;
  		$txt .= "\n";
    }

    # process results from TESTEXECUTEte_uibench.script.htm
    if ($file =~ /TESTEXECUTEte_uibench.Script.htm/i) {

        my %tests;

        # Format 1 : Result displayed in microseconds - Max and Min available
        while ($txt =~ /TID:\s+([^\s]+?)\s+Rot:\s+([0-9]+)\s+SrcMode:\s+([0-9]+)\s+DestMode:\s+([0-9]+)\s+Iters:\s+([0-9]+)\s+TrimmedMean:\s+([0-9]+)\s+us.*?\s+Max:\s+([0-9]+)\s+Min:\s+([0-9]+)\s+/gms) {
            my ($testname,$rot,$srcmode,$destmode,$iters,$microseconds,$max,$min) = ($1,$2,$3,$4,$5,$6,$7,$8);
            next if not defined $max or not defined $min;
            $testname = sprintf('%s,Rot:%1d,SrcMode:%2d,DestMode:%2d',$testname,$rot,$srcmode,$destmode);
            $tests{$testname} = 1; # record the fact that this test has been processed
            push @$result, {'testname' => "$testname,Mean", 'value' => $microseconds, 'type' => 'microseconds'};
            #push @$result, {'testname' => "$testname,Max", 'value' => $max, 'type' => 'microseconds'};
            #push @$result, {'testname' => "$testname,Min", 'value' => $min, 'type' => 'microseconds'};
            print "\t$testname:Mean\t$microseconds\t$microseconds\n" if $DEBUG > 1;
            print "\t$testname:Max\t$max\t$max\n" if $DEBUG > 1;
            print "\t$testname:Min\t$min\t$min\n" if $DEBUG > 1;
        }

        # Format 2 : Result displayed in microseconds - Max and Min not available
        while ($txt =~ /TID:\s+(.*?)\s+Rot:\s+([0-9]+)\s+SrcMode:\s+([0-9]+)\s+DestMode:\s+([0-9]+)\s+Iters:\s+([0-9]+)\s+TrimmedMean:\s+([0-9]+)\s+us/g) {
            my ($testname,$rot,$srcmode,$destmode,$iters,$microseconds) = ($1,$2,$3,$4,$5,$6);
            $testname = sprintf('%s,Rot:%1d,SrcMode:%2d,DestMode:%2d',$testname,$rot,$srcmode,$destmode);
            next if exists $tests{$testname} ;# do not process this test again
            push @$result, {'testname' => "$testname,Mean", 'value' => $microseconds, 'type' => 'microseconds'};
            print "\t$testname:Mean\t$microseconds\t$microseconds\n" if $DEBUG > 1;
        }

        # Format 3 : Result displayed in pixels/second - Max and Min available
        while ($txt =~ /TID:\s+([^\s]+?)\s+Rot:\s+([0-9]+)\s+SrcMode:\s+([0-9]+)\s+DestMode:\s+([0-9]+)\s+Iters:\s+([0-9]+)\s+TrimmedMean:\s+([0-9]+)\s+pixels\/second.*?\s+Max:\s+([0-9]+)\s+Min:\s+([0-9]+)\s+/gms) {
            my ($testname,$rot,$srcmode,$destmode,$iters,$pps,$max,$min) = ($1,$2,$3,$4,$5,$6,$7,$8);
            next if not defined $max or not defined $min;
            $testname = sprintf('%s,Rot:%1d,SrcMode:%2d,DestMode:%2d',$testname,$rot,$srcmode,$destmode);
            $tests{$testname} = 1; # record the fact that this test has been processed
            push @$result, {'testname' => "$testname,Mean", 'value' => $pps, 'type' => 'pixels/second'};
            #push @$result, {'testname' => "$testname,Max", 'value' => $max, 'type' => 'microseconds'};
            #push @$result, {'testname' => "$testname,Min", 'value' => $min, 'type' => 'microseconds'};
            print "\t$testname:Mean\t$pps\t$pps\n" if $DEBUG > 1;
            print "\t$testname:Max\t$max\t$max\n" if $DEBUG > 1;
            print "\t$testname:Min\t$min\t$min\n" if $DEBUG > 1;
        }

        # Format 4 : Result displayed in pixels/second - Max and Min not available
        while ($txt =~ /TID:\s+(.*?)\s+Rot:\s+([0-9]+)\s+SrcMode:\s+([0-9]+)\s+DestMode:\s+([0-9]+)\s+Iters:\s+([0-9]+)\s+TrimmedMean:\s+([0-9]+)\s+pixels\/second/g) {
            my ($testname,$rot,$srcmode,$destmode,$iters,$pps) = ($1,$2,$3,$4,$5,$6);
            $testname = sprintf('%s,Rot:%1d,SrcMode:%2d,DestMode:%2d',$testname,$rot,$srcmode,$destmode);
            next if exists $tests{$testname} ;# do not process this test again
            push @$result, {'testname' => "$testname,Mean", 'value' => $pps, 'type' => 'pixels/second'};
            print "\t$testname:Mean\t$pps\t$pps\n" if $DEBUG > 1;
        }

		# Format 5 : Result displayed in characters/second - Max and Min available
        while ($txt =~ /TID:\s+([^\s]+?)\s+Rot:\s+([0-9]+)\s+SrcMode:\s+([0-9]+)\s+DestMode:\s+([0-9]+)\s+Iters:\s+([0-9]+)\s+TrimmedMean:\s+([0-9]+)\s+characters\/second.*?\s+Max:\s+([0-9]+)\s+Min:\s+([0-9]+)\s+/gms) {
            my ($testname,$rot,$srcmode,$destmode,$iters,$cps,$max,$min) = ($1,$2,$3,$4,$5,$6,$7,$8);
            next if not defined $max or not defined $min;
            $testname = sprintf('%s,Rot:%1d,SrcMode:%2d,DestMode:%2d',$testname,$rot,$srcmode,$destmode);
            $tests{$testname} = 1; # record the fact that this test has been processed
            push @$result, {'testname' => "$testname,Mean", 'value' => $cps, 'type' => 'characters/second'};
            #push @$result, {'testname' => "$testname,Max", 'value' => $max, 'type' => 'microseconds'};
            #push @$result, {'testname' => "$testname,Min", 'value' => $min, 'type' => 'microseconds'};
            print "\t$testname:Mean\t$cps\t$cps\n" if $DEBUG > 1;
            print "\t$testname:Max\t$max\t$max\n" if $DEBUG > 1;
            print "\t$testname:Min\t$min\t$min\n" if $DEBUG > 1;
        }

        # Format 6 : Result displayed in characters/second - Max and Min not available
        while ($txt =~ /TID:\s+(.*?)\s+Rot:\s+([0-9]+)\s+SrcMode:\s+([0-9]+)\s+DestMode:\s+([0-9]+)\s+Iters:\s+([0-9]+)\s+TrimmedMean:\s+([0-9]+)\s+characters\/second/g) {
            my ($testname,$rot,$srcmode,$destmode,$iters,$cps) = ($1,$2,$3,$4,$5,$6);
            $testname = sprintf('%s,Rot:%1d,SrcMode:%2d,DestMode:%2d',$testname,$rot,$srcmode,$destmode);
            next if exists $tests{$testname} ;# do not process this test again
            push @$result, {'testname' => "$testname,Mean", 'value' => $cps, 'type' => 'characters/second'};
            print "\t$testname:Mean\t$cps\t$cps\n" if $DEBUG > 1;
        }
    }
}


# process the text associated with 1 testcase and extract performace results
sub process_build_rtest_text {
    my ($txt, $result, $logfile, $testname) = @_;
    print "Function process_build_rtest_text: $logfile, $testname\n" if $DEBUG > 1;

    next if not defined $testname or $testname =~ /^\s*$/;
    $testname =~ s/[-]/:/g; # replace - with : as subtitle seperator
    $testname =~ s/\s*performance test\s*$//; # strip unnecessary suffix to test name
    print "$testname \n" if $DEBUG > 1;    

    if ($testname eq 'T_BENCH') {
        while ($txt =~ /^\s*(.*?):\s*(.*?)\s+ms$/gim) {
            my ($subname,$num) = ($1,$2);
            push @$result, {'testname' => "$testname:$subname", 'value' => $num, 'type' => 'milliseconds'};
            print "\t$testname:$subname\t$num\tmilliseconds\n" if $DEBUG > 1;
        } 

    } 
   return $result;
}

# return a connection to the databasse
sub connect_to_db {
  print "Connecting to DB\n" if $DEBUG;
  my $dbh = DBI->connect('DBI:PgPP:dbname=metadata;host=smglinx.symbian.intra', 'performance_uibench', 'grui');
  print "Connected to DB\n" if $DEBUG;
  return $dbh;
}

# save the result to the database
sub process_sql {
  my ($dbh,$team,$platform,$build,$testdate,$testname,$resulttype,$result) = @_;

  print "Processing $testname result\n" if $DEBUG > 1;
  if ($DEBUG > 1) {
    shift;
    print 'Saving ' . join(',',@_) . "\n";
  }
  my $sth = $dbh->prepare("INSERT INTO performance.performance_uibench_tests (team,platform,build,testdate,testname,resulttype,result) VALUES(?,?,?,?,?,?,?)") or die "Prepare Error $dbh->errorstr";

  $sth->execute($team,
		$platform,
		$build,
		$testdate,
		$testname,
		$resulttype,
		$result)
      or die ("Error inserting values $team,$platform,$build,$testdate,$testname,$resulttype,$result\n $dbh->errorstr");

  print "Saved result\n" if $DEBUG > 1;
}

# read in and return the contents of a file
sub read_file {
    my ($file) = @_;

    open (FH,"<",$file) or die "Error reading from $file\n$!\n";
    my $file_size = (stat ($file))[7];    # Size of file
    my $data;
    read(FH, $data, $file_size);
    close(FH);

    return $data;
}

# before saving to the database delete any duplicate data
sub remove_duplicates {
  my ($dbh,$team,$build,$platform,$date) = @_;

  my $sth = $dbh->prepare("delete from performance.performance_uibench_tests where team = ? and build = ? and platform = ?  and testdate = ?") or die "Prepare Error $dbh->errorstr";

  $sth->execute($team, $build, $platform, $date)
      or die ("Error deleteing  values $team,$build\n $dbh->errorstr");

}


# ensure that the modules necessary to drive the database interactions are available and loaded
sub check_db_drivers {

    print "Checking for DBI module\n" if $DEBUG;
    eval{require DBI};
    if($@) {
      print "Failed to load DBI\n Now attempting to download and install\nppm install DBI\n";
      system("ppm","install","DBI");
      require DBI;
    }

    print "Checking for DBD::PgPP database driver module\n" if $DEBUG;
    eval{require DBD::PgPP};
    if($@) {
      print "Failed to load DBD::PgPP\n Now attempting to download and install\nppm install DBD-PgPP\n";
      system("ppm","install","DBD-PgPP");
      require DBD::PgPP;
    }
}

sub find_performance_files {
    if ($File::Find::name 
	=~ /\/t_performance.txt/i) {
        my $afile = lc($File::Find::name);
  	    $afile =~ s/\\/\//g;
   	    push  @files, lc($afile);
    	print "Found $afile\n" if $DEBUG;
	   print "+" if not $DEBUG;
    }
}
