#!/bin/bash
#
# testing.sh - a testing script for the querier module
# This script, written in bash, attempts to check how well
# the module works.
# It runs a number of test cases and edge cases.
#
# Usage - make test
#
# CJ Aggarwal, May 2021

# Defining myvalgrind for later use
myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'

# Error cases with invalid arguments

# No arguments
./querier

# One argument
./querier ~/cs50-dev/shared/tse/output/letters-10

# Too many arguments
./querier ~/cs50-dev/shared/tse/output/letters-10 ~/cs50-dev/shared/tse/output/letters-10.index index.out

# Invalid directory but valid file
./querier ~cs50-dev/tse/output/letters-9 ~/cs50-dev/shared/tse/output/letters-10.index

# Non crawler directory
./querier ~/cs50-dev/tse/output ~/cs50-dev/shared/tse/output/letters-10.index

# Non-existent path for file name
./querier ~/cs50-dev/shared/tse/output/letters-10 ~cs50-dev/tse/output/letters-9.index

# Valid Arguments but Invalid Queries

./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index < invalidqueries

# Valid Arguments with Valid Queries with various spacing and capitalizations

./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index	< validqueries

# Valgrind check for memory leaks

$myvalgrind ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index < validqueries
