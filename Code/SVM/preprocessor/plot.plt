set multiplot layout 2,3

plot "temp.A" using 2:41 title "A"
plot "temp.B" using 2:41 title "B"
plot "temp.C" using 2:41 title "C"
plot "temp.D" using 2:41 title "D"
plot "temp.E" using 2:41 title "E"
plot "temp.A" using 2:41 title "A", \
     "temp.B" using 2:41 title "B", \
     "temp.C" using 2:41 title "C", \
     "temp.D" using 2:41 title "D", \
     "temp.E" using 2:41 title "E", \

unset multiplot

