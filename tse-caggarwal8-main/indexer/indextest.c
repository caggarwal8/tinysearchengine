/*
 * indextest.c - a tester for the indexer module
 *
 * indextest reads from a file, converts it to an index,
 * and prints that index to a new file
 * 
 * this might be in a different order than the original file
 * please compare using the given indexcmp method
 *
 * Please see the specs for more detailed information.
 *
 * CJ Aggarwal, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/index.h"
#include "../common/pagedir.h"

bool checkArgs(int argc, char* argv[]);

/*
 * The main function of this file
 * 
 * Calls index_load from index.c to load
 * the file into an index, then calls index_save
 * to save the new index to the given new file/
 *
 * Inputs: the arguments 
 * oldIndexFilename and newIndexFilename
 * old should be readable and new should be writeable
 *
 * Output: exit code
 */
int
main (const int argc, char* argv[])
{
  if (!checkArgs(argc, argv)) {
    exit(1);
  }

  FILE* fpOld = fopen(argv[1], "r"); // readable
  FILE* fpNew = fopen(argv[2], "w"); // writeable
  index_t* index = index_new(500); // between 200 and 900

  index_load(index, fpOld); // from index.c
  index_save(index, fpNew); // from index.c
  index_delete(index);
  
  fclose(fpOld);
  fclose(fpNew);
  
  return 0;
}

/*
 * checkArgs ensures that the given arguments are valid
 *
 * oldIndexFilename should be a readable file
 * newIndexFilename should be a writeable file
 *
 * inputs: the arguments
 * output: true if the arguments are valid; otherwise, false
 */
bool
checkArgs(int argc, char*argv[])
{
  if (argc != 3) {
    fprintf(stderr, "Invalid arguments\nUsage: ./indextest oldIndexFilename newIndexFilename\n");
    return false;
  }

  FILE* fpOld;
  if ((fpOld = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Invalid arguments - oldIndexFilename cannot be read\n");
    fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
    return false;
  }
  
  else {
    fclose(fpOld);
  }

  FILE* fpNew;
  if ((fpNew = fopen(argv[2], "w")) == NULL) {
    fprintf(stderr, "Invalid arguments - newIndexFilename is not writeable\n");
    fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
    return false;
  }

  else {
    fclose(fpNew);
  }

  return true;
}
