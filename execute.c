/* expression.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Executes the -print or -exec expressions (or both) on the specified path.
 * @param   path        Path to a file or directory
 * @param   settings    Settings structure.
 * @return  Whether or not the execution was successful.
 */
int	    execute(const char *path, const Settings *settings) {
    pid_t pid;
    int status;
    pid = fork();

    if(pid<0){
        perror("fork");
        return -1;
    }else if(pid==0){
        if(settings->print){
            printf("%s\n", path);
        }

        if(settings->exec_argc>0){
            char *args[settings->exec_argc + 2];
            args[0] = settings->exec_argv[0];

            for(int i=1; i<=settings->exec_argc;++i){
                args[i] = settings->exec_argv[i-1];
            }

            args[settings->exec_argc+1] = NULL;

            if(execvp(args[0], args)==-1){
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }

        exit(EXIT_SUCCESS);
    } else {
        if(waitpid(pid, &status, 0) == -1){
            perror("waitpid");
            return -1;
        }

        if(WIFEXITED(status)){
            return WIFEXITED(status);
        }else{
            return -1;
        }
    }

    return 0;
}

int main() {
    Settings s;
    s.access = 0; // No access filtering
    s.empty = false;
    s.name = NULL;
    s.path = NULL;
    s.type = 0; // No type filtering
    s.perm = 0; // No permission filtering
    s.newer = 0; // No newer filtering
    s.uid = 0; // No UID filtering
    s.gid = 0; // No GID filtering
    s.print = false; // Print the path
    s.exec_argc = 1; // Example command: echo hello world
    s.exec_argv = (char *[]){"echo", "hello", "world"};

    // Provide a sample path
    const char *samplePath = "/path/to/your/sample/file.txt";

    // Test the execute function
    int result = execute(samplePath, &s);

    if (result == -1) {
        printf("Error executing the command.\n");
    } else {
        printf("Execution successful. Exit status: %d\n", result);
    }

    return 0;
}