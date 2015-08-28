include ../flags.mk

all: all_o

all_o: fast.o

%.o: %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
	make -C tools clean