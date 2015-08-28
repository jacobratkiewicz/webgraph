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

#ifndef BVG_BOOST_OUT_EDGE_ITERATOR_HPP
#define BVG_BOOST_OUT_EDGE_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/tuple/tuple.hpp>
#include "types.hpp"


namespace webgraph { namespace bv_graph { namespace boost_integration {
   class out_edge_iterator : public boost::iterator_facade<out_edge_iterator,
                                                           edge_descriptor,
                                                           boost::forward_traversal_tag,
                                                           edge_descriptor> {
   private:
      vertex_descriptor v;
      webgraph::bv_graph::graph::successor_iterator s, s_end;
      edge_descriptor cur_edge;

   public:
      out_edge_iterator( const vertex_descriptor& v,
                         const webgraph::bv_graph::graph& g ) {
         boost::tie( s, s_end ) = g.get_successors( v );
         cur_edge = make_pair( v, *s );
      }

      out_edge_iterator() {}

      friend class boost::iterator_core_access;
   private:
      void increment() {
         ++s;
         cur_edge = make_pair( v, *s );
      }

      edge_descriptor dereference() const {
         return cur_edge;
      }
      
      bool equal( const out_edge_iterator& other ) const {
         return 
            (s == s_end && other.s == other.s_end) ||
            (s == other.s && s_end == other.s_end);
      }
   };
}}}

#endif
