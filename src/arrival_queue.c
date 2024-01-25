// arrival_queue.c

#include "arrival_queue.h"

ProcessesArrivalQueue *createProcessesArrivalQueue(){
    ProcessesArrivalQueue *processesArrivalQueue = malloc(sizeof(ProcessesArrivalQueue));
    processesArrivalQueue->process = NULL;
    processesArrivalQueue->next = NULL;
    
    return processesArrivalQueue;
}

ProcessesArrivalQueue* insert_arrival_process(ProcessesArrivalQueue *processesArrivalQueue, Process *process) {
    ProcessesArrivalQueue *newProcessesArrivalQueue = malloc(sizeof(ProcessesArrivalQueue));
    newProcessesArrivalQueue->process = process;
    newProcessesArrivalQueue->next = NULL;
    // compare the process arrival time and insert at the right place
    if(processesArrivalQueue-> process == NULL){
        processesArrivalQueue->process = process;
        //printf("Process name:(inserted at the beggining) %s\n", processesArrivalQueue->process->name);
        return processesArrivalQueue;
        
    }
    else if (processesArrivalQueue->process->arrivalTime > process->arrivalTime){
        newProcessesArrivalQueue->next = processesArrivalQueue;
        processesArrivalQueue = newProcessesArrivalQueue;
        //printf("Process name:(inserted at the beggining else if) %s\n", processesArrivalQueue->process->name);
        return processesArrivalQueue;
        
    }
    else{
        ProcessesArrivalQueue *temp = processesArrivalQueue;
        while (temp->next != NULL && temp->next->process->arrivalTime < process->arrivalTime){
            temp = temp->next;
        }
        newProcessesArrivalQueue->next = temp->next;
        temp->next = newProcessesArrivalQueue;
        return processesArrivalQueue;
        
    }
    
}


