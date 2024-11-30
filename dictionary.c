#include "dictionary.h"

 void openDataDictionary()
 {
    char name[50];
    printf("Enter a data Dictionary name:");
    fgets(name, sizeof(name),stdin);
    FILE *dataDictionary = initializeDataDictionary(name);
    entitiesMenu(dataDictionary);
 }

FILE *initializeDataDictionary(const char *dictionaryName) {
    FILE *dictionary = fopen(dictionaryName, "w+");
    if (dictionary == NULL) {
        perror("Error opening file");
        exit(1);
    }

    long mainHeader = EMPTY_POINTER;
    fwrite(&mainHeader, sizeof(long), 1, dictionary);
    return dictionary;
}

int appendEntity(FILE *dataDictionary, ENTITY newEntity) {
    fseek(dataDictionary, 0, SEEK_END);  
    long entityDirection = ftell(dataDictionary); 
    fwrite(newEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);
    fwrite(&newEntity.dataPointer, sizeof(long), 1, dataDictionary);
    fwrite(&newEntity.attributesPointer, sizeof(long), 1, dataDictionary);
    fwrite(&newEntity.nextEntity, sizeof(long), 1, dataDictionary);
    return entityDirection;
}

void createEntity(FILE *dataDictionary) {
    ENTITY newEntity;
    printf("Enter the entity name:\n");
    fgets(newEntity.name, sizeof(newEntity.name), stdin);
    newEntity.name[strcspn(newEntity.name, "\n")] = '\0';  
    newEntity.dataPointer = EMPTY_POINTER;
    newEntity.attributesPointer = EMPTY_POINTER;
    newEntity.nextEntity = EMPTY_POINTER;
    
    long entityDirection = appendEntity(dataDictionary, newEntity);
    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);
}

void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *newEntityName, long newEntityDirection) {
    long nextEntityPointer = -1;
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&nextEntityPointer, sizeof(long), 1, dataDictionary);

    if (nextEntityPointer == -1) {
        fseek(dataDictionary, currentEntityPointer, SEEK_SET);
        fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);
    } else {
        char currentEntityName[DATA_BLOCK_SIZE];
        fseek(dataDictionary, currentEntityPointer + sizeof(long), SEEK_SET);  
        fread(currentEntityName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);

        if (strcmp(currentEntityName, newEntityName) < 0) {
            reorderEntities(dataDictionary, nextEntityPointer, newEntityName, newEntityDirection);
        } else {
            fseek(dataDictionary, currentEntityPointer, SEEK_SET);
            fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);  
            fseek(dataDictionary, newEntityDirection + DATA_BLOCK_SIZE + sizeof(long) * 2, SEEK_SET); 
            fwrite(&nextEntityPointer, sizeof(long), 1, dataDictionary); 
        }
    }
}

void printEntities(FILE *dataDictionary) {
    long currentEntityPointer;
    ENTITY currentEntity;

    
    fseek(dataDictionary, 0, SEEK_SET);
    fread(&currentEntityPointer, sizeof(long), 1, dataDictionary);
    
    if (currentEntityPointer == -1) {
        printf("No entities found.\n");
        return;
    }

    printf("Entities:\n");
    while (currentEntityPointer != -1) {
        fseek(dataDictionary, currentEntityPointer, SEEK_SET);
        fread(&currentEntity, sizeof(ENTITY), 1, dataDictionary);
        printf("Entity Name: %s\n", currentEntity.name);
        currentEntityPointer = currentEntity.nextEntity;
    }
}


void entitiesMenu(FILE *dataDictionary) {
    int option;
    char name[DATA_BLOCK_SIZE];
    ENTITY currentEntity;

    do {
        printf("-----Entities Menu-----\n");
        printf(" 1.- New Entity\n 2.- Delete Entity\n 3.- Print Entities\n 4.- Attributes Menu \n 5.- Finish\n");
        scanf("%d", &option);
        getchar(); 

        switch (option) {
            case 1:
                createEntity(dataDictionary);
                break;
            case 2:
                printf("Enter the name of the entity to delete:\n");
                scanf("%s", name);
                currentEntity = removeEntity(dataDictionary, MAIN_ENTITY_POINTER, name);
                if (currentEntity.name[0] != '\0') {
                    printf("Entity %s removed successfully.\n", currentEntity.name);
                }
                break;
            case 3:
                printEntities(dataDictionary);
                break;
            case 4:
                printf("Enter the entity name:\n");
                scanf("%s", name);
                currentEntity = findEntity(dataDictionary, name);
                if (currentEntity.name[0] != '\0') {
                  Attributes_menu(dataDictionary, currentEntity);
                } else {
                    printf("Entity not found.\n");
                }
                break;
            case 5:
                printf("Exiting menu.\n");
                break;
            default:
                printf("Invalid option, please try again.\n");
                break;
        }

    } while (option != 5);
}
















void createAttribute(FILE *dataDictionary, ENTITY currentEntity)
{
    ATTRIBUTE newAttribute;
    int length;
    printf("Enter the attribute name:\n");
    fgets(newAttribute.name, sizeof(newAttribute.name), stdin);
    printf("Is primary key? 1:Yes, 0:No");
    scanf("%d" , &newAttribute.isPrimary);
    printf("Atribute type?:\n 1- Int\n 2- Float\n 3- Char\n 4.- Long\n 5-Bool\n");
    scanf("%d" , &newAttribute.type);

    switch (newAttribute.type)
    {
    case 1:
            newAttribute.size = sizeof(int);
            break;
    case 2:
            newAttribute.size = sizeof(float);
            break;
    case 3:
            printf("string length?");
            scanf("%d", & length);
            newAttribute.size  = sizeof(char)* length;
            break;
    case 4:
            newAttribute.size  = sizeof(long);
            break;
    case 5:
            newAttribute.size  = sizeof(bool);
            break;
    }
    

    newAttribute.nextAtribute =EMPTY_POINTER;
    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAtributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);
}


void removeAttribute(FILE *dataDictionary, long currentEntityPointer, const char *AttributeName)
{
    ENTITY currentEntity;
    long currentAttributePointer;
    ATTRIBUTE currentAtribute;
    fseek(dataDictionary,currentEntityPointer,SEEK_SET);
    fread(&currentEntity,sizeof(ENTITY),1,dataDictionary);

    currentAttributePointer=currentEntity.attributesPointer;

    if(currentAttributePointer ==-1)
    {
        return;
    }
    else
    {
        while(currentAttributePointer != -1)
        {
            fseek(dataDictionary,currentAttributePointer,SEEK_SET);
            fread(&currentAtribute,sizeof(ATTRIBUTE),1,dataDictionary);

            if(strcmp(AttributeName, currentAtribute.name) ==0);
            {
                long nextAtribute;
                fseek(dataDictionary,currentAttributePointer,SEEK_SET);
                fread(&nextAtribute,sizeof(long),1,dataDictionary);

                fseek(dataDictionary, currentEntityPointer + sizeof(ENTITY) ,SEEK_SET);
                fwrite(&currentAtribute.nextAtribute,sizeof(long),1,dataDictionary);
                return;
            }
            fread(&currentAttributePointer,sizeof(long),1,dataDictionary);

            currentAttributePointer= currentAtribute.nextAtribute;
        }
    }

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
    long entityDirection = ftell(dataDictionary); 

    fwrite(newAttribute.name, DATA_BLOCK_SIZE,1, dataDictionary);
    fwrite(&newAttribute.isPrimary,sizeof(long),1, dataDictionary);
    fwrite(&newAttribute.type,sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.size,sizeof(long),1, dataDictionary);
    fwrite(&newAttribute.nextAtribute,sizeof(long),1, dataDictionary);

    return entityDirection;
}





void printAttributes(FILE *dataDictionary, ENTITY CurrentEntity) {
   long currentAttributePointer = CurrentEntity.attributesPointer;

    if (currentAttributePointer == -1) {
        printf("Entidad '%s' no tiene atributos.\n", CurrentEntity.name);
        return;
    }

    ATTRIBUTE CurrentAttribute;
    while (currentAttributePointer != -1) {
        fseek(dataDictionary, currentAttributePointer, SEEK_SET);
        fread(&CurrentAttribute, sizeof(ATTRIBUTE), 1, dataDictionary);

        printf("  Name: %s\n", CurrentAttribute.name);
      //  printf("  primary key: %s\n", CurrentAttribute.isPrimary);

        currentAttributePointer = CurrentAttribute.nextAtribute;
    }
}
 

 
void Attributes_menu(FILE *dataDictionary, ENTITY currentEntity)
{
    int option;
    char name[DATA_BLOCK_SIZE];
    do
    {
        printf("-----Entities Menu-----\n");
        printf(" 1.- New Attribute\n 2.- Delete Attribute\n 3.- Print Attributes\n 4.- Exit\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            getchar();  
            createAttribute(dataDictionary, currentEntity); 
            break;
        case 2:
            printf("Enter name of the attribute to delete:\n");
            scanf("%s", name);
            removeAttribute(dataDictionary, currentEntity.dataPointer, name);  
            break;
        case 3:
            printAttributes(dataDictionary, currentEntity);  
            break;
        case 4:
            printf("Exiting attribute menu...\n");
            break;

        default:
            printf("Invalid option, please try again.\n");
            break;
        }

    } while (option != 4);
}


 void   main_menu() 
 {
    int option;
        do
        {
             printf("-----Main menu-----\n");
             printf(" 1.- New data dictionary \n 2.- open data dictionary \n 3.- finish\n");
             scanf("%d", &option);
            
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


