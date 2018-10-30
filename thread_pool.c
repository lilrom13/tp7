#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

// Create a thread pool. This pool must be protected against
// concurrent accesses.
thread_pool_t * thread_pool_init(int core_pool_size, int max_pool_size) {
  thread_pool_t * thread_pool;

  thread_pool = (thread_pool_t *) malloc(sizeof(thread_pool_t));
  thread_pool->core_pool_size = core_pool_size;
  thread_pool->max_pool_size  = max_pool_size;
  thread_pool->size           = 0;
  return thread_pool;
}

// Create a thread. If the number of threads created is not greater
// than core_pool_size, create a new thread. If it is and force is set
// to true, create a new thread. If a thread is created, use run as a
// main procedure and future as run parameter.
int pool_thread_create (thread_pool_t * thread_pool,
			run_func_t      run,
			void          * future,
			int             force) {
  int done = 0;

  // Always create a thread as long as there are less then
  // core_pool_size threads created.
  if (thread_pool->size < thread_pool->core_pool_size) {
  }

  return done;
}

// Decrease thread number and broadcast update.
void pool_thread_remove (thread_pool_t * thread_pool) {
}  

// Wait until thread number equals zero.
void wait_thread_pool_empty (thread_pool_t * thread_pool) {
  sleep (20);
}  

