set xlabel "frame size"
set ylabel "page faults"
set title "locality reference string"
plot "locality_fifo_pf.dat" with linespoints title "FIFO", \
     "locality_arb_pf.dat" with linespoints title "ARB", \
     "locality_esc_pf.dat" with linespoints title "ESC", \
     "locality_far_pf.dat" with linespoints title "FAR"
