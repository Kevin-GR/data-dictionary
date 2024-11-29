#include "main.h"

int main(int argc, char** argv) {
    char name[50];
    printf("Enter a data Dictionary name:");

    fgets(name, sizeof(name),stdin);

    initializeDataDictionary(name);
    return 0;
}


