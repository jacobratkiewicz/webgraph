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
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include "cxxopts.hpp"

#include "../webgraph/webgraph.hpp"

int main(int argc, char **argv)
{
    using namespace std;
    using namespace webgraph::bv_graph;

    cxxopts::Options options("convert_to_binary_edgelist", "Convert webgraph format to binary edgelist format");
    
    options.add_options()
        ("h,help", "Show help message")
        ("input", "Input webgraph file path", cxxopts::value<string>())
        ("output", "Output binary edgelist file path", cxxopts::value<string>())
        ("do_not_remap", "Disable node ID remapping (even if .nodes file exists)", cxxopts::value<bool>()->implicit_value("true")->default_value("false"))
        ("short", "Use 32-bit vertex IDs instead of 64-bit", cxxopts::value<bool>()->implicit_value("true")->default_value("false"))
    ;
    
    options.parse_positional({"input", "output"});
    options.positional_help("<input webgraph file> <output binary edgelist file>");
    
    auto result = options.parse(argc, argv);
    
    if (result.count("help")) {
        cout << options.help() << endl;
        return 0;
    }
    
    if (!result.count("input") || !result.count("output")) {
        cerr << "Error: Input and output file paths are required" << endl;
        cout << options.help() << endl;
        return 1;
    }
    
    string input_name = result["input"].as<string>();
    string output_name = result["output"].as<string>();
    bool use_short = result["short"].as<bool>();
    
    if (use_short) {
        cerr << "Using 32-bit vertex ID format" << endl;
    } else {
        cerr << "Using 64-bit vertex ID format" << endl;
    }
    
    // 检查是否存在.nodes文件，如果存在则自动启用重映射
    string nodes_file = input_name + ".nodes";
    bool do_not_remap = result["do_not_remap"].as<bool>();
    bool remap_nodes = false;
    
    ifstream nodes_check(nodes_file);
    if (nodes_check.good()) {
        nodes_check.close();
        if (do_not_remap) {
            cerr << "Detected " << nodes_file << " file, but node remapping is disabled" << endl;
            remap_nodes = false;
        } else {
            cerr << "Detected " << nodes_file << " file, enabling node remapping" << endl;
            remap_nodes = true;
        }
    } else {
        cerr << "No " << nodes_file << " file detected, node remapping disabled" << endl;
        remap_nodes = false;
    }
    
    // 读取节点映射文件
    vector<uint64_t> node_mapping;
    if (remap_nodes) {
        ifstream nodes_in(nodes_file);
        if (!nodes_in.is_open()) {
            cerr << "Error: Cannot open node mapping file " << nodes_file << endl;
            return 1;
        }
        
        string line;
        while (getline(nodes_in, line)) {
            if (!line.empty()) {
                node_mapping.push_back(stoull(line));
            }
        }
        nodes_in.close();
        cerr << "Loaded " << node_mapping.size() << " node mappings" << endl;
    }

    typedef boost::shared_ptr<graph> graph_ptr;

    graph_ptr gp = graph::load_sequential(input_name);

    //   cerr << "about to try to get node iterator.\n";

    graph::node_iterator n, n_end;

    tie(n, n_end) = gp->get_node_iterator(0);

    cerr << "num vertices:\n";
    cerr << gp->get_num_nodes() << endl;

    ofstream fout(output_name, ios::binary);
    if(!fout.is_open()) {
        cout << "Open output file failed." << endl;
        exit(1);
    }


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
            
            // 应用节点映射
            if (remap_nodes) {
                if (src < node_mapping.size()) {
                    src = node_mapping[src];
                }
                if (dest < node_mapping.size()) {
                    dest = node_mapping[dest];
                }
            }
            
            if (use_short) {
                uint32_t src32 = static_cast<uint32_t>(src);
                uint32_t dest32 = static_cast<uint32_t>(dest);
                fout.write((char *)&src32, sizeof(uint32_t));
                fout.write((char *)&dest32, sizeof(uint32_t));
            } else {
                fout.write((char *)&src, sizeof(uint64_t));
                fout.write((char *)&dest, sizeof(uint64_t));
            }
            
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
