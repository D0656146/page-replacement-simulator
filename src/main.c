#include <stdlib.h>
#include <time.h>

#include "replace_algos.h"
#include "replace_utils.h"
#include "simulator.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    PRSimulator* simulator = NewPRSimulator(799);
    MemoryReferences* references = NewMemoryReferences(200000, 0.2);

    RandomMemoryReferences_RP(references, 800, 25);
    // LocalityMemoryReferences_RP(references, 800, 100, 40, 53);

    // SimulateResult* result = FirstInFirstOut_MA(simulator, references);
    // SimulateResult* result = AdditionalReferenceBits_MA(simulator, references, 8, 100);
    SimulateResult* result = EnhancedSecondChance_MA(simulator, references);

    PrintSimulateResult(result);
}
