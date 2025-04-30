#ifndef _stat_h_
#define _stat_h_
#include "task.h"

enum State {UNUSED, EMBRYO, SLEEPING, RUNNING, ZOMBIE};

struct process_stat_s {
	task * proc;
	float idle_time;
	float completion_span;
	float start_response;

	float initial_time;
	float finish_time;
	float duration;

	enum State state;
};
typedef struct process_stat_s task_metrics;

struct scheduling_stat_s {
	task_list * process_queue;
	task_list * time_chart;
	float avg_turnaround_time;
	float avg_waiting_time;
	float avg_response_time;
	float throughput;
	int total_quanta;
};
typedef struct scheduling_stat_s scheduling_stat;

struct average_stats_s{
	float avg_response_time;
	float avg_wait_time;
	float avg_turnaround;
	float avg_throughput;
};

typedef struct average_stats_s metrics_summary;

metrics_summary first_come_first_serve_np(task_list * processes);
metrics_summary shortest_job_first_np(task_list * processes);
metrics_summary shortest_remaining_time_p(task_list * processes);
metrics_summary round_robin_p(task_list * processes,int time_slice);
metrics_summary highest_priority_first_p(task_list * processes);
metrics_summary highest_priority_first_np(task_list * processes);
metrics_summary highest_priority_first_p_aging(task_list * processes);
metrics_summary highest_priority_first_np_aging(task_list * processes);

// Print Stat//
metrics_summary print_policy_stat(task_list * ll);
#endif