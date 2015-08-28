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

#include "../../../webgraph/boost/integration.hpp"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <cassert>

int main(int argc, char** argv) {
   typedef webgraph::bv_graph::graph graph;
   typedef boost::shared_ptr<graph> graph_ptr;

   assert( argc > 1 );

   graph_ptr gp = webgraph::bv_graph::graph::load( argv[1] );

   typedef boost::graph_traits< graph > bv_traits;

   cerr << "Here are the vertices, with their successors." << endl;
   
   bv_traits::vertex_iterator v, v_end;
   for( tie( v, v_end ) = boost::vertices( *gp ); v != v_end; ++v ) {
      cerr << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << *v << " : ";

      bv_traits::adjacency_iterator a, a_end;
      for( tie( a, a_end ) = boost::adjacent_vertices( *v, *gp );
           a != a_end;
           ++a ) {
         cerr << *a << " ";
      }
      
      cerr << "In other words,\n" << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      
       bv_traits::out_edge_iterator oe, oe_end;
       for( tie( oe, oe_end ) = boost::out_edges( *v, *gp );
            oe != oe_end;
            ++oe ) {
          cerr << "<" << oe->first << ", " << oe->second << "> ";
       }

      cerr << endl;
   }

   return 0;
}
