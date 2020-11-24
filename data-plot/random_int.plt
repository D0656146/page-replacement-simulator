set xlabel "frame size"
set ylabel "interrupts"
set title "random reference string"
plot "random_fifo_int.dat" with linespoints title "FIFO", \
     "random_arb_int.dat" with linespoints title "ARB", \
     "random_esc_int.dat" with linespoints title "ESC", \
     "random_far_int.dat" with linespoints title "FAR"
