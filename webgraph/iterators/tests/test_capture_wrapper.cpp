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

#include "../iterator_wrappers.hpp"
#include "../utility_iterator_base.hpp"
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <iostream>

int main() {
   using namespace std;
   using namespace webgraph::bv_graph::iterator_wrappers;

   vector<int> test_vector (10);
   int j = 0;
   for( vector<int>::iterator i = test_vector.begin(); i != test_vector.end(); ++i )
      *i = j++;

   cerr << "Here is the vector by itself: \n";
   copy( test_vector.begin(), test_vector.end(), ostream_iterator<int>(cerr, " ") );

   cerr << "\nHere is the vector through an itor capture wrapper: \n";

   itor_capture_wrapper<vector<int>::iterator, int> cap( test_vector.begin(), 0, 10 );

   while( cap.has_next() ) {
      cerr << cap.next() << " ";
   }

   cerr << endl;

   itor_capture_wrapper<vector<int>::iterator, int> cap2( test_vector.begin(), 0, 10 );
   java_to_cpp<int> jv( cap2 ), j_end;

   cerr << "Here it is again through a java to cpp adapter:\n";

   copy( jv, j_end, ostream_iterator<int>(cerr, " ") );

   cerr << endl;

   return 0;
}
