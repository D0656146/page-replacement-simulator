#include "replace_utils.h"

#include <stdio.h>
#include <stdlib.h>

MemoryReferences* NewMemoryReferences(int length, double modify_rate) {
    MemoryReferences* instance = malloc(sizeof(MemoryReferences));
    instance->length = length;
    instance->reference_string = malloc(length * sizeof(int));
    instance->modify_string = calloc(length, sizeof(bool));
    for (int c = 0; c < length; c++) {
        if ((double)rand() / RAND_MAX < modify_rate) {
            instance->modify_string[c] = true;
        }
    }
    return instance;
}

void FreeMemoryReferences(MemoryReferences* reference) {
    free(reference->reference_string);
    free(reference->modify_string);
    free(reference);
}

void PrintMemoryReferences(MemoryReferences* reference) {
    for (int c = 0; c < reference->length; c++) {
        printf("(%d, %d)", reference->reference_string[c], reference->modify_string[c]);
    }
    printf("\n");
}

void RandomMemoryReferences_RP(MemoryReferences* reference,
                               int num_all_page,
                               int longest_period) {
    int c_ref = 0;
    while (true) {
        int period = rand() % longest_period + 1;
        int start_point = rand() % (num_all_page - period + 1);
        for (int c = 0; c < period; c++) {
            reference->reference_string[c_ref] = start_point + c;
            c_ref++;
            if (c_ref == reference->length) {
                return;
            }
        }
    }
}

void LocalityMemoryReferences_RP(MemoryReferences* reference,
                                 int num_all_page,
                                 int function_length,
                                 int shortest_period,
                                 int longest_period) {
    int period, start_point;
    for (int c_ref = 0; c_ref < reference->length; c_ref++) {
        if (c_ref % function_length == 0) {
            period = rand() % (longest_period - shortest_period + 1);
            start_point = rand() % (num_all_page - period + 1);
        }
        reference->reference_string[c_ref] = start_point + rand() % period;
    }
}
