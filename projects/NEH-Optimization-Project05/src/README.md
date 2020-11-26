# NEH Heuristic Algorithms

## Compile

### Option 1

```bash
> g++ -Wall -w -O2 -std=c++11 main.cpp FSS.cpp FSSB.cpp FSSNW.cpp NEH.cpp -o NEH
```

### Option 2

```bash
> make
```

## Run

### Option 1

> If there's a single file

```bash
> ./NEH <file_name>
```

#### Example

```bash
> ./NEH Taillard_Series/1.txt
```

### Option 2

> If there are many file in directory

```bash
> ./Run.sh <file_path> <output_filename>
```

#### Example

```bash
> ./Run.sh Taillard_Series Results.csv
```

## Notes

1. To make `Run.sh` have executable privileges type this command `chmod +x Run.sh`.
1. NEH heuristic algorithm analysis for Flow-shop Scheduling (FSS), Flow-shop Scheduling with Blocking (FSSB) and Flow-shop.
1. Scheduling with No-Wait (FSSNW) of the Taillard data sets. This will find the best job schedule given the NEH algorithm for Johnson's algorithm implementation. It takes the first two jobs and finds the best solution of the partial permutation. This continues by adding one job to the sequence each time, while L>3.

---

## Machine specs

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6 GB RAM
1. Windows 10 Pro, 64-bit
1. JetBrains CLion 2018.1.2 IDE, Cygwin64 Debugger

> NOTE: although this was coded on a Windows machine it was tested on a Linux virtual environment via Cygwin64
