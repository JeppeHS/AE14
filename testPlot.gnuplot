set title "Test plot"
set xlabel "Time in nanoseconds"
set ylabel "Array size"

set terminal png
set output "testplot.png"


plot "test.csv" using 1:2 with lines title "Test1", "test.csv" using 1:2 with points title "Test2"

