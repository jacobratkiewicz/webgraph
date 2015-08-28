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

#include "../../asciigraph/offline_vertex_iterator.hpp"

#include <iostream>
#include <algorithm>

int main( int, char** ) {
   using namespace std;
   using namespace webgraph::ascii_graph;

   offline_vertex_iterator itor( "dom.graph" ), end;

   cerr << "Just made two iterators - " << endl
        << "itor - " << itor.as_str() << endl
        << "end - " << end.as_str() << endl;

   int i = 0;

   while( itor != end && i++ < 10 ) {
      cerr << *itor << endl;

      ++itor;
   }

   // now COPY the iterator!!!

   offline_vertex_iterator st = itor;

   while( st != end ) {
      cerr << *st << endl;
      ++st;
   }

   return 0;
}
