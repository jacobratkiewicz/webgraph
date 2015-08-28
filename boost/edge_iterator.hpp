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

#ifndef WEBGRAPH_BOOST_EDGE_ITERATOR_HPP
#define WEBGRAPH_BOOST_EDGE_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/tuple/tuple.hpp>
#include <cassert>

#include "types.hpp"
#include <webgraph/iterators/node_iterator.hpp>

// TODO this will have to change to work with offline graphs.

namespace webgraph { namespace bv_graph { namespace boost_integration {

   class edge_iterator : public boost::iterator_facade<edge_iterator,
                                                      edge_descriptor,
                                                      boost::forward_traversal_tag,
                                                      edge_descriptor> {
   private:
      // data members
      webgraph::bv_graph::graph::node_iterator v, v_end;
      webgraph::bv_graph::graph::successor_iterator s, s_end;
      vertex_descriptor current_vertex;
      bool end_marker;
      
   public:
      // interface
      edge_iterator( const webgraph::bv_graph::graph& gg ) {

         tie( v, v_end ) = gg.get_node_iterator(0);

         assert( v != v_end );

         do {
            
            tie( s, s_end ) = successors( v );
            current_vertex = *v;
            ++v;
         } while( s == s_end && v != v_end );


         end_marker = s == s_end; // shouldn't happen...
      }
      
      edge_iterator() {
         end_marker = true;
      }

      friend class boost::iterator_core_access;
   private:
      ////////////////////////////////////////////////////////////////////////////////
      void increment() {
         ++s;

         while( s == s_end && v != v_end ) {
            tie( s, s_end ) = successors( v );
            current_vertex = *v;
            ++v;
         }

         end_marker = s == s_end;
      }

      ////////////////////////////////////////////////////////////////////////////////
      edge_descriptor dereference() const {
         return make_pair( current_vertex, *s );
      }

      ////////////////////////////////////////////////////////////////////////////////
      bool equal( const edge_iterator& other ) const {
         if( end_marker && other.end_marker ) {
            return true;
         } else {
            return v == other.v && s == other.s;
         }
      }
   };
}}}

#endif

