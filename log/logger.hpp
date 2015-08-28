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

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#ifndef CONFIG_FAST
#include <fstream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <map>
#include <iostream>
#include "../utils/singleton_resource_manager.hpp"

namespace logs {

enum log_level_t { LEVEL_NONE, LEVEL_STATUS, LEVEL_DEBUG, LEVEL_EVERYTHING, LEVEL_MAX };
extern const char* const LEVEL_NAMES[];

class module_logger;

////////////////////////////////////////////////////////////////////////////////
class log_stream {
private:
   module_logger* underlying;
   int my_level;
   log_stream( module_logger* u, int ml);
   
public:
   template< typename T > log_stream operator << ( const T& rhs );
   
   friend class module_logger;
};

////////////////////////////////////////////////////////////////////////////////
class module_logger : public boost::noncopyable {
   std::string filename;
   log_level_t log_level;
   std::string module_name;

private:
   module_logger( const std::string& fn, const std::string& mn, log_level_t ll = LEVEL_DEBUG ) :
      filename(fn), 
      log_level(ll), 
      module_name(mn) {
      // does nothing
   }

   std::ofstream& get_stream();
   
public:
   template< typename T > log_stream operator << ( const T& rhs ) {
      log_stream ls( this, log_level );
      ls << rhs;
      
      return ls;
   }
   
   log_stream operator << ( log_level_t level ) {
        return log_stream( this, level );
   }

   ~module_logger() { 
      // does nothing
   }
   
   log_level_t get_log_level() {
      //      std::cerr << "i am : " << module_name << std::endl
      //		<< "my log level : " << log_level << std::endl
      //		<< "parent : " << parent_log_level() << std::endl;

      return std::min( log_level, parent_log_level() );  
   }
   
   friend class log_stream;
   friend void register_logger( const std::string& module_name, const std::string& fn, 
                      const log_level_t );
private:
   log_level_t parent_log_level();
   typedef std::map<std::string, boost::shared_ptr<module_logger> > logger_registry_t;
   friend logger_registry_t& get_registry();
};

////////////////////////////////////////////////////////////////////////////////
module_logger& logger();
module_logger& logger(const std::string& module_name);
void register_logger( const std::string& module_name, const std::string& fn, 
                      const log_level_t = LEVEL_DEBUG );
void register_logger( const std::string& module_name, const log_level_t ll = LEVEL_DEBUG );


////////////////////////////////////////////////////////////////////////////////
template< typename T > 
log_stream log_stream::operator << ( const T& rhs ) {
   if( my_level <= underlying->get_log_level() ) {
      underlying->get_stream() << rhs;
      underlying->get_stream().flush();
   }
//    else {
//       underlying->get_stream() << "Made decision not to print\n"
// 			       << "\t" << rhs << "\n"
// 			       << ".. because my log_level is " << my_level << " and underlying level is "
// 			       << underlying->get_log_level() << "\n";
//    }
   
   return *this;  
}

}

#endif /* CONFIG_FAST */
#endif /*LOGGER_HPP_*/
