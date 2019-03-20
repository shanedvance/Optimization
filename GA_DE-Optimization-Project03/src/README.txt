To compile from command line:
-------------------------------
	Option 1:
	==========
	g++ -std=c++11 -U__STRICT_ANSI__ main.cpp stdafx.cpp BenchmarkResults.cpp Benchmark.cpp ConstraintsFile.cpp GeneticAlgorithms.cpp -o GAFitness
	
	Option 2:
	==========
	make

To execute the compiled program:
--------------------------------
	./GAFitness


**NOTES: 	1. the "Shekel's_Foxhole_Data.txt" is necessary for this program 
		   to work as the fitness function requires it.
		2. the "constraints.csv" file is necessary as this contains all the necessary
		   information for each function; such as its range, iterations, strategy, etc.
		3. the user has the option to change the strategy for the differential evolution (option 2) 
		   function. These strategies can be updated via the "contraints.csv" file. Strategies 1-5 are 
		   exponential while strategies 6-10 are binomial mutation and crossover.
		4. the user can refresh the contraints file by selecting option 3.
		5. after executing the compiled program the results will be saved in
		   the file name of your choice (remember to put *.csv since it's comma-delimited).

--------------------------------------
---- Machine Specs.-------------------
--------------------------------------

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6 GB RAM
2. Windows 10 Pro, 64-bit
3. JetBrains CLion 2018.1.2 IDE, Cygwin64 Debugger

**NOTE:	although this was coded on a Windows machine in was tested on a Linux virtual environment via Cygwin64.