include ../flags.mk

compress_webgraph: compress_webgraph.o
	g++ -L$(LIBS) -o compress_webgraph compress_webgraph.o \
			 -lwebgraph -lboost_regex -lboost_program_options -lboost_filesystem

install:
	cp compress_webgraph ~/random-bin

%.o : %.cpp
	g++ -I$(INCLUDES) $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
