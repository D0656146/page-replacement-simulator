set xlabel "frame size"
set ylabel "interrupts"
set title "locality reference string"
plot "locality_fifo_int.dat" with linespoints title "FIFO", \
     "locality_arb_int.dat" with linespoints title "ARB", \
     "locality_esc_int.dat" with linespoints title "ESC", \
     "locality_far_int.dat" with linespoints title "FAR"
