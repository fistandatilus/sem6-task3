#include "msr.h"

#ifndef APPROXIMATION_H
#define APPROXIMATION_H

class approximation
{
  private:
    msr gramm;
    msr precond;
    size_t *msr_template = nullptr;
    double *coeffs = nullptr;
    double (*f)(double, double);
    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;
    size_t nx = 0;
    size_t ny = 0;

  public:
    double operator()(double x, double y);
    double residual1(int p, int thread);
    double residual2(int p, int thread);
    double residual3(int p, int thread);
    double residual4(int p, int thread);
};

#endif
