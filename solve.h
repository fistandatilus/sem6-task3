#ifndef SOLVE_H
#define SOLVE_H

#include "msr.h"

void inv_m_mul_vec(msr &m, double *d, double *r, double *v, size_t start, size_t stride);
int solve(msr &a, double *b, msr &m, double *d, double *x, size_t n, double *r, double *u, double *v, int p, int thread, int max_it);
void mul_msr_by_vec(msr &a, double *x, double *ret, size_t start, size_t stride);
double dot_prod(const double *u, const double *v, size_t start, size_t stride);

#endif
