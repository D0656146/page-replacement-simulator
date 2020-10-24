#include "simulator.h"

#include <stdlib.h>
#include <stdio.h>

SimulateResult* NewSimulateResult() {
    SimulateResult* instance = malloc(sizeof(SimulateResult));
    instance->num_page_fault = 0;
    instance->num_disk_write = 0;
    instance->num_extra_interrupt = 0;
    return instance;
}

void FreeSimulateResult(SimulateResult* result) {
    free(result);
}

void PrintSimulateResult(SimulateResult* result){
    printf("page faults: %d\n", result->num_page_fault);
    printf("disk writes: %d\n", result->num_disk_write);
    printf("interrupts: %d\n", result->num_extra_interrupt);
}

PRSimulator* NewPRSimulator(int memory_size) {
    PRSimulator* instance = malloc(sizeof(PRSimulator));
    instance->memory_size = memory_size;
    instance->reference_bit_ar = calloc(memory_size, sizeof(bool));
    instance->dirty_bit_ar = calloc(memory_size, sizeof(bool));
    instance->memory_ar = malloc(memory_size * sizeof(int));
    for (int c = 0; c < memory_size; c++) {
            instance->memory_ar[c] = EMPTY_PAGE;
    }
    return instance;
}

void FreePRSimulator(PRSimulator* simulator) {
    free(simulator->memory_ar);
    free(simulator->reference_bit_ar);
    free(simulator->dirty_bit_ar);
    free(simulator);
};

int FindPage(PRSimulator* self, int page) {
    for (int c = 0; c < self->memory_size; c++) {
        if (self->memory_ar[c] == page) {
            return c;
        }
    }
    return -1;
}
