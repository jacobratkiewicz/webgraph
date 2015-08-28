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

#ifndef MASKED_ITERATOR_HPP_
#define MASKED_ITERATOR_HPP_

#include "utility_iterator_base.hpp"
#include "../../log/logger.hpp"
#include <vector>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <exception>
#include <boost/shared_ptr.hpp>

//#define HARDCORE_DEBUG_MI

namespace webgraph { namespace bv_graph { namespace utility_iterators {

/** A {@link MaskedIntIterator} is based on an array, the <em>mask</em>, and on an
 *  <em>underlying iterator</em>. The sum of the values contained in the mask must not
 *  exceed the number of elements returned by the underlying iterator. Moreover, all
 *  integers in the mask must be positive, except possibly for the first one, which may be
 *  zero.
 *
 *  <P>Mask values are interpreted as specifying inclusion-exclusion blocks.  Suppose that
 *  the underlying iterator returns <var>N</var> values, and that the mask is
 *  <var>n</var><sub>0</sub>, <var>n</var><sub>1</sub>, &hellip;,
 *  <var>n</var><sub>k</sub>.  Then, the first <var>n</var><sub>0</sub> values returned by
 *  the underlying iterator must be kept, the next <var>n</var><sub>1</sub> values must be
 *  ignored, the next <var>n</var><sub>2</sub> must be kept and so on. The last
 *  <var>N</var>&minus;(<var>n</var><sub>0</sub>+&hellip;+<var>n</var><sub>k</sub>) must
 *  be kept if <var>k</var> is odd, and must be ignored otherwise.  An instance of this
 *  class will returns the kept values only, in increasing order.
 * 
 * itor_type is the iterator type
 * value_type is returned by the iterator.
 */
template<class val_type>
class masked_iterator : public utility_iterator_base<val_type> {
private:
   typedef boost::shared_ptr< utility_iterator_base<val_type> > underlying_ptr;
   /** The underlying iterator. */
   underlying_ptr underlying;
   
   /** The mask. */
   std::vector<int> mask;

   /** explicit mask length */
   unsigned mask_len;
   
   /** This index in mask always represents an exclusion block. */
   unsigned curr_mask;
   /** How many integers are left in the current inclusion block. If
    *      <code>0</code> everything left must be discarded; if
     *      <code>-1</code> all remaining values must be kept. */
   int left;
   /** Whether {@link #underlying} is exhausted. This is accessed so
    * frequently that it is better to cache it. */
   bool not_over;
   
   void init() {
      curr_mask = 0;

      if ( curr_mask < mask_len ) {
         left = mask[ curr_mask++ ];
         advance();
      }
      else 
         left = -1;
      not_over = underlying->has_next();	
   }

   void copy( const masked_iterator& other ) {
      if( other.underlying != NULL ) 
         underlying.reset( other.underlying->clone() );
      else
         underlying.reset();

      mask = other.mask;
      mask_len = other.mask_len;
      curr_mask = other.curr_mask;
      left = other.left;
      not_over = other.not_over;
   }

 public:
   masked_iterator( const masked_iterator& other ) {
      copy( other );
   }

   masked_iterator( const std::vector<int>& m, underlying_ptr ul ) :
      underlying(ul), mask( m ), mask_len( m.size() )
      {
         init();
      }


   /** Creates a new masked iterator using a given mask, mask length
    * and underlying iterator.
    * 
    * @param mask a mask, or <code>null</code>, meaning an empty mask
    * (everything is copied).  
    * @param maskLen an explicit mask length.
    * @param underlying an underlying iterator.
    */
   masked_iterator( const std::vector<int>& m, 
                    const int ml, 
                    underlying_ptr ul ) :
      underlying( ul ), mask( m ), mask_len( ml )
      {
         init();
      }
   

#ifdef HARDCORE_DEBUG_MI
   val_type next() {
      val_type val = next_internal();
      logs::logger( "iterators" ) << LEVEL_DEBUG
                                  << "masked iterator next about to return " << val
                                  << "\n";
      return val;
   }

   val_type next_internal();
#else
   val_type next();
#endif

   bool has_next() const;
   int skip( int n );

   masked_iterator& operator = ( const masked_iterator& other ) {
      copy( other );
   }

   masked_iterator* clone() const {
      return new masked_iterator( *this );
   }
	   
   std::string as_str() const {
      std::ostringstream oss;

      oss << "masked_iterator masking:\n"
          << "\t" << underlying->as_str();

      return oss.str();
   }   

private:
   void advance();
};

template<class val_type>
#ifdef HARDCORE_DEBUG_MI
val_type masked_iterator<val_type>::next_internal() {
#else
val_type masked_iterator<val_type>::next() {
#endif
   if ( ! has_next() ) 
      throw std::logic_error( "Trying to dereference empty masked_iterator." );

   const int nxt = underlying->next();
   if ( left > 0 ) {
      left--;
      advance();
   }
   not_over = underlying->has_next();
   return nxt;
}

template<class val_type>
bool masked_iterator<val_type>::has_next() const {
   if ( left == 0 ) 
      return false;
   if ( left < 0 ) {
      if( not_over ) assert( underlying->has_next() );
      return not_over;
   }

   assert( underlying->has_next() );
   return true;
}

template<class val_type>
void masked_iterator<val_type>::advance() {
   if ( left == 0 && curr_mask < mask_len ) {
      underlying->skip( mask[ curr_mask++ ] );
      if ( curr_mask < mask_len ) 
         left = mask[ curr_mask++ ];
      else left = -1;	
   }
}

template<class val_type>
int masked_iterator<val_type>::skip( int n ) {
   int skipped = 0;
   
   while( skipped < n && ( left > 0 || left == -1 && not_over ) ) {
      if ( left == -1 ) 
         skipped += underlying->skip( n - skipped );
      else {
         if ( n - skipped < left ) {
            underlying->skip( n - skipped );
            not_over = underlying->has_next();
            left -= ( n - skipped );
            return n;
         }
         else {
            underlying->skip( left );
            skipped += left;
            left = 0;
            advance();
            not_over = underlying->has_next();
         }
      }
   }
   
   return skipped;
}

} } }
#endif /*MASKED_ITERATOR_HPP_*/
