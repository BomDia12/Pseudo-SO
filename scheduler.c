#include "./scheduler.h"

scheduler * get_scheduler() {
    static scheduler sched = {
        .system_resources = { 
            .scanner = { .value = 1, .name = "Scanner" },
            .printer_1 = { .value = 1, .name = "Printer 1" },
            .printer_2 = { .value = 1, .name = "Printer 2" },
            .modem = { .value = 1, .name = "Modem" },
            .sata_1 = { .value = 1, .name = "SATA 1" },
            .sata_2 = { .value = 1, .name = "SATA 2" },
            .sata_3 = { .value = 1, .name = "SATA 3" }
        }
    };
    return &sched;
}

void main_loop(process_list * plist) {
    scheduler * sched = get_scheduler();
    while (plist->process_count > 0) {
        // pediu para executar algum processo
        while (plist->processes[plist->offset]->entry_time == sched->curr_time) {
            schedule_process(plist->processes[plist->offset], sched)
            plist->offset++;
        }
        if (sched->curr_process) {
            sched->curr_process->curr_instruction++;
            if (sched->curr_process->curr_instruction >= sched->curr_process->total_instructions) {
                // TODO: free
                // TODO: release resources
                sched->curr_process = NULL; // Process completed
                select_next_process(sched);
            } else if (sched->preemptive_mode && sched->curr_time == sched->preemptive_time) {
                if (sched->curr_process->priority == 1 || sched->curr_process->priority < 5) {
                    sched->curr_process->priority++;
                }
                schedule_process(sched->curr_process, sched); // Re-schedule current process
                process * next_process = select_next_process(sched);
                // if there are no resources, block the process
            }
        }
        sched->curr_time++;
    }
}

void schedule_process(process * process, scheduler * scheduler) {
    switch (process->priority) {
        case 0:
            enqueue(scheduler->rt_queue, process);
            break;
        case 1:
            enqueue(scheduler->user_queue_1, process);
            break;
        case 2:
            enqueue(scheduler->user_queue_2, process);
            break;
        case 3:
            enqueue(scheduler->user_queue_3, process);
            break;
        case 4:
            enqueue(scheduler->user_queue_4, process);
            break;
        default:
            enqueue(scheduler->user_queue_5, process);
            break;
    }
}

process * select_next_process(scheduler * scheduler) {
    // apply aging

    if (scheduler->rt_queue->curr_size > 0) {
        return dequeue(scheduler->rt_queue);
    }
    if (scheduler->user_queue_1->curr_size > 0) {
        return dequeue(scheduler->user_queue_1);
    }
    if (scheduler->user_queue_2->curr_size > 0) {
        return dequeue(scheduler->user_queue_2);
    }
    if (scheduler->user_queue_3->curr_size > 0) {
        return dequeue(scheduler->user_queue_3);
    }
    if (scheduler->user_queue_4->curr_size > 0) {
        return dequeue(scheduler->user_queue_4);
    }
    if (scheduler->user_queue_5->curr_size > 0) {
        return dequeue(scheduler->user_queue_5);
    }
    scheduler->curr_process = NULL; // No process to schedule
    return NULL;
}