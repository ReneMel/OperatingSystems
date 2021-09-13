#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  


struct numbers
{
   int num1;
   int num2;
};


void *operate(void* args){
   struct numbers *ndata= (struct numbers*) args;

   //operate
   int sum= ndata->num2 + ndata->num1;
   int mult= ndata->num2 * ndata->num1;
   int rest= ndata->num2 - ndata->num1;

   //file
   FILE *file;

   file = fopen("result.txt", "w");

   if (file==NULL)
   {
      printf("Error creando file de texto");
   }
   
   fprintf(file, "El valor de la suma: %d\n", sum);
   fprintf(file, "El valor de la multiplicación: %d\n", mult);
   fprintf(file, "El valor de la resta: %d\n", rest);

   fclose(file);
   printf("Archivo creado con exito\n");

   free(args);
   pthread_exit(NULL);
}


int main()
{
   pthread_t th;

   struct numbers *args= calloc(sizeof(struct numbers),1);

   args->num1=3;
   args->num2=5;


   if(pthread_create(&th, NULL, &operate, args)){
     printf("Error creando el hilo.\n");
     abort();
   }

   printf("Espera union de hilos (join)\n");

   if(pthread_join(th, NULL)){
     printf("Error uniendo los hilos.\n");
     abort();
   }



   return 0;
}