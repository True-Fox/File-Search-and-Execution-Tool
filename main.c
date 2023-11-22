/* driver.c */

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

/* Functions */

void	    usage(const char *program_name, int status) {
    fprintf(stderr, "Usage: %s PATH [OPTIONS] [EXPRESSION]\n", program_name);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "    -executable     File is executable or directory is searchable to user\n");
    fprintf(stderr, "    -readable       File readable to user\n");
    fprintf(stderr, "    -writable       File is writable to user\n\n");
    fprintf(stderr, "    -type [f|d]     File is of type f for regular file or d for directory\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -empty          File or directory is empty\n\n");
    fprintf(stderr, "    -name  pattern  Base of file name matches shell pattern\n");
    fprintf(stderr, "    -path  pattern  Path of file matches shell pattern\n\n");
    fprintf(stderr, "    -perm  mode     File's permission bits are exactly mode (octal)\n");
    fprintf(stderr, "    -newer file     File was modified more recently than file\n\n");
    fprintf(stderr, "    -uid   n        File's numeric user ID is n\n");
    fprintf(stderr, "    -gid   n        File's numeric group ID is n\n");
    fprintf(stderr, "\nExpressions:\n\n");
    fprintf(stderr, "    -print          Display file path (default)\n");
    fprintf(stderr, "    -exec cmd {} ;  Execute command on path\n");
    exit(status);
}

/* Main Execution */

int	    main(int argc, char *argv[]) {
    Settings settings = {
        .access = 0,
        .empty = false,
        .type = 0,
        .name = NULL,
        .path = NULL,
        .perm = 0,
        .newer = 0,
        .uid = -1,
        .gid = -1,
        .print = false,
        .exec_argc = 0,
        .exec_argv = NULL
    };

    for(int i=1;i<argc;i++){
        const char *arg = argv[i];

        if(arg[0] == '-'){
            if(strcmp(arg, "-executable")==0) settings.access |= X_OK;
            else if(strcmp(arg, "-readable")==0) settings.access |= R_OK;
            else if(strcmp(arg, "-writable")==0) settings.access |= W_OK;
            else if(strcmp(arg, "-type")==0){
                if(i+1 < argc){
                    i++;
                    if(strcmp(argv[i], "f")==0) settings.type = S_IFREG;
                    else if(strcmp(argv[i], "d")==0) settings.type = S_IFDIR;
                    else{
                        fprintf(stderr, "Invalid argument after -type\n");
                        exit(EXIT_FAILURE);
                    }
                }else{
                    fprintf(stderr, "Missing argument after -type\n");
                    exit(EXIT_FAILURE);
                }
            } else if(strcmp(arg, "-empty")==0) settings.empty=true;
            else if(strcmp(arg, "-name")==0){
                if(i+1 < argc){
                    i++;
                    settings.name = argv[i];
                }else{
                    fprintf(stderr, "Missing argument after -name\n");
                    exit(EXIT_FAILURE);    
                }
            }else if(strcmp(arg, "-path")==0){
                if(i+1 < argc){
                    i++;
                    settings.path = argv[i];
                }else{
                    fprintf(stderr, "Missing argument after -path\n");
                    exit(EXIT_FAILURE);
                }
            }else if( strcmp(arg, "-perm")==0){
                if(i+1 < argc){
                    settings.perm = strtol(argv[i], NULL, 8);
                }else{
                    fprintf(stderr, "Missing argument after -perm\n");
                    exit(EXIT_FAILURE);
                }
            }else if(strcmp(arg, "-newer")==0){
                if(i+1 < argc){
                    settings.newer = get_mtime(argv[i+1]);
                    i++;
                }else{
                    fprintf(stderr, "Missing argument after -newer\n");
                    exit(EXIT_FAILURE);
                }
            }else if(strcmp(arg, "-uid") == 0){
                if(i+1 < argc){
                    settings.uid = atoi(argv[i+1]);
                    i++;
                }else{
                    fprintf(stderr, "Missing argument after -uid\n");
                    exit(EXIT_FAILURE);    
                }
            }else if(strcmp(arg, "-gid")==0){
                if(i+1 < argc){
                    settings.gid = atoi(argv[i+1]);
                    i++;
                }else{
                    fprintf(stderr, "Missing argument after -gid\n");
                    exit(EXIT_FAILURE);
                }
            }else if(strcmp(arg, "-print")==0){
                settings.print = true;
            }else if(strcmp(arg, "-exec")==0){
                if(i+2 < argc){
                    settings.exec_argc = 2;
                    settings.exec_argv = argv+i;
                }else{
                    fprintf(stderr, "Missing command after -exec\n");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
