#include "dictionary.h"

void initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;
    int stringsToSave =0;
    printf("Initializing Data Dictionary...\n");

    FILE *dictionary = fopen (dictionaryName, "W+");
    fwrite (&mainHeader, sizeof(mainHeader)1, dictionary);
    printf("cuantas cadenas quieres guardar");
    scanf("%d", &stringsToSave);
    fflush(stdin);
    
    for(int counter=0; counter< stringsToSave; counter++)
    {
        NODE currentNode;
        currentNode.next =EMPTY_POINTER;
        printf("enter a value for node #%d", counter);
        scanf("%s[^ \n]", currentNode.value);
        fgets(&(currentNode.value),sizeof(currentNode.value), dictionary);    
    }
    return EXIT_SUCCESS;
}
