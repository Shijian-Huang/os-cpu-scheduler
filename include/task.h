#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "utility.h"

struct process_s {
	unsigned char task_id;
	float entry_time;
	float duration;
	unsigned char urgency; 
};

typedef struct process_s task;

task *create_process(char task_id, float entry_time, float duration, unsigned char urgency);

task *duplicate_task(task *proc);

task_list *produce_tasks(int n);

#endif