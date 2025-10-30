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
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "../webgraph/webgraph.hpp"

int main(int argc, char **argv)
{
    using namespace std;
    using namespace webgraph::bv_graph;

    if (argc < 3)
    {
        cerr << "convert_to_text [webgraph_path] [output_file_path]" << endl;
        return 1;
    }

    string input_name = argv[1];
    string output_name = argv[2];


    typedef boost::shared_ptr<graph> graph_ptr;

    graph_ptr gp = graph::load_sequential(input_name);

    //   cerr << "about to try to get node iterator.\n";

    graph::node_iterator n, n_end;

    tie(n, n_end) = gp->get_node_iterator(0);

    cerr << "num vertices:\n";
    cerr << gp->get_num_nodes() << endl;

    ofstream fout(output_name);

    size_t node_count = 0;
    size_t edge_count = 0;
    size_t total_node = gp->get_num_nodes();

    while (n != n_end)
    {
        webgraph::bv_graph::graph::successor_iterator succ, succ_end;

        // cerr << "outdegree : " << outdegree(n) << endl;
        tie(succ, succ_end) = successors(n);

        while (succ != succ_end)
        {
            // cerr << edge_count << "th" << endl;
            uint64_t src = *n;
            uint64_t dest = *succ;
            fout << src << " " << dest << endl;
            
            // cerr << src << " " << dest << endl;
            
            ++succ;
            ++edge_count;
        }

        // cerr << "before ++n" << endl;
        
        ++n;
        ++node_count;

        if(node_count % 100000 == 0)
        {
            cerr << "progress: " << node_count << "/" << total_node << endl;
            cerr << "edge count: " << edge_count << endl;
        }
    }

    cerr << "node count: " << node_count << endl;
    cerr << "edge count: " << edge_count << endl;

    return 0;
}
