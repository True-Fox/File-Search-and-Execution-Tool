/* search.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>

/**
 * Recursively search through the provided root directory and list all directories
 * @param   root        Path to directory
 * @param   settings    Settings structure
 * @return  Whether or not the search was successful.
 */
int search(const char *root, const Settings *settings) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(root);
    if (dir == NULL) {
        return -1;
    }

    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char path[PATH_MAX];
                snprintf(path, sizeof(path), "%s/%s", root, entry->d_name);
                printf("%s\n", path);

                search(path, settings);
            }
        }
    }
    closedir(dir);
    return EXIT_SUCCESS;
}

int main() {
    Settings s;
    search("tests", &s);
    return 0;
}