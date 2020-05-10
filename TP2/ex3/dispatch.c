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

    printf("Addition");

    while(1){
        //lit l'entrée standard en boucle
        read(pipe[0],a,1);
        read(pipe[0],b,1);
        int sum = a[0]+b[0];
        sprintf(str, "%d", sum);
        //écrit sur la sortie standard
        write(STDOUT_FILENO, str, 1);
    }
}

// void multiplication(){
//     //lit l'entrée standard en boucle
//     int mul = a*b;
//     //écrit sur la sortie standard
// }

// void soustraction(){
//     //lit l'entrée standard en boucle
//     int sub = a-b;
//     //écrit sur la sortie standard
// }

int main(void){
    pid_t pid[3];
    int pipefds[6];
    int i;
    
    for (i=0; i<3; i++){
        if (pipe(pipefds + i*2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        pid[i] = fork();
        switch (pid[i]){
            case 0: //fils
                if(i==0){
                    addition(pipefds + i*2);
                }else if(i==1){
                    // multiplication(pipefd);
                }else{
                    // soustraction(pipefd);
                }
                break;       
            
            default: //père
                if (i==2){
                    char* operation;
                    char *a,*b;

                    close(pipefds[0]);
                    close((pipefds+2)[0]);
                    close((pipefds+4)[0]);

                    while (1){              
                        printf("Entrez un calcul (addition / multiplication / soustraction)\nex: \"addition 2 4\" \n\n> ");
                        scanf("%s %c %c", operation, a, b);
                        if(strcmp(operation,"addition")==0){
                            printf("Addition 1");
                            write(pipefds[1],a,strlen(a)+1);
                            write(pipefds[1],b,strlen(b)+1);
                        }else if(strcmp(operation,"multiplication")==0){
                            write((pipefds+2)[1],a,strlen(a)+1);
                            write((pipefds+2)[1],b,strlen(b)+1);
                        }else if (strcmp(operation,"soustraction")==0){
                            write((pipefds+4)[1],a,strlen(a)+1);
                            write((pipefds+4)[1],b,strlen(b)+1);
                        }
                    }
                    break;
                }
        }
    }

    return 0;
}