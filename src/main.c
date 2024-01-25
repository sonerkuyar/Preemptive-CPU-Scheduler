#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "ready_queue.h"
#include "arrival_queue.h"

// function to create an instruction
ReadyQueue *readyQueue;
ProcessesArrivalQueue *arrivalQueue;
ProcessesArrivalQueue *finishedProcessesQueue;
void checkNewArrival(int *current_time);
void executeProcess(Process *process, int *current_time);

int main()
{
    // read 21 instruction from "instructions.txt" and store them in an array of instructions each line is "instruction_name duration\n"
    FILE *fp;
    fp = fopen("instructions.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file.You should put the instruction.txt file to same folder with executable.\n");
        exit(1);
    }

    int instuctions_duration[21];
    // read file and pu into the array in line order
    char line[100];
    int i = 0;
    while (fgets(line, 100, fp) != NULL)
    {
        // if line is empty skip it

        char *token = strtok(line, " ");
        token = strtok(NULL, " ");
        instuctions_duration[i] = atoi(token);
        i++;
    }
    // print durations

    fclose(fp);

    arrivalQueue = createProcessesArrivalQueue();
    finishedProcessesQueue = createProcessesArrivalQueue();

    // read process stream from "definition.txt" each line is "process_name priority arrival_time type\n"
    fp = fopen("definition.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file. You should put the definition.txt file to same folder with executable.\n");
        exit(1);
    }

    // read file and put into the array in line order
    char line2[100];
    i = 0;

    while (fgets(line2, 100, fp) != NULL)
    {

        char *token = strtok(line2, " ");

        // allocate memory for the process name
        char *process_name = strcpy(malloc(strlen(token) + 1), token);
        token = strtok(NULL, " ");

        int priority = atoi(token);
        token = strtok(NULL, " ");

        int arrival_time = atoi(token);
        token = strtok(NULL, " ");
        size_t tokenLength = strcspn(token, "\n");
        token[tokenLength] = '\0';
        char *type = strcpy(malloc(strlen(token) + 1), token);

        // get process instructions from <process_name>.txt
        char *process_file_name = strcpy(malloc(strlen(process_name) + 1), process_name);
        process_file_name = strcat(process_file_name, ".txt");

        FILE *fp2;
        fp2 = fopen(process_file_name, "r");
        if (fp2 == NULL)
        {
            printf("Error opening file. Put the process files to same folder with executable.\n");
            exit(1);
        }

        // read file and put into the array in line order
        char line3[20];
        int instruction_count = 0;
        int j = 0;
        int process_instructions[20];
        while (fgets(line3, 100, fp2) != NULL)
        {
            // one word in each line <instruction_name>\n
            size_t tokenLength = strcspn(line3, "\n");
            line3[tokenLength] = '\0';
            char *instruction_name = strcpy(malloc(strlen(line3) + 1), line3);
            // instruction_name = "instr<instruction_number>"" find  instruction_durations[<instruction_number> -1]
            if (strcmp(instruction_name, "exit") == 0)
            {
                process_instructions[j] = 10;
                j++;
                break;
            }
            char *instruction_number = strtok(instruction_name, "instr");

            int instruction_index = atoi(instruction_number);
            process_instructions[j] = instuctions_duration[instruction_index - 1];
            j++;
        }

        instruction_count = j;

        Process *process = createProcess(process_name, arrival_time, 0, priority, type, process_instructions, instruction_count);

        arrivalQueue = insert_arrival_process(arrivalQueue, process);

        // printf("Process time quantum: %d\n", process->timeQuantum);
        i++;
    }
    fclose(fp);

    // create ready queue
    int current_time = 0;
    readyQueue = createReadyQueue();

    // load the first process into the ready queue
    Process *current_process = malloc(sizeof(Process));
    current_process = NULL;

    while (readyQueue != NULL || arrivalQueue != NULL)
    {
        // TODO : CPU idle condition
        if (readyQueue == NULL && current_process == NULL && current_time < arrivalQueue->process->arrivalTime)
        {
            // wait for new process from arrival queue
            current_time = arrivalQueue->process->arrivalTime;
            // printf("CPU idle for %d\n", current_time);
        }

        while (arrivalQueue != NULL && arrivalQueue->process->arrivalTime <= current_time)
        {
            // insert the process into the ready queue
            ReadyQueue *insertedReadyQueue = createReadyQueue();
            insertedReadyQueue->process = arrivalQueue->process;
            readyQueue = insertReadyQueue(readyQueue, insertedReadyQueue);
            // printf("process %s inserted into ready queue\n", arrivalQueue->process->name);
            arrivalQueue = arrivalQueue->next;
            if (arrivalQueue == NULL)
            {
                break;
            }
        }
        // get the first element of ready queue and execute it
        if (current_process == NULL)
        {
            current_time += 10;
        }

        current_process = readyQueue->process;
        readyQueue = readyQueue->next;
        executeProcess(current_process, &current_time);
        // printf("current time: %d after executing %s\n", current_time,current_process->name);

        // make operations of returning process

        if (current_process->completionTime != 0)
        {
            finishedProcessesQueue = insert_arrival_process(finishedProcessesQueue, current_process);
            current_process = NULL;

            continue;
        }

        // check for time quantum if silver process total quantum is 3 then upgrade it to gold
        if (current_process->type == 2 && current_process->totalQuantum == 3)
        {
            current_process->type = 1;
            current_process->typeName = "GOLD";
            current_process->totalQuantum = 0;
            // insert the current process into the ready queue
        }
        // if gold process time quantum is 5 then upgrade it to platinum
        else if (current_process->type == 1 && current_process->totalQuantum == 5)
        {
            current_process->type = 0;
            current_process->typeName = "PLATINUM";
            current_process->totalQuantum = 0;

            // if current process upgraded to platinium but there is no platinium in ready queue dont make cs
            if (readyQueue != NULL && readyQueue->process->type == 0)
            {
                // make the current process preemptive one context switch
                // copy current process to temp
                Process *temp = malloc(sizeof(Process));
                temp = current_process;
                // make the readyque process current process
                current_process = readyQueue->process;
                readyQueue = readyQueue->next;
                // insert the temp process into the ready queue
                ReadyQueue *insertedReadyQueue = createReadyQueue();
                insertedReadyQueue->process = temp;
                readyQueue = insertReadyQueue(readyQueue, insertedReadyQueue);
                // add cs time and execute the current platinum process
                current_time += 10;
                executeProcess(current_process, &current_time);
                // make operations of returning process
                if (current_process->completionTime != 0)
                {
                    finishedProcessesQueue = insert_arrival_process(finishedProcessesQueue, current_process);
                    current_process = NULL;
                    continue;
                }
            }
            // else there are no platinium in ready queue so dont make cs
            else
            {
                executeProcess(current_process, &current_time);
                // make operations of returning process
                if (current_process->completionTime != 0)
                {

                    finishedProcessesQueue = insert_arrival_process(finishedProcessesQueue, current_process);
                    current_process = NULL;
                    continue;
                }
            }
        }

        // insert the current process into the ready queue
        ReadyQueue *insertedReadyQueue = createReadyQueue();
        insertedReadyQueue->process = current_process;
        readyQueue = insertReadyQueue(readyQueue, insertedReadyQueue);
        if (current_process != readyQueue->process)
        {
            current_process = NULL;
        }
    }

    // calculate average turnarorund time
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int process_count = 0;
    while (finishedProcessesQueue != NULL)
    {
        // printf("Process name: %s\n", finishedProcessesQueue->process->name);
        // printf("Process arrival time: %d\n", finishedProcessesQueue->process->arrivalTime);
        //  printf("Process burst time: %d\n", finishedProcessesQueue->process->burstTime);
        //  printf("process completion time: %d\n", finishedProcessesQueue->process->completionTime);
        total_turnaround_time += finishedProcessesQueue->process->completionTime - finishedProcessesQueue->process->firstarrivalTime;
        total_waiting_time += finishedProcessesQueue->process->completionTime - finishedProcessesQueue->process->firstarrivalTime - finishedProcessesQueue->process->burstTime;
        finishedProcessesQueue = finishedProcessesQueue->next;
        process_count++;
    }

    float average_turnaround_time = (float)total_turnaround_time / (float)process_count;
    float average_waiting_time = (float)total_waiting_time / (float)process_count;
    if (average_waiting_time == (int)average_waiting_time)
    {
        // Print as integer
        printf("%d\n", (int)average_waiting_time);
    }
    else
    {
        // Print with decimal places
        printf("%.1f\n", average_waiting_time);
    }

    if (average_turnaround_time == (int)average_turnaround_time)
    {
        // Print as integer
        printf("%d\n", (int)average_turnaround_time);
    }
    else
    {
        // Print with decimal places
        printf("%.1f\n", average_turnaround_time);
    }

    return 0;
}
void checkNewArrival(int *current_time)
{
    while (arrivalQueue != NULL && arrivalQueue->process->arrivalTime <= *current_time)
    {
        // insert the process into the ready queue
        ReadyQueue *insertedReadyQueue = createReadyQueue();
        insertedReadyQueue->process = arrivalQueue->process;
        readyQueue = insertReadyQueue(readyQueue, insertedReadyQueue);
        arrivalQueue = arrivalQueue->next;
        if (arrivalQueue == NULL)
        {
            break;
        }
    }
}

void executeProcess(Process *process, int *current_time)
{
    int quantum = process->type == 2 ? 80 : 120;
    int quantumTime = 0;

    if (process->type == 0)
    {
        // if the process is platinum execute all instructions
        for (int i = process->lastInstructionIndex; i < process->numInstructions; i++)
        {

            process->burstTime += process->instructionStream[i];
            *current_time += process->instructionStream[i];
        }

        process->completionTime = *current_time;
        // printf("Completed Process name: %s, completion_time: %d\n", process->name, *current_time);
    }
    else
    {
        while (quantumTime < quantum)
        {

            process->burstTime += process->instructionStream[process->lastInstructionIndex];
            *current_time += process->instructionStream[process->lastInstructionIndex];
            quantumTime += process->instructionStream[process->lastInstructionIndex];

            process->lastInstructionIndex++;

            // check if there is a new arrival

            checkNewArrival(current_time);
            // check if the process finished

            if (process->lastInstructionIndex == process->numInstructions)
            {

                process->completionTime = *current_time;
                // printf("Completed Process name: %s, completion_time: %d\n", process->name, process->completionTime);
                break;
            }
            if (readyQueue != NULL && (readyQueue->process->priority > process->priority || readyQueue->process->type == 0))
            {
                // if the new arrival has higher priority then stop executing the current process

                break;
            }
        }
        process->totalQuantum += 1;
        process->arrivalTime = *current_time;
    }
}
