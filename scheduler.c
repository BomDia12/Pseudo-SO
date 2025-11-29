#include "scheduler.h"

scheduler * get_scheduler() {
    static scheduler sched = {
        .curr_time = 0,
        .preemptive_mode = true,
        .preemptive_time = 5,
        .rt_queue = {
            .priority = 0,
            .curr_size = 0,
            .first = NULL,
            .last = NULL,
            .alloted_time = 1,
            .preemptive = false
        },
        .user_queue_1 = {
            .priority = 1,
            .curr_size = 0,
            .first = NULL,
            .last = NULL,
            .alloted_time = 6,
            .preemptive = true
        },
        .user_queue_2 = {
            .priority = 2,
            .curr_size = 0,
            .first = NULL,
            .last = NULL,
            .alloted_time = 5,
            .preemptive = true
        },
        .user_queue_3 = {
            .priority = 3,
            .curr_size = 0,
            .first = NULL,
            .last = NULL,
            .alloted_time = 4,
            .preemptive = true
        },
        .user_queue_4 = {
            .priority = 4,
            .curr_size = 0,
            .first = NULL,
            .last = NULL,
            .alloted_time = 3,
            .preemptive = true
        },
        .user_queue_5 = {
            .priority = 5,
            .curr_size = 0,
            .first = NULL,
            .last = NULL,
            .alloted_time = 2,
            .preemptive = true
        },
        .blocked_processes = {
            .processes = NULL,
            .count = 0
        },
        .allocated_blocks = 0,
        .memory_blocks = NULL,
        .curr_process = NULL,
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

void scheduler_process(process_list * plist) {
    scheduler * sched = get_scheduler();

    while (true) {

        while (plist->offset < plist->process_count && plist->processes[plist->offset]->entry_time == sched->curr_time) {
            schedule_process(plist->processes[plist->offset], sched);
            print_process_info(plist->processes[plist->offset]);
            plist->offset++;
        }

        process * new_process = NULL;
        if (sched->curr_process) {
            printf("Executando processo %d na instrução %d...\n", sched->curr_process->pid, sched->curr_process->curr_instruction);
            sched->curr_process->curr_instruction++;
            if (sched->curr_process->curr_instruction >= sched->curr_process->total_instructions) {
                remove_memory(sched->curr_process);
                // TODO: release resources
                printf("Processo %d finalizado!\n", sched->curr_process->pid);
                printf("-------------------------------\n\n");
                sched->curr_process = NULL;
                new_process = select_next_process(sched);
            } else if (sched->preemptive_mode && sched->curr_time == sched->preemptive_time) {
                if (sched->curr_process->priority == 1 || sched->curr_process->priority < 5) {
                    sched->curr_process->priority++;
                    sched->curr_process->last_promotion_time = sched->curr_time;
                }
                schedule_process(sched->curr_process, sched);
                new_process = select_next_process(sched);
            }
        } else {
            new_process = select_next_process(sched);
        }

        while (new_process) {
            if (resources_available(new_process, sched) == 0) {
                sched->curr_process = new_process;
                break;
            }
            block_process(new_process, sched);
            new_process = select_next_process(sched);
        }

        sched->curr_time++;

        if (sched->curr_process == NULL && plist->offset >= plist->process_count) {
            bool all_queues_empty = 
                sched->rt_queue.curr_size == 0 &&
                sched->user_queue_1.curr_size == 0 &&
                sched->user_queue_2.curr_size == 0 &&
                sched->user_queue_3.curr_size == 0 &&
                sched->user_queue_4.curr_size == 0 &&
                sched->user_queue_5.curr_size == 0 &&
                sched->blocked_processes.count == 0;
            if (all_queues_empty) {
                printf("Todos os processos foram finalizados!\n");
                break;
            }
        }
    }
}

void schedule_process(process * process, scheduler * scheduler) {
    switch (process->priority) {
        case 0:
            enqueue(&scheduler->rt_queue, process);
            break;
        case 1:
            enqueue(&scheduler->user_queue_1, process);
            break;
        case 2:
            enqueue(&scheduler->user_queue_2, process);
            break;
        case 3:
            enqueue(&scheduler->user_queue_3, process);
            break;
        case 4:
            enqueue(&scheduler->user_queue_4, process);
            break;
        default:
            enqueue(&scheduler->user_queue_5, process);
            break;
    }
}

process * select_next_process(scheduler * scheduler) {
    age_processes(scheduler);

    if (scheduler->rt_queue.curr_size > 0) {
        return dequeue(&scheduler->rt_queue);
    }
    if (scheduler->user_queue_1.curr_size > 0) {
        return dequeue(&scheduler->user_queue_1);
    }
    if (scheduler->user_queue_2.curr_size > 0) {
        return dequeue(&scheduler->user_queue_2);
    }
    if (scheduler->user_queue_3.curr_size > 0) {
        return dequeue(&scheduler->user_queue_3);
    }
    if (scheduler->user_queue_4.curr_size > 0) {
        return dequeue(&scheduler->user_queue_4);
    }
    if (scheduler->user_queue_5.curr_size > 0) {
        return dequeue(&scheduler->user_queue_5);
    }

    scheduler->curr_process = NULL; // No process to schedule
    return NULL;
}

process * dequeue(queue * q) {
    if (q->curr_size <= 0) {
        return NULL; // Queue is empty
    }

    process * proc = q->first->process;
    queue_item * old_first = q->first;
    q->first = q->first->next;
    free(old_first);
    q->curr_size--;
    return proc;
}

void enqueue(queue * q, process * p) {
    printf("Alocando processo %d na fila %d...\n", p->pid, q->priority);
    queue_item * item = (queue_item *) malloc(sizeof(queue_item));
    item->process = p;
    item->next = NULL;

    if (q->first == NULL) {
        q->first = item;
    } else {
        queue_item * curr_last = q->last;
        curr_last->next = item;
    }

    q->last = item;
    q->curr_size++;
}

int resources_available(process * proc, scheduler * sched) {
    int res = 0;
    // Check scanner
    // Check printers
    // Check modem
    // Check SATAs
    res = allocate_memory(proc);

    if (res != 0) {
        return res;
    }

    return 0;
}

void block_process(process * proc, scheduler * sched) {
    if (sched->blocked_processes.count == 0) {
        sched->blocked_processes.processes = malloc(sizeof(process*));
    } else {
        sched->blocked_processes.processes = realloc(
            sched->blocked_processes.processes,
            sizeof(process*) * (sched->blocked_processes.count + 1)
        );
    }
    sched->blocked_processes.processes[sched->blocked_processes.count] = proc;
    sched->blocked_processes.count++;
}

void age_processes(scheduler * sched) {   
    int aging_threshold = 20; // time units after which aging occurs
    queue * queues[] = {
        &sched->user_queue_5,
        &sched->user_queue_4,
        &sched->user_queue_3,
        &sched->user_queue_2
    };

    for (int i = 0; i < 4; i++) {
        queue * q = queues[i];
        queue_item * current = q->first;
        while (current != NULL) {
            process * proc = current->process;
            if (sched->curr_time - proc->last_promotion_time >= aging_threshold) {
                if (proc->priority > 1) {
                    proc->priority--;
                    proc->last_promotion_time = sched->curr_time;
                    process * to_requeue = dequeue(q);
                    schedule_process(to_requeue, sched);
                    current = q->first; // Restart from the beginning of the queue
                    continue;
                }
            } else {
                // if head of queue not eligible for aging, no process further in queue will be eligible
                break;
            }
            current = current->next;
        }
    }
}
