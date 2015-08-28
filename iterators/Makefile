include ../../flags.mk

all: all_o
ifndef CONFIG_FAST
	$(MAKE) -C tests all
endif

all_o: node_iterator.o
ifndef CONFIG_FAST
	$(MAKE) -C tests all_o
endif

%.o : %.cpp  %.hpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
