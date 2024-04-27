#ifndef THREAD_RELATED_H
#define THREAD_RELATED_H

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>

double get_cpu_time();
double get_full_time();
void start_and_size(unsigned int p, unsigned int thread, size_t n, size_t &start, size_t &size);

enum class status
{
  ok,
  error_mem,
  error_data,
};

template <class T>
void reduce_sum(int p, T *a = nullptr, size_t n = 0);

template <class T>
void reduce_max(int p, T *a = nullptr, size_t n = 0);

#endif //THREAD_RELATED_H
