#ifndef DICTIONARY_H
#define DICTIONARY_H
#define EMPTY_POINTER -1
#define DATA_BLOCK_NAME 50

#include <stdio.h>
#include <stdlib.h>
#include <string.g>

typedef struct Node
{
  char nombre[DATA_BLOCK_NAME];
  long dataPointer; 
  long atributos;
  long SigEntidad;
}ENTITY;

FILE *initializeDataDictionary(const char dictonaryName);
int creaEntidad(FIle *DiccionarioDatos, ENTITY NuevaEntidad);


#endif
