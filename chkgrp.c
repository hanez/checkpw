/**
 * chkgrp is a program that checks if a user is a member of a group.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2025
 * Homepage: https://git.xw3.org/hanez/chkgrp
 * License: Apache-2.0 (see LICENSE)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#ifndef LOGIN_NAME_MAX
#define LOGIN_NAME_MAX 256
#endif

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <username> <groupname>\n", argv[0]);
        return 2;
    }

    const char *username = argv[1];
    const char *groupname = argv[2];

    // Runtime check for max name length
    long max_name_len = sysconf(_SC_LOGIN_NAME_MAX);
    if (max_name_len <= 0 || max_name_len > 1024) {
        max_name_len = LOGIN_NAME_MAX;
    }

    if (strlen(username) > (size_t)max_name_len) {
        fprintf(stderr, "Error: Username too long (max %ld characters).\n",
                max_name_len);
        return 2;
    }

    if (strlen(groupname) > (size_t)max_name_len) {
        fprintf(stderr, "Error: Group name too long (max %ld characters).\n",
                max_name_len);
        return 2;
    }

    struct passwd *pw = getpwnam(username);
    if (!pw) {
        fprintf(stderr, "Error: User '%s' not found.\n", username);
        return 2;
    }

    struct group *gr = getgrnam(groupname);
    if (!gr) {
        fprintf(stderr, "Error: Group '%s' not found.\n", groupname);
        return 2;
    }

    if (pw->pw_gid == gr->gr_gid) {
        puts("Yes");
        return 0;
    }

    for (char **members = gr->gr_mem; *members != NULL; members++) {
        if (strcmp(*members, username) == 0) {
            puts("Yes");
            return 0;
        }
    }

    puts("No");
    return 1;
}

