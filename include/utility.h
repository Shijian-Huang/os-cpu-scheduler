#ifndef _utility_h_
#define _utility_h_

#include <stdio.h>

struct node_s {
	struct node_s *next;
	struct node_s *prev;
	void *data;
};

typedef struct node_s task_node;

struct linked_list_s {
	task_node *head;
	task_node *tail;
	int size;
};

typedef struct linked_list_s task_list;

task_node *init_task_node(void *data);
task_list *init_task_list();
void append_task(task_list *ll, void *data);
void delete_task(task_list *ll, void *data);
void swap_nodes(task_node *a, task_node *b);
void add_after(task_list *ll, task_node *after_node, void *data);
void sort(task_list *ll, int (*cmp)(void *data1, void *data2));

// Queue Implementation

typedef struct linked_list_s task_queue;

task_queue *create_queue();
void enqueue(task_queue *q, void *data);
void *dequeue(task_queue *q);

void display_queue(FILE *log, task_queue *task_queue);

#endif