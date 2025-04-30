#include <stdio.h>
#include "../include/utility.h"
#include "../include/task.h"
#include "../include/stat.h"

int main(int argc, char **argv) {
    FILE *log = fopen("output/report.txt", "w");
    if (log == NULL) {
        fprintf(stderr, "Failed to open report.txt for writing.\n");
        return 1;
    }

    int round_counter = 0;
    
    metrics_summary fcfs[6], sjf[6], srf[6], rr[6], hpfp[6], hpfnp[6], final[6];

    while (round_counter < 5) {
       
        fprintf(log, "\n*************************************************\n");

        fprintf(log, "\t\t\t\tRound %d\n", round_counter + 1);
        fprintf(log, "*************************************************\n");

        task_list *process_list = produce_tasks(52);
        task_node *process_pointer = process_list->head;

        fprintf(log, "-------------------------------------------------\n");

        fprintf(log, "Process ID | Arrival Time | Run Time | Priority |\n");
        fprintf(log, "-------------------------------------------------\n");
        while (process_pointer != NULL) {
            task *proc = (task *)process_pointer->data;
            fprintf(log, "%10c | %12.1f | %8.1f | %8d |\n", proc->task_id, proc->entry_time, proc->duration, proc->urgency);
            process_pointer = process_pointer->next;
        }
        fprintf(log, "--------------------------------------------------\n");
        fprintf(log, "Total No. of Processes : %d\n", process_list->size);

        fprintf(log, "--------------------------------------------------\n");

        
        fcfs[round_counter] = first_come_first_serve_np(process_list);

        
        rr[round_counter] = round_robin_p(process_list, 5);

        
        sjf[round_counter] = shortest_job_first_np(process_list);

        
        srf[round_counter] = shortest_remaining_time_p(process_list);

        
        hpfp[round_counter] = highest_priority_first_p(process_list);

        
        hpfnp[round_counter] = highest_priority_first_np(process_list);

        round_counter++;
    }

    for (int i = 0; i < 6; i++) {
        
        final[i].avg_response_time = 0;
        final[i].avg_wait_time = 0;
        final[i].avg_turnaround = 0;
        final[i].avg_throughput = 0;
    }

   
    for (int i = 0; i < 5; i++) {
        final[0].avg_response_time += fcfs[i].avg_response_time;
        final[1].avg_response_time += sjf[i].avg_response_time;
        final[2].avg_response_time += srf[i].avg_response_time;
        final[3].avg_response_time += rr[i].avg_response_time;
        final[4].avg_response_time += hpfp[i].avg_response_time;
        final[5].avg_response_time += hpfnp[i].avg_response_time;

        final[0].avg_wait_time += fcfs[i].avg_wait_time;
        final[1].avg_wait_time += sjf[i].avg_wait_time;
        final[2].avg_wait_time += srf[i].avg_wait_time;
        final[3].avg_wait_time += rr[i].avg_wait_time;
        final[4].avg_wait_time += hpfp[i].avg_wait_time;
        final[5].avg_wait_time += hpfnp[i].avg_wait_time;

        final[0].avg_turnaround += fcfs[i].avg_turnaround;
        final[1].avg_turnaround += sjf[i].avg_turnaround;
        final[2].avg_turnaround += srf[i].avg_turnaround;
        final[3].avg_turnaround += rr[i].avg_turnaround;
        final[4].avg_turnaround += hpfp[i].avg_turnaround;
        final[5].avg_turnaround += hpfnp[i].avg_turnaround;

        final[0].avg_throughput += fcfs[i].avg_throughput;
        final[1].avg_throughput += sjf[i].avg_throughput;
        final[2].avg_throughput += srf[i].avg_throughput;
        final[3].avg_throughput += rr[i].avg_throughput;
        final[4].avg_throughput += hpfp[i].avg_throughput;
        final[5].avg_throughput += hpfnp[i].avg_throughput;
    }

    
    for (int i = 0; i < 6; i++) {
        final[i].avg_response_time /= 5;
        final[i].avg_wait_time /= 5;
        final[i].avg_turnaround /= 5;
        final[i].avg_throughput /= 5;
    }

    fprintf(log, "\n\n\n");
    fprintf(log, "_____________________________________________________________________________________________________________________________\n\n");
    fprintf(log, "The average of the 5 runs of every algorithm is as follows:\n");
    fprintf(log, "\n");

   
    fprintf(log, "ALGORITHM: First-come First-served (FCFS) [non-preemptive]:\n");
    fprintf(log, "Average Response Time: %.1f\n", final[0].avg_response_time);
    fprintf(log, "Average Wait Time: %.1f\n", final[0].avg_wait_time);
    fprintf(log, "Average Turn Around Time: %.1f\n", final[0].avg_turnaround);
    fprintf(log, "Average Throughput: %.1f\n", final[0].avg_throughput);
    fprintf(log, "\n");

    fprintf(log, "ALGORITHM: Round Robin (RR) [preemptive]:\n");
    fprintf(log, "Average Response Time: %.1f\n", final[3].avg_response_time);
    fprintf(log, "Average Wait Time: %.1f\n", final[3].avg_wait_time);
    fprintf(log, "Average Turn Around Time: %.1f\n", final[3].avg_turnaround);
    fprintf(log, "Average Throughput: %.1f\n", final[3].avg_throughput);
    fprintf(log, "\n");

    fprintf(log, "ALGORITHM: Shortest Job First (SJF) [non-preemptive]:\n");
    fprintf(log, "Average Response Time: %.1f\n", final[1].avg_response_time);
    fprintf(log, "Average Wait Time: %.1f\n", final[1].avg_wait_time);
    fprintf(log, "Average Turn Around Time: %.1f\n", final[1].avg_turnaround);
    fprintf(log, "Average Throughput: %.1f\n", final[1].avg_throughput);
    fprintf(log, "\n");

    fprintf(log, "ALGORITHM: Shortest Remaining Time (SRT) [preemptive]:\n");
    fprintf(log, "Average Response Time: %.1f\n", final[2].avg_response_time);
    fprintf(log, "Average Wait Time: %.1f\n", final[2].avg_wait_time);
    fprintf(log, "Average Turn Around Time: %.1f\n", final[2].avg_turnaround);
    fprintf(log, "Average Throughput: %.1f\n", final[2].avg_throughput);
    fprintf(log, "\n");

    fprintf(log, "ALGORITHM: Highest Priority First (HPF) [preemptive]:\n");
    fprintf(log, "Average Response Time: %.1f\n", final[4].avg_response_time);
    fprintf(log, "Average Wait Time: %.1f\n", final[4].avg_wait_time);
    fprintf(log, "Average Turn Around Time: %.1f\n", final[4].avg_turnaround);
    fprintf(log, "Average Throughput: %.1f\n", final[4].avg_throughput);
    fprintf(log, "\n");

    fprintf(log, "ALGORITHM: Highest Priority First (HPF) [non-preemptive]:\n");
    fprintf(log, "Average Response Time: %.1f\n", final[5].avg_response_time);
    fprintf(log, "Average Wait Time: %.1f\n", final[5].avg_wait_time);
    fprintf(log, "Average Turn Around Time: %.1f\n", final[5].avg_turnaround);
    fprintf(log, "Average Throughput: %.1f\n", final[5].avg_throughput);

    fclose(log);
}
