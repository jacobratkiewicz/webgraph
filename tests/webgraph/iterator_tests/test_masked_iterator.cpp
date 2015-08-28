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
#include <cstdlib>

#include "../../../webgraph/iterators/masked_iterator.hpp"
#include "compare_iterators.hpp"

using namespace std;
using namespace webgraph;
using namespace webgraph::bv_graph::utility_iterators;

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv ) {
   // Reads the length and number of 0s
   unsigned n = atoi( argv[ 1 ] );
   unsigned z = atoi( argv[ 2 ] );

   vector<int> x( n );

   vector<bool> keep( n );

   vector<int> res;
   vector<int> blocks;

   unsigned i, j, p = 0;
   bool dep;

   // Generate
   for ( i = 0; i < n; i++ ) 
      p = x[ i ] = p + ( rand() % 1000 );

   for ( i = 0; i < n-z; i++ ) 
      keep[ i ] = true;

   for ( i = 0; i < n; i++ ) {
      j = i + (int)( rand() % ( n - i ) );
      // swap...
      dep = keep[ i ]; 
      keep[ i ] = keep[ j ]; 
      keep[ j ] = dep;
   }
        
   // Compute result
   for ( i = 0; i < n; i++ ) 
      if ( keep[ i ] ) 
         res.push_back( x[ i ] );

   // Prepare blocks
   bool look_at = true;

   int curr = 0;
   for ( i = 0; i < n; i++ ) {
      if ( keep[ i ] == look_at ) 
         curr++;
      else {
         blocks.push_back( curr );
         look_at = !look_at;
         curr = 1;
      }
   }

   // Output 
   cerr << "GENERATED:";
   for ( i = 0; i < n; i++ ) {
      if ( keep[ i ] ) 
         cerr << "*";
      cerr << x[ i ] << "  ";
   }

   cerr << "\nBLOCKS:";

   for ( i = 0; i < blocks.size(); i++ )
      cerr << blocks[ i ] << "  ";
   
   cerr << "\nEXPECTED RESULT:";

   for ( i = 0; i < res.size(); i++ ) 
      cerr << res[ i ] << " ";

   cerr << endl;

   typedef webgraph::bv_graph::iterator_wrappers::cpp_to_java<vector<int>::iterator, int> wrapper_t;

   typedef masked_iterator<int> mi_t;

   boost::shared_ptr< wrapper_t > wrapped( new wrapper_t( x.begin(), x.end() ) );

   mi_t mi( blocks, wrapped );

   cerr << "Testing iterators normally\n";
   compare_iterators( mi, res.begin(), res.end() );

   cerr << "Testing iterators normally\n";
   compare_iterators( mi, res.begin(), res.end() );

   cerr << "Testing iterators polymorphically\n";
   compare_iterators_polymorphic( mi, res.begin(), res.end() );

   cerr << "Testing iterators wrapped..\n";
   compare_iterators_wrapped( mi, res.begin(), res.end() );

   return 0;
} 
