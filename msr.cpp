#include "msr.h"

int msr::init_gramm(size_t nx, size_t ny, int p, int thread)
{
  n = nx*ny;
  size = n + 6*nx*ny - 4*nx - 4*ny + 3;
  size_t flag = 0;
  if (thread == 0)
  {
    erase();
    data = new double[size];
    indexes = new size_t[size];
    if (!data || !indexes)
      flag = 1;
  }

  reduce_sum(p, &flag, 1);
  if (flag)
    return 1;

  size_t stride, start;
  start_and_size(p, thread, n, start, stride);

  if (thread == 0)
  {
    indexes[0] = n + 1;
    indexes[indexes[0]] = 1;
    indexes[indexes[0] + 1] = nx;

    for (size_t i = 1; i < nx - 1; i++)
    {
      indexes[i] = (n + 1) + i*4 - 2;
      indexes[indexes[i]] = i - 1;
      indexes[indexes[i] + 1] = i + 1;
      indexes[indexes[i] + 2] = i + nx - 1;
      indexes[indexes[i] + 3] = i + nx;
    }

    indexes[nx-1] = (n + 1) + nx*4 - 6;
    indexes[indexes[nx - 1]] = nx - 2;
    indexes[indexes[nx - 1] + 1] = 2*nx - 2;
    indexes[indexes[nx - 1] + 2] = 2*nx - 1;

    start = 1;
    stride--;
  }

  size_t index_step = (6*nx - 4);
  size_t index_offset = n + 1 + (start - 1)*index_step + 4*nx - 3;

  if (thread == p-1)
  {
    size_t base = n - nx;
    index_offset += index_step*(stride - 1);
    indexes[base] = index_offset;
    indexes[indexes[base]]     = base - nx;
    indexes[indexes[base] + 1] = base - nx + 1;
    indexes[indexes[base] + 2] = base + 1;

    for (size_t j = base + 1; j - base < nx - 1; j++)
    {
      indexes[j] = index_offset + (j - base)*4 - 1;
      indexes[indexes[j]]     = j - nx;
      indexes[indexes[j] + 1] = j - nx + 1;
      indexes[indexes[j] + 2] = j - 1;
      indexes[indexes[j] + 3] = j + 1;
    }
    base = n - 1;
    indexes[base] = index_offset + 4*nx - 5;
    indexes[indexes[base]]     = base - nx;
    indexes[indexes[base] + 1] = base + 1;

    index_offset = n + 1 + (start - 1)*index_step + 4*nx - 3;
    stride--;
  }

  for (size_t i = start; i < start + stride; i++, index_offset += index_step)
  {
    size_t base = i*nx;
    indexes[base] = index_offset;
    indexes[indexes[base]]     = base - nx;
    indexes[indexes[base] + 1] = base - nx + 1;
    indexes[indexes[base] + 2] = base + 1;
    indexes[indexes[base] + 3] = base + nx;

    for (size_t j = base + 1; j - base < nx - 1; j++)
    {
      indexes[j] = index_offset + (j - base)*6 - 2;
      indexes[indexes[j]]     = j - nx;
      indexes[indexes[j] + 1] = j - nx + 1;
      indexes[indexes[j] + 2] = j - 1;
      indexes[indexes[j] + 3] = j + 1;
      indexes[indexes[j] + 4] = j + nx - 1;
      indexes[indexes[j] + 5] = j + nx;
    }
    base = i*nx + nx - 1;
    indexes[base] = index_offset + stride - 4;
    indexes[indexes[base]]     = base - nx;
    indexes[indexes[base] + 1] = base - nx + 1;
    indexes[indexes[base] + 2] = base + 1;
    indexes[indexes[base] + 3] = base + nx;
  }

  return 0;
}
