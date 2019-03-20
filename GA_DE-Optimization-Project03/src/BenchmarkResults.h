//
// Created by Shane on 4/18/2018.
//

#ifndef OPTIMIZATION_BENCHMARKCLIENT_H
#define OPTIMIZATION_BENCHMARKCLIENT_H

#include "stdafx.h"
#include "ConstraintsFile.h"
#include "Benchmark.h"
#include "GeneticAlgorithms.h"

using namespace std;

/**
 *
 * This just allows for using CF do define the ConstraintsFile class. Just of ease of typing.
 *
 * @typedef ContraintsFile
 */
typedef ConstraintsFile CF;

/**
 *
 * This just allows for using GA do define the GeneticAlgorithms class. Just of ease of typing.
 *
 * @typedef GeneticAlgorithms
 */
typedef GeneticAlgorithms GA;

void run();
void mainMenu(int);
string toString(double);
void compute(Benchmark::Fitness, Benchmark *, GA, CF, int, unsigned int);

#endif //OPTIMIZATION_BENCHMARKCLIENT_H