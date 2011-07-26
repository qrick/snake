/*
 *	  queue.h
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

#ifndef QUEUE_HEADER
#define QUEUE_HEADER

struct queue_node
{
	struct queue_node *next;	
	int x;
	int y;
};
 
struct queue
{
	struct queue_node *first;
	struct queue_node *last;
};
 
int enqueue(struct queue *q, const int x, const int y);
 
int dequeue(struct queue *q, int *x, int *y);
 
void init_queue(struct queue *q);
 
int queue_empty_p(const struct queue *q);

#endif
