#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Podriamos poner define, pero es más exacto el const
const int numBarcos1 = 4;
const int numBarcos2 = 2; 
const int numBarcos3 = 1;


int menu();
void hundirLaFlota(int opcionLeida, int filas,  int col);
void colocarBarcosManualmente(int *t, int filas, int col);
void colocarBarcosAutomaticamente(int *t, int filas, int col);
void inicializarTablero(int *t, int filas, int col);
void imprimirTablero(int *t, int filas, int col);
void imprimirTableroArchivo(int *t, int filas, int col, FILE *parchivo);
int comprobacionEspacioParaBarco(int *t, int filas, int col, int iniFila, int iniCol, int tamBarco, int orientacion);
int compruebaGanador (int *b, int filas, int col);
int compruebaDisparo(int *t, int filas, int col, int posFila, int posCol);
void juegoManual(int filas, int col);
void juegoAutomatico(int filas, int col);

int main(int argc, char *argv[]){

	/*argv[0]==> nombre del programa
	  argv[1]==> num filasilas
	  argv[2]==> num col
	*/

	int filas = atoi(argv[1]);
	int col = atoi(argv[2]);
	int opcionLeida;
	srand(time(NULL));

	if(argc!=3){
		printf("ERROR! Ha de introducir 3 elementos como argumentos! \n");
		int exit=-1;
		return exit;
	}else{
		if(filas>0 && col>0 && filas>=5 && col>=5){
			printf("Número de filas del tablero: %d \nNúmero columnas del tablero: %d \n", filas, col);

			opcionLeida=menu();
			hundirLaFlota(opcionLeida, filas, col);

		}else{
			printf("ERROR! Ha de introducir numeros positivos y mayores de 5! \n");
		}
	}

return 0;
}

int menu(){
//Función menu.
//	- INPUTS: nada
//	- OUTPUTS: la opción leída (1, 2 ó 3)
//  - Presenta el menú por pantalla y lee una opción. Si no es 1, 2 ó 3 da un mensaje de error y vuelve a leerla hasta que sea correcta.
int numIntroducido;
int opcionLeida;

printf("                                                                                                       \n");
printf(" _|_|_|      _|_|    _|_|_|_|_|  _|_|_|_|_|  _|        _|_|_|_|    _|_|_|  _|    _|  _|_|_|  _|_|_|    \n");
printf(" _|    _|  _|    _|      _|          _|      _|        _|        _|        _|    _|    _|    _|    _|  \n");
printf(" _|_|_|    _|_|_|_|      _|          _|      _|        _|_|_|      _|_|    _|_|_|_|    _|    _|_|_|    \n");
printf(" _|    _|  _|    _|      _|          _|      _|        _|              _|  _|    _|    _|    _|        \n");
printf(" _|_|_|    _|    _|      _|          _|      _|_|_|_|  _|_|_|_|  _|_|_|    _|    _|  _|_|_|  _|        \n");
printf("                                                                                                       \n");


do{
	printf("     ##################################\n");
	printf("     #\tSeleccione una opcion:        #\n");
	printf("     #\t[1] Jugar (player vs pc)      #\n");
	printf("     #\t[2] Jugar (pc vs pc)          #\n");
	printf("     #\t[3] Salir                     #\n");
	printf("     ##################################\n");
	scanf("%d",&numIntroducido);

	switch(numIntroducido){
		case 1: 
			opcionLeida=1;
			break;
		case 2: 
			opcionLeida=2;
			break;
		case 3: 
			opcionLeida=3;
			break;
		default:
			printf("ERROR! Los números han de ser del 1-3 \n");
			printf("\n");
	}	
}while(numIntroducido!=3 && numIntroducido!=2 && numIntroducido!=1);
	 
return opcionLeida;	
}

void hundirLaFlota(int opcionLeida, int filas,  int col){
//Función hundirLaFlota
//	- INPUTS: opción (1, 2 ó 3), filas y columnas del tablero
//  - OUTPUTS: nada
//  - Según la opción leída llama a jugar manual (opción 1), jugar automático (opción 2) o muestra un mensaje de fin (opción 3)
	switch(opcionLeida){
		case 1:
			printf("\n");
			printf("Ha seleccionado JuegoManual \n");
			juegoManual(filas, col);
			break;
		case 2: 
			printf("\n");
			printf("Ha seleccionado JuegoAutomatico \n");
			juegoAutomatico(filas, col);
			break;
		case 3:
			printf("\n");
			printf("Ha seleccionado salir. Adiós!\n");
			break;
	}
}

void juegoManual(int filas, int col){
//Función juegoManual
// 	- INPUTS: número de filas y número de columnas del tablero
//	- OUTPUTS: nada
//	- Juega de forma manual:
//		-- Crea los 4 tableros, los inicializa (llamando a la función inicializar tantas veces como tableros haya) y en 2 pone los barcos de forma manual o automática, según indique el usuario
//		-- Escribe por pantalla los tableros y las tiradas al inicio del juego
//		-- Por turnos, cada jugador elige una fila y columna de 1 al número de filas y de 1 al número de columnas. Se comprueba si hay barco indicando agua o tocado (se indica por pantalla).
//		-- Se comprueba si hay ganador. Si lo hay acaba la partida indicando quíén ha ganado por pantalla
//		-- EXTRA: comprobar que no se ha repetido ese disparo
	int *tablero1, *tablero2, *tableroBarcos1, *tableroBarcos2;

	//creacion de punteros e inicializamos 4 veces estos
	tablero1=(int*)malloc(sizeof(int)*filas*col);
	tablero2=(int*)malloc(sizeof(int)*filas*col);
	tableroBarcos1=(int*)malloc(sizeof(int)*filas*col);
	tableroBarcos2=(int*)malloc(sizeof(int)*filas*col);

	inicializarTablero(tablero1,filas,col);
	inicializarTablero(tablero2,filas,col);
	inicializarTablero(tableroBarcos1,filas,col);
	inicializarTablero(tableroBarcos2,filas,col);

	int random; //variable que nos permite elegir turno del primer jugador al azar
	random=0+rand()%(2);

	if(random==1){
			printf("\nComienza el Jugador 1! (es azar sorry :v)\n\n");
			int seleccion; //variable permite al jugador elegir entre colocar los barcos de forma manual o automatica
		do{

			printf("¿Cómo desea jugar?\n");
			printf("[1] Colocar los barcos Manual \n");
			printf("[2] Colocar los barcos Automático \n");

			scanf("%d",&seleccion);

			if(seleccion==1){
				printf("\nPues los colocas tú\n");
				colocarBarcosManualmente(tablero1,filas,col);
				colocarBarcosAutomaticamente(tablero2,filas,col);
			}else{
				if(seleccion==2){
					printf("\nDeja que la maquina los coloque por ti\n");
					colocarBarcosAutomaticamente(tablero1,filas,col);
					colocarBarcosAutomaticamente(tablero2,filas,col);
				}else{
					printf("Esa opción no es la correcta! \n");
					printf("\n");
				}
			}
		}while(seleccion!=1 && seleccion!=2);

		printf("\n	TABLERO JUGADOR 1 \n");
		imprimirTablero(tablero1, filas, col);
		printf("\n 	TABLERO JUGADOR 2 \n");
		imprimirTablero(tablero2, filas, col);

		int f,c,aux,ganador=0,comprobacion=0;
		int count=0;
		int fVerdad,cVerdad;
		do{	
			int vuelta; //variable evita se entreguen coordenadas por pantalla fuera del rango filas*columnas
			do{
				printf("\n");
				printf("Introduzca coordenada X disparo: ");
				scanf("%d",&f);
				f--;
				fVerdad=f+1;
				printf("Introduzca coordenada Y disparo: ");
				scanf("%d",&c);
				printf("\n");
				c--;
				cVerdad=c+1;
				comprobacion=compruebaDisparo(tablero2,filas,col,f,c);
				if(f>filas-1 || f<0|| c>col-1 || c<0){
					vuelta=0;
					printf("Coordenadas incorrectas!\n");
				}else{
					vuelta=1;
					printf("Jugador 1: (%d,%d) ",fVerdad,cVerdad);
				}
			}while(vuelta==0);
			
			switch(comprobacion){
				case 0:
					*(tablero2+f*col+c)=-9;
					*(tableroBarcos1+f*col+c)=-9;
					printf("AGUA\n");
					break;
				case 1:
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero2+f*col+c)=-1;
					*(tableroBarcos1+f*col+c)=-1;
					break;
				case 2:	
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-2;
					*(tableroBarcos1+f*col+c)=-2;
					break;
				case 3:
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-3;
					*(tableroBarcos1+f*col+c)=-3;
					break;
				case 8:
					printf("DISPARO YA EFECTUADO \n");
					break;
			}

			f=rand()%filas;
			fVerdad=f+1;
			c=rand()%col;
			cVerdad=c+1;
			comprobacion=compruebaDisparo(tablero1,filas,col,f,c);
			printf("Jugador 2: (%d,%d) ",fVerdad,cVerdad);
			switch(comprobacion){
				case 0:
					printf("AGUA\n");
					*(tablero1+f*col+c)=-9;
					*(tableroBarcos2+f*col*c)=-9;
					break;
				case 1:
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero1+f*col+c)=-1;
					*(tableroBarcos2+f*col+c)=-1;
					break;
				case 2:
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-2;
					*(tableroBarcos2+f*col+c)=-2;
					break;
				case 3:
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-3;
					*(tableroBarcos2+f*col+c)=-3;
					break;
				case 8:
					printf("DISPARO YA EFECTUADO \n");
					break;
			}
		

			if(compruebaGanador(tablero2,filas,col)==1){
				ganador=1;
				aux=1;
			}
	
			if(compruebaGanador(tablero1,filas,col)==1){
				ganador=1;
				aux=2;
			}
		
		

		}while(ganador == 0);
		

		if(aux==1){
			printf("HA GANADO EL JUGADOR 1\n");

			printf("\nTABLERO JUGADOR 2 DISPAROS RECIBIDOS POR JUGADOR 1\n");
			imprimirTablero(tableroBarcos1, filas, col);

		}else{

			if(aux==2){
				printf("HA GANADO EL JUGADOR 2\n");

				printf("\nTABLERO JUGADOR 1 DISPAROS RECIBIDOS POR JUGADOR 2 \n");
				imprimirTablero(tableroBarcos2, filas, col);
			}
		}

	}else{
		
		printf("\nComienza jugador 2! (es azar sorry :v)\n\n");
		int seleccion;
		do{

			printf("¿Cómo desea jugar?\n");
			printf("[1] Colocar los barcos Manual \n");
			printf("[2] Colocar los barcos Automático \n");

			scanf("%d",&seleccion);

			if(seleccion==1){
				printf("\nPues los colocas tú\n");
				colocarBarcosManualmente(tablero1,filas,col);
				colocarBarcosAutomaticamente(tablero2,filas,col);
			}else{
				if(seleccion==2){
					printf("\nDeja que la maquina los coloque por ti\n");
					colocarBarcosAutomaticamente(tablero1,filas,col);
					colocarBarcosAutomaticamente(tablero2,filas,col);
				}else{
					printf("Esa opción no es la correcta! \n");
					printf("\n");
				}
			}
		}while(seleccion!=1 && seleccion!=2);

		printf("\n  	TABLERO JUGADOR 2 \n");
		imprimirTablero(tablero2, filas, col);
		printf("\n 	TABLERO JUGADOR 1 \n");
		imprimirTablero(tablero1, filas, col);

		int f,c,aux,ganador=0,comprobacion=0;
		int count=0;
		int fVerdad,cVerdad;

		do{
			f=rand()%filas;
			fVerdad=f+1;
			c=rand()%col;
			cVerdad=c+1;
			comprobacion=compruebaDisparo(tablero1,filas,col,f,c);
			printf("Jugador 2: (%d,%d) ",fVerdad,cVerdad);
			switch(comprobacion){
				case 0:	
					printf("AGUA\n");
					*(tablero1+f*col+c)=-9;
					*(tableroBarcos2+f*col*c)=-9;
					break;
				case 1:
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero1+f*col+c)=-1;
					*(tableroBarcos2+f*col+c)=-1;
					break;
				case 2:
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-2;
					*(tableroBarcos2+f*col+c)=-2;
					break;
				case 3:
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-3;
					*(tableroBarcos2+f*col+c)=-3;
					break;
				case 8:
					printf("DISPARO YA EFECTUADO \n");
					break;
			}

			int vuelta; 
			do{
				printf("\n");
				printf("Introduzca coordenada X disparo: ");
				scanf("%d",&f);
				f--;
				fVerdad=f+1;
				printf("Introduzca coordenada Y disparo: ");
				scanf("%d",&c);
				printf("\n");
				c--;
				cVerdad=c+1;
				comprobacion=compruebaDisparo(tablero2,filas,col,f,c);
				if(f>filas-1 || f<0 || c>col-1 || c<0){
					vuelta=0;
					printf("Coordenadas incorrectas!\n");
				}else{
					vuelta=1;
					printf("Jugador 1: (%d,%d) ",fVerdad,cVerdad);
				}
			}while(vuelta==0);

			switch(comprobacion){
				case 0:
					*(tablero2+f*col+c)=-9;
					*(tableroBarcos1+f*col+c)=-9;
					printf("AGUA\n");
					break;
				case 1:
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero2+f*col+c)=-1;
					*(tableroBarcos1+f*col+c)=-1;
					break;
				case 2:
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-2;
					*(tableroBarcos1+f*col+c)=-2;
					break;
				case 3:
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-3;
					*(tableroBarcos1+f*col+c)=-3;
					break;
				case 8:
					printf("DISPARO YA EFECTUADO \n");
					break;
			}

			if(compruebaGanador(tablero2,filas,col)==1){
				ganador=1;
				aux=1;
			}
	
			if(compruebaGanador(tablero1,filas,col)==1){
				ganador=1;
				aux=2;
			}
		}while(ganador == 0);


		if(aux==1){
			printf("HA GANADO EL JUGADOR 1\n");

			printf("\nTABLERO JUGADOR 2 DISPAROS RECIBIDOS POR JUGADOR 1\n");
			imprimirTablero(tableroBarcos1, filas, col);

		}else{

			if(aux==2){
				printf("HA GANADO EL JUGADOR 2\n");

				printf("\nTABLERO JUGADOR 1 DISPAROS RECIBIDOS POR JUGADOR 2\n");
				imprimirTablero(tableroBarcos2, filas, col);
			}
		}
	}
}


void juegoAutomatico(int filas, int col){
//Función juegoAutomatico
// 	- INPUTS: número de filas y número de columnas del tablero
//	- OUTPUTS: nada
//	- Juega de forma automática:
//		-- Abre el archivo para escribir en él todas las salidas
//		-- Crea los 4 tableros, los inicializa (llamando a la función inicializar tantas veces como tableros haya) y en 2 pone los barcos automáticamente
//		-- Escribe en el archivo los tableros y las tiradas al inicio del juego
//		-- Por turnos, cada jugador genera una fila y columna automáticamente (se indica en el archivo). Se comprueba si hay barco indicando agua o tocado (se indica en el archivo).
//		-- Se comprueba si hay ganador. Si lo hay acaba la partida indicando quíén ha ganado en el archivo
//		-- EXTRA: comprobar que no se ha repetido ese disparo
FILE *parchivo;

	int *tablero1, *tablero2, *tableroBarcos1, *tableroBarcos2;

	parchivo=fopen("juegoAutomatico", "a");

	if(parchivo==NULL){
		fprintf(stderr, "ERROR al abrir el archivo \n");
		exit(EXIT_FAILURE);
	}

	fprintf(parchivo, "Ha seleccionado 'Juego Automatico' \n");

	//creacion de punteros e inicializamos 4 veces estos
	tablero1=(int*)malloc(sizeof(int)*filas*col);
	tablero2=(int*)malloc(sizeof(int)*filas*col);
	tableroBarcos1=(int*)malloc(sizeof(int)*filas*col);
	tableroBarcos2=(int*)malloc(sizeof(int)*filas*col);

	inicializarTablero(tablero1,filas,col);
	inicializarTablero(tablero2,filas,col);
	inicializarTablero(tableroBarcos1,filas,col);
	inicializarTablero(tableroBarcos2,filas,col);

	//llamamos dos veces a la funcion colocarBarcosAutomaticamente
	colocarBarcosAutomaticamente(tablero1,filas,col);
	colocarBarcosAutomaticamente(tablero2,filas,col);

	int random;
	random=0+rand()%(2);

	if(random==1){
		printf("\nComienza el Jugador 1! \n\n");

		fprintf(parchivo, "\nTABLERO JUGADOR 1 \n");
		imprimirTableroArchivo(tablero1, filas, col, parchivo);
		fprintf(parchivo, "\nTABLERO JUGADOR 2 \n");
		imprimirTableroArchivo(tablero2, filas, col, parchivo);

		printf("\n     	  TABLERO JUGADOR 1 \n");
		imprimirTablero(tablero1, filas, col);
		printf("\n         TABLERO JUGADOR 2 \n");
		imprimirTablero(tablero2, filas, col);

		int f,c,aux,ganador=0,comprobacion=0;
		int count=0;
		int fVerdad,cVerdad;
		do{	

			f=rand()%filas;
			fVerdad=f+1;
			c=rand()%col;
			cVerdad=c+1;
			comprobacion=compruebaDisparo(tablero2,filas,col,f,c);
			fprintf(parchivo, "Jugador 1: (%d,%d) ",fVerdad,cVerdad);
			printf("Jugador 1: (%d,%d) ",fVerdad,cVerdad);
			switch(comprobacion){
				case 0:
					fprintf(parchivo,"AGUA\n");
					printf("AGUA\n");
					*(tablero2+f*col+c)=-9;
					*(tableroBarcos1+f*col+c)=-9;
					break;
				case 1:
					fprintf(parchivo,"BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero2+f*col+c)=-1;
					*(tableroBarcos1+f*col+c)=-1;
					break;
				case 2:
					fprintf(parchivo,"BARCO 2 POSICIONES TOCADO\n");
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-2;
					*(tableroBarcos1+f*col+c)=-2;
					break;
				case 3:
					fprintf(parchivo,"BARCO 3 POSICIONES TOCADO\n");
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-3;
					*(tableroBarcos1+f*col+c)=-3;
					break;
				case 8:
					fprintf(parchivo,"DISPARO YA EFECTUADO \n");
					printf("DISPARO YA EFECTUADO \n");
					break;
			}


			f=rand()%filas;
			fVerdad=f+1;
			c=rand()%col;
			cVerdad=c+1;
			comprobacion=compruebaDisparo(tablero1,filas,col,f,c);
			fprintf(parchivo, "Jugador 2: (%d,%d) ",fVerdad,cVerdad);
			printf("Jugador 2: (%d,%d) ",fVerdad,cVerdad);
			switch(comprobacion){
				case 0:
					fprintf(parchivo,"AGUA\n");
					printf("AGUA\n");
					*(tablero1+f*col+c)=-9;
					*(tableroBarcos2+f*col*c)=-9;
					break;
				case 1:
					fprintf(parchivo,"BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero1+f*col+c)=-1;
					*(tableroBarcos2+f*col+c)=-1;
					break;
				case 2:
					fprintf(parchivo,"BARCO 2 POSICIONES TOCADO\n");
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-2;
					*(tableroBarcos2+f*col+c)=-2;
					break;
				case 3:
					fprintf(parchivo,"BARCO 3 POSICIONES TOCADO\n");
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-3;
					*(tableroBarcos2+f*col+c)=-3;
					break;
				case 8:
					fprintf(parchivo,"DISPARO YA EFECTUADO \n");
					printf("DISPARO YA EFECTUADO \n");
					break;
			}
		

			if(compruebaGanador(tablero2,filas,col)==1){
				ganador=1;
				aux=1;
			}

			if(compruebaGanador(tablero1,filas,col)==1){
				ganador=1;
				aux=2;
			}
		}while(ganador == 0);


		if(aux==1){
			fprintf(parchivo, "HA GANADO EL JUGADOR 1\n");
			printf("HA GANADO EL JUGADOR 1\n");

			fprintf(parchivo, "\nTABLERO JUGADOR 2 DISPAROS RECIBIDOS POR JUGADOR 1\n");
			printf("\nTABLERO JUGADOR 2 DISPAROS RECIBIDOS POR JUGADOR 1\n");
			imprimirTableroArchivo(tableroBarcos1, filas, col, parchivo);
			imprimirTablero(tableroBarcos1, filas, col);

		}else{

			if(aux==2){
				fprintf(parchivo, "HA GANADO EL JUGADOR 2\n");
				printf("HA GANADO EL JUGADOR 2\n");

				fprintf(parchivo, "\nTABLERO JUGADOR 1 DISPAROS RECIBIDOS POR JUGADOR 2\n");
				printf("\nTABLERO JUGADOR 1 DISPAROS RECIBIDOS POR JUGADOR 2\n");
				imprimirTableroArchivo(tableroBarcos2, filas, col, parchivo);
				imprimirTablero(tableroBarcos2, filas, col);
			}
		}

	}else{

		printf("\nComienza el Jugador 2! \n\n");

		fprintf(parchivo, "\nTABLERO JUGADOR 2 \n");
		imprimirTableroArchivo(tablero2, filas, col, parchivo);
		fprintf(parchivo, "\nTABLERO JUGADOR 1 \n");
		imprimirTableroArchivo(tablero1, filas, col, parchivo);

		printf("\n          TABLERO JUGADOR 2 \n");
		imprimirTablero(tablero2, filas, col);
		printf("\n          TABLERO JUGADOR 1 \n");
		imprimirTablero(tablero1, filas, col);

		int f,c,aux,ganador=0,comprobacion=0;
		int count=0;
		int fVerdad,cVerdad;
		do{	
			f=rand()%filas;
			fVerdad=f+1;
			c=rand()%col;
			cVerdad=c+1;
			comprobacion=compruebaDisparo(tablero1,filas,col,f,c);
			fprintf(parchivo, "Jugador 2: (%d,%d) ",fVerdad,cVerdad);
			printf("Jugador 2: (%d,%d) ",fVerdad,cVerdad);
			switch(comprobacion){
				case 0:
					fprintf(parchivo,"AGUA\n");
					printf("AGUA\n");
					*(tablero1+f*col+c)=-9;
					*(tableroBarcos2+f*col*c)=-9;
					break;
				case 1:
					fprintf(parchivo,"BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero1+f*col+c)=-1;
					*(tableroBarcos2+f*col+c)=-1;
					break;
				case 2:
					fprintf(parchivo,"BARCO 2 POSICIONES TOCADO\n");
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-2;
					*(tableroBarcos2+f*col+c)=-2;
					break;
				case 3:
					fprintf(parchivo,"BARCO 3 POSICIONES TOCADO\n");
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero1+f*col+c)=-3;
					*(tableroBarcos2+f*col+c)=-3;
					break;
				case 8:
					fprintf(parchivo,"DISPARO YA EFECTUADO \n");
					printf("DISPARO YA EFECTUADO \n");
					break;
			}

			f=rand()%filas;
			fVerdad=f+1;
			c=rand()%col;
			cVerdad=c+1;
			comprobacion=compruebaDisparo(tablero2,filas,col,f,c);
			fprintf(parchivo, "Jugador 1: (%d,%d) ",fVerdad,cVerdad);
			printf("Jugador 1: (%d,%d) ",fVerdad,cVerdad);
			switch(comprobacion){
				case 0:
					fprintf(parchivo,"AGUA\n");
					printf("AGUA\n");
					*(tablero2+f*col+c)=-9;
					*(tableroBarcos1+f*col+c)=-9;
					break;
				case 1:
					fprintf(parchivo,"BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					printf("BARCO 1 POSICION TOCADO Y HUNDIDO\n");
					*(tablero2+f*col+c)=-1;
					*(tableroBarcos1+f*col+c)=-1;
					break;
				case 2:
					fprintf(parchivo,"BARCO 2 POSICIONES TOCADO\n");
					printf("BARCO 2 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-2;
					*(tableroBarcos1+f*col+c)=-2;
					break;
				case 3:
					fprintf(parchivo,"BARCO 3 POSICIONES TOCADO\n");
					printf("BARCO 3 POSICIONES TOCADO\n");
					*(tablero2+f*col+c)=-3;
					*(tableroBarcos1+f*col+c)=-3;
					break;
				case 8:
					fprintf(parchivo,"DISPARO YA EFECTUADO \n");
					printf("DISPARO YA EFECTUADO \n");
					break;
			}


			if(compruebaGanador(tablero2,filas,col)==1){
				ganador=1;
				aux=1;
			}

			if(compruebaGanador(tablero1,filas,col)==1){
				ganador=1;
				aux=2;
			}

		}while(ganador==0);

		if(aux==1){
			fprintf(parchivo, "HA GANADO EL JUGADOR 1\n");
			printf("HA GANADO EL JUGADOR 1\n");

			fprintf(parchivo, "\nTABLERO JUGADOR 2 DISPAROS RECIBIDOS POR JUGADOR 1\n");
			printf("\nTABLERO JUGADOR 2 DISPAROS RECIBIDOS POR JUGADOR 1\n");
			imprimirTableroArchivo(tableroBarcos1, filas, col, parchivo);
			imprimirTablero(tableroBarcos1, filas, col);


		}else{

			if(aux==2){
				fprintf(parchivo, "HA GANADO EL JUGADOR 2\n");
				printf("HA GANADO EL JUGADOR 2\n");

				fprintf(parchivo, "\nTABLERO JUGADOR 1 DISPAROS RECIBIDOS POR JUGADOR 2\n");
				printf("\nTABLERO JUGADOR 1 DISPAROS RECIBIDOS POR JUGADOR 2\n");
				imprimirTableroArchivo(tableroBarcos2, filas, col, parchivo);
				imprimirTablero(tableroBarcos2, filas, col);
			}
		}
	}
	

fclose(parchivo);

free(tablero1);
free(tablero2);
free(tableroBarcos1);
free(tableroBarcos2);
}


void inicializarTablero(int *t, int filas, int col){
//Función inicializarTablero
//INPUTS:
//	- puntero a un tablero (barcos, tiradas o lo que sea)
//	- filas
//	- columnas
//OUTPUS: nada
//Inicializa a 0 la tabla
	int i,j;
	for(i=0;i<filas;i++){
		for(j=0;j<col;j++){
			*(t+i*col+j)=0;
		}
	}
}



void imprimirTableroArchivo(int *t, int filas, int col, FILE *parchivo){
//Función imprimirTableroArchivo
//INPUTS:
//	- puntero a un tablero (barcos, tiradas o lo que sea)
//	- filas
//	- columnas
// 	- puntero a archivo
//OUTPUTs: nada
//Imprime en el archivo la matriz pasada
	int i,j;
	for (i=0; i<filas; i++){
		for(j=0; j<col; j++){
			fprintf(parchivo,"%d \t ",*(t+i*col+j));
		}
		fprintf(parchivo,"\n");
	}
}

void imprimirTablero(int *t, int filas, int col){
/*
	Funcion imprime tablero por terminal
*/
	int i,j;
	for (i=0; i<filas; i++){
		for(j=0; j<col; j++){
			printf("%d \t ",*(t+i*col+j));
		}
		printf("\n");
	}
}

int compruebaGanador (int *t, int filas, int col){
//Función compruebaGanador
//INPUTs:
//	- puntero a un tablero de barcos
//	- filas
//	- columnas
//OUTPUTS: nada
//Busca si quedan barcos por hundir. Si no, devuelve 0. 
int contador=0;
int ganador=0;
for(int i=0; i<(filas*col);i++){
	if(*(t+i)>0){
		contador++;
	}
}

if(contador==0){
	ganador=1;
}

return ganador;
}

int compruebaDisparo(int *t, int filas, int col, int posFila, int posCol){
//Función compruebaDisparo
//INPUTS:
//	- tablero de barcos
//	- filas
//	- columnas
//	- tiro (fila y columna)
//OUTPUTS: 0 si agua, 1 si hay un barco de 1, 2 si hay un barco de 2, 3 si hay un barco de 3
	int disparo=0;
	switch(*(t+posFila*col+posCol)){
		case 0:
			disparo=0;
			break;
		case 1:
			disparo=1;
			break;
		case 2:
			disparo=2;
			break;
		case 3:
			disparo=3;
			break;
		default:
			disparo=8;
			break;
	}

return disparo;
}

int comprobacionEspacioParaBarco(int *t, int filas, int col, int iniFila, int iniCol, int tamBarco, int orientacion){
//Función comprobacionEspacioParaBarco
//INPUTS:
//	- tablero de barcos
//	- filas
//	- columnas
//	- posición (fila)
//	- posición (columna)
//	- tamaño del barco (1, 2 ó 3)
//	- orientación (0 horizontal, 1 vertical)
//OUTPUTS: 1 si cabe, 0 si no cabe
//Comprueba si cabe un barco a partir de la posición indicada en la orientación indicada. Devuelve 0 si no cabe, 1 si sí.
int comprobar=0;
if(orientacion==0){
	switch(tamBarco){
		case 1:
			if(iniFila>=0 && iniCol>=0 && iniFila<filas && iniCol<col && (*(t+iniFila*col+iniCol)==0)){
				comprobar=1;
			}
			break;

		//Comprobamos espacio barcos tamanyo 2
		case 2: 
			if(iniFila>=0 && iniCol>=0 && iniFila<filas && iniCol<col-1 && (*(t+iniFila*col+iniCol)==0) && (*(t+iniFila*col+(iniCol+1))==0)){
				comprobar=1;
			}
			break;
		
		//Comprobamos espacio barcos tamanyo 3
		case 3:
			if(iniFila>=0 && iniCol>=0 && iniFila<filas && iniCol<col-2 &&  (*(t+iniFila*col+iniCol)==0) && (*(t+iniFila*col+(iniCol+1))==0) && (*(t+iniFila*col+(iniCol+2))==0)){
				comprobar=1;
			}
			break;
	}

}
else if(orientacion==1){
	switch(tamBarco){
		case 1:
			if(iniFila>=0 && iniCol>=0 && iniFila<filas && iniCol<col && (*(t+iniFila*col+iniCol)==0)){
				comprobar=1;
			}
			break;

		//Comprobamos espacio barcos tamanyo 2
		case 2: 
			if(iniFila>=0 && iniCol>=0 && iniFila<filas-1 && iniCol<col && (*(t+iniFila*col+iniCol)==0) && (*(t+(iniFila+1)*col+iniCol)==0)){
				comprobar=1;
			}
			break;
		
		//Comprobamos espacio barcos tamanyo 3
		case 3:
			if(iniFila>=0 && iniCol>=0 && iniFila<filas-2 && iniCol<col && (*(t+iniFila*col+iniCol)==0)  && (*(t+(iniFila+1)*col+iniCol)==0) && (*(t+(iniFila+2)*col+iniCol)==0)){
				comprobar=1;
			}
			break;
	}
}

return comprobar;
}

void colocarBarcosAutomaticamente(int *t, int filas, int col){
//Función colocarBarcosAutomaticamente
//INPUTS:
//	- Tablero de barcos
//	- filas
//	- columnas	
//OUTPUTS: nada
//Coloca de forma automatica 4 barcos de 1 posición, 2 de 2 posiciones y 1 de 3 posiciones en el tablero
	
	//generacion de barcos
	int filaAleatoria;
	int colAleatoria;
	int orientacion;
	int tambarco;

	//generar una posicion aleatoria fila-columna
	//la variable cabe recibe el valor devuelto por "comprobar" en la funcion comprobacionEspacioParaBarco
	//generamos 4 veces posicion x,y barcos 1 hueco

		int cabe;
		int contador=0; //4 barcos
		do{
			tambarco=1;
			orientacion=0;
			filaAleatoria=0+rand()%(filas);
			colAleatoria=0+rand()%(col);

				cabe=comprobacionEspacioParaBarco(t,filas,col,filaAleatoria,colAleatoria,tambarco,orientacion);
				if(cabe==1){
					*(t+filaAleatoria*col+colAleatoria)=1;
					contador++;
				}
		}while(contador<4);

	//generamos 2 veces posicion x,y barcos 2 huecos
	//la variable cabe1 recibe el valor devuelto por "comprobar" en la funcion comprobacionEspacioParaBarco
		int cabe1;
		int contador1=-1;	
		do{
			tambarco=2;
			filaAleatoria=rand()%(filas);
			colAleatoria=rand()%(col);
			orientacion=rand()%(2);

			cabe1=comprobacionEspacioParaBarco(t,filas,col,filaAleatoria,colAleatoria,tambarco,orientacion);
			if(cabe1==1){
				if(orientacion==1){
					for(int i=0;i<2;i++){
						for(int j=0;j<1;j++){
							*(t+(filaAleatoria+i)*col+(colAleatoria+j))=2;
							contador1++;
						}
					}
				}else{
					for(int i=0;i<1;i++){
						for(int j=0;j<2;j++){
							*(t+(filaAleatoria+i)*col+(colAleatoria+j))=2;
							contador1++;
						}
					}
				}
			}
		}while(contador1<2);

		

	//generamos 1 vez posicion x,y barcos 3 huecos
	//la variable cabe2 recibe el valor devuelto por "comprobar" en la funcion comprobacionEspacioParaBarco 
	
	int cabe2;
	int contador2=0;
	do{
		tambarco=3;
		filaAleatoria=rand()%(filas);
		colAleatoria=rand()%(col);
		orientacion=rand()%2;

		cabe2=comprobacionEspacioParaBarco(t,filas,col,filaAleatoria,colAleatoria,tambarco,orientacion);
			if(cabe2==1){
				if(orientacion==1){
					for(int i=0;i<3;i++){
						for(int j=0;j<1;j++){
							*(t+(filaAleatoria+i)*col+(colAleatoria+j))=3;
							contador2++;
						}
					}
				}else{ 
					for(int i=0;i<1;i++){
						for(int j=0;j<3;j++){
							*(t+(filaAleatoria+i)*col+(colAleatoria+j))=3;
							contador2++;
						}	
					}
				}
			}
	}while(contador2<1);
	
//NOTA: en un inicio use un do-while (en vez de un for) y un while (en vez de do-while)
	//por qué de la anterior forma no se ejecutaba y de esta si?
}


void colocarBarcosManualmente(int *t, int filas, int col){
//Función colocarBarcosManualmente
//INPUTS:
//	- Tablero de barcos
//	- filas
//	- columnas	
//OUTPUTS: nada
//Coloca de forma manual 4 barcos de 1 posición, 2 de 2 posiciones y 1 de 3 posiciones en el tablero
	int filaIntroducida;
	int colIntroducida;
	int orientacion;
	int tambarco;

	//generamos 4 veces posicion x,y barcos 1 hueco
		int cabe;
		int correcto;
		int contador=0;

		do{
			tambarco=1;
			orientacion=0;

			printf("\n	~~Barcos 1 posicion~~\n");
			printf("Coordenada X: ");
			scanf("%d",&filaIntroducida);
			printf("Coordenada Y: ");
			scanf("%d",&colIntroducida);
			
			if(filaIntroducida<1 || colIntroducida>col || filaIntroducida>filas || colIntroducida<1){
				printf("Coordenada generada incorrecta! Se sale del tablero! \n");
			}else{
				filaIntroducida--;
				colIntroducida--;

				if(*(t+filaIntroducida*col+colIntroducida)==0){	
					cabe=comprobacionEspacioParaBarco(t,filas,col,filaIntroducida,colIntroducida,tambarco,orientacion);
					if(cabe==1){
						*(t+filaIntroducida*col+colIntroducida)=1;
						contador++;
						printf("Num barcos introducidos: %d\n",contador);
					}
				}else{
					printf("Posicion ocupada! \n");
				}
			}

		}while(contador<4);

	//generamos 2 veces posicion x,y barcos 2 huecos
	//la variable cabe1 recibe el valor devuelto por "comprobar" en la funcion comprobacionEspacioParaBarco
		int cabe1;
		tambarco=2;
		//NOTA: ¿por qué no me funciona si contador1=0?
		int contador1=-1;	
		do{	
			int oriCorrect;	
			do{
				printf("\n	~~Barcos 2 posiciones~~\n");
				printf("Orientacion [0-Horizontal] [1-Vertical]: ");
				scanf("%d",&orientacion);
				if(orientacion!=0 && orientacion!=1){
					oriCorrect=0;
					printf("\nOrientacion incorrecta! Valores entre 0-1\n");
				}else{
					oriCorrect=1;
				}

			}while(oriCorrect==0);

				printf("Coordenada X: ");
				scanf("%d",&filaIntroducida);
				printf("Coordenada Y: ");
				scanf("%d",&colIntroducida);

			if(colIntroducida+1>col || filaIntroducida+1>filas){
				printf("El barco se sale del tablero!\n");
			}
			
			if(filaIntroducida<1 || colIntroducida>col || filaIntroducida>filas || colIntroducida<1){
				printf("Coordenada generada incorrecta!\n");
			}else{
				filaIntroducida--;
				colIntroducida--;

				cabe1=comprobacionEspacioParaBarco(t,filas,col,filaIntroducida,colIntroducida,tambarco,orientacion);
				if(*(t+filaIntroducida*col+colIntroducida)==0){
					if(cabe1==1){
						if(orientacion==1){
							for(int i=0;i<2;i++){
								for(int j=0;j<1;j++){
								*(t+(filaIntroducida+i)*col+(colIntroducida+j))=2;
								contador1++;
								}
							}
						}else{
							if(orientacion==0){
								for(int i=0;i<1;i++){
									for(int j=0;j<2;j++){
										*(t+(filaIntroducida+i)*col+(colIntroducida+j))=2;
										contador1++;
									}
								}
							}
						}
					}
				}else{
					printf("Posicion ya ocupada! \n");
				}
			}
				
		}while(contador1<2);

		

	//generamos 1 vez posicion x,y barcos 3 huecos
	//la variable count2 recibe el valor devuelto por "comprobar" en la funcion comprobacionEspacioParaBarco 
	
	int cabe2;
	tambarco=3;
	int contador2=0;
	do{
		int oriCorrect;	
		do{
			printf("\n	~~Barcos 3 posiciones~~\n");
			printf("Orientacion [0-Horizontal] [1-Vertical]: ");
			scanf("%d",&orientacion);
			if(orientacion!=0 && orientacion!=1){
				oriCorrect=0;
				printf("\nOrientación incorrecta! Valores entre 0-1\n");
			}else{
				oriCorrect=1;
			}

		}while(oriCorrect==0);

		printf("Coordenada X: ");
		scanf("%d",&filaIntroducida);
		printf("Coordenada Y: ");
		scanf("%d",&colIntroducida);

		if(colIntroducida+1>col || colIntroducida+2>col || filaIntroducida+1>filas || filaIntroducida+2>filas){
			printf("El barco se sale del tablero!\n");
		}

		if(filaIntroducida<1 || colIntroducida>col || filaIntroducida>filas || colIntroducida<1){
			printf("Coordenada generada incorrecta! \n");
		}else{
			filaIntroducida--;
			colIntroducida--;
		
				cabe2=comprobacionEspacioParaBarco(t,filas,col,filaIntroducida,colIntroducida,tambarco,orientacion);
				if(*(t+filaIntroducida*col+colIntroducida)==0){
					if(cabe2==1){
						if(orientacion==1){
							for(int i=0;i<3;i++){
								for(int j=0;j<1;j++){
									*(t+(filaIntroducida+i)*col+(colIntroducida+j))=3;
									contador2++;
								}
							}
						}else{ 
							for(int i=0;i<1;i++){
								for(int j=0;j<3;j++){
									*(t+(filaIntroducida+i)*col+(colIntroducida+j))=3;
									contador2++;
								}	
							}
						}
					}	
				}else{
					printf("Posición ya ocupada!\n");
				}
		}
	}while(contador2<1);
	
}