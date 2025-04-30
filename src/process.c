#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/task.h"
#include "../include/utility.h"


int task_compare_by_entry(void *data1, void *data2) {
    task *process1 = (task *)data1;
    task *process2 = (task *)data2;

    if (process1->entry_time < process2->entry_time) {
        return -1;
    } else if (process1->entry_time == process2->entry_time) {
        return 0;
    } else {
        return 1;
    }
}

task_list *produce_tasks(int number_of_processes) {
    task_list *process_list = init_task_list();
    char proc_id = 'A';
    unsigned char urgency;
    float entry_time, duration;
    time_t t;

    srand((unsigned)time(&t));

    if (process_list == NULL) {
        fprintf(stderr, "Unable to create Linked List\n");
    }

    while (number_of_processes--) {
        entry_time = (float)(rand() % 100);
        duration = (float)((rand() % 100) + 1) / 10;
        urgency = (rand() % 4) + 1;
        task *new_process = create_process(proc_id, entry_time, duration, urgency);

        append_task(process_list, new_process);
        proc_id++;
    }

    sort(process_list, task_compare_by_entry);

    task_node *pointer = process_list->head;
    proc_id = 'A';

    while (pointer != NULL) {
        ((task *)pointer->data)->task_id = proc_id;
        if (proc_id == 'Z') {
            proc_id = 'a' - 1;
        }
        proc_id++;
        pointer = pointer->next;
    }

    return process_list;
}

task *create_process(char proc_id, float entry_time, float duration, unsigned char urgency) {
    task *new_process = (task *)malloc(sizeof(task));
    new_process->task_id = proc_id;
    new_process->entry_time = entry_time;
    new_process->duration = duration;
    new_process->urgency = urgency;
    return new_process;
}



