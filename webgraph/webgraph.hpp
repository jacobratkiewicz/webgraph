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

#ifndef WEBGRAPH_H_
#define WEBGRAPH_H_

#include <string>
#include <vector>
#include <utility>
#include <climits>

#include <boost/progress.hpp>
#include <boost/shared_ptr.hpp>

#include "types.hpp"
#include "../asciigraph/offline_graph.hpp"
#include "compression_flags.hpp"
#include "iterators/utility_iterator_base.hpp"
#include "iterators/iterator_wrappers.hpp"
#include "iterators/node_iterator.hpp"
#include "../bitstreams/input_bitstream.hpp"
#include "../bitstreams/output_bitstream.hpp"
#include "../log/logger.hpp"

namespace webgraph { namespace bv_graph {

   namespace utility_iterators {
      template<class val_type>
      class residual_iterator;
   }


class graph //: ImmutableGraph 
{
   ////////////// PRIVATE MEMBERS
private:
   
   const static int STD_BUFFER_SIZE = 1024 * 1024;
        
   /** The compression flags used. */
   int flags;
   
   /** These bools were used in the Java version to enable conditional compilation
    * Might be able to remove them now? */
   const static bool STATS = false;
   const static bool DEBUG = false;

   /** 
    * Internal successor iterator pointer - used because internal iterators
    * are polymorphic and thus must be manipulated through pointers.
    */
   typedef boost::shared_ptr<utility_iterators::utility_iterator_base<int> > 
      internal_succ_itor_ptr;

   /** Scratch variables used by the {@link #diffComp(OutputBitStream, int, int, int[],
    * int, int[], int, boolean)} method. */

   //private IntArrayList extras = new IntArrayList(), blocks = new IntArrayList(), 
   //      residuals = new IntArrayList(),
   //      left = new IntArrayList(), len = new IntArrayList();
   
   // TODO this will have to be simulated.
   
   //private PrintWriter offsetStats, outdegreeStats, blockCountStats, blockStats,
   //intervalCountStats, referenceStats, leftStats, lenStats, residualStats,
   //residualCountStats;
   
   ////////////// PUBLIC MEMBERS
public:
   friend class node_iterator;
   friend class utility_iterators::residual_iterator<int>;
   
   typedef webgraph::bv_graph::node_iterator node_iterator;
   typedef std::pair<node_iterator, node_iterator> node_itor_pair;

   typedef webgraph::bv_graph::iterator_wrappers::java_to_cpp<int> successor_iterator;
   typedef std::pair<successor_iterator, successor_iterator> succ_itor_pair;
   
   /** This number classifies the present graph format. When new features require
       introducing binary incompatibilities, this number is bumped so to ensure that old
       classes do not try to read graphs they cannot understand. */
   const static int BVGRAPH_VERSION = 0;
   
   /** Default backward reference maximum length (no bound). */
   const static int DEFAULT_MAX_REF_COUNT; // initialized in .cpp
   
   /** Default window size. */
   const static int DEFAULT_WINDOW_SIZE = 10; //7; TODO CHANGE THIS
   
   /** Default minimum interval length. */
   const static int DEFAULT_MIN_INTERVAL_LENGTH = 3;
   
   /** Default offset step. */
   const static int DEFAULT_OFFSET_STEP = 1;
        
   /** Default value of <var>k</var>. */
   const static int DEFAULT_ZETA_K = 5; //3; TODO CHANGE THIS

   /** Flag: write outdegrees using &gamma; coding (default). */
   const static int OUTDEGREES_GAMMA = compression_flags::GAMMA;

   /** Flag: write outdegrees using &delta; coding. */
   const static int OUTDEGREES_DELTA = compression_flags::DELTA;

   /** Flag: write copy-block lists using &gamma; coding (default). */
   const static int BLOCKS_GAMMA = compression_flags::GAMMA << 4;

   /** Flag: write copy-block lists using &delta; coding. */
   const static int BLOCKS_DELTA = compression_flags::DELTA << 4;

   /** Flag: write residuals using &gamma; coding. */
   const static int RESIDUALS_GAMMA = compression_flags::GAMMA << 8;

   /** Flag: write residuals using &zeta;<sub><var>k</var></sub> coding (default). */
   const static int RESIDUALS_ZETA = compression_flags::ZETA << 8;

   /** Flag: write residuals using &delta; coding. */
   const static int RESIDUALS_DELTA = compression_flags::DELTA << 8;

   /** Flag: write residuals using variable-length nibble coding. */
   const static int RESIDUALS_NIBBLE = compression_flags::NIBBLE << 8;

   /** Flag: write references using &gamma; coding. */
   const static int REFERENCES_GAMMA = compression_flags::GAMMA << 12;

   /** Flag: write references using &delta; coding. */
   const static int REFERENCES_DELTA = compression_flags::DELTA << 12;

   /** Flag: write references using unary coding (default). */
   const static int REFERENCES_UNARY = compression_flags::UNARY << 12;

   /** Flag: write block counts using &gamma; coding (default). */
   const static int BLOCK_COUNT_GAMMA = compression_flags::GAMMA << 16;

   /** Flag: write block counts using &delta; coding. */
   const static int BLOCK_COUNT_DELTA = compression_flags::DELTA << 16;

   /** Flag: write block counts using unary coding. */
   const static int BLOCK_COUNT_UNARY = compression_flags::UNARY << 16;

   /** Flag: write offsets using &gamma; coding (default). */
   const static int OFFSETS_GAMMA = compression_flags::GAMMA << 20;

   /** Flag: write offsets using &delta; coding. */
   const static int OFFSETS_DELTA = compression_flags::DELTA << 20;

   ////////////// PROTECTED MEMBERS
protected:
   /** The initial length of an array that will contain a successor list. */
   const static int INITIAL_SUCCESSOR_LIST_LENGTH = 1024;
        
   /** A special value for {@link #minIntervalLength} interpreted as meaning that the
    * minimum interval length is infinity. */
   const static int NO_INTERVALS = 0;

   /** The basename of the graph. This may be "", but trying to load the
    * graph with an offset step of -1 will cause an exception. */
   std::string basename;
        
   /** The number of nodes of the graph. */
   long n;

   /** The number of arcs of the graph. */
   long m;

   /** When {@link #offsetStep} is not -1, whether this graph is directly loaded into
    * {@link #graphMemory}, or rather wrapped in a {@link
    * it.unimi.dsi.mg4j.io.FastMultiByteArrayInputStream} specified by {@link
    * #graphStream}. */
   bool in_memory;

   /** The byte array storing the compressed graph, if {@link #in_memory} is true and
    *  {@link #offsetStep} is not -1.
    *  
    * <P>This variable is loaded with a copy of the graph file, or with a rearrangement of
    * the latter, depending on whether {@link #offsetStep} is smaller than or equal to
    * one. If {@link #offsetStep} is -1, this variable is <code>null</code>, and node
    * iterators are generated by opening streams directly on the graph file. */
   boost::shared_ptr< std::vector<byte> > graph_memory_ptr;
   std::vector<byte>& graph_memory;

   /** The long array input stream storing the compressed graph, if {@link #in_memory} is
    * false and {@link #offsetStep} is not -1.
    * 
    * <P>It is loaded with a copy of the graph file. If {@link #offsetStep} is -1, this
    * variable is <code>null</code>, and node iterators are generated by opening streams
    * directly on the graph file. */
         
//   ArrayInputStream* graph_stream;

   /** This variable is <code>null</code> iff {@link #offsetStep} is zero or less
    * (impliying that offsets have not been loaded).  Otherwise, the entry of index
    * <var>i</var> represent the offset (in bits) at which each the <var>i</var>-th
    * successor-list block starts. The last entry contains the length in bits of the whole
    * graph file. */
   std::vector<long> offset;

   /** The maximum reference count. */
   int max_ref_count;

   /** The window size. Zero means no references. */
   int window_size;

   /** The minimum interval length. */
   int min_interval_length;

   /** The offset step. Special values are 0, meaning no offsets, and -1, meaning no
    * offset and no {@link #graph_memory}.
    */
   int offset_step;

   /** The value of <var>k</var> for &zeta;<sub><var>k</var></sub> coding (for residuals). */
   int zeta_k;

   /** The coding for outdegrees. By default, we use &gamma; coding. */
   int outdegree_coding;

   /** The coding for copy-block lists. By default, we use &gamma; coding. */
   int block_coding;

   /** The coding for residuals. By default, we use &zeta; coding. */
   int residual_coding;

   /** The coding for references. By default, we use unary coding. */
   int reference_coding;

   /** The coding for block counts. By default, we use &gamma; coding. */
   int block_count_coding;

   /** The coding for offsets. By default, we use &gamma; coding. */
   int offset_coding;

   /** A bit stream wrapping {@link #graph_memory}, or {@link #graphStream}, used
       <em>only</em> by {@link #outdegree(int)}. It is declared here for efficiency
       reasons. It is thus safe for it to be mutable. */
   mutable ibitstream outdegree_ibs;

   /** A cache maintaining the outdegrees from {@link #cacheStart} (inclusive) to {@link
    * #outdegreeCacheEnd} (exclusive). */
   mutable std::vector<int> outdegree_cache;
   /** The first node in the outdegree cache. */
   mutable int outdegree_cache_start;
   /** The number of last node of the outdegree cache plus one. */
   mutable int outdegree_cache_end;

   /** A cache maintaining the offsets from {@link #outdegree_cache_start} (inclusive) to
    * {@link #offset_cacheEnd} (exclusive). */
   // This stuff is accessed by position, and is mutable for efficiency reasons.
   mutable std::vector<int> offset_cache;
   /** The number of last node of the offset cache plus one. This is never greater than
    * {@link #outdegree_cache_end}. */
   mutable int offset_cache_end;
   
   /** These are only used by differentially_compress. Would be preferable to put their declarations
    * there, at some point */
   std::vector<int> extras;
   std::vector<int> blocks;
   std::vector<int> len;
   std::vector<int> left;
   std::vector<int> residuals;

#ifndef CONFIG_FAST
   static logs::module_logger& lg() {
      return logs::logger( "webgraph" );
   }
#endif

   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   // START OF METHODS
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////

protected:
   graph() : graph_memory_ptr( new std::vector<byte> ),
             graph_memory( *graph_memory_ptr ),
             max_ref_count(DEFAULT_MAX_REF_COUNT),
             window_size(DEFAULT_WINDOW_SIZE),
             min_interval_length(DEFAULT_MIN_INTERVAL_LENGTH),
             offset_step(DEFAULT_OFFSET_STEP),
             zeta_k(DEFAULT_ZETA_K),
             outdegree_coding(webgraph::compression_flags::GAMMA),
             block_coding(webgraph::compression_flags::GAMMA),
             residual_coding(webgraph::compression_flags::ZETA),
             reference_coding(webgraph::compression_flags::UNARY),
             block_count_coding(webgraph::compression_flags::GAMMA),
             offset_coding(webgraph::compression_flags::GAMMA),
             outdegree_cache_start(INT_MAX),
             outdegree_cache_end(INT_MAX),
             offset_cache_end(INT_MAX) {
#ifndef CONFIG_FAST
      // doesn't really need to do anything except register a logger.
      logs::register_logger( "webgraph", logs::LEVEL_MAX );
#endif
   }
        
public:
   virtual ~graph() {
      // TODO write me - if needed?
   }
   
   long get_num_nodes() const;
   long get_num_arcs() const;
        
   std::string get_basename() const;

   int get_max_ref_count() const;
   int get_window_size() const;
   int get_offset_step() const;

protected:
   int read_offset( ibitstream& ibs ) const;
   int write_offset( obitstream& obs, int x ) const;
   int read_outdegree( ibitstream& ibs ) const;
   int write_outdegree( obitstream& obs, int d ) const;
   int read_reference( ibitstream& ibs ) const;
   int write_reference( obitstream& obs, int ref ) const;
   int read_block_count( ibitstream& ibs ) const;
   int write_block_count( obitstream& obs, int count ) const;
   int read_block( ibitstream& ibs ) const;
   int write_block( obitstream& obs, int block ) const;
   int read_residual( ibitstream& ibs ) const;
   int write_residual( obitstream& obs, int residual ) const;

public:

   int outdegree( int x ) const;

protected:

   void skip_node( ibitstream& ibs, int x, int outd ) const;

   int position( ibitstream& ibs, int x ) const;

public:
   succ_itor_pair get_successors( int x ) const;
   
private:
   internal_succ_itor_ptr get_successors_internal( int x ) const;

   internal_succ_itor_ptr get_successors_internal( int x, boost::shared_ptr<ibitstream> ibs, 
                                                   std::vector< std::vector<int> >& window, 
                                                   std::vector<int>& outd, 
                                                   std::vector<int>& blockOutdegrees ) const;
public:
   std::pair<node_iterator, node_iterator> get_node_iterator( int from ) const;
        
private:
   void set_flags( int flags );
   static std::string flags_to_string( int flags );
   static int string_to_flags( std::string flag_string );

public:
   typedef boost::shared_ptr<graph> graph_ptr;
   
   static graph_ptr load( std::string basename, int offset_step, std::ostream* log = NULL );
   static graph_ptr load( std::string basename, std::ostream* log = NULL );
   static graph_ptr load_sequential( std::string basename, std::ostream* log = NULL );
   static graph_ptr load_offline( std::string basename, std::ostream* log = NULL );

protected:
   void load_internal( std::string basename, int offset_step, std::ostream* log = NULL );
   static int intervalize( const std::vector<int>& x, int min_interval, std::vector<int>& left, 
                           std::vector<int>& len, 
                           std::vector<int>& residuals );
                
private: 
   int differentially_compress( obitstream& obs, int current_node, int ref, 
                                std::vector<unsigned int>& ref_list, int ref_length, 
                                std::vector<unsigned int>& current_list, 
                                int current_len, bool for_real );
        
public:
//   static void store( boost::shared_ptr<graph> graph, std::string
//                      basename, int window_size, int max_ref_count,
//                      int min_interval_length, int zeta_k, int
//                      flags, std::ostream* log = NULL );
   static void store_offline_graph( webgraph::ascii_graph::offline_graph graph, 
                                    std::string basename, int window_size, int max_ref_count, 
                                    int min_interval_length, int zeta_k, int flags, std::ostream* log = NULL );

private:
//   void store_internal( boost::shared_ptr<graph> graph, std::string basename, std::ostream* log = NULL );
   void store_offline_graph_internal( webgraph::ascii_graph::offline_graph graph, 
                                      std::string basename, 
                                      std::ostream* log = NULL );
        
public:
   void write_offsets( obitstream& obs, std::ostream* log = NULL );

//public:
//   static void main( std::string args[] );
};

} }
#endif /*WEBGRAPH_H_*/
