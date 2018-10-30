#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "scenario.h"

long      nb_nodes;
long      core_pool_size;
long      max_pool_size;
long      blocking_queue_size;
long      keep_alive_time;
long      period;
node_t  * nodes;
bool   ** deps;

int getString (FILE * f, char * s, char * file, int line) {
  char b[64];
  char * c;;
  
  while (fgets (b, 64, f) != NULL) {
    c = strchr (b, '\n');
    *c = '\0';
    if (strcmp (s, b) == 0)
      return 1;
  }
  printf ("getString failed to catch %s in %s:%d\n", s, file, line);
  exit (1);
  return 0;
}

int getLong (FILE * f, long * l, char * file, int line) {
  char b[64];
  char * c;;
  
  if (fgets (b, 64, f) != NULL) {
    c = strchr (b, '\n');
    *c = '\0';
    *l = strtol (b, NULL, 10);
    if ((*l != 0) || (errno != EINVAL))
      return 1;
  }
  return 0;
}

void readFile (char * filename) {
  FILE * file;
  ulong i, j;

  file = fopen (filename, "r");
  
  getString (file, "#core_pool_size", __FILE__, __LINE__);
  getLong   (file, (long *) &core_pool_size, __FILE__, __LINE__);
  printf ("core_pool_size = %ld\n", core_pool_size);

  getString (file, "#max_pool_size", __FILE__, __LINE__);
  getLong   (file, (long *) &max_pool_size, __FILE__, __LINE__);
  printf ("max_pool_size = %ld\n", max_pool_size);

  getString (file, "#blocking_queue_size", __FILE__, __LINE__);
  getLong   (file, (long *) &blocking_queue_size, __FILE__, __LINE__);
  printf ("blocking_queue_size = %ld\n", blocking_queue_size);

  getString (file, "#keep_alive_time", __FILE__, __LINE__);
  getLong   (file, (long *) &keep_alive_time, __FILE__, __LINE__);

  getString (file, "#period", __FILE__, __LINE__);
  getLong   (file, (long *) &period, __FILE__, __LINE__);

  getString (file, "#nb_nodes", __FILE__, __LINE__);
  getLong   (file, (long *) &nb_nodes, __FILE__, __LINE__);

  getString (file, "#wcet", __FILE__, __LINE__);
  nodes = (node_t *) malloc ((ulong) (nb_nodes) * sizeof(node_t));
  for (i = 0; i < nb_nodes; i++) {
    getLong (file, (long *) &nodes[i].wcet, __FILE__, __LINE__);
    // printf ("wcet[%lu]=%ld\n", i, nodes[i].wcet);
  }

  getString (file, "#preds", __FILE__, __LINE__);
  deps = (bool **) malloc ((ulong) (nb_nodes) * sizeof(bool *));
  for (i = 0; i < nb_nodes; i++)
    deps[i] = (bool *) malloc ((ulong) (nb_nodes) * sizeof(bool));
  for (i = 0; i < nb_nodes; i++) {
    for (j = 0; j < nb_nodes; j++) {
      fscanf (file, "%hd", &deps[i][j]);
      // printf ("deps[%lu, %lu]=%hd\n", i, j, deps[i][j]);
      if (j != nb_nodes - 1) fscanf(file, ",");
    }
  }

  for (i = 0; i < nb_nodes; i++) {
    nodes[i].id = i;
    nodes[i].nDeps = 0;
    for (j = 0; j < nb_nodes; j++) if (deps[i][j] != 0) nodes[i].nDeps++;
    if (nodes[i].nDeps == 0)
      nodes[i].deps = NULL;
    else
      nodes[i].deps = protected_buffer_init (0, nodes[i].nDeps);
  }
}
