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
			+ Informar situación final
				+ Gano X o O
				* Empate
			+ Verificar a partir del 5 turno inclusive
			-En caso de empate
				* deberia terminar
				*(extra) deberia comenzar otra ronda
				*(extra) deberia definirse al ganador con un juego de piedra papel o tijera
		+ Comienza inicialmente X o O
		- Informar errpres
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
const int TURNO_EMEPZAR_VERIF = 4;
const int MAX_VERIF_DIAG = 2;

const int FILA_DEF = -1;
const int COL_DEF = -1;

const char SIMB_JUGADOR_A = 'X';
const char SIMB_JUGADOR_B = 'O';
#define SIMB_VACIO = '';
const char SIMB_JUGADOR_INICIAL_DEF = SIMB_JUGADOR_A;

const int MOVIMIENTO_INVALIDO = 0;
const int MOVIMIENTO_VALIDO = 1;

const int NO_GANO = 0;
const int SI_GANO = 1;

#define SIN_ERROR 0
#define ERROR_TAMANIO 1
#define ERROR_CASILLA_OCUPADA 2

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
	Pre: "fila" y "col" correctamente definidos y asignados
	Post: se generan posiciones aleatorias para "fila" y "col"
*/
void recibirMovimientoBot(int *fila, int *col){
	//generar un numero aleatorio para fila y columna
	//rand()
	*fila = rand() % MAX_FILA; // numero aleatorio entre 0 y 2
	*col = rand() % MAX_COLUM;
}

/*
	Descripcion: informa el error detectado
	Pre: "tipoError" correctamente definido y actualizado
	Post: informa el "tipoError" detectado mediante consola 
*/
void informarError(int tipoError){
	printf("\nError: ");
	switch(tipoError){
		case ERROR_TAMANIO:
			printf("Posicion invalida, valores de 0 a 2\n\n");
			break;
		case ERROR_CASILLA_OCUPADA:
			printf("Casilla ocupada\n\n");
	}
}

/*
	Descripcion: verifica la casilla del movimiento a ingresar
	Pre: "tablero", "tipoError", fila y col correctamente definidos y actualizados
	Post: devuelve "MOVIMIENTO_INVALIDO" si la casiila esta ocupada, caso controario devuelve "MOVIMIENTO_VALIDO"
		  actualiza "tipoError" con error:"ERROR_CASILLA_OCUPADA" si se dqetecta error
*/
int verificarCasillaOcupada(char tablero[MAX_FILA][MAX_COLUM], int *tipoError, int fila, int col){
	int validacionMovimiento = MOVIMIENTO_INVALIDO;
	
	if(!tablero[fila][col]){ //verifico si esta libre el lugar, si es "0" entra al if
		validacionMovimiento = MOVIMIENTO_VALIDO;
	}else{
		*tipoError = ERROR_CASILLA_OCUPADA;
	}	
	
	return validacionMovimiento;
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
int verificarMovimiento(int fila, int col, char tablero[MAX_FILA][MAX_COLUM]){
	int validacionMovimiento = MOVIMIENTO_INVALIDO;
	int tipoError = SIN_ERROR;
	if((fila >= 0 && fila < MAX_FILA) && (col >= 0 && col < MAX_COLUM)){  //verifico que esta dentro de la matriz
		validacionMovimiento = verificarCasillaOcupada(tablero, &tipoError, fila, col);							   
	}else{
		tipoError = ERROR_TAMANIO;
	}
	if(tipoError){
		informarError(tipoError);
	}
	return validacionMovimiento;
}

/*
	Descripcion: solicitamos el movimiento del jugador actual
	Pre: "simbActual" y "tablero" punteros correctamente asignados
	Post: solicita y asigna el movimiento del jugado en "fila" y "col"
*/
void recibirMovimiento(int *fila, int *col, char simbActual, char tablero[MAX_FILA][MAX_COLUM]){
	do{	
		if (simbActual == SIMB_JUGADOR_A){
			recibirMovimientoUsuario(fila, col);//no se usa el & porq fila y col ya tienen la direccion de memoria
		}else{
			recibirMovimientoBot(fila, col);
		}
	}while( !verificarMovimiento(*fila, *col, tablero) );//podria decirse que cancelamos el pasaje por referencia	
}														 // y usamos un pasaje por valor

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
	Pre: "turnoActual" y "simbActual" correctamente asignados
	Post: muestra mediante consola la informacion del turno actual
*/
void mostrarInfoTurno(int turnoActual, char simbActual, char nombreJugadorActual[MAX_NOMBRE]){
	printf("\n  Turno numero: %i\n", turnoActual+1);
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
	Descripcion: verifica el 3 en lina horizontal
	Pre: "tablero" y "simbActual" correctamente definidos y actualizados
	Post: devolver "SI_GANO" si se logro un 3 en linea horizontal, caso contrario devuelve "NO_GANO"
*/
int verificacionHorizontal(char tablero[MAX_FILA][MAX_COLUM], char simbActual){
	int estadoGano = NO_GANO;
	for(int verfHor = 0; verfHor < MAX_FILA; verfHor++){
		printf("verifif hor\n");
		if(tablero[verfHor][0] == simbActual && tablero[verfHor][1] == simbActual && tablero[verfHor][2] == simbActual){
			estadoGano = SI_GANO;
			verfHor = MAX_FILA;
		}
	}
	return estadoGano;
}

/*
	Descripcion: verifica el 3 en lina vertical
	Pre: "tablero" y "simbActual" correctamente definidos y actualizados
	Post: devolver "SI_GANO" si se logro un 3 en linea vertical, caso contrario devuelve "NO_GANO"
*/
int verirficacionVertical(char tablero[MAX_FILA][MAX_COLUM], char simbActual){
	int estadoGano = NO_GANO;
	for(int verfVert = 0; verfVert < MAX_COLUM; verfVert++){
			printf("verif vert\n");
			if(tablero[0][verfVert] == simbActual && tablero[1][verfVert] == simbActual && tablero[2][verfVert] == simbActual){
				estadoGano = SI_GANO;
				verfVert = MAX_COLUM;
			}
		}
	return estadoGano;
}

/*
	Descripcion: verifica el 3 en lina diagonal
	Pre: "tablero" y "simbActual" correctamente definidos y actualizados
	Post: devolver "SI_GANO" si se logro un 3 en linea diagonal, caso contrario devuelve "NO_GANO"
*/
int verificacionDiagonal(char tablero[MAX_FILA][MAX_COLUM], char simbActual){
	int estadoGano = NO_GANO;
	for(int verfDiag = 0; verfDiag <= MAX_VERIF_DIAG; verfDiag+=2){//verfDiag = verfDiag +2;
			printf("verif diag\n");
			if(tablero[verfDiag][0] == simbActual && tablero[1][1] == simbActual && tablero[2-verfDiag][2] == simbActual){
				estadoGano = SI_GANO;
				verfDiag = MAX_VERIF_DIAG;
			}
		}
	return estadoGano;
}

/*
	Descripcion: verifica si algun jugador gano/realizo 3 en linea
	Pre: "tablero" y "simbActual" correctamente definidos y actualizados
		 "estadoGanoJuego" correctamente definido apunta a la varaiable que contiene el estado del juego
	Post: devuelve "SI_GANO" si se logra verificar un 3 en linea, caso contrario devuelve "NO_GANO"
		  actualiza "estadoGanoJuego" con el valor de "estadoGano"
	*/
int verificarGanoJugador(char tablero[MAX_FILA][MAX_COLUM], char simbActual, int *estadoGanoJuego){
	int estadoGano = NO_GANO;
	estadoGano = verificacionHorizontal(tablero, simbActual);
	if (!estadoGano){
		estadoGano = verirficacionVertical(tablero, simbActual);
	}
	if(!estadoGano){
		estadoGano = verificacionDiagonal(tablero, simbActual);
	}

	*estadoGanoJuego = estadoGano;

	return estadoGano;
}

/*
	Descripcion: informa la situacion final del juegi
	Pre: "nombreJugadorActual", "tablero" y "estadoGanoJuego" correctamente definidos y actualizados
	Post: informar la situacion final del juego
*/
void informarSituacionFinal(char nombreJugadorActual[MAX_NOMBRE], char tablero[MAX_FILA][MAX_COLUM], int estadoGanoJuego){
	if (estadoGanoJuego){
		printf("\nJugador %s GANO!! :D\n\n", nombreJugadorActual);
	}else{
		printf("\nEMPATE!! :(\n\n");
	}
	mostrarTablero(tablero);
	printf("\nGracias por jugar, vuelva prontos!!\n");
}

/*
	Descripcion: evaluamos la situacion del turno
	Pre: "turnoActual", "tablero", "simbActual", "estadoGanoJuego" correctamente definidos y actualizados
	Post: dependiendo de la situacion del turno actual:
		- alternar los turnos
		- verifica si algun jugador gano
		- actualiza el "turnoActual" si algun jugador gano
*/
void evaluarSituacionTurno(int *turnoActual, char tablero[MAX_FILA][MAX_COLUM], char *simbActual, int *estadoGanoJuego){
	if(*turnoActual >= TURNO_EMEPZAR_VERIF && verificarGanoJugador(tablero, *simbActual, estadoGanoJuego)){	
		*turnoActual = MAX_TURNOS;
	}else{
		alternarTurnos(simbActual);
	}
}

/*
	Descripcion: inicia el juego hasta que se obtiene un final
	Pre: tablero, nombreJugadorA, nombreJugadorB correctamente definidos y actualizado
	Post: iniciamos el juego implementando los requisitos establecidos
*/
void iniciarJuego(char tablero[MAX_FILA][MAX_COLUM], char nombreJugadorA[MAX_NOMBRE], char nombreJugadorB[MAX_NOMBRE]){ //tablero es un parametro
	char simbActual = SIMB_JUGADOR_INICIAL_DEF;
	int fila = FILA_DEF;
	int col = COL_DEF;
	char nombreJugadorActual[MAX_NOMBRE] = NOMBRE_DEF_JUG_ACT;
	int estadoGanoJuego = NO_GANO;
	
	for (int turnoActual = 0; turnoActual < MAX_TURNOS; turnoActual++){
		selecNombreJugadorAct(nombreJugadorActual, nombreJugadorA, nombreJugadorB, simbActual);
		mostrarInfoTurno(turnoActual, simbActual, nombreJugadorActual);
		mostrarTablero(tablero);
		recibirMovimiento(&fila, &col, simbActual, tablero);
		tablero[fila][col] = simbActual;//agrego el movimiento al tablero
		//verificar estado si gano o empate 
		evaluarSituacionTurno(&turnoActual, tablero, &simbActual, &estadoGanoJuego);
	}
	informarSituacionFinal(nombreJugadorActual, tablero, estadoGanoJuego);
}

int main(){

	srand(time(NULL));//alteramos la semilla para que el numero aleatorio sea mas aleatorio

	char tablero[MAX_FILA][MAX_COLUM] = {};						//
	char nombreJugadorA[MAX_NOMBRE] = NOMBRE_DEF_JUG_A; 		//
	char nombreJugadorB[MAX_NOMBRE] = NOMBRE_DEF_JUG_B; 		//crear un modulo "configuracionInicialJuego"
	recibirNombresJugadores(nombreJugadorA, nombreJugadorB); 	//
	presentarJuego(nombreJugadorA, nombreJugadorB);				//
	
	iniciarJuego(tablero, nombreJugadorA, nombreJugadorB); //tablero se envia como argumento

	return 0;
}