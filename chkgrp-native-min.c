/**
 * chkgrp-min-native is a program that checks if a user is a member of a group.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2025
 * Homepage: https://git.xw3.org/hanez/chkusr
 * License: Apache-2.0 (see LICENSE)
 */

#include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAME 256

int main(int argc, char *argv[])
{
    if (argc != 3)
        return 2;

    const char *username = argv[1];
    const char *groupname = argv[2];

    if (strlen(username) > MAX_NAME || strlen(groupname) > MAX_NAME)
        return 2;

    FILE *fp = fopen("/etc/group", "r");
    if (!fp)
        return 2;

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        char *grp = strtok(line, ":");
        if (!grp || strcmp(grp, groupname) != 0)
            continue;

        strtok(NULL, ":"); // skip password
        strtok(NULL, ":"); // skip GID

        char *members = strtok(NULL, ":\n");
        if (!members) break;

        char *m = strtok(members, ",");
        while (m) {
            if (!strcmp(m, username)) {
                puts("Yes");
                fclose(fp);
                return 0;
            }
            m = strtok(NULL, ",");
        }

        break; // group matched, no user found
    }

    fclose(fp);
    puts("No");
    return 1;
}

