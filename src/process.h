// process.h

#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTION 20

typedef struct Process {
    char *name;
    int firstarrivalTime;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int *instructionStream;
    int numInstructions;
    int lastInstructionIndex;
    int priority;
    int type;
    int timeQuantum;
    int totalQuantum;
    char *typeName;
} Process;



typedef struct Instruction {
    char *name;
    int duration;
} Instruction;

Process *createProcess(char *name, int arrivalTime, int burstTime, int priority, char* type, int* instructionStream, int numInstructions);
Instruction *createInstruction(char *name, int duration);

#endif // PROCESS_H