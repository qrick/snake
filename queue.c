/*
 *	  queue.c
 *	  
 *	  Copyright 2011 gisty <amcour@gmail.com>
 *	  
 *	  This program is free software; you can redistribute it and/or modify
 *	  it under the terms of the GNU General Public License as published by
 *	  the Free Software Foundation; either version 2 of the License, or
 *	  (at your option) any later version.
 *	  
 *	  This program is distributed in the hope that it will be useful,
 *	  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	  GNU General Public License for more details.
 *	  
 *	  You should have received a copy of the GNU General Public License
 *	  along with this program; if not, write to the Free Software
 *	  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *	  MA 02110-1301, USA.
 */

#include <errno.h>
#include <stdlib.h>

#include "queue.h"

int enqueue(struct queue *q, const int x, const int y)
{
	struct queue_node *node = malloc(sizeof(struct queue_node));
	if (node == NULL) {
		errno = ENOMEM;
		return 1;
	}
	node->x = x;
	node->y = y;
	if (q->first == NULL)
		q->first = q->last = node;
	else {
		q->last->next = node;
		q->last = node;
	}
	node->next = NULL;
	return 0;
}
 
int dequeue(struct queue *q, int *x, int *y)
{
	if (!q->first) {
		*x = 0;
		*y = 0;
		return 1;
	}
	*x = q->first->x;
	*y = q->first->y;
	struct queue_node *tmp = q->first;
	if (q->first == q->last)
		q->first = q->last = NULL;
	else
		q->first = q->first->next;
 
	free(tmp);
	return 0;
}
 
void init_queue(struct queue *q)
{
	q->first = q->last = NULL;
}
 
int queue_empty_p(const struct queue *q)
{
	return q->first == NULL;
}
