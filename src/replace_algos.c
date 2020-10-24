#include "replace_algos.h"

SimulateResult* FirstInFirstOut_MA(PRSimulator* simulator, MemoryReferences* references) {
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
                                           MemoryReferences* references,
                                           int num_arbits,
                                           int interrupt_period) {
    SimulateResult* result = NewSimulateResult();
    int queue_tail = 0;
    int bit_queue_tail = 0;
    bool arbits[simulator->memory_size][num_arbits];
    int reference_times[simulator->memory_size];
    for (int c_mem = 0; c_mem < simulator->memory_size; c_mem++) {
        for (int c_bit = 0; c_bit < num_arbits; c_bit++) {
            arbits[c_mem][c_bit] = false;
        }
        reference_times[c_mem] = 0;
    }
    for (int c = 0; c < references->length; c++) {
        // +1 for not interrupting first round
        if ((c + 1) % interrupt_period == 0) {
            result->num_extra_interrupt++;
            for (int c_mem = 0; c_mem < simulator->memory_size; c_mem++) {
                reference_times[c_mem] -= arbits[c_mem][bit_queue_tail];
                reference_times[c_mem] += simulator->reference_bit_ar[c_mem];

                arbits[c_mem][bit_queue_tail] = simulator->reference_bit_ar[c_mem];

                bit_queue_tail++;
                if (bit_queue_tail == num_arbits) {
                    bit_queue_tail = 0;
                }
            }
            // printf("[arb] interrupt \n");
        }
        int page_location = FindPage(simulator, references->reference_string[c]);
        if (page_location == PAGE_NOT_FOUND) {
            int victim_value = num_arbits;
            for (int c_q = 0; c_q < simulator->memory_size; c_q++) {
                if (victim_value > reference_times[(c_q + queue_tail) % simulator->memory_size]) {
                    victim_value = reference_times[(c_q + queue_tail) % simulator->memory_size];
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

SimulateResult* EnhancedSecondChance_MA(PRSimulator* simulator, MemoryReferences* references) {
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
