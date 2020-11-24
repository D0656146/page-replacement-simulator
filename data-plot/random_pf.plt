set xlabel "frame size"
set ylabel "page faults"
set title "random reference string"
plot "random_fifo_pf.dat" with linespoints title "FIFO", \
     "random_arb_pf.dat" with linespoints title "ARB", \
     "random_esc_pf.dat" with linespoints title "ESC", \
     "random_far_pf.dat" with linespoints title "FAR"
