#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <stdbool.h>

typedef struct {
    int num_page_fault;
    int num_disk_write;
    int num_extra_interrupt;
} SimulateResult;

SimulateResult* NewSimulateResult();
void FreeSimulateResult(SimulateResult* result);
void PrintSimulateResult(const SimulateResult* result);

#define EMPTY_PAGE (-1)
typedef struct {
    int* memory_ar;
    bool* reference_bit_ar;
    bool* dirty_bit_ar;
    int memory_size;
} PRSimulator;

PRSimulator* NewPRSimulator(int memory_size);
void FreePRSimulator(PRSimulator* simulator);

#define PAGE_NOT_FOUND (-1)
int FindPage(const PRSimulator* self, const int page);

#endif  // SIMULATOR_H_
