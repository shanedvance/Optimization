//
// Created by Shane on 4/13/2018.
//

#ifndef OPTIMIZATION_LOCALSEARCH_H
#define OPTIMIZATION_LOCALSEARCH_H

#include "ConstraintsFile.h"
#include "Benchmark.h"
#include "stdafx.h"

using namespace std;

class LocalSearch
{
public:
  LocalSearch();
  explicit LocalSearch(const Benchmark &);
  ~LocalSearch();
  void setCount(int count);
  int getCount();
  vector<double> randomWalk(Benchmark::Fitness, vector<double>, double *, int, Range);
  vector<double> localSearch(Benchmark::Fitness, const vector<double> &, double *, double, Range);
  vector<double> iterativeLocalSearch(Benchmark::Fitness, const vector<double> &, double *, double, int, Range);

private:
  Benchmark bm;
  int count;

  double random(Range rng)
  {
    // Set up pseudo-random generator using built-in C++ mersenne_twister_engine
    random_device seed{};
    mt19937 engine{seed()};
    uniform_real_distribution<double> generate_random_number{rng.LB, rng.UB};
    double randNumber;

    return generate_random_number(engine);
  }

  vector<double> random_vector(int dim, Range rng)
  {
    vector<double> temp;

    // Create a vector of dimensionSize containing random real numbers
    for (int i = 0; i < dim; ++i)
    {
      temp.emplace_back(random(rng));
    }

    return temp;
  }

  /* This is the function that will update the vector's neighborhood based on the Empirical Gradient Descent
       formula. */
  vector<double> neighborhood(const vector<double> &x, double f_x, double alpha,
                              Range rng, Benchmark::Fitness f)
  {
    vector<double> solution, x_temp;
    double x_loc, gradient;

    for (unsigned int i = 0; i < x.size(); ++i)
    {
      x_temp = x; // reset x_temp to the initial solution

      x_temp.at(i) = x_temp.at(i) + alpha; // update the current index

      gradient = ((this->bm.*f)(x_temp)-f_x) / alpha; // calculate the gradient

      /* Gradient descent formula */
      x_loc = x.at(i) - alpha * gradient;

      /* Check if f(x_loc) <= f(x) */
      if (x_loc > rng.UB || x_loc < rng.LB)
      {
        x_loc = this->random(rng);
      }

      solution.emplace_back(x_loc); // put the new 'x' location in the solution vector
    }

    return solution;
  }
};

#endif //OPTIMIZATION_LOCALSEARCH_H
