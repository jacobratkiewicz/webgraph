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


#include "../output_bitstream.hpp"
#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <algorithm>

/** A debugging wrapper for output bit streams.
 *
 * <P>This class can be used to wrap an output bit stream. The semantics of the
 * resulting write operations is unchanged, but each operation will be logged.
 *
 * <P>To simplify the output, some operations have a simplified representation. In particular:
 *
 * <dl>
 * <dt><samp>|</samp>
 * <dd>{@link OutputBitStream#flush() flush()};
 * <dt><samp>-></samp>
 * <dd>{@link OutputBitStream#position(long) position()};
 * <dt><samp>[</samp>
 * <dd>creation;
 * <dt><samp>]</samp>
 * <dd>{@link OutputBitStream#close() close()};
 * <dt><samp>{<var>x</var>}</samp>
 * <dd>explicit bits;
 * <dt><samp>{<var>x</var>:<var>b</var>}</samp>
 * <dd>minimal binary coding of <var>x</var> with bound <var>b</var>;
 * <dt><samp>{<var>M</var>:<var>x</var>}</samp>
 * <dd>write <var>x</var> with coding <var>M</var>; the latter can be U (unary), g (&gamma;), z (&zeta;), d (&delta;), G (Golomb), GS (skewed Golomb);
 * when appropriate, <var>x</var> is followed by an extra integer (modulus, etc.).
 * </dl>
 *
 * @author Paolo Boldi
 * @author Sebastiano Vigna
 * @since 0.7.1
 */
namespace webgraph {

   class debug_obitstream : public obitstream {
      obitstream& obs;
      std::ofstream& out;

public:
   /** Creates a new debug output bit stream wrapping a given output bit stream and logging on a given writer.
    *
    * @param obs the output bit stream to wrap.
    * @param pw a print stream that will receive the logging data.
    */
   debug_obitstream( obitstream& os, std::ofstream& o ) : 
      obs(os), out(o) {
      out << "[";
   }
   
   void flush() {
      out << " |";
      obs.flush();
   }

//    void close()  {
//       out << " |]";
//       obs.close();
//    }

   int align() {
      out << " |";
      return obs.align();
   }

//    void position( final long position ) throws IOException {
//       pw.print( " ." + position );
//       obs.position( position );
//    }

private:
   static std::string byte_to_binary( int x ) {
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

   static std::string int_to_binary( long x, int len ) {
      std::string s;

      for( int i = 0 ; i < 64; i++ ) {
         s += (char)( '0' + ( x % 2 ) );
         x >>= 1;
      }
   
      s = s.substr( 0, len );
      std::reverse( s.begin(), s.end() );

      return s;
   }

// public int write( byte bits[], int len ) {
//    MutableString s = new MutableString( " {" );
//    for( int i = 0; i < bits.length; i++ ) s.append( byte2Binary( bits[ i ] ) );
//    pw.print( s.length( len ).append( "}" ) );
//    return obs.write( bits, len );
// }

// public int writeBit( boolean bit ) throws java.io.IOException {
//    pw.print( " {" + ( bit ? '1' : '0' ) + "}" );
//    return obs.writeBit( bit );
// }

// public int writeBit( int  bit ) throws java.io.IOException {
//    pw.print( " {" + bit + "}" );
//    return obs.writeBit( bit );
// }

public:
    int write_int( int x, int len ) {
       out << " {" << int_to_binary( x, len ) << "}" << "\n";
       return obs.write_int( x, len );
    }

// public int writeLong( long x, int len ) throws java.io.IOException {
//    pw.print( " {" + int2Binary( x, len ) + "}" );
//    return obs.writeLong( x, len );
// }

   int write_unary( int x ) {
      out << " {U:" << x << "}" << "\n";
      return obs.write_unary( x );
   }

   int write_gamma( int x ) {
      out << " {g:" << x << "}" << "\n";
      return obs.write_gamma( x );
   }

   int write_delta( int x ) {
      out << " {d:" << x << "}" << "\n";
      return obs.write_delta( x );
   }

   int write_zeta( int x, int k ) {
      out << " {z" << k << ":" << x << "}" << "\n";
      return obs.write_zeta( x, k );
   }
};

}
