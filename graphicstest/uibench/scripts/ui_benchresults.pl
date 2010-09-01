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
# This script extracts the data from a uibench.htm file
# 
#

my $htm=$ARGV[0];
print "Parsing $htm, time, unit, rot, src, dst\n";
my $testdata = process_test_log2($htm,'st_perf_');
foreach my $result (@$testdata) {
	next if defined $testname and $result->{'testname'} ne $testname;
	if ($ARGV[1] eq '--notestname') {
		print "$result->{'dura'}\n";
	} elsif ($ARGV[1] eq '-tab') {
		print "$result->{'testname'}	$result->{'dura'}	$result->{'unit'}	$result->{'rota'}	$result->{'smode'}	$result->{'dmode'}\n";
	} else {
		print "$result->{'testname'},$result->{'dura'},$result->{'unit'},$result->{'rota'},$result->{'smode'},$result->{'dmode'}\n";
	}
}
exit;

sub process_test_log2 {
    my ($file,$prefix) = @_;
    print "Processing $file\n" if $DEBUG;
    open(LOG,$file) or die "Error opening $file\n$!\n";

    my $result = [];
    my ($name,$start,$end);

    while (my $line = <LOG>) {
	
	if ($line =~ /[0-9:]{12}\s+.+?\s+TID:\s*/) {
		($prt1,$prt2)=split(/\s+TID:\s+/,$line);
		($tname,$prt3)=split(/\s+Rot:\s+/,$prt2);
		($rotation,$x1,$srcMode,$x2,$dstMode,$x3,$iters,$x5,$duration, $unit)=split(/\s+/,$prt3);		
		push @$result, {'testname' => $tname, 'dura' => $duration, 'rota' => $rotation, 'smode' => $srcMode, 'dmode' => $dstMode, 'unit' => $unit};
		}
    }
    return $result;
}

