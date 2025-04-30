#include <stdio.h>
#include <stdlib.h>
#include "../include/stat.h"
#include "../include/utility.h"

task_metrics *init_task_metrics(task *proc);

int compare_run_time(void *data1, void *data2) {
    task_metrics *ps1 = (task_metrics *)data1;
    task_metrics *ps2 = (task_metrics *)data2;
    if (((task *)ps1->proc)->duration < ((task *)ps2->proc)->duration) {
        return -1;
    } else {
        return 1;
    }
}

metrics_summary shortest_job_first_np(task_list *processes) {
    int time_counter = 0;

    task_queue *process_order = (task_queue *)create_queue();
    task_node *process_node = processes->head;

    if (processes->head == NULL) {
        fprintf(stderr, "No processes to schedule\n");
    }

    task_metrics *active_process = NULL;

    task_list *completed_processes = init_task_list();
    printf("\nShortest Job First Algorithm:\n");

    while (time_counter < 100 || active_process != NULL) {
        if (process_node != NULL) {
            task *incoming_process = (task *)(process_node->data);
            while (process_node != NULL && incoming_process->entry_time <= time_counter) {
                enqueue(process_order, init_task_metrics(incoming_process));
                sort(process_order, compare_run_time);
                process_node = process_node->next;

                if (process_node != NULL) {
                    incoming_process = (task *)(process_node->data);
                }
            }
        }

        if (active_process == NULL && process_order->size > 0) {
            active_process = (task_metrics *)dequeue(process_order);
        }

        if (active_process != NULL) {
            task *executing_process = active_process->proc;

            printf("%c", executing_process->task_id);

            if (active_process->initial_time == -1) {
                active_process->initial_time = time_counter;
            }

            active_process->duration++;

            if (active_process->duration >= executing_process->duration) {
                active_process->finish_time = time_counter;
                append_task(completed_processes, active_process);
                active_process = NULL;
            }
        } else {
            printf("_");
        }

        time_counter++;
    }

    printf("\n");

    return print_policy_stat(completed_processes);
}
