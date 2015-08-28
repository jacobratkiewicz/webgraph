include ../flags.mk

ifdef CONFIG_DBG
all: all_o
	$(MAKE) -C tests all

all_o: logger.o
	$(MAKE) -C tests all_o

%.o : %.cpp  %.hpp
	g++ $(FLAGS) -c $<
else
all:
	@echo "Nothing to do for logs in fast configuration..."
endif

clean:
	rm -f *.o
	rm -f *~
	$(MAKE) -C tests clean

