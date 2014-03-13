set datafile separator ","

set terminal png
set xlabel "Array size"
set logscale x

nAlgos = 3

set key autotitle columnhead

set output "Branch misses.png"
set ylabel "#Branch-misses"
set logscale y
plot for [i=2:nAlgos+1] 'Branch misses.csv' using 1:i with linespoints

set output "Task clock.png"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'Task clock.csv' using 1:i with linespoints

set output "CPU clock.png"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'CPU clock.csv' using 1:i with linespoints

set output "Instructions.png"
set ylabel "#Instructions"
plot for [i=2:nAlgos+1] 'Instructions.csv' using 1:i with linespoints

set output "Cache refs.png"
set ylabel "#cache refs"
plot for [i=2:nAlgos+1] 'Cache refs.csv' using 1:i with linespoints

set output "Cache misses.png"
set ylabel "#cache misses"
set nologscale y
plot for [i=2:nAlgos+1] 'Cache misses.csv' using 1:i with linespoints

set output "Cpu cycles.png"
set ylabel "#Cpu cycles"
plot for [i=2:nAlgos+1] 'Cpu cycles.csv' using 1:i with linespoints

set output "Page faults.png"
set ylabel "#page faults"
plot for [i=2:nAlgos+1] 'Page faults.csv' using 1:i with linespoints

set output "Page faults Non-IO.png"
set ylabel "#page faults Non-IO"
plot for [i=2:nAlgos+1] 'Page faults Non-IO.csv' using 1:i with linespoints

set output "Page faults IO.png"
set ylabel "#page faults IO"
set nologscale y
plot for [i=2:nAlgos+1] 'Page faults IO.csv' using 1:i with linespoints

#set output "Cache_L1D accesses.png"
#set ylabel "#Cache_L1D accesses"
#plot for [i=2:nAlgos+1] 'Cache_L1D accesses.csv' using 1:i with linespoints

#set output "Cache_L1D misses.png"
#set ylabel "#Cache_L1D misses"
#plot for [i=2:nAlgos+1] 'Cache_L1D misses.csv' using 1:i with linespoints

#set output "Time.png"
#set ylabel "Time in ns"
#plot for [i=2:nAlgos+1] 'Time in ns.csv' using 1:i with linespoints

