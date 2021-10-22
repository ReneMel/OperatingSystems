/*************************************************************************
	Sistemas Operativos.
	Colas de mensajes. Battleship, jugador1 (bnp1)
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
		4 = Empate.
	
*************************************************************************/

#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

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
char winningmove(int i, int j);
int isdraw(void);
int endgame(int fil, int col);




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
	

	//indicar que jugador1 ha iniciado
	enviar.tipo = 1;
	enviar.m2_fil = 0;
	enviar.m2_col = 0;
	enviar.tipo_mensaje = 0;

	//longitud del mensaje
	tam = sizeof(mensaje) - sizeof(long);

	//inicializando la cola de mensajes
	id01 = msgget(LLAVE,0600|IPC_CREAT);

	//envia el mensaje al jugador2
	msgsnd(id01,&enviar,tam,0);

	//esperando a jugador2...
	printf("\nesperando a jugador 2...\n");
	msgrcv(id01,&recibir,tam,2,0); //recibe el mensaje de jugador2

	if(recibir.tipo_mensaje != 0){ //fallo confirmacion de jugador2
		printf("\nerror al iniciar, intente de nuevo....\n");
		msgctl(id01,IPC_RMID,0); //elimina la cola de mensajes
		return 0;
	}
	
	//muestra los tableros
	mostrarMatrices();
	

	//inicio del juego
	while(1){
		printf("\nIngrese las coordenadas de ataque:\n");
		printf("fila: ");
		scanf("%d",&fil);		
		printf("columna: ");
		scanf("%d",&col);	
		if(fil >= 0 && fil <3 && col >= 0 && col <3){
			if(tablero[fil][col] == 'X' || tablero[fil][col] == 'O'){
				printf("casilla ocupada!!!\n");
			}
			else{	
				tablero[fil][col]='X';
				//comprobar si el movimiento de J1 es ganador
				if(endgame(fil,col)==1){
					winner= isdraw()==1?'0':'X';		
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 2;
					msgsnd(id01,&enviar,tam,0);
					break;
				}

				mostrarMatrices();


				enviar.tipo = 1;
				enviar.m2_fil = fil;
				enviar.m2_col = col;
				enviar.tipo_mensaje = 1;
				msgsnd(id01,&enviar,tam,0);
		
				//esperando al jugador 2
				printf("\nesperando a jugador 2...\n");
				msgrcv(id01,&recibir,tam,0,0);

				tablero[recibir.m2_fil][recibir.m2_col]='O';
				printf("pos %d\n",recibir.m2_fil );
				mostrarMatrices();

				////comprobar si el movimiento de J2 es ganador
				if(endgame(recibir.m2_fil,recibir.m2_col)==1){
					winner= isdraw()==1?'1':'O';			
					enviar.m2_fil = fil;
					enviar.m2_col = col;
					enviar.tipo_mensaje = 3;
					msgsnd(id01,&enviar,tam,0);
					break;
				}
				
				
			} //fin del else
		}else{
			printf("¡la casilla (%d,%d) no existe!\n",fil, col);
		}
	} //fin del while

	mostrarMatrices();
  	if (winner == 'X' || winner == 'O') {
    	printf("El ganador es el Jugador %c!\n", winner=='X'? '1': '2');
  	} 

  	if (isdraw()==1)
  	{
		printf('is draw\n');
  		enviar.m2_fil = fil;
		enviar.m2_col = col;
		enviar.tipo_mensaje = 4;	
		msgsnd(id01,&enviar,tam,0);
  	}



}



int isdraw() {
  for(int i = 0; i < 3; ++i) {
    for(int j = 0; j < 3; ++j) {
      if (tablero[i][j] == 'Z') {
        // empty square, so game ain't over yet
        return 0;
      }
    }
  }
  // no empty squares, so it's a draw
  return 1;
}


char winningmove(int i, int j) {
  if (tablero[i][j] == tablero[i][(j+1)%3]
   && tablero[i][j] == tablero[i][(j+2)%3] &&tablero[i][j]!='Z')
  {
    // got a column
    return tablero[i][j];
  }
  else if (tablero[i][j] == tablero[(i+1)%3][j]
          && tablero[i][j] == tablero[(i+2)%3][j] &&tablero[i][j]!='Z')
  {
    // got a row
    return tablero[i][j];
  }
  else if (i == j && tablero[i][j] == tablero[(i+1)%3][(j+1)%3]
                  && tablero[i][j] == tablero[(i+2)%3][(j+2)%3] &&tablero[i][j]!='Z')
  {
    // got the forward diagonal
    return tablero[i][j];
  }
  else if (i+j == 2 && tablero[i][j] == tablero[(i+2)%3][(j+1)%3]
                    && tablero[i][j] == tablero[(i+1)%3][(j+2)%3] &&tablero[i][j]!='Z')
  {
    // got the reverse diagonal
    return tablero[i][j];
  }
  else {
    // got nothing
    return 0;
  }
}

int endgame(int fil, int col){

	if(isdraw()==1 || winningmove(fil, col)!=0){
		return 1;
	}

	return 0;
}



void mostrarMatrices(void){
	system("clear");
	printf("JUGADOR 1 - Tic Tac Toe\n");
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


