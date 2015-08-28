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

#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "../webgraph/webgraph.hpp"

int main( int argc, char** argv ) {
   using namespace std;
   using namespace webgraph::bv_graph;

   if( argc < 2 ) {
      cerr << "You have to provide name of graph file.\n";
      return 1;
   }
   
//#ifndef CONFIG_FAST
//   logs::register_logger( "print_webgraph", logs::LEVEL_MAX );
//#endif

   string name = argv[1];

   typedef boost::shared_ptr<graph> graph_ptr;

   graph_ptr gp = graph::load( name );
   
//   cerr << "about to try to get node iterator.\n";
   
   graph::node_iterator n, n_end;
   
   tie(n, n_end) = gp->get_node_iterator( 0 );
   
   cerr << "num vertices:\n";
   cerr << gp->get_num_nodes() << endl;

   cerr << "here are vertices:\n";
   
   while( n != n_end ) {
       webgraph::bv_graph::graph::successor_iterator succ, succ_end;
       
       tie( succ, succ_end ) = successors( n );
//       vector<int> succ = successor_vector( n );
      
       cerr << "outdegree : " << outdegree(n) << endl;

       cerr << "********************   " << *n << "\n";

      while( succ != succ_end ) {
         cerr << *succ << " ";
         ++succ;
      }
      
      cerr << endl;

//       copy( succ.begin(), succ.end(), ostream_iterator<int>(cerr, " " ) );
//       if( succ.size() > 0 ) 
//          cerr << succ[0];

//       for( int i = 1; i < succ.size(); ++i )
//          cerr << " " << succ[i];
      
//       ostringstream strstr;
//       copy( succ.begin(), succ.end(), ostream_iterator<int>(strstr, " " ) );

//      logs::logger( "print_webgraph" ) << strstr.str() << "\n";

     cerr << endl;

      ++n;  
   }

   return 0;  
}
