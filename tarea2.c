#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>




typedef struct {
  struct Nodo* padre;
  char tipo[64];
  void* contenido;
} Nodo;

typedef struct {
  int largoactual;
  int largomaximo;
  Nodo* arreglo;
} Lista;

typedef struct {
  char nombre[128];
  Lista* hijos;
} Directorio;

typedef struct {
  char nombre[128];
  char contenido[256];
} Archivo;


// Crea una nueva lista.
Lista* crear_lista(int largo_maximo_inicial){
Lista* lista = (Lista*)malloc(sizeof(Lista));
lista->largoactual=0;
lista->largomaximo=largo_maximo_inicial;
lista->arreglo=(Nodo**)malloc(largo_maximo_inicial * sizeof(Nodo*));
return lista;
}

// Agrega el nodo en la posicion lista.largo_actual de lista.arreglo.
// Si lista.largo_actual >= lista.largo_maximo, entonces se duplica
// el tamaño del arreglo y se actualiza lista.largo_maximo.
void insertar_lista(Lista* lista, Nodo* nodo){
    if (lista->largoactual >= lista->largomaximo){
        int largo_nuevo = lista->largomaximo * 2;
        Nodo** nuevo_arreglo = (Nodo**)malloc(largo_nuevo * sizeof(Nodo*));

        for (int i = 0 ; i<lista->largomaximo ;i++) {
        nuevo_arreglo[i] = &lista->arreglo[i]; 
        }
        free(lista->arreglo);
        lista->arreglo = nuevo_arreglo;
        lista->largomaximo = largo_nuevo;
    }
    lista->arreglo[lista->largoactual] = *nodo;
    lista->largoactual++;
}

void insertar_listaa(Lista *lista, Nodo *nodo) {
    if (lista->largoactual >= lista->largomaximo) {
        lista->largomaximo *= 2;
        lista->arreglo = realloc(lista->arreglo, lista->largomaximo * sizeof(Nodo *));
    }
    lista->arreglo[lista->largoactual++] = *nodo;
}



Nodo *buscar_elemento(Directorio *actual, char *nombre, bool es_directorio) {
    for (int i = 0; i < actual->hijos->largoactual; i++) {
        Nodo *nodo = &actual->hijos->arreglo[i];
        if (es_directorio && strcmp(nodo->tipo, "Directorio") == 0) {
            Directorio *dir = (Directorio *)nodo->contenido;
            if (strcmp(dir->nombre, nombre) == 0) {
                return nodo;
            }
        } else if (!es_directorio && strcmp(nodo->tipo, "Archivo") == 0) {
            Archivo *archivo = (Archivo *)nodo->contenido;
            if (strcmp(archivo->nombre, nombre) == 0) {
                return nodo;
            }
        }
    }
    printf("no se encontro lo buscado");
    return NULL;
}


Nodo *buscar_directorioo(Directorio *actual, char *nombre) {
    return buscar_elemento(actual, nombre, true);
}

Nodo *buscar_archivoo(Directorio *actual, char *nombre) {
    return buscar_elemento(actual, nombre, false);
}






// Retorna el hijo del directorio actual de un nombre dado.
Nodo* buscar_directorio(Directorio* actual, char* nombre){
    Lista* hijos= actual->hijos;

    for (int i =0; i<hijos->largoactual ; i++   ){
        Nodo* nodo_acutal = &hijos->arreglo[i];
        Directorio* dir_actual = (Directorio*) nodo_acutal->contenido;
        if(strcmp(dir_actual->nombre,nombre ) == 0){
            return nodo_acutal;

        }
    }
printf("no se encontro el directorio deseado");
return NULL;
}






// Retorna el archivo del directorio actual de un nombre dado.
Nodo* buscar_archivo(Directorio* actual, char* nombre){
    Lista *hijos = actual->hijos;

    for( int i = 0 ; i<hijos->largoactual; i++){
        Nodo *nodo_actual = &hijos->arreglo[i];
        
        if(strcmp(nodo_actual->contenido, "Archivo") == 0 && strcmp(((Archivo*)nodo_actual->contenido)->nombre , nombre)== 0)
        return nodo_actual;

    }
    return NULL;


}

// Crea un nuevo Nodo
Nodo* crear_nodo(Nodo* padre, char* tipo, char* nombre){
    Nodo *nodo = malloc(sizeof(Nodo));
    nodo->padre = padre;
    strcpy(nodo->tipo,tipo);

    if(strcmp(tipo,"Directorio") == 0){
        Directorio *dir = malloc(sizeof(Directorio));
        strcpy(dir->nombre,nombre);
        dir->hijos = crear_lista(10);
        nodo->contenido = dir;

    }
    
    else {
        Archivo *archivo = malloc(sizeof(Archivo));
        strcpy(archivo->nombre, nombre);
        archivo->contenido[0] = '\0';
        nodo->contenido = archivo;
        }
    return nodo;

}

// Crea un directorio y lo agrega como hijo del actual.
void mkdir(Nodo* actual, char* nombre_directorio){
    Directorio *dir_actual = (Directorio *)actual->contenido;
    Nodo *nodo = crear_nodo(dir_actual,"Directorio",nombre_directorio);
    insertar_lista(dir_actual->hijos,nodo);
}

// Crea un archivo y lo agrega como hijo del directorio actual.
void touch(Nodo* actual, char* nombre_archivo){
    Directorio *dir_actual = (Directorio *)actual->contenido;
    Nodo *nodo = crear_nodo(dir_actual,"Archivo",nombre_archivo);
    insertar_lista(dir_actual->hijos,nodo);
}

// Busca un archivo y cambia su contenido.
void write(Nodo* actual, char* nombre_archivo, char* contenido){
    Directorio* directorio_actual = (Directorio*)actual->contenido;
    Nodo* archivo_nodo = buscar_archivo(directorio_actual, nombre_archivo);
    // Si se encontró el archivo, actualizar su contenido
    if (archivo_nodo != NULL) {
        Archivo* archivo = (Archivo*)archivo_nodo->contenido;
        strncpy(archivo->contenido, contenido, sizeof(archivo->contenido) - 1);
        archivo->contenido[sizeof(archivo->contenido) - 1] = '\0';
    } else {
        printf("Archivo '%s' no encontrado en el directorio actual.\n", nombre_archivo);
    }    
}

// Busca un archivo e imprime su contenido.
void cat(Nodo* actual, char* nombre_archivo){
    Directorio* dir_actual = (Directorio*)actual->contenido;
    Nodo *archivo_nodo = buscar_archivo(dir_actual,nombre_archivo);
    if (archivo_nodo != NULL){
        Archivo* archivo = (Archivo*)archivo_nodo->contenido;
        print("%s\n",archivo->contenido);
    }else{
        printf("Archivo '%s' no encontrado en el directorio actual.\n", nombre_archivo);
    }
}

// Imprime todos los hijos del directorio actual o del seleccionado.
void ls(Nodo* actual){
    Directorio *dir_actual = (Directorio*)actual->contenido;
    Lista* lista = dir_actual->hijos;
    for(int i = 0; i<lista->largomaximo;i++){
        Nodo *nodo_actual = &lista->arreglo[i];
        if(strcmp(nodo_actual->tipo, "Directorio") == 0){
            pritnf("./%s\n",((Directorio*)nodo_actual->contenido)->nombre);
        }
        else if (strcmp(nodo_actual->tipo,"Archivo") == 0)
        {
            printf("./%s\n",((Archivo*)nodo_actual->contenido)->nombre);

        }
        
    }
}
void ls_dir(Nodo* actual, char* nombre_directorio){
    if(strcmp(nombre_directorio,".") == 0 ){
        ls(actual);
    }
    else{
        Directorio *dir_buscado = (Directorio*)actual->contenido;
        Nodo *directorio_nodo = buscar_directorio(dir_buscado,nombre_directorio);
        if( directorio_nodo != NULL){
            ls(directorio_nodo);
        } 
        else{
            print("directorio buscado no existente")
        }
    }
    





}

// Aplica la instrucción a todos los subdirectorios.
void mapdir(Nodo* actual, void (*instruccion)(Nodo*, char*), char* parametro_instruccion);