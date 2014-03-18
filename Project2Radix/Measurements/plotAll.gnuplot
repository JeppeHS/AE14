set datafile separator ","

set terminal pdf
set xlabel "Array size"
set logscale x
set logscale y

set key top left

#set linetype 2 lc rgb 'black'

nAlgos = 6

set key autotitle columnhead

set output "Branch_misses.pdf"
set ylabel "#Branch-misses"
plot for [i=2:nAlgos+1] 'Branch misses.csv' using 1:i with linespoints

set output "Task_clock.pdf"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'Task clock.csv' using 1:i with linespoints

set output "CPU_clock.pdf"
set ylabel "clock units??"
plot for [i=2:nAlgos+1] 'CPU clock.csv' using 1:i with linespoints

set output "Instructions.pdf"
set ylabel "#Instructions"
plot for [i=2:nAlgos+1] 'Instructions.csv' using 1:i with linespoints

set output "Cache_refs.pdf"
set ylabel "#cache refs"
plot for [i=2:nAlgos+1] 'Cache refs.csv' using 1:i with linespoints

set output "Cache_misses.pdf"
set ylabel "#cache misses"
plot for [i=2:nAlgos+1] 'Cache misses.csv' using 1:i with linespoints

set output "Cpu_cycles.pdf"
set ylabel "#Cpu cycles"
plot for [i=2:nAlgos+1] 'Cpu cycles.csv' using 1:i with linespoints

set nologscale y

set output "Page_faults.pdf"
set ylabel "#page faults"
plot for [i=2:nAlgos+1] 'Page faults.csv' using 1:i with linespoints

set output "Page_faults_Non-IO.pdf"
set ylabel "#page faults Non-IO"
plot for [i=2:nAlgos+1] 'Page faults Non-IO.csv' using 1:i with linespoints

set output "Page_faults_IO.pdf"
set ylabel "#page faults IO"
plot for [i=2:nAlgos+1] 'Page faults IO.csv' using 1:i with linespoints

#set output "Cache_L1D_accesses.pdf"
#set ylabel "#Cache_L1D accesses"
#plot for [i=2:nAlgos+1] 'Cache_L1D accesses.csv' using 1:i with linespoints

#set output "Cache_L1D_misses.pdf"
#set ylabel "#Cache_L1D misses"
#plot for [i=2:nAlgos+1] 'Cache_L1D misses.csv' using 1:i with linespoints

#set output "Time.pdf"
#set ylabel "Time in ns"
#plot for [i=2:nAlgos+1] 'Time in ns.csv' using 1:i with linespoints

