#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "executor.h"
#include "scenario.h"
#include "utils.h"

callable_t * callables;
future_t ** futures;

void print_log(long time, char * msg, int wcet, int period);

void * main_run (void * arg) {
  node_t * n = (node_t *) arg;
  struct timespec ts1, ts2;
  
  ts1.tv_sec  = n->wcet / 1000;
  ts1.tv_nsec = (n->wcet % 1000) * 1000000;

  print_log (elapsed_time(), "initiate", n->wcet, period);
  nanosleep(&ts1, &ts2);
  print_log (elapsed_time(), "complete", n->wcet, period);
  return NULL;
}

int main(int argc, char *argv[]) {
  int i;

  if (argc != 2) {
    printf("Usage : %s <scenario file>\n", argv[0]);
    exit(1);
  }

  init_utils();

  readFile(argv[1]);

  callables = (callable_t *) malloc(sizeof(callable_t) * nb_nodes);
  futures = (future_t **) malloc(sizeof(future_t *) * nb_nodes);
  
  set_start_time();

  executor_t * executor =
    executor_init
    (core_pool_size,
     max_pool_size,
     keep_alive_time,
     blocking_queue_size);
  void * result;

  for (i = 0; i < nb_nodes; i++) {
    callables[i].params = (void *) &nodes[i];
    callables[i].run    = main_run;
    callables[i].period = period;
    futures[i] = submit_callable (executor, &callables[i]);
    if (futures[i] == NULL)
      printf ("%06ld callable %d not handled\n", elapsed_time(), i);
  }
  for (i = 0; i < nb_nodes; i++) {
    if (futures[i] != NULL) {
      result = get_callable_result (futures[i]);
      printf ("%06ld got result from callable %d\n", elapsed_time(), i);
    }
  }
  
  executor_shutdown(executor);
}

void print_log(long time, char * msg, int wcet, int period) {
    printf("%06ld %s execution=%d period=%d\n",
           time, msg, wcet, period);
}

