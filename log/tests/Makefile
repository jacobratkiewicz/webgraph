include ../../flags.mk

all:
	make -C .. all_o
	g++ $(FLAGS) -o test_logger test_logger.cpp ../logger.o -lboost_regex

all_o: test_logger.o

%.o : %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o
	rm -f *~

