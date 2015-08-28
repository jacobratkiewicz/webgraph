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

#ifndef OFFLINEASCIIGRAPHEDGEITERATOR_HPP_
#define OFFLINEASCIIGRAPHEDGEITERATOR_HPP_

#include <utility>
#include <sstream>
#include <boost/iterator/iterator_facade.hpp>

#include "offline_vertex_iterator.hpp"

#include "vertex.hpp"
#include "edge.hpp"

//#define DEBUG 0
#include <iostream>

namespace webgraph { namespace ascii_graph {
   using namespace std;

   class offline_edge_iterator : 
      public boost::iterator_facade< 
      offline_edge_iterator,
      edge const,
      boost::forward_traversal_tag
      >
   {
   private:
      edge current_edge;

      int current_successor_index;

      offline_vertex_iterator current_vertex;
      offline_vertex_iterator last_vertex;

      bool is_end_marker;
      
      //! for sanity checks
      unsigned long num_vertices;

      void fetch_next_edge();
   public:	
      offline_edge_iterator();
      explicit offline_edge_iterator(const char* filename);
      offline_edge_iterator(const offline_edge_iterator& rhs);
      virtual ~offline_edge_iterator();

      offline_edge_iterator& operator = ( const offline_edge_iterator& rhs );

   private:
      // adapter methods
      friend class boost::iterator_core_access;

      void increment() {
         fetch_next_edge();
      }

      bool equal( const offline_edge_iterator& other ) const {
         // this assumes they are from the same graph.
#ifdef DEBUG
         cerr << "Comparing for equality between\n"
              << this->as_str() << endl
              << " and " << endl
              << other.as_str() << endl;
         bool val = (this->is_end_marker && other.is_end_marker) || 
                    (current_edge == other.current_edge && !this->is_end_marker && !other.is_end_marker);
                    
         cerr << "will return " << val << endl;
#endif         
         
         return (this->is_end_marker && other.is_end_marker) || 
                (current_edge == other.current_edge && 
                 !this->is_end_marker && !other.is_end_marker);
      }

      const edge& dereference() const {
         return current_edge;
      }

   public:
      /*! For debugging
       * returns a string representation of this iterator
       */
//       string as_str() const {
//          ostringstream o;

//          o << "ascii_graph::offline_edge_iterator - "
//            << "\tcurrent_edge: " << "<" << current_edge.first << ", " 
//            << current_edge.second << ">\n"
//            << "\tcurrent_vertex: " << current_vertex.as_str() << "\n"
//            << ")\n";
		
//          return o.str();
//       }
   };

} }

#endif /*OFFLINEASCIIGRAPHEDGEITERATOR_HPP_*/
