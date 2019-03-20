//
// Created by Shane on 3/28/2018.
//

#include "Benchmark.h"

using namespace std;

/**
 * This is the default constructor for Benchmark.
 */

Benchmark::Benchmark() : simulation_count(0)
{

    this->csvHeaders.emplace_back("Name");
    this->csvHeaders.emplace_back("Dimension");
    this->csvHeaders.emplace_back("SIMU-");

}

/**
 *
 * This is an overloaded constructor for Benchmark that allows for creating simulation test data.
 *
 * @param dimension amount of dimensions.
 * @param max amount of simulations.
 * @param rng this is the range (interval the values are in.
 */

Benchmark::Benchmark(int dimension, int max, Range rng) : simulation_count(max)
{

    this->csvHeaders.emplace_back("Name");
    this->csvHeaders.emplace_back("Dimension");
    this->csvHeaders.emplace_back("SIMU-");

    this->generate_random_matrix(dimension, max, rng);

}

/**
 * This is the destructor for Benchmark that will do clean up.
 */

Benchmark::~Benchmark()
{

    this->matrix.clear();

}

/**
 *
 * This will read in a text file (*.txt) for Shekel's Foxhole test.
 *
 * @param name the name of the file
 */

void Benchmark::readText(const char *name)
{
    this->read_txt_file(name);
}

/**
 *
 * This will write all the test data to a comma-delimited text file (*.csv).
 *
 * @param name the name of the file
 */

void Benchmark::toCSV(const char *name)
{
    this->write_csv_file(name);
}

/**
 *
 * This adds the computed results to Benchmark for converting to a CSV file.
 *
 * @param results
 */

void Benchmark::add_results(const vector<string> &results)
{
    this->csvResults.emplace_back(results);
}

/**
 * Clears the simulation test data.
 */

void Benchmark::clear_test_data()
{
    this->matrix.clear();
}

/**
 *
 * This will set the amount of simulations being performed on the benchmark algorithm
 *
 * @param count the amount of simulation performed
 */

void Benchmark::setSimulation(int count)
{
    this->simulation_count = count;
}

/**
 *
 * This is a getter method to retrieve the simulation test data.
 *
 * @return The the test data
 */

vector<vector<double>> Benchmark::test_data()
{
    return this->matrix;
}

/**
 *
 * This will randomly generate the simulation test data for the given parameters.
 *
 * @param dimension amount of dimensions.
 * @param max amount of simulations.
 * @param rng this is the range (interval the values are in.
 */

void Benchmark::pseudo_random_matrix(int dimension, int max, Range rng)
{

    // This is used for the CSV file
    this->simulation_count = max;

    this->generate_random_matrix(dimension, max, rng);

}

/**
 * This will add Time (sec) to the headers for the CSV file.
 */

void Benchmark::addHeader(string hd)
{

    // Avoid adding duplicates
    if (find(this->csvHeaders.begin(), this->csvHeaders.end(), hd) == this->csvHeaders.end())
    {
        this->csvHeaders.emplace_back(hd);
    }

}

/**
 *
 * Find the benchmark result for Schwefel.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::schwefel(vector<double> x)
{

    double result = 0.0;

    // summation {1,...,n}
    for (double value : x)
    {
        result = result + (-value * sin(sqrt(abs(value))));
    }

    return result;

}

/**
 *
 * Find the benchmark result for De Jong 1st.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::first_de_jong(vector<double> x)
{

    double result = 0.0;

    // summation {1,...,n}
    for (double value : x)
    {
        result = result + (pow(value, 2.0));
    }

    return result;

}

/**
 *
 * Find the benchmark result for Rosenbrock.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::rosenbrock(vector<double> x)
{

    double value_one, value_two, value_three, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        value_one = x.at(i);
        value_two = x.at(i + 1);
        value_three = pow(value_one, 2.0) - value_two;

        result = result + (100.0 * pow(value_three, 2.0) + (1.0 - pow(value_one, 2.0)));
    }

    return result;

}

/**
 *
 * Find the benchmark result for Rastrigin's Saddle.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::rastrigin(vector<double> x)
{

    double result = 0.0;

    // summation {1,...,n}
    for (double value : x)
    {
        result = result + (pow(value, 2.0) - 10.0 * cos((2.0 * M_PI) * value));
    }

    return (2 * x.size()) * result;

}

/**
 *
 * Find the benchmark result for Griewank.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::griewank(vector<double> x)
{

    double value, result_one = 0.0, result_two = 0.0;

    // summation {1,...,n} and product {1,...,n}
    for (unsigned int i = 0; i < x.size(); ++i)
    {
        value = x.at(i);

        result_one = result_one + (pow(value, 2.0) / 4000.0);
        result_two = result_two * (cos(value / sqrt(static_cast<double>(i) + 1.0)));
    }

    return 1.0 + (result_one - result_two);

}

/**
 *
 * Find the benchmark result for Sine Envelope Sine Wave.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::sine_envelope_sine_wave(vector<double> x)
{

    double value_one, value_two, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        value_one = pow(x.at(i), 2.0);
        value_two = pow(x.at(i + 1), 2.0);

        result = result + (0.5 + ((pow(sin(value_one + value_two - 0.5), 2.0))
                                  / (1.0 + 0.001 * pow((value_one + value_two), 2.0))));
    }

    return -result;

}

/**
 *
 * Find the benchmark result for Stretch V Sine Wave.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::stretch_v_sine_wave(vector<double> x)
{

    double value_one, value_two, result = 0.0, four_root = 1.0 / 4.0, ten_root = 1.0 / 10.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        value_one = pow(x.at(i), 2.0);
        value_two = pow(x.at(i + 1), 2.0);

        result = result + (pow(value_one + value_two, four_root)
                           * sin(pow(50.0 * pow(value_one + value_two, ten_root), 2.0)) + 1.0);
    }

    return result;

}

/**
 *
 * Find the benchmark result for Ackley One.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::ackley_one(vector<double> x)
{

    double value_one, value_two, value_three, value_four, result = 0.0, fraction_e = 1.0 / pow(M_E, 0.2);

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        value_one = pow(x.at(i), 2.0);
        value_two = pow(x.at(i + 1), 2.0);
        value_three = cos(2.0 * x.at(i));
        value_four = sin(2.0 * x.at(i + 1));

        result = result + (fraction_e * sqrt(value_one + value_two) + 3.0 * (value_three + value_four));
    }

    return result;

}

/**
 *
 * Find the benchmark result for Ackley Two.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::ackley_two(vector<double> x)
{

    double denominator_one, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        denominator_one = (pow(M_E, 0.2)
                                  * sqrt((pow(x.at(i), 2.0) + pow(x.at(i + 1), 2.0)) / 2.0));

        result = result +
                 (20.0 + M_E - (20.0 / denominator_one)
                  - pow(M_E, 0.5 * (cos(2.0 * M_PI * x.at(i)) + cos(2.0 * M_PI * x.at(i + 1)))));
    }

    return result;

}

/**
 *
 * Find the benchmark result for Egg Holder.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::egg_holder(vector<double> x)
{

    double sqrt_one, sqrt_two, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        sqrt_one = sqrt(abs(x.at(i) - x.at(i + 1) - 47.0));
        sqrt_two = sqrt(abs(x.at(i + 1) + 47.0 + (x.at(i) / 2.0)));

        result = result + (-x.at(i) * sin(sqrt_one) - (x.at(i + 1) + 47.0) * sin(sqrt_two));
    }

    return result;

}

/**
 *
 * Find the benchmark result for Rana.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::rana(vector<double> x)
{

    double sqrt_one, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        sqrt_one = sqrt(abs(x.at(i + 1) - x.at(i) + 1.0));

        result = result + (x.at(i) * sin(sqrt_one)
                           * cos(sqrt_one) + (x.at(i + 1) + 1.0) * cos(sqrt_one) * sin(sqrt_one));
    }

    return result;

}

/**
 *
 * Find the benchmark result for Pathological.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::pathological(vector<double> x)
{

    double sqrt_one, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        sqrt_one = sqrt(100.0 * pow(x.at(i), 2.0) + pow(x.at(i + 1), 2.0));

        result = result + (0.5 + (sin(pow(sqrt_one, 2.0))
                                  - 0.5 / (1.0 + 0.001 * pow(pow(x.at(i), 2.0) - (2.0 * x.at(i)) * x.at(i + 1)
                                                                  + pow(x.at(i + 1) , 2.0), 2.0))));
    }

    return result;

}

/**
 *
 * Find the benchmark result for Michalewicz.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::michalewicz(vector<double> x)
{

    double value, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size(); ++i)
    {
        value = ((static_cast<double>(i) + 1.0) * pow(x.at(i), 2.0)) / M_PI;

        result = result + (sin(x.at(i) * sin(pow(value, 20.0))));
    }

    return -result;

}

/**
 *
 * Find the benchmark result for Master's Cosine Wave.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::masters_cosine_wave(vector<double> x)
{

    double value_one, value_two, result = 0.0;

    // summation {1,...,n}
    for (unsigned int i = 0; i < x.size() - 1; ++i)
    {
        value_one = pow(M_E, - (1.0 / 8.0) * (pow(x.at(i), 2.0) + pow(x.at(i + 1), 2.0)
                                              + (0.5 * (x.at(i + 1) * x.at(i)))));
        value_two = sqrt(pow(x.at(i), 2.0)
                                     + pow(x.at(i + 1), 2.0) + (0.5 * (x.at(i) * x.at(i + 1))));

        result = result + (value_one * cos(4.0 * value_two));
    }

    return -result;

}

/**
 *
 * Find the benchmark result for Shekel's Foxhole.
 *
 * @param x vector containing stochastic numbers in given range for all dimensions
 * @return the calculated fitness result.
 */

double Benchmark::shekel_foxholes(vector<double> x)
{

    double denominator_result, result = 0.0;

    // summation {1,...,m}
    for (unsigned int i = 0; i < this->c.size(); ++i)
    {
        denominator_result = 0.0;

        // summation {1,...,n}
        for (unsigned int j = 0; j < this->a.at(i).size(); ++j)
        {
            denominator_result = denominator_result + (pow(x.at(j) - this->a.at(i).at(j), 2.0));
        }

        result = result + (1.0 / (this->c.at(i) + denominator_result));
    }

    return -result;

}