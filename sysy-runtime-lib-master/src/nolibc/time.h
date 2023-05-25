#ifndef LIBSYSY_TIME_H_
#define LIBSYSY_TIME_H_

#include "sys.h"
#include "types.h"

struct timeval {
  long tv_sec;
  long tv_usec;
};

int gettimeofday(struct timeval *tv, void *tz);

#endif  // LIBSYSY_TIME_H_
