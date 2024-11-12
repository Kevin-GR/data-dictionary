#include "dictionary.h"

FILE *initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;
    printf("Initializing Data Dictionary...\n");
    FILE *dictionary = fopen (dictionaryName, "W+");
    fwrite (&mainHeader, sizeof(mainHeader),1, dictionary);
    return dictionary;
}

int creaEntidad(FILE *DiccionarioDatos, ENTITY NuevaEntidad)
{
    fseek(DiccionarioDatos,0, SEEK_END);
    long DireccionEntidad = ftell(DiccionarioDatos); //direccion de la nueva entidad
    fwrite(&NuevaEntidad.nombre,DATA_BLOCK_SIZE,1, DiccionarioDatos);
    fwrite(&NuevaEntidad.dataPointer,sizeof(long),1, DiccionarioDatos);
    fwrite(&NuevaEntidad.atributos,sizeof(long),1, DiccionarioDatos);
    fwrite(&NuevaEntidad.SigEntidad,sizeof(long),1, DiccionarioDatos);
    return DireccionEntidad;
}

void reordenaEntidades(FILE *diccionarioDatos,ENTITY NuevaEntidad, long NuevaDirEntidad)
{
    long ApuntadorEntidad =0;
    long DirEntidad;
    ENTITY currentEntity;
    fseek(diccionarioDatos,0,SEEK_SET);
    fread(&DirEntidad, sizeof(long),1,diccionarioDatos);

    while(DirEntidad != -1)
    {
        fseek(diccionarioDatos, DirEntidad,SEEK_SET);
        fread(&currentEntity.nombre,DATA_BLOCK_SIZE,1,diccionarioDatos);
        fread(&currentEntity.dataPointer,sizeof(long),1,diccionarioDatos);
        fread(&currentEntity.atributos,sizeof(long),1,diccionarioDatos);
        ApuntadorEntidad= ftell(diccionarioDatos);
        fread(&currentEntity.SigEntidad,sizeof(long),1,diccionarioDatos);
        DirEntidad=currentEntity.SigEntidad;
    }
    fseek(diccionarioDatos, ApuntadorEntidad,SEEK_SET);
    fwrite(&NuevaDirEntidad,sizeof(long),1, diccionarioDatos);
}