#!/bin/bash

for references_method in "random" "locality" "pareto"; do
    for algorithm in "fifo" "arb" "esc" "far"; do
        for num_frames in 20 40 60 80 100; do
            echo "- - -"
            ./dist/os_hw.exe ${references_method} ${algorithm} ${num_frames}
        done
    done
done
