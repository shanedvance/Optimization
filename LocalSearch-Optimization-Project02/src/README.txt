To compile from command line:
-------------------------------
	g++ -std=c++11 -U__STRICT_ANSI__ main.cpp stdafx.cpp BenchmarkResults.cpp Benchmark.cpp ConstraintsFile.cpp LocalSearch.cpp -o LSFitness

To execute the compiled program:
--------------------------------
	./LSFitness


**NOTE: 	1. the "Shekel's_Foxhole_Data.txt" is necessary for this program 
		   to work as the function requires it.
		2. the "constraints.csv" file is necessary as this contains all the necessary
		   information for each function; such as its range, iterations, delta, etc.
		3. after executing the compiled program the results will be saved in
		   the file name of your choice (remember to put *.csv since it's comma-delimited).


--------------------------------------
---- Machine Specs.-------------------
--------------------------------------

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6GB RAM
2. Windows 10 Pro, 64-bit
3. JetBrains CLion 2017.2.3 IDE, Cygwin64 Debugger