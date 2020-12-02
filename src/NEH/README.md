# NEH Heuristic Algorithms

## Compile

```bash
$ make
```

## Run

### Single file

```bash
$ ./NEH <file_name>
```

### Multiple files

```bash
$ chmod +x run.sh
$ ./run.sh <file_path> <output_filename>
```

## Notes

1. `NEH` heuristic algorithm analysis for Flow-shop Scheduling (`FSS`), Flow-shop Scheduling with Blocking (`FSSB`), and Flow-shop.
1. Scheduling with No-Wait (`FSSNW`) of the `Taillard` data sets.
    1. Finds the best job schedule given the `NEH` algorithm for the `Johnson's algorithm` implementation.
    1. It takes the first two jobs and finds the best solution of the partial permutation.
    1. The process continues by adding one job to the sequence each time, while `L>3`.

---

## Machine specs

1. Dell Alienware M14X R2, 2.4 GHz Intel Core i7 (3rd gen.), 6 GB RAM
1. Windows 10 Pro, 64-bit
1. JetBrains CLion 2018.1.2 IDE, Cygwin64 Debugger

> NOTE: Although this was coded on a Windows machine it was tested on a Linux virtual environment via Cygwin64.
