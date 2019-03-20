//
// Created by Shane on 4/16/2018.
//

#include "ConstraintsFile.h"

/**
 * This is the default constructor for the constraints class
 */

ConstraintsFile::ConstraintsFile() = default;

/**
 *
 * This will read constraints file with the values used for the Benchmark testing
 *
 * @param name the name of the file you would like to read
 */

ConstraintsFile::ConstraintsFile(const char *name)
{
    this->readFile(name);
}