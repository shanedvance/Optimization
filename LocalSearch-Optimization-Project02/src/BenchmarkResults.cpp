//
// Created by Shane on 4/18/2018.
//

#include "BenchmarkResults.h"
using namespace std;

/**
 * This will be used by main.cpp to run the program
 */

void run()
{

    // this is a vector containing all the fitness functions
    vector<Benchmark::Fitness> fitness =
            {&Benchmark::schwefel, &Benchmark::first_de_jong, &Benchmark::rosenbrock, &Benchmark::rastrigin,
             &Benchmark::griewank, &Benchmark::sine_envelope_sine_wave, &Benchmark::stretch_v_sine_wave,
             &Benchmark::ackley_one, &Benchmark::ackley_two, &Benchmark::egg_holder, &Benchmark::rana,
             &Benchmark::pathological, &Benchmark::michalewicz, &Benchmark::masters_cosine_wave,
             &Benchmark::shekel_foxholes};

    while (true)
    {

        Variables var{};
        string name;

        // check what system we are running on
        #ifdef WIN32
            system("CLS"); // clear console screen for windows
        #else
            system("clear"); // clear the console screen for unix
        #endif

        // create some output allowing the user to select an option
        cout << "OPTIONS:" << endl;
        cout << "===============" << endl;
        cout << "1. Random Walk" << endl;
        cout << "2. Local Search" << endl;
        cout << "3. Iterative Local Search" << endl;
        cout << "4. Exit" << endl;
        cout << "Select an option: ";

        cin >> var.selected_fn;

        if (var.selected_fn == 4)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            cout << endl << "Gathering results..." << endl;
        }

        ConstraintsFile constraints("constraints.csv"); // read initialization file
        Benchmark bm;
        bm.readText(constraints.extraFile.c_str());
        LocalSearch ls(bm);

        // initialize our variables
        var.simulations = constraints.simulations;
        var.random_walk_itr = constraints.randWalkItr;
        var.itr_local_search = constraints.itrLS;
        var.delta = constraints.delta;

        // iterate through all the fitness functions
        for (unsigned int i = 0; i < fitness.size(); ++i)
        {
            var.dimension = constraints.dimension.at(i);
            /*******************************************************************************
            ***This allows for running each of the fitness functions in their own thread***
            *******************************************************************************/
            var.dimension = constraints.dimension.at(i);
            future<bool> retValue = async(&compute, fitness.at(i), &bm, &ls, var,
                                                    constraints.range.at(i), constraints.functionName.at(i));

            if (!retValue.get()) { exit(EXIT_SUCCESS); }

        }

        cout << endl << "What do you want to name your output file (need *.csv)? ";
        cin >> name;
        cout << endl;

        var.fitness = bm;
        var.name = name.c_str();

        /****************************************************************
         ***This allows for creating the files while I run other tests***
         ****************************************************************/
        thread generateFile(&createFile, var);
        generateFile.join();

    }

}

/**
 *
 * This is a thread function for creating a file while we run some other tests
 *
 * @param var an object that we want to use in the thread
 * @return
 */

void createFile(Variables var)
{

    var.fitness.toCSV(var.name);

}

/**
 *
 * This converts a double to a string
 *
 * @param val pass a double that you would like to convert to a double
 * @return
 */

string toString(double val)
{

    stringstream ss;
    ss << val;
    return ss.str();

}

/**
 *
 * This will compute each of the function for n-dimensions and m-simulation
 *
 * @param f the fitness function we want to use
 * @param bm the Benchmark class that contains the test functions
 * @param ls the LocalSearch class that contains the local search functions
 * @param var the variable that we need
 * @param rng the range of values we will be testing with for each function
 * @param name the name of the function, will be used for the file output
 * @return
 */

bool compute(Benchmark::Fitness f, Benchmark *bm, LocalSearch *ls, Variables var, Range rng, const string &name)
{

    clock_t benchmark_start;
    double benchmark_duration, fitness_0;

    for (int dim = var.dimension.LB; dim <= var.dimension.UB; dim += var.dimension.LB)
    {

        // Create our simulation data
        bm->pseudo_random_matrix(dim, var.simulations, rng);

        // Setup our data structures
        vector<string> temp;
        vector<vector<double>> data = bm->test_data();

        temp.emplace_back(name);
        temp.emplace_back(to_string(dim));

        // Computes the data for the given function
        benchmark_start = clock();

        // Iterate through all of our simulations
        for (const vector<double> &vec : data)
        {

            // Needs to be reinitialized each time because it gets modified by each
            // function that is being used
            fitness_0 = DOUBLE_MAX;

            // Switch to the the function we want to use
            switch (var.selected_fn - 1)
            {
                case 0:
                {
                    // Random walk (a.k.a. blind worker)
                    ls->randomWalk(f, vec, &fitness_0, var.random_walk_itr, rng);
                    break;
                }
                case 1:
                {
                    // Local search
                    ls->localSearch(f, vec, &fitness_0, var.delta, rng);
                    break;
                }
                case 2:
                {
                    // Iterative local search
                    ls->iterativeLocalSearch(f, vec, &fitness_0, var.delta, var.itr_local_search, rng);
                    break;
                }
                default:
                {
                    return false;
                }

            }

            temp.emplace_back(toString(fitness_0));

        }

        benchmark_duration = (clock() - benchmark_start) / static_cast<double>(CLOCKS_PER_SEC);

        // add time
        bm->addHeader("T(s)");
        temp.emplace_back(toString(benchmark_duration));

        // add iteration count
        bm->addHeader("#");
        temp.emplace_back(toString(round(ls->getCount() / var.simulations)));
        ls->setCount(0);

        // Add our results to the Benchmark object for use creating the CSV file
        (*bm).add_results(temp);
        bm->clear_test_data();
    }

    return true;

}