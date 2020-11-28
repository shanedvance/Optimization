//
// Created by Shane on 4/18/2018.
//

#ifndef OPTIMIZATION_BENCHMARKCLIENT_H
#define OPTIMIZATION_BENCHMARKCLIENT_H

#include "stdafx.h"
#include "ConstraintsFile.h"
#include "Benchmark.h"
#include "SwarmIntelligence.h"

using namespace std;

/**
 *
 * This just allows for using CF to define the ConstraintsFile class. Just of ease of typing.
 *
 * @typedef ContraintsFile
 */
typedef ConstraintsFile CF;

/**
 *
 * This just allows for using SI to define the SwarmIntelligence class. Just of ease of typing.
 *
 * @typedef SwarmIntelligence
 */
typedef SwarmIntelligence SI;

void run();
void mainMenu();
string toString(double);
void compute(Benchmark::Fitness, Benchmark *, SI, CF, int, unsigned int);

#endif //OPTIMIZATION_BENCHMARKCLIENT_H
