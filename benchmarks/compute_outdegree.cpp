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

   graph_traits<graph>::vertex_iterator v, v_end;

   size_t m = 0;

   for( tie( v, v_end ) = vertices( *g ); v != v_end; ++v ) {
      size_t n = out_degree( *v, *g );
      if( n > m ) {
         m = n;
      }
   }

   cout << m << endl;

   end = timing::timer();

   double elapsed = timing::calculate_elapsed( start, end );

   cout << " " << elapsed << endl;
   
   return 0;
}
