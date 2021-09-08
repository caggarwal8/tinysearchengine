/*
 * index.c - index data structure
 *
 * See index.h for more information
 *
 * CJ Aggarwal, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "index.h"
#include "../libcs50/file.h"

/* global variable definition */
typedef hashtable_t index_t;

/* index_new - see index.h for more information */
index_t*
index_new(int slots)
{
  return (index_t*) hashtable_new(slots);
}

/* index_find - see index.h for more information */
void*
index_find(index_t* index, char* key)
{
  return (index_t*) hashtable_find(index, key);
}

/* index_insert - see index.h for more information */
bool
index_insert(index_t* index, char* key, void* item)
{
  return (index_t*) hashtable_insert(index, key, item);
}

/* deleteFunction - see index.h for more information */
void
deleteFunction(void* item)
{
  counters_delete((counters_t*)item);
}

/* index_delete - see index.h for more information */
void
index_delete(index_t* index)
{
  hashtable_delete(index, deleteFunction);
}

/* countersIterateFunction - see index.h for more information */
void
countersIterateFunction(void* fp, const int key, int count)
{
  fprintf((FILE*)fp, " %d %d", key, count);
}

/* iterateFunction - see index.h for more information */
void
iterateFunction(void* fp, const char* key, void* item)
{
  fprintf(fp, "%s ", key);
  counters_iterate(item, fp, countersIterateFunction);
  fprintf(fp, "\n");
}

/* index_save - see index.h for more information */
void
index_save(index_t* index, FILE* fp)
{
  hashtable_iterate(index, fp, iterateFunction);
}

/* index_load - see index.h for more information */
void
index_load(index_t* index, FILE* fp)
{
  char* word;
  int id;
  int count;
  counters_t* counter;

  while (!feof(fp)) {
    word = file_readWord(fp);

    if (word != NULL) {
      while ((fscanf(fp, "%d %d ", &id, &count)) == 2) { //while input is two integers
        if ((counter = index_find(index, word)) == NULL) {
          counter = counters_new(); //create new counnter
          counters_set(counter, id, count);
          index_insert(index, word, counter);
        }
        else {
          counters_set(counter, id, count);
        }
      }
    }
    free(word); // no memory leaks
  }
}
