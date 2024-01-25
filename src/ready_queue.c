// ready_queue.c

#include "ready_queue.h"

// function to create a ready queue
ReadyQueue *createReadyQueue(){
    ReadyQueue *readyQueue = malloc(sizeof(readyQueue));
    readyQueue->process = NULL;
    readyQueue->next = NULL;
    
    return readyQueue;
}

// function to compare two processes
ReadyQueue *compareTwoProcesses(ReadyQueue *readyQueue, ReadyQueue *insertedReadyQueue){// compares the two process and returns the one with higher priority
    // if the two processes is the Platinum type 
    if(readyQueue->process->type + insertedReadyQueue->process->type == 0){ // if both of them are platinium compare priorities
        // both platinium
        if(readyQueue->process->priority == insertedReadyQueue->process->priority){
            // if the two processes have the same priority then compare the arrival time
            if(readyQueue->process->arrivalTime == insertedReadyQueue->process->arrivalTime){
                // if the two processes have the same arrival time then compare the process name
                if(strcmp(readyQueue->process->name, insertedReadyQueue->process->name) < 0){
                    // if the ready queue process name is smaller then it has higher priority
                    return readyQueue;
                }
                else{
                    // if the inserted process name is smaller then it has higher priority
                    return insertedReadyQueue;
                }
            }
            else if(readyQueue->process->arrivalTime < insertedReadyQueue->process->arrivalTime){
                // if the ready queue process has smaller arrival time then it has higher priority
                return readyQueue;
            }
            else{
                // if the inserted process has smaller arrival time then it has higher priority
                return insertedReadyQueue;
            }
        }
        else if(readyQueue->process->priority > insertedReadyQueue->process->priority){
            // if the ready queue process has higher priority then it has higher priority
            return readyQueue;
        }
        else{
            // if the inserted process has higher priority then it has higher priority
            return insertedReadyQueue;
        }
        //TODO
    }
    else if(readyQueue->process->type * insertedReadyQueue->process->type == 0){ // if one of them is platinium return it
        // one of them is platinium
        if(readyQueue->process->type == 0){
            // ready queue is platinium
            return readyQueue;
        }
        else{
            // inserted ready queue is platinium
            return insertedReadyQueue;
        }
    }
    else{ // none of them is platinium compare priorities
        if(readyQueue->process->priority == insertedReadyQueue->process->priority){
            // if the two processes have the same priority then compare the arrival time
            if(readyQueue->process->arrivalTime == insertedReadyQueue->process->arrivalTime){
                // if the two processes have the same arrival time then compare the process name
                if(strcmp(readyQueue->process->name, insertedReadyQueue->process->name) < 0){
                    // if the ready queue process name is smaller then it has higher priority
                    return readyQueue;
                }
                else{
                    // if the inserted process name is smaller then it has higher priority
                    return insertedReadyQueue;
                }
            }
            else if(readyQueue->process->arrivalTime < insertedReadyQueue->process->arrivalTime){
                // if the ready queue process has smaller arrival time then it has higher priority
                return readyQueue;
            }
            else{
                // if the inserted process has smaller arrival time then it has higher priority
                return insertedReadyQueue;
            }
        }
        else if(readyQueue->process->priority > insertedReadyQueue->process->priority){
            // if the ready queue process has higher priority then it has higher priority
            return readyQueue;
        }
        else{
            // if the inserted process has higher priority then it has higher priority
            return insertedReadyQueue;
        }

    }

   
}


// function to insert a process into the ready queue
ReadyQueue *insertReadyQueue(ReadyQueue *readyQueue, ReadyQueue *insertedReadyQueue){
    // recursively compare the two processes and insert the insertedReadyQueue at the right place
    //printf("Insert function called\n");
    if ( readyQueue == NULL || readyQueue->process == NULL){
        readyQueue = insertedReadyQueue;
        return readyQueue;
    }
    else{
        //printf("comparing two processes\n");
        if(readyQueue == compareTwoProcesses(readyQueue, insertedReadyQueue)){
            //if ready queue is the one with higher priority then check the next one
            readyQueue->next = insertReadyQueue(readyQueue->next, insertedReadyQueue);
            return readyQueue;

        }
        else{
            // if inserted ready queue is the one with higher priority then insert it at the beginning
            insertedReadyQueue->next = readyQueue;
            readyQueue = insertedReadyQueue;
            return readyQueue;
        }

    }    
    
}
