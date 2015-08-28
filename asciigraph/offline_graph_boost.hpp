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

#ifndef OFFLINE_GRAPH_BOOST_HPP
#define OFFLINE_GRAPH_BOOST_HPP

#include "offline_graph.hpp"
#include "offline_vertex_iterator.hpp"
#include "offline_edge_iterator.hpp"

#include <exception>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/property_map.hpp>
#include <iostream>
#include <string>

/*!
 * This is the adapter to make this graph work with boost.
 */

//#if !defined BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
namespace boost {

   namespace ag = webgraph::ascii_graph;
   
   // Say what kind of creature this offline graph is.
   struct offline_graph_traversal_category:
      public virtual vertex_list_graph_tag,
      public virtual edge_list_graph_tag,
      public virtual incidence_graph_tag,
      public virtual adjacency_graph_tag {};

   template<>
   struct graph_traits<webgraph::ascii_graph::offline_graph > {
      //* Vertex and edge descriptors - required for all concepts

      typedef ag::offline_graph::vertex_type vertex_descriptor;
      typedef ag::offline_graph::edge_type edge_descriptor;      
      typedef directed_tag directed_category;
      typedef disallow_parallel_edge_tag edge_parallel_category;
      typedef offline_graph_traversal_category traversal_category;
      
      ////////////////////////////////////////////////////////////////////////////////
      /*! vertex_iterator - required by VertexListGraph concept
       *
       */
      typedef ag::offline_vertex_iterator vertex_iterator;
      typedef unsigned int vertices_size_type;
      
      ////////////////////////////////////////////////////////////////////////////////
      /*! edge iterator - required by EdgeListGraph concept
       *
       */
      typedef ag::offline_edge_iterator edge_iterator;
      typedef unsigned int edges_size_type;

      ////////////////////////////////////////////////////////////////////////////////
      /*! out_edge_iterator - required by IncidenceGraph concept
       *
       */
      class out_edge_iterator: 
         public boost::iterator_facade<out_edge_iterator, 
                                       edge_descriptor, 
                                       boost::forward_traversal_tag, 
                                       edge_descriptor> {
      public:
         out_edge_iterator( const vertex_descriptor& vv ) : 
            v( vv ),
            curpos(0),
            end_marker( false ) {
         }
         
         out_edge_iterator() : end_marker(true) {
         }

         friend class boost::iterator_core_access;

         out_edge_iterator( const out_edge_iterator& other ) {
            *this = other;
         }

         out_edge_iterator& operator = ( const out_edge_iterator& other ) {
            v = other.v;
            curpos = other.curpos;
            end_marker = other.end_marker;

            return *this;
         }

      private:
         vertex_descriptor v;
         unsigned curpos;
         bool end_marker;

         void increment() {
            assert( curpos < v.get_successors().size() );
            ++curpos;
         }

         edge_descriptor dereference() const {
//            std::cerr << "curpos: " << curpos << std::endl;

            return std::make_pair( v, v.get_successors().at(curpos) );
         }

         bool equal( const out_edge_iterator& other ) const {
            if( end_marker ) {
               return other.end_marker || other.curpos == other.v.get_successors().size();
            } else if( other.end_marker ) {
               return end_marker || curpos == v.get_successors().size();
            } else {
               return v == other.v;
            }
         }
      };

      typedef unsigned int degree_size_type;

      ////////////////////////////////////////////////////////////////////////////////
      /*! Now, this is all it takes to get an adjacency graph concept
       */
      typedef boost::adjacency_iterator_generator<ag::offline_graph,
                                                  vertex_descriptor,
                                                  out_edge_iterator>::type adjacency_iterator;
         
   }; // end of graph traits definition
   
   ////////////////////////////////////////////////////////////////////////////////
   /*! Stuff for the VertexListGraph concept
    *
    */
   std::pair< graph_traits<webgraph::ascii_graph::offline_graph>::vertex_iterator,
              graph_traits<webgraph::ascii_graph::offline_graph>::vertex_iterator >
   vertices( const webgraph::ascii_graph::offline_graph& g ) {
      return g.get_vertex_iterator( 0 );
   }
   
   graph_traits<webgraph::ascii_graph::offline_graph>::vertices_size_type 
   num_vertices( const webgraph::ascii_graph::offline_graph& g ) {
      return g.get_num_nodes();
   }

   ////////////////////////////////////////////////////////////////////////////////
   /*! Need this for IncidenceGraph and EdgeListGraph
    *
    */
   graph_traits<webgraph::ascii_graph::offline_graph>::vertex_descriptor 
   source(
      graph_traits<webgraph::ascii_graph::offline_graph>::edge_descriptor e,
      const webgraph::ascii_graph::offline_graph& g)
   {
      return e.first;
   }
   
   graph_traits<webgraph::ascii_graph::offline_graph>::vertex_descriptor 
   target(
      graph_traits<webgraph::ascii_graph::offline_graph>::edge_descriptor e,
      const webgraph::ascii_graph::offline_graph& g)
   {
      return e.second;
   }

   ////////////////////////////////////////////////////////////////////////////////
   /*! Need this for EdgeListGraph
    *
    */
   std::pair<graph_traits<webgraph::ascii_graph::offline_graph>::edge_iterator,
             graph_traits<webgraph::ascii_graph::offline_graph>::edge_iterator>
   edges( const webgraph::ascii_graph::offline_graph& g ) {
      return g.get_edge_iterator();
   }

   graph_traits<webgraph::ascii_graph::offline_graph>::edges_size_type
   num_edges( const webgraph::ascii_graph::offline_graph& g ) {
      return g.get_num_edges();
   }

   ////////////////////////////////////////////////////////////////////////////////
   /*! Required for IncidenceGraph
    *
    */
   std::pair<graph_traits<webgraph::ascii_graph::offline_graph>::out_edge_iterator,
             graph_traits<webgraph::ascii_graph::offline_graph>::out_edge_iterator>
   out_edges( const graph_traits<webgraph::ascii_graph::offline_graph>::vertex_descriptor& v,
              const webgraph::ascii_graph::offline_graph& g ) {

      typedef graph_traits<webgraph::ascii_graph::offline_graph>::out_edge_iterator oeitor;
      
      oeitor beg( v );
      oeitor end;

      return std::make_pair( beg, end );
   }

   graph_traits<webgraph::ascii_graph::offline_graph>::degree_size_type
   out_degree( const graph_traits<webgraph::ascii_graph::offline_graph>::vertex_descriptor& v,
               const webgraph::ascii_graph::offline_graph& g ) {
      return v.get_successors().size();
   }

   ////////////////////////////////////////////////////////////////////////////////
   /*! Required for AdjacencyListGraph
    */
   std::pair<graph_traits<webgraph::ascii_graph::offline_graph>::adjacency_iterator,
             graph_traits<webgraph::ascii_graph::offline_graph>::adjacency_iterator>
   adjacent_vertices(const graph_traits<webgraph::ascii_graph::offline_graph>::vertex_descriptor& v,
                     const webgraph::ascii_graph::offline_graph& g) {
      typedef graph_traits<webgraph::ascii_graph::offline_graph>::out_edge_iterator oeitor;

      oeitor ebeg, eend;
      tie( ebeg, eend ) = out_edges( v, g );
      
      typedef graph_traits<webgraph::ascii_graph::offline_graph>::adjacency_iterator aitor;

      aitor abeg( ebeg, &g ), aend( eend, &g );

      return make_pair( abeg, aend );
   }

   ////////////////////////////////////////////////////////////////////////////////
   /*! This is for making property maps out of these graphs.
    *
    */
 
   // we just need this for a type name; the data is actually stored in the graph.
   class vertex_index_accessor {}; // models readable property map
   
   vertex_index_accessor
   get( boost::vertex_index_t, const webgraph::ascii_graph::offline_graph& ) {
      // needs to return a type that, given a vertex descriptor, returns its index in
      // the graph

      return vertex_index_accessor();
   }
   
   webgraph::ascii_graph::vertex_label_t get( const vertex_index_accessor&,
                                              const webgraph::ascii_graph::vertex_descriptor& d ) {
      return d.get_label();
   }
                                              

   template<>
   class property_traits<vertex_index_accessor> {
   public:
      typedef webgraph::ascii_graph::vertex_label_t     value_type;
      typedef webgraph::ascii_graph::vertex_label_t     reference;
      typedef webgraph::ascii_graph::vertex_descriptor  key_type;
      typedef readable_property_map_tag                 category;
   };
}

#endif
