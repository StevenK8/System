#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
    if(argc == 1){
        printf("Veuillez entrer une commande à exécuter\n");
        return (EXIT_FAILURE);
    }else{
        char *args[argc];
        int i;

        for (i=2; i<argc; i++){
            args[i-2] = argv[i];
        }
        args[argc-2] = NULL;

        printf("Appending output of '");
        for (i=0; i<argc-2; i++){
            printf("%s ",args[i]);
        }
        printf("' in %s file.\n",argv[1]);
        
        int fdout = open(argv[1],O_WRONLY | O_CREAT, 0600);

        dup2(fdout, STDOUT_FILENO);
        close(fdout);
        execvp(args[0], args);
    }
    
    return 0;
}
