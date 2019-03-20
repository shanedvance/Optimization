//
// Created by Shane on 4/18/2018.
//

#include "BenchmarkRun.h"

using namespace std;

/**
 * This will be used by main.cpp to run the program. This will perform various evaluation at the command of the user.
 */

void run()
{

    int selection = 0;
    string name;
    const char *cfName = "constraints.csv";

    Benchmark bm;
    CF constraints(cfName); // read initialization file
    // read the constraints from the file
    bm.readText(constraints.extraFile.c_str());
    bm.setSimulation(constraints.totalSimulations);
    SI si(bm);

    // this is a vector containing all the fitness functions
    vector<Benchmark::Fitness> fitness =
            {&Benchmark::schwefel, &Benchmark::first_de_jong, &Benchmark::rosenbrock, &Benchmark::rastrigin,
             &Benchmark::griewank, &Benchmark::sine_envelope_sine_wave, &Benchmark::stretch_v_sine_wave,
             &Benchmark::ackley_one, &Benchmark::ackley_two, &Benchmark::egg_holder, &Benchmark::rana,
             &Benchmark::pathological, &Benchmark::michalewicz, &Benchmark::masters_cosine_wave,
             &Benchmark::shekel_foxholes};

    // continually loop through the prompts and execution of the programs until the user exits the program
    while (true)
    {

        // check what system we are running on
        #ifdef WIN32
            system("CLS");   // clear console screen for windows
        #else
            system("clear"); // clear the console screen for unix
        #endif

        // create some output allowing the user to select an option
        mainMenu();
        cin >> selection;

        if (selection == 3)
        {
            // this will refresh everything
            run();
        }
        else if (selection == 4)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            cout << endl << "Gathering results..." << endl;
        }

        // iterate through all the objective functions
        for (unsigned int i = 0; i < fitness.size(); ++i)
        {
            compute(fitness.at(i), &bm, si, constraints, selection, i);
        }

        // prompt user for file naming
        cout << endl << "What do you want to name your output file (need *.csv)? ";
        cin >> name;
        cout << endl;

        // write to csv file
        bm.toCSV(name.c_str());

        cout << "Creating file: " << name << endl;

        // clear the input buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
    }

}

/**
 *
 * This converts a double to a string in scientific notation
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
 * This will give a prompt for the user giving them options to select from to execute a program
 */

void mainMenu()
{
    // create some output allowing the user to select an option
    cout << "Swarm Algorithms:" << endl;
    cout << "===============================" << endl;
    cout << "1. Particle Swarm Optimization" << endl;
    cout << "2. Firefly Algorithm " << endl;
    cout << "3. Refresh constraints file" << endl;
    cout << "4. Exit" << endl;
    cout << "Select an option: ";
}

/**
 *
 * This will compute each of the function for n-dimensions and m-simulation
 *
 * @param f the fitness function we want to use
 * @param bm the Benchmark class that contains the test functions
 * @param si the SwarmIntelligence (SI) class that contains the SI functions
 * @param cf this is the constraints file
 * @param selection this is the option that is selected by the user for the program to run
 * @param i this is the index for which is used by the constraints file
 */

void compute(Benchmark::Fitness f, Benchmark *bm, SI si, CF cf, int selection, unsigned int i)
{

    clock_t benchmark_start;
    double bmDuration;

    // iterate through the dimension amounts
    for (int dim = cf.dimension.at(i).LB; dim <= cf.dimension.at(i).UB; dim += cf.dimension.at(i).LB)
    {

        // Setup our data structures
        vector<string> temp;

        // add the function name and dimension to the temp vector
        temp.emplace_back(cf.functionName.at(i));
        temp.emplace_back(toString(dim));

        // times how long the program ran for the given dimension
        benchmark_start = clock();

        for (int sim = 0; sim < cf.totalSimulations; ++sim)
        {
            // Switch to the the function we want to use
            switch (selection - 1)
            {
                case 0:
                {
                    double sol = si.pso(f, dim, cf.c1, cf.c2, cf.iMax, cf.pMax, cf.range.at(i));
                    temp.emplace_back(toString(sol));
                    break;
                }
                case 1:
                {
                    double sol = si.firefly(f, dim, cf.iMax, cf.pMax, cf.alpha, cf.beta, cf.gamma, cf.range.at(i));
                    temp.emplace_back(toString(sol));
                    break;
                }
                default:
                {
                    perror("invalid input. exiting the program.");
                    exit(EXIT_FAILURE);
                }
            }
        }

        bmDuration = (clock() - benchmark_start) / static_cast<double>(CLOCKS_PER_SEC);

        // add time to the benchmark class for the CSV file
        bm->addHeader("T(s)");
        temp.emplace_back(toString(bmDuration));

        // Add our results to the Benchmark object for use creating the CSV file
        bm->add_results(temp);

    }

}