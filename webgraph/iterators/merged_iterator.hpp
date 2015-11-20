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

#ifndef MERGED_ITERATOR_HPP_
#define MERGED_ITERATOR_HPP_

#include "utility_iterator_base.hpp"
#include "../../log/logger.hpp"
#include <cassert>
#include <string>
#include <sstream>
#include <limits>
#include <exception>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

//#define HARDCORE_DEBUG_MERGED_ITOR

namespace webgraph { namespace bv_graph { namespace utility_iterators {

/** A merged integer iterator based on two other iterators returning comparable values in
 *  an increasing fashion simply returns the (ordered) merge of the two iterators, without
 *  duplicates.
 * 
 * itor : should be a forward traversal output iterator
 * val_type : needs to have < defined on it
 */

template<class val_type>
class merged_iterator : public utility_iterator_base<val_type> {
public:
   typedef boost::shared_ptr< utility_iterator_base<val_type> > underlying_ptr;

private:
   underlying_ptr it0, it1;
   bool valid0, valid1;
   val_type curr0, curr1;
   int n;

   void init() {
      if ( valid0 = it0->has_next() ) 
         curr0 = it0->next();
      if ( valid1 = it1->has_next() ) 
         curr1 = it1->next();
   }

   void copy( const merged_iterator<val_type>& other );

public:
   merged_iterator( underlying_ptr i0, underlying_ptr i1 ) :
      it0(i0), it1(i1), n(std::numeric_limits<int>::max()) 
      {
         init();
      }

   merged_iterator( const merged_iterator<val_type>& other ) {
      copy( other );
   }
                       
   /** Creates a new merged iterator by merging two given iterators;
    * the resulting iterator will not emit more than <code>n</code>
    * integers.
    * 
    * @param it0 the first increasing iterator.
    * @param it1 the second increasing iterator.
    * @param n the maximum number of integers this iterator will enumerate.
    */
   merged_iterator( underlying_ptr i0, underlying_ptr i1, int sz ) :
      it0( i0 ), it1( i1 ), n(sz) {
      init();
   }

   merged_iterator<val_type>& operator = ( const merged_iterator& other ) {
      copy( other );
      return *this;
   }

   bool has_next() const {
      return n != 0 && ( valid0 || valid1 );
   }

#ifdef HARDCORE_DEBUG_MERGED_ITOR
   val_type next() {
      val_type v = next_internal();

      logs::logger( "iterators" ) << LEVEL_DEBUG 
                                  << "merged iterator next about to return " << v 
                                  << "\n";

      return v;
   }
   
   val_type next_internal();
#else
   val_type next();
#endif

   std::string as_str() const {
      std::ostringstream oss;
      
      oss << "merged iterator merging:\n"
          << it0->as_str() << "\nand\n"
          << it1->as_str() << "\n";
      
      return oss.str();
   }

   int skip( int how_many ) {
      int num_skipped = 0;

      while( num_skipped < how_many && has_next() ) {
         next();
         ++num_skipped;
      }

      return num_skipped;
   }

   merged_iterator* clone() const { 
      return new merged_iterator( *this );
   }
};

////////////////////////////////////////////////////////////////////////////////

template<class val_type>
void merged_iterator<val_type>::copy( const merged_iterator<val_type>& other ) {
   if( other.it0 != NULL ) {
      it0.reset( other.it0->clone() );
   } else {
      it0.reset();
   }

   if( other.it1 != NULL ) {
      it1.reset( other.it1->clone() );
   } else {
      it1.reset();
   }
   
   valid0 = other.valid0;
   valid1 = other.valid1;
   curr0 = other.curr0;
   curr1 = other.curr1;
   n = other.n;
}

template< class val_type >
#ifdef HARDCORE_DEBUG_MERGED_ITOR
val_type merged_iterator<val_type>::next_internal() {
#else
val_type merged_iterator<val_type>::next() {
#endif
   if ( !has_next() ) 
      throw std::logic_error( "Trying to dereference empty merged_iterator." );
   n--;
   
   int current;
   
   if ( !valid0 ) {
      current = curr1;
      if ( valid1 = it1->has_next() ) 
         curr1 = it1->next();
      return current;
   } 
   if ( !valid1 ) {
      current = curr0;
      if ( valid0 = it0->has_next() ) 
         curr0 = it0->next();
      return current;
   } 
   if ( curr0 < curr1 ) {
      current = curr0;
      if ( valid0 = it0->has_next() ) 
         curr0 = it0->next();
   } 
   else if ( curr0 > curr1 ) {
      current = curr1;
      if ( valid1 = it1->has_next() ) 
         curr1 = it1->next();
   } 
   else {
      current = curr0;
      if ( valid0 = it0->has_next() ) 
         curr0 = it0->next();
      if ( valid1 = it1->has_next() ) 
         curr1 = it1->next();
   }
   return current;
}
   
} } }


#endif /*MERGED_ITERATOR_HPP_*/
