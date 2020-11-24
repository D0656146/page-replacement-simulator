#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "replace_algos.h"
#include "replace_utils.h"
#include "simulator.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    const char* references_method = argv[1];
    const char* algorithm = argv[2];
    const int num_frames = atoi(argv[3]);

    const int references_size = 200000;
    const double modify_rate = 0.3;
    const int num_pages = 800;
    const int random_shortest_period = 1;
    const int random_longest_period = 25;
    const int function_length = 100;
    const int locality_shortest_period = 32;
    const int locality_longest_period = 53;
    const double pareto_shape_value = log(5) / log(4);
    const int num_arbits = 8;
    const int interrupt_period = 10;
    const int runs = 10;

    SimulateResult* average = NewSimulateResult();
    for (int c = 0; c < runs; c++) {
        PRSimulator* simulator = NewPRSimulator(num_frames);
        MemoryReferences* references = NewMemoryReferences(references_size, modify_rate);
        SimulateResult* result;

        if (strcmp(references_method, "random") == 0) {
            RandomMemoryReferences_RP(references,
                                      num_pages,
                                      random_shortest_period,
                                      random_longest_period);
        } else if (strcmp(references_method, "locality") == 0) {
            LocalityMemoryReferences_RP(references,
                                        num_pages,
                                        function_length,
                                        locality_shortest_period,
                                        locality_longest_period);
        } else if (strcmp(references_method, "pareto") == 0) {
            ParetoMemoryReferences_RP(references, num_pages, pareto_shape_value);
        }

        if (strcmp(algorithm, "fifo") == 0) {
            result = FirstInFirstOut_MA(simulator, references);
        } else if (strcmp(algorithm, "arb") == 0) {
            result = AdditionalReferenceBits_MA(simulator, references, num_arbits, interrupt_period);
        } else if (strcmp(algorithm, "esc") == 0) {
            result = EnhancedSecondChance_MA(simulator, references);
        } else if (strcmp(algorithm, "far") == 0) {
            result = Farest_MA(simulator, references);
        }
        average->num_page_fault += result->num_page_fault;
        average->num_extra_interrupt += result->num_extra_interrupt;
        average->num_disk_write += result->num_disk_write;
    }
    average->num_page_fault /= runs;
    average->num_extra_interrupt /= runs;
    average->num_disk_write /= runs;

    printf("references method: %s\n", references_method);
    printf("algorithm: %s\n", algorithm);
    printf("number of frames: %d\n", num_frames);
    PrintSimulateResult(average);

    char filename[100];
    FILE* fptr;

    sprintf(filename, "data-plot/%s_%s_pf.dat", references_method, algorithm);
    while (!(fptr = fopen(filename, "a+"))) {
        fopen(filename, "w");
    }
    fprintf(fptr, "%d %d\n", num_frames, average->num_page_fault);
    fclose(fptr);

    sprintf(filename, "data-plot/%s_%s_int.dat", references_method, algorithm);
    while (!(fptr = fopen(filename, "a+"))) {
        fopen(filename, "w");
    }
    fprintf(fptr, "%d %d\n", num_frames, average->num_extra_interrupt);
    fclose(fptr);

    sprintf(filename, "data-plot/%s_%s_dw.dat", references_method, algorithm);
    while (!(fptr = fopen(filename, "a+"))) {
        fopen(filename, "w");
    }
    fprintf(fptr, "%d %d\n", num_frames, average->num_disk_write);
    fclose(fptr);

    return 0;
}
