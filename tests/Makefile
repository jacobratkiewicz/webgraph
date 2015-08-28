include ../../flags.mk

all: all_o print_webgraph

print_webgraph: print_webgraph.o
	make print_webgraph.o
	make -C .. everything_for_webgraph
	g++ $(FLAGS) -o print_webgraph print_webgraph.o \
				 ../webgraph.o ../../asciigraph/offline_graph.o \
				 ../iterators/node_iterator.o \
				 ../iterators/residual_iterator.o \
				 ../../bitstreams/input_bitstream.o \
				 ../../bitstreams/output_bitstream.o \
				 ../compression_flags.o \
				 ../../utils/fast.o \
				 ../../asciigraph/offline_vertex_iterator.o \
				 ../../properties/properties.o \
				 ../../asciigraph/offline_edge_iterator.o \
				 -lboost_regex -lboost_filesystem -lboost_program_options


all_o: print_webgraph.o

%.o : %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~


