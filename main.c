#include "main.h"

int main(int argc, char** argv) {
    char name[50];

    printf("inserta el nombre del diccionario:");

    fgets(name, sizeof(name),stdin);
    FILE *dataDictionary = initializeDataDictionary(name);
    createEntity(dataDictionary);

   
    fclose(dataDictionary);
    return 0;
}

void createEntity(FILE *dataDictionary)
{
      
    ENTITY newEntity;
    printf("Inserta el nombre de la entidad");
    fgets(newEntity.name, sizeof(newEntity.name), stdin);
    newEntity.dataPointer= EMPTY_POINTER;
    newEntity.attributesPointer= EMPTY_POINTER;
    newEntity.nextEntity= EMPTY_POINTER;

    long entityDirection = creaEntidad(dataDictionary, newEntity);
    reorderEntities(dataDictionary, newEntity.dataPointer,newEntity.name, newEntity.attributesPointer);
}