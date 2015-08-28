include ../../flags.mk

all: generate_random_graph

generate_random_graph: generate_random_graph.o
	g++ $(FLAGS) -o generate_random_graph generate_random_graph.o -lboost_program_options

%.o: %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
