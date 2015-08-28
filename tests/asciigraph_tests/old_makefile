include ../../flags.mk

all: test_offline_edge_iterator test_offline_vertex_iterator test_ascii_graph

all_o: test_ascii_graph.o test_offline_edge_iterator.o test_offline_vertex_iterator.o 

test_offline_vertex_iterator: test_offline_vertex_iterator.o  
	@make -C .. offline_vertex_iterator.o
	g++ $(FLAGS) -o test_offline_vertex_iterator $^ ../offline_vertex_iterator.o

test_offline_edge_iterator: test_offline_edge_iterator.o 
	@make -C .. offline_vertex_iterator.o
	@make -C .. offline_edge_iterator.o
	g++ $(FLAGS) -o test_offline_edge_iterator $^ ../offline_vertex_iterator.o ../offline_edge_iterator.o

test_ascii_graph: test_ascii_graph.o
	@make -C .. offline_vertex_iterator.o
	@make -C .. offline_edge_iterator.o
	@make -C .. offline_graph.o
	g++ $(FLAGS) -o test_ascii_graph test_ascii_graph.o ../offline_vertex_iterator.o ../offline_edge_iterator.o \
					../offline_graph.o

test%.o: test%.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~

