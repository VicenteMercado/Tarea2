#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
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
  int cantidadElementos;
} tipoLista;

typedef struct{
  char nombre[100];
  int cantidad;
  char marca[100];
  int precio;
} tipoProductoCarro;

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
  FILE *archivoProductos = fopen(nombreArchivo, "wt");
  if (archivoProductos == NULL)
    {
      return;
    }
  tipoProducto* producto;
  producto = firstMap(mapa_nombres);
  while(producto != NULL)
  {
      fputs(producto->nombre, archivoProductos);
      fputc(',', archivoProductos);
      fputs(producto->marca, archivoProductos);
      fputc(',', archivoProductos);
      fputs(producto->tipo, archivoProductos);
      fputc(',', archivoProductos);
      fprintf(archivoProductos, "%d", producto->stock);
      fputc(',', archivoProductos);
      fprintf(archivoProductos, "%d", producto->precio);
      fputc('\n', archivoProductos);
      producto=nextMap(mapa_nombres);
  }  
  fclose(archivoProductos);

}


void agregarProducto(char* nomProd, char* nomMarca, char* nomTipo, int cantDisp, int precio, Map* prodPorNombre, Map* prodPorTipo, Map* prodPorMarca)
{
  tipoProducto* productoNuevo = (tipoProducto *) malloc (sizeof(tipoProducto));

  nomProd[0] = toupper(nomProd[0]);
  nomMarca[0] = toupper(nomMarca[0]);
  nomTipo[0] = toupper(nomTipo[0]);

  strcpy(productoNuevo->nombre, nomProd);
  strcpy(productoNuevo->marca, nomMarca);
  strcpy(productoNuevo->tipo, nomTipo);
  productoNuevo->stock = cantDisp;
  productoNuevo->precio = precio;

  tipoLista* nuevaMarca;
  bool crearNuevaMarca = false;
  tipoLista* nuevoTipo;
  bool crearNuevoTipo = false;

  tipoProducto* busquedaNombre = firstMap(prodPorNombre);
  tipoLista* busquedaMarca = firstMap(prodPorMarca);
  tipoProducto* busquedaListaMarca;
  tipoProducto* busquedaListaTipo;
  tipoLista* busquedaTipo = firstMap(prodPorTipo);

  while (busquedaNombre != NULL)
  {
    if (strcmp(busquedaNombre->nombre, productoNuevo->nombre) == 0) //Que ya esté en el nombre, significa que ya va a estar en los demás
    {
      busquedaNombre->stock += cantDisp;
      break;
    }
    busquedaNombre = nextMap(prodPorNombre);
  }

  while (busquedaMarca != NULL)
  {
    if (strcmp(busquedaMarca->nombre, productoNuevo->marca) == 0)
    {
      crearNuevaMarca = true;
      nuevaMarca = busquedaMarca;
      busquedaListaMarca = firstList(busquedaMarca->lista);
      while (busquedaListaMarca != NULL)
      {
        if (strcmp(busquedaListaMarca->nombre, productoNuevo->nombre) == 0)
        {
          busquedaListaMarca->stock = busquedaNombre->stock;
          break;
        }
        busquedaListaMarca = nextList(busquedaMarca->lista);
      }
    }
    busquedaMarca = nextMap(prodPorMarca);
  }

  if (crearNuevaMarca == false)
  {
    nuevaMarca = (tipoLista *) malloc (sizeof(tipoLista));
    nuevaMarca->lista = createList();
    strcpy(nuevaMarca->nombre, nomMarca);
    insertMap(prodPorMarca, nuevaMarca->nombre, nuevaMarca);
  }

  while (busquedaTipo != NULL)
  {
    if (strcmp(busquedaTipo->nombre, productoNuevo->tipo) == 0)
    {
      crearNuevoTipo = true;
      nuevoTipo = busquedaTipo;
      busquedaListaTipo = firstList(busquedaTipo->lista);
      while (busquedaListaTipo != NULL)
      {
        if (strcmp(busquedaListaTipo->nombre, productoNuevo->nombre) == 0)
        {
          busquedaListaTipo->stock = busquedaNombre->stock;
          break;
        }
        busquedaListaTipo = nextList(busquedaTipo->lista);
      }
    }
    busquedaTipo = nextMap(prodPorTipo);
  }

  if (crearNuevoTipo == false)
  {
    nuevoTipo = (tipoLista *) malloc (sizeof(tipoLista));
    nuevoTipo->lista = createList();
    strcpy(nuevoTipo->nombre, nomTipo);
    insertMap(prodPorTipo, nuevoTipo->nombre, nuevoTipo);
  }

  if (busquedaNombre != NULL && strcmp(busquedaNombre->nombre, productoNuevo->nombre) == 0)
  {
    printf("Stock de %s actualizado\n", busquedaNombre->nombre);
    return;
  }

  insertMap(prodPorNombre, productoNuevo->nombre, productoNuevo);

  if (crearNuevaMarca == true && crearNuevoTipo == true)
  {
    pushBack(nuevoTipo->lista, productoNuevo);
    pushBack(nuevaMarca->lista, productoNuevo);
    printf("%s fue agregado al catalogo\n", productoNuevo->nombre);
    return;
  }

  pushBack(nuevaMarca->lista, productoNuevo);
  pushBack(nuevoTipo->lista, productoNuevo);
  printf("%s fue agregado al catalogo\n", productoNuevo->nombre);

}

void BuscarTipo (char* tipo, Map* prodPorTipo)
{
   //marcador de si hay un producto encontrado 
  bool prodEncontrado = false;
  tipoLista* listaProductos = searchMap(prodPorTipo, tipo);
  if (listaProductos != NULL)
  {
    //se encontró una lista de productos de esta marca 
    prodEncontrado = true;
    tipoProducto* producto = firstList(listaProductos->lista);
    while(producto != NULL)
    {
      //se muestran los datos del producto 
      printf("%s, ", producto->nombre);
      printf("%s, ", producto->marca);
      printf("%s, ", producto->tipo);
      printf("%d, ", producto->stock);
      printf("%d\n", producto->precio);
      producto = nextList(listaProductos->lista);
    }
  }

  //de no existir el tipo debe mostrar un mensaje por pantalla 
  if (prodEncontrado == false)
  {
    printf("No existe este tipo de producto");
  }
}

void BuscarMarca (char* marca, Map* prodPorMarca)
{
  //marcador de si hay un producto encontrado 
  bool prodEncontrado = false;
  tipoLista* listaProductos = searchMap(prodPorMarca, marca);
  if (listaProductos != NULL)
  {
    //se encontró una lista de productos de esta marca 
    prodEncontrado = true;
    tipoProducto* producto = firstList(listaProductos->lista);
    while(producto != NULL)
    {
      //se muestran los datos del producto 
      printf("%s, ", producto->nombre);
      printf("%s, ", producto->marca);
      printf("%s, ", producto->tipo);
      printf("%d, ", producto->stock);
      printf("%d\n", producto->precio);
      producto = nextList(listaProductos->lista);
    }
  }

  //de no existir la marca debe mostrar un mensaje por pantalla 
  if (prodEncontrado == false)
  {
    printf("No existe esta marca de producto");
  }
}

void BuscarNombre (char* nombre, Map* prodPornombre)
{
  tipoProducto* producto = searchMap(prodPornombre, nombre);
  if (producto != NULL)
  {
    //se encontró un producto con este nombre, por lo que se muestran los datos del producto 
    printf("%s, ", producto->nombre);
    printf("%s, ", producto->marca);
    printf("%s, ", producto->tipo);
    printf("%d, ", producto->stock);
    printf("%d\n", producto->precio);
  }
  else
  {
    //en caso de no haber encontrado el producto, mostrar un mensaje por pantalla
    printf("No se ha encontrado el producto con el nombre ingresado");
  }
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
                int precioAEntero = atoi(precio);
                agregarProducto(nombre, marca, tipo, stockAEntero, precioAEntero, prodPorNombre, prodPorTipo, prodPorMarca);
        }
        k++;
    }

  printf("Todos los datos han sido copiados o el stock ha sido modificado\n");
  fclose(archivoProductos);
}

void muestraTodosProductos(Map* prodPorNombre){ //CASE 7
  printf("\n"); //Se empieza con salto de línea para mejor distinción.
  tipoProducto* prodActual = firstMap(prodPorNombre); //Ayudará a recorrer cada producto.

  if(!prodActual){
    printf("No hay productos existentes!\n\n");
    return;
  }
  
  while(prodActual != NULL){
     printf("Nombre del producto: %s\n", prodActual->nombre); 
     printf("Marca: %s\n", prodActual->marca);
     printf("Tipo: %s\n", prodActual->tipo); 
     printf("Stock: %d\n", prodActual->stock); 
     printf("Precio: $%d\n", prodActual->precio); 

     prodActual = nextMap(prodPorNombre);
  }

}

void eliminarProdCarrito(char* nomCarrito, Map* prodPorNombre, List* listaCarritos){
  tipoLista* carrito = firstList(listaCarritos); //Comienza busqueda del carrito nomCarrito.

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
    printf("\nNo existe el carrito de dicho nombre!\n\n");
    return;
  }

  //Comienza proceso de eliminación de producto

  popBack(carrito->lista);
  printf("\nEl ultimo producto ingresado en el carrito %s ha sido eliminado correctamente\n", nomCarrito);
  
}

void agregaProductoCarrito(char* nomProd, int cant, char* nomCarrito, List* listaCarritos, Map* prodPorNombre){
 
    tipoProductoCarro*  producto =(tipoProductoCarro*) malloc (sizeof(tipoProductoCarro));
    strcpy(producto->nombre, nomProd);
    producto->cantidad= cant;
    tipoLista* revisarCarrito = firstList(listaCarritos);
    tipoProducto* buscadorProducto = searchMap(prodPorNombre, nomProd);
    producto->precio = buscadorProducto->precio;

     while (revisarCarrito != NULL)
        {
          if (strcmp(revisarCarrito->nombre, nomCarrito) == 0)
          {
                  break;
          }
          else
          {
                  revisarCarrito = nextList(listaCarritos);
          }
        }
    if (revisarCarrito == NULL)
        {
                revisarCarrito  = (tipoLista*) malloc(sizeof(tipoLista));
                strcpy(revisarCarrito ->nombre, nomCarrito);
                revisarCarrito->cantidadElementos = 0;
                revisarCarrito->lista  = createList();
                pushBack(listaCarritos, revisarCarrito );
        }
    if (!firstList(revisarCarrito->lista))
        {
                pushFront(revisarCarrito->lista, producto);
                printf("Su producto ha sido agregado a su carrito de compra\n");
                revisarCarrito->cantidadElementos++;
                return;
        }
    else
    {
      pushBack(revisarCarrito->lista, producto);
      printf("Su producto ha sido agregado a su carrito de compra\n");
      revisarCarrito->cantidadElementos++;
      return;
    }  
}

void concretarCompra(char *nomCarrito, List* listaCarritos, Map* productosPorNombre){ //FUNCIÓN FALTA DE TESTEO.

  tipoLista* carrito = firstList(listaCarritos); //Comienza busqueda del carrito nomCarrito.

  //Si no hay ningún carrito, se imprime este mensaje en pantalla y finaliza la función.
  if (!firstList(listaCarritos))
  {
    printf("\nNo existen carritos, por favor crear uno como minimo\n\n");
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
  tipoProductoCarro* prod = firstList(carrito->lista); //Ayudará a recorrer los productos del carrito.

  while(prod != NULL){ //Se van sumando los precios de todos los productos.
    totalPago += prod->precio * prod->cantidad;
    prod = nextList(carrito->lista);
  }

  printf("El precio total por su carrito es: %d pesos.\n\n", totalPago); //Se imprime cantidad total.

  prod = firstList(carrito->lista); //prod se devuelve a la primera posición, esta vez para conseguir los nombres
                                       // y precios de cada producto.

  while(prod != NULL){ //Se imprimen productos uno por uno.
    printf("%s - $%d (%d unidades)\n", prod->nombre, prod->precio, prod->cantidad);
    prod = nextList(carrito->lista);
  }

  prod = firstList(carrito->lista); //prod se devuelve a la primera posición una vez más,
                                    //ahora para reducir el stock de los productos correspondientes

  tipoProducto* busquedaNombre = firstMap(productosPorNombre);

  while (prod != NULL){ //Se elimina stock correspondiente de cada producto.
      busquedaNombre = firstMap(productosPorNombre);
      while(busquedaNombre != NULL){
        if(strcmp(prod->nombre,busquedaNombre->nombre)==0){
          printf("\nProducto %s tiene %d unidades disponibles, ", prod->nombre, busquedaNombre->stock);
          if(busquedaNombre->stock < prod->cantidad){
            printf("ERROR!\nno existe stock disponible para su compra.");
            exit(EXIT_FAILURE);
          }
          busquedaNombre->stock -= prod->cantidad;
          printf("tras la compra quedan %d unidades.", busquedaNombre->stock);
        }
        busquedaNombre = nextMap(productosPorNombre);  
      }
    prod = nextList(carrito->lista);
  }

  tipoLista* posCarrito = firstList(listaCarritos); //Variable para la posición del carrito en la lista

  while(posCarrito != NULL){  //Se recupera la posición del carrito
    if(strcmp(posCarrito->nombre, carrito->nombre)==0) break;
    else{
      posCarrito = nextList(listaCarritos);
    }
  }

  printf("\nMuchas gracias por tu compra!\n\n");

  popCurrent(listaCarritos); //Se elimina el carrito.
}

void mostrarCarritosCompra(List * listaCarritos)
{
  tipoLista* carrito = firstList(listaCarritos);
   while(carrito != NULL)
     {
       int cant = 0;
       printf("Nombre del carrito: %s\n", carrito->nombre);
       tipoProductoCarro * producto = firstList(carrito->lista);
       while(producto != NULL)
       {
           printf("%s", producto->nombre);
           printf(" %d \n", producto->cantidad);
           cant++;
           producto = nextList(carrito->lista);
       }
       printf("La cantidad de productos que tiene el carrito %s es: %d\n", carrito->nombre, cant);
       carrito=nextList(listaCarritos);
     }                                  
}



int main(){
    Map* productosPorNombre = createMap(is_equal_string); //Mapa de productos por nombre (String)
    setSortFunction(productosPorNombre,lower_than_string);
    Map* productosPorTipo = createMap(is_equal_string); //Mapa de productos por tipo
    setSortFunction(productosPorTipo,lower_than_string);
    Map* productosPorMarca = createMap(is_equal_string); //Mapa de productos por marca (String)
    setSortFunction(productosPorMarca,lower_than_string);
    List* listaCarritos = createList(); //Lista global de carritos.

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
           case 2: getchar();
                   printf("Ingrese el nombre del archivo al que desea exportar los productos\n");
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
           case 4: getchar();
                   printf("Ingrese el nombre del tipo a buscar: ");
                   scanf("%100[^\n]s", tipo);
                   getchar();
                   BuscarTipo(tipo, productosPorTipo);
                   break;
           case 5: getchar();
                   printf("Ingrese el nombre de la marca a buscar: ");
                   scanf("%100[^\n]s", nombreMarca);
                   BuscarMarca(nombreMarca, productosPorMarca);
                   break;
           case 6: getchar();
                   printf("Ingrese el nombre del producto a buscar: ");
                   scanf("%100[^\n]s", nombreProducto);
                   BuscarNombre(nombreProducto, productosPorNombre);
                   break;
           case 7: muestraTodosProductos(productosPorNombre);
                   break;
           case 8: getchar();
                   printf("Ingrese el nombre de el carrito\n");
                   scanf("%100[^\n]s", carrito);
                   //printf("el carrito se llama %s \n", carrito);
                   while(nombreProducto[0] != '0')
                   {
                      printf("Ingrese el nombre del producto que desea ingresar, si su carrito se encuentra listo, escriba un 0\n");
                      getchar();
                      scanf("%100[^\n]s", nombreProducto);
                      if(nombreProducto[0] == '0') break;
                      printf("Ingrese la cantidad que desea de %s ", nombreProducto);
                      getchar();
                      scanf("%d", &cantidadCompra);
                      agregaProductoCarrito(nombreProducto, cantidadCompra, carrito, listaCarritos, productosPorNombre);
                   }
                   nombreProducto[0] = '1';
                    printf("el carrito se llama %s \n", carrito);

                   break;
           case 9: printf("Por favor, ingrese el nombre de su carrito: ");
                   getchar();
                   scanf("%100[^\n]s", carrito);
                   eliminarProdCarrito(carrito, productosPorNombre, listaCarritos);
                   break;
           case 10: printf("Por favor, ingrese el nombre de su carrito: ");
                    getchar();
                    scanf("%100[^\n]s", carrito);
                    concretarCompra(carrito, listaCarritos, productosPorNombre);
                    break;
           case 11: mostrarCarritosCompra(listaCarritos);
                    break;
           case 0: break;
        }
        printf("\n");
    }

    return 0;
}