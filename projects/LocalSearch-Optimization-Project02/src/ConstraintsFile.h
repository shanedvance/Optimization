//
// Created by Shane on 4/16/2018.
//

#ifndef OPTIMIZATION_CONSTRAINTSFILE_H
#define OPTIMIZATION_CONSTRAINTSFILE_H

#include "stdafx.h"
using namespace std;

struct Range
{
    Range() = default;
    Range(double lb, double ub) : LB(lb), UB(ub) {}
    double LB, UB;
};

struct Dimension
{
    Dimension() = default;
    Dimension(int lb, int ub) : LB(lb), UB(ub) {}
    int LB, UB;
};

class ConstraintsFile
{
public:

    ConstraintsFile();
    explicit ConstraintsFile(const char *);
    vector<string> functionName;
    vector<Range> range;
    vector<Dimension> dimension;
    string extraFile;
    int simulations;
    double delta;
    int randWalkItr; // Random walk Iterations
    int itrLS; // Iterative Local Search

private:

    vector<string> tokenize(char *str_ptr, const char *delimiter)
    {

        vector<string> temp;

        // This will store the tokenized string to a char_ptr
        char *token_ptr = strtok(str_ptr, delimiter);

        // Loops through all the tokens until there are no more
        while(token_ptr != nullptr)
        {
            temp.emplace_back(token_ptr);
            token_ptr = strtok(nullptr, delimiter);
        }

        return temp;

    }


    void readFile(const char *name)
    {

        int count = 0;

        FILE *file;
        file = fopen(name, "r");

        char buffer[1024];

        if (name == nullptr)
        {
            perror("had trouble opening the CSV file");
        }
        else
        {
            // iterative through the file until we get to the end
            while (!feof(file))
            {

                // gets the current line
                fgets(buffer, sizeof(buffer), file);

                // these are the headers of the columns
                if (!(count == 0 || count == 2 || count == 3))
                {
                    string str = buffer;
                    vector<string> temp = this->tokenize(&str[0u], ",\r\n");

                    // if the count is one then we are at the second row
                    // and do the following
                    if (count == 1)
                    {
                        this->simulations = stoi(temp.at(0));
                        this->delta = stod(temp.at(1));
                        this->randWalkItr = stoi(temp.at(2));
                        this->itrLS = stoi(temp.at(3));
                    }
                    else
                    {
                        // read in all the constraints that we need for the file
                        this->functionName.emplace_back(temp.at(0));

                        if (temp.at(1) != "null")
                        {
                            this->extraFile = temp.at(1);
                        }

                        vector<string> rTemp =
                                this->tokenize(&temp.at(2).substr(1, temp.at(2).size() - 2)[0u], ";\r\n");

                        double lb, ub;

                        if (rTemp.at(0) == "pi")
                        {
                            lb = M_PI;
                            ub = stod(rTemp.at(1));
                        }
                        else if (rTemp.at(1) == "pi")
                        {
                            lb = stod(rTemp.at(0));
                            ub = M_PI;
                        }
                        else
                        {
                            lb = stod(rTemp.at(0));
                            ub = stod(rTemp.at(1));
                        }

                        this->range.emplace_back(Range(lb, ub));

                        rTemp = this->tokenize(&temp.at(3).substr(1, temp.at(3).size() - 2)[0u], ";\r\n");
                        this->dimension.emplace_back(Dimension(stoi(rTemp.at(0)), stoi(rTemp.at(1))));
                    }


                }

                // clears the buffer
                memset(&buffer[0], 0, sizeof(buffer));

                ++count;
            }

        }

        fclose(file);

    }

};

#endif //OPTIMIZATION_CONSTRAINTSFILE_H