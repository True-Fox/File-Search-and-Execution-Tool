#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>
#include <time.h>

typedef struct {
    char *path;
    char *name;
    mode_t perm;
    time_t newer;
    mode_t type;
} FindSettings;

void usage(const char *program_name, int status) {
    fprintf(stderr, "Usage: %s [OPTIONS]\n", program_name);
    fprintf(stderr, "Search for files with specified criteria.\n");
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, "    -name PATTERN   File name pattern to search for\n");
    fprintf(stderr, "    -type TYPE      File type to search for (f: regular file, d: directory)\n");
    fprintf(stderr, "    -path STRING    Path where the pattern should be searched for\n");
    fprintf(stderr, "    -h, --help      Display this help and exit\n");
    exit(status);
}

time_t get_mtime(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return -1;
    }
    return st.st_mtime;
}

void find(const char *root, const FindSettings *settings) {
     //printf("Searching in: %s\n", root); 
    if (settings->name != NULL && fnmatch(settings->name, root, 0) == 0) {
        printf("%s\n", root);
    }

    DIR *dir;
    struct dirent *entry;
    struct stat st;

    dir = opendir(root);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", root, entry->d_name);

        if (lstat(fullpath, &st) != 0) {
            perror("lstat");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            find(fullpath, settings);
        }

        const char *filename = strrchr(fullpath, '/');
        if (filename == NULL) {
            filename = fullpath;
        } else {
            filename++;
        }

        if ((settings->name == NULL || fnmatch(settings->name, filename, 0) == 0)
         && (settings->type == 0 || ((st.st_mode & S_IFMT) == settings->type))) {
            printf("%s\n", fullpath);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    FindSettings settings = {
        .path = ".",
        .name = NULL,
        .perm = 0,
        .newer = 0,
        .type = 0
    };

    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            usage(argv[0], EXIT_SUCCESS);
        }
        if (strcmp(arg, "-name") == 0) {
            if (i + 1 < argc) {
                i++;
                settings.name = argv[i];
            } else {
                fprintf(stderr, "Missing argument after -name\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(arg, "-type") == 0) {
            if (i + 1 < argc) {
                i++;
                if (strcmp(argv[i], "f") == 0) {
                    settings.type = S_IFREG;
                } else if (strcmp(argv[i], "d") == 0) {
                    settings.type = S_IFDIR;
                } else {
                    fprintf(stderr, "Invalid argument after -type\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Missing argument after -type\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(arg, "-perm") == 0) {
            if (i + 1 < argc) {
                i++;
                settings.perm = strtol(argv[i], NULL, 8);
            } else {
                fprintf(stderr, "Missing argument after -perm\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(arg, "-newer") == 0) {
            if (i + 1 < argc) {
                i++;
                settings.newer = get_mtime(argv[i]);
            } else {
                fprintf(stderr, "Missing argument after -newer\n");
                exit(EXIT_FAILURE);
            }
        } else if (strcmp(arg, "-path") == 0) {
            if (i + 1 < argc) {
                i++;
                settings.path = argv[i];
            } else {
                fprintf(stderr, "Missing argument after -path\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (argc == 1) {
        settings.type = 0;
        find(settings.path, &settings);
    } else if(argc>=2){
        find(settings.path, &settings);
    } else {
        usage(argv[0], EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
