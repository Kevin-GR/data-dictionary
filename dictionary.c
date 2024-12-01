#include "dictionary.h"

FILE *initializeDataDictionary(const char *dictionaryName) 
{
    long mainHeader = EMPTY_POINTER;
    FILE *dictionary = fopen (dictionaryName, "w+");
    fwrite (&mainHeader, sizeof(mainHeader),1, dictionary);
    return dictionary;  
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

int appendEntity(FILE *dataDictionary, ENTITY newEntity)
{
    fseek(dataDictionary, 0, SEEK_END);
    long entityDirection = ftell(dataDictionary); 
    fwrite(newEntity.name, sizeof(ENTITY), 1, dataDictionary);
    fwrite(&newEntity.dataPointer, sizeof(ENTITY), 1, dataDictionary);
    fwrite(&newEntity.attributesPointer, sizeof(ENTITY), 1, dataDictionary);
    fwrite(&newEntity.nextEntity, sizeof(ENTITY), 1, dataDictionary);

    return entityDirection;
}

void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *newEntityName, long newEntityDirection)
{
    long currentEntityDirection = -1;
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary); 

    if (currentEntityDirection == -1)  
    {
        fseek(dataDictionary, currentEntityPointer, SEEK_SET);
        fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);  
    }
    else
    {
        char currentEntityName[DATA_BLOCK_SIZE];
        long nextEntityDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityDirection, SEEK_SET);  
        fread(&currentEntityName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);  
        nextHeaderPointer= ftell(dataDictionary)+ (sizeof(long)*2);


        if (strcmp(currentEntityName, newEntityName) > 0)
        {

            fseek(dataDictionary, currentEntityPointer, SEEK_SET);
            fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);  
            fseek(dataDictionary, newEntityDirection + DATA_BLOCK_SIZE + sizeof(long) * 2, SEEK_SET); 
            fwrite(&currentEntityDirection, sizeof(long), 1, dataDictionary); 
        }
        else
        {

            reorderEntities(dataDictionary, nextHeaderPointer, newEntityName, newEntityDirection);
        }
    }
}


ENTITY removeEntity(FILE *dataDictionary,long currentEntityPointer, const char *entityName) {

 /*  long currentEntityDirection = -1;
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

    if(currentEntityDirection == -1)
    return;

    else
    {
        ENTITY resultEntity;
        long nextEntityDirection;
        long nextHeaderPointer;
   
        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(&resultEntity.name, sizeof(char),DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary)+ (sizeof(long)*2);
        if(strcmp(resultEntity.name,entityName)==0)
        {
            fread(&resultEntity.dataPointer, sizeof(long),1,dataDictionary);
            fread(&resultEntity.attributesPointer, sizeof(long),1,dataDictionary);
            fread(&resultEntity.nextEntity, sizeof(long),1,dataDictionary);

            fseek(dataDictionary,currentEntityPointer, SEEK_SET);
            fwrite(&resultEntity.nextEntity, sizeof(long),1,dataDictionary);

            return resultEntity;
        }
        else
        {
             return removeEntity(dataDictionary,nextHeaderPointer,entityName);   
        }
    }

    printf("Entity '%s' not found.\n", entityName);*/
}

void entitiesMenu(FILE *dataDictionary)
{
    int option;
    char name[DATA_BLOCK_SIZE];
    ENTITY currentEntity;

    do {
        printf("-----Entities Menu-----\n");
        printf(" 1.- New Entity\n 2.- Delete Entity\n 3.- Print Entities\n 4.- Attributes Menu\n 5.- Finish\n");
        scanf("%d", &option);
        getchar();  

        switch (option) {
            case 1:
                createEntity(dataDictionary);
                break;
            case 2:
                printf("Enter the name of the entity to delete:\n");
                scanf("%s", name);
               // removeEntity(dataDictionary, name);
              
                break;
            case 3:
                printEntities(dataDictionary);
                break;
            case 4:
                printf("Enter the entity name:\n");
                scanf("%s", name);
                currentEntity = findEntity(dataDictionary, name);
                if (currentEntity.name[0] != '\0') {
                    printf("%ld", currentEntity.attributesPointer);
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

    fclose(dataDictionary);
}


void printEntities(FILE *dataDictionary) {
    fseek(dataDictionary, 0, SEEK_SET);
    long currentEntityDirection = -1;
    fread(&currentEntityDirection, sizeof(long), 1, dataDictionary);
    printf("--------List of entities--------\n");
    while (currentEntityDirection != -1) {
        ENTITY entity;
        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(entity.name, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        fread(&entity.dataPointer, sizeof(long), 1, dataDictionary);
        fread(&entity.attributesPointer, sizeof(long), 1, dataDictionary);
        fread(&entity.nextEntity, sizeof(long), 1, dataDictionary);

        printf("\tEntity Name: %s\n", entity.name);
        currentEntityDirection = entity.nextEntity;
    }
}

 ENTITY findEntity(FILE *dataDictionary,const char *name)
 {
    ENTITY currentEntity;
    fseek(dataDictionary,0,SEEK_SET);

    while(fread(&currentEntity,sizeof(ENTITY),1,dataDictionary) == 1)
    {
        if(strcmp(currentEntity.name, name) == 0)
        printf("%ld",currentEntity.dataPointer);
        return currentEntity;
    }
 }


























void createAttribute(FILE *dataDictionary, ENTITY currentEntity)
{
    ATTRIBUTE newAttribute;
    int length, aux;
    getchar();
    printf("Enter the entity name:\n");
    fgets(newAttribute.name, sizeof(newAttribute.name), stdin);
    printf("Is primary key? 1:Yes, 0:No");
    scanf("%d", &aux);
    newAttribute.isPrimary= aux;
    printf("Atribute type?:\n 1- Int\n 2- Float\n 3- Char\n 4.- Long\n 5-Bool\n");
    scanf("%ld" , &newAttribute.type);

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
    newAttribute.nextAtribute = EMPTY_POINTER;
    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAtributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);

}

long appendAttribute(FILE *dataDictionary, ATTRIBUTE newAttribute)
{
    fseek(dataDictionary,0, SEEK_END);
    long AttibuteDirection = ftell(dataDictionary); 
    fwrite(newAttribute.name, DATA_BLOCK_SIZE,1, dataDictionary);
    fwrite(&newAttribute.isPrimary,sizeof(bool),1, dataDictionary);
    fwrite(&newAttribute.type,sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.size,sizeof(long),1, dataDictionary);
    fwrite(&newAttribute.nextAtribute,sizeof(long),1, dataDictionary);   
    return AttibuteDirection;
}


void reorderAtributes(FILE *dataDictionary, long currentAttributePointer, const char *newAttributeName, long newAttributeDirection)
{
    long currentAttributeDirection =-1;
    fseek(dataDictionary, currentAttributePointer,SEEK_SET);
    fread(&currentAttributeDirection, sizeof(currentAttributeDirection),1,dataDictionary);

    if(currentAttributePointer == -1)
    {
        fseek(dataDictionary, currentAttributePointer,SEEK_SET);
        fwrite(&newAttributeDirection,sizeof(long),1,dataDictionary);
    }
    else
    {
        char currentAttributeName[DATA_BLOCK_SIZE];
        long nextAtributeDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentAttributeDirection,SEEK_SET);
        fread(&currentAttributeName,sizeof(char),DATA_BLOCK_SIZE,dataDictionary);
        nextHeaderPointer = ftell(dataDictionary)+sizeof(bool)+(sizeof(long)*2);

       if(strcmp(currentAttributeName, newAttributeName) < 0){
				
				reorderAtributes(dataDictionary,nextHeaderPointer,newAttributeName,newAttributeDirection);
			}
			else{

				fseek(dataDictionary,currentAttributePointer,SEEK_SET);
				fwrite(&newAttributeDirection, sizeof(long),1,dataDictionary);

				fseek(dataDictionary,newAttributeDirection + 50 + sizeof(bool) + (sizeof(long) * 2),SEEK_SET);
				fwrite(&currentAttributeDirection, sizeof(long),1,dataDictionary);
			}
    }
   
}

void removeAttribute(FILE *dataDictionary, long currentEntityPointer, const char *AttributeName) {
    ENTITY currentEntity;
    long currentAttributePointer;
    ATTRIBUTE currentAttribute;

    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntity, sizeof(ENTITY), 1, dataDictionary);

    currentAttributePointer = currentEntity.attributesPointer;

    if (currentAttributePointer == -1) {
        return;
    } else {
        while (currentAttributePointer != -1) {
            fseek(dataDictionary, currentAttributePointer, SEEK_SET);
            fread(&currentAttribute, sizeof(ATTRIBUTE), 1, dataDictionary);

            if (strcmp(AttributeName, currentAttribute.name) == 0) {
                long nextAttribute;
                fseek(dataDictionary, currentAttributePointer, SEEK_SET);
                fread(&nextAttribute, sizeof(long), 1, dataDictionary);

                fseek(dataDictionary, currentEntityPointer + sizeof(ENTITY), SEEK_SET);
                fwrite(&currentAttribute.nextAtribute, sizeof(long), 1, dataDictionary);
                return;
            }

            fseek(dataDictionary, currentAttributePointer + sizeof(ATTRIBUTE), SEEK_SET);
            fread(&currentAttributePointer, sizeof(long), 1, dataDictionary);

            currentAttributePointer = currentAttribute.nextAtribute;
        }
    }
}

void printAttributes(FILE *dataDictionary, ENTITY CurrentEntity) {
   long currentAttributePointer = CurrentEntity.attributesPointer;
    printf("--------List of Attributes for Entity: %s--------\n", CurrentEntity.name);
    while (currentAttributePointer != -1) {
        ATTRIBUTE CurrentAttribute;
        fseek(dataDictionary,currentAttributePointer,SEEK_SET);
        fread(&CurrentAttribute, sizeof(ATTRIBUTE),1,dataDictionary);
        
        printf("Attribute Name: %s\n", CurrentAttribute.name);
        if(CurrentAttribute.isPrimary ==1)
        printf("Is Primary: Yes %s\n");
        else
        printf("Is Primary: No %s\n");
        printf("Type: %ld\n", CurrentAttribute.type);
        printf("Size: %ld\n", CurrentAttribute.size);
        currentAttributePointer = CurrentAttribute.nextAtribute;
    }

    
}
 
void Attributes_menu(FILE *dataDictionary, ENTITY currentEntity)
{
    printf("%ld", currentEntity.attributesPointer);
    printf("%ld", currentEntity.dataPointer);
   
    int option;
    char name[DATA_BLOCK_SIZE];
    do
    {
        printf("-----Attributes Menu-----\n");
        printf(" 1.- New Attribute\n 2.- Delete Attribute\n 3.- Print Attributes\n 4.- Exit\n");
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            createAttribute(dataDictionary, currentEntity); 
            break;
        case 2:
            printf("Enter name of the attribute to delete:\n");
            scanf("%s", name);
            removeAttribute(dataDictionary, currentEntity.dataPointer, name);  
            break;
        case 3:
            printf("%ld", currentEntity.attributesPointer);
            printAttributes(dataDictionary, currentEntity);  
            break;
        case 4:
            printf("Exit to attribute menu...\n");
            break;

        default:
            printf("Invalid option, please try again.\n");
            break;
        }

    } while (option != 4);
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
    entitiesMenu(dataDictionary);
 }