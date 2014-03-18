set datafile separator ","

set terminal pdf
set xlabel "Array size"
set logscale x
set key top left

nAlgos = 4

set key autotitle columnhead

set output "Branch misses.pdf"
set ylabel "#Branch-misses"
set logscale y
plot for [i=2:nAlgos+1] 'Branch misses.csv' using 1:i with linespoints

set output "Task clock.pdf"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'Task clock.csv' using 1:i with linespoints

set output "CPU clock.pdf"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'CPU clock.csv' using 1:i with linespoints

set output "Instructions.pdf"
set ylabel "#Instructions"
plot for [i=2:nAlgos+1] 'Instructions.csv' using 1:i with linespoints

#set nologscale y

set output "Cache refs.pdf"
set ylabel "#cache refs"
plot for [i=2:nAlgos+1] 'Cache refs.csv' using 1:i with linespoints

set output "Cache misses.pdf"
set ylabel "#cache misses"
plot for [i=2:nAlgos+1] 'Cache misses.csv' using 1:i with linespoints

set output "Cpu cycles.pdf"
set ylabel "#Cpu cycles"
set logscale y
plot for [i=2:nAlgos+1] 'Cpu cycles.csv' using 1:i with linespoints

set output "Page faults.pdf"
set ylabel "#page faults"
plot for [i=2:nAlgos+1] 'Page faults.csv' using 1:i with linespoints

set output "Page faults Non-IO.pdf"
set ylabel "#page faults Non-IO"
plot for [i=2:nAlgos+1] 'Page faults Non-IO.csv' using 1:i with linespoints

set output "Page faults IO.pdf"
set ylabel "#page faults IO"
set nologscale y
plot for [i=2:nAlgos+1] 'Page faults IO.csv' using 1:i with linespoints

#set output "Cache_L1D accesses.pdf"
#set ylabel "#Cache_L1D accesses"
#plot for [i=2:nAlgos+1] 'Cache_L1D accesses.csv' using 1:i with linespoints

#set output "Cache_L1D misses.pdf"
#set ylabel "#Cache_L1D misses"
#plot for [i=2:nAlgos+1] 'Cache_L1D misses.csv' using 1:i with linespoints

#set output "Time.pdf"
#set ylabel "Time in ns"
#plot for [i=2:nAlgos+1] 'Time in ns.csv' using 1:i with linespoints

