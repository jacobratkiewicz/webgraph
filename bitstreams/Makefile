include ../flags.mk

all: all_o
ifndef CONFIG_FAST
	$(MAKE) -C tests all
endif

all_o: bitstreams
ifndef CONFIG_FAST
	$(MAKE) -C tests all_o
endif

bitstreams: input_bitstream.o output_bitstream.o ../utils/fast.o

%.o: %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
