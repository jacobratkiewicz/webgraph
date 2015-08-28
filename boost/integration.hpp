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

#ifndef WEBGRAPH_BOOST_HPP
#define WEBGRAPH_BOOST_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/property_map.hpp>
#include <boost/graph/graph_concepts.hpp>

#include <iostream>
#include <string>
#include <utility>

#include "../webgraph.hpp"
#include "../iterators/node_iterator.hpp"
#include "../types.hpp"
#include "edge_iterator.hpp"
#include "out_edge_iterator.hpp"

namespace boost {

   // we assume in the following that we're only dealing with online graphs.
   // At some point these types should be pulled apart, since online and offline graphs
   // are different creatures, really.

   struct bv_graph_traversal_category :
      public virtual vertex_list_graph_tag,
      public virtual edge_list_graph_tag,
      public virtual incidence_graph_tag,
      public virtual adjacency_graph_tag
   {};

   namespace bvg_boost = webgraph::bv_graph::boost_integration;

   template<>
   struct graph_traits< webgraph::bv_graph::graph > {

      //! Vertex and edge descriptors - required for all concepts
      //! These are from types.hpp
      typedef bvg_boost::vertex_descriptor vertex_descriptor;
      typedef bvg_boost::edge_descriptor edge_descriptor;
      
      // Tags
      typedef directed_tag directed_category;
      typedef disallow_parallel_edge_tag edge_parallel_category;
      typedef bv_graph_traversal_category traversal_category;

      ////////////////////////////////////////////////////////////////////////////////
      /*! 
       * vertex_iterator - required by Vertex List Graph concept
       */
      typedef webgraph::bv_graph::node_iterator vertex_iterator;
      typedef unsigned int vertices_size_type;

      ////////////////////////////////////////////////////////////////////////////////
      /*!
       * edge_iterator - required by Edge List Graph concept. Defined in edge_iterator.hpp
       */
      typedef bvg_boost::edge_iterator edge_iterator;
      typedef unsigned int edges_size_type;

      ////////////////////////////////////////////////////////////////////////////////
      // the following is what necessitates an online graph.
      ////////////////////////////////////////////////////////////////////////////////
      
      ////////////////////////////////////////////////////////////////////////////////
      /*! 
       * out_edge_iterator - required by incidence graph concept
       */

      typedef bvg_boost::out_edge_iterator out_edge_iterator;
      typedef unsigned int degree_size_type;
      
      ////////////////////////////////////////////////////////////////////////////////
      /*! required for the adjacency graph concept
       */
//       typedef boost::adjacency_iterator_generator<ag::offline_graph,
//                                                   vertex_descriptor,
//                                                   out_edge_iterator>::type adjacency_iterator;

      typedef webgraph::bv_graph::graph::successor_iterator adjacency_iterator;


   }; // end of graph traits definition

   typedef graph_traits<webgraph::bv_graph::graph> bvg_traits;

   ////////////////////////////////////////////////////////////////////////////////
   /*!
    * Support functions for Vertex List Graph concept
    */
   std::pair< bvg_traits::vertex_iterator, bvg_traits::vertex_iterator >
   vertices( const webgraph::bv_graph::graph& g ) {
      return g.get_node_iterator(0);
   }

   bvg_traits::vertices_size_type num_vertices( const webgraph::bv_graph::graph& g ) {
      return static_cast<bvg_traits::vertices_size_type>(g.get_num_nodes());
   }
                           
   ////////////////////////////////////////////////////////////////////////////////
   /*! 
    * Support for EdgeListGraph concept (and Incidence Graph)
    */
   bvg_traits::vertex_descriptor source( bvg_traits::edge_descriptor e,
                                         const webgraph::bv_graph::graph& g ) {
      return e.first;
   }

   bvg_traits::vertex_descriptor target( bvg_traits::edge_descriptor e,
                                         const webgraph::bv_graph::graph& g ) {
      return e.second;
   }

   std::pair< bvg_traits::edge_iterator, bvg_traits::edge_iterator >
   edges( const webgraph::bv_graph::graph& g ) {
      return make_pair( bvg_boost::edge_iterator( g ),
                        bvg_boost::edge_iterator() );
   }

   bvg_traits::edges_size_type num_edges( const webgraph::bv_graph::graph& g ) {
      return g.get_num_arcs();
   }


   ////////////////////////////////////////////////////////////////////////////////
   /*!
    * Required for IncidenceGraph (and requires the backing graph to be online).
    */
   std::pair< bvg_traits::out_edge_iterator,
              bvg_traits::out_edge_iterator> 
   out_edges( const bvg_traits::vertex_descriptor& v,
              const webgraph::bv_graph::graph& g ) {
      typedef bvg_traits::out_edge_iterator oeitor;

      oeitor beg( v, g );
      oeitor end;

      return make_pair( beg, end );
   }

   bvg_traits::degree_size_type out_degree( const bvg_traits::vertex_descriptor& v,
                                            const webgraph::bv_graph::graph& g ) {
      return g.outdegree( v );
   }
   
   ////////////////////////////////////////////////////////////////////////////////
   /*!
    * Required for AdjacencyListGraph (requires backing graph to be online).
    */
   std::pair< bvg_traits::adjacency_iterator,
              bvg_traits::adjacency_iterator >
   adjacent_vertices( const bvg_traits::vertex_descriptor& v,
                      const webgraph::bv_graph::graph& g ) {
      return g.get_successors( v );
   }
   

   ////////////////////////////////////////////////////////////////////////////////
   /*!
    * This is for making property maps out of these graphs. It's pretty dumb but required
    * for syntactic correctness with the Boost stuff...
    */
   class vertex_index_accessor {}; // models readable property map somehow.
   
   vertex_index_accessor get( boost::vertex_index_t, const webgraph::bv_graph::graph& g ) {
      return vertex_index_accessor();
   }

   typedef unsigned int vertex_label_t;

   vertex_label_t get( const vertex_index_accessor&,
                       const bvg_traits::vertex_descriptor& v ) {
      return static_cast<vertex_label_t>( v );
   }

   template<>
   class property_traits<vertex_index_accessor> {
   public:
      typedef vertex_label_t                 value_type;
      typedef vertex_label_t                 reference;
      typedef bvg_traits::vertex_descriptor  key_type;
      typedef readable_property_map_tag      category;
   };
}

#endif

