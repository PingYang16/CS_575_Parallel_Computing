#!/bin/csh

foreach t ( 1 2 4 8 )
  foreach n ( 100 500 1000 2500 5000 7500 10000 15000 )
        g++ -O3 project2.cpp  -DNUMT=$t -DNUMNODES=$n  -o project2  -lm  -fopenmp
    ./project2
  end
end