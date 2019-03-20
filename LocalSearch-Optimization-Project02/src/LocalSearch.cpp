//
// Created by Shane on 4/13/2018.
//

#include "LocalSearch.h"
using namespace std;

/**
 * This sets up the LocalSearch algorithm to be used.
 */

LocalSearch::LocalSearch() : count(0) {};

/**
 *
 * This is a constructor that passes reference to the benchmark data
 *
 * @param bm the Benchmark class that has the test data we need
 */

LocalSearch::LocalSearch(const Benchmark &bm) : count(0)
{
    this->bm = bm;
}

/**
 * This is the default destructor for LocalSearch
 */

LocalSearch::~LocalSearch() = default;

/**
 *
 * This will set the count for the current iteration
 *
 * @param count initialize a value
 */

void LocalSearch::setCount(int count)
{
    this->count = count;
}

/**
 * This will get the amount of times it took to find the best solution
 * @return
 */

int LocalSearch::getCount()
{
    return this->count;
}

/**
 *
 * This performs the random walk (a.k.a blind worker) algorithm for finding the best solution
 * for the local optima
 *
 * @param f_cost the fitness function we want to use
 * @param arg the vector we want to perform random walk on
 * @param fitness_0 the initial best fitness
 * @param itr how many times we want to iterate
 * @param dim the amount of dimensions
 * @param rng the range of the values of lower-bound and upper-bound
 * @return
 */

vector<double> LocalSearch::randomWalk(Benchmark::Fitness f_cost, vector<double> arg,
                                            double *fitness_0, int itr, Range rng)
{

    double fitness;
    vector<double> arg_best, results;

    for (int i = 1; i <= itr; ++i)
    {

        fitness = ((this->bm).*f_cost)(arg);

        // check the found fitness with the current best fitness
        if (fitness < *fitness_0)
        {
            *fitness_0 = fitness;
            arg_best = arg;

            ++this->count; // get how many times it took to find best solution
        }

        results.emplace_back(*fitness_0);

        arg = this->random_vector(static_cast<int>(arg.size()), rng);

    }

    return arg_best;

}

/**
 *
 * This performs the local search algorithm using the empirical gradient descent to find the best solution of the
 * local optima
 *
 * @param f the fitness function we want to use
 * @param init the initial random vector
 * @param f_best the initial best solution
 * @param delta the delta we will use for the neighborhood search
 * @param rng the range of values we will be testing with
 * @return
 */

vector<double> LocalSearch::localSearch(Benchmark::Fitness f, const vector<double> &init,
                                             double *f_best, double delta, Range rng)
{

    bool foundBetter = true;
    vector<double> x_loc, x_best = init;

    // check if we have found a better solution
    while(foundBetter)
    {

        foundBetter = false;

        // generate a neighborhood
        x_loc = this->neighborhood(x_best, *f_best = (this->bm.*f)(x_best), delta, rng, f);

        if ((this->bm.*f)(x_loc) < *f_best)
        {
            x_best = x_loc;
            foundBetter = true;
            ++this->count; // how many iterations it took to find best
        }

    }

    return x_best;

}

/**
 *
 * This performs the iterative local search algorithm using the empirical gradient descent to find the best solution
 * of the local optima
 *
 * @param f the fitness function we want to use
 * @param init the initial random vector
 * @param f_best the initial best solution
 * @param delta the delta we will use for the neighborhood search
 * @param t_max the maximum amount of iterations we want to test with
 * @param rng the range of values we will be testing with
 * @return
 */

vector<double> LocalSearch::iterativeLocalSearch(Benchmark::Fitness f, const vector<double> &init,
                                                      double *f_best, double delta, int t_max, Range rng)
{

    double f_global_best = 0.0;
    vector<double> x_best = init, x_global_best = init;

    *f_best = (this->bm.*f)(x_best); // find the fitness of the initial best solution

    for (int t = 1; t <= t_max; ++t)
    {

        x_global_best = this->localSearch(f, x_global_best, &f_global_best, delta, rng);

        // check the best solution against the global best
        if (*f_best < f_global_best)
        {
            // we found a better solution so update
            x_best = x_global_best;
            *f_best = f_global_best;
        }

        x_global_best = this->random_vector(static_cast<int>(init.size()), rng); // generate new random solution

    }

    return x_best;

}