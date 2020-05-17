// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 

#define MAX_TIME 20
#define MIN_TIME 5
#define TUNNEL_TIME 4

sem_t mutex;

struct arg_struct {
    int random_time;
    char* name;
};

void* train(void* arguments) 
{
    struct arg_struct *args = arguments;
    while (1){
        printf("\nLe %s fait un tour de %d secondes..\n", args -> name, args -> random_time);
        //Parcourt une distance pendant une durée aléatoire
        sleep(args -> random_time);

        printf("\nLe %s est devant le tunnel\n", args -> name);
        sem_wait(&mutex); 
        printf("\n\tLe %s entre dans le tunnel..\n", args -> name);

        //Section critique 
        sleep(TUNNEL_TIME); 
        
        //signal 
        sem_post(&mutex);
    }
} 


int main() 
{ 
	sem_init(&mutex, 0, 1);
	pthread_t RER_A,RER_B,RER_C,TrainDeBanlieue;

    srand(time(NULL));
    struct arg_struct argsA = {rand() % (MAX_TIME + 1 - MIN_TIME) + MIN_TIME, "RER A"};
    struct arg_struct argsB = {rand() % (MAX_TIME + 1 - MIN_TIME) + MIN_TIME, "RER B"};
    struct arg_struct argsC = {rand() % (MAX_TIME + 1 - MIN_TIME) + MIN_TIME, "RER C"};
    struct arg_struct argsT = {rand() % (MAX_TIME + 1 - MIN_TIME) + MIN_TIME, "Train de banlieue"};


    // Création des threads
	pthread_create(&RER_A,NULL,train,(void *)&argsA);
	pthread_create(&RER_B,NULL,train,(void *)&argsB);
    pthread_create(&RER_C,NULL,train,(void *)&argsC);
    pthread_create(&TrainDeBanlieue,NULL,train,(void *)&argsT);

	pthread_join(RER_A,NULL); 
	pthread_join(RER_B,NULL);
    pthread_join(RER_C,NULL); 
    pthread_join(TrainDeBanlieue,NULL);
	sem_destroy(&mutex); 
	return 0; 
} 
