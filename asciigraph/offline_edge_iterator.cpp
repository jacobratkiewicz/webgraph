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

#include "offline_edge_iterator.hpp"

#include <iostream>
#include <iterator>
#include <cassert>
#include <algorithm>
#include <boost/utility.hpp>
#include <boost/tuple/tuple.hpp>

using namespace boost;
using namespace boost::tuples;

using namespace std;

//#define DEBUG 1

namespace webgraph { namespace ascii_graph {

////////////////////////////////////////////////////////////////////////////////
/*! Default constructor
 * We can basically do whatever we want here.
 */
offline_edge_iterator::offline_edge_iterator()
{
   is_end_marker = true;
}

////////////////////////////////////////////////////////////////////////////////
//! Construct an iterator given just the filename containing the adjacency relationship.
offline_edge_iterator::offline_edge_iterator(const char* filename) : 
	current_vertex(filename), last_vertex()
{
   is_end_marker = false;
   
   ifstream getnumv( filename );
   unsigned long numv;

   getnumv >> numv;

   this->num_vertices = numv;
	
   // this assumes there will be at least one vertex in the graph, so
   assert( num_vertices >= 1 );

   current_successor_index = -1;
   fetch_next_edge();
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor.
offline_edge_iterator::offline_edge_iterator( const offline_edge_iterator& rhs ) {
   // el cheapo
   *this = rhs;
}

////////////////////////////////////////////////////////////////////////////////
//! assignment operator
offline_edge_iterator&
offline_edge_iterator::operator = (const offline_edge_iterator& rhs ) {
   current_edge = rhs.current_edge;
   current_successor_index = rhs.current_successor_index;
   current_vertex = rhs.current_vertex;
   last_vertex = rhs.last_vertex;
   is_end_marker = rhs.is_end_marker;
   num_vertices = rhs.num_vertices;

   return *this;
}
                                                                   

////////////////////////////////////////////////////////////////////////////////
//! Common private method called whenever the iterator needs to be advanced.

void offline_edge_iterator::fetch_next_edge() {
   assert( !is_end_marker );
	
   // Check to see if the current vertex has another successor.
   // current_successor++;
   if( current_successor_index < outdegree(current_vertex) ) {
      ++current_successor_index;
#ifdef DEBUG
      cerr << "fetch_next_edge\n"
           << "\tIncrementing current successor index. it's now " 
           << current_successor_index << endl
           << "\tThis is because vertex is : " << current_vertex.as_str()
           << "\toutdegree(vertex) : " << outdegree(current_vertex)
           << endl;
#endif
   }

   // check to see if we've run out of successors for the current node. If we have, increment -
   // also skip any successor-less nodes.
	
   while( current_successor_index == outdegree(current_vertex) ) {
      ++current_vertex;
		current_successor_index = 0;
      	 
      if( current_vertex == last_vertex ) {
         // then we're at the end of the graph
         this->is_end_marker = true;
         return;	
      }
#ifdef DEBUG
      cerr << "fetch_next_edge:\n"
           << "Incrementing vertex. New vertex is " 
           << current_vertex.as_str()
           << endl;
#endif
   }
	
   // Sanity checks.
   assert( 0 <= successors(current_vertex)[0] );
   assert( successors(current_vertex)[0] < num_vertices );

   current_edge = make_pair( *current_vertex, 
                             successors(current_vertex)[current_successor_index] );

#ifdef DEBUG
   cerr << "fetch_next_edge called.\n"
        << "\tcurrent_edge : <" << current_edge.first << ", " << current_edge.second << ">\n";
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor
offline_edge_iterator::~offline_edge_iterator()
{
	// nothing
}

} } // end namespace
