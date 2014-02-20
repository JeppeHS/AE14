set datafile separator ","

set terminal png
set xlabel "Array size"

nAlgos = 2

set key autotitle columnhead

set output "Branch misses.png"
set ylabel "#Branch-misses"
plot for [i=2:nAlgos+1] 'Branch misses' using 1:i with lines

set output "Task clock.png"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'Task clock' using 1:i with lines

set output "CPU clock.png"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'CPU clock' using 1:i with lines

set output "Instructions.png"
set ylabel "#Instructions"
plot for [i=2:nAlgos+1] 'Instructions' using 1:i with lines

set output "Cache refs.png"
set ylabel "#cache refs"
plot for [i=2:nAlgos+1] 'Cache refs' using 1:i with lines

set output "Cache misses.png"
set ylabel "#cache misses"
plot for [i=2:nAlgos+1] 'Cache misses' using 1:i with lines
