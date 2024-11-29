#include "dictionary.h"

FILE *initializeDataDictionary(const char *dictionaryName) {
    long mainHeader = EMPTY_POINTER;
    printf("Initializing Data Dictionary...\n");
    FILE *dictionary = fopen (dictionaryName, "w+");
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

    printf("Enter the entity name:\n");
    fgets(newEntity.name, sizeof(newEntity.name), stdin);
    newEntity.dataPointer= EMPTY_POINTER;
    newEntity.attributesPointer= EMPTY_POINTER;
    newEntity.nextEntity= EMPTY_POINTER;

    long entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);
}

void createAtribute(FILE *dataDictionary, ENTITY currentEntity)
{
    ATTRIBUTE newAttribute;
    printf("Enter the attribute name:\n");
    fgets(newAttribute.name, sizeof(newAttribute.name), stdin);
    newAttribute.isPrimary = false;
    newAttribute.type =1;
    newAttribute.size = sizeof(int);

    newAttribute.nextAtribute =EMPTY_POINTER;

    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAtributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);
}


void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *neweEntityName, long newEntityDirection)
{
    long currentEntityDirection =-1;
    
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
            fseek(dataDictionary, newEntityDirection+DATA_BLOCK_SIZE+sizeof(long)*2, SEEK_SET);
            fwrite(&currentEntityDirection, sizeof(long),1, dataDictionary);
        }
    }
   
}


ENTITY removeEntity(FILE *dataDictionary, long currentEntityPointer, const char *EntityName)
{
    /*long currentEntityDirection =-1;
    
    fseek(dataDictionary,currentEntityPointer,SEEK_SET);
    fread(&currentEntityPointer, sizeof(currentEntityDirection),1,dataDictionary);

    if(currentEntityPointer == -1)
    {
        return 0;
    }
    else
    {
        char currentEntityName[DATA_BLOCK_SIZE];
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
            fseek(dataDictionary,currentEntityPointer,SEEK_SET);
            fwrite(&newEntityDirection, sizeof(long),1,dataDictionary);
            fseek(dataDictionary,newEntityDirection+DATA_BLOCK_SIZE+(sizeof(long)*2),SEEK_SET);
            fwrite(&currentEntityDirection, sizeof(long),DATA_BLOCK_SIZE,dataDictionary);
            
        }
        
    }
   */
}


void reorderAtributes(FILE *dataDictionary, long currentAttributePointer, const char *newAttributeName, long newAttributeDirection)
{
    long currentAttributeDirection =-1;
    
   
    fseek(dataDictionary,currentAttributeDirection,SEEK_SET);
    fread(&currentAttributePointer, sizeof(long),1,dataDictionary);

    if(currentAttributePointer == -1)
    {
        fseek(dataDictionary, currentAttributePointer,SEEK_SET);
        fwrite(&newAttributeName,sizeof(long),1,dataDictionary);
    }
    else
    {
        char currentAttributeName[DATA_BLOCK_SIZE];
        long nextAtributeDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentAttributePointer,SEEK_SET);
        fread(&currentAttributeName,sizeof(char),DATA_BLOCK_SIZE,dataDictionary);
        nextHeaderPointer = ftell(dataDictionary)+ sizeof(bool)+ (sizeof(long)*2);

        if(strcmp(currentAttributeName, newAttributeName)<0)
        {
            reorderAtributes(dataDictionary, nextHeaderPointer,newAttributeName,newAttributeDirection);
        }
        else
        {
            fseek(dataDictionary, currentAttributePointer,SEEK_SET);
            fwrite(&newAttributeDirection,sizeof(long),1, dataDictionary);
            fseek(dataDictionary, newAttributeDirection+DATA_BLOCK_SIZE+(sizeof(long)*2),SEEK_SET);
            fwrite(&currentAttributeDirection, sizeof(long),1,dataDictionary);
        }
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

void printEntityData(FILE *dataDictionary, long currentEntityPointer)
{
    long currentEntityDirection =-1;
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection),1,dataDictionary);
    if(currentEntityDirection == -1)
    {
        return;
    }
    else
    {
        long blocksize;
        fseek(dataDictionary,currentEntityDirection, SEEK_SET);
    //    fread(&curre);
    }
}

void  printDataRecords(FILE *dataDictionary, long attributesPointer,long currentRecordPointer, int blockSize)
{
    long currentRecordDirection;
    fseek(dataDictionary,currentRecordPointer, SEEK_SET);
    fread(&currentRecordDirection, sizeof(currentRecordDirection),1,dataDictionary);

    if(currentRecordDirection ==-1)
    {
        return;
    }
    else
    {
        long nextRecordPointer =currentRecordDirection+blockSize;
        int defaultOffset=0;
        printf("\t\t");
        //printRecordData(dataDictionary,attributesPointer, currentRecordDirection, defaultOffset);
        printf("\n");
     //   printRecordData(dataDictionary,attributesPointer, nextRecordPointer, blockSize);
    }
}
/*
 void printRecordData(FILE *dataDictionary, long currentAttributePointer, long currentRecordDirection, blockSize)
{
    long currentRecordDirection;
    fseek(dataDictionary,currentRecordPointer, SEEK_SET);
    fread(&currentRecordDirection, sizeof(currentRecordDirection),1,dataDictionary);

    if(currentRecordDirection ==-1)
    {
        return;
    }
    else
    {
        char  currentAttributename[NAME_LENGHT];
        long nextAttributePointer;

        int atr
        long nextRecordPointer =currentRecordDirection+blockSize;
        int defaultOffset=0;
        printf("\t\t");
        printRecordData(dataDictionary,attributesPointer, currentRecordDirection, defaultOffset);
        printf("\n");
        printRecordData(dataDictionary,attributesPointer, nextRecordPointer, blockSize);
    }
}*/

 
 void Entities_menu(FILE *dataDictionary)
 {
    int option;
        do
        {
             printf("-----Entities menu-----\n");
       
             printf("1.- New Entity\n 2.- Delete entity\n 3.- Print entities \n 4.- Menu attributes  5.- finish");
             scanf("%d", &option);
            getchar();
            switch (option)
            {
            case 1:
                createEntity(dataDictionary);
                break;
            case 2:
                removeEntity(dataDictionary,);
                break;
            case 3:
                createEntity(dataDictionary);
                break;
            case 4:
                createEntity(dataDictionary);
                break;
            case 5:
                createEntity(dataDictionary);
                break;
            
            default:
                break;
            }

        }while (option != 5);
 }

 void Attributes_menu(FILE *dataDictionary)
 {
    int option;
        do
        {
             printf("-----Entities menu-----\n");
       
             printf(" 1.- New Entity\n 2.- Delete entity\n 3.- Print entities \n 4.- Menu attributes\n  5.- finish\n");
             scanf("%d", &option);

            switch (option)
            {
            case 1:
                createEntity(dataDictionary);
                break;
            case 2:
                createEntity(dataDictionary);
                break;
            case 3:
                createEntity(dataDictionary);
                break;
            case 4:
                createEntity(dataDictionary);
                break;
            case 5:
                createEntity(dataDictionary);
                break;
            
            default:
                break;
            }

        }while (option != 5);
 }

 void  main_menu()
 {
    int option;
   

              do
        {
             printf("-----Main menu-----\n");
             printf(" 1.- New data dictionary \n 2.- open data dictionary \n 3.- finish\n");
             scanf("%d", &option);
            getchar();
            switch (option)
            {
            case 1:
                openDataDictionary();
                break;
            case 2:
                openDataDictionary();
                break;
            default:
                break;
            }

        }while (option != 3);
 }

 void openDataDictionary()
 {
    char name[50];
    printf("Enter a data Dictionary name:");
    fgets(name, sizeof(name),stdin);
    FILE *dataDictionary = initializeDataDictionary(name);
    Attributes_menu(dataDictionary);
 }