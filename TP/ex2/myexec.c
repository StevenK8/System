#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    if(argc == 1){
        printf("Veuillez entrer une commande à exécuter\n");
        return (EXIT_FAILURE);
    }else{
        char *cmd = argv[1];
        char *args[argc];
        int i;
        for (i=1; i<argc; i++){
            args[i-1] = argv[i];
        }
        args[i] = NULL;
        execvp(cmd, args);
    }
    
    return 0;
}
