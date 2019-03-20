//
// Created by Shane on 4/26/2018.
//

#include "GeneticAlgorithms.h"

using namespace std;

/**
 *
 * This sets up the genetic algorithm class.
 *
 * @param newBM passes this for evaluating the functions
 */

GeneticAlgorithms::GeneticAlgorithms(const Benchmark &newBM) : bm(newBM) {}

/**
 *
 * This is a version of the Genetic Algorithms (GA) that is known as Simple GA. This is the simplest algorithms of the
 * GA's. This will find the best (most optimal solution) of the given fitness function with respect to its dimension
 * and population.
 *
 * @param f the fitness (cost) evaluator
 * @param ns the size of the population
 * @param dim the amount of chromosomes in the population (the dimension)
 * @param rng the range of values
 * @param t_max the max amount of generations
 * @param cr the crossover rate
 * @param m the mutation parameter
 * @param er the elitism rate
 * @return the best solution
 */

vector<double> GeneticAlgorithms::simpleGA(Benchmark::Fitness f, int ns, unsigned int dim, Range rng, int t_max,
                                                         double cr, Mutation m, double er)
{

    vector<double> bestSolutions;

    double elitism = er * ns;
    /* Initialize population                                                  */
    vector<Population> p = this->generate_population(ns, dim, rng);
    /* Evaluate the fitness of the population                                */
    this->evaluate(f, &p);
    /* Normalize the fitness                                                 */
    this->getFitness(&p);
    /* Search for the best fitness in each generation                        */
    for (int t = 1; t <= t_max; ++t)
    {
        vector<NewPopulation> np;
        for (unsigned int s = 0; s < static_cast<unsigned int>(ns); s += 2)
        {
            /* This selects parents using the Roulette Wheel selector        */
            Parent parent = this->select(p);
            /* This is the mating process of the parents to create offspring */
            Child child = this->crossOver(parent, cr);
            /* This performs a mutation using the mutation parameters        */
            this->mutate(&child.one, m, rng);
            this->mutate(&child.two, m, rng);
            /* Add the children to the new population                        */
            np.emplace_back(NewPopulation(child.one));
            np.emplace_back(NewPopulation(child.two));
        }
        /* Evaluate the fitness of the new population                        */
        this->evaluate(f, &np);
        this->reduce(&p, &np, elitism);
        /* Normalize the fitness                                             */
        this->getFitness(&p);
        bestSolutions.emplace_back(this->getBestSolution(f, p, true));
    }
    return bestSolutions;

}

/**
 *
 * This will evaluate the best solution of the fitness function using differential evolution and a selected strategy.
 * The strategies vary from 1-10 and the first 5 are exponential and the other 5 are binomial. It goes through and
 * selects random indexes to be evaluated with respect to its selected strategy.
 *
 * @param fn the fitness function
 * @param dim the amount of dimensions needed
 * @param g_max the max amount of generations
 * @param np the size of the population
 * @param F the mutation rate
 * @param cr the crossover rate
 * @param rng the range of the fitness function
 * @param strategy the selected strategy (1-10)
 * @return the best solution
 */

vector<double> GeneticAlgorithms::diffEvolution(Benchmark::Fitness fn, unsigned int dim, int g_max, int np, double F,
                                                double cr, Range rng, int strategy)
{

    if (np >= 4)
    {
        vector<double> bestSolution;


        this->dim = dim;

        /* Initialize population                                                  */
        vector<Population> x = this->generate_population(np, dim, rng);
        /* Evaluate the fitness of the population                                 */
        this->evaluate(fn, &x);

        for (int g = 1; g <= g_max; ++g)
        {
            // the best genome which is used for the noise equation
            vector<double> x_best = this->getBestGenome(fn, x);
            for (unsigned int i = 0; i < static_cast<unsigned int>(np); ++i)
            {
                // mutate and recombine
                vector<double> u = x.at(i).genome;
                this->mutateStrategy(&u, x_best, x, F, cr, i, np, strategy);

                // select by comparing the fitness
                if ((this->bm.*fn)(u) <= x.at(i).cost)
                {
                    x.at(i).genome = u;
                }
            }

            /* Evaluate the fitness of the population                                 */
            this->evaluate(fn, &x);
            bestSolution.emplace_back(this->getBestSolution(fn, x, false));
        }

        return bestSolution;
    }
    else
    {
        perror("the population size is too small to evaluate on");
        exit(EXIT_FAILURE);
    }

}