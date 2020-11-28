//
// Created by Shane on 4/18/2018.
//

#include "stdafx.h"
#include "BenchmarkRun.h"

#define __STRICT_ANSI__

using namespace std;

int main(int argc, char **argv)
{
  if (argc > 1)
  {
    perror("too many arguments");
    return EXIT_FAILURE;
  }

  run();

  return EXIT_SUCCESS;
}
