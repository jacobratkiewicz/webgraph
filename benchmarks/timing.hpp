#ifndef TIMING_HPP
#define TIMING_HPP

#include <sys/time.h>

namespace timing {

typedef timeval time_t;

inline time_t timer() {
   timeval t;
   gettimeofday( &t, NULL );

   return t;
}


inline double calculate_elapsed( const time_t& start, const time_t& finish ) {
   size_t num_seconds = finish.tv_sec - start.tv_sec;
   double num_us;

   if( num_seconds == 0 ) {
      num_us = finish.tv_usec - start.tv_usec;
   } else {
      num_us = 10e6 - start.tv_usec + finish.tv_usec;
   }

   return num_seconds + num_us / 10e6;
}

}


#endif
