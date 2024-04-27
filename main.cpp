#include <pthread.h>

#include "approximation.h"

void *thread_func(void *void_arg);

struct arguments
{
  approximation *approx;
  double a;
  double b;
  double c;
  double d;
  double eps;
  size_t nx;
  size_t ny;
  int max_it;

  double r1;
  double r2;
  double r3;
  double r4;
  double t1;
  double t2;
  int it;
  status stat;

  int p;
  int thread;
};

int main(int argc, char *argv[])
{
  int p, it, max_it, k, task = 8;
  double r1, r2, r3, r4, t1, t2, eps, a, b, c, d;
  size_t nx, ny;

  if (!(argc == 11 && sscanf(argv[1], "%lf", &a) != 1 && sscanf(argv[2], "%lf", &b) != 1 && b >= a && sscanf(argv[3], "%lf", &c) != 1 && sscanf(argv[4], "%lf", &d) != 1 && c >= d
        && sscanf(argv[5], "%ld", &nx) != 1 && nx > 0 && sscanf(argv[6], "%ld", &ny) != 1 && ny > 0 &&sscanf(argv[7], "%d", &k) != 1 && k >= 0 && k <= 8
        && sscanf(argv[8], "%lf", &eps) != 1 && eps > 0 && sscanf(argv[9], "%d", &max_it) != 1 && max_it >= 1 && sscanf(argv[10], "%d", &p) && p >= 1))
  {
    printf("Program usage: %s a b c d nx ny k eps max_it p\na b c d - ends of segments\nnx ny - amounts of segments subdivisions\nk - function to approximate\neps - desired precision\nmax_it - maximum number of iterations\np - amount of threads to use\n", argv[0]);
    return -1;
  }

  pthread_t *tid = new pthread_t[p];
  arguments *arg = new arguments[p];
  approximation approx;

  return 0;
}

void *thread_func(void *void_arg)
{
  arguments *arg = (arguments *)void_arg;

  approximation *approx = arg->approx;
  double a = arg->a;
  double b = arg->b;
  double c = arg->c;
  double d = arg->d;
  double eps = arg->eps;
  size_t nx = arg->nx;
  size_t ny = arg->ny;
  int max_it = arg->max_it;

  int p = arg->p;
  int thread = arg->thread;


}
