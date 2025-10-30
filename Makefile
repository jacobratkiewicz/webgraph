SHELL := /bin/bash

include flags.mk

files = asciigraph/offline_graph.o \
	asciigraph/vertex.o \
	asciigraph/offline_edge_iterator.o \
	asciigraph/offline_vertex_iterator.o \
	bitstreams/input_bitstream.o \
	bitstreams/output_bitstream.o \
	properties/properties.o \
	utils/fast.o \
	webgraph/compression_flags.o \
	webgraph/webgraph.o \
	webgraph/iterators/node_iterator.o

#
# default target
#
all: info libs tests

info:
	@echo "Compile WebGraph ..."
	@echo "Shell: $(SHELL)"
	@echo "INCLUDES: $(INCLUDES)"
	@echo "LIBS: $(LIBS)"
	@echo "FLAGS: $(FLAGS)"

tests: libs
	pushd tests && \
	$(MAKE) -C . && \
	popd



libs:
	$(MAKE) -C asciigraph all
	$(MAKE) -C utils all
	$(MAKE) -C bitstreams all
	$(MAKE) -C properties all
	$(MAKE) -C webgraph all
#	$(MAKE) -C log all
	ar -r libwebgraph.a $(files)

clean:
	rm -f core*
	rm -f *~
	rm -f *.o
	rm -f *.a
	rm -f *.so
	@$(MAKE) -C asciigraph clean
	@$(MAKE) -C bitstreams clean
	@$(MAKE) -C properties clean
	@$(MAKE) -C utils clean
	@$(MAKE) -C webgraph clean
	@$(MAKE) -C log clean
	@$(MAKE) -C tests clean

.PHONY: all tests libs info clean

