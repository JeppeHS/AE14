#set datafile separator ","
#set title "Test plot"
#set xlabel "Time in nanoseconds"
#set ylabel "Array size"

set terminal png
set output "testplot.png"

#datafile = 'bla'
#firstrow = system('< head -1 '.datafile)
#set xlabel word(firstrow, 1)
#set ylabel word(firstrow, 3)
#plot 'bla' using 1:2:4 with labels
plot 'bla' using 1:2 with lines title columnhead(1) set ylabel columnhead(2)


#plot "BFS.csv" using 1:3 every ::1 with lines title "Test1"
#plot "BFS.csv" using 1:3 every ::1 with points title "Test2"

