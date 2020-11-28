//
// Created by Shane on 5/10/2018.
//

#ifndef OPTIMIZATION_SWARMINTELLIGENCE_H
#define OPTIMIZATION_SWARMINTELLIGENCE_H

#include "ConstraintsFile.h"
#include "Benchmark.h"
#include "stdafx.h"

class SwarmIntelligence
{
public:
  SwarmIntelligence();
  explicit SwarmIntelligence(Benchmark);
  ~SwarmIntelligence();
  double pso(Benchmark::Fitness, int, double, double, int, int, Range);
  double firefly(Benchmark::Fitness, int, int, int, double, double, double, Range);

private:
  Benchmark bm;

  double random(Range rng)
  {
    // Set up pseudo-random generator using built-in C++ mersenne_twister_engine
    random_device seed{};
    mt19937 engine{seed()};
    uniform_real_distribution<double> generate_random_number{rng.LB, rng.UB};

    return generate_random_number(engine);
  }

  double min(const vector<double> &p)
  {
    // grab the smallest element from the list
    auto gBest = min_element(p.begin(), p.end());
    return *gBest;
  }

  double min(const vector<Population> &p)
  {
    // grab the fittest member from the population
    auto gBest = min_element(p.begin(), p.end());
    return gBest->cost;
  }

  double sum(vector<Population> p)
  {
    // calculate the total fitness for the population
    double result = 0.0;
    for (const Population &pop : p)
    {
      result = result + pop.cost; // compute the total sum
    }
    return result; // return the result
  }

  void psoPopulation(Benchmark::Fitness fn, int pMax, int dim, Range rng, vector<Population> *p, vector<double> *pBest)
  {
    for (unsigned int i = 0; i < pMax; ++i)
    {
      Population pop;
      for (unsigned int j = 0; j < dim; ++j)
      {
        // generate particles randomly
        pop.particle.emplace_back(this->random(rng));
        // calculate particles velocity
        pop.velocity.emplace_back(this->random(Range(0.0, 0.5 * (rng.UB - rng.LB))));
      }

      p->emplace_back(pop);
      // calculate particle fitness
      p->at(i).cost = (this->bm.*fn)(p->at(i).particle);
      // set best for each particle
      pBest->emplace_back(p->at(i).cost);
    }
  }

  void ffPopulation(Benchmark::Fitness fn, int fMax, int dim, Range rng, vector<Population> *f)
  {
    for (unsigned int i = 0; i < fMax; ++i)
    {
      Population pop;
      for (unsigned int j = 0; j < dim; ++j)
      {
        // generate fireflies randomly
        pop.particle.emplace_back(this->random(rng));
      }

      f->emplace_back(pop);

      // evaluate the particles fitness
      f->at(i).cost = (this->bm.*fn)(f->at(i).particle);
    }
  }
};

#endif //OPTIMIZATION_SWARMINTELLIGENCE_H
