/*
 * pagedir.c - see pagedir.h for more information
 *
 * CJ Aggarwal, April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"

/* initializing helper functions to be called later on */
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(char* directory);

/* pagedir_init - see pagedir.h for more information */
bool
pagedir_init(const char* pageDirectory)
{
  char* fileName = (char*)malloc(strlen(pageDirectory)+strlen("./crawler") + 1);
  // allocating the correct amount of memory
  sprintf(fileName, "%s/.crawler", pageDirectory); // print to fileName
  FILE* fp;
  if ((fp = fopen(fileName, "w")) == NULL) { // make sure it is writeable
    return false;
  }
  fclose(fp);
  free(fileName); // no memory leaks
  return true;
}

/* pagedir_save - see pagedir.h for more information */
void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
  // allocating the correct amount of memory - 4 for three digits
  char* fileName = (char*)malloc((strlen(pageDirectory)+4));
  sprintf(fileName, "%s/%d", pageDirectory, docID); // use id and print to fileName
  FILE* fp;
  fp = fopen(fileName, "w"); // make sure it is writeable
  fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
  fclose(fp);
  free(fileName); // no memory leaks
}

/* pagedir_validate - see pagedir.h for more information */
bool
pagedir_validate(char* directory)
{
  if (directory == NULL) { // to prevent a core dump
    fprintf(stderr, "Invalid directory - null\n");
    return false;
  }

  char fname[strlen(directory)*4 + 10]; //ensure there is enough space
  sprintf(fname, "%s/.crawler", directory); //make sure it is a crawler directory
  FILE* fp;
  if ((fp = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "Invalid directory - cannot be read\n");
    return false;
  }
  else {
    fclose(fp);
  }

  return true;
}
