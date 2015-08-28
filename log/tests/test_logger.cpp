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

#include <iostream>
#include "../logger.hpp"

using namespace std;

int main( int, char** ) {
#ifndef CONFIG_FAST
   cerr << "Not testing logger as it is not compiled.\n";
#else
   cerr << "Getting the default logger and printing some stuff.";
   
   using namespace logs;
   
   logger() << "Root log level - " << logger().get_log_level() << "\n";

   logger() << LEVEL_DEBUG 
            << "This message is being output to the default logger at the debug level.\n";
   logger() << LEVEL_STATUS << "This message is being output at the status level.\n";
   logger() << LEVEL_STATUS << "This message should be as well.\n";
   logger() << "Registering a logger for module \"a\" with level STATUS....\n";
   
   register_logger( "a", LEVEL_STATUS );
   
   logger("a") << "This message is going out to a.\n";
   
   register_logger( "a::b", LEVEL_STATUS );
   
   logger("a::b") << "this message is going out to a::b\n";
   
   logger("a::b") << LEVEL_DEBUG << "We should not see this message.\n";

   register_logger( "c::d", LEVEL_STATUS );

   logger( "c::d" ) << "This is a test. Should be debug level?" << "\n";
   logger( "c::d" ) << LEVEL_DEBUG << "Should not see this message.\n";
#endif
}
