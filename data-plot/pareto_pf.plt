set xlabel "frame size"
set ylabel "page faults"
set title "pareto reference string"
plot "pareto_fifo_pf.dat" with linespoints title "FIFO", \
     "pareto_arb_pf.dat" with linespoints title "ARB", \
     "pareto_esc_pf.dat" with linespoints title "ESC", \
     "pareto_far_pf.dat" with linespoints title "FAR"
