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

#include "properties.hpp"

#include <boost/regex.hpp>
#include <iostream>
#include <ctime>

//#define HARDCORE_DEBUG

namespace webgraph
{

using namespace std;

properties::properties()
{
}

properties::~properties()
{
}

void properties::load( std::istream& in ) {
   using namespace boost;

   regex comment( "#.*" );
   regex splitter( "(.*?)\\s*=\\s*(.*)" );
   regex splitterblank( "(.*?)\\s*=\\s*$" );

   string nextline;

#ifdef HARDCORE_DEBUG
   cerr << "Load called.\n";
#endif

   while( getline( in, nextline ) ) {
#ifdef HARDCORE_DEBUG
      cerr << "Just fetched line: " << nextline << endl;
#endif
      
      // throw away comments
      if( regex_match( nextline, comment ) ) {
         continue;
      }
      
      smatch what;

      if( regex_match( nextline, what, splitter ) ) {
         // what[1] contains the key, what[2] contains the value
         back[what[1]] = what[2];
      } else if( regex_match( nextline, what, splitterblank ) ) {
         back[what[1]] = "";  
      } else {
//         cerr << "Everything broke for " << nextline << endl;
         abort();  
      }
#ifdef HARDCORE_DEBUG      
      cerr << "Loaded property " << what[1] << " = " << back[what[1]] << endl;
#endif
   }
}

void properties::store( std::ostream& out, const std::string& title ) {
   using namespace std;
   
   
   time_t rawtime;
   struct tm * timeinfo;
   
   time ( &rawtime );
   timeinfo = localtime ( &rawtime );

   out << "# " << title << "\n" 
       << "# " << asctime( timeinfo );

   for( map<string, string>::iterator itor = back.begin();
        itor != back.end();
        ++itor ) {
      out << itor->first << "=" << itor->second << "\n";
   }
}


void properties::store( std::ostream& out ) {
   store( out, "" );
}

}
