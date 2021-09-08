#!/bin/bash
#
# testing.sh - a testing script for the indexer module
# This script, written in bash, attempts to check how well
# the module works.
# It runs a number of test cases and edge cases.
#
# Usage - make test
#
# CJ Aggarwal, May 2021

# Defining myvalgrind for later use
myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'

# Error cases

# No arguments
./indexer

# One argument
./indexer ~/cs50-dev/shared/tse/output/letters-10

# Too many arguments
./indexer ~/cs50-dev/shared/tse/output/letters-10 index.out index.out

# Invalid directory but valid file
./indexer ~cs50-dev/tse/output/letters-9 index.out

# Non crawler directory
./indexer ~/cs50-dev/tse/ouptut index.out

# Non-existent path for file name
./indexer ~/cs50-dev/shared/tse/output/letters-10 ~cs50-dev/tse/output/letters-9/index.out

# Read only directory for file name
./indexer ~/cs50-dev/shared/tse/output/letters-10 ~/cs50-dev/shared/tse/output/letters-10

# Read only file for file name
./indexer ~/cs50-dev/shared/tse/output/letters-10 ~/cs50-dev/shared/tse/output/letters-10/1

# Making a new directory to store new files

mkdir ../index-outputs

# Testing indexer on letters-0

./indexer ~/cs50-dev/shared/tse/output/letters-0 ../index-outputs/letters0.out

# Comparing correct output with ours using indexcmp

~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/letters-0.index ../index-outputs/letters0.out

# Using indextest

./indextest ../index-outputs/letters0.out ../index-outputs/letters0.test

# Comparing letter0.out with letters0.test using indexcmp

~/cs50-dev/shared/tse/indexcmp ../index-outputs/letters0.out ../index-outputs/letters0.test

# Testing indexer on letters-1

./indexer ~/cs50-dev/shared/tse/output/letters-1 ../index-outputs/letters1.out

# Comparing outputs with indexcmp

~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/letters-1.index ../index-outputs/letters1.out

# Using indextest

./indextest ../index-outputs/letters1.out ../index-outputs/letters1.test

# Comparing letter1.out with letters1.test using indexcmp

~/cs50-dev/shared/tse/indexcmp ../index-outputs/letters1.out ../index-outputs/letters1.test

# Testing indexer on wikipedia-1

./indexer ~/cs50-dev/shared/tse/output/wikipedia-1 ../index-outputs/wiki1.out

# Comparing outputs with indexcmp

~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/wikipedia-1.index ../index-outputs/wiki1.out

# Using indextest

./indextest ../index-outputs/wiki1.out ../index-outputs/wiki1.test

# Comparing wiki1.out with wiki1.test using indexcmp

~/cs50-dev/shared/tse/indexcmp ../index-outputs/wiki1.out ../index-outputs/wiki1.test

# Testing indexer on toscrape-1

./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../index-outputs/toscrape1.out

# Comparing outputs with indexcmp

~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/toscrape-1.index ../index-outputs/toscrape1.out

# Using indextest

./indextest ../index-outputs/toscrape1.out ../index-outputs/toscrape1.test

# Comparing toscrape1.out with toscrape1.test using indexcmp

~/cs50-dev/shared/tse/indexcmp ../index-outputs/toscrape1.out ../index-outputs/toscrape1.test

# Checking if myvalgrind works

$myvalgrind ./indexer ~/cs50-dev/shared/tse/output/letters-10 ../index-outputs/letters10.out

# Comparing outputs with indexcmp

~/cs50-dev/shared/tse/indexcmp ~/cs50-dev/shared/tse/output/letters-10.index ../index-outputs/letters10.out

# Using myvalgrind on indextest

$myvalgrind ./indextest ../index-outputs/letters10.out ../index-outputs/letters10.test

# Comparing letters10.out with letters10.test using indexcmp

~/cs50-dev/shared/tse/indexcmp ../index-outputs/letters10.out ../index-outputs/letters10.test
