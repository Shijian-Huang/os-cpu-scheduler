#include <stdio.h>
#include <stdlib.h>
#include "../include/stat.h"
#include "../include/utility.h"

task_metrics *init_task_metrics(task *proc);

int compare_remaining_time(void *data1, void *data2) {
    task_metrics *ps1 = (task_metrics *)data1;
    task_metrics *ps2 = (task_metrics *)data2;
    
    float remaining_time1 = ((task *)ps1->proc)->duration - ps1->duration;
    float remaining_time2 = ((task *)ps2->proc)->duration - ps2->duration;

    if (remaining_time1 < remaining_time2) {
        return -1;
    } else {
        return 1;
    }
}

void print_queue_srt(task_queue *q) {
    if (q->head != NULL) {
        task_node *n = q->head;
        printf("Queue Contains:\n");

        while (n != NULL) {
            task_metrics *ps = n->data;
            task *p = ps->proc;
            float remaining_time = p->duration - ps->duration;
            printf("Process Id %c Remaining Time %.1f\n", p->task_id, remaining_time);
            n = n->next;
        }
    }
    return;
}

metrics_summary shortest_remaining_time_p(task_list *processes) {
    int quanta = 0;

    task_queue *process_queue = create_queue();
    task_list *ll = init_task_list();

    task_node *process_pointer = processes->head;
    
    if (processes->head == NULL) {
        fprintf(stderr, "No processes to schedule\n");
    }

    task_metrics *scheduled_process = NULL;
    printf("\nShortest Remaining Time Algorithm:\n");

    while (quanta < 100 || scheduled_process != NULL) {
        if (scheduled_process != NULL) {
            enqueue(process_queue, scheduled_process);
            scheduled_process = NULL;
        }

        if (process_pointer != NULL) {
            task *new_process = (task *)(process_pointer->data);
            while (process_pointer != NULL && new_process->entry_time <= quanta) {
                enqueue(process_queue, init_task_metrics(new_process));
                sort(process_queue, compare_remaining_time);
                process_pointer = process_pointer->next;

                if (process_pointer != NULL) {
                    new_process = (task *)(process_pointer->data);
                }
            }
        }

        if (scheduled_process == NULL && process_queue->size > 0) {
            scheduled_process = (task_metrics *)dequeue(process_queue);

            while (quanta >= 100 && scheduled_process->initial_time == -1) {
                scheduled_process = (task_metrics *)dequeue(process_queue);
            }
        }

        if (scheduled_process != NULL) {
            task *proc = scheduled_process->proc;

            printf("%c", proc->task_id);

            if (scheduled_process->initial_time == -1) {
                scheduled_process->initial_time = quanta;
            }

            scheduled_process->duration++;

            if (scheduled_process->duration >= proc->duration) {
                scheduled_process->finish_time = quanta;
                append_task(ll, scheduled_process);
                scheduled_process = NULL;
            }
        } else {
            printf("_");
        }

        quanta++;
    }

    return print_policy_stat(ll);
}
