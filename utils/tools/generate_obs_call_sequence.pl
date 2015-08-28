#!/usr/bin/perl -w

# Generates a call  sequence, in either Perl or C++, given log data for OBS.

my $java_preamble = <<DONE;
import it.unimi.dsi.mg4j.io.*;
import java.io.*;

/**
 * Test harness for output bitstream
 */
public class OutputBitstreamTest {
    public static void main( String[] args ) throws Exception {
        PrintStream ps = new PrintStream( new BufferedOutputStream( new FileOutputStream( "java_obs_log" )));

        byte[] dat = new byte[800];

        DebugOutputBitStream obs = new DebugOutputBitStream( new OutputBitStream( dat ), ps );

        // Call a bunch of methods.

        PrintWriter out = new PrintWriter(ps);
DONE


my $java_postamble = <<DONE;
        out.close();
    }

    public static void logArray( byte[] ary, PrintWriter pw ) {
        pw.print( "\t\t" );
        
        for( int i = 0; i < ary.length; i++ ) {
            String str = Integer.toHexString( ary[i] );
            // only the last two digits count.
            str = str.substring( (int)Math.max(str.length() - 2, 0), str.length() );

            pw.print( str + " " );
        }

        pw.println();

        pw.flush();
    }
}
DONE
# emacs doesnt know how to do format tags
    ;
my $cpp_preamble = <<DONE;
#include "../output_bitstream.hpp"
#include "debug_obitstream.hpp"
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

using namespace std;

void log_array( const vector<unsigned char>& ary, ostream& log );

/**
 * Test harness for output bitstream
 */
int main( int argc, char** argv ) {
//   ofstream log( "cpp_obs_log");

//   boost::shared_ptr<vector<unsigned char> > dat( new vector<unsigned char>(1000) );
   
   webgraph::obitstream obs( "obs_dump.dat" );

//   webgraph::debug_obitstream obs( obs_underlying, log );

   // Call a bunch of methods.
DONE

my $cpp_postamble = <<DONE;
   return 0;
}

void log_array( const vector<unsigned char>& ary, ostream& log ) {
   log << "\t\t";
        
   for( unsigned i = 0; i < ary.size(); i++ ) {
      log << hex << (int)ary[i] << " ";
   }

   log << endl;
}                                            
DONE

my %java_tbl = (
    'preamble' => sub { return $java_preamble; },
    'postamble' => sub { return $java_postamble; },
    'z' => sub { my ($a, $p) = @_; return "writeZeta( $a, $p );"; },
    'g' => sub { my ($a, $p) = @_; return "writeGamma( $a );"; },
    'U' => sub { my ($a, $p) = @_; return "writeUnary( $a );"; },
    'log' => sub { return "logArray( dat, out );"; }
    );

my %cpp_tbl = (
    'preamble' => sub { return $cpp_preamble; },
    'postamble' => sub { return $cpp_postamble; },
    'z' => sub { my ($a, $p) = @_; return "write_zeta( $a, $p );"; },
    'g' => sub { my ($a, $p) = @_; return "write_gamma( $a );"; },
    'U' => sub { my ($a, $p) = @_; return "write_unary( $a );"; },
    'log' => sub { return "log_array( *dat, log );"; }
    );

my ($infile_name)  = @ARGV;

die if not defined $infile_name;

open my $infile, $infile_name or die;

my $tbl;

if( $infile_name =~ /java/i ) {
    $tbl = \%java_tbl;
} elsif( $infile_name =~ /cpp/i ) {
    $tbl = \%cpp_tbl;
} else {
    die "invalid translation";
}

print $tbl->{'preamble'}->();

while( my $nextline = <$infile> ) {
    next if $nextline !~ /\{(.)(.*):(.*)\}/;

    my ($op, $param, $arg) = ($1, $2, $3);

    print "obs.", $tbl->{$op}->( $arg, $param ), "\n";
#    print $tbl->{'log'}->(), "\n";
}

print $tbl->{'postamble'}->();

