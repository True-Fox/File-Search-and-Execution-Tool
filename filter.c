#include "search.h"

#include <dirent.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fnmatch.h>
#include <string.h>

/**
 * Test whether or not the given path should be filtered out of the output.
 * @param   path        Path to file or directory.
 * @param   settings    Settings structure.
 * @return  Whether or not the path should be excluded from the output (true to
 * exclude, false to include).
 */


bool filter(const char *path, const Settings *settings) {
    struct stat st;

    if (lstat(path, &st) != 0) {
        perror("lstat");
        return true;
    }

    if (S_ISREG(st.st_mode)) {
        if (settings->empty && st.st_size == 0) {
            return true;
        }
    }
    else if (S_ISDIR(st.st_mode)) {
        DIR *dir = opendir(path);
        if (dir != NULL) {
            struct dirent *entry;
            bool hasFiles = false;
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    hasFiles = true;
                    break;
                }
            }
            closedir(dir);

            if (settings->empty && !hasFiles) {
                return true;
            }
        } else {
            perror("opendir");
            return true;
        }
    }

    if (settings->access & EXECUTABLE && access(path, X_OK) != 0) {
        return true;
    }

    if (settings->access & READABLE && access(path, R_OK) != 0) {
        return true;
    }

    if (settings->access & WRITABLE && access(path, W_OK) != 0) {
        return true;
    }

    if (settings->name != NULL) {
        if (fnmatch(settings->name, strrchr(path, '/') + 1, 0) != 0) {
            return true;
        }
    }

    if (settings->path != NULL) {
        if (fnmatch(settings->path, path, 0) != 0) {
            return true;
        }
    }

    if (settings->type != 0 && ((st.st_mode & S_IFMT) != settings->type)) {
        return true;
    }

    if (settings->perm != 0 && (st.st_mode & ALLPERMS) != settings->perm) {
        return true;
    }

    if (settings->newer != 0 && st.st_mtime <= settings->newer) {
        return true;
    }

    if (settings->uid != 0 && st.st_uid != settings->uid) {
        return true;
    }

    if (settings->gid != 0 && st.st_gid != settings->gid) {
        return true;
    }

    return false;
}

/*int main() {
    Settings s;
    s.access = READABLE | WRITABLE;
    s.empty = false;
    s.name = "*.txt";
    s.path = NULL;
    s.type = 0; 
    s.perm = 0; 
    s.newer = 0; 
    s.uid = 0; 
    s.gid = 0; 
    s.print = false;
    s.exec_argc = 0; 
    s.exec_argv = NULL;

    const char *samplePath = "/workspaces/a-search-tool/tests/1st level/2nd level/test.txt";

    if (filter(samplePath, &s)) {
        printf("The file should be excluded.\n");
    } else {
        printf("The file should not be excluded.\n");
    }

    return 0;
}*/