#include "msr.h"

size_t bin_search(size_t *a, size_t n, size_t x)
{
  int l = 0, r = n, t;
  while (l != r)
  {
    t = (l + r)/2;
    if (x > a[t]) l = t + 1;
    else r = t;
  }
  return r;
}


int form_preconditioner(msr &a, msr &precond, double *diag, int p, int thread)
{
  size_t ret = 0;
  if (p == 0)
    ret = precond.copy_template(a);
  reduce_sum(p, &ret, 1);
  if (ret)
    return ret;
  size_t start, stride;
  start_and_size(p, thread, a.n, start, stride);
  double eps = a.norm * EPS;
  if (stride != 0)
  {
    for (size_t i = start; i - start < stride && ret == 0; i++)
    {
      double s = 0;
      size_t l;
      for (l = 0; a.indexes[a.indexes[i]+l] < start; l++);
      for (; a.indexes[a.indexes[i]+l] < i; l++)
      {
        size_t j = a.indexes[a.indexes[i]+l];
        size_t k = bin_search(a.indexes + a.indexes[j], a.indexes[j+1] - a.indexes[j], i);
        if (a.indexes[a.indexes[j] + k] == i)
          s += precond.data[a.indexes[j] + k] * precond.data[a.indexes[j] + k] * diag[j];
      }
      s = a.data[i] - s;
      if (fabs(s) < eps)
        ret = 1;
      precond.data[i] = sqrt(s);
      diag[i] = copysign(1., s);

      for (size_t jc = 0; jc < a.indexes[i+1] - a.indexes[i] && a.indexes[a.indexes[i] + jc] < i; jc++)
      {
        size_t j = a.indexes[a.indexes[i] + jc];
        s = a.data[a.indexes[i] + j];
        size_t lc;
        for (lc = 0; a.indexes[a.indexes[i]+lc] < start; lc++);
        for (; a.indexes[a.indexes[i]+lc] < i; l++)
        {
          size_t l = a.indexes[a.indexes[i]+lc];
          size_t ki = bin_search(a.indexes + a.indexes[l], a.indexes[l+1] - a.indexes[l], i);
          size_t kj = bin_search(a.indexes + a.indexes[l], a.indexes[l+1] - a.indexes[l], j);
          if (a.indexes[a.indexes[l] + ki] == i && a.indexes[a.indexes[l] + kj] == j)
            s -= precond.data[a.indexes[l] + ki] * precond.data[a.indexes[l] + kj] * diag[l];
        }
        precond.data[a.indexes[i] + jc] = s/(precond.data[i]*diag[i]);
      }
    }
  }
  reduce_sum(p, &ret, 1);
  if (ret)
    return 2;
  return 0;
}
