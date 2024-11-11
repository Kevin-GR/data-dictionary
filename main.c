#include "main.h"

int main(int argc, char** argv) {
    char nombre[50];

    printf("inserta el nombre del diccionario:");

    fgets(nombre, sizeof(nombre),stdin);

    FILE *dataDictionary = initializeDataDictionary(nombre);

    ENTITY NuevaEntidad;

    printf("Inserta el nombre de la entidad");
    fgets(NuevaEntidad.nombre, sizeof(NuevaEntidad.nombre), stdin);
    NuevaEntidad.dataPointer= EMPTY_POINTER;
    NuevaEntidad.atributos= EMPTY_POINTER;
    NuevaEntidad.SigEntida= EMPTY_POINTER;
    
    creaEntidad(dataDictionary, NuevaEntidad);
    fclose(dataDictionary);
    return 0;
}