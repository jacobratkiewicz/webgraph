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

#include "offline_vertex_iterator.hpp"

#include <sstream>
#include <iterator>
#include <algorithm>

namespace webgraph { namespace ascii_graph {
	
////////////////////////////////////////////////////////////////////////////////
/*! Default constructor, used to construct and end marker.
 *
 */
offline_vertex_iterator::offline_vertex_iterator() {
   init();
   end_marker = true;
}

////////////////////////////////////////////////////////////////////////////////
/*! Construct an offline vertex iterator that will iterate over the vertices in the given file.
 *
 */
offline_vertex_iterator::offline_vertex_iterator(const char* filename)
{
   init();
   
   this->filename = filename;
   back.open(filename);

   end_marker = false;
   
   string tmp;
   getline( back, tmp );
   
   istringstream iss(tmp);
   
   iss >> num_vertices;
   
   current_descriptor.label_ref() = 0; // just to make sure it's valid.
   
   increment();
   
   current_descriptor.label_ref() = 0;
}

////////////////////////////////////////////////////////////////////////////////
/*! Copy constructor.
 *
 */
offline_vertex_iterator::offline_vertex_iterator( const offline_vertex_iterator& that ) {
   init();
   copy(that);
}

////////////////////////////////////////////////////////////////////////////////
/*! Assignment operator
 */
offline_vertex_iterator& offline_vertex_iterator::operator = (
   const offline_vertex_iterator& rhs ) {
   
   copy( rhs );

   return *this;
}

////////////////////////////////////////////////////////////////////////////////
/*! Cheap way to implement copy and assignment.
 */
void offline_vertex_iterator::copy( const offline_vertex_iterator& other ) {
   if( other.end_marker ) {
      if( back.is_open() )
         back.close();
      
      end_marker = true;  
   } else {
      current_descriptor = other.current_descriptor;
      if( back.is_open())
         back.close();

      back.clear();
      
      back.open( other.filename.c_str() );
      back.seekg( other.get_pos );
      get_pos = other.get_pos;
      num_vertices = other.num_vertices;
      end_marker = false;
      filename = other.filename;
   }
}

////////////////////////////////////////////////////////////////////////////////
/*! Backing function for ++'s
 */
void offline_vertex_iterator::increment() {
   string tmp;
   
   bool success = getline( back, tmp );
   
   if( !success ) {
      end_marker = true;
#if 0
      cerr << "Just failed to perform a getline. Current state: " << endl
           << as_str() << endl
           << "back.good() = " << back.good() << endl;
#endif      
      return;
   }
   
   current_descriptor.successors_ref().clear();
   istringstream iss( tmp );
   
   std::copy( istream_iterator<vertex_label_t>(iss), 
              istream_iterator<vertex_label_t>(), 
              back_inserter(current_descriptor.successors_ref()) );
      
   get_pos = back.tellg();
   current_descriptor.label_ref()++;
}

////////////////////////////////////////////////////////////////////////////////
/*! Return a string representation of this iterator, for debugging purposes.
 */
string offline_vertex_iterator::as_str() const {
   ostringstream o;
   
   o << "ascii_graph::offline_vertex_iterator\n"
     << "\tcurrent_descriptor:\n\t" << current_descriptor.as_str() << "\n"
     << "\tfilename: " << filename << "\n"
     << "\tseek position: " << get_pos << "\n"
     << "\tis end marker?: " << end_marker << "\n";
   
   return o.str();
}

////////////////////////////////////////////////////////////////////////////////
/*! Destructor (nop)
 */
offline_vertex_iterator::~offline_vertex_iterator()
{
}

////////////////////////////////////////////////////////////////////////////////

int outdegree( const offline_vertex_iterator& me ) {
   return me.current_descriptor.get_successors().size();   
}
      
////////////////////////////////////////////////////////////////////////////////
const std::vector<vertex_label_t>& successors( const offline_vertex_iterator& me ) {
   return me.current_descriptor.get_successors();
}

} }
