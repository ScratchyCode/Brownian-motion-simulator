#!/usr/bin/gnuplot
set autoscale
set format y "%s"
set title "Brownian motion"
set key reverse Left outside
set grid
#set style data linespoints
plot "Path.dat" with lines

exit
