To compile from command line:
-------------------------------
	Option 1:
	==========
	g++ -Wall -w -O2 -std=c++11 main.cpp FSS.cpp FSSB.cpp FSSNW.cpp NEH.cpp -o NEH
	
	Option 2:
	==========
	make

To execute the compiled program:
--------------------------------
	Option 1 (for single file):
	========================================
	./NEH [FILENAME]
		
		Example:
		=========
		./NEH Taillard_Series/1.txt		

	Option 2 (for many files in directory):
	========================================
	./Run.sh [FILEPATH] [OUTPUT_FILENAME]
		
		Example:
		=========
		./Run.sh Taillard_Series Results.csv


**NOTES:	1. To make Run.sh have executable privileges type this command "chmod +x Run.sh" (exclusive of the quotation marks)
		2. NEH heuristic algorithm analysis for Flow-shop Scheduling (FSS), Flow-shop Scheduling with Blocking (FSSB) and Flow-shop 
		   Scheduling with No-Wait (FSSNW) of the Taillard data sets. This will find the best job schedule given the NEH algorithm for 
		   Johnson's algorithm implemenation. It takes the first two jobs and finds the best solution of the partial permuation. This 
		   continues by adding one job to the sequence each time, while L>3.

--------------------------------------
---- Machine Specs.-------------------
--------------------------------------

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6 GB RAM
2. Windows 10 Pro, 64-bit
3. JetBrains CLion 2018.1.2 IDE, Cygwin64 Debugger

**NOTE:	although this was coded on a Windows machine it was tested on a Linux virtual environment via Ubuntu for Windows.
