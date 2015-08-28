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
//   ofstream log( "cpp_obs_log");

//   boost::shared_ptr<vector<unsigned char> > dat( new vector<unsigned char>(1000) );
   
   webgraph::obitstream obs( "obs_dump.dat" );

//   webgraph::debug_obitstream obs( obs_underlying, log );

   // Call a bunch of methods.
obs.write_gamma( 5 );
obs.write_unary( 0 );
obs.write_gamma( 0 );
obs.write_zeta( 44, 5 );
obs.write_zeta( 15, 5 );
obs.write_zeta( 33, 5 );
obs.write_zeta( 2, 5 );
obs.write_zeta( 12, 5 );
obs.write_gamma( 1 );
obs.write_unary( 0 );
obs.write_gamma( 0 );
obs.write_zeta( 168, 5 );
obs.write_gamma( 12 );
obs.write_unary( 0 );
obs.write_gamma( 0 );
obs.write_zeta( 6, 5 );
obs.write_zeta( 5, 5 );
obs.write_zeta( 19, 5 );
obs.write_zeta( 7, 5 );
obs.write_zeta( 7, 5 );
obs.write_zeta( 0, 5 );
obs.write_zeta( 5, 5 );
obs.write_zeta( 17, 5 );
obs.write_zeta( 5, 5 );
obs.write_zeta( 2, 5 );
obs.write_zeta( 14, 5 );
obs.write_zeta( 2, 5 );
obs.write_gamma( 17 );
obs.write_unary( 0 );
obs.write_gamma( 0 );
obs.write_zeta( 8, 5 );
obs.write_zeta( 0, 5 );
obs.write_zeta( 1, 5 );
obs.write_zeta( 4, 5 );
obs.write_zeta( 5, 5 );
obs.write_zeta( 1, 5 );
obs.write_zeta( 0, 5 );
obs.write_zeta( 16, 5 );
obs.write_zeta( 2, 5 );
obs.write_zeta( 0, 5 );
obs.write_zeta( 5, 5 );
obs.write_zeta( 0, 5 );
obs.write_zeta( 11, 5 );
obs.write_zeta( 16, 5 );
obs.write_zeta( 1, 5 );
obs.write_zeta( 11, 5 );
obs.write_zeta( 0, 5 );
obs.write_gamma( 15 );
obs.write_unary( 1 );
obs.write_gamma( 8 );
obs.write_gamma( 0 );
obs.write_gamma( 3 );
obs.write_gamma( 0 );
obs.write_gamma( 0 );
obs.write_gamma( 1 );
obs.write_gamma( 0 );
obs.write_gamma( 0 );
obs.write_gamma( 5 );
obs.write_gamma( 0 );
obs.write_zeta( 4, 5 );
obs.write_zeta( 15, 5 );
obs.write_zeta( 4, 5 );
obs.write_zeta( 8, 5 );
obs.write_zeta( 2, 5 );
obs.write_zeta( 3, 5 );
obs.write_zeta( 28, 5 );
obs.write_zeta( 4, 5 );
obs.write_zeta( 6, 5 );
obs.write_zeta( 6, 5 );
obs.write_gamma( 13 );
obs.write_unary( 0 );
obs.write_gamma( 0 );
obs.write_zeta( 7, 5 );
obs.write_zeta( 6, 5 );
obs.write_zeta( 1, 5 );
obs.write_zeta( 0, 5 );
obs.write_zeta( 5, 5 );
obs.write_zeta( 0, 5 );
obs.write_zeta( 32, 5 );
obs.write_zeta( 2, 5 );
obs.write_zeta( 4, 5 );
obs.write_zeta( 12, 5 );
obs.write_zeta( 14, 5 );
obs.write_zeta( 3, 5 );
obs.write_zeta( 4, 5 );
obs.write_gamma( 10 );
obs.write_unary( 0 );
obs.write_gamma( 0 );
obs.write_zeta( 12, 5 );
obs.write_zeta( 4, 5 );
obs.write_zeta( 17, 5 );
   return 0;
}

void log_array( const vector<unsigned char>& ary, ostream& log ) {
   log << "		";
        
   for( unsigned i = 0; i < ary.size(); i++ ) {
      log << hex << (int)ary[i] << " ";
   }

   log << endl;
}                                            
