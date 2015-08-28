all: bitstream_stress_test compute_indegree compute_outdegree

include ../flags.mk

INCLUDES = /nobackup/jpr/mylibs/include
LIBS = /nobackup/jpr/mylibs/lib

bitstream_stress_test: bitstream_stress_test.o
	g++ $(FLAGS) -o bitstream_stress_test -I$(INCLUDES) -L$(LIBS) \
			-L../bitstreams bitstream_stress_test.o \
			-lbitstreams -lutil

compute_indegree: compute_indegree.o
	g++ $(FLAGS) -o compute_indegree compute_indegree.o -L.. \
			-lwebgraph -lboost_filesystem -lboost_regex

compute_outdegree: compute_outdegree.o
	g++ $(FLAGS) -o compute_outdegree compute_outdegree.o -L.. \
			-lwebgraph -lboost_filesystem -lboost_regex

%.o: %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
