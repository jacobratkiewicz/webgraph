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
   ofstream log( "cpp_obs_log");

   boost::shared_ptr<vector<unsigned char> > dat( new vector<unsigned char>(5) );

   webgraph::obitstream obs_underlying( dat );

   webgraph::debug_obitstream obs( obs_underlying, log );

   // Call a bunch of methods.
   // obs.write_gamma( 5 );
// log_array( *dat, log );
// obs.write_unary( 0 );
// log_array( *dat, log );
// obs.write_gamma( 0 );
// log_array( *dat, log );
// obs.write_zeta( 44, 5 );
// log_array( *dat, log );
// obs.write_zeta( 15, 5 );
// log_array( *dat, log );
// obs.write_zeta( 33, 5 );
// log_array( *dat, log );
// obs.write_zeta( 2, 5 );
// log_array( *dat, log );
// obs.write_zeta( 12, 5 );
// log_array( *dat, log );
   obs.write_gamma( 1 );
   log_array( *dat, log );
   obs.write_unary( 0 );
   log_array( *dat, log );
   obs.write_gamma( 0 );
   log_array( *dat, log );
   obs.write_zeta( 168, 5 );
   log_array( *dat, log );
}

void log_array( const vector<unsigned char>& ary, ostream& log ) {
   log << "		";
        
   for( unsigned i = 0; i < ary.size(); i++ ) {
      log << hex << (int)ary[i] << " ";
   }

   log << endl;
}                                            
