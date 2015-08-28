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

#ifndef UTILITY_ITERATOR_BASE_HPP
#define UTILITY_ITERATOR_BASE_HPP

#include <utility>
#include <string>

namespace webgraph { namespace bv_graph { namespace utility_iterators {
   // nothing here.. just serves as a polymorphic base class
   template<typename val_type>
   class utility_iterator_base {
   public:
      utility_iterator_base() {}
      virtual ~utility_iterator_base() {}

      virtual val_type next() = 0;
      virtual bool has_next() const = 0;
      virtual int skip( int how_many ) = 0;

      virtual std::string as_str() const = 0;

      // need this for the C++ pass-by-value idiom to work with polymorphism
      virtual utility_iterator_base<val_type>* clone() const = 0;
   };
} } }

#endif
