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
    fwrite(newEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);
    fwrite(&newEntity.dataPointer, sizeof(long), 1, dataDictionary);
    fwrite(&newEntity.attributesPointer, sizeof(long), 1, dataDictionary);
    fwrite(&newEntity.nextEntity, sizeof(long), 1, dataDictionary);
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

void removeEntity(FILE *dataDictionary, const char *entityName) {
    long currentEntityDirection = -1;
    long previousEntityDirection = -1;
    ENTITY resultEntity;

    // Leer la dirección de la primera entidad
    fseek(dataDictionary, 0, SEEK_SET);
    fread(&currentEntityDirection, sizeof(long), 1, dataDictionary);
    printf("Starting at head entity direction: %ld\n", currentEntityDirection);

    // Recorremos las entidades mientras no lleguemos al final de la lista
    while (currentEntityDirection != -1) {
        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(&resultEntity, sizeof(ENTITY), 1, dataDictionary);

        // Eliminar el salto de línea al final del nombre si lo tiene
        resultEntity.name[strcspn(resultEntity.name, "\n")] = 0;

        // Imprimir nombre y dirección de la entidad para depuración
        printf("Checking entity: %s, at direction %ld\n", resultEntity.name, currentEntityDirection);

        // Si encontramos la entidad a eliminar
        if (strcmp(resultEntity.name, entityName) == 0) {
            printf("Found entity: %s, removing...\n", entityName);

            // Si la entidad a eliminar es la primera (cabeza de lista)
            if (previousEntityDirection == -1) {
                // Actualizamos la cabeza de la lista, que es el primer puntero de la lista
                fseek(dataDictionary, 0, SEEK_SET);
                fwrite(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary);
                printf("Updated head entity pointer to %ld\n", resultEntity.nextEntity);
            } else {
                
                fseek(dataDictionary, previousEntityDirection + DATA_BLOCK_SIZE + sizeof(long), SEEK_SET);
                fwrite(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary);
                printf("Updated previous entity's next pointer to %ld\n", resultEntity.nextEntity);
            }

            printf("Entity '%s' removed.\n", entityName);
            return;  
        }


        previousEntityDirection = currentEntityDirection;
        currentEntityDirection = resultEntity.nextEntity;
    }

    // Si no encontramos la entidad
    printf("Entity '%s' not found.\n", entityName);
}



void entitiesMenu(FILE *dataDictionary)
{
    int option;
    char name[DATA_BLOCK_SIZE];
    ENTITY currentEntity;
    int dir;
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
                 fgets(name, sizeof(name), stdin);
                 name[strcspn(name, "\n")] = 0;
                 removeEntity(dataDictionary, name);
                break;
            case 3:
                printEntities(dataDictionary);
                break;
            case 4:
                 printf("Name of the entity: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; 

                 dir = findEntity(dataDictionary, name);
                 ENTITY currentEntity;
                 fseek(dataDictionary, dir, SEEK_SET);  
                 fread(&currentEntity, sizeof(ENTITY), 1, dataDictionary);  


    printf("Entity Name: %s\n", currentEntity.name);
    printf("Attributes Pointer: %ld\n", currentEntity.attributesPointer);
    printf("Data Pointer: %ld\n", currentEntity.dataPointer);
    printf("Next Entity Pointer: %ld\n", currentEntity.nextEntity);
    Attributes_menu(dataDictionary, currentEntity);

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

        printf("\tEntity Name: %s \t %ld \t %ld, \t %ld \n", entity.name, entity.attributesPointer, entity.dataPointer, entity.nextEntity);
        currentEntityDirection = entity.nextEntity;
    }
}

int findEntity(FILE *dataDictionary, const char *name) {
    long currentEntityDirection = -1;
    ENTITY resultEntity;

    // Empezamos leyendo la dirección del primer bloque de la lista
    fseek(dataDictionary, 0, SEEK_SET);
    fread(&currentEntityDirection, sizeof(long), 1, dataDictionary);

    while (currentEntityDirection != EMPTY_POINTER) {
        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(&resultEntity, sizeof(ENTITY), 1, dataDictionary);

        resultEntity.name[strcspn(resultEntity.name, "\n")] = 0;

        // Comparamos el nombre de la entidad con el nombre buscado
        if (strcmp(resultEntity.name, name) == 0) {
            printf("\nEntity '%s' found at direction %ld.\n", resultEntity.name, currentEntityDirection);
            return currentEntityDirection;  
        }

        // Continuamos con la siguiente entidad
        currentEntityDirection = resultEntity.nextEntity;
    }

    return 0;
}




void createAttribute(FILE *dataDictionary, ENTITY currentEntity)
{
    ATTRIBUTE newAttribute;
    int length, aux;
    getchar();
    printf("Enter the entity name:\n");
    scanf("%s",newAttribute.name);
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

    if(currentAttributeDirection == -1L)
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
				
				fseek(dataDictionary,newAttributeDirection + DATA_BLOCK_SIZE + sizeof(bool) + (sizeof(long) * 2),SEEK_SET);
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
        printf("Is Primary: Yes \n");
        else
        printf("Is Primary: No ");
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
        printf("-----Attributes of %s----\n", currentEntity.name);
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

 