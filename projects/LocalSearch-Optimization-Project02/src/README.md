# Local Search Algorithms

## Compile

```bash
> g++ -std=c++11 -U__STRICT_ANSI__ main.cpp stdafx.cpp BenchmarkResults.cpp Benchmark.cpp ConstraintsFile.cpp LocalSearch.cpp -o LSFitness
```

## Run

```bash
> ./LSFitness
```

## Note

1. The `Shekel's_Foxhole_Data.txt` is necessary for this program to work as the function requires it.
1. The `constraints.csv` file is necessary as this contains all the necessary information for each function; such as its range, iterations, delta, etc.
1. After executing the compiled program the results will be saved in the file name of your choice (remember to put `*.csv` since it's comma-delimited).

---

## Machine specs

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6 GB RAM
1. Windows 10 Pro, 64-bit
1. JetBrains CLion 2017.2.3 IDE, Cygwin64 Debugger

> NOTE: although this was coded on a Windows machine it was tested on a Linux virtual environment via Cygwin64
