#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t create_process(void)
{
    pid_t pid;

    do {
	pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));

    return pid;
}


void affiche_nombre(int debut,int fin)
{
    while (debut <= fin){
        printf("%d ",debut);
        debut++;
    }
}

int main(int argc, char* argv[])
{
    int N;

    if(argc == 1 || atoi(argv[1])<=0){
        printf("Veuillez entrer un nombre N positif en paramètre : « ./exo1 100000 »\n");
        return (EXIT_FAILURE);
    }else{
        N = atoi(argv[1]);
    }

    pid_t pid1 = create_process();

    switch (pid1) {
        case -1:
            perror("fork");
            return EXIT_FAILURE;
            break;
        /* Si on est dans le fils */
        case 0:
            affiche_nombre(1,N/2);
            return 0;
    }

    wait(&pid1);
    pid_t pid2 = create_process();

    switch (pid2) {
        case -1:
            perror("fork");
            return EXIT_FAILURE;
            break;
        /* Si on est dans le fils */
        case 0:
            affiche_nombre((N/2)+1,N);
            return 0;
    }
    
    wait(&pid2);
    printf("\n");

    return EXIT_SUCCESS;
}

