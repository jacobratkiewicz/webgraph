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
#include <set>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

#include "../../../webgraph/iterators/merged_iterator.hpp"
#include "compare_iterators.hpp"

using namespace std;
using namespace webgraph;
using namespace webgraph::bv_graph::utility_iterators;

int tiny_rand() {
   return rand();
}

int main( int argc, char** argv ) {
   // Reads two lengths
   int n0 = atoi( argv[1] );
   int n1 = atoi( argv[2] );

   vector<int> x0( n0 ), x1( n1 );
        
   x0.resize( n0 );
   x1.resize( n1 );
        
   // fill both with random numbers
   srand( time(NULL) );
        
   generate( x0.begin(), x0.end(), tiny_rand );
   generate( x1.begin(), x1.end(), tiny_rand ); 
        
   sort( x0.begin(), x0.end() );
   sort( x1.begin(), x1.end() );
        
   vector<int> expected;
        
   vector<int>::iterator x0_end = unique( x0.begin(), x0.end() );
   x0.erase( x0_end, x0.end() );
        
   vector<int>::iterator x1_end = unique( x1.begin(), x1.end() );
   x1.erase( x1_end, x1.end() );   
        
   set_union( x0.begin(), x0.end(),
              x1.begin(), x1.end(),
              back_insert_iterator<vector<int> > (expected) );
        
   cerr << "x0.size() : " << x0.size() << endl
        << "x1.size() : " << x1.size() << endl
        << "expected.size() : " << expected.size() << endl;

   typedef merged_iterator<int> mi_t;
   typedef webgraph::bv_graph::iterator_wrappers::cpp_to_java< vector<int>::iterator, int > wrapper_t;
   typedef boost::shared_ptr<wrapper_t> wrapper_ptr;

   wrapper_ptr x0_wrap( new wrapper_t(x0.begin(), x0.end()) );
   wrapper_ptr x1_wrap( new wrapper_t(x1.begin(), x1.end()) );
   
   mi_t m( x0_wrap, x1_wrap );
        
   cerr << "comparing iterators normally..\n";

   compare_iterators( m, expected.begin(), expected.end() );

   cerr << "Comparing iterators polymorphically..\n";
   compare_iterators_polymorphic( m, expected.begin(), expected.end() );
                                  

   cerr << "Comparing iterators wrapped..\n";
   compare_iterators_wrapped( m, expected.begin(), expected.end() );
   return 0;                       
}
