#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>
#include <numeric>
#include <iostream>

#include "../webgraph/webgraph.hpp"
#include "../webgraph/boost/integration.hpp"

#include "timing.hpp"

int main( int argc, char** argv ) {
   using namespace std;
   using namespace boost;
   using webgraph::bv_graph::graph;

   typedef shared_ptr<graph> graph_ptr;

   timing::time_t start, end;
   start = timing::timer();

   graph_ptr g = graph::load( argv[1] );

   typedef vector<size_t> indegree_vector_t;
   indegree_vector_t indegrees( num_vertices(*g) );
   fill( indegrees.begin(), indegrees.end(), size_t(0) );

   graph_traits<graph>::vertex_iterator v, v_end;

   for( tie( v, v_end ) = vertices( *g ); v != v_end; ++v ) {
      graph_traits<graph>::adjacency_iterator a, a_end;

      for( tie( a, a_end ) = adjacent_vertices( *v, *g ); a != a_end; ++a ) {
         indegrees[*a]++;
      }
   }

   size_t m = *(max_element( indegrees.begin(), indegrees.end()));

   cout << m << endl;

   end = timing::timer();

   double elapsed = timing::calculate_elapsed( start, end );

   cout << " " << elapsed << endl;
   
   return 0;
}
