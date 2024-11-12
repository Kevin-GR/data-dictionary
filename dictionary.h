#ifndef DICTIONARY_H
#define DICTIONARY_H
#define EMPTY_POINTER -1
#define DATA_BLOCK_SIZE 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
  char nombre[DATA_BLOCK_SIZE];
  long dataPointer; 
  long atributos;
  long SigEntidad;
}ENTITY;

FILE *initializeDataDictionary(const char *dictonaryName);
int creaEntidad(FILE *DiccionarioDatos, ENTITY NuevaEntidad);
void reordenaEntidades(FILE *diccionarioDatos,ENTITY NuevaEntidad, long NuevaDirEntidad);

#endif
