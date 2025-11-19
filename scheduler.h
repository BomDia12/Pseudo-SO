#pragma once

#include "types.h"

scheduler * get_scheduler();

void main_loop(process_list * plist);

void schedule_process(process * process, scheduler * scheduler);

process * select_next_process(scheduler * scheduler); // applies aging

process * dequeue(queue * q);

void enqueue(queue * q, process * p);
