#include "msr.h"

#define UNFOLD 8

void mul_msr_by_vec(msr &a, double *x, double *ret, size_t start, size_t stride)
{
  for (size_t i = start; i < start + stride; i++)
  {
    double s = a.data[i] * x[i];
    size_t len = a.indexes[i+1] - a.indexes[i];
    for (size_t j = 0; j < len; j++)
      s += a.data[a.indexes[i] + j] * x[a.indexes[a.indexes[i] + j]];
    ret[i] = s;
  }
}

static inline void subtract_vecs_coeff(double *a, double *b, double c, int n)
//does a -= c*b in sense of vectors
{
  int i, rem = n%UNFOLD;
  for (i = 0; i < rem; i++)
    a[i] -= c*b[i];
  for(; i < n; i += UNFOLD)
  {
    a[i] -= c*b[i];
    a[i+1] -= c*b[i+1];
    a[i+2] -= c*b[i+2];
    a[i+3] -= c*b[i+3];
    a[i+4] -= c*b[i+4];
    a[i+5] -= c*b[i+5];
    a[i+6] -= c*b[i+6];
    a[i+7] -= c*b[i+7];
  }
}

int inv_m_mul_vec(msr &m, double *d, double *r, double *v, size_t start, size_t stride)
{
  for (size_t j = start + stride - 1; j >= start; j++)
  {

  }
}

int solve(msr &a, double *b, msr &m, double *d, double *x, size_t n, double *r, double *u, double *v, int p, int thread, int max_it)
{
  size_t stride, start;
  start_and_size(p, thread, n, start, stride);

  memset(x + start, 0, stride*(sizeof(double)));
  mul_msr_by_vec(a, x, r, start, stride);
  subtract_vecs_coeff(r, b, 1, n);
  double eps = 0;

  for (size_t i = start; i < start + stride; i++)
    eps += fabs(b[i]);
  reduce_sum(p, &eps, 1);
  eps *= EPS*EPS;

  for (int iter = 1; iter <= max_it; iter++)
  {
    inv_m_mul_vec(m, d, r, v, start, stride);
  }
}
