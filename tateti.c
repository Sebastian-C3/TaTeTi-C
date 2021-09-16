#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*
	Requisitos
		+ Mostrar info del juego al principio
		+ Mostrar trablero
		+ Juego es por turnos
			+ verifique el movimiento
			+ Diferenciar Jugadores
			+ Informacion actual de cada turno
		+ Recibir nombres
		- Verificar si alguno logro 3 en linea o empate
			- Informar situación final
				- Gano X o O
				- Empate
		+ Comienza inicialmente X o O
		+ Crear un bot como segundo jugador (el jugador 1 es el usuario)
*/

//nombre del juego
#define NOMBRE_JUEGO "Ta Te Ti"

#define MAX_NOMBRE 100

#define NOMBRE_DEF_JUG_A "def nombre A"
#define NOMBRE_DEF_JUG_B "def nombre B"
#define NOMBRE_DEF_JUG_ACT "def nombre jugador actual"

/*Lo ideal seria utilizar lectura de archivos*/
#define INFO_INICIAL_JUEGO "Info del Juego:\n\tJuego entre dos jugadores: X y O, que marcan los espacios de un tablero de 9 casillas alternadamente.\n\tCada jugador solo debe colocar su simbolo una vez por turno y no debe ser sobre una casilla ya jugada.\n\tSe debe conseguir realizar una linea recta o diagonal por simbolo.\n\n"

#define MAX_FILA 3
#define MAX_COLUM 3
#define MAX_TURNOS MAX_FILA*MAX_COLUM

const int FILA_DEF = -1;
const int COL_DEF = -1;

const char SIMB_JUGADOR_A = 'X';
const char SIMB_JUGADOR_B = 'O';
#define SIMB_VACIO = '';
const char SIMB_JUGADOR_INICIAL_DEF = SIMB_JUGADOR_A;

const int MOVIMIENTO_INVALIDO = 0;
const int MOVIMIENTO_VALIDO = 1;

/*
	Descripcion: recibe los nombres de los jugadores
	Pre: "nombreJugadorA" y "nombreJugadorB" correctamente definidos y asignados
	Pist: asigna los nuevos nombres a los jugadores
*/
void recibirNombresJugadores(char nombreJugadorA[MAX_NOMBRE], char nombreJugadorB[MAX_NOMBRE]){
	printf("Ingrese nombre de jugador A\n");
	scanf(" %s", nombreJugadorA);
	printf("Ingrese nombre de jugador B\n");
	scanf(" %s", nombreJugadorB);
	printf("nombres asignados...\n");
}

/*
	Descripcion: Muestra la presentacion del juego
	Pre: "nombreJugadorA" y "nombreJugadorB" correctamente definidos y asignados con los nombres de los jugadores actuales
	Post: Muestra el "NOMBRE_JUEGO", "INFO_INICIAL_JUEGO" y los nombres de los jugadores
*/
void presentarJuego(char nombreJugadorA[MAX_NOMBRE], char nombreJugadorB[MAX_NOMBRE]){
	printf("\n\t\t*** %s ***\n\n", NOMBRE_JUEGO);
	//info del juego - INFO_INICIAL_JUEGO (recomendado que lo levante de un archivo)
	printf("%s\n", INFO_INICIAL_JUEGO);
	printf("Nombre de juagador A: %s\n", nombreJugadorA);
	printf("Nombre de juagador B: %s\n\n", nombreJugadorB);
}

/*
	Descripcion: muestra el tablero actualizado
	Pre: "tablero" correctamente definido y asignado con valores actuales correspondientes
	Post: muestra el tablero con los valores actuales
*/
void mostrarTablero(char tablero[MAX_FILA][MAX_COLUM]){
	printf("\t   c0\t c1    c2\n");
	for (int fila = 0; fila < MAX_FILA; fila++)	{
		printf("\tf%i", fila);
		for (int col = 0; col < MAX_COLUM; col++){
			printf(" [%c]  ", tablero[fila][col]);
		}
		printf("\n\n");
	}
}

/*
	Descripcion: solicitamos el movimiento del usuario
	Pre: "fila" y "col" correctamente definidos y asignados
	Post: recibe nuevos valores para "fila" y "col"
*/
void recibirMovimientoUsuario(int *fila, int *col){
	printf("Ingrese fila\n");
	scanf(" %i", fila);
	printf("Ingrese columna\n");
	scanf(" %i", col);
}

/*
	Descripcion: recibimos el movimiento del bot
	Pre: 
	Post: 
*/
void recibirMovimientoBot(int *fila, int *col){
	//generar un numero aleatorio para fila y columna
	//rand()
	*fila = rand() % MAX_FILA; // numero aleatorio entre 0 y 2
	*col = rand() % MAX_COLUM;
}

/*
	Descripcion: verifica si el moviemiento es valido
				 Es valido si:
				 	- La posicion esta dentro del "tablero"
				 	- La posicion contiene un valor "SIMB_VACIO"
	Pre: "fila" y "col" corresponden al movimiento ingresado por el jugador
		 "tablero" correctamente defido y actualizado
	Post: Verifica si el movimiento es valido, si lo es devuelvo el valor de "MOVIMIENTO_VALIDO", caso contrario devuelvo "MOVIMIENTO_INVALIDO"
*/
int verificarMovimiento(int *fila, int *col, char tablero[MAX_FILA][MAX_COLUM]){
	int validacionMovimiento = MOVIMIENTO_INVALIDO;
	if((*fila >= 0 && *fila < MAX_FILA) && (*col >= 0 && *col < MAX_COLUM)){  //verifico que esta dentro de la matriz
		if(tablero[*fila][*col] == 0){ //verifico si esta libre el lugar
			validacionMovimiento = MOVIMIENTO_VALIDO;
		}
	}
	return validacionMovimiento;
}

/*
	Descripcion: solicitamos el movimiento del jugador actual
	Pre: "fila" y "col" punteros correctamente asignados
	Post: solicita y asigna el movimiento del jugado en "fila" y "col"
*/
void recibirMovimiento(int *fila, int *col, char simbActual, char tablero[MAX_FILA][MAX_COLUM]){
	do{	
		if (simbActual == SIMB_JUGADOR_A){
			recibirMovimientoUsuario(fila, col);//no se usa el & porq fila y col ya tienen la direccion de memoria
		}else{
			recibirMovimientoBot(fila, col);
		}
	}while( !verificarMovimiento(fila, col, tablero) );	
}

/*
	Descripcion: cambia de simbolo al correspondiente del siguiente jugador
	Pre: "simbActual" debe corresponder al simbolo del jugador actual en el juego
	Post: cambia el "simbActual" al del siguiente jugador
*/
void alternarTurnos(char *simbActual){
	char simbolo = *simbActual;
	if(simbolo == SIMB_JUGADOR_A){
	*simbActual = SIMB_JUGADOR_B;
	}else{
	*simbActual = SIMB_JUGADOR_A;
	}
}

/*
	Descripcion: muestra la informacion del turno
	Pre: "turnosActual" y "simbActual" correctamente asignados
	Post: muestra mediante consola la informacion del turno actual
*/
void mostrarInfoTurno(int turnosActual, char simbActual, char nombreJugadorActual[MAX_NOMBRE]){
	printf("\n  Turno numero: %i\n", turnosActual+1);
	printf("  Jugador actual: %s [%c]\n\n",nombreJugadorActual, simbActual);
	
	
}
/*
	Descripcion: seleccionar el nombre del jugador actual
	Pre: "simbActual", "nombreJugadorActual", "nombreJugadorA" y "nombreJugadorB" correctamente asignados y definidos
	Post: copia el nombre del jugador actual en el turno actual a "nombreJugadorActual"
*/
void selecNombreJugadorAct(char nombreJugadorActual[MAX_NOMBRE], char nombreJugadorA[MAX_NOMBRE], char nombreJugadorB[MAX_NOMBRE], char simbActual){
	if (simbActual == SIMB_JUGADOR_A){
		strcpy(nombreJugadorActual, nombreJugadorA); //nombreJugadorActual = nombreJugadorA;
	}else{
		strcpy(nombreJugadorActual, nombreJugadorB);
	}

}

/*
	Descripcion: inicia el juego hasta que se obtiene un final
*/
void iniciarJuego(char tablero[MAX_FILA][MAX_COLUM], char nombreJugadorA[MAX_NOMBRE], char nombreJugadorB[MAX_NOMBRE]){ //tablero es un parametro
	char simbActual = SIMB_JUGADOR_INICIAL_DEF;
	int fila = FILA_DEF;
	int col = COL_DEF;
	char nombreJugadorActual[MAX_NOMBRE] = NOMBRE_DEF_JUG_ACT;

	for (int turnosActual = 0; turnosActual < MAX_TURNOS; turnosActual++){
		selecNombreJugadorAct(nombreJugadorActual, nombreJugadorA, nombreJugadorB, simbActual);
		mostrarInfoTurno(turnosActual, simbActual, nombreJugadorActual);
		
		mostrarTablero(tablero);
		recibirMovimiento(&fila, &col, simbActual, tablero);
		tablero[fila][col] = simbActual;//agrego el movimiento al tablero
		//verificar estado si gano o empate
		//verificaciones
			//movimiento realizado
		//implementar el bot
		alternarTurnos(&simbActual);
	}
}

int main(){

	srand(time(NULL));//alteramos la semilla para que el numero aleatorio sea mas aleatorio

	char tablero[MAX_FILA][MAX_COLUM] = {};
	
	//nombre de jugadores
	char nombreJugadorA[MAX_NOMBRE] = NOMBRE_DEF_JUG_A; 
	char nombreJugadorB[MAX_NOMBRE] = NOMBRE_DEF_JUG_B; 

	recibirNombresJugadores(nombreJugadorA, nombreJugadorB); 
	presentarJuego(nombreJugadorA, nombreJugadorB);
	//tablero
	iniciarJuego(tablero, nombreJugadorA, nombreJugadorB); //tablero se envia como argumento

	return 0;
}