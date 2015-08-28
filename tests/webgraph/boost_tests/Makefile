include ../../../flags.mk

linklibs = -lboost_regex -lboost_filesystem -lboost_program_options -lwebgraph

all: print_graph test_incidence_and_adjacency bv_to_ascii compute_pagerank

print_graph: print_graph.o
	g++ $(FLAGS) -o print_graph print_graph.o $(linklibs)

test_incidence_and_adjacency: test_incidence_and_adjacency.o
	g++ $(FLAGS) -o test_incidence_and_adjacency test_incidence_and_adjacency.o $(linklibs)

bv_to_ascii: bv_to_ascii.o
	g++ $(FLAGS) -o bv_to_ascii bv_to_ascii.o $(linklibs)

compute_pagerank: compute_pagerank.o
	g++ $(FLAGS) -o compute_pagerank compute_pagerank.o $(linklibs)

clean:
	rm -f *.o

%.o: %.cpp
	g++ $(INCLUDES) $(FLAGS) -c $<
