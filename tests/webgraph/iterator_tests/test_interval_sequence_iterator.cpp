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

#include <iostream>
#include <vector>
#include <algorithm>
#include "../../../webgraph/iterators/interval_sequence_iterator.hpp"
#include "compare_iterators.hpp"

class interval_size_generator {
   int avg;
   
public:
   interval_size_generator( int a ) {
      avg = a;
   }  
   
   bool coin_is_heads() {
      return rand() % 2 == 0;  
   }
   
   int sign() {
      if( coin_is_heads() )
         return 1;
      else
         return -1;  
   }
   
   size_t operator() () {
      return avg + sign() * (rand() % avg) / 2;  
   }
};

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char**argv) {
   using namespace std;
   using namespace webgraph::bv_graph;
   
   int num_intervals = atoi( argv[1] );
   int avg_size_of_interval = atoi( argv[2] );
   
   vector<int> sizes( num_intervals ); 
   generate( sizes.begin(),  sizes.end(), interval_size_generator(avg_size_of_interval) );

   cerr << "Here are the sizes : ";
   copy( sizes.begin(), sizes.end(),
         ostream_iterator< size_t >( cerr, " " ) );

   cerr << endl;
   
   int start_of_last_interval = rand() % 100;
   
   vector<int> left( num_intervals );
   left[0] = start_of_last_interval;


   for( int i = 1; i < num_intervals; i++ ) {
      // it's at least as long as the size, but could be twice that,
      // just to spice things up
      
      int end_of_last_interval = start_of_last_interval + sizes[i-1];
      int start_of_this_interval = end_of_last_interval + rand() % sizes[i-1] + 1;

      left[i] = start_of_this_interval;
      
      // set things up for next time
      start_of_last_interval = start_of_this_interval;
   }
   
   // data is generated, print it
   cerr << "We expect to see:\n";
   
   vector<int> expected;
   
   for( int i = 0; i < num_intervals; i++ ) {
      for( int j = 0; j < sizes[i]; j++ ) {
         expected.push_back( left[i] + j );  
      }
   }
  
   copy( expected.begin(), expected.end(),
         ostream_iterator<int>( cerr, " " ) );
         
   cerr << endl;
   
   cerr << "Here's what we really see:\n";
   
   using namespace utility_iterators;

   interval_sequence_iterator<int> ist( left, sizes );
   
   cerr << "Comparing the iterators normally...\n";
   compare_iterators( ist, expected.begin(), expected.end() );
   
   cerr << "Comparing the iterators polymorphically...\n";
   compare_iterators_polymorphic( ist, expected.begin(), expected.end() );
   
   cerr << "Comparing wrapped iterators...\n";
   compare_iterators_wrapped( ist, expected.begin(), expected.end() );
   
   return 0;
}

