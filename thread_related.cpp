#include "thread_related.h"

double get_cpu_time()
{
  rusage buf;
  getrusage(RUSAGE_THREAD, &buf);
  return buf.ru_utime.tv_sec + buf.ru_utime.tv_usec*1e-6;
}

double get_full_time()
{
  timeval buf;
  gettimeofday(&buf, 0);
  return buf.tv_sec + buf.tv_usec / 1e6;
}

void start_and_size(unsigned int p, unsigned int thread, size_t n, size_t &start, size_t &size)
{
  size = n/p;
  size_t res = n - size*p;
  start = size*thread + (thread < res ? thread : res);
  if (thread < res) size++;

}

template <class T>
void reduce_sum(int p, T *a, size_t n)
{
  static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  static pthread_cond_t condvar_in = PTHREAD_COND_INITIALIZER;
  static pthread_cond_t condvar_out = PTHREAD_COND_INITIALIZER;
  static int threads_in = 0, threads_out = 0;
  static T *pres = nullptr;
  pthread_mutex_lock(&mutex);
  if (pres == nullptr)
  {
    pres = a;
  }
  else
  {
    for (size_t i = 0; i < n; i++)
    {
      pres[i] += a[i];
    }
  }
  threads_in++;
  if (threads_in >= p)
  {
    threads_out = 0;
    pthread_cond_broadcast(&condvar_in);
  }
  else
  {
    while (threads_in < p)
      pthread_cond_wait(&condvar_in, &mutex);
  }
  if (pres != a)
  {
    for (size_t i = 0; i < n; i++)
      a[i] = pres[i];
  }
  threads_out++;
  if (threads_out >= p)
  {
    pres = nullptr;
    threads_in = 0;
    pthread_cond_broadcast(&condvar_out);
  }
  else
    while (threads_out < p)
      pthread_cond_wait(&condvar_out, &mutex);
  pthread_mutex_unlock(&mutex);
}
