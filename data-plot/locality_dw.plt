set xlabel "frame size"
set ylabel "disk writes"
set title "locality reference string"
plot "locality_fifo_dw.dat" with linespoints title "FIFO", \
     "locality_arb_dw.dat" with linespoints title "ARB", \
     "locality_esc_dw.dat" with linespoints title "ESC", \
     "locality_far_dw.dat" with linespoints title "FAR"
