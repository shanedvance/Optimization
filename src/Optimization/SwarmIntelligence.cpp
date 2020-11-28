//
// Created by Shane on 5/10/2018.
//

#include "SwarmIntelligence.h"

/**
 *
 * This is the default constructor for the Swarm Intelligence class.
 *
 */
SwarmIntelligence::SwarmIntelligence() = default;

/**
 *
 * This is the default constructor that passes in the BenchmarkClass. This way we can use the functions from there.
 *
 * @param newBM Contains information with regard to the benchmark class
 */
SwarmIntelligence::SwarmIntelligence(Benchmark newBM) : bm(newBM) {}

/**
 *
 * This is the default destructor for the SwarmIntelligence class.
 *
 */
SwarmIntelligence::~SwarmIntelligence() = default;

/**
 *
 * This is the Particle Swarm Optimization (PSO) algorithm that will evaluate the objective (fitness) function. It is
 * optimized to find the minimum value of the given objective function.
 *
 * @param fn the fitness function
 * @param dim the amount of dimensions
 * @param c1 the step size of first value
 * @param c2 the step size of second value
 * @param iMax max iterations
 * @param pMax max population size
 * @param rng the range of values from fitness function
 * @return the best solution
 */
double SwarmIntelligence::pso(Benchmark::Fitness fn, int dim, double c1, double c2, int iMax, int pMax, Range rng)
{
  vector<Population> p;
  vector<double> pBest;
  double gBest, maxVelocity = 0.1 * rng.UB; // max velocity is 1/10th of the upper bound

  // initialize the population
  this->psoPopulation(fn, pMax, dim, rng, &p, &pBest);

  // set gBest from all particles
  gBest = this->min(pBest);

  // iterate for each generation
  for (int t = 0; t < iMax; ++t)
  {
// threaded for loop for going through the population
#pragma omp parallel for shared(gBest, pBest, p)
    for (unsigned int j = 0; j < pMax; ++j)
    {
// threaded for loop to perform calculations on population
#pragma omp parallel for shared(gBest, pBest)
      for (unsigned int i = 0; i < dim; ++i)
      {
        // calculate new velocity
        p.at(j).velocity.at(i) = p.at(j).velocity.at(i) + c1 * this->random(Range(0.0, 1.0)) * (pBest.at(j) - p.at(j).particle.at(i)) + c2 * this->random(Range(0.0, 1.0)) * (pBest.at(j) - p.at(j).particle.at(i));

        // calculate new particle
        p.at(j).particle.at(i) += p.at(j).velocity.at(i);

        // check if our velocity has gone out of bounds
        if (p.at(j).velocity.at(i) > maxVelocity)
        {
          p.at(j).velocity.at(i) = this->random(Range(0.0, 0.5 * (rng.UB - rng.LB)));
        }

        // check if the particle has gone out of bounds
        if (p.at(j).particle.at(i) > rng.UB || p.at(j).particle.at(i) < rng.LB)
        {
          p.at(j).particle.at(i) = this->random(rng);
        }
      }

      // calculate the particle fitness
      p.at(j).cost = (this->bm.*fn)(p.at(j).particle);

      // check if the particle velocity has improved
      if (p.at(j).cost < pBest.at(j))
      {
        pBest.at(j) = p.at(j).cost;
      }

      // check if gBest has improved
      if (pBest.at(j) < gBest)
      {
// critical section needs to be locked and then unlocked
#pragma omp critical
        {
          gBest = pBest.at(j);
        }
      }
    }
  }

  // returns the best particle found
  return gBest;
}

/**
 *
 * This is the Particle Swarm Optimization (PSO) algorithm that will evaluate the objective (fitness) function. It is
 * optimized to find the minimum value of the given objective function.
 *
 * @param fn the fitness function
 * @param dim the dimension count
 * @param iMax max iterations
 * @param fMax the max amount of fireflies
 * @param alpha the alpha value
 * @param betamin the minimum value of betamin
 * @param gamma the gamma to be evaluated
 * @param rng the range of fitness function
 * @return the best solution
 */
double SwarmIntelligence::firefly(Benchmark::Fitness fn, int dim, int iMax, int fMax, double alpha, double betamin,
                                  double gamma, Range rng)
{
  vector<Population> f;

  // create population
  this->ffPopulation(fn, fMax, dim, rng, &f);

  double gBest, scale = abs(rng.UB - rng.LB), beta0 = 1.0, beta, temp, delta, r;

  // grab initial best solution
  gBest = this->min(f);

  // iterate for each generation
  for (int t = 0; t < iMax; ++t)
  {
// put current fireflies in threads
#pragma omp parallel for
    for (unsigned int i = 0; i < fMax; ++i)
    {
// put other fireflies in threads
#pragma omp parallel for
      for (unsigned int j = 0; j < fMax; ++j)
      {
        // check if intensity_j < intensity_i
        if (f.at(j).cost < f.at(i).cost)
        {

          // calculate eqn. 3
          r = 0.0;
// perform calculation in it own threads
#pragma omp parallel for reduction(+ \
                                   : r)
          for (unsigned int d = 0; d < dim; ++d)
          {
            r += pow(f.at(i).particle.at(d) - f.at(j).particle.at(d), 2.0);
          }
          r = sqrt(r);

          // calculate eqn. 2
          beta = ((beta0 - betamin) * exp(-gamma * pow(r, 2.0))) + betamin;

// calculate eqn. 4
// perform calculation in threads
#pragma omp parallel for
          for (unsigned int d = 0; d < dim; ++d)
          {
            r = this->random(Range(0.0, 1.0));
            temp = alpha * (r - 0.5) * scale;
            delta = f.at(j).particle.at(d) - f.at(i).particle.at(d);
            f.at(i).particle.at(d) = f.at(i).particle.at(d) + (1.0 - beta) + delta * beta + temp;

            // check if the firefly flew out of bounds
            if (f.at(i).particle.at(d) > rng.UB || f.at(i).particle.at(d) < rng.LB)
            {
              f.at(i).particle.at(d) = this->random(rng);
            }
          }
        }
      }

      // evaluate the fireflies intensity/fitness
      f.at(i).cost = (this->bm.*fn)(f.at(i).particle);

      // update our best solution
      if (f.at(i).cost < gBest)
      {
#pragma omp critical
        {
          gBest = f.at(i).cost;
        }
      }
    }

    // rank each firefly according to its intensity (a.k.a. fitness)
    sort(f.begin(), f.end());
  }

  // check if after all generations if a better solution has arrived
  double checkBest = this->min(f);

  // update the best solution
  if (checkBest < gBest)
  {
    gBest = checkBest;
  }

  // return the best solution
  return gBest;
}
