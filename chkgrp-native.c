/**
 * chkgrp-native is a program that checks if a user is a member of a group.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2025
 * Homepage: https://git.xw3.org/hanez/chkgrp
 * License: Apache-2.0 (see LICENSE)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_NAME 256

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <username> <groupname>\n", argv[0]);
        return 2;
    }

    const char *username = argv[1];
    const char *groupname = argv[2];

    if (strlen(username) > MAX_NAME || strlen(groupname) > MAX_NAME) {
        fprintf(stderr, "Error: Name too long (max %d characters).\n", MAX_NAME);
        return 2;
    }

    FILE *fp = fopen("/etc/group", "r");
    if (!fp) {
        perror("Error opening /etc/group");
        return 2;
    }

    char line[MAX_LINE];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *grp_name = strtok(line, ":");
        if (!grp_name) continue;

        strtok(NULL, ":"); // skip password
        strtok(NULL, ":"); // skip GID

        char *members = strtok(NULL, ":\n");
        if (!members) continue;

        if (strcmp(grp_name, groupname) != 0)
            continue;

        // Group found, now check for user in member list
        found = 1;
        char *member = strtok(members, ",");
        while (member) {
            if (strcmp(member, username) == 0) {
                fclose(fp);
                puts("Yes");
                return 0;
            }
            member = strtok(NULL, ",");
        }

        break; // no need to continue scanning
    }

    fclose(fp);

    if (!found) {
        fprintf(stderr, "Group '%s' not found.\n", groupname);
        return 2;
    }

    puts("No");
    return 1;
}

