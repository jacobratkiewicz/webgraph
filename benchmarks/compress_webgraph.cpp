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
 * This program is compiled by the Makefile in this directory.
 * Run it to see usage. Defaults should work for most parameters.
 * 
 * Sample usage: given a graph called some-graph.txt, the following will
 * compress it to three files - compressed_graph.graph, 
 * compressed_graph.offsets, and compressed_graph.properties.
 *
 *      ./compress_webgraph --source=some_graph --dest=compressed_graph
 */

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "../webgraph/webgraph.hpp"
#include "timing.hpp"

/** Reads an immutable graph and stores it as a {@link BVGraph}.
 */
int main( int argc, char** argv ) {
   namespace po = boost::program_options;
   namespace bvg = webgraph::bv_graph;
   using namespace std;

   string src, dest;
   
   int window_size = -1, 
      max_ref_count = -1, 
      min_interval_length = -1, 
      zeta_k = 5, 
      flags = 0,
      quantum = 10000;

   bool offline = false, write_offsets = false;

   ostringstream help_message_oss;

   /// TODO - change this so that source and dest are positional arguments
   help_message_oss 
      << "Usage: " << argv[0] << "[OPTIONS] --source=SOURCE --dest=[DEST]\n"
      << "Compresses a graph differentially. The SOURCE and DEST parameters are\n"
      << "basenames from which suitable file names will be stemmed.\n"
      << "\n"
      << "If DEST is omitted, no recompression is performed. This is useful\n"
      << "in conjunction with --offsets.\n"
      << "\n"
      << "Optional arguments";

   po::options_description desc( help_message_oss.str() );
   desc.add_options()
      ("help,h", "Print help message")
      
      ("comp,c", 
       po::value<int>(), 
       "A compression flag (may be specified several times)")
      
      ("window-size,w", 
       po::value<int>(&window_size)->default_value( bvg::graph::DEFAULT_WINDOW_SIZE ),
       "Reference window size")
      
      ("max-ref-count,m", 
       po::value<int>(&max_ref_count)->default_value(bvg::graph::DEFAULT_MAX_REF_COUNT),
       "Maximum number of backward references")
      
      ("graph-class,g", 
       po::value<string>(),
       "Set graph class")

      ("min-interval-length", 
       po::value<int>(&min_interval_length)->
                          default_value(bvg::graph::DEFAULT_MIN_INTERVAL_LENGTH),
       "Set minimum interval length")
      
      ("zeta-k,k", 
       po::value<int>(&zeta_k)->default_value(bvg::graph::DEFAULT_ZETA_K),
       "the k parameter for zeta-k codes")
      
      ("offline,o", "Use the offline load method to reduce memory consumption")
      
      ("offsets,O", "Generate offsets for the source graph")
      
      ("quantum,q",
       po::value<int>(&quantum)->default_value( quantum ),
       "Set value for progress meter quantum")

      ("source,s",
       po::value<string>(&src),
       "Set source graph file")

      ("dest,d",
       po::value<string>(&dest),
       "Set destination file")
      ;

   po::variables_map vm;
   po::store( po::parse_command_line( argc, argv, desc), vm );
   po::notify( vm );

   if( vm.count( "help" ) ) {
      cout << desc << endl;
      return 1;
   }

   if( vm.count("comp") ) {
      cerr << "Not implemented yet." << endl;
      //         flags |= BVGraph.class.getField( g.getOptarg() ).getInt( BVGraph.class );
      return 1;
   }

   if( vm.count("graph-class") ) {
      if( vm["graph-class"].as<string>() != "AsciiGraph" ) {
         cerr << "The only allowable parameter for graph-class is AsciiGraph right now.\n";

         return 1;
      }
   }

   if( vm.count( "offline" ) ) {
      offline = true;
   }

   if( vm.count( "offsets" ) ) {
      write_offsets = true;
   }
   
   if( !vm.count( "source" ) || !vm.count( "dest") ) {
      cerr << "For now you must specify either a source or a dest." << "\n";
      
      cerr << desc;  
      
      return 0;
   }

   ////////////////////////////////////////////////////////////////////////////////
   // All options are now collected. Start doing the actual loading.
   ////////////////////////////////////////////////////////////////////////////////

   timing::time_t start = timimg::timer();

   namespace ag = webgraph::ascii_graph;

   ag::offline_graph graph = ag::offline_graph::load( src );

   //  ostream* log = &cerr;

   if( dest != "" ) {
      cerr << "About to call store offline graph...\n";
      bvg::graph::store_offline_graph( graph, dest, window_size, max_ref_count, 
                                       min_interval_length, 
                                       zeta_k, flags, NULL );
   }
   else {
      if ( write_offsets ) {
         cerr << "Not implemented yet\n";
         return 1;
      }
   }

   timimg::time_t stop = timing::timer();

   cout << timing::calculate_elapsed( start, stop );

   return 0;
}
