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

#ifndef FSTREAM_MANAGER_HPP
#define FSTREAM_MANAGER_HPP

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <boost/utility.hpp>

/** 
 * This class manages resources associated with keys, ensuring that only one resource
 * associated with a key exists. (this may obviously be applied to filenames and fstreams,
 * for instance).
 *
 * It is meant to be used as a singleton, so a suitable factory method is provided.
 */

namespace utils {
namespace resource_manager {

// this is a functor that just calls a constructor with the given argument, and returns
// a pointer to the result (which will be on the heap).
template<class arg_type, class class_type>
struct constructor_caller {
   class_type* operator() ( const arg_type& a ) {
      return new class_type( a );
   }
};

// Specialize for ofstreams and strings, since (annoyingly) the ofstream ctor wants an old-style
// string.
template<>
struct constructor_caller<std::string, std::ofstream> {
   std::ofstream* operator() ( const std::string& a ) {
      return new std::ofstream( a.c_str() );
   }
};


template< class key_type, class resource_type, 
	  class generator_type = constructor_caller<key_type, resource_type> >
class singleton_resource_manager : public boost::noncopyable {
   typedef boost::shared_ptr< resource_type > rp;

   // registry
   std::map< key_type, rp > registry;
   
   // "generator" functor which is used to generate new resources as necessary.
   // it is assumed that this is "prototyped" as follows
   // resource_type* generator( const key_type& constructor_args );
   // It should also be default-constructable.
   generator_type generator;

   singleton_resource_manager(generator_type gen) : generator(gen) {}
   singleton_resource_manager() {}

public:
   resource_type& get( const key_type& k ) {
      // create a new one if it doesn't exist.
      if( registry.find( k ) == registry.end() )
	 registry[k].reset( generator(k) );

      return *registry[k]; 
   }

   static singleton_resource_manager<key_type, resource_type, generator_type>& get_instance() {
      static singleton_resource_manager<key_type, resource_type, generator_type> me;

      return me;
   }
};

} }
#endif
