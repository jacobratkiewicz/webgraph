#
include ../flags.mk

all: all_o
#ifndef CONFIG_FAST
#	$(MAKE) -C tests all
#endif

all_o: offline_edge_iterator.o offline_vertex_iterator.o offline_graph.o vertex.o edge.o
ifndef CONFIG_FAST
#	$(MAKE) -C tests all_o
endif


tokenizer_test: tokenizer_test.cpp
	g++ $(FLAGS) -o tokenizer_test tokenizer_test.cpp

ascii_graph_test: ascii_graph_test.o \
		  offline_vertex_iterator.o \
		  ascii_graph.o \
		  offline_edge_iterator.o \
		  vertex.o \
		  edge.o
	g++ $(FLAGS) -o ascii_graph_test \
			ascii_graph_test.o \
			offline_vertex_iterator.o ascii_graph.o \
			offline_edge_iterator.o \
			vertex.o \
			edge.o

%.o : %.cpp  %.hpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
	$(MAKE) -C tests clean



