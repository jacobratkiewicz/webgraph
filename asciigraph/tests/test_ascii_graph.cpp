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

#include "../offline_graph.hpp"
#include <iostream>
#include <utility>
#include <boost/tuple/tuple.hpp>
#include <iterator>
#include <algorithm>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace webgraph::ascii_graph;
using namespace boost;

void print_all_vertices_and_edges( offline_graph_ptr oag );
void print_first_n_edges( offline_graph_ptr oag, int n );

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char* argv[] ) {
   cerr << "Started.\n";

   if( argc < 2 ) {
      cerr << "Usage is " << argv[0] << " [graph-basename]" << endl;
      return 1;
   }
        
   offline_graph o = offline_graph::load( argv[1] );

//      print_first_n_edges( oag, 10 );
   print_all_vertices_and_edges( o );
        
   return 0;       
}

////////////////////////////////////////////////////////////////////////////////
void print_first_n_edges( offline_graph o, int n ) {
   offline_graph::edge_iterator begin, end;
        
   tie( begin, end ) = o.get_edge_iterator();
        
   cerr << "About to start printing first " << n << " edges.\n";
        
   int i = 0;
        
   for( offline_graph::edge_iterator iter = begin;
        iter != end && i < n;
        iter++, i++ ) {
      offline_graph::edge_type next_edge;
                
      next_edge = *iter;
                
      cerr << "<" << next_edge.first << ", " << next_edge.second << ">";      
   }       
        
   cerr << "Done.\n";
}

////////////////////////////////////////////////////////////////////////////////
void print_all_vertices_and_edges( offline_graph_ptr oag ) {
   offline_graph::vertex_iterator begin, end;

//   cerr << "This is what they look like, straight out of th graph.\n";
//   cerr << o.get_vertex_iterator().first.as_str() << endl;
//   cerr << o.get_vertex_iterator().second.as_str() << endl;

//   cerr << "Now tie them, and see what they look like afterwards.\n";

   tie( begin, end ) = o.get_vertex_iterator();

//   cerr << "Begin is " << begin.as_str();
        
//   cerr << " and end is " << end.as_str() << endl;
        
   int i = 0;
        
   cerr << "Vertices are: " << endl;
        
   for( offline_graph::vertex_iterator iter = begin;
        iter != end;
        ++iter ) {
                
      cerr << *iter << ": ";
                
      ostream_iterator<int> meh( cerr, " " );
                
      copy( successors(iter).begin(), successors(iter).end(), meh );
                
      cerr << endl;
   }
        
   cerr << "\n**************************************\n";
        
   cerr << "Now iterating over all edges. Here goes...\n";
        
   offline_graph::edge_iterator e_begin, e_end;
        
   tie( e_begin, e_end ) = o.get_edge_iterator();
        
   cerr << "Got edge iterator.\n";
        
   i = 0;
        
   for( offline_graph::edge_iterator iter = e_begin;
        iter != e_end;
        iter++) {
      offline_graph::edge_type edge = *iter;
                
      cerr << "<" << edge.first << ", " << edge.second << ">\n";
   }
   
   cerr << "Done." << endl;
}
