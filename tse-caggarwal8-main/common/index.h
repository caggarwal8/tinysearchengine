/*
 * index.h - index data structure for the indexer module
 *
 * Uses a hashtable of counters to keep track of how many instances
 * there are of each word in each document
 *
 * This data strucutre provides a number of functions, including
 * index_new, index_find, and more for basic usage.
 *
 * It heavily uses hashtable and counter functions.
 *
 * CJ Aggarwal, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/* global type */
typedef hashtable_t index_t;

/* functions */

/*
 * index_new - creates a new index data structue with the specified
 * number of slots
 *
 * employs hashtable_new
 *
 * Input: int slots - number of slots requested
 * Output: initialized index_t*
 */
index_t* index_new(int slots);

/*
 * index_find - searches for the given key in the given index
 * and returns a pointer to the counter if found
 *
 * input: index_t* index- the given index; char*key - the given key
 * 
 * output: the counter if found; null if not
 */
void* index_find(index_t* index, char* key);

/*
 * index_insert - inserts a key and item into the given index
 * uses hashtable index
 *
 * input:
 * index_t* index- the index
 * char* key - the key to be inserted
 * void* item - the corresponding item to be inserted
 * 
 * output: true if successful; false otherwise
 */
bool index_insert(index_t* index, char* key, void* item);

/* deleteFunction - a helper function to be called in
 * index_delete
 *
 * uses the counter_delete function
 *
 * input: void* item - the item to be deleted
 *
 * output: none
 */
void deleteFunction(void* item);

/*
 * index_delete - deletes a given index
 *
 * input: the index to be deleted
 * ouput: none 
 */
void index_delete(index_t* index);

/*
 * countersIterateFunction - a helper function to be called in iterateFunction
 *
 * prints out the key and count
 *
 * input: the file, key, and count
 * output: none
 */
void countersIterateFunction(void* fp, const int key, int count);

/*
 * iterateFunction - calls a counter iteration and prints each key and count
 *
 * input: the file, key, and item
 * output: none
 */
void iterateFunction(void* fp, const char* key, void* item);

/*
 * prints the index to a given file
 *
 * uses a hashtable iteration and other helper methods
 *
 * input: the index and the file
 * output: none
 */
void index_save(index_t* index, FILE* fp);

/*
 * loads a file into an index data structure by parsing through the file
 *
 * input: the index to be written into and the file to be read
 *
 * output: none
 */
void index_load(index_t* index, FILE* fp);
