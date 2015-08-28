#ifndef BENCHMARK_CALLERS_HPP
#define BENCHMARK_CALLERS_HPP

// want to write this as fast as possible
// gamma, delta, zeta, nibble

namespace benchmark {

// ibs stuff

namespace ibs {

struct caller_base {
   virtual int operator() (webgraph::ibitstream* i) = 0;	
};

struct gamma_caller : public caller_base {
   virtual int operator() (webgraph::ibitstream* i) {
      return i->read_gamma();
   }
};

struct delta_caller : public caller_base {
   virtual int operator() (webgraph::ibitstream* i) {
      return i->read_delta();
   }
};

struct zeta_caller : public caller_base {
   virtual int operator() (webgraph::ibitstream* i) {
      return i->read_zeta(5);
   }
};

struct nibble_caller : public caller_base {
   virtual int operator() (webgraph::ibitstream* i) {
      return i->read_nibble();
   }
};

}

// obs stuff
namespace obs {

struct caller_base {
   virtual void operator() ( webgraph::obitstream* i, int param ) = 0;	
};

struct gamma_caller : public caller_base {
   virtual void operator() ( webgraph::obitstream* o, int param ) {
      o->write_gamma( param );
   }
};

struct delta_caller : public caller_base {
   virtual void operator() ( webgraph::obitstream* o, int param ) {
      o->write_delta( param );
   }
};

struct zeta_caller : public caller_base {
   virtual void operator() ( webgraph::obitstream* o, int param ) {
      o->write_zeta( param, 5 );
   }
};

struct nibble_caller : public caller_base {
   virtual void operator() ( webgraph::obitstream* o, int param ) {
      o->write_nibble( param );
   }
};

}

}

#endif
