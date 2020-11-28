# AI and Optimization Algorithms

## Compile

### Option 1

```bash
> g++ -Wall -w -std=c++11 -O2 -U__STRICT_ANSI__ main.cpp stdafx.cpp BenchmarkRun.cpp Benchmark.cpp ConstraintsFile.cpp SwarmIntelligence.cpp -o Optimization
```

### Option 2

```bash
> make
```

## Run

```bash
> ./Optimization
```

## Notes

1. Requires you to have a GNU compiler, usually installed on most devices, as it uses [OpenMP](https://www.openmp.org/) for threading.
1. The `Shekel's_Foxhole_Data.txt` is necessary for this program to work as the fitness function requires it.
1. The `constraints.csv` file is necessary as this contains all the necessary parameters for each function; such as its range, iterations, parameters, etc.
1. The user can refresh the constraints file by selecting option 3.
1. After executing the compiled program the results will be saved in
1. The file name of your choice (remember to put `*.csv` since it is comma-delimited).

---

## Machine specs

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6 GB RAM
1. Windows 10 Pro, 64-bit
1. JetBrains CLion 2018.1.2 IDE, Cygwin64 Debugger

> NOTE: Although this was coded on a Windows machine it was tested on a Linux virtual environment via Cygwin64.
