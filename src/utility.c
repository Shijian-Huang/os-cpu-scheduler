#include <stdio.h>
#include <stdlib.h>
#include "../include/utility.h"
#include "../include/task.h"
#include "../include/stat.h"



task_list *init_task_list() {
	task_list *new_ll =  (task_list *) malloc(sizeof(task_list));
	new_ll->head = NULL;
	new_ll->tail = NULL;
	new_ll->size = 0;
	return new_ll;
}

task_node *init_task_node(void *data) {
	task_node *new_node = (task_node *) malloc(sizeof(task_node));
	new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

void append_task(task_list *ll, void *data) {
	task_node *new_node = init_task_node(data);
	if(ll->size == 0) {
		ll->head = new_node;
		ll->tail = new_node;
		ll->size = 1 ;
	} else {
		new_node->prev = ll->tail;
		ll->tail->next = new_node;
		ll->tail = new_node;
		ll->size += 1;
	}
}

void delete_task(task_list *ll, void *data) {
	task_node *curr_node = ll->head;

	while(curr_node != NULL && curr_node->data != data) {
		curr_node = curr_node->next;
	}

	if(curr_node != NULL) {
		if(curr_node->prev != NULL) {
			curr_node->prev->next = curr_node->next;
		}
		if(curr_node->next != NULL) {
			curr_node->next->prev = curr_node->prev;
		}
		if(ll->head == curr_node) {
			ll->head = curr_node->next;
		}
		if(ll->tail == curr_node) {
			ll->tail = curr_node->prev;
		}
		ll->size --;
		free(curr_node);
	}
}

void remove_head(task_list *ll) {
	task_node *curr_node = ll->head;
	if(curr_node != NULL) {
		ll->head = curr_node->next;
		if(ll->tail == curr_node) {
			ll->tail = curr_node->prev;
		}
		ll->size --;
		free(curr_node);
	}
}

void add_after(task_list *ll, task_node *after_node, void *data) {
	task_node *new_node = init_task_node(data);

	task_node *next_node = after_node->next;
	new_node->next = next_node;
	if(next_node != NULL) next_node->prev = new_node;

	new_node->prev = after_node;
	after_node->next = new_node;

	if(ll->tail == after_node) {
		ll->tail = new_node;
	}

	ll->size++;
}

void sort(task_list *ll, int (*cmp)(void *data1, void *data2)) {
	task_node *i = ll->head;
	while(i!=NULL) {
		task_node *j = i->next;
		while(j!=NULL) {
			void *p1 = i->data;
			void *p2 = j->data;
			if((*cmp)(p1,p2) > 0) {
				swap_nodes(i,j);
			}
			j=j->next;
		}
		i = i->next;
	}
}

void swap_nodes(task_node *a, task_node *b) {
	void *temp = a->data;
	a->data = b->data;
	b->data = temp;
}

task_queue *create_queue() {
	return init_task_list();
}

void enqueue(task_queue *q, void *data) {
	task_node *new_node = init_task_node(data);

	new_node->prev = q->tail;
	if(q->tail != NULL) {
		q->tail->next = new_node;
		q->tail = new_node;
	} else {
		q->tail = new_node;
		q->head = new_node;
	}
	q->size += 1;
}

void *dequeue(task_queue *q) {
	if(q->head != NULL) {
		task_node *curr_node = q->head;
		void *data = curr_node->data;

		task_node *next_node = q->head->next;

		if(next_node != NULL) next_node->prev = NULL;
		q->head = next_node;

		if(q->tail == curr_node) {
			q->tail = NULL;
		}

		q->size--;
		free(curr_node);
		return data;
	}
	return NULL;
}

void display_queue(FILE *log, task_queue *task_queue) {
    task_node *ptr = task_queue->head;
    fprintf(log, "<");
    while (ptr != NULL) {
        fprintf(log, "%c ", ((task *)ptr->data)->task_id);
        ptr = ptr->next;
    }
    fprintf(log, ">\n");
}

metrics_summary print_policy_stat(task_list *ll) {
	metrics_summary avg;
	printf("\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("Process Name\t| Arrival Time | Start Time | End Time | Run Time | Response Time | Wait Time | Turn Around Time | Priority |\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	task_node *ptr = ll->head;
	float avg_response_time = 0;
	float avg_wait_time = 0;
	float avg_turnaround = 0;
	int process_count = 0;
	while(ptr!=NULL) {
		task_metrics *stat = (task_metrics *)ptr->data;
		if(stat == NULL) printf("No Stat\n");
		task *proc = (task *)stat->proc;
		if(proc == NULL) printf("No Process\n");
		float entry_time = proc->entry_time;
		float duration = proc->duration;
		float start_response = stat->initial_time - entry_time;
		float turnaround = stat->finish_time - proc->entry_time + 1;
		float idle_time = turnaround - duration;
		unsigned char urgency = proc->urgency;
		avg_response_time += start_response;
		avg_wait_time += idle_time;
		avg_turnaround += turnaround;
		process_count++;
		printf("%16c|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|%10u|\n",proc->task_id,entry_time, stat->initial_time, stat->finish_time, duration, start_response, idle_time, turnaround,urgency);
		ptr = ptr->next;
	}
	avg.avg_throughput = process_count;
	if(process_count == 0) process_count = 1;
	avg_response_time = avg_response_time / process_count;
	avg_wait_time = avg_wait_time / process_count;
	avg_turnaround = avg_turnaround / process_count;
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	printf("%16s|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f| %17.1f|\n"," Average",0.0, 0.0, 0.0,0.0,avg_response_time, avg_wait_time, avg_turnaround);
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	avg.avg_response_time = avg_response_time;
	avg.avg_wait_time = avg_wait_time;
	avg.avg_turnaround = avg_turnaround;

	return avg;
}
