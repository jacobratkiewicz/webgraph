
include ../flags.mk

all: everything_for_webgraph
	$(MAKE) -C iterators all

everything_for_webgraph: all_o 
	$(MAKE) webgraph.o
	$(MAKE) -C ../asciigraph/ all_o
	$(MAKE) -C iterators/ all_o
	$(MAKE) -C ../bitstreams/ all_o
	$(MAKE) -C ../properties/ all_o
	$(MAKE) -C ../utils all_o

# 	g++ $(FLAGS) -o webgraph webgraph.o ../asciigraph/offline_graph.o \
# 				 iterators/node_iterator.o iterators/residual_iterator.o \
# 				 ../bitstreams/input_bitstream.o ../bitstreams/output_bitstream.o \
# 				 compression_flags.o ../utils/fast.o \
# 				 ../asciigraph/offline_vertex_iterator.o \
# 				 ../properties/properties.o \
# 				 ../asciigraph/offline_edge_iterator.o \
# 				 -lboost_regex -lboost_filesystem -lboost_program_options

all_o: compression_flags.o webgraph.o webgraph_vertex.o
	$(MAKE) -C iterators all_o

%.o : %.cpp  %.hpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
	$(MAKE) -C iterators clean

