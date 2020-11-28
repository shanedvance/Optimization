//
// Created by Shane on 4/26/2018.
//

#ifndef OPTIMIZATION_GENETICALGORITHMS_H
#define OPTIMIZATION_GENETICALGORITHMS_H

#include "Benchmark.h"
#include "ConstraintsFile.h"

using namespace std;

/**
 *
 * This creates an individual whether it be a child or parent.
 *
 * @typedef Parent and Child
 */
typedef struct Parent
{
  vector<double> one;
  vector<double> two;

} Child;

class GeneticAlgorithms
{
public:
  explicit GeneticAlgorithms(const Benchmark &);
  double simpleGA(Benchmark::Fitness, int, unsigned int, Range, int, double, Mutation, double);
  double diffEvolution(Benchmark::Fitness, unsigned int, int, int, double, double, Range, int);

private:
  Benchmark bm;
  int ns;
  unsigned dim;
  double totalFitness;

  double random(Range rng)
  {
    // Set up pseudo-random generator using built-in C++ mersenne_twister_engine
    random_device seed{};
    mt19937 engine{seed()};
    uniform_real_distribution<double> generate_random_number{rng.LB, rng.UB};

    return generate_random_number(engine);
  }

  // over loaded function of random
  unsigned int random(unsigned int lb, unsigned int ub)
  {
    // Set up pseudo-random generator using built-in C++ mersenne_twister_engine
    random_device seed{};
    mt19937 engine{seed()};
    uniform_int_distribution<unsigned int> generate_random_number{lb, ub};

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

  vector<Population> generate_population(int ns, unsigned int dim, Range rng)
  {
    this->ns = ns;
    this->dim = dim;

    vector<Population> populations;
    for (int i = 0; i < ns; ++i)
    {
      populations.emplace_back(Population(this->random_vector(dim, rng))); // add the new pop to the vector
    }
    return populations; // return the population
  }

  vector<double> join(const vector<double> &p1, const vector<double> &p2, unsigned int d)
  {
    vector<double> temp;

    // go through {0...d} and join
    for (unsigned int i = 0; i < d; ++i)
    {
      temp.emplace_back(p1.at(i));
    }

    // go through {d...dim} and join
    for (auto j = d; j < this->dim; ++j)
    {
      temp.emplace_back(p2.at(j));
    }

    return temp;
  }

  vector<double> selectParent(const vector<Population> &p)
  {
    double r = this->random(Range(1.0, this->totalFitness)); // get random number
    unsigned int s = 0;
    for (/* initialized */; s < static_cast<unsigned int>(this->ns) - 1 && r > 0; ++s)
    {
      r = r - p.at(s).cost; // reduce because the fitness is high
    }
    return p.at(s).particle; // return the fittest parent
  }

  Parent select(const vector<Population> &p)
  {
    Parent parent;
    parent.one = this->selectParent(p); // select individual for parent one
    parent.two = this->selectParent(p); // select individual for parent two
    return parent;
  }

  Child crossOver(const Parent &parent, double cr)
  {
    Child child;
    if (this->random(Range(0.0, 1.0)) < cr)
    {
      unsigned int d = this->random(1, this->dim);
      child.one = this->join(parent.one, parent.two, d); // create child one
      child.two = this->join(parent.two, parent.one, d); // create child two
    }
    else
    {
      child.one = parent.one; // child one is exact copy of parent 1
      child.two = parent.two; // child two is exact copy of parent 2
    }
    return child; // return the children
  }

  void mutate(vector<double> *child, Mutation m, Range rng)
  {
    for (unsigned int i = 0; i < this->dim; ++i)
    {
      if (this->random(Range(0.0, 1.0)) < m.rate)
      {
        // mutate the child making it different than its parents
        child->at(i) = child->at(i) + this->random(Range(-1.0, 1.0)) * (rng.UB - rng.LB) * m.range *
                                          pow(2.0, (-1.0 * this->random(Range(0.0, 1.0)) * m.precision));

        // check if we are still in the scope
        if (child->at(i) > rng.UB || child->at(i) < rng.LB)
        {
          child->at(i) = this->random(rng);
        }
      }
    }
  }

  void reduce(vector<Population> *p, vector<NewPopulation> *np, double elitism)
  {
    /* Sort the Population and NewPopulation by cost (a.k.a. evaluation function) */
    sort(p->begin(), p->end());
    sort(np->begin(), np->end());

    for (unsigned int s = 1; s <= elitism; ++s)
    {
      np->at((this->ns - 1) + 1 - s) = p->at(s - 1);
    }

    /* Swap the Population and NewPopulation */
    p->swap(*np);
  }

  double sum(vector<Population> p)
  {
    double result = 0.0;
    for (const Population &pop : p)
    {
      result = result + pop.cost; // compute the total sum
    }
    return result; // return the result
  }

  void getFitness(vector<Population> *p)
  {
    for (Population &pop : *p)
    {
      if (pop.cost >= 0.0)
      {
        pop.cost = 1.0 / (1.0 + pop.cost); // normalize if cost is big
      }
      else
      {
        pop.cost = 1.0 + abs(pop.cost); // normalize if cost is small
      }
    }
    this->totalFitness = this->sum(*p);
  }

  double getBestSolution(Benchmark::Fitness f, const vector<Population> &p, bool isSimpleGA)
  {
    if (isSimpleGA)
    {
      auto x_best = max_element(p.begin(), p.end());
      return (this->bm.*f)(x_best->particle);
    }
    else
    {
      auto x_best = min_element(p.begin(), p.end());
      return x_best->cost;
    }
  }

  vector<double> getBestGenome(Benchmark::Fitness f, const vector<Population> &p)
  {
    auto x_best = min_element(p.begin(), p.end());
    return x_best->particle;
  }

  void evaluate(Benchmark::Fitness f, vector<Population> *p)
  {
    // evaluate the function for each individual in the population
    for (Population &pop : *p)
    {
      pop.cost = (this->bm.*f)(pop.particle);
    }
    this->totalFitness = this->sum(*p);
  }

  void selectIndexes(unsigned int *r1, unsigned int *r2, unsigned int *r3, unsigned int *r4, unsigned int *r5,
                     int np, int i, int strategy)
  {
    // Check if r1 != r2 != r3 != r4 != r5 != i
    *r2 = this->random(0, static_cast<unsigned int>(np) - 1);
    *r3 = this->random(0, static_cast<unsigned int>(np) - 1);

    if (!(strategy == 1 || strategy == 6))
    {
      *r1 = this->random(0, static_cast<unsigned int>(np) - 1);
      while (*r1 == (*r2 || *r3 || *r4 || *r5 || i))
      {
        *r1 = this->random(0, static_cast<unsigned int>(np) - 1);
      }
    }
    while (*r2 == (*r1 || *r3 || *r4 || *r5 || i))
    {
      *r2 = this->random(0, static_cast<unsigned int>(np) - 1);
    }
    while (*r3 == (*r1 || *r2 || *r4 || *r5 || i))
    {
      *r3 = this->random(0, static_cast<unsigned int>(np) - 1);
    }
    if (strategy == 4 || strategy == 5 || strategy == 9 || strategy == 10)
    {
      *r4 = this->random(0, static_cast<unsigned int>(np) - 1);
      while (*r4 == (*r1 || *r2 || *r3 || *r5 || i))
      {
        *r4 = this->random(0, static_cast<unsigned int>(np) - 1);
      }
    }
    if (strategy == 5 || strategy == 10)
    {
      *r5 = this->random(0, static_cast<unsigned int>(np) - 1);
      while (*r5 == (*r1 || *r2 || *r3 || *r4 || i))
      {
        *r5 = this->random(0, static_cast<unsigned int>(np) - 1);
      }
    }
  }

  void mutateStrategy(vector<double> *u, vector<double> x_best, vector<Population> x, double F, double cr,
                      unsigned int i, int np, int strategy)
  {
    unsigned int r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
    this->selectIndexes(&r1, &r2, &r3, &r4, &r5, np, i, strategy);

    // switch to the strategy that we want to perform the mutation and crossover on
    // whether it be exponential or binomial crossover
    switch (strategy)
    {
    // DE/best/1/exp
    case 1:
    {
      unsigned int l = 0;
      unsigned int j = this->random(0, this->dim - 1);
      do
      {
        u->at(j) = x_best.at(j) + F * (x.at(r2).particle.at(j) - x.at(r3).particle.at(j));
        j = (j + 1) % this->dim;
        ++l;
      } while (this->random(Range(0.0, 1.0)) < cr && l < this->dim);
      break;
    }
    // DE/rand/1/exp
    case 2:
    {
      unsigned int l = 0;
      unsigned int j = this->random(0, this->dim - 1);
      do
      {
        u->at(j) = x.at(r3).particle.at(j) + F * (x.at(r1).particle.at(j) - x.at(r2).particle.at(j));
        j = (j + 1) % this->dim;
        ++l;
      } while (this->random(Range(0.0, 1.0)) < cr && l < this->dim);
      break;
    }
    // DE/rand-to-best/1/exp
    case 3:
    {
      unsigned int l = 0;
      unsigned int j = this->random(0, this->dim - 1);
      do
      {
        u->at(j) = x.at(i).particle.at(j) + F * (x_best.at(j) - x.at(i).particle.at(j)) + F * (x.at(r1).particle.at(j) - x.at(r2).particle.at(j));
        j = (j + 1) % this->dim;
        ++l;
      } while (this->random(Range(0.0, 1.0)) < cr && l < this->dim);
      break;
    }
    // DE/best/2/exp
    case 4:
    {
      unsigned int l = 0;
      unsigned int j = this->random(0, this->dim - 1);
      do
      {
        u->at(j) = x_best.at(j) + F * (x.at(r1).particle.at(j) + x.at(r2).particle.at(j) - x.at(r3).particle.at(j) - x.at(r4).particle.at(j));
        j = (j + 1) % this->dim;
        ++l;
      } while (this->random(Range(0.0, 1.0)) < cr && l < this->dim);
      break;
    }
    // DE/rand/2/exp
    case 5:
    {
      unsigned int l = 0;
      unsigned int j = this->random(0, this->dim - 1);
      do
      {
        u->at(j) = x.at(r5).particle.at(j) + F * (x.at(r1).particle.at(j) + x.at(r2).particle.at(j) - x.at(r3).particle.at(j) - x.at(r4).particle.at(j));
        j = (j + 1) % this->dim;
        ++l;
      } while (this->random(Range(0.0, 1.0)) < cr && l < this->dim);
      break;
    }
    // DE/best/1/bin
    case 6:
    {
      unsigned int j_rand = this->random(0, this->dim - 1);
      for (unsigned int j = 0; j < this->dim; ++j)
      {
        if (this->random(Range(0.0, 1.0)) < cr || j == j_rand)
        {
          u->at(j) = x_best.at(j) + F * (x.at(r2).particle.at(j) - x.at(r3).particle.at(j));
        }
      }
      break;
    }
    // DE/rand/1/bin
    case 7:
    {
      unsigned int j_rand = this->random(0, this->dim - 1);
      for (unsigned int j = 0; j < this->dim; ++j)
      {
        if (this->random(Range(0.0, 1.0)) < cr || j == j_rand)
        {
          u->at(j) = x.at(r3).particle.at(j) + F * (x.at(r1).particle.at(j) - x.at(r2).particle.at(j));
        }
      }
      break;
    }
    // DE/rand-to-best/1/bin
    case 8:
    {
      unsigned int j_rand = this->random(0, this->dim - 1);
      for (unsigned int j = 0; j < this->dim; ++j)
      {
        if (this->random(Range(0.0, 1.0)) < cr || j == j_rand)
        {
          u->at(j) = x.at(i).particle.at(j) + F * (x_best.at(j) - x.at(i).particle.at(j)) + F * (x.at(r1).particle.at(j) - x.at(r2).particle.at(j));
        }
      }
      break;
    }
    // DE/best/2/bin
    case 9:
    {
      unsigned int j_rand = this->random(0, this->dim - 1);
      for (unsigned int j = 0; j < this->dim; ++j)
      {
        if (this->random(Range(0.0, 1.0)) < cr || j == j_rand)
        {
          u->at(j) = x_best.at(j) + F * (x.at(r1).particle.at(j) + x.at(r2).particle.at(j) - x.at(r3).particle.at(j) - x.at(r4).particle.at(j));
        }
      }
      break;
    }
    // DE/rand/2/bin
    case 10:
    {
      unsigned int j_rand = this->random(0, this->dim - 1);
      for (unsigned int j = 0; j < this->dim; ++j)
      {
        if (this->random(Range(0.0, 1.0)) < cr || j == j_rand)
        {
          u->at(j) = x.at(r5).particle.at(j) + F * (x.at(r1).particle.at(j) + x.at(r2).particle.at(j) - x.at(r3).particle.at(j) - x.at(r4).particle.at(j));
        }
      }
      break;
    }
    default:
    {
      perror("invalid strategy selection");
      exit(EXIT_FAILURE);
    }
    }
  }
};

#endif //OPTIMIZATION_GENETICALGORITHMS_H
