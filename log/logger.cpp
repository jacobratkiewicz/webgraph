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

#include "logger.hpp"

#ifndef CONFIG_FAST

#include <map>
#include <boost/regex.hpp>

namespace logs {

using namespace std;

log_stream::log_stream( module_logger* u, int ml) : underlying(u), my_level(ml) {
   *this << underlying->module_name << " (" << LEVEL_NAMES[ml] << "): ";
}


const char* const LEVEL_NAMES[] = { "NONE", "STATUS", "DEBUG", "EVERYTHING", "MAX" };

const char* const DEFAULT_LOGGER_NAME = "LOG";
const char* const DEFAULT_LOGGER_FILE = "LOG.txt";

typedef map<string, boost::shared_ptr<module_logger> > logger_registry_t;

////////////////////////////////////////////////////////////////////////////////
/**
 * Gives access to the logger registry. This function is private to the module.
 */
logger_registry_t& get_registry() {
   static logger_registry_t singleton;
   static bool initialized = false;
   
   if( !initialized ) {
      initialized = true;
      singleton[DEFAULT_LOGGER_NAME].reset(new module_logger( DEFAULT_LOGGER_FILE,
                                                              DEFAULT_LOGGER_NAME, 
                                                              LEVEL_MAX )); 
   }
   
   return singleton;  
}

////////////////////////////////////////////////////////////////////////////////
module_logger& logger() {
   return logger( DEFAULT_LOGGER_NAME );
}

////////////////////////////////////////////////////////////////////////////////
string parent_logger_name( const string& module_name ) {
   boost::regex chopper( "^(.*)::.*$" );
   boost::smatch parent_match;
   
   if( boost::regex_match( module_name, parent_match, chopper ) ) {
     // Found some ::
     if( get_registry().find( parent_match[1] ) != get_registry().end() )
       return parent_match[1];
     else
       // chop again
       return parent_logger_name( parent_match[1] );
   } else {
      return DEFAULT_LOGGER_NAME;  
   }
}

////////////////////////////////////////////////////////////////////////////////
module_logger& logger( const string& module_name ) { 
   assert( get_registry().find( module_name ) != get_registry().end() );
   return *(get_registry())[module_name];  
}

////////////////////////////////////////////////////////////////////////////////
void register_logger( const string& module_name, const std::string& fn, const log_level_t ll) {
   // TODO - change this later to handle multiple registration attempts intelligently.
   //assert( get_registry().find( module_name ) == get_registry().end() );
  if( get_registry().find( module_name ) != get_registry().end() ) {
    // then it already exists
    // TODO - decide what to do if the new version is defined with different
    // params.
    return;
  }
   
   get_registry()[module_name].reset(new module_logger( fn, module_name, ll));
}

////////////////////////////////////////////////////////////////////////////////
void register_logger( const string& module_name, const log_level_t ll ) {
  register_logger( module_name, DEFAULT_LOGGER_FILE, ll );
}
  
////////////////////////////////////////////////////////////////////////////////
// need a quick functor to call the constructor of an ofstream

ofstream& module_logger::get_stream() {
  typedef utils::resource_manager::singleton_resource_manager<
    string, 
    ofstream
    > mgr_t;
  
  return mgr_t::get_instance().get( filename );
}

////////////////////////////////////////////////////////////////////////////////
log_level_t module_logger::parent_log_level() {
   return logger( parent_logger_name( module_name ) ).log_level;
}

} // end namespace

#endif
