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
  size_t *indexes = nullptr; //does not own
  double norm = 0;
  size_t n = 0;
  size_t size = 0;

  int set_template(size_t *ind, size_t n, size_t size);
  void fill_gramm(size_t nx, size_t ny, int p, int thread, double a, double b, double c, double d);
  void erase()
  {
    if (data)
      delete[] data;
    data = nullptr;
    indexes = nullptr;
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
double bprod(size_t i, size_t j, size_t nx, size_t ny, double a, double b, double hx, double hy, double f(double, double));
void fill_right_side(size_t nx, size_t ny, double *right, int p, int thread, double a, double b, double c, double d, double f(double, double));
int init_gramm_struct(size_t nx, size_t ny, int p, int thread, size_t **indexes_ret);

int solve(msr &a, double *b, msr &m, double *d, double *x, size_t n, double *r, double *u, double *v, int p, int thread);
void mul_msr_by_vec(msr &a, double *x, double *ret, size_t start, size_t stride);

#endif //MSR_H
