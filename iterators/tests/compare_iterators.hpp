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

#ifndef COMPARE_ITERATORS_HPP
#define COMPARE_ITERATORS_HPP

#include "../utility_iterator_base.hpp"
#include "../iterator_wrappers.hpp"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
template<class my_itor, class exp_itor>
void compare_iterators( my_itor mi,
                        exp_itor expected,
                        exp_itor expected_end ) {
   using namespace std;

   while( mi.has_next() ) {
      int cur_value = mi.next();

      if( cur_value != *expected ) {
         cerr << "Error - got " << cur_value << ", expected " << *expected << endl;
         return;
      }

      if( expected == expected_end && mi.has_next() ) {
         cerr << "mi is still returning stuff, but expected is done" << endl;
         return;
      }

      if( expected == expected_end && !mi.has_next() ) {
         break;
      }
      ++expected;
   }

   if( expected != expected_end ) {
      cerr << "mi ran out too soon." << endl;
      return;
   }

   cerr << "Everything seems to have worked.\n";
}

////////////////////////////////////////////////////////////////////////////////
template<class my_itor, class exp_itor>
void compare_iterators_polymorphic( my_itor mi,
                                    exp_itor expected,
                                    exp_itor expected_end ) {
   // cast mine to the base class before doing anything...
   using namespace webgraph::bv_graph::utility_iterators;
   using namespace std;
   
   utility_iterator_base<int>* uib = &mi;

   while( uib->has_next() )  {
      int cur_value = uib->next();
      
      if( cur_value != *expected ) {
         cerr << "Error - got " << cur_value 
              << ", expected " << *expected << endl;
      }

      if( expected == expected_end && uib->has_next() ) {
         cerr << "uib is still returning stuff, but expected is done" << endl;
         return;
      }

      if( expected == expected_end && !uib->has_next() ) {
         break;
      }
      
      ++expected;
   }

   if( expected != expected_end ) {
      cerr << "uib ran out too soon." << endl;
      return;
   }
   
   cerr << "Everything seems to have worked.\n";
}

////////////////////////////////////////////////////////////////////////////////
template<class my_itor, class exp_itor>
void compare_iterators_wrapped( my_itor mi,
                                exp_itor expected, exp_itor expected_end ) {
   using namespace std;
   using namespace webgraph::bv_graph;

   // wrap both, and compare them backwards.
   typedef iterator_wrappers::java_to_cpp<int> jtc_t;
   typedef iterator_wrappers::cpp_to_java<exp_itor, int> ctj_t;

   jtc_t jtc( mi ), jtc_end;
   ctj_t ctj( expected, expected_end );

   cerr << "Comparing wrapped iterators backwards.\n";
   compare_iterators( ctj, jtc, jtc_end );

   cerr << "Now comparing the cpp->java iterator polymorphically.\n";
   compare_iterators_polymorphic( ctj, jtc, jtc_end );
}

#endif
