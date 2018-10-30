#include "protected_buffer.h"

typedef unsigned long ulong;
typedef short bool;
typedef struct {
  int    id;
  long   wcet;
  long   nDeps;
  protected_buffer_t * deps;
} node_t;
  

extern long      nb_nodes;
extern long      core_pool_size;
extern long      max_pool_size;
extern long      blocking_queue_size;
extern long      keep_alive_time;
extern long      period;
extern node_t  * nodes;
extern bool   ** deps;

void readFile (char * filename);
