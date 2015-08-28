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

#include "../../../webgraph/iterators/iterator_wrappers.hpp"
#include "../../../webgraph/iterators/utility_iterator_base.hpp"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>

int myrand() {
   return rand() % 100;
}

int main( int ac, char** av ) {
   using namespace std;
   using namespace webgraph::bv_graph::iterator_wrappers;
   using namespace webgraph::bv_graph::utility_iterators;

   vector<int> blah(10);
   srand( time(NULL) );

   generate( blah.begin(), blah.end(), myrand );

   cerr << "Printing out 10 random numbers from vector with STL iterator.\n";

   for( vector<int>::iterator itor = blah.begin();
        itor != blah.end();
        ++itor ) {
      cerr << *itor << " ";
   }
   
   cerr << "\nNow the same 10 using iterator wrapped with Java idiom.\n";

   cpp_to_java<vector<int>::iterator, int> ctj( blah.begin(), blah.end() );

   while( ctj.has_next() ) {
      cerr << ctj.next() << " ";
   }

   cpp_to_java<vector<int>::iterator, int> ctj2( blah.begin(), blah.end() );

   utility_iterator_base<int>* u = &ctj2;

   cerr << "\nPrinting polymorphically.\n";

   while( u->has_next() ) {
      cerr << u->next() << " ";
   }

   cerr << "\nNow re-wrapping as a Java iterator and printing again.\n";
   
   cpp_to_java< vector<int>::iterator, int > ctj_3( blah.begin(), blah.end() );
   java_to_cpp<int> jtc( ctj_3 ), jtc_end;

   java_to_cpp<int> jtc2;
   jtc2 = jtc;

   for( ; jtc != jtc_end; ++jtc ) 
      cerr << *jtc << " ";

   cerr << endl;

   for( ; jtc2 != jtc_end; ++jtc2 ) 
      cerr << *jtc2 << " ";

   cerr << endl;

   return 0;
}
