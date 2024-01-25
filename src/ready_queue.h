// ready_queue.h

#ifndef READY_QUEUE_H
#define READY_QUEUE_H


#include "process.h"


typedef struct ReadyQueue {
    Process *process;
    struct ReadyQueue *next;
    
} ReadyQueue;

ReadyQueue *createReadyQueue();
ReadyQueue *compareTwoProcesses(ReadyQueue *readyQueue, ReadyQueue *insertedReadyQueue);// compares the two process and returns the one with higher priority
ReadyQueue *insertReadyQueue(ReadyQueue *readyQueue, ReadyQueue *insertedReadyQueue);// inserts the process into the ready queue

#endif
