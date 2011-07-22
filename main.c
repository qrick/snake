/*
 *	  main.c
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

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#define SIZEX 20
#define SIZEY 50

static struct termios stored_settings;

void set_keypress(void)
{
	struct termios new_settings;

	tcgetattr(0,&stored_settings);

	new_settings = stored_settings;

	/* Disable canonical mode, and set buffer size to 1 byte */
	new_settings.c_lflag &= ( ICANON | ECHO );
	new_settings.c_cc[VTIME] = 0;
	new_settings.c_cc[VMIN] = 1;

	tcsetattr(0,TCSANOW,&new_settings);
	return;
}

void reset_keypress(void)
{
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}

void printLine(int length)
{	
	putchar('+');
	for(int i = 0; i <= length * 2 + 2; i++)
		putchar('-');
	putchar('+');
	putchar('\n');
}

void clrScr(void)
{
	for (int i = 0; i < 30; i++)
		putchar('\n');
}

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

int main(int argc, char **argv)
{
	int ground[SIZEY][SIZEX];
	int prevdir = 0, dir = 0;
	int curX, curY, tmpX, tmpY;
	int notkill = 1;
	int count = 0;
	
	char key;
	
	struct queue Q;
	init_queue(&Q);
	
	fd_set rfds;
	struct timeval tv;
	int retval;
	/* Watch stdin (fd 0) to see when it has input. */
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	tv.tv_sec = 0;
	tv.tv_usec = 50000;	

	set_keypress();

	for (int y = 0; i <= SIZEY - 1; i++)
	for (int x = 0; j <= SIZEX - 1; j++)
		ground[y][x] = 0;

	srand(time(0));
	
	curX = random() % SIZEX;
	curY = random() % SIZEY;
	enqueue(&Q, curX, curY);
	ground[curY][curX] = 1;
	
	while(notkill)
	{		
		clrScr();
		printf(" * W - up\n * A - left\n * D - right\n * S - down\n"
				" * Q - exit\n");
		printLine(SIZEY - 1);
		for (int i = 0; i <= SIZEY - 1; i++)
		{
			putchar('|');
			putchar(' ');
			for (int j = 0; j <= SIZEX - 1; j++)
			{
				if( ground[i][j] == 0)
					putchar(' ');
				else if(i == curY && j == curX)
					putchar('@');
				else
					putchar('#');
				putchar(' ');
			}
			putchar('|');
			putchar('\n');
		}
		printLine(SIZEY - 1);
		
		retval = select(1, &rfds, NULL, NULL, &tv);
		if(retval)
			key = getc(stdin);
		/*
		 * 0 - left
		 * 1 - up
		 * 2 - right
		 * 3 - down
		 */
		
		prevdir = dir;
		
		switch(key)
		{
			case 'w': dir = 1;  break;
			case 'a': dir = 0;  break;
			case 's': dir = 3;  break;
			case 'd': dir = 2;  break;
			case 'q': notkill = 0; break;
		}
		
		if(abs(prevdir - dir) == 2)
			dir = prevdir;
		if(dir == 0 && (curX - 1) >= 0)
			curX--;
		else if(dir == 1 && (curY - 1) >= 0)
			curY--;
		else if(dir == 2 && (curX + 1) <= SIZEX - 1)
			curX++;
		else if(dir == 3 && (curY + 1) <= SIZEY - 1)
			curY++;
		else
			notkill = 0;
		
		if(count == 2)
		{
			dequeue(&Q, &tmpX, &tmpY);
			ground[tmpY][tmpX] = 0;
			count = 0;
		}
		
		if(ground[curY][curX] == 1)
		{
			puts("CRASH!");
			notkill = 0;
			queue_empty_p(&Q);
		}
		enqueue(&Q, curX, curY);
		ground[curY][curX] = 1;
		count++;
		usleep(50000);
	}
	reset_keypress();
	return 0;
}
