#!/bin/csh

foreach t ( 1 2 4 8 12 16 20 24 32 )
  foreach n ( 1 10 100 1000 10000 100000 500000 1000000 )
        g++ -O3   project1.cpp  -DNUMT=$t -DNUMTRIALS=$n  -o project1  -lm  -fopenmp
    ./project1
  end
end
