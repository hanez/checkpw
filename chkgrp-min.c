/**
 * chkgrp-min is a program that checks if a user is a member of a group.
 *
 * Author: Johannes Findeisen <you@hanez.org> - 2025
 * Homepage: https://git.xw3.org/hanez/chkusr
 * License: Apache-2.0 (see LICENSE)
 */

 #include "version.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#define MAX_NAME 256

int main(int argc, char *argv[])
{
    if (argc != 3)
        return 2;

    if (strlen(argv[1]) > MAX_NAME || strlen(argv[2]) > MAX_NAME)
        return 2;

    struct passwd *pw = getpwnam(argv[1]);
    if (!pw)
        return 2;

    struct group *gr = getgrnam(argv[2]);
    if (!gr)
        return 2;

    if (pw->pw_gid == gr->gr_gid) {
        puts("Yes");
        return 0;
    }

    for (char **m = gr->gr_mem; *m; ++m) {
        if (!strcmp(*m, argv[1])) {
            puts("Yes");
            return 0;
        }
    }

    puts("No");
    return 1;
}

