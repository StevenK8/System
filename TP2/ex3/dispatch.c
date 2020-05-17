#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


void addition(int *pipe){
    close(pipe[1]);
    char ab [2][100];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],ab,sizeof(ab));
        printf("%s + %s = ",ab[0],ab[1]);

        int sum = atoi(ab[0])+atoi(ab[1]);
        //écrit sur la sortie standard
        printf("%d\n> ",sum);
    }
}

void multiplication(int *pipe){
    close(pipe[1]);
    char ab [2][100];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],ab,sizeof(ab));
        printf("%s * %s = ",ab[0],ab[1]);

        int mul = atoi(ab[0])*atoi(ab[1]);
        //écrit sur la sortie standard
        printf("%d\n> ",mul);
    }
}

void soustraction(int *pipe){
    close(pipe[1]);
    char ab [2][100];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],ab,sizeof(ab));
        printf("%s - %s = ",ab[0],ab[1]);

        int sub = atoi(ab[0])-atoi(ab[1]);
        //écrit sur la sortie standard
        printf("%d\n> ",sub);
    }
}

int main(void){
    pid_t pid[3];
    int pipefds[6];
    int i;
    
    for (i=0; i<3; i++){
        if (pipe(pipefds + i*2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        fflush(stdout);
        if ((pid[i] = fork()) == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pid[i] == 0){ //fils
            if(i==0){
                addition(pipefds + i*2);
            }else if(i==1){
                multiplication(pipefds + i*2);
            }else{
                soustraction(pipefds + i*2);
            }
            exit(EXIT_FAILURE);
        }
        close((pipefds+2*i)[0]);
    }

    char operation[50];
    char a [100], b[100];
    char ab[2][100];


    printf("Entrez un calcul (addition / multiplication / soustraction)\nex: \"addition 2 4\" \n\n> ");
    while (1){
        scanf("%s %s %s", &operation, &a, &b);

        memcpy(ab[0], a, sizeof(ab[0]));
        memcpy(ab[1], b, sizeof(ab[0]));
        if(strcmp(operation,"addition")==0){
            write(pipefds[1],ab,sizeof(ab));
        }else if(strcmp(operation,"multiplication")==0){
            write((pipefds+2)[1],ab,sizeof(ab));
        }else if (strcmp(operation,"soustraction")==0){
            write((pipefds+4)[1],ab,sizeof(ab));
        }else{
            for (i=0; i<3; i++){
                close((pipefds + i*2)[1]);
                kill(pid[i], SIGKILL);
            }
            exit(EXIT_SUCCESS);
            break;
        }
    }

    return 0;
}