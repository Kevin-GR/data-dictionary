#ifndef DICTIONARY_H
#define DICTIONARY_H
#define EMPTY_POINTER -1
#define DATA_BLOCK_SIZE 50
#define MAIN_ENTITY_POINTER -1

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
  int isPrimary;
  int type;
  long size;
  long nextAtribute;
}ATTRIBUTE;



FILE *initializeDataDictionary(const char *dictonaryName);
int appendEntity(FILE *dataDictionary, ENTITY NuevaEntidad);
ENTITY removeEntity(FILE *dataDictionary, long currentEntityPointer, const char *EntityName);
void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *newEntityName, long newEntity);
int appendAttribute(FILE *dataDictionary, ATTRIBUTE newAttribute);
void reorderAtributes(FILE *dataDictionary, long currentAttributePointer, const char *newAttributeName, long newAttributeDirection);
void openDataDictionary();
void entitiesMenu(FILE *dataDictionary);
ENTITY findEntity(FILE *dataDictionary,const char *name);
void Attributes_menu(FILE *dataDictionary, ENTITY currentEntity);
void main_menu();


#endif

