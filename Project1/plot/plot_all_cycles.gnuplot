data_dir = "../data/"

set title "Clock cycles vs array size"

set ylabel "Array size"
set xlabel "Clock cycles"

set terminal png
set output "out/clock_cycles.png"

plot data_dir."LinSearch.csv" using 1:2 with linespoints title "Linear search", \
data_dir."Inorder.csv" using 1:2 with linespoints title "Inorder", \
data_dir."BFS.csv" using 1:2 with linespoints title "BFS", \
data_dir."DFS.csv" using 1:2 with linespoints title "DFS"

