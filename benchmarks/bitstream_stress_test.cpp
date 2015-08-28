
/*               
 * Copyright (c) 2007, Jacob Ratkiewicz
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

#include "../bitstreams/output_bitstream.hpp"
#include "../bitstreams/input_bitstream.hpp"
#include "../utils/fast.hpp"

#include "timing.hpp"

#include "callers.hpp"

#include <vector>
#include <cstdlib>
#include <sys/time.h>
#include <iostream>

#include <util/random_number_source.hpp>

#include <boost/lexical_cast.hpp>

////////////////////////////////////////////////////////////////////////////////
/**
 * main method
 */

int main( int argc, char** argv ) {
   using namespace std;
   namespace bibs = benchmark::ibs;
   namespace bobs = benchmark::obs;

   const size_t NUM_CALLERS = 4;
   const size_t NUM_TESTS = boost::lexical_cast<size_t>(argv[1]);
   const size_t DATA_DIVISOR = boost::lexical_cast<size_t>(argv[2]);

   bibs::caller_base* ibs_callers[] = { new bibs::gamma_caller(),
                                        new bibs::delta_caller(),
                                        new bibs::zeta_caller(),
                                        new bibs::nibble_caller() };

   bobs::caller_base* obs_callers[] = { new bobs::gamma_caller(),
                                        new bobs::delta_caller(),
                                        new bobs::zeta_caller(),
                                        new bobs::nibble_caller() };

   vector<size_t> call_sequence( NUM_TESTS / DATA_DIVISOR );
   vector<int> params( NUM_TESTS / DATA_DIVISOR );

   cout << NUM_TESTS << " " << DATA_DIVISOR << " ";
   
   srand( time(NULL) );

   generate( call_sequence.begin(), call_sequence.end(), util::random_number_source(NUM_CALLERS) );
   generate( params.begin(), params.end(), util::random_number_source(100) );

   for( size_t i = 0; i < params.size(); ++i ) {
      if( call_sequence[i] == 2 ) { // zeta_caller
         params[i] %= 32;
      }
   }

   webgraph::obitstream obs( "/tmp/what" );

   // start time

   timeval start;
   gettimeofday( &start, NULL );

   for( size_t j = 0; j < DATA_DIVISOR; j++ ) {
      for( size_t i = 0; i < NUM_TESTS / DATA_DIVISOR; ++i ) {
         obs_callers[call_sequence[i]]->operator()( &obs, params[i] );
      }
   }

   timeval finish;
   gettimeofday( &finish, NULL );
   
   double elapsed = timing::calculate_elapsed( start, finish );

   cout << elapsed << " ";

   obs.flush();

   // time to read

   gettimeofday( &start, NULL );
   int next_guy;

   webgraph::ibitstream ibs( "/tmp/what" );

   for( size_t j = 0; j < DATA_DIVISOR; j++ ) {
      for( size_t i = 0; i < NUM_TESTS / DATA_DIVISOR; ++i ) {
         if( (next_guy = ibs_callers[call_sequence[i]]->operator() (&ibs)) != params[i] ) {
            cerr << "Error: next_guy = " << next_guy << ", params[" << i << "] = " << params[i]
                 << endl;
         }
      }
   }

   gettimeofday( &finish, NULL );

   elapsed = timing::calculate_elapsed( start, finish );

   cout << elapsed << endl;

   return 0;
}


