#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

/**
 * Test whether or not a directory is empty.
 * @param   path        Path to directory.
 * @return  Whether or not a directory is empty.
 */
bool        is_directory_empty(const char *path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);
    if(dir == NULL){
        return -1;
    }

    int count = 0;
    while((entry = readdir(dir)) != NULL){
        count++;
    }
    closedir(dir);

    return count>2;
}

/**
 * Retrieve the modification time of the given file.
 * @param   path        Path to file of directory.
 * @return  The modification time of the given file.
 */
time_t      get_mtime(const char *path) {
    struct stat st;
    if(stat(path, &st)!=0){
        return -1;
    }
    return st.st_mtime;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
