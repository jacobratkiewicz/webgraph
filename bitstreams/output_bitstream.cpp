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

#include "output_bitstream.hpp"

#include <iostream>

namespace webgraph {
using namespace std;
#ifndef CONFIG_FAST
using namespace logs;
#endif

/** Writes a byte to the stream.
 *
 * <P>This method takes care of managing the buffering logic transparently.
 *
 * <P>However, this method does <em>not</em> update {@link #writtenBits}.
 * The caller should increment {@link #writtenBits} by 8 at each call.
 */
void obitstream::write( const int b ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "Write called on byte " << b << "\n";
#endif

   if ( avail-- == 0 ) {
      assert( os != NULL );
         
      if ( no_buffer ) {
         os->write( (char*)&b, sizeof(b) );
         position++;
         avail = 0;
         return;
      }
      
      // just to make the following a little cleaner..
      vector<byte>& buf = *buffer;
      
      os->write( (char*)&buf[0], buf.size() );
      position += buf.size();
      avail = buf.size() - 1;
      pos = 0;
   }
      
   (*buffer)[ pos++ ]  = (byte)b;
}


/** Writes bits in the bit buffer, possibly flushing it.
 *
 * You cannot write more than {@link #free} bits with this method. However,
 * after having written {@link #free} bits the bit buffer will be empty. In
 * particular, there should never be 0 free bits in the buffer.
 *
 * @param b the bits to write in the <strong>lower</strong> positions; 
 * the remaining positions must be zero.
 * @param len the number of bits to write (0 is safe and causes no action).
 * @return the number of bits written.
 */

int obitstream::write_in_current( const int b, const int len ) {
#ifndef CONFIG_FAST
   logger("obs") << LEVEL_DEBUG << "write_in_current( " << b << ", " << len << " )\n";
#endif
   current |= ( b & ( ( 1 << len ) - 1 ) ) << ( free -= len );
   if ( free == 0 ) {
      write( current );
      free = 8;
      current = 0;
   }
      
   written_bits += len;
   return len;
}

/** Sets this stream bit position, if it is based on a {@link RepositionableStream} or
 * on a {@link java.nio.channels.FileChannel}.
 *
 * <P>Given an underlying stream that implements {@link
 * RepositionableStream} or that can provide a {@link
 * java.nio.channels.FileChannel} via the <code>getChannel()</code> method,
 * a call to this method has the same semantics of a {@link #flush()},
 * followed by a call to {@link
 * java.nio.channels.FileChannel#position(long) position(position / 8)} on
 * the byte stream. Currently there is no clean, working way of supporting
 * out-of-byte-boundary positioning.
 *
 * @param position the new position expressed as a bit offset; it must be byte-aligned.
 * @throws IllegalArgumentException when trying to position outside of byte boundaries.
 * @throws UnsupportedOperationException if the underlying byte stream does not implement
 */

void obitstream::set_position( const long position ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "set_position( " << position << " )\n";
#endif
   assert( position >= 0 );
   assert( ( position & 7 ) == 0 );
      
   if ( wrapping ) {
      assert( (unsigned long)position <= buffer->size() );
      flush();
      free = 8;
      pos = (int)position;
      avail = buffer->size() - pos;
   } else {
      flush();
      if ( position >> 3 != this->position ) 
         os->seekp( this->position = position >> 3 );
   }
}

/** Writes a sequence of bits, starting from a given offset.
 *
 * Bits will be written in the natural way: the first bit is bit 7 of the
 * first byte, the eightth bit is bit 0 of the first byte, the ninth bit is
 * bit 7 of the second byte and so on.
 *
 * @param bits a vector containing the bits to be written.
 * @param offset a bit offset from which to start to write.
 * @param len a bit length.
 * @return the number of bits written (<code>len</code>).
 */

int obitstream::write( const byte bits[], const int offset, const int len ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write( bits, offset, len )\n";
#endif

   const int initial = 8 - ( offset & 0x7 );
   if ( initial == 8 ) 
      return write_byte_offset( bits, offset / 8, len );
   if ( len <= initial ) 
      /// used to be >>>
      return write_int( ( 0xFF & bits[ offset / 8 ] ) >> ( initial - len ), len );
   else
      return write_int( bits[ offset / 8 ], initial ) + 
         write_byte_offset( bits, offset / 8 + 1, len - initial );
}


/** Writes a sequence of bits, starting from a given byte offset.
 *
 * Bits will be written in the natural way: the first bit is bit 7 of the
 * first byte, the eightth bit is bit 0 of the first byte, the ninth bit is
 * bit 7 of the second byte and so on.
 *
 * <p>This method is used to support methods such as {@link #write(byte[],int,int)}.
 *
 * @param bits a vector containing the bits to be written.
 * @param offset an offset, expressed in <strong>bytes</strong>.
 * @param len a bit length.
 * @return the number of bits written (<code>len</code>).
 */

int obitstream::write_byte_offset( const byte bits[], 
                                   const int offset, int len ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write_byte_offset( bits, offset, len )\n";
#endif

//    	for( int i = 0; i < len/8; i++ )
//    		cerr << (int)bits[i] << " ";
   		
//    	cerr << endl;
      
   if ( len == 0 ) return 0;
   if ( len <= free ) {
      /// used to be >>>
      // this is okay because bits[] is unsigned, so 0's will be shifted in
      return write_in_current( bits[ offset ] >> 8 - len, len );
   }
   else {
      const int shift = free;
      int i, j;
         
//      cerr << "shift = " << shift << endl;
         
      // used to be >>>
      write_in_current( bits[ offset ] >> 8 - shift, shift );
         
      len -= shift;
         
      j = offset;
      i = len >> 3;
      while( i-- != 0 ) {
         // used to be >>>
         write( bits[ j ] << shift | ( bits[ j + 1 ] & 0xFF ) >> 8 - shift );
         written_bits += 8;
         j++;
      }
         
      const int q = len & 7;
      if ( q != 0 ) {
         if ( q <= 8 - shift ) {
            /// used to be >>>
            write_in_current( bits[ j ] >> 8 - shift - q, q );
         }
         else {
            write_in_current( bits[ j ], 8 - shift );
            /// used to be >>>
            write_in_current( bits[ j + 1 ] >> 16 - q - shift, q + shift - 8 );
         }
      }
         
      return len + shift;
   }
}

/** Writes a fixed number of bits from an integer.
 *
 * @param x an integer.
 * @param len a bit length; this many lower bits of the first argument will be written
 * (the most significant bit first).
 * @return the number of bits written (<code>len</code>).
 */

int obitstream::write_int( int x, const int len ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write_int( " << x << ", " << len << " )\n";
#endif

   assert( len >= 0 && len <= 32 );

   if ( len <= free ) 
      return write_in_current( x, len );

   const int q = ( len - free ) & 7, blocks = ( len - free ) >> 3;
   int i = blocks;

   if ( q != 0 ) {
      temp_buffer[ blocks ] = (byte)x;
      x >>= q;
   }

   while( i-- != 0 ) {
      temp_buffer[ i ] = (byte)x;
      // used to be >>>
      x >>= 8;
   }

   write_in_current( x, free );

   for( i = 0; i < blocks; i++ ) 
//      write( &temp_buffer[ i ], sizeof( temp_buffer[i] )  );
      write( temp_buffer[i] );

   written_bits += blocks << 3;

   if ( q != 0 ) 
      write_in_current( temp_buffer[ blocks ], q );

   return len;
}

/** Writes a natural number in unary coding.
 *
 * Note that by unary coding we mean that 1 encodes 0, 01 encodes 1 and so on.
 *
 * @param x a natural number.
 * @return the number of bits written.
 * @throws IllegalArgumentException if you try to write a negative number.
 */

int obitstream::write_unary( int x ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write_unary( " << x << " )\n";
#endif

   assert( x >= 0 );
      
   if ( x < free ) {
#ifndef CONFIG_FAST
      logger("obs") << LEVEL_EVERYTHING << "Doing simple unary write.\n";
#endif
      return write_in_current( 1, x + 1 );
   }

#ifndef CONFIG_FAST      
   logger("obs") << LEVEL_EVERYTHING << "Doing more complicated unary write.\n";
#endif

   const int shift = free;
   x -= shift;
      
   written_bits += shift;
   write( current );
   free = 8;
   current = 0;
      
   int i = x >> 3;
      
   written_bits += ( x & 0x7FFFFFF8 );
      
   while( i-- != 0 ) 
      write( 0 );
      
   write_in_current( 1, ( x & 7 ) + 1 );
      
   return x + shift + 1;
}

//    /** Writes a long natural number in unary coding.
//     *
//     * @see #writeUnary(int)
//     *
//     * @param x a long natural number.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you write to write a negative number.
//     */

// public long writeLongUnary( long x ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );

//    if ( x < free ) return writeInCurrent( 1, (int)x + 1 );

//    final int shift = free;
//    x -= shift;

//    writtenBits += shift;
//    write( current );
//    free = 8;
//    current = 0;

//    long i = x >> 3;

//    writtenBits += ( x & 0x7FFFFFFFFFFFFFF8L );

//    while( i-- != 0 ) write( 0 );

//    writeInCurrent( 1, (int)( x & 7 ) + 1 );

//    return x + shift + 1;
// }

int obitstream::most_significant_bit( const int x ) {
   return
      ( x < 1<<15 ?
        ( x < 1<<7 ?
          ( x < 1<<3 ?
            ( x < 1<<1 ?
              ( x < 1<<0 ?
                x < 0 ? 31 : -1 /* 6 */
                :
                0 /* 5 */
                 )
              :
              ( x < 1<<2 ?
                1 /* 5 */
                :
                2 /* 5 */
                 )
               )
            :
            ( x < 1<<5 ?
              ( x < 1<<4 ?
                3 /* 5 */
                :
                4 /* 5 */
                 )
              :
              ( x < 1<<6 ?
                5 /* 5 */
                :
                6 /* 5 */
                 )
               )
             )
          :
          ( x < 1<<11 ?
            ( x < 1<<9 ?
              ( x < 1<<8 ?
                7 /* 5 */
                :
                8 /* 5 */
                 )
              :
              ( x < 1<<10 ?
                9 /* 5 */
                :
                10 /* 5 */
                 )
               )
            :
            ( x < 1<<13 ?
              ( x < 1<<12 ?
                11 /* 5 */
                :
                12 /* 5 */
                 )
              :
              ( x < 1<<14 ?
                13 /* 5 */
                :
                14 /* 5 */
                 )
               )
             )
           )
        :
        ( x < 1<<23 ?
          ( x < 1<<19 ?
            ( x < 1<<17 ?
              ( x < 1<<16 ?
                15 /* 5 */
                :
                16 /* 5 */
                 )
              :
              ( x < 1<<18 ?
                17 /* 5 */
                :
                18 /* 5 */
                 )
               )
            :
            ( x < 1<<21 ?
              ( x < 1<<20 ?
                19 /* 5 */
                :
                20 /* 5 */
                 )
              :
              ( x < 1<<22 ?
                21 /* 5 */
                :
                22 /* 5 */
                 )
               )
             )
          :
          ( x < 1<<27 ?
            ( x < 1<<25 ?
              ( x < 1<<24 ?
                23 /* 5 */
                :
                24 /* 5 */
                 )
              :
              ( x < 1<<26 ?
                25 /* 5 */
                :
                26 /* 5 */
                 )
               )
            :
            ( x < 1<<29 ?
              ( x < 1<<28 ?
                27 /* 5 */
                :
                28 /* 5 */
                 )
              :
              ( x < 1<<30 ?
                29 /* 5 */
                :
                30 /* 5 */
                 )
               )
             )
           )
         );
}

/** Writes a natural number in &gamma; coding.
 *
 * The &gamma; coding of a positive number of <var>k</var> bits is
 * obtained writing <var>k</var>-1 in unary, followed by the lower
 * <var>k</var>-1 bits of the number. The coding of a natural number is
 * obtained by adding one and coding.
 *
 * @param x a natural number.
 * @return the number of bits written.
 * @throws IllegalArgumentException if you write to write a negative number.
 */

int obitstream::write_gamma( int x ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write_gamma( " << x << " )\n";
#endif
   assert( x >= 0 );
   
   const int msb = most_significant_bit( ++x );

#ifndef CONFIG_FAST
   logger("obs") << LEVEL_EVERYTHING << "\tmsb = " << msb << "\n";
#endif

   const int l = write_unary( msb );
   return l + ( msb != 0 ? write_int( x, msb ) : 0 );
}

//    /** Writes a long natural number in &gamma; coding.
//     *
//     * @see #writeGamma(int)
//     *
//     * @param x a long natural number.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you write to write a negative number.
//     */

// public int writeLongGamma( long x ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );

//    final int msb = Fast.mostSignificantBit( ++x );
//    final int l = writeUnary( msb );
//    return l + ( msb != 0 ? writeLong( x, msb ) : 0 );
// }

/** Writes a natural number in &delta; coding.
 *
 * The &delta; coding of a positive number of <var>k</var> bits is
 * obtained writing <var>k</var>-1 in &gamma; coding, followed by the
 * lower <var>k</var>-1 bits of the number. The coding of a natural
 * number is obtained by adding one and coding.
 *
 * @param x a natural number.
 * @return the number of bits written.
 * @throws IllegalArgumentException if you write to write a negative number.
 */

int obitstream::write_delta( int x ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write_delta( " << x << " )\n";
#endif

   assert( x >= 0 );
   
   const int msb = most_significant_bit( ++x );
   const int l = write_gamma( msb );
   return l + ( msb != 0 ? write_int( x, msb ) : 0 );
}

//    /** Writes a long natural number in &delta; coding.
//     *
//     * @see #writeDelta(int)
//     *
//     * @param x a long natural number.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you write to write a negative number.
//     * @deprecated As of <code>MG4J</code> 0.2, replaced by {@link #writeLongDelta(long)}.
//     */

// public int writeDelta( long x ) throws IOException {
//    return writeLongDelta( x );
// }

//    /** Writes a long natural number in &delta; coding.
//     *
//     * @see #writeDelta(int)
//     *
//     * @param x a long natural number.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you write to write a negative number.
//     */

// public int writeLongDelta( long x ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );

//    final int msb = Fast.mostSignificantBit( ++x );
//    final int l = writeGamma( msb );
//    return l + ( msb != 0 ? writeLong( x, msb ) : 0 );
// }

//    /** Writes a natural number in a limited range using a minimal binary coding.
//     *
//     * @param x a natural number.
//     * @param b a strict upper bound for <code>x</code>.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a nonpositive base.
//     */

// public int writeMinimalBinary( final int x, final int b ) throws IOException {
//    if ( b < 1 ) throw new IllegalArgumentException( "The bound " + b + " is not positive" );

//    return writeMinimalBinary( x, b, Fast.mostSignificantBit( b ) );
// }

//    /** Writes a natural number in a limited range using a minimal binary coding.
//     *
//     * This method is faster than {@link #writeMinimalBinary(int,int)} because it does not
//     * have to compute <code>log2b</code>.
//     *
//     * @param x a natural number.
//     * @param b a strict upper bound for <code>x</code>.
//     * @param log2b the floor of the base-2 logarithm of the bound.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a nonpositive base.
//     */

// public int writeMinimalBinary( final int x, final int b, final int log2b ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );
//    if ( b < 1 ) throw new IllegalArgumentException( "The bound " + b + " is not positive" );
//    if ( x >= b ) throw new IllegalArgumentException( "The argument " + x + " exceeds the bound " + b );

//    // Numbers smaller than m are encoded in log2b bits.
//    final int m = ( 1 << log2b + 1 ) - b; 

//    if ( x < m ) return writeInt( x, log2b );
//    else return writeInt( m + x, log2b + 1 );
// }


//    /** Writes a long natural number in a limited range using a minimal binary coding.
//     *
//     * @param x a natural number.
//     * @param b a strict upper bound for <code>x</code>.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a nonpositive base.
//     */

// public int writeLongMinimalBinary( final long x, final long b ) throws IOException {
//    if ( b < 1 ) throw new IllegalArgumentException( "The bound " + b + " is not positive" );

//    return writeLongMinimalBinary( x, b, Fast.mostSignificantBit( b ) );
// }


//    /** Writes a long natural number in a limited range using a minimal binary coding.
//     *
//     * This method is faster than {@link #writeLongMinimalBinary(long,long)} because it does not
//     * have to compute <code>log2b</code>.
//     *
//     * @param x a long natural number.
//     * @param b a strict upper bound for <code>x</code>.
//     * @param log2b the floor of the base-2 logarithm of the bound.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a nonpositive base.
//     */

// public int writeLongMinimalBinary( final long x, final long b, final int log2b ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );
//    if ( b < 1 ) throw new IllegalArgumentException( "The bound " + b + " is not positive" );
//    if ( x >= b ) throw new IllegalArgumentException( "The argument " + x + " exceeds the bound " + b );

//    // Numbers smaller than m are encoded in log2b bits.
//    final long m = ( 1 << log2b + 1 ) - b; 

//    if ( x < m ) return writeLong( x, log2b );
//    else return writeLong( m + x, log2b + 1 );
// }



//    /** Writes a natural number in Golomb coding.
//     *
//     * <P>This method implements also the case in which <code>b</code> is 0: in this case,
//     * the argument <code>x</code> may only be zero, and nothing will be written. 
//     *
//     * @param x a natural number.
//     * @param b the modulus for the coding.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a negative modulus.
//     */

// public int writeGolomb( final int x, final int b ) throws IOException {
//    return writeGolomb( x, b, Fast.mostSignificantBit( b ) );
// }

//    /** Writes a natural number in Golomb coding.
//     *
//     * This method is faster than {@link #writeGolomb(int,int)} because it does not
//     * have to compute <code>log2b</code>.
//     *
//     * <P>This method implements also the case in which <code>b</code> is 0: in this case,
//     * the argument <code>x</code> may only be zero, and nothing will be written. 
//     *
//     * @param x a natural number.
//     * @param b the modulus for the coding.
//     * @param log2b the floor of the base-2 logarithm of the coding modulus (it is irrelevant when <code>b</code> is zero).
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a negative modulus.
//     */

// public int writeGolomb( final int x, final int b, final int log2b ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );
//    if ( b < 0 ) throw new IllegalArgumentException( "The modulus " + b + " is negative" );
//    if ( b == 0 ) {
//       if ( x != 0 ) throw new IllegalArgumentException( "The modulus is 0, but the argument is " + x );
//       return 0;
//    }

//    final int l = writeUnary( x / b );

//    // The remainder to be encoded.
//    return l + writeMinimalBinary( x % b, b, log2b );
// }



//    /** Writes a long natural number in Golomb coding.
//     *
//     * <P>This method implements also the case in which <code>b</code> is 0: in this case,
//     * the argument <code>x</code> may only be zero, and nothing will be written. 
//     *
//     * @param x a long natural number.
//     * @param b the modulus for the coding.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a negative modulus.
//     */

// public long writeLongGolomb( final long x, final long b ) throws IOException {
//    return writeLongGolomb( x, b, Fast.mostSignificantBit( b ) );
// }

//    /** Writes a long natural number in Golomb coding.
//     *
//     * This method is faster than {@link #writeLongGolomb(long,long)} because it does not
//     * have to compute <code>log2b</code>.
//     *
//     * <P>This method implements also the case in which <code>b</code> is 0: in this case,
//     * the argument <code>x</code> may only be zero, and nothing will be written. 
//     *
//     * @param x a long natural number.
//     * @param b the modulus for the coding.
//     * @param log2b the floor of the base-2 logarithm of the coding modulus (it is irrelevant when <code>b</code> is zero).
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a negative modulus.
//     */

// public long writeLongGolomb( final long x, final long b, final int log2b ) throws IOException {

//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );
//    if ( b < 1 ) throw new IllegalArgumentException( "The modulus " + b + " is not positive" );
//    if ( b == 0 ) {
//       if ( x != 0 ) throw new IllegalArgumentException( "The modulus is 0, but the argument is " + x );
//       return 0;
//    }

//    final long l = writeLongUnary( x / b );

//    // The remainder to be encoded.
//    return l + writeLongMinimalBinary( x % b, b, log2b );
// }


//    /** Writes a natural number in skewed Golomb coding.
//     *
//     * <P>This method implements also the case in which <code>b</code> is 0: in this case,
//     * the argument <code>x</code> may only be zero, and nothing will be written. 
//     *
//     * @param x a natural number.
//     * @param b the modulus for the coding.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a negative modulus.
//     */

// public int writeSkewedGolomb( final int x, final int b ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );
//    if ( b < 0 ) throw new IllegalArgumentException( "The modulus " + b + " is negative" );
//    if ( b == 0 ) {
//       if ( x != 0 ) throw new IllegalArgumentException( "The modulus is 0, but the argument is " + x );
//       return 0;
//    }

//    final int i = Fast.mostSignificantBit( x / b + 1 );
//    final int l = writeUnary( i );
//    final int M = ( ( 1 << i + 1 ) - 1 ) * b;
//    final int m = ( M / ( 2 * b ) ) * b;

//    return l + writeMinimalBinary( x - m, M - m );
// }

//    /** Writes a long natural number in skewed Golomb coding.
//     * 
//     * <P>This method implements also the case in which <code>b</code> is 0: in this case,
//     * the argument <code>x</code> may only be zero, and nothing will be written. 
//     *
//     * @param x a long natural number.
//     * @param b the modulus for the coding.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a negative modulus.
//     */

// public long writeLongSkewedGolomb( final long x, final long b ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );
//    if ( b < 0 ) throw new IllegalArgumentException( "The modulus " + b + " is negative" );
//    if ( b == 0 ) {
//       if ( x != 0 ) throw new IllegalArgumentException( "The modulus is 0, but the argument is " + x );
//       return 0;
//    }

//    final long i = Fast.mostSignificantBit( x / b + 1 );
//    final long l = writeLongUnary( i );
//    final long M = ( ( 1 << i + 1 ) - 1 ) * b;
//    final long m = ( M / ( 2 * b ) ) * b;

//    return l + writeLongMinimalBinary( x - m, M - m );
// }


/** Writes a natural number in &zeta; coding.
 *
 * <P>&zeta; coding (with modulo <var>k</var>) records positive numbers in
 * the intervals
 * [1,2<sup><var>k</var></sup>-1],[2<sup><var>k</var></sup>,2<sup><var>k</var>+1</sup>-1],&hellip;,[2<sup><var>hk</var></sup>,2<sup>(<var>h</var>+1)<var>k</var></sup>-1]
 * by coding <var>h</var> in unary, followed by a minimal binary coding of
 * the offset in the interval.  The coding of a natural number is obtained
 * by adding one and coding.
 * 
 * <P>A detailed analysis of &zeta; codes is given in 
 * <blockquote>Paolo
 * Boldi and Sebastiano Vigna, <a href="http://vigna.dsi.unimi.it/papers.php#BoVWFII"><em>The WebGraph Framework II: Codes for the World-Wide Web</em></a>.
 * </blockquote>
 *
 * @param x a natural number.
 * @param k the shrinking factor.
 * @return the number of bits written.
 * @throws IllegalArgumentException if you try to write a negative number or use a nonpositive shrinking factor.
 */

int obitstream::write_zeta( int x, const int k ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write_zeta( " << x << ", " << k << " )\n";
#endif

   assert( x >= 0 );
   assert( k > 0 );

   const int msb = most_significant_bit( ++x );
   const int h = msb / k;
   const int l = write_unary( h );
   const int left = 1 << h * k;
   return l + ( x - left < left 
                ? write_int( x - left, h * k + k - 1 ) 
                : write_int( x, h * k + k ) );
}

//    /** Writes a long natural number in &zeta; coding.
//     *
//     * @param x a long natural number.
//     * @param k the shrinking factor.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number or use a nonpositive shrinking factor.
//     */

// public long writeLongZeta( long x, final int k ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );
//    if ( k < 1 ) throw new IllegalArgumentException( "The shrinking factor " + k + " is not positive" );

//    final int msb = Fast.mostSignificantBit( ++x );
//    final int h = msb / k;
//    final int l = writeUnary( h );
//    final long left = 1 << h * k;
//    return l + ( x - left < left 
//                 ? writeLong( x - left, h * k + k - 1 ) 
//                 : writeLong( x, h * k + k ) );
// }
	

/** Writes a natural number in variable-length nibble coding.
 *
 * <P>Variable-length nibble coding records a natural number by padding its binary
 * representation to the left using zeroes, until its length is a multiple of three.
 * Then, the resulting string is
 * broken in blocks of 3 bits, and each block is prefixed with a bit, which is
 * zero for all blocks except for the last one. 
 * @param x a natural number.
 * @return the number of bits written.
 * @throws IllegalArgumentException if you try to write a negative number.
 */

int obitstream::write_nibble( const int x ) {
#ifndef CONFIG_FAST
   logger( "obs" ) << LEVEL_DEBUG << "write_nibble( " << x << " )\n";
#endif 

   assert( x > 0 );

   if ( x == 0 ) 
      return write_int( 8, 4 );

   const int msb = most_significant_bit( x );
   int h = msb / 3;
   do {
      write_bit( h == 0 );
      write_int( x >> h * 3 , 3 );
   } while( h-- != 0 );
   return ( ( msb / 3 ) + 1 ) << 2;
}
	
//    /** Writes a long natural number in variable-length nibble coding.
//     *
//     * @param x a long natural number.
//     * @return the number of bits written.
//     * @throws IllegalArgumentException if you try to write a negative number.
//     */

// public int writeLongNibble( final long x ) throws IOException {
//    if ( x < 0 ) throw new IllegalArgumentException( "The argument " + x + " is negative" );

//    if ( x == 0 ) return writeInt( 8, 4 );
//    final int msb = Fast.mostSignificantBit( x );
//    int h = msb / 3;
//    do {
//       writeBit( h == 0 );
//       writeInt( (int)( x >> h * 3 ) , 3 );
//    } while( h-- != 0 );
//    return ( ( msb / 3 ) + 1 ) << 2;
// }
	
}



