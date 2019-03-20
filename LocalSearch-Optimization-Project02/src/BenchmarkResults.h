//
// Created by Shane on 4/18/2018.
//

#ifndef OPTIMIZATION_BENCHMARKCLIENT_H
#define OPTIMIZATION_BENCHMARKCLIENT_H

#include "stdafx.h"
#include "ConstraintsFile.h"
#include "Benchmark.h"
#include "LocalSearch.h"
using namespace std;

#define DOUBLE_MAX numeric_limits<double>::max()

struct Variables
{
    Variables() = default;
    int simulations{}, random_walk_itr{}, itr_local_search{};
    double delta{};
    Dimension dimension{};
    int selected_fn{};

    // used for thread in the compute function
    Benchmark fitness;
    const char *name{};
};

void run();
void createFile(Variables);
string toString(double);
bool compute(Benchmark::Fitness, Benchmark *, LocalSearch *, Variables, Range, const string &);

#endif //OPTIMIZATION_BENCHMARKCLIENT_H
