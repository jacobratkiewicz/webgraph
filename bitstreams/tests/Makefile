include ../../flags.mk

all: reader_writer_test

obitstream_test_minimal: obitstream_test_minimal.o
	make -C .. bitstreams
	make -C ../../log logger.o
	make -C ../../utils fast.o
	g++ $(FLAGS) -o obitstream_test_minimal obitstream_test_minimal.o \
			../output_bitstream.o ../../log/logger.o ../../utils/fast.o -lboost_regex

ibitstream_test: ibitstream_test.o
	make -C .. bitstreams
	make -C ../../log logger.o
	make -C ../../utils fast.o
	g++ $(FLAGS) -o ibitstream_test ibitstream_test.o ../input_bitstream.o \
			../../log/logger.o ../../utils/fast.o -lboost_regex

obitstream_test: obitstream_test.o
	make -C .. bitstreams
	make -C ../../log logger.o
	make -C ../../utils fast.o
	g++ $(FLAGS) -o obitstream_test obitstream_test.o ../output_bitstream.o \
			../../log/logger.o ../../utils/fast.o -lboost_regex

reader_writer_test: reader_writer_test.o
	make -C .. bitstreams
	make -C ../../log logger.o
	make -C ../../utils fast.o
	g++ $(FLAGS) -o reader_writer_test reader_writer_test.o ../input_bitstream.o ../output_bitstream.o ../../log/logger.o ../../utils/fast.o -lboost_regex

all_o: reader_writer_test.o

%.o: %.cpp
	g++ $(FLAGS) -c $<

clean:
	rm -f reader_writer_test *.o
	rm -f *~
