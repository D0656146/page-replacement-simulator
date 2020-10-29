#ifndef REPLACE_UTILS_H_
#define REPLACE_UTILS_H_

#include <stdbool.h>

typedef struct {
    int* reference_string;
    bool* modify_string;
    int length;
} MemoryReferences;

MemoryReferences* NewMemoryReferences(const int length, const double modify_rate);
void FreeMemoryReferences(MemoryReferences* reference);
void PrintMemoryReferences(const MemoryReferences* reference);

void RandomMemoryReferences_RP(MemoryReferences* reference,
                               const int num_all_page,
                               const int shortest_period,
                               const int longest_period);

void LocalityMemoryReferences_RP(MemoryReferences* reference,
                                 const int num_all_page,
                                 const int function_length,
                                 const int shortest_period,
                                 const int longest_period);

void ParetoMemoryReferences_RP(MemoryReferences* reference,
                               const int num_all_page,
                               const double shape_value);

#endif  // REPLACE_UTILS_H_
