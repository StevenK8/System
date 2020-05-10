#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

pid_t create_process(void)
{
    pid_t pid;

    do {
	pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));

    return pid;
}


int execute_fils(char **args)
{
    int fdout = open("/dev/null",O_WRONLY);
    // Redirige la sortie standard vers /dev/null
    dup2(fdout, STDOUT_FILENO);
    // Redirige également la sortie d'erreur - "Le  programme ne  devra  rien afficherd’autre que «OK»ou «ERREUR». "
    dup2(fdout, STDERR_FILENO);
    close(fdout);
    return execvp(args[0],args);
}

int main(int argc, char* argv[])
{

    if(argc == 1){
        printf("Veuillez entrer une commande en paramètre : « ./ex2 ls -la »\n");
        return (EXIT_FAILURE);
    }

    char *args[argc];
    pid_t pidt;
    int i;

    for (i=1; i<argc; i++){
        args[i-1] = argv[i];
    }
    args[argc-1] = NULL;

    pidt = create_process();
    switch (pidt) {
        case -1:
            perror("fork");
            return EXIT_FAILURE;
            break;
        /* Si on est dans le fils */
        case 0:
            return execute_fils(args);
    }

    int status;
    int error=0;

    waitpid(pidt,&status, 0);
    if ( WIFEXITED(status) ) {
        int es = WEXITSTATUS(status);
        if(es!=0){
            error++;
        }
    }
    if(error==0){
        printf("OK\n");
        return EXIT_SUCCESS;
    }else{
        printf("ERREUR\n");
        return error;
    }
}

