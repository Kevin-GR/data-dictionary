#ifndef DICTIONARY_H
#define DICTIONARY_H
#define EMPTY_POINTER -1
#define DATA_BLOCK_SIZE 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node
{
  char name[DATA_BLOCK_SIZE];
  long dataPointer; 
  long attributesPointer;
  long nextEntity;
}ENTITY;

typedef struct attribute
{
  char name[DATA_BLOCK_SIZE];
  bool isPrimary;current
  int type;
  long size;
  long nextAtribute;
}ATTRIBUTE;


FILE *initializeDataDictionary(const char *dictonaryName);
int appendEntity(FILE *dataDictionary, ENTITY NuevaEntidad);

void removeEntity(FILE *dataDictionary, long currentEntityPointer, const char *newEntityName);
void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *newEntityName, long newEntity);

void reorderAtributees(FILE *dataDictionary, long currentEntityPointer, const char *neweEntityName, long newEntity);


#endif
