To compile from command line:
-------------------------------
	Option 1:
	==========
	g++ -Wall -w -std=c++11 -O2 -U__STRICT_ANSI__ main.cpp stdafx.cpp BenchmarkRun.cpp Benchmark.cpp ConstraintsFile.cpp SwarmIntelligence.cpp -o SIFitness
	
	Option 2:
	==========
	make

To execute the compiled program:
--------------------------------
	./SIFitness


**NOTES: 	1. this requires you to have a GNU compiler, usually installed on most devices,
		   as it uses OpenMP for threading. 
		   For more information visit: https://www.openmp.org/
		2. the "Shekel's_Foxhole_Data.txt" is necessary for this program 
		   to work as the fitness function requires it.
		3. the "constraints.csv" file is necessary as this contains all the necessary
		   parameters for each function; such as its range, iterations, parameters, etc.
		4. the user can refresh the contraints file by selecting option 3.
		5. after executing the compiled program the results will be saved in
		   the file name of your choice (remember to put *.csv since it is comma-delimited).

--------------------------------------
---- Machine Specs.-------------------
--------------------------------------

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6 GB RAM
2. Windows 10 Pro, 64-bit
3. JetBrains CLion 2018.1.2 IDE, Cygwin64 Debugger

**NOTE:	although this was coded on a Windows machine it was tested on a Linux virtual environment via Ubuntu for Windows.
