// process.c

#include "process.h"

// function to create a process
Process *createProcess(char *name, int arrivalTime, int burstTime, int priority, char* type, int* instructionStream, int numInstructions){
    Process *process = malloc(sizeof(Process));
    process->name = name;
    process->arrivalTime = arrivalTime;
    process->firstarrivalTime = arrivalTime;
    process->burstTime = burstTime;
    process->priority = priority;
    process->typeName = type;
    process->numInstructions = numInstructions;
    // allocate memory for the instruction stream and copy the instructions
    process->instructionStream = malloc(sizeof(int) * numInstructions);
    // dont pass the reference copy values
    for (int i = 0; i < numInstructions; i++){
        process->instructionStream[i] = instructionStream[i];
    }
    
    if (strcmp(type, "PLATINUM") == 0){
        process->type = 0;
    }
    else if (strcmp(type, "GOLD") == 0){
        process->type = 1;
    }
    else if (strcmp(type, "SILVER") == 0){
        process->type = 2;
    }
    else{
        printf("type is: %s\n", type);
        printf("Invalid process type\n");
        exit(1);
    }
    process->burstTime = burstTime;
    
    return process;
}


Instruction *createInstruction(char *name, int duration){
    Instruction *instruction = malloc(sizeof(Instruction));
    instruction->name = name;
    instruction->duration = duration;
    
    return instruction;
}