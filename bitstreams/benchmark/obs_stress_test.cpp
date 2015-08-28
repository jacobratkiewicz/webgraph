/*               
 * Portions copyright (c) 2003-2007, Paolo Boldi and Sebastiano Vigna. Translation copyright (c) 2007, Jacob Ratkiewicz
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include "../output_bitstream.hpp"
#include "../../utils/fast.hpp"

#include <vector>
#include <boost/program_options.hpp>

/**
 * This module's job is to stress-test the output bitstream stuff by
 * calling random methods a large number of times. It saves both its
 * call sequence and results in a file for comparison with the java
 * version.
 */

/* Have to balance two competing goals here: 
 *
 * 1) Similarity to real world. calling write_unary 1000 times and
 * then write_gamma 1000 times does not have this property.  
 * 2) Fairness. Want both languages to have a fighting chance.
 */

using namespace std;

enum obs_member_enum_t {
  WRITE_UNARY, WRITE_GAMMA, WRITE_DELTA, WRITE_ZETA, NUM_OBS_FUNS;
};


char NAMES[] = {
  'U',
  'g',
  'd',
  'z'
};

vector<obs_member_enum_t> generate_call_sequence( int length ) {
  srand();

  vector<obs_member_enum_t> retval( length );

  for( int i = 0; i < length; i++ ) {
    retval[i] = obs_member_enum_t(rand() % NUM_OBS_FUNS);
  }

  return retval;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * main method
 */

int main( int ac, char** av ) {

  namespace po = boost::program_options;

  int size;
  string output_basename;

  po::options_description desc( "obs stress test usage:" );
  desc.add_options()
    ( "help,h", "Print this message" )
    ( "size,s", po::value<int>(size), "Number of calls in the stress test." )
    ( "output,o", po::value<string>(output_basename), "Basename of output files." );

  po::variables_map vm;
  po::store( po::parse_command_line( ac, av, desc ), vm );
  po::notify(vm);

  if( vm.count( "help" ) || !vm.count( "size" ) || !vm.count( "output" ) ) {
    cerr << desc << endl;

    return 1;
  }

  cerr << "Generating call sequence...";
  vector<obs_member_enum_t> call_sequence = generate_call_sequence( size );
  vector<int> args( size );

  obitstream obs( output_basename + ".data" );

  // start timer

  for( int i = 0; i < size; ++i ) {
    int arg = rand();

    switch( *i ) {
    case WRITE_UNARY:
      args[i] = arg;
      obs.write_unary( arg );
      break;
    case WRITE_GAMMA:
      args[i] = arg;
      obs.write_gamma( arg );
      break;
    case WRITE_DELTA:
      args[i] = arg;
      obs.write_delta( arg );
      break;
    case WRITE_ZETA:
      args[i] = arg;
      obs.write_zeta( arg, 5 );
      break;
    default:
      abort();
    }
  }

  // stop timer
  
  // dump the call sequence
  ofstream calls ( output_basename + ".calls" );

  cerr << "Dumping call sequence to " << output_basename << ".calls" << endl;

  for( int i = 0; i < size; i++ ) {
    calls << NAMES[ call_sequence[i] ] << " " << args[ i ] << "\n";
  }

  return 0;
}



}
