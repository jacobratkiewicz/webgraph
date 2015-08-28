include ../flags.mk

all: all_o

all_o: properties.o

%.o : %.cpp  %.hpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~
