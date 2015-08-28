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

/*
 * usage: given a compressed graph g (consisting of g.graph, g.offsets, and
 * g.properties), the following will run this program:
 * 
 *  print_graph g
 *
 */

#include <webgraph/boost/integration.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <cassert>

int main(int argc, char** argv) {
   typedef webgraph::bv_graph::graph graph;
   typedef boost::shared_ptr<graph> graph_ptr;

   assert( argc > 1 );

   graph_ptr gp = webgraph::bv_graph::graph::load( argv[1] );

   typedef boost::graph_traits< graph > bv_traits;

   cerr << "Here are the vertices ...." << endl;
   
   bv_traits::vertex_iterator v, v_end;
   for( tie( v, v_end ) = boost::vertices( *gp ); v != v_end; ++v ) {
      cerr << *v << endl;
   }
   
   cerr << "Here are the vertices again ...." << endl;
   
   bv_traits::vertex_iterator v2, v2_end;
   for( tie( v2, v2_end ) = boost::vertices( *gp ); v2 != v2_end; ++v2 ) {
      cerr << *v2 << endl;
   }

   cerr << "Now here are the edges..." << endl;
   
   bv_traits::edge_iterator e, e_end;
   for( tie( e, e_end ) = boost::edges( *gp ); e != e_end; ++e ) {
      cerr << "<" << e->first << ", " << e->second << ">" << endl;
   }

   return 0;
}
