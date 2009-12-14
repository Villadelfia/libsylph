#!/usr/bin/perl -w

use strict;
use Switch;
use File::Basename;

my $_filename;
my $_lineno = 0;
my $_output;
my $_classname;
my $_buffer;
my $_funcbuffer = "";
my $_varbuffer = "";
my $_funclist = "";
my $_headerset = 0;
# 0 = default/reading, 1 = header, 2 = function/setup/teardown, 
# 3  = vars
my $_state = 0;
# buffer to store everything in until state change
my $_smallbuf = "";
# list of acceptable commands
my @_commands = qw(.Header .SetUp .TearDown .Vars .End);
my %_declared = ();

sub error {
    my $msg = shift;
    print STDERR "error: $msg\n";
    exit 1;
}
sub errorl {
    my $lineno = shift;
    my $msg = shift;
    error("$_filename:$lineno: $msg");
}

sub warning {
    my $msg = shift;
    print STDERR "warning: $msg\n";
}

sub warningl {
    my $lineno = shift;
    my $msg = shift;
    warning("$_filename:$lineno: $msg");
}

sub writebuffer {
    open(COUT,">",$_output) or error $!;
    print COUT $_buffer;
    close(COUT);
}

sub flushsmallbuf {
    switch($_state) {
        # header
        case 1 {
            # append to buffer
            $_buffer .= $_smallbuf;
        }
        # function
        case 2 {
            # append }
            $_smallbuf .= "}\n\n";
            # append to funcbuffer
            $_funcbuffer .= $_smallbuf;
        }
        # variable
        case 3 {
            # append to varbuffer
            $_varbuffer .= $_smallbuf;
        }
    }
    # reset state and smallbuf
    $_smallbuf = "";
    $_state = 0;
}

sub special {
    my $line = shift;
    if($_state == 0 && $_smallbuf ne "") {
        errorl($_lineno, "Filler not in section");
    }
    
    if(!($line =~ m/^@\.?[[:alnum:]_]+:.*$/) && !($line eq "@.End")) {
        errorl($_lineno, "Invalid \@-section identifier: `$line'");
    }
    
    #my command is your line
    $line =~ m/^@(\.?[[:alnum:]]+):?/;
    my $command = $1;
    
    #check for invalid command
    if($command =~ m/^\./) {
        my @match = grep { $_ eq $command } @_commands;
        if(scalar @match != 1) {
            errorl $_lineno, "Invalid command: `$command'";
        }
    }
    
    #check for non-strict syntax
    if($_state != 0) {
        if($command ne ".End") {
            warningl $_lineno, "New \@-section without closing \@.End";
            warningl $_lineno, "Assuming implicit \@.End";
        }
        flushsmallbuf;
    }
    
    #find what command we're actually going to use...
    switch ($command) {
        case ".Header" {
            if($_headerset) {
                errorl $_lineno, "Double declaration of `\@.Header' section";
            }
            $_state = 1;
            $_headerset = 1;
            $_buffer .= "#line $_lineno \"$_filename\"\n";
            return;
        }
        case ".SetUp" {
            if($_declared{".SetUp"}) {
                errorl $_lineno, "Double declaration of `\@.SetUp' section";
            }
            $_state = 2;
            $_declared{".SetUp"} = 1;
            $_funcbuffer .= "#line $_lineno \"$_filename\"\nvoid setUp() {\n";
            return;
        }
        case ".TearDown" {
            if($_declared{".TearDown"}) {
                errorl $_lineno,"Double declaration of `\@.TearDown' section";
            }
            $_state = 2;
            $_declared{".TearDown"} = 1;
            $_funcbuffer .= "#line $_lineno \"$_filename\"\nvoid tearDown() {\n";
            return;
        }
        case ".Vars" {
            if($_varbuffer ne "") {
                errorl $_lineno, "Double declaration of `\@.Vars' section";
            }
            $_state = 3;
            $_varbuffer = "\n#line $_lineno \"$_filename\"\n";
            return;
        }
        case ".End" {
            #okay, done
            return;
        }
    }
    
    # no special command... Then it must be a function name...
    if($_declared{$command}) {
        errorl $_lineno, "Double declaration of `$command' testcase";
    }
    if($command eq "setUp") {
        errorl $_lineno, "Tried to create a testcase with name `setUp'"
    }
    if($command eq "tearDown") {
        errorl $_lineno, "Tried to create a testcase with name `tearDown'"
    }
    # everything OK!
    $_state = 2;
    $_declared{$command} = 1;
    $_funcbuffer .= "#line $_lineno \"$_filename\"\nvoid $command() {\n";
    
    # check for extra attrs...   
    if($line =~ m/:\s*throw\(\s*([[:alnum:]:_]+)\s*\)$/) {
        $_funclist .= "CPPUNIT_TEST_EXCEPTION($command,$1);\n";
    } else {
        $_funclist .= "CPPUNIT_TEST($command);\n";
    }
}

sub parse {
    my $classname = $_classname;
    $classname =~ s/\.cpp$//;
    $classname = "Test" . ucfirst $classname;
    
    $_buffer = <<"EOT";
/* Generated by mktest.pl. Do not modify by hand! */

// include CPPUnit header
#include <cppunit/extensions/HelperMacros.h>   

// @.Header 
EOT
    
    open(CIN, "<", $_filename) or error $!;
    while(my $line = <CIN>) {
        $_lineno++;
        #test if $line is a special marker
        my $chompline = $line;
        chomp $chompline;
        if($chompline =~ m/^@.*/) { 
            special $chompline;
        }
        
        # nop, copy in to out.
        else { 
            if(!($line eq "\n") || $_state != 0) { 
                $_smallbuf .= "$line";
            }    
        }
    }
    #okay, so we are done. Now let's put everything togheter...
    #$_buffer should contain the header...
    #so, add the class declaration
    $_buffer .= <<"EOT";

class $classname : public CppUnit::TestFixture {
// Function list
CPPUNIT_TEST_SUITE($classname);
EOT
    #add the function lists
    $_buffer .= $_funclist;
    
    #add the functions
    $_buffer .= <<"EOT";
CPPUNIT_TEST_SUITE_END();

// Test cases
public:
EOT
    $_buffer .= $_funcbuffer;
    
    #add the vars
    $_buffer .= "// Variable list\nprivate:$_varbuffer";
    
    #finish off...
    $_buffer .= <<"EOT";

};

// register test suite with CppUnit
CPPUNIT_TEST_SUITE_REGISTRATION($classname);
EOT
    #done!
    close(CIN);
}

sub main {
    #first check for args...
    if($#ARGV != 1 or $ARGV[0] =~ m/^-{1,2}h(elp)?$/) {
        printf STDERR "Usage: mktest.pl <in> <out>" ;
        exit 1;
    }
    $_filename = $ARGV[0];
    
    #check if filename is a testcase (tst)
    if(not $_filename =~ m/.*\.tst$/) {
        warning "$_filename does not have .tst extension -- is it testcase?" ;
    }
    
    #get the name of the output file
    $_output = basename $_filename;
    $_output =~ s/\..*?$/.cpp/;
    $_classname = $_output;
    $_output = $ARGV[1] . "/" . $_output;
    parse();
    writebuffer();
}

main;
