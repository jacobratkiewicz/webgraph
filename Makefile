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
all: libs

tests: libs
	$(MAKE) -C tests


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
	@$(MAKE) -C asciigraph clean
	@$(MAKE) -C bitstreams clean
	@$(MAKE) -C properties clean
	@$(MAKE) -C utils clean
	@$(MAKE) -C webgraph clean
	@$(MAKE) -C log clean
	@$(MAKE) -C tests clean

