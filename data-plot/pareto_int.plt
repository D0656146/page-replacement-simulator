set xlabel "frame size"
set ylabel "interrupts"
set title "pareto reference string"
plot "pareto_fifo_int.dat" with linespoints title "FIFO", \
     "pareto_arb_int.dat" with linespoints title "ARB", \
     "pareto_esc_int.dat" with linespoints title "ESC", \
     "pareto_far_int.dat" with linespoints title "FAR"
