#include "dictionary.h"

void initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;
    printf("Initializing Data Dictionary...\n");

    FILE *dictionary = fopen (dictionaryName, "W+");

    fwrite (&mainHeader, sizeof(mainHeader))
    
}
