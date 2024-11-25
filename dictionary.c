#include "dictionary.h"

FILE *initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;
    printf("Initializing Data Dictionary...\n");
    FILE *dictionary = fopen (dictionaryName, "Wb+");
    fwrite (&mainHeader, sizeof(mainHeader),1, dictionary);
    return dictionary;
}

int appendEntity(FILE *dataDictionary, ENTITY newEntity)
{
    fseek(dataDictionary,0, SEEK_END);
    long entityDirection = ftell(dataDictionary); 

    fwrite(newEntity.name,DATA_BLOCK_SIZE,1, dataDictionary);
    fwrite(&newEntity.dataPointer,sizeof(long),1, dataDictionary);
    fwrite(&newEntity.attributesPointer,sizeof(long),1, dataDictionary);
    fwrite(&newEntity.nextEntity,sizeof(long),1, dataDictionary);

    return entityDirection;
}

void createEntity(FILE *dataDictionary)
{
    ENTITY newEntity;
    printf("Enter the Entity name");
    fgets(newEntity.name, sizeof(newEntity.name),stdin);
    newEntity.dataPointer =EMPTY_POINTER;
    newEntity.attributesPointer =EMPTY_POINTER;
    newEntity.nextEntity =EMPTY_POINTER;

    long attributeDirection= appendEntity(dataDictionary, newAttribute);
    reorderEntities(dataDictionary,current);

}

void createAtribute(FILE *dataDictionary)
{
    ATTRIBUTE newAttribute;

    printf("enter the attribute name");
    fgets(newAttribute.name, sizeof(newAttribute.name),stdin);

    //printf("is primary");
    newAttribute.isPrimary =false;

    newAttribute.type =EMPTY_POINTER;
    newAttribute.size =EMPTY_POINTER;
    newAttribute.nextAtribute =EMPTY_POINTER;

    long attributeDirection= appendEntity(dataDictionary, newAttribute);
    //reorderEntities(dataDictionary,current);

}

void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *neweEntityName, long newEntityDirection)
{
    long currentEntityDIrection =-1;
    
    fseek(dataDictionary,currentEntityPointer,SEEK_SET);
    fread(&currentEntityPointer, sizeof(long),1,dataDictionary);

    if(currentEntityPointer == -1)
    {
        fseek(dataDictionary, currentEntityPointer,SEEK_SET);
        fwrite(&newEntityDirection,sizeof(long),1,dataDictionary);
    }
    else
    {
        char currentEntityName[DATA_BLOCK_SIZE];
        long nextAtributeDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityPointer,SEEK_SET);
        fread(&currentEntityName,sizeof(char),DATA_BLOCK_SIZE,dataDictionary);
        nextHeaderPointer = ftell(dataDictionary);

        if(strcmp(currentEntityName, neweEntityName)<0)
        {
            reorderEntities(dataDictionary, nextHeaderPointer, neweEntityName, newEntityDirection);
        }
        else
        {
            fseek(dataDictionary, currentEntityPointer,SEEK_SET);
            fwrite(&newEntityDirection, sizeof(long),1, dataDictionary);
            fseek(data, newEntityDirection+DATA_BLOCK_SIZE+sizeof(long)*2, SEEK_SET);
            fwrite(&currentEntityDirection, sizeof(long),1, dataDictionary);
        }
    }
   
}

ENTITY removeEntity(FILE *dataDictionary, long currentEntityPointer, const char *EntityName)
{
    long currentEntityDirection =-1;
    
    fseek(dataDictionary,currentEntityPointer,SEEK_SET);
    fread(&currentEntityPointer, sizeof(currentEntityDirection),1,dataDictionary);

    if(currentEntityPointer == -1)
    {
        return ;
    }
    else
    {
        ENTITY resultEntity;
        long nextEntityDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityPointer,SEEK_SET);
        fread(resultEntity.name,sizeof(char),DATA_BLOCK_SIZE,dataDictionary);
        nextHeaderPointer = ftell(dataDictionary)+ (sizeof(long)*2);
        if(strcmp(resultEntity.name,EntityName)==0)
        {
            fread(&resultEntity.dataPointer, sizeof(long),1,dataDictionary);
            fread(&resultEntity.attributesPointer, sizeof(long),1,dataDictionary);
            fread(&resultEntity.nextEntity, sizeof(long),1,dataDictionary);

            fseek(dataDictionary,currentEntityPointer,SEEK_SET);
            fwrite(&resultEntity.nextEntity,sizeof(long),1, dataDictionary);
        }
        else
        {
            removeEntity(dataDictionary, nextHeaderPointer, EntityName);
        }
        
    }
   
}

void reorderAtributes(FILE *dataDictionary, long currentEntityPointer, const char *neweEntityName, long newEntityDirection)
{
    long currentEntityDIrection =-1;
    
   
    fseek(dataDictionary,currentEntityPointer,SEEK_SET);
    fread(&currentEntityPointer, sizeof(long),1,dataDictionary);

    if(currentEntityPointer == -1)
    {
        fseek(dataDictionary, currentEntityPointer,SEEK_SET);
        fwrite(&newEntityDirection,sizeof(long),1,dataDictionary);
    }
    else
    {
        char currentEntityName[DATA_BLOCK_SIZE];
        long nextAtributeDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityPointer,SEEK_SET);
        fread(&currentEntityName,sizeof(char),DATA_BLOCK_SIZE,dataDictionary);
        nextHeaderPointer = ftell(dataDictionary);

        
    }
   
}

int appendAttribute(FILE *dataDictionary, ATTRIBUTE newAttribute)
{
    fseek(dataDictionary,0, SEEK_END);
    long entityDirection = ftell(dataDictionary); //direccion de la nueva entidad

    fwrite(newAttribute.name, DATA_BLOCK_SIZE,1, dataDictionary);
    fwrite(&newAttribute.isPrimary,sizeof(long),1, dataDictionary);
    fwrite(&newAttribute.type,sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.size,sizeof(long),1, dataDictionary);
    fwrite(&newAttribute.nextAtribute,sizeof(long),1, dataDictionary);

    return entityDirection;
}
