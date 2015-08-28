/*               
 *  Copyright (c) 2007, Jacob Ratkiewicz.
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

/*********************************************************************************/

/*
 * usage: given a compressed graph g (consisting of g.graph, g.offsets, and
 * g.properties), the following will run this program:
 * 
 * bv_to_ascii --source=g --dest=somethingelse
 *
 */

#include "../../../webgraph/boost/integration.hpp"

#include <boost/program_options.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>


// template<class graph_type>
// void do_conversion_edge( typename boost::shared_ptr<graph_type> gp,
//                          std::ofstream& out );

template<class graph_type>
void do_conversion_vertex( boost::shared_ptr<graph_type> gp,
                           std::ofstream& out );

int main(int argc, char** argv) {
   typedef webgraph::bv_graph::graph graph;
   typedef boost::shared_ptr<graph> graph_ptr;
   namespace po = boost::program_options;
   using namespace std;

   ostringstream help_message;
   
   help_message << "Usage:"
                << "\t" << argv[0] << "--source=source --dest=dest\n"
                << "Takes a BV-graph triplet (.graph, .properties, .offsets) and converts\n"
                << "it to a graph-txt file.\n";

   // TODO make these positional
   po::options_description desc( help_message.str() );

   string source_basename, dest_basename;

   desc.add_options()
      ("help,h", "Print this help message.")
      ("src", 
       po::value<string>(&source_basename),
       "Specify the source (BVgraph) file basename.")
      ("dest",
       po::value<string>(&dest_basename),
       "Specify the destination (graph-txt) basename. Omit any extension.")
      ("elg",
       "Use the EdgeListGraph aspect of the graph to do the conversion (for debugging).")
      ("ag", "Use the AdjacencyGraph aspect of the graph to do the conversion (default).")
      ;
   
   po::variables_map vm;
   po::store( po::parse_command_line( argc, argv, desc ), vm );

   po::notify(vm);

   if( vm.count("help") || !vm.count("src") || !vm.count("dest") ) {
      cout << desc << endl;
      return 1;
   }

   ////////////////////////////////////////////////////////////
   // args collected, work can begin
   ////////////////////////////////////////////////////////////

   graph_ptr gp = webgraph::bv_graph::graph::load( source_basename );
   dest_basename += ".graph-txt";
   ofstream outfile( dest_basename.c_str() );
   
   typedef boost::graph_traits< graph > bv_traits;

   if( vm.count( "elg" ) ) {
      cerr << "not done.\n";
//      do_conversion_edge( gp, outfile );
      return 1;
   } else {
       do_conversion_vertex( gp, outfile );
    }

   cerr << "Done.\n";

   return 0;
}

/* Too annoying.
template<class graph_type>
void do_conversion_edge( typename boost::shared_ptr<graph_type> gp,
                         std::ofstream& out ) {

   using namespace std;

   cout << "About to do conversion based on the EdgeListGraph concept.\n";

   typedef typename boost::graph_traits<graph_type>::vertex_descriptor vertex_t;
   typedef typename boost::graph_traits<graph_type>::edge_descriptor edge_t;
   typedef typename boost::graph_traits<graph_type>::edge_iterator edge_iterator_t;

   edge_iterator e, e_end;
   boost::tie( e, e_end ) = boost::edges( *gp );

   typename boost::graph_traits<graph_type>::vertices_size_type num_v =
      boost::num_vertices(*gp);

   out << num_v << endl;

   // set things up for iteration.. do the first edge specially.
   edge_t prev_edge = *e++;
   while( e != e_end ) {
      edge_t this_edge = *e;
      out << prev_edge->second;
      
      if( this_edge->first == prev_edge->first ) {
         out << " ";
      } else {
         out << 
      }
   }
}
*/

template<class graph_type>
void do_conversion_vertex( boost::shared_ptr<graph_type> gp,
                           std::ofstream& out ) {
   using namespace std;

   typedef typename boost::graph_traits<graph_type>::vertices_size_type v_size_t;

   v_size_t num_v = boost::num_vertices( *gp );
   out << num_v << endl;

   typedef typename boost::graph_traits<graph_type>::vertex_descriptor vertex_t;
   typedef typename boost::graph_traits<graph_type>::vertex_iterator vertex_itor_t;
   typedef typename boost::graph_traits<graph_type>::adjacency_iterator adj_itor_t;

   vertex_itor_t v, v_end;

   for( boost::tie( v, v_end ) = boost::vertices(*gp);
        v != v_end;
        ++v ) {
      // retrieve and print the successor list.
      adj_itor_t a, a_end;
      tie( a, a_end ) = boost::adjacent_vertices( *v, *gp );
      
      if( a != a_end ) {
         out << *a++;
      }

      while( a != a_end ) {
         out << " " << *a++;
      }

      out << endl;
   }
}
