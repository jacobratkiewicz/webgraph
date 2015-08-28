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

#ifndef INPUT_BITSTREAM_HPP
#define INPUT_BITSTREAM_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include "../utils/fast.hpp"
#include <exception>

#include <boost/shared_ptr.hpp>

//#define LOGGING

namespace webgraph {

// This little guy gets used internally by read() and refill()
struct eof_exception : public std::exception {
   eof_exception() {}
};


/** Bit-level input stream.
 *
 * <P>This class wraps any InputStream so that you can treat it as
 * <em>bit</em> stream.  Constructors and methods closely resemble those of
 * InputStream. Data can be read from such a stream in several ways:
 * reading an integer or long in fixed-width, unary, &gamma;, &delta;, &zeta; and Golomb
 * coding, or reading a number of bits that will be stored in a vector of
 * bytes. There is limited support for #mark(int)/#reset()
 * operations.
 *
 * <P>This class can also {@linkplain #InputBitStream(byte[]) wrap a byte
 * array}; this is much more lightweight than wrapping a {@link
 * FastByteArrayInputStream} wrapping the array. Overflowing the array
 * will cause an java.io.EOFException.
 * 
 * <P>Note that when reading using a vector of bytes bits are read in the
 * stream format (see OutputBitStream): the first bit is bit 7 of the
 * first byte, the eighth bit is bit 0 of the first byte, the ninth bit is bit
 * 7 of the second byte and so on. When reading integers using some coding,
 * instead, they are stored in the standard way, that is, in the <strong>lower</strong>
 * bits.
 *
 * <P>Additional features:
 * 
 * <UL>
 *
 * <LI>This class provides an internal buffer. By setting a buffer of
 * length 0 at creation time, you can actually bypass the buffering system:
 * Note, however, that several classes providing buffering have synchronised
 * methods, so using a wrapper instead of the internal buffer is likely to lead
 * to a performance drop.
 *
 * <LI>To work around the schizophrenic relationship between streams and random
 * access files in java.io, this class provides a #flush()
 * method that resets the internal state. At this point, you can safely reposition
 * the underlying stream and read again afterwards. For instance, this is safe
 * and will perform as expected:
 * <PRE>
 * FileInputStream fis = new FileInputStream( ... );
 * InputBitStream ibs = new InputBitStream( fis );
 * ... read operations on ibs ...
 * ibs.flush();
 * fis.getChannel().position( ... );
 * ... other read operations on ibs ...
 * </PRE>
 *
 * <P>As a commodity, an instance of this class will try to cast the underlying byte
 * stream to a RepositionableStream and to fetch by reflection the {@link
 * java.nio.channels.FileChannel} underlying the given input stream, in this
 * order.  If either reference can be successfully fetched, you can use
 * directly the #position(long) position() method with argument
 * <code>pos</code> with the same semantics of a #flush(), followed by
 * a call to <code>position(pos / 8)</code> (where the latter method belongs
 * either to the underlying stream or to its underlying file channel), followed
 * by a #skip(long) skip(pos % 8).
 *
 * <LI>You can unget bits, integers and longs. A buffer {@link
 * #UNGET_BUFFER_SIZE} bytes long is allocated to this purpose. The buffer is
 * cleared on #reset()/#flush().
 *
 * <li>This class supports also overflowing: if the overflow flag is set when
 * the EOF of the underlying InputStream is met, then zeroes are
 * subsequently read, and no exception is thrown. 
 *
 * <li>Finally, this class implements partially the interface of a boolean iterator.
 * More precisely, #nextBoolean() will return the same bit as #readBit(),
 * and also the same exceptions, whereas <em>#hasNext() will always return true</em>:
 * you must be prepared to catch a java.lang.RuntimeException wrapping an 
 IOException
 * in case the file ends (unless, of course, you used the overflowing features). It
 * is very difficult to implement completely an eager operator using a input-stream
 * based model.
 *
 * </ul>
 *
 * <P><STRONG>This class is not synchronised</STRONG>. If multiple threads
 * access an instance of this class concurrently, they must be synchronised externally.
 *
 * @see java.io.InputStream
 * @see it.unimi.dsi.mg4j.io.OutputBitStream
 * @author Sebastiano Vigna
 * @since 0.1
 */

class ibitstream {

public:
   /** The size of the #unget(boolean) buffer in bytes. */
   const static int UNGET_BUFFER_SIZE = 16;
   /** The default size of the byte buffer in bytes (16Ki). */
   const static int DEFAULT_BUFFER_SIZE = 16 * 1024;

private:
   /** The number of bits actually read from this bit stream. */
   long read_bits;
   /** Current bit buffer: the lowest #fill bits represent the current content
    * (the remaining bits are undefined). */
   int current;
   /** The stream buffer. */
	
protected:
   typedef unsigned char byte;

   boost::shared_ptr< std::vector<byte> > buffer;
   /** Whether we should use the byte buffer. */
   bool no_buffer;
   /** Current number of bits in the bit buffer (stored low). */
   unsigned int fill;
   /** Current position in the byte buffer. */
   unsigned int pos;
   /** Current number of bytes available in the byte buffer. */
   unsigned int avail;
   /** Current position of the first byte in the byte buffer. */
   unsigned long position;
   /** Byte buffer for ungetting bits. It is allocated on demand. */
   std::vector<byte> unget_bytes;
   /** How many bytes does the unget buffer hold (highest index to go out first) ?. */
   unsigned int unget_count;
   /** Whether we allow overflowing the stream (getting zeroes). */
   bool overflow;
   /** In case we allow overflow, whether we are past EOF. */
   bool past_eof;
   /** True if we are wrapping an array. */
   bool wrapping;

private:
   /** The stream backing this bit stream */
   boost::shared_ptr<std::istream> is; 

   /** Precomputed byte parsing for &gamma; coding. bits 0-7 contain the value, bits 8-15
    * the length of the code. 0 means that parsing goes beyond 8 bits. */
   const static int GAMMA[];

   /** Precomputed byte parsing for &delta; coding. bits 0-7 contain the value, bits 8-15
    * the length of the code. 0 means that parsing goes beyond 8 bits. */
   const static int DELTA[]; 
   
   /** Precomputed byte parsing for &zeta;<sub>3</sub> coding. bits 0-7 contain the value,
    * bits 8-15 the length of the code. 0 means that parsing goes beyond 8 bits. */
   const static int ZETA_3[]; 
   
   /** precomputed array of most significant bits given bytes */
   static const int BYTEMSB[];

   void init() {
      read_bits = 0;
      current = 0;
      no_buffer = true;
      fill = 0;
      pos = 0;
      avail = 0;
      position = 0;
      unget_count = 0;
      overflow = false;
      past_eof = false;
      wrapping = false;
   }
   
   void init( const boost::shared_ptr<std::istream>& is, 
              int buf_size ) {
      init();
      
      this->is = is;
      assert( this->is->good() );
      
      no_buffer = (buf_size == 0);
  
      assert( buffer->size() == (unsigned)buf_size );
//      if ( !no_buffer ) 
//         buffer->resize(buf_size);  			  	
   }
   
   /** This constructor exists just to provide fake initialisation.
    * 
    * TODO find a more elegant way to do this.
    */
public:
   ibitstream() {
      init();
   }
   
   /** Creates a new input bit stream wrapping a given input stream with a specified buffer size.
    *
    * @param is the input stream to wrap.
    * @param bufSize the size in byte of the buffer; it may be 0, denoting no buffering.
    */
   ibitstream( const boost::shared_ptr<std::istream>& is, 
               int buf_size = DEFAULT_BUFFER_SIZE ) :
      buffer( (buf_size == 0) ? NULL : new std::vector<byte>(buf_size) )
   {
	  init(is, buf_size );
   }

   /** Creates a new input bit stream wrapping a given byte array.
    *
    * NOTE: almost unavoidably this copies A. Any way to prevent? Pass in a pointer?
    * 
    * @param a the byte array to wrap.
    */
   ibitstream( const boost::shared_ptr< std::vector<byte> >& a ) : buffer(a) {
      init();
      
      // is = NullInputStrema.getInstance();
      
      avail = a->size();
      no_buffer = false;
      wrapping = true;
#ifdef LOGGING
      std::cerr << "##################################################\n"
                << "new ibitstream created from buffer.\n"
                << "first 50 bytes of buffer:\n";
      for( int i = 0; i < 50; i++ ) {
         std::cerr << utils::int_to_binary( (*a)[i], 8 ) << " ";
         if( i + 1 % 10 == 0 )
            std::cerr << "\n";
      }
      std::cerr << "\n";
#endif
   }

   /** Creates a new input bit stream reading from a file.
    *
    * @param name the name of the file.
    * @param bufSize the size in byte of the buffer; it may be 0, denoting no buffering.
    */
   ibitstream( std::string file_name, int buf_size = DEFAULT_BUFFER_SIZE ) :
      buffer( (buf_size == 0) ? NULL : new std::vector<byte>(buf_size) )
   {
      boost::shared_ptr<std::istream> i( new std::ifstream( file_name.c_str() ) );
      
      init( i, buf_size );
   }

   /**
    * has to have  virtual destructor... (NOT)
    */
   /* virtual */~ibitstream() {}

   ////////////////////////////////////////////////////////////////////////////////
   /**
    * Attaches this input bitstream to the given buffer.
    */
   /* virtual */void attach( boost::shared_ptr<std::vector<unsigned char> > buf ) {
      buffer = buf;
      init();

      avail = buf->size();
      no_buffer = false;
      wrapping = true;

#ifdef LOGGING
      std::cerr << "##################################################\n"
           << "new ibitstream created by ATTACHING to buffer.\n"
           << "first 50 bytes of buffer:\n";
      for( int i = 0; i < 50; i++ ) {
         std::cerr << utils::int_to_binary( (*buffer)[i], 8 ) << " ";
         if( i + 1 % 10 == 0 )
            std::cerr << "\n";
      }
      std::cerr << "\n";
#endif
   }

   /** Flushes the bit stream. All state information associated to the stream is reset. This
    * includes bytes prefetched from the stream, bits in the bit buffer and unget'd bits.
    *
    * <P>This method is provided so that users of this class can easily wrap repositionable
    * streams (for instance, file-based streams, which can be repositioned using
    * the underlying java.nio.channels.FileChannel). It is guaranteed that after calling
    * this method the underlying stream can be repositioned, and that the next read
    * will draw data from the stream.
    */
   
   /* virtual */void flush() {
      if ( ! wrapping ) {
         position += pos;
         avail = 0;
         pos = 0;
      }
      fill = 0;
      unget_count = 0;
   }
   
   /** Sets the overflow flag.
    *
    * <P>If this flag is true, then after the end of the underlying {@link
    * InputStream} we will continue returning zeroes (in particular, no
    * exception will be thrown).
    *
    * @param overflow the new value of the flag.
    * @see #pastEOF()
    */

   void set_overflow( bool overflow ) {
      this->overflow = overflow;
   }

   /** Returns the number of bits that can be read (or skipped over) from this
    * bit stream without blocking by the next caller of a method.
    *
    * @return the number of bits that can be read from this bit stream without blocking.
    */
   
//    long available() throws IOException {
//       return ( is.available() + ungetCount + avail ) * 8 + fill;
//    }

   /** Gets the overflow flag.
    *
    * @return the current value of the flag.
    * @see #overflow(boolean)
    */
   bool get_overflow() {
      return overflow;
   }

   /** Checks whether we are past EOF.
    *
    * @return <code>true</code> if we are reading zeroes past the EOF.
    * @see #overflow(boolean)
    */
   bool get_past_eof() {
      return past_eof;
   }

   /** Returns the number of bits read from this bit stream.
    *
    * <P>Note that #unget(boolean) ungetting bits from this stream
    * will result in the number of read bits to be <em>decreased</em>.
    *
    * @return the number of bits read so far.
    */
   /* virtual */long get_read_bits() {
      return read_bits;
   }
   
   /** Sets the number of bits read from this bit stream.
    *
    * <P>This method is provided so that, for instance, the 
    * user can reset via <code>readBits(0)</code> the read-bits count
    * after a #flush().
    *
    * @param readBits the new value for the number of bits read so far.
    */
   /* virtual */void set_read_bits( long read_bits ) {
      this->read_bits = read_bits;
   }
   
   ////////////////////////////////////////////////////////////////////////////////
private:

   /** Reads the next byte from the stream.
    *
    * <P>This method takes care of managing the buffering logic
    * and the unget buffer transparently.
    *
    * <P>However, this method does <em>not</em> update #readBits.
    * The caller should increment #readBits by 8 at each call, unless
    * the bit are used to load #current.
    */

   int read();

   /** Refills #current. 
    * 
    * <P>This method must be <em>only</em> when #fill &lt; 8. It first tries to
    * fill the lower byte in case #fill = 0. Then, it tries to refill another
    * byte, but in this case EOF is not considered an error, and it is not thrown.
    */
   
   void refill() {
      assert( fill < 8 );
		
      if ( fill == 0 ) {
         current = read();
         fill = 8;
#ifdef LOGGING
         std::cerr << "\tRefilled from empty; current = " << utils::int_to_binary( current, 8 )
                   << "\n";
#endif
      }
      
      // WATCH watch this part
      if( avail > 0 ) {
         int r = read();
#ifdef LOGGING
         std::cerr << "\tIn the process of refilling; just fetched " 
                   << utils::int_to_binary(r & 0xFF, 8)
                   << "\n";
#endif
         current = ( current << 8 ) | r;
         fill += 8;
#ifdef LOGGING
         std::cerr << "Refilled from partially full; current = " 
                   << utils::int_to_binary( current, fill )
                   << "\n";
#endif
      } else {
         // we know we have to read a file, so catch that pesky eof exception
         try {
            int r = read();
#ifdef LOGGING
            std::cerr << "\tIn the process of refilling; just fetched " 
                      << utils::int_to_binary(r & 0xFF, 8)
                      << "\n";
#endif
            current = ( current << 8 ) | r;
            fill += 8;
#ifdef LOGGING
            std::cerr << "Refilled from partially full; current = " 
                      << utils::int_to_binary( current, fill )
                      << "\n";
#endif      
         } catch( eof_exception e ) {
            // don't care.
         }
      }
   }
   /** Reads bits from the bit buffer, possibly refilling it.
    *
    * <P>This method is the basic mean for extracting bits from the underlying stream.
    * 
    * <P>You cannot read more than #fill bits with this method (unless {@link
    * #fill} is 0, and <code>len</code> is nonzero, in which case the buffer will be
    * refilled for you with 8 bits), and if you read exactly #fill bits the buffer
    * will be empty afterwards. In particular, there will never be 8 bits in the buffer.
    *
    * <P>The bit buffer stores its content in the lower #fill bits. The content
    * of the remaining bits is undefined.
    *
    * <P>This method updates #readBits.
    *
    * @param len the number of bits to read.
    * @return the bits read (in the <strong>lower</strong> positions).
    * @throws AssertionError if one tries to read more bits than available in the buffer and
    * assertions are enabled.
    */
   
   int read_from_current( unsigned int len );
   
   /** Aligns the stream.
    *
    * After a call to this function, the stream is byte aligned. Bits that have been
    * read to align are discarded.
    */
   
   /* virtual */void align() {
#ifdef LOGGING
      std::cerr << "Align called.\n";
#endif
      if ( ( fill & 7 ) == 0 ) return;
      read_bits += fill & 7;
      fill &= ~7;
   }

   /** Ungets a bit.
    *
    * The provided bit will be the next one read from the stream.
    *
    * @param bit a bit.
    */
   
// public void ungetBit( final boolean bit ) throws IOException {
//    unget( bit ? 1 : 0 );
// }

// 	/** Ungets a bit.
// 	 *
// 	 * @param bit a bit.
// 	 * @deprecated As of <code>MG4J</code> 0.6, replaced by #ungetBit(boolean).
// 	 */

// 	public void unget( final boolean bit ) throws IOException {
// 		ungetBit( bit );
// 	}

// 	/** Ungets a bit.
// 	 * @param bit a bit.
// 	 * @see #unget(boolean)
// 	 */

// 	public void ungetBit( final int bit ) throws IOException {
// 		if ( bit < 0 || bit > 1 ) throw new IllegalArgumentException( "The argument " + bit + " is not a bit." );

// 		if ( ASSERTS ) assert fill < 16;

// 		if ( bit != 0 ) current |= 1 << fill;
// 		else current &= ( 1 << fill ) - 1;
// 		fill++;
// 		readBits--;

// 		if ( fill == 16 ) {
// 			if ( ungetCount == UNGET_BUFFER_SIZE ) throw new IOException( "Unget buffer overflow." );
// 			if ( ungetBytes == null ) ungetBytes = new byte[ UNGET_BUFFER_SIZE ];
// 			ungetBytes[ ungetCount++ ] = (byte)current;
// 			fill = 8;
// 			current >>= 8;
// 		}
// 	}

// 	/** Ungets a bit.
// 	 * @param bit a bit.
// 	 * @deprecated As of <code>MG4J</code> 0.6, replaced by #ungetBit(int).
// 	 */

// 	public void unget( final int bit ) throws IOException {
// 		ungetBit( bit );
// 	}

// 	/** Ungets a long.
// 	 *
// 	 * The provided long will be the next one read from the stream, if the
// 	 * same length is used. This means that the bit of index <code>len</code>-1
// 	 * of <code>x</code> will be the next one to be read from the stream.
// 	 *
// 	 * @param x a long.
// 	 * @param len the number of (lower) bits to unget from <code>x</code>.
// 	 */

// 	public void ungetLong( long x, final int len ) throws IOException {
// 		if ( len < 0 || len > 64 ) throw new IllegalArgumentException( "You cannot unget " + len + " bits from a long." );
// 		int i = len;
// 		while( i-- != 0 ) {
// 			unget( (int)( x & 1 ) );
// 			x >>>= 1;
// 		}
// 	}

// 	/** Ungets an integer.
// 	 *
// 	 * The provided integer will be the next one read from the stream, if the
// 	 * same length is used. This means that the bit of index <code>len</code>-1
// 	 * of <code>x</code> will be the next one to be read from the stream.
// 	 *
// 	 * @param x an integer.
// 	 * @param len the number of (lower) bits to unget from <code>x</code>.
// 	 */

// 	public void ungetInt( int x, final int len ) throws IOException {
// 		if ( len < 0 || len > 32 ) throw new IllegalArgumentException( "You cannot unget " + len + " bits from an integer." );
// 		int i = len;
// 		while( i-- != 0 ) {
// 			unget( x & 1 );
// 			x >>>= 1;
// 		}
// 	}

public:	 
   /** Reads a sequence of bits. 
    *
    * Bits will be read in the natural way: the first bit is bit 7 of the
    * first byte, the eightth bit is bit 0 of the first byte, the ninth bit is
    * bit 7 of the second byte and so on.
    *
    * @param bits an array of bytes to store the result.
    * @param len the number of bits to read.
    */

   /* virtual */void read( byte bits[], unsigned int len );

  
   /** Reads a bit.
    *
    * @return the next bit from the stream.
    */
   
   /* virtual */int read_bit() {
      return read_from_current( 1 );
   }


   /** Reads a fixed number of bits into an integer.
    *
    * @param len a bit length.  @return an integer whose lower <code>len</code> bits are
    * taken from the stream; the rest is zeroed.
    */
   
   /* virtual */int read_int( unsigned int len );

//    /** Reads a fixed number of bits into a long.
//     *
//     * @param len a bit length.
//     * @return a long whose lower <code>len</code> bits are taken from the stream; the rest is zeroed.
//     */

// 	public long readLong( int len ) throws IOException {
// 		int i;
// 		long x = 0;

// 		if ( len < 0 || len > 64 ) throw new IllegalArgumentException( "You cannot read " + len + " bits into a long." );

// 		if ( len <= fill ) return read_from_current( len );

// 		len -= fill;
// 		x = read_from_current( fill );

// 		i = len >> 3;
// 		while( i-- != 0 ) x = x << 8 | read();
// 		readBits += len & ~7;

// 		len &= 7;

// 		return ( x << len ) | read_from_current( len );
// 	}

   /** Skips the given number of bits. 
    *
    * @param n the number of bits to skip.
    * @return the actual number of skipped bits.
    */
   
   /* virtual */long skip( unsigned long n );
   
   /** Sets this stream bit position, if it is based on a RepositionableStream or
    * on a java.nio.channels.FileChannel.
    */

   /* virtual */void set_position( unsigned long position );


//    /** Tests if this stream supports the #mark(int) and #reset() methods.
//     *
//     * <P>This method will just delegate the test to the underlying InputStream.
//     * @return whether this stream supports #mark(int)/#reset().
//     */
   
//    bool markSupported() {
//       return is.markSupported();
//    }
	 
//    /** Marks the current position in this input stream. A subsequent call to
//     * the #reset() method repositions this stream at the last marked position so
//     * that subsequent reads re-read the same bits.
//     *
//     * <P>This method will just delegate the mark to the underlying InputStream.
//     * Moreover, it will throw an exception if you try to mark outsite byte boundaries.
//     *
//     * @param readLimit the maximum limit of bytes that can be read before the mark position becomes invalid.
//     * @throws IOException if you try to mark outside byte boundaries.
//     */
   
// 	public void mark( final int readLimit ) throws IOException {
// 		if ( fill != 0 ) throw new IOException( "You cannot mark a bit stream outside of byte boundaries." );
// 		is.mark( readLimit );
// 	}

// 	/** Repositions this bit stream to the position at the time the #mark(int) method was last called.
// 	 *
// 	 * <P>This method will just #flush() flush the stream and delegate
// 	 * the reset to the underlying InputStream.
// 	 */

// 	public void reset() throws IOException {
// 		flush();
// 		is.reset();
// 	}

   /** Reads a natural number in unary coding.
    *
    * Note that by unary coding we mean that 1 encodes 0, 01 encodes 1 and so on.
    *
    * @return the next unary-encoded natural number.
    */
   /* virtual */int read_unary();

   /* virtual */int read_gamma();

   /* virtual */int read_delta();

   /* virtual */int read_zeta(int k);

   /* virtual */int read_nibble();
};

} // end namespace?

#endif /*INPUT_BITSTREAM_HPP_*/
