// arrival_queue.h

#ifndef ARRIVAL_QUEUE_H
#define ARRIVAL_QUEUE_H

#include "process.h"

typedef struct ProcessesArrivalQueue {
    Process *process;
    struct ProcessesArrivalQueue *next;
    
} ProcessesArrivalQueue;

ProcessesArrivalQueue *createProcessesArrivalQueue();
ProcessesArrivalQueue* insert_arrival_process(ProcessesArrivalQueue *processesArrivalQueue, Process *process);

#endif

