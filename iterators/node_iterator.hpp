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

#ifndef NODE_ITERATOR_HPP
#define NODE_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <sstream>
#include <string>

#include "iterator_wrappers.hpp"
#include "../../bitstreams/input_bitstream.hpp"
#include "../../log/logger.hpp"

namespace webgraph { namespace bv_graph {

class graph;

class node_iterator : 
   public boost::iterator_facade<
      node_iterator,
      int,
      boost::forward_traversal_tag,
      int> {
private:
   int n; // = numNodes();
   
   /** Our bit stream. */
   boost::shared_ptr<ibitstream> ibs;
   /** We keep the size of the cyclic buffer (the window size + 1 ) in a local variable. */
   int cyclic_buffer_size; // = windowSize + 1;
   /** At any time, window will be ready to be passed to {@link BVGraph#successors(int,
    * InputBitStream, int[][], int[], int[])} */ 
   std::vector< std::vector<int> > window; // = new int[ cyclicBufferSize ][ INITIAL_SUCCESSOR_LIST_LENGTH ];
   /** At any time, outd will be ready to be passed to {@link BVGraph#successors(int,
    * InputBitStream, int[][], int[], int[])} */ 
   std::vector<int> outd; // = new int[ cyclicBufferSize ];
   /** At any time, blockOutdegrees will be ready to be passed to {@link
    * BVGraph#successors(int, InputBitStream, int[][], int[], int[])} */ 
   std::vector<int> block_outdegrees; // = offsetStep > 1 ? new int[ offsetStep ] : null;
   /** The index of the node from which we started iterating. */
   int from;
   /** The index of the node just before the next one. */
   int curr;
   /** the graph that this iterator is iterating over. */
   const graph* owner;
   /** wether this is meant to stand as an end marker */
   bool end_marker;

   ////////////////////////////////////////////////////////////////////////////////
   // methods
public:
   node_iterator() {
      this->from = 0;
      this->cyclic_buffer_size = 0;
      this->owner = NULL;
      this->n = 0;
      this->end_marker = true;

#ifndef CONFIG_FAST
      logs::register_logger( "iterators", logs::LEVEL_MAX );
#endif
   }

   ////////////////////////////////////////////////////////////////////////////////
   node_iterator( const graph* owner, boost::shared_ptr<ibitstream> is, 
                  int from, int window_size );

   node_iterator( const node_iterator& rhs ) {
      copy( rhs );
   }

   ~node_iterator() {
      // nothing needed.
   }

   node_iterator& operator = ( const node_iterator& rhs ) {
      copy( rhs );

      return *this;
   }

private:
   friend class boost::iterator_core_access;

   void copy( const node_iterator& other );


   /** At each call, we build the successor iterator (making a call to {@link
    *  BVGraph#successors(int, InputBitStream, int[][], int[], int[])}, and we completely
    *  iterate over it, filling the appropriate entry in <code>window</code>.
    */
   void increment();

//    int nextInt() {
//       if ( ! hasNext() ) throw new NoSuchElementException();
      
//       final int currIndex = ++curr % cyclicBufferSize;
//       final IntIterator i = BVGraph.this.successors( curr, ibs, window, outd, blockOutdegrees );
      
//       if ( window[ currIndex ].length < outd[ currIndex ] ) window[ currIndex ] = new int[ outd[ currIndex ] ];
//       IntIterators.unwrap( i, window[ currIndex ] );
      
//       return curr;
//    }
   
   int dereference() const {
      return curr;
   }

   bool equal( const node_iterator& rhs ) const;

public:
   typedef webgraph::bv_graph::iterator_wrappers::java_to_cpp<int> succ_itor_wrapper;
   friend std::pair<succ_itor_wrapper, succ_itor_wrapper> successors( node_iterator& rhs );

   friend std::vector<int> successor_vector( const node_iterator& rhs );
   friend int outdegree( const node_iterator& rhs );
   friend class graph;
};

int outdegree( const node_iterator& itor );

} }

   
#endif
