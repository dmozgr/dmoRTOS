#include "circularlinkedlist.h"

struct circularlinkedlist_t{
	struct node_t *head;
};

struct node_t{
	void *data;
	struct node_t* next;
};

uint8_t circularlinkedlist_size(circularlinkedlist_t *list)
{
	uint8_t count = 0;
	node_t *temp = list->head;
	
  if (temp != NULL) {
		do {
				temp = temp->next;
				count++;
		} while (temp != list->head);
  }
	
	return count;
}

circularlinkedlist_t* circularlinkedlist_new()
{
	circularlinkedlist_t *instance = (circularlinkedlist_t*)malloc(sizeof(circularlinkedlist_t));
	return instance;
}

void circularlinkedlist_insert(circularlinkedlist_t *list, void *data)
{
	node_t *new_node = (node_t*)malloc(sizeof(node_t));
	new_node->data = data;
	new_node->next = NULL;
	
	node_t *current = list->head;
	
	if( list->head == NULL)
	{
		new_node->next = new_node;
		list->head = new_node;
	}
	else
	{
		while(current->next != list->head)
			current = current->next;
		current->next = new_node;
		new_node->next = list->head;
		list->head = new_node;
	}
}

void* circularlinkedlist_get_by_index(circularlinkedlist_t *list, uint8_t index)
{
	if( index >= circularlinkedlist_size(list))
		return NULL;
	
	node_t *temp = list->head;
	uint8_t count = 0;
	
  if (temp != NULL) {
		do {
				if( count == index)
					return temp->data;
				temp = temp->next;
				count++;
		} while (temp != list->head);
  }
	
	return NULL;
}

