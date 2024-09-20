#ifndef DICTIONARY_H
#define DICTIONARY_H
 #define   EMPTY_POINTER -1
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
  char value [50];
  struct Node *next;
  // in memory pointer
  // file pointer
  long next; 
}NODE;

void initializeDataDictionary();

#endif
