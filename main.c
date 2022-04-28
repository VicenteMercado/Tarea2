#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "list.h"

typedef struct
{
  char nombre[100];
  char marca[100];
  char tipo[100];
  int stock;
  int precio;
} tipoProducto;

typedef struct
{
  List* lista;
  char nombre[100];
} tipoLista;

/*typedef struct{ //FALTA AGREGAR MÁS PROPIEDADES
  tipoLista* listaDeCarrito;
  char nombreCarrito[100];

} tipoCarrito;*/

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

void exportarProductos(char* nombreArchivo, Map* mapa_nombres)
{
  FILE *archivoCanciones = fopen(nombreArchivo, "wt");
  if (archivoCanciones == NULL)
    {
      return;
    }
  tipoProducto* producto;
  producto = firstMap(mapa_nombres);
  while(producto != NULL)
  {
    //nombre maarca tipo precio
      fputs(producto->nombre, archivoCanciones);
      fputc(',', archivoCanciones);
      fputs(producto->marca, archivoCanciones);
      fputc(',', archivoCanciones);
      fputs(producto->tipo, archivoCanciones);
      fputc(',', archivoCanciones);
      fprintf(archivoCanciones, "%d", producto->precio);
      fputc('\n', archivoCanciones);
      producto=nextMap(mapa_nombres);
  }  
  fclose(archivoCanciones);

}


void agregarProducto(char* nomProd, char* nomMarca, char* nomTipo, int cantDisp, int precio, Map* prodPorNombre, Map* prodPorTipo, Map* prodPorMarca)
{
  tipoProducto* productoNuevo = (tipoProducto *) malloc (sizeof(tipoProducto));

  strcpy(productoNuevo->nombre, nomProd);
  strcpy(productoNuevo->marca, nomMarca);
  strcpy(productoNuevo->tipo, nomTipo);
  productoNuevo->stock = cantDisp;
  productoNuevo->precio = precio;

  tipoProducto* busquedaNombre = firstMap(prodPorNombre);
  tipoLista* busquedaMarca = firstMap(prodPorMarca);
  tipoProducto* busquedaListaMarca;
  tipoProducto* busquedaListaTipo;
  tipoLista* busquedaTipo = firstMap(prodPorTipo);

  while (busquedaNombre)
  {
    if (strcmp(busquedaNombre->nombre, productoNuevo->nombre) == 0) //Que ya esté en el nombre, significa que ya va a estar en los demás
    {
      busquedaNombre->stock += cantDisp;
      break;
    }
    else
      busquedaNombre = nextMap(prodPorNombre);
  }

  while (busquedaMarca)
  {
    if (strcmp(busquedaMarca->nombre, productoNuevo->marca) == 0)
    {
      busquedaListaMarca = firstList(busquedaMarca->lista);
      while (busquedaListaMarca != NULL)
      {
        if (strcmp(busquedaListaMarca->nombre, productoNuevo->nombre) == 0)
        {
          busquedaListaMarca->stock += cantDisp;
          break;
        }
        else
          busquedaListaMarca = nextList(busquedaMarca->lista);
      }
    }
    else
      busquedaMarca = nextMap(prodPorMarca);
  }

  while (busquedaTipo)
  {
    if (strcmp(busquedaTipo->nombre, productoNuevo->tipo) == 0)
    {
      busquedaListaTipo = firstList(busquedaTipo->lista);
      while (busquedaListaTipo != NULL)
      {
        if (strcmp(busquedaListaTipo->nombre, productoNuevo->nombre) == 0)
        {
          busquedaListaTipo->stock += cantDisp;
          break;
        }
        else
          busquedaListaTipo = nextList(busquedaTipo->lista);
      }
    }
    else
      busquedaTipo = nextMap(prodPorTipo);
  }

  printf("busquedanombre = %s\n", busquedaNombre->nombre);

  if (busquedaNombre && strcmp(busquedaNombre->nombre, productoNuevo->nombre) == 0)
  {
    printf("Stock de %s actualizado\n", busquedaNombre->nombre);
    return;
  }

  printf("Antes de insertar en el mapa nombre\n");

  insertMap(prodPorNombre, productoNuevo->nombre, productoNuevo);
}

void importarProductos(char* nombreArchivo, Map* prodPorNombre, Map* prodPorMarca, Map* prodPorTipo)
{
  FILE *archivoProductos = fopen(nombreArchivo, "rt");
  if (archivoProductos == NULL)
  {
    //Si no se haya el archivo, se avisa al usuario y se regresa al menú.    
    printf("\nArchivo no encontrado!\n");
    return;
  }
  printf("Su archivo se ha abierto correctamente!\n");

  char linea[1024];
  int i;
  int k = 0;

  while (fgets(linea, 1023, archivoProductos) != NULL)
    {
        for (i = 0; i < 1; i++)
        {
                char *nombre = get_csv_field(linea, i);
                char *marca = get_csv_field(linea, i+1);
                char *tipo = get_csv_field(linea, i+2);
                char* stock = get_csv_field(linea, i+3);
                int stockAEntero = atoi(stock);
                char* precio = get_csv_field(linea, i+4);
                int precioAEntero = atoi(stock);
                agregarProducto(nombre, marca, tipo, stockAEntero, precioAEntero, prodPorNombre, prodPorTipo, prodPorMarca);
        }
        k++;
    }

  printf("Todos los datos han sido copiados o el stock ha sido modificado\n");
  fclose(archivoProductos);
}

void muestraProductosTipo(char* nomTipo){

}

void agregaProductoCarrito(char* nomProd, int cant, char* nomCarrito){

}

void concretarCompra(char *nomCarrito, List* listaCarritos, Map* productosPorNombre, Map* productosPorTipo, Map* productosPorMarca){ //FUNCIÓN INCOMPLETA.

  tipoLista* carrito = firstList(listaCarritos); //Comienza busqueda del carrito nomCarrito.

  //Si no hay ningún carrito, se imprime este mensaje en pantalla y finaliza la función.
  if (!firstList(listaCarritos))
  {
    printf("No existen carritos, por favor crear uno como mínimo\n\n");
    return;
  }

  while (carrito != NULL)
  {
    if (strcmp(carrito->nombre, nomCarrito) == 0)
    {
        break; //Si los nombres son iguales, se proceden a mostrar los datos del carrito.
    }
    else
    {
       carrito = nextList(listaCarritos); //si no, se avanza a la siguiente posición
    }
  }

  if (carrito == NULL){ //Si no se encontró el carrito nomCarrito, se imprime un mensaje referente y se devuelve al menú.
    printf("No existe el carrito de dicho nombre!\n\n");
    return;
  }

  //Comienza procesamiento de carrito.

  int totalPago = 0; //Variable que almacena el total a pagar por carrito.
  tipoProducto* prod = firstMap(productosPorNombre); //Ayudará a recorrer los productos del carrito.

  while(prod != NULL){ //Se van sumando los precios de todos los productos.
    totalPago += prod->precio;
    prod = nextMap(productosPorNombre);
  }

  printf("El precio total por su carrito es: %d pesos.\n\n", totalPago); //Se imprime cantidad total.

  prod = firstMap(productosPorNombre); //prod se devuelve a la primera posición, esta vez para conseguir los nombres
                                       // y precios de cada producto.

  while(prod != NULL){ //Se imprimen productos uno por uno.
    printf("%s - $%d\n", prod->nombre, prod->precio);
    prod = nextMap(productosPorNombre);
  }

  //FALTA PROCESAR ELIMINACIÓN DE STOCK Y ELIMINAR CARRITO!!!

  tipoProducto* busquedaNombre = firstMap(productosPorNombre); 
  //tipoProducto* busquedaTipo = 
  //tipoProducto* busquedaMarca =







}



int main(){
    Map* productosPorNombre = createMap(is_equal_string); //Mapa de productos por nombre (String)
    Map* productosPorTipo = createMap(is_equal_string); //Mapa de productos por tipo (Pensaba en dividir los tipos
                                                     //por números, no sé si se les ocurre algo más)
    Map* productosPorMarca = createMap(is_equal_string); //Mapa de productos por marca (String)
    List* listaCarritos = createList(); //Lista global de carritos.
    //List* listaGlobalProductos = createList(); //TAL VEZ ESTA NO SEA NECESARIA

    char *nombreProducto = (char*) malloc (100*sizeof(char));
    char *nombreMarca = (char*) malloc (100*sizeof(char));
    char *tipo = (char*) malloc (100*sizeof(char));
    char *carrito = (char*) malloc (100*sizeof(char));
    char *nombreArchivo = (char*) malloc (100*sizeof(char));
    int stock; //Cantidad disponible de un producto.
    int cantidadCompra; //Cantidad a comprar de un producto.
    int precio;
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
           case 1: getchar();
                   printf("Ingrese el nombre del archivo: ");
                   scanf("%100[^\n]s", nombreArchivo);
                   getchar();
                   importarProductos(nombreArchivo, productosPorNombre, productosPorMarca, productosPorTipo);
                   break;
           case 2: printf("Ingrese el nombre del archivo al que desea exportar los productos\n");
                   scanf("%100[^\n]s", nombreArchivo);
                   getchar();
                   exportarProductos(nombreArchivo, productosPorNombre);
                   break;
           case 3: printf("Ingrese el nombre del producto: ");
                   getchar();
                   scanf("%100[^\n]s", nombreProducto);
                   getchar();
                   printf("Ingrese la marca del producto: ");
                   scanf("%100[^\n]s", nombreMarca);
                   getchar();
                   printf("Indique el tipo del producto: ");
                   scanf("%100[^\n]s", tipo);
                   getchar();
                   printf("Ingrese el stock del producto indicado: ");
                   scanf("%d", &stock);
                   printf("Indique el precio del producto: ");
                   scanf("%d", &precio);
                   agregarProducto(nombreProducto, nombreMarca, tipo, stock, precio, productosPorNombre, productosPorTipo, productosPorMarca);
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