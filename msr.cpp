#include "msr.h"

int msr::copy(msr &x)
{
  erase();
  n = x.n;
  size = x.size;
  norm = x.norm;
  if (x.data != nullptr)
  {
    data = new double[size];
    if (!data)
      return 1;
    memcpy(data, x.data, size * sizeof(double));
  }
  if (x.indexes != nullptr)
  {
    indexes = new size_t[size];
    if (!indexes)
      return 1;
    memcpy(indexes, x.indexes, size * sizeof(size_t));
  }
  return 0;
}


int msr::copy_template(msr &x)
{
  erase();
  n = x.n;
  size = x.size;
  if (x.data != nullptr)
  {
    data = new double[size];
    if (!data)
      return 1;
  }
  if (x.indexes != nullptr)
  {
    indexes = new size_t[size];
    if (!indexes)
      return 1;
    memcpy(indexes, x.indexes, size * sizeof(size_t));
  }
  return 0;
}
