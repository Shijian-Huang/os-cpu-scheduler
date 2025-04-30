#include <stdio.h>
#include <stdlib.h>
#include "../include/stat.h"
#include "../include/utility.h"

task_metrics *init_task_metrics(task* proc);

metrics_summary round_robin_p(task_list *processes, int time_slice) {
    int current_time = 0;

    task_queue *process_queue = (task_queue *)create_queue();
    task_node *process_ptr = processes->head;

    if (processes->head == NULL) {
        fprintf(stderr, "No processes to schedule\n");
    }

    task_metrics *scheduled_process = NULL;

    task_list *ll = init_task_list();
    printf("\nRound Robin Algorithm:\n");
    task_node *current_node = NULL;
    int current_run_time = 0;

    while (current_time < 100 || process_queue->size > 0) {
        if (process_ptr != NULL && current_time < 100) {
            task *new_process = (task *)(process_ptr->data);
            while (process_ptr != NULL && new_process->entry_time <= current_time) {
                enqueue(process_queue, init_task_metrics(new_process));
                process_ptr = process_ptr->next;

                if (process_ptr != NULL) {
                    new_process = (task *)(process_ptr->data);
                }
            }
        }

        if (current_node == NULL) {
            current_run_time = 0;
            current_node = process_queue->head;
        } else if (current_run_time == time_slice) {
            current_run_time = 0;
            current_node = current_node->next;

            if (current_node == NULL) {
                current_node = process_queue->head;
            }
        }

        if (current_node != NULL) {
            scheduled_process = (task_metrics *)current_node->data;
            task *proc = scheduled_process->proc;

            if (current_time >= 100) {
                if (scheduled_process->initial_time == -1) {
                    free(scheduled_process);
                    task_node *next = current_node->next;
                    delete_task(process_queue, current_node->data);
                    current_node = next;
                    current_run_time = 0;
                    continue;
                }
            }

            printf("%c", proc->task_id);
            current_run_time++;

            if (scheduled_process->initial_time == -1) {
                scheduled_process->initial_time = current_time;
            }

            scheduled_process->duration++;

            if (scheduled_process->duration >= proc->duration) {
                scheduled_process->finish_time = current_time;
                append_task(ll, scheduled_process);
                task_node *next = current_node->next;
                delete_task(process_queue, current_node->data);
                current_node = next;
                current_run_time = 0;
            }
        } else {
            printf("_");
        }

        current_time++;
    }

    return print_policy_stat(ll);
}
