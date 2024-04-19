#include "msr.h"

int msr::init_gramm_struct(size_t nx, size_t ny, int p, int thread)
{
  n = (nx + 1)*(ny + 1);
  size = n + 1 + 6*(nx - 1)*(ny - 1) + 2*4*(nx - 1) + 2*4*(ny - 1) + 6 + 4;
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
  start_and_size(p, thread, ny, start, stride);

  if (thread == 0)
  {
    indexes[0] = n + 1;
    indexes[indexes[0]]     = 1;
    indexes[indexes[0] + 1] = nx + 1;

    for (size_t i = 1; i < nx; i++)
    {
      indexes[i] = (n + 1) + i*4 - 2;
      indexes[indexes[i]]     = i - 1;
      indexes[indexes[i] + 1] = i + 1;
      indexes[indexes[i] + 2] = i + nx;
      indexes[indexes[i] + 3] = i + nx + 1;
    }

    indexes[nx] = (n + 1) + nx*4 - 2;
    indexes[indexes[nx]]     = nx - 1;
    indexes[indexes[nx] + 1] = 2*nx;
    indexes[indexes[nx] + 2] = 2*nx + 1;

    start = 1;
    stride--;
  }

  size_t index_step = (6*nx + 2);
  size_t index_offset = n + 1 + (start - 1)*index_step + 4*(nx + 1) - 3;

  if (thread == p-1)
  {
    indexes[n] = size;

    size_t base = n - (nx + 1);
    size_t loc_index_offset = index_offset + index_step*(stride - 1);
    indexes[base] = loc_index_offset;
    indexes[indexes[base]]     = base - (nx + 1);
    indexes[indexes[base] + 1] = base - nx;
    indexes[indexes[base] + 2] = base + 1;

    for (size_t j = base + 1; j - base < nx; j++)
    {
      indexes[j] = loc_index_offset + (j - base)*4 - 1;
      indexes[indexes[j]]     = j - (nx + 1);
      indexes[indexes[j] + 1] = j - nx;
      indexes[indexes[j] + 2] = j - 1;
      indexes[indexes[j] + 3] = j + 1;
    }
    base = n - 1;
    indexes[base] = loc_index_offset + 4*nx - 1;
    indexes[indexes[base]]     = base - (nx - 1);
    indexes[indexes[base] + 1] = base + 1;
    stride--;

  }

  for (size_t i = start; i < start + stride; i++, index_offset += index_step)
  {
    size_t base = i*(nx + 1);
    indexes[base] = index_offset;
    indexes[indexes[base]]     = base - (nx + 1);
    indexes[indexes[base] + 1] = base - nx;
    indexes[indexes[base] + 2] = base + 1;
    indexes[indexes[base] + 3] = base + nx + 1;

    for (size_t j = base + 1; j - base < nx; j++)
    {
      indexes[j] = index_offset + (j - base)*6 - 2;
      indexes[indexes[j]]     = j - (nx + 1);
      indexes[indexes[j] + 1] = j - nx;
      indexes[indexes[j] + 2] = j - 1;
      indexes[indexes[j] + 3] = j + 1;
      indexes[indexes[j] + 4] = j + nx;
      indexes[indexes[j] + 5] = j + nx + 1;
    }
    base = i*nx + nx - 1;
    indexes[base] = index_offset + stride - 4;
    indexes[indexes[base]]     = base - (nx + 1);
    indexes[indexes[base] + 1] = base - nx;
    indexes[indexes[base] + 2] = base + 1;
    indexes[indexes[base] + 3] = base + nx + 1;
  }

  return 0;
}

void msr::fill_gramm(size_t nx, size_t ny, int p, int thread, double a, double b, double c, double d)
{
  size_t stride, start;
  start_and_size(p, thread, ny, start, stride);

  double hx = (b - a)/nx;
  double hy = (d - c)/ny;
  norm = hx*hy;

  if (thread == 0)
  {
    size_t j = 0;

    data[j]              = hx*hy/12.;
    data[indexes[j]]     = hx*hy/24.;
    data[indexes[j] + 1] = hx*hy/24.;

    for (j++; j < nx; j++)
    {
      data[j]              = hx*hy/4;
      data[indexes[j]]     = hx*hy/24.;
      data[indexes[j] + 1] = hx*hy/24.;
      data[indexes[j] + 2] = hx*hy/12.;
      data[indexes[j] + 3] = hx*hy/12.;
    }

    data[j]              = hx*hy/6.;
    data[indexes[j]]     = hx*hy/24.;
    data[indexes[j] + 1] = hx*hy/12.;
    data[indexes[j] + 2] = hx*hy/24.;

    stride--;
    start = 1;
  }

  if (thread == p - 1)
  {
    size_t j = (nx + 1)*ny;

    data[n] = 0;

    data[j]              = hx*hy/6.;
    data[indexes[j]]     = hx*hy/24.;
    data[indexes[j] + 1] = hx*hy/12.;
    data[indexes[j] + 2] = hx*hy/24.;

    for (j++; j < (nx + 1)*(ny + 1) - 1; j++)
    {
      data[j]              = hx*hy/4;
      data[indexes[j]]     = hx*hy/12.;
      data[indexes[j] + 1] = hx*hy/12.;
      data[indexes[j] + 2] = hx*hy/24.;
      data[indexes[j] + 3] = hx*hy/24.;
    }

    data[j]              = hx*hy/12.;
    data[indexes[j]]     = hx*hy/24.;
    data[indexes[j] + 1] = hx*hy/24.;

    stride--;
  }

  for (size_t i = start; i < start + stride; i++)
  {
    size_t j = i*nx;

    data[j]              = hx*hy/4;
    data[indexes[j]]     = hx*hy/24.;
    data[indexes[j] + 1] = hx*hy/12.;
    data[indexes[j] + 2] = hx*hy/12.;
    data[indexes[j] + 3] = hx*hy/24.;

    for (j++; j < (i + 1)*(nx + 1) - 1; j++)
    {
      data[j]              = hx*hy/2.;
      data[indexes[j]]     = hx*hy/12.;
      data[indexes[j] + 1] = hx*hy/12.;
      data[indexes[j] + 2] = hx*hy/12.;
      data[indexes[j] + 3] = hx*hy/12.;
      data[indexes[j] + 4] = hx*hy/12.;
      data[indexes[j] + 5] = hx*hy/12.;
    }

    data[j]              = hx*hy/4;
    data[indexes[j]]     = hx*hy/24.;
    data[indexes[j] + 1] = hx*hy/12.;
    data[indexes[j] + 2] = hx*hy/12.;
    data[indexes[j] + 3] = hx*hy/24.;
  }

}
