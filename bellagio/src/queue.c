/**
  src/queue.c

  Implements a simple FIFO structure used for queueing OMX buffers.

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "omx_comp_debug_levels.h"

/** Initialize a queue descriptor
 *
 * @param queue The queue descriptor to initialize.
 * The user needs to allocate the queue
 */
int queue_init(queue_t* queue) {
  int i;
  qelem_t* newelem;
  qelem_t* current;
  i = pthread_mutex_init(&queue->mutex, NULL);
  if (i!=0) {
	  return -1;
  }
  queue->first = malloc(sizeof(qelem_t));
  if (!(queue->first)) {
	  return -1;
  }
  memset(queue->first, 0, sizeof(qelem_t));
  current = queue->last = queue->first;
  queue->nelem = 0;
  for (i = 0; i<MAX_QUEUE_ELEMENTS - 2; i++) {
    newelem = malloc(sizeof(qelem_t));
    if (!newelem) {
    	// the memory is not enough. Free all
    	while(queue->first!=NULL) {
        	current = queue->first->q_forw;
        	free(queue->first);
        	queue->first = current;
    	}
    	return -1;
    }
    memset(newelem, 0, sizeof(qelem_t));
    current->q_forw = newelem;
    current = newelem;
  }
  current->q_forw = queue->first;
  return 0;
}

/** Deinitialize a queue descriptor
 * flushing all of its internal data
 *
 * @param queue the queue descriptor to dump
 */
void queue_deinit(queue_t* queue) {
  int i;
  qelem_t* current;
  current = queue->first;
  for (i = 0; i<MAX_QUEUE_ELEMENTS - 2; i++) {
    if (current != NULL) {
      current = current->q_forw;
      free(queue->first);
      queue->first = current;
    }
  }
  if(queue->first) {
    free(queue->first);
    queue->first = NULL;
  }
  pthread_mutex_destroy(&queue->mutex);
}

/** Enqueue an element to the given queue descriptor
 *
 * @param queue the queue descriptor where to queue data
 *
 * @param data the data to be enqueued
 *
 * @return -1 if the queue is full
 */
int queue(queue_t* queue, void* data) {
  if (queue->last->data != NULL) {
    return -1;
  }
  pthread_mutex_lock(&queue->mutex);
  queue->last->data = data;
  queue->last = queue->last->q_forw;
  queue->nelem++;
  pthread_mutex_unlock(&queue->mutex);
  return 0;
}

/** Dequeue an element from the given queue descriptor
 *
 * @param queue the queue descriptor from which to dequeue the element
 *
 * @return the element that has bee dequeued. If the queue is empty
 *  a NULL value is returned
 */
void* dequeue(queue_t* queue) {
  void* data;
  if (queue->first->data == NULL) {
    return NULL;
  }
  pthread_mutex_lock(&queue->mutex);
  data = queue->first->data;
  queue->first->data = NULL;
  queue->first = queue->first->q_forw;
  queue->nelem--;
  pthread_mutex_unlock(&queue->mutex);

  return data;
}

/** Returns the number of elements hold in the queue
 *
 * @param queue the requested queue
 *
 * @return the number of elements in the queue
 */
int getquenelem(queue_t* queue) {
  int qelem;
  pthread_mutex_lock(&queue->mutex);
  qelem = queue->nelem;
  pthread_mutex_unlock(&queue->mutex);
  return qelem;
}
