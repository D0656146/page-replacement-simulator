#include "replace_algos.h"

#include <math.h>

SimulateResult* FirstInFirstOut_MA(PRSimulator* simulator,
                                   const MemoryReferences* references) {
    SimulateResult* result = NewSimulateResult();
    int queue_tail = 0;
    for (int c = 0; c < references->length; c++) {
        int page_location = FindPage(simulator, references->reference_string[c]);
        if (page_location == PAGE_NOT_FOUND) {
            result->num_page_fault++;
            if (simulator->dirty_bit_ar[queue_tail]) {
                simulator->dirty_bit_ar[queue_tail] = false;
                result->num_disk_write++;
                // printf("[fifo] disk write \n");
            }
            simulator->memory_ar[queue_tail] = references->reference_string[c];
            page_location = queue_tail;

            queue_tail++;
            if (queue_tail == simulator->memory_size) {
                queue_tail = 0;
            }
            // printf("[fifo] page fault \n");
        }
        simulator->reference_bit_ar[page_location] = true;
        if (references->modify_string[c]) {
            simulator->dirty_bit_ar[page_location] = true;
        }
    }
    // write back
    for (int c = 0; c < simulator->memory_size; c++) {
        if (simulator->dirty_bit_ar[c]) {
            result->num_disk_write++;
        }
    }
    return result;
}

SimulateResult* AdditionalReferenceBits_MA(PRSimulator* simulator,
                                           const MemoryReferences* references,
                                           const int num_arbits,
                                           const int interrupt_period) {
    SimulateResult* result = NewSimulateResult();
    int queue_tail = 0;
    int reference_value[simulator->memory_size];
    for (int c_mem = 0; c_mem < simulator->memory_size; c_mem++) {
        reference_value[c_mem] = 0;
    }
    for (int c = 0; c < references->length; c++) {
        // +1 for not interrupting first round
        if ((c + 1) % interrupt_period == 0) {
            result->num_extra_interrupt++;
            for (int c_mem = 0; c_mem < simulator->memory_size; c_mem++) {
                // count reference value
                reference_value[c_mem] /= 2;
                reference_value[c_mem] += simulator->reference_bit_ar[c_mem] * pow(2, num_arbits - 1);
            }
            // printf("[arb] interrupt \n");
        }
        int page_location = FindPage(simulator, references->reference_string[c]);
        if (page_location == PAGE_NOT_FOUND) {
            // find victim
            int victim_value = pow(2, num_arbits);
            for (int c_q = 0; c_q < simulator->memory_size; c_q++) {
                if (victim_value > reference_value[(c_q + queue_tail) % simulator->memory_size]) {
                    victim_value = reference_value[(c_q + queue_tail) % simulator->memory_size];
                    page_location = (c_q + queue_tail) % simulator->memory_size;
                }
            }
            queue_tail = page_location;
            result->num_page_fault++;
            if (simulator->dirty_bit_ar[queue_tail]) {
                simulator->dirty_bit_ar[queue_tail] = false;
                result->num_disk_write++;
                // printf("[arb] disk write \n");
            }
            simulator->memory_ar[queue_tail] = references->reference_string[c];
            queue_tail++;
            if (queue_tail == simulator->memory_size) {
                queue_tail = 0;
            }
            // printf("[arb] page fault \n");
        }
        simulator->reference_bit_ar[page_location] = true;
        if (references->modify_string[c]) {
            simulator->dirty_bit_ar[page_location] = true;
        }
    }
    // write back
    for (int c = 0; c < simulator->memory_size; c++) {
        if (simulator->dirty_bit_ar[c]) {
            result->num_disk_write++;
        }
    }
    return result;
}

SimulateResult* EnhancedSecondChance_MA(PRSimulator* simulator,
                                        const MemoryReferences* references) {
    SimulateResult* result = NewSimulateResult();
    int queue_tail = 0;
    for (int c = 0; c < references->length; c++) {
        int page_location = FindPage(simulator, references->reference_string[c]);
        if (page_location == PAGE_NOT_FOUND) {
        SearchZeroZero:
            // search for (ref, dirty) = (0, 0)
            for (int c_q = 0; c_q < simulator->memory_size; c_q++) {
                if (!simulator->reference_bit_ar[(c_q + queue_tail) % simulator->memory_size] &&
                    !simulator->dirty_bit_ar[(c_q + queue_tail) % simulator->memory_size]) {
                    page_location = (c_q + queue_tail) % simulator->memory_size;
                    break;
                }
            }
            if (page_location == PAGE_NOT_FOUND) {
                // search for (ref, dirty) = (0, 1)
                for (int c_q = 0; c_q < simulator->memory_size; c_q++) {
                    if (simulator->reference_bit_ar[(c_q + queue_tail) % simulator->memory_size]) {
                        simulator->reference_bit_ar[(c_q + queue_tail) % simulator->memory_size] = false;
                    } else {
                        page_location = (c_q + queue_tail) % simulator->memory_size;
                        break;
                    }
                }
            }
            if (page_location == PAGE_NOT_FOUND) {
                goto SearchZeroZero;
            }
            queue_tail = page_location;
            result->num_page_fault++;
            if (simulator->dirty_bit_ar[queue_tail]) {
                simulator->dirty_bit_ar[queue_tail] = false;
                result->num_disk_write++;
                // printf("[esc] disk write \n");
            }
            simulator->memory_ar[queue_tail] = references->reference_string[c];
            queue_tail++;
            if (queue_tail == simulator->memory_size) {
                queue_tail = 0;
            }
            // printf("[esc] page fault \n");
        }
        simulator->reference_bit_ar[page_location] = true;
        if (references->modify_string[c]) {
            simulator->dirty_bit_ar[page_location] = true;
        }
    }
    // write back
    for (int c = 0; c < simulator->memory_size; c++) {
        if (simulator->dirty_bit_ar[c]) {
            result->num_disk_write++;
        }
    }
    return result;
}

SimulateResult* Farest_MA(PRSimulator* simulator,
                          const MemoryReferences* references) {
    SimulateResult* result = NewSimulateResult();
    for (int c = 0; c < references->length; c++) {
        int page_location = FindPage(simulator, references->reference_string[c]);
        if (page_location == PAGE_NOT_FOUND) {
            // find the farest page
            int farest_distance = 0;
            for (int c_mem = 0; c_mem < simulator->memory_size; c_mem++) {
                int distance = simulator->memory_ar[c_mem] - references->reference_string[c];
                if (distance < 0) {
                    distance = 0 - distance;
                }
                if (distance > farest_distance) {
                    farest_distance = distance;
                    page_location = c_mem;
                }
            }
            result->num_page_fault++;
            if (simulator->dirty_bit_ar[page_location]) {
                simulator->dirty_bit_ar[page_location] = false;
                result->num_disk_write++;
                // printf("[far] disk write \n");
            }
            simulator->memory_ar[page_location] = references->reference_string[c];
            page_location = page_location;
            // printf("[far] page fault \n");
        }
        simulator->reference_bit_ar[page_location] = true;
        if (references->modify_string[c]) {
            simulator->dirty_bit_ar[page_location] = true;
        }
    }
    // write back
    for (int c = 0; c < simulator->memory_size; c++) {
        if (simulator->dirty_bit_ar[c]) {
            result->num_disk_write++;
        }
    }
    return result;
}
