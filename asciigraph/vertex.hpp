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

#ifndef ASCIIGRAPHNODE_HPP
#define ASCIIGRAPHNODE_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>

namespace webgraph { namespace ascii_graph {

typedef unsigned int vertex_label_t;

// A vertex is described by both its label (v) and its successors
class vertex_descriptor {
private:
   vertex_label_t label;
   std::vector<vertex_label_t> successors;
   bool __successors_loaded;

public:
   ////////////////////////////////////////////////////////////
   vertex_descriptor() { __successors_loaded = false; }
   
   ////////////////////////////////////////////////////////////
   vertex_descriptor( vertex_label_t l ) {
      label = l;
      __successors_loaded = false;
   }

   ////////////////////////////////////////////////////////////
   operator vertex_label_t() const {
      return label;
   }

   ////////////////////////////////////////////////////////////
   friend std::ostream& operator << (std::ostream& out, const vertex_descriptor& v );
   
   ////////////////////////////////////////////////////////////
   bool operator == ( const vertex_descriptor& rhs ) const {
      return label == rhs.label && successors == rhs.successors;
   }

   ////////////////////////////////////////////////////////////
   bool successors_loaded() const { return __successors_loaded; };
   void successors_loaded( bool sl ) { __successors_loaded = sl; }

   ////////////////////////////////////////////////////////////
   const vertex_label_t& get_label() const { return label; }
   vertex_label_t& label_ref() { return label; }

   ////////////////////////////////////////////////////////////
   const std::vector<vertex_label_t>& get_successors() const {
      if( __successors_loaded ) {
         return successors;
      } else {
         throw 
          std::logic_error( "Attempt to get successors of descriptor which does not have them loaded." );
      }
   }

   ////////////////////////////////////////////////////////////
   std::vector<vertex_label_t>& successors_ref() {
      __successors_loaded = true;
      return successors;
   }

   std::string as_str() const;
};

} }

#endif
