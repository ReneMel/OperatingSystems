#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




int main()
{
	int bytesCounter;
	char mensaje[100], *nfifo="tuberia01";
	int fd;


	printf("consumidor...\n");

	fd=open(nfifo, O_RDONLY);
	if (fd==-1)
	{
		perror("Error al accesar a la pipe");
	}else {
		bytesCounter=read(fd,mensaje,100);
		mensaje[bytesCounter]='\0';
		printf("Mensaje recibido: %s\n", mensaje );
		close(fd);
	}

	unlink(nfifo);

}