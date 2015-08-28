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

#ifndef INTERVAL_SEQUENCE_ITERATOR_HPP_
#define INTERVAL_SEQUENCE_ITERATOR_HPP_

#include <cassert>
#include <boost/iterator/iterator_facade.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include "utility_iterator_base.hpp"
#include "../../log/logger.hpp"

//#define HARDCORE_DEBUG_INT_SEQ_ITOR

namespace webgraph { namespace bv_graph { namespace utility_iterators {

/** This class iterates over the integers contained in a sequence of intervals,
 * which are specified by their left points and the number of integers they contain.
 */
template< class integral_type >
class interval_sequence_iterator : public utility_iterator_base<integral_type> {
private:
   /** The left extremes. */
   std::vector<integral_type> left;

   /** The lengths. */
   std::vector<integral_type> len;

   /** The index of the current interval. */
   unsigned int curr_interval;

   /** The left point of the current interval */
   unsigned int curr_left;

   /** The current position in the current interval: the next integer to be output is
    * left[cur_interval] + cur_index. */
   unsigned int curr_index;
   
   /** number of intervals left.. this should always be equal to left.size() -
    * cur_interval */
   unsigned n;
        
   void init() {
      /** takes care of initializing the default stuff */
      curr_interval = 0;
      curr_left = 0;
      curr_index = 0;
   }
public:
   interval_sequence_iterator( const std::vector<integral_type>& lf, 
                               const std::vector<integral_type>& ln ) :
      left( lf ), len(ln), n(lf.size()) {
      init();
      advance();
   }
   
   /** Creates a new interval-sequence iterator by specifying arrays of left extremes and
    * lengths, and the number of valid entries. Note that the two arrays are <em>not</em>
    * copied, so they are supposed not to be changed during the iteration.
    * 
    * @param left an array containing the left extremes of the intervals generating this
    * iterator.
    *
    * @param len an array (of the same length as <code>left</code>) containing the number
    * of integers in each interval.
    *
    * @param N the number of valid entries in <code>left</code> and <code>len</code>.
    */
   
   interval_sequence_iterator( const std::vector<integral_type>& lf, 
                               const std::vector<integral_type>& ln, 
                               const int sz ) : left(lf), len(ln), n(sz) {
      init();
      advance();
   }
	
public:

// Make the next() method log debugging information if needed.
#ifdef HARDCORE_DEBUG_INT_SEQ_ITOR
   integral_type next() {
      integral_type result = next_internal();
      logs::logger( "iterators" ) << logs::LEVEL_MAX
                                  << "interval sequence iterator next about to return "
                                  << result << "\n";
      return result;
   }
   
   integral_type next_internal();
#else
   integral_type next();
#endif

   interval_sequence_iterator<integral_type>* clone() const {
      return new interval_sequence_iterator<integral_type>( *this );
   }

   bool has_next() const {
      return n != 0;
   }
   
   int skip( int how_many );

   ////////////////////////////////////////////////////////////////////////////////
   std::string as_str() const {
      using namespace std;

      ostringstream oss;

      oss << "interval_sequence_iterator:\n"
          << "\tleft.size() = " << left.size() << "\n"
          << "\tlen.size() = " << len.size() << "\n"
          << "\tcur_interval = " << curr_interval << "\n"
          << "\tcur_index = " << curr_index << "\n"
          << "\tn = " << n << "\n";
      
      return oss.str();
   }

private:
   void advance();
};

////////////////////////////////////////////////////////////////////////////////
template<typename integral_type>
#ifdef HARDCORE_DEBUG_INT_SEQ_ITOR
integral_type interval_sequence_iterator<integral_type>::next_internal() {
#else
integral_type interval_sequence_iterator<integral_type>::next() {
#endif
   if ( ! has_next() ) 
      throw std::logic_error("Attempt to fetch next element of empty integral_type iterator.");
   
   const int next = curr_left + curr_index++;
   advance();
   return next;
}

////////////////////////////////////////////////////////////////////////////////
template<typename integral_type>
int interval_sequence_iterator<integral_type>::skip( int how_many ) {
   int skipped = 0;
   
   while( skipped < how_many && how_many != 0 ) {
      if ( how_many - skipped < len[ curr_interval ] - (int)curr_index ) {
         curr_index += ( how_many - skipped );
         return how_many;
      } else {
         skipped += len[ curr_interval ] - curr_index;
         curr_index = len[ curr_interval ];
         advance();
      }
   }
   
   return skipped;
}

template<typename integral_type>
void interval_sequence_iterator<integral_type>::advance() {
   while( n != 0 ) {
      curr_left = left[ curr_interval ];
      if ( (int)curr_index < len[ curr_interval ] ) 
         break;
      n--;
      curr_interval++;
         curr_index = 0;
   }
}
   
} } }

#endif /*INTERVAL_SEQUENCE_ITERATOR_HPP_*/
