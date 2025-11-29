#pragma once

#include "types.h"
#include "memory.h"

scheduler * get_scheduler();

void scheduler_process(process_list * plist);

void schedule_process(process * process, scheduler * scheduler);

process * select_next_process(scheduler * scheduler); // applies aging

process * dequeue(queue * q);

void enqueue(queue * q, process * p);

int resources_available(process * proc, scheduler * sched);

void block_process(process * proc, scheduler * sched);

void age_processes(scheduler * sched);
