# Tests makefile

include ../../../flags.mk

all: test_merged_iterator test_masked_iterator test_interval_sequence_iterator

all_o: test_interval_sequence_iterator.o test_masked_iterator.o test_merged_iterator.o

test_wrapped_iterators: test_wrapped_iterators.o
	g++ $(FLAGS) -o test_wrapped_iterators test_wrapped_iterators.o

test_merged_iterator: test_merged_iterator.o
	g++ $(FLAGS) -o test_merged_iterator test_merged_iterator.o

test_masked_iterator: test_masked_iterator.o
	g++ $(FLAGS) -o test_masked_iterator test_masked_iterator.o

test_interval_sequence_iterator: test_interval_sequence_iterator.o
	g++ $(FLAGS) -o test_interval_sequence_iterator test_interval_sequence_iterator.o

test_iterator_wrappers: test_iterator_wrappers.o
	g++ $(FLAGS) -o test_iterator_wrappers test_iterator_wrappers.o

clean:
	rm -f *.o
	rm -f test_merged_iterator
	rm -f test_masked_iterator
	rm -f core.*
	rm -f *~

%.o : %.cpp
	g++ $(FLAGS) -c $<

