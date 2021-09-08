#!/bin/bash
#
# testing.sh - testing script written in bash that tests the crawler module
#
# usage: call make test &> testing.out in terminal
#
# runs the crawler module in a number of situations, assesing whether it works
# correctly as per the specs
#
# checks edge cases and error cases as well
#
# CJ Aggarwal, April 2021

# Some tests are commented out due to time restrictions. toscrape at depth 1 is handled by valgrind

# defining myvalgrind for later use
myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'

# running with no arguments
./crawler

# running with an external url
./crawler https://www.google.com testing 1

# running with too many arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/letters-0 0 too many arguments

# running with a non existent directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/wrong-1 0

# running with a maxDepth that is too low
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/letters-0 -1

# running with a maxDepth that is too high
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/letters-0 11

# running valgrind on a medium-sized test case to check for memory leaks
$myvalgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ results/toscrape-1 1

# letters at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/letters-0 0

# letters at depth 1
#./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/letters-1 1

# letters at depth 2
#./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/letters-2 2

# letters at depth 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html results/letters-10 10

# toscrape at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ results/toscrape-0 0

# toscrape at depth 1
#./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ results/toscrape-1 1

# toscrape at depth 2
#./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ results/toscrape-2 2

# toscrape at depth 3
#./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ results/toscrape-3 3

# wikipedia at depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ results/wiki-0 0

# wikipedia at depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ results/wiki-1	1

# wikipedia at depth 2
#./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ results/wiki-2 2
