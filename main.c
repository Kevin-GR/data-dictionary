#include "main.h"

int main(int argc, char** argv) {
    char name[50];
    printf("Enter a data Dictionary name:");

    fgets(name, sizeof(name),stdin);

    FILE *dataDictionary = initializeDataDictionary(name);
    Entities_menu(dataDictionary);
    /*createEntity(dataDictionary);

    ENTITY currentEntity ;
    currentEntity.dataPointer = 58;
    currentEntity.attributesPointer = 66;
    currentEntity.nextEntity = 74;

    createAtribute(dataDictionary, currentEntity);
    createAtribute(dataDictionary, currentEntity);

    fclose(dataDictionary);*/

    return 0;
}


