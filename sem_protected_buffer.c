#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "circular_buffer.h"
#include "protected_buffer.h"
#include "utils.h"

// Initialise the protected buffer structure above. 
protected_buffer_t * sem_protected_buffer_init(int length) {
  protected_buffer_t * b;
  b = (protected_buffer_t *)malloc(sizeof(protected_buffer_t));
  b->buffer = circular_buffer_init(length);
  // Initialize the synchronization attributes
  // Use these filenames as named semaphores
  sem_unlink ("/tmp/empty_slots");
  sem_unlink ("/tmp/full_slots");
  return b;
}

// Extract an element from buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void * sem_protected_buffer_get(protected_buffer_t * b){
  void * d;
  
  // Enter mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  
  d = circular_buffer_get(b->buffer);

  // Leave mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  
  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void sem_protected_buffer_put(protected_buffer_t * b, void * d){

  // Enter mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  
  circular_buffer_put(b->buffer, d);

  // Leave mutual exclusion and enforce synchronisation semantics
  // using semaphores.
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, return NULL. Otherwise, return the element.
void * sem_protected_buffer_remove(protected_buffer_t * b){
  void * d;
  
  // Enter mutual exclusion and enforce synchronisation semantics
  // using semaphores.

  d = circular_buffer_get(b->buffer);

  // Leave mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, return 0. Otherwise, return 1.
int sem_protected_buffer_add(protected_buffer_t * b, void * d){
  int done;
  
  // Leave mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  
  done = circular_buffer_put(b->buffer, d);
  
  // Leave mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  return 1;
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return the element if
// successful. Otherwise, return NULL.
void * sem_protected_buffer_poll(protected_buffer_t * b, struct timespec *abstime){
  void * d = NULL;
  
  // Enter mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  
  d = circular_buffer_get(b->buffer);

  // Leave mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  return d;
}

// Insert an element into buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return 0 if not
// successful. Otherwise, return 1.
int sem_protected_buffer_offer(protected_buffer_t * b, void * d, struct timespec * abstime){
  
  // Enter mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  
  circular_buffer_put(b->buffer, d);

  // Leave mutual exclusion and enforce synchronisation semantics
  // using semaphores.
  return 1;
}

