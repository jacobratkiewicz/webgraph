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
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/progress.hpp>

unsigned generate_uniform_random_graph( unsigned num_vertices, unsigned num_edges, std::ostream& dest_file );

int main( int argc, char* argv[] ) {
   namespace po = boost::program_options;
   using namespace std;

   po::options_description desc( "Usage - " );

   string dest_filename;
   unsigned num_vertices, num_edges;

   desc.add_options()
      ("dest,d", po::value<string>(&dest_filename), "Destination file name")
      ("vertices,v", po::value<unsigned>(&num_vertices), "Number of vertices")
      ("edges,e", po::value<unsigned>(&num_edges), "Expected number of edges")
      ;

   po::variables_map vm;
   po::store( po::parse_command_line( argc, argv, desc), vm );
   po::notify( vm );

   if( !vm.count( "dest" ) || !vm.count("vertices") || !vm.count("edges") ) {
      cerr << desc;

      return 1;
   }

   ofstream dest_file( dest_filename.c_str() );

   cerr << "Generating random graph...";
   unsigned edges = generate_uniform_random_graph( num_vertices, num_edges, dest_file );

   cerr << "\nEdges = " << edges << endl;
   
   return 0;
}

unsigned generate_uniform_random_graph( unsigned num_vertices, unsigned num_edges, std::ostream& dest_file ) {
   unsigned expected_edges_per_vertex = num_edges / num_vertices;
   unsigned total_edges = 0;

   using namespace std;

   srandom(time(NULL));

   boost::progress_display progress( num_vertices );

   dest_file << num_vertices << "\n";

   for( unsigned i = 0; i < num_vertices; i++ ) {
      unsigned edges_for_this_vertex = random() % (2*expected_edges_per_vertex);

      vector<unsigned> this_succ_list;
      
      unsigned edges_generated = 0;
      while( edges_generated < edges_for_this_vertex ) {
         unsigned next_edge = random() % (num_vertices-1);
         // the point is to not allow i as a choice.
         if( next_edge >= i )
            ++next_edge;

         assert( 0 <= next_edge );
         assert( next_edge < num_vertices );

         if( find( this_succ_list.begin(), this_succ_list.end(), next_edge ) == this_succ_list.end() ) {
            this_succ_list.push_back( next_edge );
            ++edges_generated;
         }
      }

      total_edges += edges_generated;

      sort( this_succ_list.begin(), this_succ_list.end() );

      // now dump the successor list.
      if( this_succ_list.size() > 0 ) {
         dest_file << this_succ_list[0];
         for( unsigned j = 1; j < this_succ_list.size(); j++ ) {
            dest_file << " " << this_succ_list[j];
         }
      }

      dest_file << "\n";
      ++progress;
   }

   return total_edges;
}
