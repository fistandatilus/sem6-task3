#ifndef MSR_H
#define MSR_H

#include <sys/types.h>
#include <string.h>
#include <math.h>
#include "thread_related.h"

#define EPS (1e-15)

struct msr
{
  double *data = nullptr;
  size_t *indexes = nullptr;
  double norm = 0;
  size_t n = 0;
  size_t size = 0;

  int init_gramm_struct(size_t nx, size_t ny, int p, int thread);
  void fill_gramm(size_t nx, size_t ny, int p, int thread, double a, double b, double c, double d);
  void erase()
  {
    if (data)
      delete[] data;
    if (indexes)
      delete[] indexes;
    n = 0;
    size = 0;
    norm = 0;
  }
  ~msr()
  {
    erase();
  }
  msr(msr &) = delete;
  msr &operator=(msr &) = delete;
  int copy(msr &x);
  int copy_template(msr &x);

};

int form_preconditioner(msr &a, msr &precond, double *diag, int p, int thread);
size_t bin_search(size_t *a, size_t n, size_t x);

#endif //MSR_H
