#ifndef REPLACE_UTILS_H_
#define REPLACE_UTILS_H_

#include <stdbool.h>

typedef struct {
    int* reference_string;
    bool* modify_string;
    int length;
} MemoryReferences;

MemoryReferences* NewMemoryReferences(int length, double modify_rate);
void FreeMemoryReferences(MemoryReferences* reference);
void PrintMemoryReferences(MemoryReferences* reference);

void RandomMemoryReferences_RP(MemoryReferences* reference,
                               int num_all_page,
                               int longest_period);

void LocalityMemoryReferences_RP(MemoryReferences* reference,
                                 int num_all_page,
                                 int function_length,
                                 int shortest_period,
                                 int longest_period);

#endif  // REPLACE_UTILS_H_
