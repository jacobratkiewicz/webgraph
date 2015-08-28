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

#include <string>
#include <algorithm>
#include <sstream>

namespace utils {

/** Maps integers bijectively into natural numbers.
 * 
 * <P>This method will map a negative integer <var>x</var> to -2<var>x</var>-1 and
 * a nonnegative integer <var>x</var> to 2<var>x</var>. It can be used to save arbitrary
 * integers using the standard coding methods (which all work on natural numbers).
 * 
 * <P>The inverse of the above map is computed by {@link #nat2int(int)}
 *
 * @param x an integer.
 * @return the argument mapped into natural numbers.
 * @see #nat2int(int)
 */

int int2nat( int x ) {
   return x >= 0 ? x << 1 : ( -x << 1 ) - 1;
}

/** Maps natural numbers bijectively into integers.
 * 
 * <P>This method computes the inverse of {@link #int2nat(int)}
 *
 * @param x a natural  number.
 * @return the argument mapped into an integer.
 * @see #int2nat(int)
 */

int nat2int( int x ) {
   return x % 2 == 0 ? x >> 1 : -( ( x + 1 ) >> 1 );
}


std::string byte_to_binary( int x ) {
  std::string s;
  
  for( int i = 0 ; i < 8; i++ ) {
    char str[] = {0,0};
    str[0] = char( '0' + (x % 2) );
    
    s += str;

    x >>= 1;
  }
  
  std::reverse(s.begin(), s.end());
  
  return s;
}

std::string int_to_binary( long x, int len ) {
  std::string s;
  
  for( int i = 0 ; i < 64; i++ ) {
    s += (char)( '0' + ( x % 2 ) );
    x >>= 1;
      }
  
  s = s.substr( 0, len );
  std::reverse( s.begin(), s.end() );

  for( int i = s.size() - 4; i > 0; i -= 4 ) {
     s.insert( i, " " ); 
  }

  return s;
}

std::string byte_as_hex( int b ) {
   using namespace std;
   ostringstream oss;

   oss << hex << b;

   string str = oss.str().substr(0,2);
   return str;
}

}
