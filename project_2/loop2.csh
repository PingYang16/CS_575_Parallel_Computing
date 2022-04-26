#!/bin/csh

foreach t ( 1 2 4 8 )
	foreach n ( 100 200 400 800 1000 2000 4000 8000 )
		g++ -O3 -DNUMT=$t -DNUMNODES=$n project2.cpp -o project2 -lm -fopenmp
		./project2
	end
end
