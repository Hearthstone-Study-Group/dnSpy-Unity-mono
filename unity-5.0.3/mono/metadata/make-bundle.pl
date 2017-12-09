#!/usr/bin/perl -w
# Copyright (C) 2003 Ximian, Inc.
# Paolo Molaro (lupus@ximian.com)
#
# Create an header file to be included in the mono libraries to
# bundle assemblies inside the runtime.
# The template file contains a list of assemblies, one per line,
# with the name followed by a ':' and the filename.
# Lines starting with '#' and empty lines are ignored.
# See sample-bundle for an example.
# We need to use an assembly file because gcc can't handle large arrays:-(

if ($#ARGV != 2) {
	die "Usage: make-bundle.pl template headerfile.h asm-file\n";
}

my $template = $ARGV [0];
my $header = $ARGV [1];
my $output = $ARGV [2];
my %assemblies = ();

my $line = 0;
open (T, $template) || die "Cannot open bundle template: $!\n";
while (<T>) {
	++$line;
	next if (/^\s*#/);
	next if (/^\s*$/);
	if (/^([a-zA-Z0-9-.]+):\s*(.+?)\s*$/) {
		my ($name, $filename) = ($1, $2);
		if (exists $assemblies {$name}) {
			die "Assembly $name defined multiple times.\n";
		} else {
			$assemblies {$name} = $filename;
		}
	} else {
		die "Unknown format at line $line: $_";
	}
}
close (T);

open (O, ">$output.tmp") || die "Cannot open $output: $!\n";
open (H, ">$header.tmp") || die "Cannot open $output: $!\n";
print H <<"EOF";
/* File generated by make-bundle: do not edit! */

#ifndef __MONO_BUNDLE_H__
#define __MONO_BUNDLE_H__

typedef struct {
	const char *name;
	const unsigned char *data;
	const unsigned int size;
} MonoBundledAssembly;

EOF

my $bundle_entries = "";

foreach my $name (sort keys %assemblies) {
	my $file = $assemblies {$name};
	my ($nread, $buf, $i, $cname, $need_eol, $size);
	$cname = $name;
	$cname =~ s/[-.]/_/g;
	open (F, $file) || die "Cannot open $file: $!\n";
	$size = -s F;
#	print O "/* assembly $name from $file */\n";
#	print O "static const unsigned char assembly_data_$cname [] = {\n";
	print O ".globl assembly_data_$cname\n";
	print O "\t.section .rodata\n";
	print O "\t.align 32\n";
	print O "\t.type assembly_data_$cname, \@object\n";
	print O "\t.size assembly_data_$cname, $size\n";
	print O "assembly_data_$cname:\n";
	print H "extern const unsigned char assembly_data_$cname [];\n";
	print H "static const MonoBundledAssembly assembly_bundle_$cname = {\"$name\", assembly_data_$cname, $size};\n";
	$bundle_entries .= "\t&assembly_bundle_$cname,\n";
	$need_eol = 0;
	print "Adding assembly '$name' from $file...\n";
	while (($n = sysread (F, $buf, 32))) {
		for ($i = 0; $i < $n; ++$i) {
			print O "\t.byte ", ord (substr ($buf, $i, 1)), "\n";
		}
#		print O ",\n" if ($need_eol);
#		$need_eol = 1;
#		print O "\t";
#		for ($i = 0; $i < $n; ++$i) {
#			print O ", " if $i > 0;
#			print O ord (substr ($buf, $i, 1));
#		}
	}
#	print O "\n};\n\n";
	close (F);
}

print H "\nstatic const MonoBundledAssembly* bundled_assemblies [] = {\n";
print H $bundle_entries;
print H "\tNULL\n";
print H "};\n\n";
print H "#endif /* __MONO_BUNDLE_H__ */\n";
close (O);
close (H);
rename ("$header.tmp", $header);
rename ("$output.tmp", $output);

