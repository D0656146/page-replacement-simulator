/**
 * Page Replacement Algorithms
 */

#ifndef REPLACE_ALGOS_H_
#define REPLACE_ALGOS_H_

#include <stdbool.h>
#include <stdio.h>

#include "replace_utils.h"
#include "simulator.h"

SimulateResult* FirstInFirstOut_MA(PRSimulator* simulator,
                                   const MemoryReferences* references);

SimulateResult* AdditionalReferenceBits_MA(PRSimulator* simulator,
                                           const MemoryReferences* references,
                                           const int num_arbits,
                                           const int interrupt_period);

SimulateResult* EnhancedSecondChance_MA(PRSimulator* simulator,
                                        const MemoryReferences* references);

SimulateResult* Farest_MA(PRSimulator* simulator,
                          const MemoryReferences* references);

#endif  // REPLACE_ALGOS_H_
