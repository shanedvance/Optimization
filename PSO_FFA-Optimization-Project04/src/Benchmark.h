//
// Created by Shane on 3/28/2018.
//

#ifndef OPTIMIZATION_BENCHMARK_H
#define OPTIMIZATION_BENCHMARK_H

#include "ConstraintsFile.h"
#include "stdafx.h"

using namespace std;

class Benchmark
{
    public:

        /**
         *
         * This defines a pointer to a fitness function (evaluation function) of the Benchmark class.
         *
         * @typedef Fitnesss
         */
        typedef double (Benchmark::*Fitness)(vector<double>);

        Benchmark();
        explicit Benchmark(int, int, Range);
        ~Benchmark();
        void readText(const char*);
        void toCSV(const char*);
        void add_results(const vector<string>&);
        void clear_test_data();
        void setSimulation(int count);
        void addHeader(string);
        vector<vector<double>> test_data();
        void pseudo_random_matrix(int, int, Range);
        double schwefel(vector<double>);
        double first_de_jong(vector<double>);
        double rosenbrock(vector<double>);
        double rastrigin(vector<double>);
        double griewank(vector<double>);
        double sine_envelope_sine_wave(vector<double>);
        double stretch_v_sine_wave(vector<double>);
        double ackley_one(vector<double>);
        double ackley_two(vector<double>);
        double egg_holder(vector<double>);
        double rana(vector<double>);
        double pathological(vector<double>);
        double michalewicz(vector<double>);
        double masters_cosine_wave(vector<double>);
        double shekel_foxholes(vector<double>);

    private:

        int simulation_count;

        vector<vector<double>> a, matrix;
        vector<double> c;
        vector<string> csvHeaders;
        vector<vector<string>> csvResults;

        vector<double> tokenize(char *str_ptr, const char *delimiter)
        {

            vector<double> temp;

            // This will store the tokenized string to a char_ptr
            char *token_ptr = strtok(str_ptr, delimiter);

            // Loops through all the tokens until there are no more
            while(token_ptr != nullptr)
            {
                temp.emplace_back(stod(token_ptr));
                token_ptr = strtok(nullptr, delimiter);
            }

            return temp;

        }

        void tokenize_a(char *strToken)
        {

            // This will store the tokenized string to a char array
            char *token_one = strtok(strToken, ";");

            vector<string> temp_one, temp_two;

            // Loops through all the tokens until there are no more
            while(token_one != nullptr)
            {
                temp_one.emplace_back(token_one);
                token_one = strtok(nullptr, ";");
            }

            // Iterate through all the tokenized strings
            for (string str : temp_one)
            {
                // Need to get rid of curly braces
                str = str.substr(1, str.size() - 2);
                this->a.emplace_back(this->tokenize(&str[0u], ","));
            }

        }

        // Read a file for Shekel's Foxholes and put it into a vector.
        void read_txt_file(const char *name)
        {

            FILE *textFile;
            textFile = fopen(name, "r");

            string c, a, str_temp;

            char buffer[1024];

            if (textFile == nullptr)
            {
                perror("we had trouble reading the text file");
            }
            else
            {
                // Loop through the file to gather everything we need for C
                while (buffer[0] != '\n')
                {
                    fgets(buffer, sizeof(buffer), textFile);
                    str_temp = buffer;

                    c.append(str_temp.begin(), str_temp.end() - 1);
                }

                // Get rid  of the curly braces and unnecessary characters
                c = c.substr(3, c.size() - 5);

                // Tokenize the string into a vector
                this->c = this->tokenize(&c[0u], ",");

                // Loop through the file to gather everything we need for A
                bool first = true;
                while (!feof(textFile))
                {
                    fgets(buffer, sizeof(buffer), textFile);
                    str_temp = buffer;

                    if (first)
                    {
                        a.append(str_temp.begin(), str_temp.end() - 2);
                        first = false;
                    }
                    else
                    {
                        a.append(str_temp.begin() + 13, str_temp.end() - 2);
                    }
                    a.append(";");
                }

                // Get rid  of the curly braces and unnecessary characters
                a = a.substr(7, a.size() - 8);

                // Tokenize the string into a vector
                tokenize_a(&a[0u]);

                // clear the buffer
                memset(buffer, 0, sizeof(buffer));
            }

            fclose(textFile);

        }

        void write_csv_file(const char *name)
        {

            FILE *csvFile;
            csvFile = fopen(name, "w");

            // Do some error handling
            if (csvFile == nullptr)
            {
                perror("Error opening CSV file");
            }
            else
            {

                // Write the headers to the CSV file
                for (string header : this->csvHeaders)
                {
                    const char *tempChar = nullptr;
                    if (header == "SIMU-")
                    {
                        for (int i = 1; i <= this->simulation_count; ++i)
                        {
                            string hd = header;
                            tempChar = (i < this->simulation_count) ? (hd.append(to_string(i).append(","))).c_str()
                                                                    : (hd.append(to_string(i))).c_str();
                            fputs(tempChar, csvFile);
                        }

                        if (header != this->csvHeaders.back())
                        {
                            fputs(",", csvFile);
                        }

                    }
                    else
                    {
                        tempChar = (header != this->csvHeaders.back()) ? (header.append(",")).c_str() : header.c_str();
                        fputs(tempChar, csvFile); // Put comma-delimited text in CSV file
                    }

                }

                fputs("\n", csvFile); // Put new line in CSV file

                // Write the results to the CSV file
                for (vector<string> vec : this->csvResults)
                {
                    for (unsigned int i = 0; i < vec.size(); ++i)
                    {
                        const char *tempChar = (i != vec.size() - 1) ? (vec.at(i).append(",")).c_str()
                                                                     : vec.at(i).c_str();
                        fputs(tempChar, csvFile); // Put comma-delimited text in CSV file
                    }

                    if (vec != this->csvResults.back()) { fputs("\n", csvFile); }

                }

            }

            fclose(csvFile);

        }

        double random(Range rng)
        {
            // Set up pseudo-random generator using built-in C++ mersenne_twister_engine
            random_device seed{};
            mt19937 engine{seed()};
            uniform_real_distribution<double> generate_random_number{rng.LB, rng.UB};
            return generate_random_number(engine);
        }


        vector<double> random_vector(int dim, Range rng)
        {

            vector<double> temp;

            // Create a vector of dimensionSize containing random real numbers
            for (int i = 0; i < dim; ++i)
            {
                temp.emplace_back(this->random(rng));
            }

            return temp;

        }

        void generate_random_matrix(int dim, int matrixSize, Range rng)
        {

            if (matrixSize > 0)
            {

                // Set up pseudo-random generator using built-in C++ mersenne_twister_engine
                vector<double> temp = this->random_vector(dim, rng);
                this->matrix.emplace_back(temp);

                // Keep creating a vector of values for the amount of matrixSize
                this->generate_random_matrix(dim, matrixSize - 1, rng);

            }

        }

};

#endif //OPTIMIZATION_BENCHMARK_H