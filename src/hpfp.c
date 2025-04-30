#include <stdio.h>
#include <stdlib.h>
#include "../include/stat.h"
#include "../include/utility.h"


task_metrics *init_task_metrics(task *proc);

int priority_comparison(void *data1, void *data2) {
    task_metrics *proc1 = (task_metrics *)data1;
    task_metrics *proc2 = (task_metrics *)data2;

    if ((((task *)proc1->proc)->urgency) < ((((task *)proc2->proc)->urgency))) {
        return -1;
    } else if ((((task *)proc1->proc)->urgency) == ((((task *)proc2->proc)->urgency)) && (((task *)proc1->proc)->entry_time < (((task *)proc2->proc)->entry_time))) {
        return -1;
    } else {
        return 1;
    }
}

metrics_summary highest_priority_first_p(task_list *procs) {
    int quanta = 0;

    task_list *priority_one_list = init_task_list();
    task_list *priority_two_list = init_task_list();
    task_list *priority_three_list = init_task_list();
    task_list *priority_four_list = init_task_list();

    task_queue *priority_one_queue = create_queue();
    task_queue *priority_two_queue = create_queue();
    task_queue *priority_three_queue = create_queue();
    task_queue *priority_four_queue = create_queue();

    task_node *process_pointer = procs->head;
    if (procs->head == NULL) {
        fprintf(stderr, "No processes available for scheduling\n");
    }

    printf("\nHighest Priority First Preemptive:\n");

    task_metrics *currently_executing_process = NULL;
    while (quanta < 100 || currently_executing_process != NULL) {
        if (currently_executing_process != NULL) {
            if (currently_executing_process->proc->urgency == 1) 
                enqueue(priority_one_queue, currently_executing_process);
            if (currently_executing_process->proc->urgency == 2) 
                enqueue(priority_two_queue, currently_executing_process);
            if (currently_executing_process->proc->urgency == 3) 
                enqueue(priority_three_queue, currently_executing_process);
            if (currently_executing_process->proc->urgency == 4) 
                enqueue(priority_four_queue, currently_executing_process);
        }
        if (process_pointer != NULL) {
            task *newly_arrived_process = (task *)(process_pointer->data);

            while (process_pointer != NULL && newly_arrived_process->entry_time <= quanta) {
                if (newly_arrived_process->urgency == 1) 
                    enqueue(priority_one_queue, init_task_metrics(newly_arrived_process));
                if (newly_arrived_process->urgency == 2) 
                    enqueue(priority_two_queue, init_task_metrics(newly_arrived_process));
                if (newly_arrived_process->urgency == 3) 
                    enqueue(priority_three_queue, init_task_metrics(newly_arrived_process));
                if (newly_arrived_process->urgency == 4) 
                    enqueue(priority_four_queue, init_task_metrics(newly_arrived_process));

                sort(priority_one_queue, priority_comparison);
                sort(priority_two_queue, priority_comparison);
                sort(priority_three_queue, priority_comparison);
                sort(priority_four_queue, priority_comparison);

                process_pointer = process_pointer->next;
                if (process_pointer != NULL) {
                    newly_arrived_process = (task *)(process_pointer->data);
                }
            }
        }
        if (currently_executing_process == NULL) {
            if (priority_one_queue->size > 0) 
                currently_executing_process = (task_metrics *)dequeue(priority_one_queue);
            else if (priority_two_queue->size > 0) 
                currently_executing_process = (task_metrics *)dequeue(priority_two_queue);
            else if (priority_three_queue->size > 0) 
                currently_executing_process = (task_metrics *)dequeue(priority_three_queue);
            else if (priority_four_queue->size > 0) 
                currently_executing_process = (task_metrics *)dequeue(priority_four_queue);

            if (quanta >= 100 && currently_executing_process->initial_time == -1) {
                currently_executing_process = NULL;
                continue;
            }
        }
        if (currently_executing_process != NULL) {
            task *executing_process = currently_executing_process->proc;

            printf("%c", executing_process->task_id);

            if (currently_executing_process->initial_time == -1) {
                currently_executing_process->initial_time = quanta;
            }

            currently_executing_process->duration++;

            if (currently_executing_process->duration >= executing_process->duration) {
                currently_executing_process->finish_time = quanta;
                if (currently_executing_process->proc->urgency == 1) 
                    append_task(priority_one_list, currently_executing_process);
                else if (currently_executing_process->proc->urgency == 2) 
                    append_task(priority_two_list, currently_executing_process);
                else if (currently_executing_process->proc->urgency == 3) 
                    append_task(priority_three_list, currently_executing_process);
                else if (currently_executing_process->proc->urgency == 4) 
                    append_task(priority_four_list, currently_executing_process);
                currently_executing_process = NULL;
            }
        } else {
            printf("_");
        }
        quanta++;
    }
    metrics_summary priority_one_average, priority_two_average, priority_three_average, priority_four_average, overall_average;
    printf("\nFor the Priority Queue 1");
    priority_one_average = print_policy_stat(priority_one_list);
    printf("\nFor the Priority Queue 2");
    priority_two_average = print_policy_stat(priority_two_list);
    printf("\nFor the Priority Queue 3");
    priority_three_average = print_policy_stat(priority_three_list);
    printf("\nFor the Priority Queue 4");
    priority_four_average = print_policy_stat(priority_four_list);

    overall_average.avg_response_time = (priority_one_average.avg_response_time + priority_two_average.avg_response_time + 
                                         priority_three_average.avg_response_time + priority_four_average.avg_response_time) / 4;
    overall_average.avg_wait_time = (priority_one_average.avg_wait_time + priority_two_average.avg_wait_time + 
                                     priority_three_average.avg_wait_time + priority_four_average.avg_wait_time) / 4;
    overall_average.avg_turnaround = (priority_one_average.avg_turnaround + priority_two_average.avg_turnaround + 
                                      priority_three_average.avg_turnaround + priority_four_average.avg_turnaround) / 4;
    overall_average.avg_throughput = (priority_one_average.avg_throughput + priority_two_average.avg_throughput + 
                                      priority_three_average.avg_throughput + priority_four_average.avg_throughput);

    printf("\nThe average times of Highest Priority First Preemptive for all queues:\n");
    printf("The Average Response Time: %.1f\n", overall_average.avg_response_time);
    printf("The Average Wait Time: %.1f\n", overall_average.avg_wait_time);
    printf("The Average Turn Around Time: %.1f\n", overall_average.avg_turnaround);

    return overall_average;
}