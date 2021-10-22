#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




int main(int argc, char const *argv[])
{
	int flag, fd;  //filre descriptor
	char *nfifo= "tuberia01";
	char *frase= "La gente positiva cambia el mundo, la negativa lo mantiene como está";


	printf("Productor\n");	 

	
	flag=mkfifo(nfifo, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

	if (!flag){
		/*codigo*/
		printf("Productor espera a consumidor\n");
		fd = open(nfifo, O_WRONLY);
		if (fd==-1)
		{
			perror("Error al abrir la tuberia.");
		}else{
			write(fd,frase, strlen(frase));
			printf("productor envio %s\n", argv[1]);
			close(fd);
		}


	}else{
		perror("Error al crear tuberia \n");
	}

	return 0;
}