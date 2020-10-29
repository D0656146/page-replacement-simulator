#include "replace_utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

MemoryReferences* NewMemoryReferences(const int length, const double modify_rate) {
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

void PrintMemoryReferences(const MemoryReferences* reference) {
    for (int c = 0; c < reference->length; c++) {
        printf("(%d, %d)", reference->reference_string[c], reference->modify_string[c]);
    }
    printf("\n");
}

void RandomMemoryReferences_RP(MemoryReferences* reference,
                               const int num_all_page,
                               const int shortest_period,
                               const int longest_period) {
    int c_ref = 0;
    while (true) {
        int period = shortest_period + rand() % (longest_period - shortest_period + 1);
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
                                 const int num_all_page,
                                 const int function_length,
                                 const int shortest_period,
                                 const int longest_period) {
    int period, start_point;
    for (int c_ref = 0; c_ref < reference->length; c_ref++) {
        if (c_ref % function_length == 0) {
            period = shortest_period + rand() % (longest_period - shortest_period + 1);
            start_point = rand() % (num_all_page - period + 1);
        }
        reference->reference_string[c_ref] = start_point + rand() % period;
    }
}

int RouletteWheels(const double* weights, const int num_candidates) {
    double sum_weight = 0.0;
    for (int c = 0; c < num_candidates; c++) {
        sum_weight += weights[c];
    }
    double rand_weight = sum_weight * rand() / RAND_MAX;
    for (int c = 0; c < num_candidates; c++) {
        rand_weight -= weights[c];
        if (rand_weight <= 0) {
            return c;
        }
    }
    return num_candidates - 1;
}

void ParetoMemoryReferences_RP(MemoryReferences* reference,
                               const int num_all_page,
                               const double shape_value) {
    double mapping[num_all_page];
    for (int c = 0; c < num_all_page; c++) {
        mapping[c] = c;
    }
    // shuffle
    for (int c = 0; c < num_all_page; c++) {
        int target = rand() % num_all_page;
        int temp = mapping[c];
        mapping[c] = mapping[target];
        mapping[target] = temp;
    }

    double probabilities[num_all_page];
    for (int c = 0; c < num_all_page; c++) {
        probabilities[c] = shape_value / pow(c + 1.5, shape_value + 1);
    }
    for (int c = 0; c < reference->length; c++) {
        reference->reference_string[c] = mapping[RouletteWheels(probabilities, num_all_page)];
    }
}
