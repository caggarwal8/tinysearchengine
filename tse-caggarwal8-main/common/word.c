/*
 * word.c - this file, written in C, helps indexer to normalize words
 *
 * See word.h for more information 
 *
 * CJ Aggarwal, May 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* normalizeWord - see word.h for more information */
void
normalizeWord(char* word)
{
  if (word == NULL) {
    return;
  }

  for (int i = 0; i < strlen(word); i++) {
    word[i] = tolower(word[i]);
  }
}
