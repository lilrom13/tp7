#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "circular_buffer.h"
#include "protected_buffer.h"

// Initialise the protected buffer structure above. 
protected_buffer_t * cond_protected_buffer_init(int length) {
  protected_buffer_t * b;
  b = (protected_buffer_t *)malloc(sizeof(protected_buffer_t));
  b->buffer = circular_buffer_init(length);

  // Initialize the synchronization components
  // My implementation
  // should add return code verification
  pthread_mutex_init(&b->mutex, NULL);
  pthread_cond_init(&b->cCv, NULL);
  pthread_cond_init(&b->pCv, NULL);

  return b;
}

// Extract an element from buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void * cond_protected_buffer_get(protected_buffer_t * b){
  void * d;

  // Enter mutual exclusion
  pthread_mutex_lock(&b->mutex);

  // Wait until there is a full slot to get data from the unprotected
  // circular buffer (circular_buffer_get).
  while (b->buffer->size == 0)
    pthread_cond_wait(&b->cCv, &b->mutex);
  d = circular_buffer_get(b->buffer);

  // Signal or broadcast that an empty slot is available in the
  // unprotected circular buffer (if needed)
  pthread_cond_signal(&b->pCv);

  // Leave mutual exclusion
  pthread_mutex_unlock(&b->mutex);

  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void cond_protected_buffer_put(protected_buffer_t * b, void * d){

  // Enter mutual exclusion
  pthread_mutex_lock(&b->mutex);

  // Wait until there is an empty slot to put data in the unprotected
  // circular buffer (circular_buffer_put).
  while (b->buffer->size == b->buffer->max_size)
    pthread_cond_wait(&b->pCv, &b->mutex);
  circular_buffer_put(b->buffer, d);

  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed)
  pthread_cond_signal(&b->cCv);

  // Leave mutual exclusion
  pthread_mutex_unlock(&b->mutex);
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, return NULL. Otherwise, return the element.
void * cond_protected_buffer_remove(protected_buffer_t * b){
  void * d;

  // Signal or broadcast that an empty slot is available in the
  // unprotected circular buffer (if needed)
  if (b->buffer->size != 0)
  {
    d = circular_buffer_get(b->buffer);
    pthread_cond_signal(&b->pCv);
  }
  else
    return NULL;

  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, return 0. Otherwise, return 1.
int cond_protected_buffer_add(protected_buffer_t * b, void * d){
  int done;

  // Enter mutual exclusion
  pthread_mutex_lock(&b->mutex);

  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed)
  if (b->buffer->size != b->buffer->max_size)
  {
    done = circular_buffer_put(b->buffer, d);
    pthread_cond_signal(&b->cCv);
  }
  else
    done = 0;

  // Leave mutual exclusion
  pthread_mutex_unlock(&b->mutex);

  return done;
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return the element if
// successful. Otherwise, return NULL.
void * cond_protected_buffer_poll(protected_buffer_t * b, struct timespec *abstime){
  void * d = NULL;
  int    rc = -1;

  // Enter mutual exclusion
  pthread_mutex_lock(&b->mutex);

  // Wait until there is an empty slot to put data in the unprotected
  // circular buffer (circular_buffer_put) but waits no longer than
  // the given timeout.
  if (b->buffer->size == 0)
    rc = pthread_cond_timedwait(&b->cCv, &b->mutex, abstime);

  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed)
  if (rc != ETIMEDOUT) {
    d = circular_buffer_get(b->buffer);
    pthread_cond_signal(&b->pCv);
  }

  // Leave mutual exclusion
  pthread_mutex_unlock(&b->mutex);

  return d;
}

// Insert an element into buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return 0 if not
// successful. Otherwise, return 1.
int cond_protected_buffer_offer(protected_buffer_t * b, void * d, struct timespec * abstime){
  int rc = -1;
  int done = 0;

  // Enter mutual exclusion
  pthread_mutex_lock(&b->mutex);

  // Signal or broadcast that a full slot is available in the
  // unprotected circular buffer (if needed) but waits no longer than
  // the given timeout.
  if (b->buffer->size == b->buffer->max_size)
    rc = pthread_cond_timedwait(&b->pCv, &b->mutex, abstime);

  if (rc != ETIMEDOUT) {
    done = circular_buffer_put(b->buffer, d);
    pthread_cond_signal(&b->cCv);
  }

  // Leave mutual exclusion
  pthread_mutex_unlock(&b->mutex);

  return done;
}
