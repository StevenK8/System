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
    char a [10];
    char b [10];
    char str[10];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],a,1);
        read(pipe[0],b,1);
        printf("%s + %s = ",a,b);

        int sum = a[0]+b[0];
        //écrit sur la sortie standard
        printf("%d\n> ",sum);
    }
}

void multiplication(int *pipe){
    close(pipe[1]);
    char a [10];
    char b [10];
    char str[10];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],a,10);
        printf("mult read a: %s\n",a);
        read(pipe[0],b,10);
        printf("mult read b: %s\n",b);

        printf("%s * %s = ",a,b);
        int mul = a[0]*b[0];
        //écrit sur la sortie standard
        printf("%d\n> ",mul);
    }
}

void soustraction(int *pipe){
    close(pipe[1]);
    char a [10];
    char b [10];
    char str[10];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],a,10);
        printf("sub read a: %s\n",a);
        read(pipe[0],b,10);
        printf("sub read b: %s\n",b);

        printf("%s - %s = ",a,b);
        int sub = a[0]-b[0];
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
    char a [10], b[10];


    printf("Entrez un calcul (addition / multiplication / soustraction)\nex: \"addition 2 4\" \n\n> ");
    while (1){
        scanf("%s %c %c", &operation, &a, &b);

        if(strcmp(operation,"addition")==0){
            write(pipefds[1],a,strlen(a)+1);
            write(pipefds[1],b,strlen(b)+1);
        }else if(strcmp(operation,"multiplication")==0){
            write((pipefds+2)[1],a,strlen(a)+1);
            write((pipefds+2)[1],b,strlen(b)+1);
        }else if (strcmp(operation,"soustraction")==0){
            write((pipefds+4)[1],a,strlen(a)+1);
            write((pipefds+4)[1],b,strlen(b)+1);
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