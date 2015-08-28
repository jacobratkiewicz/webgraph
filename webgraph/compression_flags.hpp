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

#ifndef COMPRESSION_FLAGS_HPP_
#define COMPRESSION_FLAGS_HPP_

namespace webgraph {
	namespace compression_flags {
		const int DELTA = 1;
  		const int GAMMA = 2;
  
//  public static final int GOLOMB = 3;
//  
//  public static final int SKEWED_GOLOMB = 4;
//  
//  public static final int ARITH = 5;
//  
//  public static final int INTERP = 6;
//  
		const int UNARY = 7;
  		const int ZETA = 8;
  		const int NIBBLE = 9;
//  
//  public static final int NONE = 255;
	
		extern const char* CODING_NAME[]; // = [ "something", "something_else" ];

	};
};

#endif /*COMPRESSION_FLAGS_HPP_*/
