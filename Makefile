all:
    gcc src/*.c -o dist/os_hw -g -Wall -static-libgcc -std=c11

clean:
    rm -rf data-plot/*.dat dist/*
