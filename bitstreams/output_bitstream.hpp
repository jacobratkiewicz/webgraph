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

#ifndef OUTPUT_BITSTREAM_HPP
#define OUTPUT_BITSTREAM_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>

#include <boost/shared_ptr.hpp>

#include "../log/logger.hpp"

namespace webgraph {

/*! Bit level output stream, meant to duplicate some of the functionality of Vigna's MG4J
 * class.  This class does not provide buffering (yet).  No guarantee about endianness is
 * given; chunks of data will be written out in exactly the order they are handed to this
 * class.
 * 
 * We do guarantee that if you write a sequence of bits to this class, then read them back
 * from ibitstream, you'll get the same sequence. That should be enough.
 * 
 * @author jacob ratkiewicz
 */

class obitstream {
public:
   /* The default size of the byte buffer in bytes (16Ki). */
   const static int DEFAULT_BUFFER_SIZE = 16 * 1024;

   typedef unsigned char byte;
protected:
   /** The underlying ostream. */
   boost::shared_ptr<std::ostream> os;
   /** The number of bits written to this bit stream. */
   long written_bits;
   /** Current bit buffer. */
   int current;
//   std::vector<bool> current_byte;
   /** The stream buffer. */
   boost::shared_ptr<std::vector<byte> > buffer;
   /** Current number of free bits in the bit buffer (the bits in the buffer are stored high). */
   int free;
   /** Current position in the byte buffer. */
   int pos;
   /** Current position of the underlying output stream. */
   long position;
   /** Current number of bytes available in the byte buffer. */
   int avail;
   /** Size of the small buffer for temporary usage. */
   const static int TEMP_BUFFER_SIZE = 128;
   /** True if we are wrapping an array. */
   bool wrapping;

   /** true iff we have no buffer */
   bool no_buffer;
private: 
   /** Small buffer for temporary usage. */
   std::vector<byte> temp_buffer;

   /**
    * Initialization function that sets everything to sensible default values
    */
   void init() {
      written_bits = 0;
      current = 0;
      free = 8;
      pos = 0;
      position = 0;
      avail = 0;
      wrapping = false;
      no_buffer = true;
      temp_buffer.resize( TEMP_BUFFER_SIZE );

#ifndef CONFIG_FAST
      logs::register_logger( "obs", logs::LEVEL_NONE );
#endif
   }
   

   void init( const boost::shared_ptr<std::ostream>& os, const int buf_size ) {
      init();
      
      this->os = os;
      
      if ( buf_size != 0 ) {
         buffer->resize( buf_size );
         avail = buf_size;
         no_buffer = false;
      } else {
        	avail = 0;
        	no_buffer = true;
      }
   }

   /** This (non-public) constructor exists just to provide fake
    * initialization
    */
protected:
   obitstream() {
      init();
   }
   
public:

   /** Creates a new output bit stream wrapping a given output stream with a specified buffer size.
    *
    * @param os the output stream to wrap.
    */
   obitstream( const boost::shared_ptr<std::ostream>& os, const int buf_size = DEFAULT_BUFFER_SIZE ) :
      buffer( (buf_size == 0) ? NULL : new std::vector<byte>(buf_size) ) {
      init(os, buf_size );
   }

   /* Creates a new output bit stream wrapping a given byte array.
    *
    * TODO find alternative to copying whole array.
    * 
    * @param a the byte array to wrap.
    */
   obitstream( boost::shared_ptr<std::vector<byte> >& a ) : buffer(a) {
      init();

      free = 8;
      avail = a->capacity();
      wrapping = true;
   }

   /** Creates a new output bit stream writing to file.
    *
    * @param name the name of the file.
    * @param bufSize the size in byte of the buffer; it may be 0, denoting no buffering.
    */
   obitstream( const std::string name, const int buf_size = DEFAULT_BUFFER_SIZE ) :
      buffer( (buf_size == 0) ? NULL : new std::vector<byte>( buf_size ) ) {
      boost::shared_ptr<std::ostream> o( new std::ofstream( name.c_str() ) );
      init(o, buf_size);
   }
   
   /**
    * dtor
    */
    ~obitstream() {
	    // make sure everything makes it to the file.
    	flush();	
    }

   /** Flushes the bit stream.
    *
    * <P>This method will align the stream, write the bit buffer, empty the
    * byte buffer and delegate to the {@link OutputStream#flush()} method of
    * the underlying output stream.
    *
    * <P>This method is provided so that users of this class can easily wrap
    * repositionable streams (for instance, file-based streams, which can be
    * repositioned using the underlying {@link
    * java.nio.channels.FileChannel}). <P> It is guaranteed that after calling
    * this method the underlying stream can be repositioned, and that the next
    * write to the underlying output stream will start with the content of the
    * first write method called afterwards.
    */

   void flush() {
#ifndef CONFIG_FAST
      logs::logger( "obs" ) << "Flush called.\n";
#endif
      
      align();
      if ( os != NULL ) {
         if ( !no_buffer ) {
            os->write( (char*)&(*buffer)[0], pos );
            position += pos;
            pos = 0;
            avail = buffer->size();
         }
         os->flush();
      }
   }

   /** Returns the number of bits written to this bit stream.
    *
    * @return the number of bits written so far.
    */
   long get_written_bits() {
      return written_bits;
   }

   /** Sets the number of bits written to this bit stream.
    *
    * <P>This method is provided so that, for instance, the 
    * user can reset via <code>writtenBits(0)</code> the written-bits count
    * after a {@link #flush()}.
    *
    * @param writtenBits the new value for the number of bits written so far.
    */
   void set_written_bits( const long written_bits ) {
      this->written_bits = written_bits;
   }

private:
   void write( const int b );
   int write_in_current( const int b, const int len );

public:
   /** Aligns the stream.
    *
    * After a call to this method, the stream is byte aligned. Zeroes
    * are used to pad it if necessary.
    *
    * @return the number of padding bits.
    */

   int align() {
#ifndef CONFIG_FAST
      logs::logger("obs") << "Align called\n";
#endif
      if ( free != 8 ) 
         return write_in_current( 0, free );
      else 
         return 0;
   }

   void set_position(const long position);
   
   /** Writes a sequence of bits. 
    *
    * Bits will be written in the natural way: the first bit is bit 7 of the
    * first byte, the eightth bit is bit 0 of the first byte, the ninth bit is
    * bit 7 of the second byte and so on.
    *
    * @param bits a vector containing the bits to be written.
    * @param len a bit length.
    * @return the number of bits written (<code>len</code>).
    */

   int write( const byte bits[], const int len ) {
      return write_byte_offset( bits, 0, len );
   }

   int write( const byte bits[], const int offset, const int len );

protected:
   int write_byte_offset( const byte bits[], 
                             const int offset, int len );

   /** Writes a bit.
    *
    * @param bit a bit.
    * @return the number of bits written.
    */
   int write_bit( const bool bit ) {
      return write_in_current( bit ? 1 : 0, 1 ); 
   }

   /** Writes a bit.
    *
    * @param bit a bit.
    * @return the number of bits written.
    */

   int write_bit( const int  bit ) {
      assert( bit == 0 || bit == 1 );
      return write_in_current( bit, 1 ); 
   }
public:
   int write_int( int x, const int len );
protected:
//    /** Writes a fixed number of bits from a long.
//     *
//     * @param x a long.
//     * @param len a bit length; this many lower bits of the first argument will be written
//     * (the most significant bit first).
//     * @return the number of bits written (<code>len</code>).
//     */

// public int writeLong( long x, final int len ) throws IOException {

//    if ( len < 0 || len > 64 ) throw new IllegalArgumentException( "You cannot write " + len + " bits to a long." );

//    if ( len <= free ) return writeInCurrent( (int)x, len );

//    final int q = ( len - free ) & 7, blocks = ( len - free ) >> 3;
//    int i = blocks;

//    if ( q != 0 ) {
//       tempBuffer[ blocks ] = (byte)x;
//       x >>= q;
//    }

//    while( i-- != 0 ) {
//       tempBuffer[ i ] = (byte)x;
//       x >>>= 8;
//    }

//    writeInCurrent( (int)x, free );
//    i = blocks;
//    for( i = 0; i < blocks; i++ ) write( tempBuffer[ i ] );
//    writtenBits += blocks << 3;

//    if ( q != 0 ) writeInCurrent( tempBuffer[ blocks ], q );
//    return len;
// }

private:
   static int most_significant_bit( const int x );


public:
   int write_unary( int x );
   int write_gamma( int x );
   int write_delta( int x );
   int write_zeta( int x, const int k );
   int write_nibble( const int x );
};

}

#endif
