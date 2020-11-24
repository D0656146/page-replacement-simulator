set xlabel "frame size"
set ylabel "disk writes"
set title "pareto reference string"
plot "pareto_fifo_dw.dat" with linespoints title "FIFO", \
     "pareto_arb_dw.dat" with linespoints title "ARB", \
     "pareto_esc_dw.dat" with linespoints title "ESC", \
     "pareto_far_dw.dat" with linespoints title "FAR"
