#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "list.h"

typedef struct
{
  char* nombre;
  char* marca;
  char* tipo;
  int stock;
  int precio;
} tipoProducto;

typedef struct
{
  List* lista;
  char* nombre;
  int cantidad;
} tipoLista;

/*
  función para comparar claves de tipo string
  retorna 1 si son iguales
*/
int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo string
  retorna 1 si son key1<key2
*/
int lower_than_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son iguales
*/
int is_equal_int(void * key1, void * key2) {
    if(*(int*)key1 == *(int*)key2) return 1;
    return 0;
}

/*
  función para comparar claves de tipo int
  retorna 1 si son key1<key2
*/
int lower_than_int(void * key1, void * key2) {
    if(*(int*)key1 < *(int*)key2) return 1;
    return 0;
}

char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }

    return NULL;
}

void exportarProductos(char* nombreArchivo)
{

}

void agregarProducto(nomProd, nomMarca, nomTipo, cantDisp, precio){

}

void importarProductos(char* nombreArchivo)
{

}

void muestraProductosTipo(nomTipo){

}

void agregaProductoCarrito(nomProd, cant, nomCarrito){

}

void concretarCompra(nomCarrito){

}



int main(){
    Map* productosPorNombre = createMap(is_equal_string); //Mapa de productos por nombre (String)
    Map* productosPorTipo = createMap(is_equal_int); //Mapa de productos por tipo (Pensaba en dividir los tipos
                                                     //por números, no sé si se les ocurre algo más)
    Map* productosPorMarca = createMap(is_equal_string); //Mapa de productos por marca (String)
    Map* ListaCarritos = createMap(is_equal_string); //Lista global de carritos.
    Map* ListaGlobalProductos = createMap(is_equal_string); //TAL VEZ ESTA NO SEA NECESARIA

    char *nombreProducto, *nombreMarca, *tipo;
    int stock; //Cantidad disponible de un producto.
    int cantidadCompra; //Cantidad a comprar de un producto.
    int precio;
    char carrito;
    int option;

    while (option != 0)
    {
        printf("******************************************\n");
        printf("1. Importar productos desde un archivo\n");
        printf("2. Exportar productos a un archivo\n");
        printf("3. Agregar producto\n");
        printf("4. Buscar productos por tipo\n");
        printf("5. Buscar productos por marca\n");
        printf("6. Buscar producto por nombre\n");
        printf("7. Mostrar todos los productos\n");
        printf("8. Agregar al carrito\n");
        printf("9. Eliminar del carrito\n");
        printf("10. Concretar compra\n");
        printf("11. Mostrar carritos de compra\n");
        printf("0. Salir\n");
        printf("******************************************\n");

        printf("Indique su opcion: ");
        scanf("%d", &option);

        switch(option)
        {
           case 1: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 2: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 3: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 4: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 5: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 6: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 7: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 8: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 9: printf("FUNCION NO IMPLEMENTADA!\n");
                   break;
           case 10: printf("FUNCION NO IMPLEMENTADA!\n");
                    break;
           case 11: printf("FUNCION NO IMPLEMENTADA!\n");
                    break;
           case 0: break;
        }
        printf("\n");
    }

    return 0;
}