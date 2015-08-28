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

#include "vertex.hpp"

#include <iostream>
#include <iterator>
#include <algorithm>
#include <sstream>

namespace webgraph { namespace ascii_graph {

std::ostream& operator << (std::ostream& out, const vertex_descriptor& v ) {
   using namespace std;

   out << v.label;

//    out << v.label << " : ";
//    copy( v.successors.begin(), v.successors.end(),
//          ostream_iterator<vertex_label_t>( out, " " ) );
//    out << "\n";
   
   return out;
}

std::string vertex_descriptor::as_str() const {
   using namespace std;

   ostringstream o;

   o << label << " : ";
   copy( successors.begin(), successors.end(),
         ostream_iterator<vertex_label_t>( o, " " ) );

   return o.str();
}


} }
