#ifndef __OS_CIRCLINKEDLIST_H__
#define __OS_CIRCLINKEDLIST_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct circularlinkedlist_t circularlinkedlist_t;
typedef struct node_t node_t;

uint8_t circularlinkedlist_size(circularlinkedlist_t *list);
circularlinkedlist_t* circularlinkedlist_new();
void circularlinkedlist_insert(circularlinkedlist_t *list, void *data);
	
#endif