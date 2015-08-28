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

#ifndef PROPERTIES_HPP_
#define PROPERTIES_HPP_

#include <fstream>
#include <string>
#include <map>

namespace webgraph
{

class properties
{
private:
   std::map<std::string, std::string> back;
 
public:
   properties();
   virtual ~properties();
   
   void load( std::istream& in );

   void store( std::ostream& out );
   void store( std::ostream& out, const std::string& title );
	
   void set_property( const std::string key, std::string value ) {
      back[key] = value;
   }
   
   bool has_property( const std::string& name ) const {
      using namespace std;
      map<string, string>::const_iterator i = back.find( name );

      if( i == back.end() )
         return false;
      else
         return true;
   }
   
   std::string get_property( const std::string& name ) const {
      return back.find(name)->second;
   }
};

}

#endif /*PROPERTIES_HPP_*/
