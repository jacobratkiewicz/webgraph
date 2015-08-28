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

#ifndef WEBGRAPHVERTEX_HPP_
#define WEBGRAPHVERTEX_HPP_

#include <utility>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

namespace webgraph
{

   using namespace std;

   class webgraph_vertex
   {
   public:
      typedef unsigned int node_type;

   private:
      vector<node_type> successors;
      int label;
	
   public:
      webgraph_vertex() : successors(0), label(-1) {	
      }

      webgraph_vertex( node_type l, const vector<node_type>& s ) : successors(s), label(l) {
      }
	
      virtual ~webgraph_vertex() {}
	
      typedef vector<node_type>::const_iterator successor_iterator;
	
      webgraph_vertex& operator = ( const webgraph_vertex& that ) {
         this->successors = that.successors;
         this->label = that.label;

         return *this;
      }
      
      pair<successor_iterator, successor_iterator> get_successor_iterator() {
         return make_pair(successors.begin(), successors.end() );	
      }

      //! Get a successor iterator starting with a particular successor.
      /*!
       * This will work because successors are unique.
       */
      pair<successor_iterator, successor_iterator> 
      get_successor_iterator( node_type successor_label ) { 
         successor_iterator start, end;

         start = find( successors.begin(), successors.end(), successor_label );
         end = successors.end();

         return make_pair( start, end );
      }

      bool operator == ( const webgraph_vertex & other ) {
         return this->label == other.label;
      }
	
      node_type get_label() {
         return label;
      }

      string as_str() const {
         ostringstream o;

         o << "<" << label << "> : ";

         copy( successors.begin(), successors.end(), ostream_iterator<int>( o, " " ) );

         return o.str();
      }
   };
}

#endif /*WEBGRAPHVERTEX_HPP_*/
