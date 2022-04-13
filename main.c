#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"

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

void agregaProducto(nomProd, nomMarca, nomTipo, cantDisp, precio){

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
    Map* ListaGlobalProductos = createMap(is_equal_string); //TAL VEZ ESTA NO SEA NECESARIA

    char nombreProducto;
    char nombreMarca;
    char stock; //Cantidad disponible de un producto.
    char cantidadCompra; //Cantidad a comprar de un producto.
    int precio;
    int tipo; //SUJETA A CAMBIOS.
    char carrito;


    

}