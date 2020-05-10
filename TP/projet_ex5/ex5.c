#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

pid_t create_process(void)
{
    pid_t pid;

    do {
	pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));

    return pid;
}


int compile(char *nomFichier)
{
    printf("Compilation de %s...\n",nomFichier);
    return execlp("gcc","gcc","-c", nomFichier, NULL);
}

int link(char **nomFichier, int num)
{
    int i;
    char* outputNames[num+3];
    outputNames[0] = "gcc";
    outputNames[1] = "-o";
    outputNames[2] = "main";
    outputNames[num+2] = NULL;

    for(i=1; i<num; i++){
        char *ptr = strtok(nomFichier[i], ".");
        char string[sizeof(ptr)+1];

        strcpy(string+strlen(string), ptr);
        strcpy(string+strlen(string), ".o");

        outputNames[i+2] = strdup (string);

        string[0] = '\0';
    }

    printf("Lien de");
    for(i=2; i<num+1; i++){
        printf(" %s", outputNames[i]);
    }
    printf("...\n");
    return execvp("gcc", outputNames);
}

int main(int argc, char* argv[])
{

    if(argc == 1){
        printf("Veuillez entrer des fichiers à compiler en paramètre : « ./exo5 test.c »\n");
        return (EXIT_FAILURE);
    }

    pid_t pid_tab[argc-1];
    int i;
    
    for(i=1; i<argc; i++){
        pid_tab[i] = create_process();
        switch (pid_tab[i]) {
            case -1:
                perror("fork");
                return EXIT_FAILURE;
                break;
            /* Si on est dans le fils */
            case 0:
                return compile(argv[i]);
        }
    }

    int status;
    int error=0;
    for(i=1; i<argc; i++){
        waitpid(pid_tab[i],&status, 0);
        if ( WIFEXITED(status) ) {
            int es = WEXITSTATUS(status);
            if(es!=0){
                printf("Exit status was %d\n", es);
                error++;
            }
        }
    }
    if(error==0){
        return link(argv,argc);
    }else{
        return error;
    }
}

