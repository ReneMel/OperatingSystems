/*************************************************************************
	Sistemas Operativos.
	Colas de mensajes. Battleship, jugador2 (bnp2)
	Autor: Carlos Miguel López Loarca

	tipo:
		0 = ver "tipo_mensaje"
		1 = jugador 1 ha iniciado.
		2 = jugador 2 ha iniciado.
	tipo_mensaje:
		0 = esperar al otro jugador.
		1 = turno del otro jugador.
		2 = ganador jugador 1.
		3 = ganador jugador 2.
		4 = Empate
	
*************************************************************************/

#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define LLAVE 34856


typedef struct mensaje{
	long tipo;
	int m2_fil;
	int m2_col;
	int tipo_mensaje;
} mensaje;

char tablero[3][3];
int i, j;
int fil, col;
int cont = 0;
int bandera = 0;
char winner='\0';

void mostrarMatrices(void);


int main(){
	//variables necesarias
	int id01;
	int tam;

	//estructuras de mensajes
	mensaje enviar;
	mensaje recibir;

	//inicializando matrices
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			tablero[i][j]='Z';
		}
	}

	//muestra los tableros vacios
	mostrarMatrices();
	


	//longitud del mensaje
	tam = sizeof(mensaje) - sizeof(long);

	//inicializando la cola de mensajes
	id01 = msgget(LLAVE,0600|IPC_CREAT);
	
	//esperando a jugador1...
	printf("\nesperando a jugador 1...\n");
	msgrcv(id01,&recibir,tam,1,0); //recibe el mensaje de jugador1
	if(recibir.tipo_mensaje != 0){ //fallo confirmacion de jugador1
		printf("\nerror al iniciar, intente de nuevo....\n");
		msgctl(id01,IPC_RMID,0); //elimina la cola de mensajes
		return 0;
	}

	//indicar que player2 ha iniciado
	enviar.tipo = 2;
	enviar.m2_fil = 0;
	enviar.m2_col = 0;
	enviar.tipo_mensaje = 0;

	//envia el mensaje al jugador1
	msgsnd(id01,&enviar,tam,0);
	
	//muestra los tableros
	mostrarMatrices();	

	//esperando al jugador 1
	printf("\nturno de jugador 1...\n");
	msgrcv(id01,&recibir,tam,0,0);
	//verifica si hundio un barco del jugador2
	if(tablero[recibir.m2_fil][recibir.m2_col] == 'Z'){
		tablero[recibir.m2_fil][recibir.m2_col]='X';
	}

	mostrarMatrices();
		
	while(1){
		printf("\nIngrese las coordenadas:\n");
		printf("fila: ");
		scanf("%d",&fil);		
		printf("columna: ");
		scanf("%d",&col);	
		if(fil >= 0 && fil <3 && col >= 0 && col <3){
			if(tablero[fil][col] == 'X' || tablero[fil][col] == 'O'){
				printf("casilla ocupada!!!\n");
			}
			else{	
				tablero[fil][col]='O';
				mostrarMatrices();
				printf("Tipo de mensaje: %d\n", recibir);



				//turno de jugador 2
				enviar.tipo = 1;
				enviar.m2_fil = fil;
				enviar.m2_col = col;
				enviar.tipo_mensaje = 1;
				msgsnd(id01,&enviar,tam,0);



			
				//esperando al jugador 1
				printf("\nturno de jugador 1...\n");
				msgrcv(id01,&recibir,tam,0,0);
				tablero[recibir.m2_fil][recibir.m2_col]='X';
				mostrarMatrices();
				printf("Tipo de mensaje: %d\n", recibir.tipo_mensaje);
				if(recibir.tipo_mensaje==2 || recibir.tipo_mensaje==3 ||recibir.tipo_mensaje==4 ){
					break;
				}
				

			}
		}
		else{
			printf("¡la casilla (%d,%d) no existe!\n",fil, col);
		}
	}
	if(recibir.tipo_mensaje!=4){
		printf("El ganador es el %s \n", recibir.tipo_mensaje==2?"Jugador 1":"Jugador 2");
	}else{
		printf("Es empate");
	}
	
}



void mostrarMatrices(void){
	system("clear");
	printf("JUGADOR 2 - Tic Tac Toe\n");
	printf("---------------------------------------------\n");
	printf("   0   1   2\n");
	for(i=0; i<3; i++){
		//Mi tablero
		printf("%d ", i);
		printf("| ");
		for(j=0; j<3; j++){
			if(tablero[i][j]=='Z'){
				printf("  | ");
			}
			else{
				printf("%c | ",tablero[i][j]);
			}
		}
		printf("\n");
	}
}
