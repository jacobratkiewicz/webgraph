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

#ifndef RESIDUAL_ITERATOR_HPP_
#define RESIDUAL_ITERATOR_HPP_

#include "utility_iterator_base.hpp"
#include "../../utils/fast.hpp"
#include "../../bitstreams/input_bitstream.hpp"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <sstream>
#include "../webgraph.hpp"
#include "../../log/logger.hpp"

//#define HARDCORE_DEBUG_RESID_ITOR

namespace webgraph { namespace bv_graph { 

//class graph;   
   
namespace utility_iterators {

template<class val_type>
class residual_iterator : public utility_iterator_base<int> {
private:
   unsigned int node;
   int prev;
   unsigned int i;
   const webgraph::bv_graph::graph* owner;
   boost::shared_ptr<ibitstream> ibs;
   
public:
   residual_iterator( int n, int res_left, 
                      const webgraph::bv_graph::graph* o, boost::shared_ptr<ibitstream> i ) :
      node(n), prev(-1), i( res_left ), owner(o), ibs(i) {}

   bool has_next() const  {
      return i != 0;
   }
   
#ifdef HARDCORE_DEBUG_RESID_ITOR
   val_type next() {
      val_type n = next_internal();
      logs::logger( "iterators" ) << LEVEL_EVERYTHING << "residual_iterator next about to return "
                                  << n << "\n";

      return n;
   }

   val_type next_internal();
#else
   val_type next();
#endif

   std::string as_str() const {
      std::ostringstream oss;

      oss << "residual iterator:\n"
          << "node = " << node << ", prev = " << prev << ", i = " << i << "\n";

      return oss.str();
   }

   residual_iterator* clone() const {
      return new residual_iterator( *this );
   }

   int skip( int how_many ) {
//      throw logic_error( "this should not happen." );

      int num_skipped = 0;

       while( has_next() && num_skipped < how_many ) {
          next();
          num_skipped++;
       }

       return num_skipped;
   }
};

template<class val_type>
#ifdef HARDCORE_DEBUG_RESID_ITOR
val_type residual_iterator<val_type>::next_internal() {
#else
val_type residual_iterator<val_type>::next() {
#endif
   if ( ! has_next() )
      throw logic_error( "Trying to dereference empty residual_iterator." );
   i--;
   if ( prev == -1 ) 
      return prev = node + utils::nat2int( owner->read_residual( *ibs ) );
   else 
      return prev = owner->read_residual( *ibs ) + prev + 1;
} 

} } }
#endif /*RESIDUAL_ITERATOR_HPP_*/
