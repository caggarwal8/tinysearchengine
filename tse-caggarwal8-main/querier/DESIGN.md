# CS50 TSE Querier
## Design Spec

## CJ Aggarwal, May 2021

This design spec was written with the help of the given design spec for the indexer module.

According to the requirements specs provided in the querier module, the Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### User interface

The querier initially interacts with the user through the command line. The user inputs a pageDirectory and an indexFilename.

```
./querier pageDirectory indexFilename
```

For example, if `letters` is a pageDirectory in `../data`,

``` bash
$ ./querier ../data/letters ../data/letters.index
```

The Querier then interacts with the user through stdin. The user provides queries and the module then uses said queries to produce a sorted list of documents with their respective scores. A query must be a sequence of words that only contains letters and spaces. No other characters can be inputted. The user can also make use of the operators: "and" and "or". These operators, however, cannot be at the very beginning, nor at the end. They also cannot be directly adjacent.

In turn, the program outputs the result of the query.

### Inputs and outputs

**Input**: the querier reads from the given index and directory as well as from the query. This is described above.

**Output**: the querier then outputs a list of documents along with their URL and their "score." The score represents how many "hits" the query returned for that particular page. The list is sorted by score in descending order.

### Functional decomposition into modules

We anticipate the following modules or functions:

1. *main*, which parses arguments and initializes other modules;
2. *checkArgs*, which ensures that the arguments are valid
3. *query*, which calls other methods to build the counter, create the array of scores, and print it.
4. *counterCreation*, which works to create the counter from the index and the array of words
5. *score*, which builds the array of scores from the counter
6. *sort*, which sorts the array of scores

And some helper modules that provide data structures:

1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
2. *word*, a module providing a function to normalize a word.
3. *counter*, a module that stores a key and a count

### Pseudo code for logic/algorithmic flow

This contains very high level pseudocode that covers the entire module. For more specific pseudocode please see IMPLEMENTATION.md

Pseudocode:

        parse arguments and ensure that they are valid
        load the index
        parse queries
              ensure that it is acceptable
              normalize it
              print the normalized query
              run through the index to create a counter based on the documents which satisfy the query
              build an array of scores using the counter
              sort the array of scores
              print the array of scores with url and score
        exit zero if no errors

### Major data structures

The score data structure will contain a score and a matching id.
The index data structure will act as a hashtable that stores words and counts
The counters data structure will be keyed by document id and will store scores
A countersHolder struct	which holds two	different counters - this is used to help with handling	"ands"

### Testing plan

We will test using integration testing.
This means that we will have a testing.sh bash script that will run the querier module under a number of different cases. We will then check those cases to make sure that they output what they are supposed to, including errors.

Some of these test cases will include:

Invalid arguments in the command line. Such as:
 1. Non-crawler directory
 2. Non-readable indexFile
 3. Non-existing directory

Invalid queries. Such as:
 1. Non-letter and non-space characters
 2. Double operator
 3. Operator at the start
 4. Operator at the end

Valid queries. Such as:
 1. Errant spacing - should ignore
 2. Operators
 3. Various capitalizations - should ignore

Check with myvalgrind