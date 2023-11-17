/* search.c */

#include "search.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <dirent.h>

/**
 * Recursively search through the provided root directory
 * @param   root        Path to directory
 * @param   settings    Settings structure
 * @return  Whether or not the search was successful.
 */
int search(const char *root, const Settings *settings) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(root);
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", root, entry->d_name);

        if (!filter(fullpath, settings)) {
            execute(fullpath, settings);

            if (settings->type == 0 && entry->d_type == DT_DIR) {
                search(fullpath, settings);
            }
        }
    }

    closedir(dir);
    return 0;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
