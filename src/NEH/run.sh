#!/bin/bash

# Shane Vance
# Dr. Donald Davendra
# Optimization (CS 471)
# 5.27.2018

set -e

# function for the output of the NEH executable
function output()
{
  # get our files from the specified directory and sort them properly
  FILE=$(find $1 -type f -name '*.txt' -exec ls -v {} +)
  # iterate and execute each file for the NEH executable
    while read -r file; do
    ./NEH $file | tr '\n' ','
    printf '\n'
  done <<< "$FILE"
}

ARGS=2

# do some error handling
if [ $# -ne "$ARGS" ]; then
  echo "Incorrect parameter count. Need 2 parameter values."
  echo "Example: ./Run.sh Taillard_Series Results.csv"
  exit 1
fi

# compute our results
echo "Gathering results for NEH heuristics..."
declare TT=($(output $1))

# get our headers for the output file
HEADER=$(echo ",FSS,,,FSSB,,,FSSNW,,")
HEADER1=$(echo "No.,NEH,T(s),#,NEH,T(s),#,NEH,T(s),#")

# print our results to a file
printf '%s\n' "${TT[@]}" | nl -b a | awk '{$2=$2};1' | tr ' ' ',' | sed "1i $HEADER" | sed "2i $HEADER1" > $2
echo "Competed!"
echo "The results are in: $2"
