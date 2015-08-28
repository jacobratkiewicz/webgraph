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

#ifndef OFFLINEVERTEXITERATOR_HPP_
#define OFFLINEVERTEXITERATOR_HPP_

#include "../webgraph/webgraph_vertex.hpp"

#include "vertex.hpp"

#include <string>
#include <sstream>
#include <boost/iterator/iterator_facade.hpp>
#include <fstream>
#include <cassert>

#include <iostream>

namespace webgraph { namespace ascii_graph {

   class offline_vertex_iterator : public boost::iterator_facade<
      offline_vertex_iterator,
      vertex_descriptor,
      boost::forward_traversal_tag,
      vertex_descriptor
    >
   {
   ////////////////////////////////////////////////////////////////////////////////
   // private section
   private:
      ////////////////////////////////////////////////////////////////////////////////
      // data members
      vertex_descriptor current_descriptor;
//      std::vector<vertex> current_successors;
      std::ifstream back;
      std::string filename;
      size_t get_pos;
      bool end_marker;
      unsigned int num_vertices;
      
      void init() {
//         current_descriptor = vertex_descriptor_t();
         get_pos = 0;
         end_marker = false;
         num_vertices = 0;   
      }
      
      void copy( const offline_vertex_iterator& other );	
   ////////////////////////////////////////////////////////////////////////////////
   // public section
   public:
      ////////////////////////////////////////
      offline_vertex_iterator();

      ////////////////////////////////////////
      offline_vertex_iterator( const offline_vertex_iterator& that );

      ////////////////////////////////////////
      explicit offline_vertex_iterator(const char* filename);
	    
      virtual ~offline_vertex_iterator();

      ////////////////////////////////////////
      string as_str() const;

      ////////////////////////////////////////
      offline_vertex_iterator& operator = ( const offline_vertex_iterator& rhs );
      
      friend int outdegree( const offline_vertex_iterator& me );
      friend const std::vector<vertex_label_t>& successors( const offline_vertex_iterator& me );
	
   ////////////////////////////////////////////////////////////////////////////////
   // Backing methods for iterator_facade
   private:
      friend class boost::iterator_core_access;

      void increment();

      bool equal( const offline_vertex_iterator& rhs ) const {
         // assumes they're both from the same graph.
         // TODO make this a bit more robust.
         
#if 0
         cerr << "Comparison between " << endl
              << this->as_str() << endl
              << " and " << endl
              << rhs.as_str() << endl;
              
         bool val = this->current_descriptor == rhs.current_descriptor || (this->end_marker && rhs.end_marker);
         
         cerr << "Will return : " << val << endl;
#endif
         
         return ((this->current_descriptor.get_label() == rhs.current_descriptor.get_label()) && 
                 !(this->end_marker || rhs.end_marker))
                || (this->end_marker && rhs.end_marker);
      }

      vertex_descriptor dereference() const {
         return current_descriptor;
      }
   };

   int outdegree( const offline_vertex_iterator& me );
   const std::vector<vertex_label_t>& successors( const offline_vertex_iterator& me );

} }

#endif /*OFFLINEVERTEXITERATOR_HPP_*/
