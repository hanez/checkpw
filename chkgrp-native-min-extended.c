/**
 * chkgrp-min-native-extended is a program that checks if a user is a member
 * of a group.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2025
 * Homepage: https://git.xw3.org/hanez/chkusr
 * License: Apache-2.0 (see LICENSE)
 */

#include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

    // Step 1: Find user in /etc/passwd to get their primary GID
    FILE *passwd = fopen("/etc/passwd", "r");
    if (!passwd)
        return 2;

    char line[MAX_LINE];
    int user_gid = -1;
    while (fgets(line, sizeof(line), passwd)) {
        char *u = strtok(line, ":");
        if (!u || strcmp(u, username) != 0)
            continue;

        strtok(NULL, ":"); // skip password
        strtok(NULL, ":"); // skip UID
        char *gid_str = strtok(NULL, ":");
        if (!gid_str)
            break;

        user_gid = atoi(gid_str);
        break;
    }
    fclose(passwd);
    if (user_gid < 0)
        return 2;

    // Step 2: Find group in /etc/group and check membership or GID match
    FILE *group = fopen("/etc/group", "r");
    if (!group)
        return 2;

    int found = 0;
    while (fgets(line, sizeof(line), group)) {
        char *grp = strtok(line, ":");
        if (!grp || strcmp(grp, groupname) != 0)
            continue;

        strtok(NULL, ":"); // skip password
        char *gid_str = strtok(NULL, ":");
        if (!gid_str)
            break;

        int group_gid = atoi(gid_str);
        if (group_gid == user_gid) {
            puts("Yes");
            fclose(group);
            return 0;
        }

        char *members = strtok(NULL, ":\n");
        if (!members)
            break;

        char *m = strtok(members, ",");
        while (m) {
            if (!strcmp(m, username)) {
                puts("Yes");
                fclose(group);
                return 0;
            }
            m = strtok(NULL, ",");
        }

        found = 1;
        break;
    }

    fclose(group);
    if (!found)
        return 2;

    puts("No");
    return 1;
}

