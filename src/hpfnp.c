#include <stdio.h>
#include <stdlib.h>
#include "../include/stat.h"
#include "../include/utility.h"

task_metrics *init_task_metrics(task *proc);
int compare_priority(void *data1, void *data2);
void print_queue(task_queue *q);

metrics_summary highest_priority_first_np(task_list *processes) {
    int current_time = 0;

    task_queue *high_priority_queue1 = create_queue();
    task_queue *high_priority_queue2 = create_queue();
    task_queue *high_priority_queue3 = create_queue();
    task_queue *high_priority_queue4 = create_queue();

    task_list *priority_list1 = init_task_list();
    task_list *priority_list2 = init_task_list();
    task_list *priority_list3 = init_task_list();
    task_list *priority_list4 = init_task_list();

    task_node *current_process_node = processes->head;

    if (processes->head == NULL) {
        fprintf(stderr, "No processes available for scheduling\n");
    }

    printf("\nExecuting Highest Priority First Non-preemptive Algorithm:\n");

    task_metrics *active_process = NULL;

    while (current_time < 100 || active_process != NULL) {
        if (current_process_node != NULL) {
            task *incoming_process = (task *)(current_process_node->data);

            while (current_process_node != NULL && incoming_process->entry_time <= current_time) {
                if (incoming_process->urgency == 1)
                    enqueue(high_priority_queue1, init_task_metrics(incoming_process));
                else if (incoming_process->urgency == 2)
                    enqueue(high_priority_queue2, init_task_metrics(incoming_process));
                else if (incoming_process->urgency == 3)
                    enqueue(high_priority_queue3, init_task_metrics(incoming_process));
                else if (incoming_process->urgency == 4)
                    enqueue(high_priority_queue4, init_task_metrics(incoming_process));

                current_process_node = current_process_node->next;
                if (current_process_node != NULL) {
                    incoming_process = (task *)(current_process_node->data);
                }
            }
        }

        if (active_process == NULL) {
            if (high_priority_queue1->size > 0)
                active_process = (task_metrics *)dequeue(high_priority_queue1);
            else if (high_priority_queue2->size > 0)
                active_process = (task_metrics *)dequeue(high_priority_queue2);
            else if (high_priority_queue3->size > 0)
                active_process = (task_metrics *)dequeue(high_priority_queue3);
            else if (high_priority_queue4->size > 0)
                active_process = (task_metrics *)dequeue(high_priority_queue4);

            if (current_time >= 100 && active_process->initial_time == -1) {
                active_process = NULL;
                continue;
            }
        }

        if (active_process != NULL) {
            task *executing_process = active_process->proc;

            printf("%c", executing_process->task_id);

            if (active_process->initial_time == -1) {
                active_process->initial_time = current_time;
            }
            active_process->duration++;

            if (active_process->duration >= executing_process->duration) {
                active_process->finish_time = current_time;

                switch (executing_process->urgency) {
                    case 1:
                        append_task(priority_list1, active_process);
                        break;
                    case 2:
                        append_task(priority_list2, active_process);
                        break;
                    case 3:
                        append_task(priority_list3, active_process);
                        break;
                    case 4:
                        append_task(priority_list4, active_process);
                        break;
                }

                active_process = NULL;
            }
        } else {
            printf("_");
        }

        current_time++;
    }

    metrics_summary avg_stats1, avg_stats2, avg_stats3, avg_stats4, overall_avg;
    printf("\nStatistics for Priority Queue 1:");
    avg_stats1 = print_policy_stat(priority_list1);
    printf("\nStatistics for Priority Queue 2:");
    avg_stats2 = print_policy_stat(priority_list2);
    printf("\nStatistics for Priority Queue 3:");
    avg_stats3 = print_policy_stat(priority_list3);
    printf("\nStatistics for Priority Queue 4:");
    avg_stats4 = print_policy_stat(priority_list4);

    overall_avg.avg_response_time = (avg_stats1.avg_response_time + avg_stats2.avg_response_time +
                                     avg_stats3.avg_response_time + avg_stats4.avg_response_time) / 4;
    overall_avg.avg_wait_time = (avg_stats1.avg_wait_time + avg_stats2.avg_wait_time +
                                 avg_stats3.avg_wait_time + avg_stats4.avg_wait_time) / 4;
    overall_avg.avg_turnaround = (avg_stats1.avg_turnaround + avg_stats2.avg_turnaround +
                                  avg_stats3.avg_turnaround + avg_stats4.avg_turnaround) / 4;
    overall_avg.avg_throughput = (avg_stats1.avg_throughput + avg_stats2.avg_throughput +
                                  avg_stats3.avg_throughput + avg_stats4.avg_throughput) / 4;

    printf("Overall average statistics for Highest Priority First Non-preemptive across all urgency queues:\n");
    printf("Average Response Time: %.1f\n", overall_avg.avg_response_time);
    printf("Average Wait Time: %.1f\n", overall_avg.avg_wait_time);
    printf("Average Turnaround Time: %.1f\n", overall_avg.avg_turnaround);

    return overall_avg;
}

