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

#ifndef EMPTY_ITERATOR_HPP
#define EMPTY_ITERATOR_HPP

#include "utility_iterator_base.hpp"
#include <string>
#include <exception>
#include <stdexcept>

namespace webgraph { namespace bv_graph { namespace utility_iterators {

class empty_iterator : public utility_iterator_base<int> {
public:
   empty_iterator() {}

   int next() {
      throw std::logic_error( "Can't increment empty iterator." );
   }

   bool has_next() const {
      return false;
   }
   
   std::string as_str() const {
      return "Empty iterator.";
   }

   int skip( int how_many ) {
      throw std::logic_error( "Can't skip on an empty iterator." );
   }
   
   empty_iterator* clone() const {
      return new empty_iterator();
   }
};

}}}

#endif
