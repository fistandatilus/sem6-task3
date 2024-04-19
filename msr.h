#ifndef MSR_H
#define MSR_H

#include <sys/types.h>
#include "thread_related.h"

struct msr
{
  double *data = nullptr;
  size_t *indexes = nullptr;
  size_t n = 0;

  int init_gramm(size_t nx, size_t ny, int p, int thread);
  void erase()
  {
    if (data)
      delete[] data;
    if (indexes)
      delete[] indexes;
  }
};

#endif //MSR_H
