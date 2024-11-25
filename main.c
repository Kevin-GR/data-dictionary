#include "main.h"

int main(int argc, char** argv) {
    char name[50];

    printf("Entera a data Dictionary name:");

    fgets(name, sizeof(name),stdin);
    FILE *dataDictionary = initializeDataDictionary(name);
    createEntity(dataDictionary);

   
    fclose(dataDictionary);
    return 0;
}

void createEntity(FILE *dataDictionary)
{
    ENTITY newEntity;

    printf("Enter the entity name");
    fgets(newEntity.name, sizeof(newEntity.name), stdin);
    newEntity.dataPointer= EMPTY_POINTER;
    newEntity.attributesPointer= EMPTY_POINTER;
    newEntity.nextEntity= EMPTY_POINTER;

    long entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, ,newEntity.name, newEntity.attributesPointer);
}

void createAtribute(FILe *dataDictionary, ENTITY currentEntity)
{
    ATTRIBUTE newAttribute;
    printf("Enter the attribute name");
    fgets(newAttribute.name, sizeof(newAttribute.name), stdin);
    newAttribute.isPrimary = false;
    newAttribute.type =1;
    newAttribute.size = sizeof(int);

    newAttribute.nextAtribute =EMPTY_POINTER;

    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAtributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);
}

