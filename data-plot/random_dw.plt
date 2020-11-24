set xlabel "frame size"
set ylabel "disk writes"
set title "random reference string"
plot "random_fifo_dw.dat" with linespoints title "FIFO", \
     "random_arb_dw.dat" with linespoints title "ARB", \
     "random_esc_dw.dat" with linespoints title "ESC", \
     "random_far_dw.dat" with linespoints title "FAR"
