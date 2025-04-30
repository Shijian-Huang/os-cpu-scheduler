#include <stdio.h>
#include <stdlib.h>
#include "../include/stat.h"
#include "../include/utility.h"

task_metrics *init_task_metrics(task *proc) {
    task_metrics *ps = (task_metrics *)malloc(sizeof(task_metrics));
    ps->proc = proc;
    ps->idle_time = 0;
    ps->completion_span = 0;
    ps->start_response = 0;
    ps->duration = 0;
    ps->initial_time = -1;
    ps->finish_time = -1;
    return ps;
}

metrics_summary first_come_first_serve_np(task_list *processes) {
    int current_time = 0;
    task_queue *process_queue = (task_queue *)create_queue();
    task_node *process_node = processes->head;
    
    if (processes->head == NULL) {
        fprintf(stderr, "There are no processes to schedule\n\n");
    }

    task_metrics *scheduled_process = NULL;
    task_list *time_chart = init_task_list();
    printf("\nFirst-Come First-Serve Algorithm:\n");
    
    while (current_time < 100 || scheduled_process != NULL) {
        if (process_node != NULL) {
            task *new_process = (task *)(process_node->data);
            if (new_process->entry_time <= current_time) {
                enqueue(process_queue, init_task_metrics(new_process));
                process_node = process_node->next;
            }
        }

        if (scheduled_process == NULL && process_queue->size > 0) {
            scheduled_process = (task_metrics *)dequeue(process_queue);
        }

        if (scheduled_process != NULL) {
            task *current_process = scheduled_process->proc;
            printf("%c", current_process->task_id);

            if (scheduled_process->initial_time == -1) {
                scheduled_process->initial_time = current_time;
            }
            scheduled_process->duration++;

            if (scheduled_process->duration >= current_process->duration) {
                scheduled_process->finish_time = current_time;
                append_task(time_chart, scheduled_process);
                scheduled_process = NULL;
            }
        } else {
            printf("_");
        }

        current_time++;
    }

    return print_policy_stat(time_chart);
}
