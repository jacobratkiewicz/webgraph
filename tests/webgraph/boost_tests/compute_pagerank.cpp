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

#include "../../../webgraph/boost/integration.hpp"

#include <boost/program_options.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/page_rank.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
   typedef webgraph::bv_graph::graph graph;
   typedef boost::shared_ptr<graph> graph_ptr;
   namespace po = boost::program_options;
   using namespace std;

   ostringstream help_message;
   
   help_message << "Usage:"
                << "\t" << argv[0] << "--source=source --dest=dest\n"
                << "Takes a BVGraph and computes pagerank for each node, storing the "
                << "result in the\ngiven file.\n";

   // TODO make these positional
   po::options_description desc( help_message.str() );

   string source_basename, dest_filename;

   desc.add_options()
      ("help,h", "Print this help message.")
      ("src", 
       po::value<string>(&source_basename),
       "Specify the source (BVgraph) file basename.")
      ("dest",
       po::value<string>(&dest_filename),
       "Specify the destination filename.")
      ;
   
   po::variables_map vm;
   po::store( po::parse_command_line( argc, argv, desc ), vm );

   po::notify(vm);

   if( vm.count("help") || !vm.count("src") || !vm.count("dest") ) {
      cout << desc << endl;
      return 1;
   }


   graph_ptr gp = webgraph::bv_graph::graph::load( source_basename );

   vector<double> rank( boost::num_vertices(*gp) );

   boost::graph::page_rank(*gp, 
                           boost::make_iterator_property_map(rank.begin(),
                                                             boost::get(boost::vertex_index, *gp)));

   ofstream out( dest_filename.c_str() );

   copy( rank.begin(), rank.end(), ostream_iterator<double>( out, "\n" ) );
}

