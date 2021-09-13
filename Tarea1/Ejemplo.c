#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void *getFactorial(void* arg){
   int value=*(int*)arg;
   //factorial
   for (int i = *(int*)arg-1; i >0 ; --i)
   {
      value=value*i;
   }

   free(arg);
   int* result= malloc(sizeof(int));
   *result=value;

   return (void*) result;
}

int main()
{
   pthread_t th;

   int num[];
   int* fResult;
   printf("Inserte un numero para buscar su factorial: "); scanf("%d", &num);

   int*a = malloc(sizeof(int));
   *a=num;  

   if(pthread_create(&th, NULL, &getFactorial, a)){
     printf("Error creando el hilo.\n");
     abort();
   }

   printf("Espera union de hilos (join)\n");

   if(pthread_join(th, (void**) &fResult)){
     printf("Error uniendo los hilos.\n");
     abort();
   }

   printf("El factorial de %d es %d \n", num, *fResult );


   return 0;
}