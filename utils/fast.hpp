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

#ifndef FAST_HPP
#define FAST_HPP

#include <string>
#include <sstream>

namespace utils {

int int2nat( int x );
int nat2int( int x );

std::string byte_to_binary( int x );
std::string int_to_binary( long x, int len );
std::string byte_as_hex( int byte );

template<class T>
std::string to_string( const T& thing ) {
   std::ostringstream o;
   
   o << thing;
   
   return o.str();
}

}

#endif

