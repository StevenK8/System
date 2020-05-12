#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>


void addition(int *pipe){
    close(pipe[1]);
    char* a;
    char* b;
    char str[10];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],a,1);
        read(pipe[0],b,1);
        int sum = a[0]+b[0];
        sprintf(str, "%d", sum);
        //écrit sur la sortie standard
        printf("<<%d>>\n",sum);
        // write(STDOUT_FILENO, str, 1);
    }
}

void multiplication(int *pipe){
    close(pipe[1]);
    char* a;
    char* b;
    char str[10];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],a,10);
        read(pipe[0],b,10);
        printf("%s * %s = ",a,b);
        int mul = a[0]*b[0];
        sprintf(str, "%d", mul);
        //écrit sur la sortie standard
        printf("%d\n>\n",mul);
    }
}

void soustraction(int *pipe){
    close(pipe[1]);
    char* a;
    char* b;
    char str[10];

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],a,10);
        read(pipe[0],b,10);
        printf("%s - %s = ",a,b);
        int sub = a[0]-b[0];
        sprintf(str, "%d", sub);
        //écrit sur la sortie standard
        printf("%d\n>\n",sub);
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
        if ((pid[i] = fork()) == -1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pid[i] == 0){ //fils
            // printf("<< %d >>",i);
            if(i==0){
                addition(pipefds + i*2);
            }else if(i==1){
                multiplication(pipefds + i*2);
            }else{
                soustraction(pipefds + i*2);
            }
            exit(EXIT_FAILURE);
        }
    }

    char operation[50];
    char a [10], b[10];

    close(pipefds[0]);
    close((pipefds+2)[0]);
    close((pipefds+4)[0]);

    printf("Entrez un calcul (addition / multiplication / soustraction)\nex: \"addition 2 4\" \n\n> ");
    while (1){
        scanf("%s %c %c", &operation, &a, &b);
        // operation = "addition";
        // a = "2";
        // b = "4";
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
                kill(pid[i], SIGKILL);
            }
            exit(EXIT_SUCCESS);
        }
    }

    return 0;
}